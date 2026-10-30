use crate::Result;
use serde::{Deserialize, Serialize};
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::path::Path;
use std::sync::{Arc, OnceLock};

#[derive(Clone, Debug, Serialize, Deserialize, PartialEq, Eq)]
pub struct Section {
    pub name: String,
    pub kind: u32,
    pub flags: u32,
    pub address: u32,
    pub offset: u32,
    pub size: u32,
    pub link: usize,
    pub info: usize,
    pub alignment: u32,
    pub entry_size: u32,
}

#[derive(Clone, Debug, Serialize, Deserialize, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Symbol {
    pub name: String,
    pub address: u32,
    pub size: u32,
    pub info: u8,
    pub section: usize,
}

impl Symbol {
    pub fn kind(&self) -> u8 {
        self.info & 15
    }
}

#[derive(Clone, Debug, Serialize, Deserialize, PartialEq, Eq)]
pub struct Relocation {
    pub section: usize,
    pub address: u32,
    pub kind: u32,
    pub symbol: usize,
    pub addend: i32,
}

#[derive(Clone, Debug)]
pub struct Elf {
    pub data: Arc<Vec<u8>>,
    pub kind: u16,
    pub sections: Vec<Section>,
    pub symbols: Vec<Symbol>,
    pub relocations: Vec<Relocation>,
    functions: Vec<usize>,
    names: HashMap<String, Vec<usize>>,
    addresses: HashMap<u32, Vec<usize>>,
    ranges: BTreeMap<usize, Vec<(u32, u64, usize)>>,
    relocation_index: BTreeMap<usize, Vec<usize>>,
    users: OnceLock<HashMap<Symbol, BTreeSet<usize>>>,
}

pub fn bytes(data: &[u8], offset: usize, size: usize) -> Result<&[u8]> {
    let end = offset.checked_add(size).ok_or("Binary range overflow")?;
    data.get(offset..end)
        .ok_or_else(|| "Truncated binary data".into())
}

pub fn u16be(data: &[u8], offset: usize) -> Result<u16> {
    Ok(u16::from_be_bytes(bytes(data, offset, 2)?.try_into()?))
}

pub fn u32be(data: &[u8], offset: usize) -> Result<u32> {
    Ok(u32::from_be_bytes(bytes(data, offset, 4)?.try_into()?))
}

pub fn string(data: &[u8], offset: usize) -> Result<String> {
    let rest = data
        .get(offset..)
        .ok_or("String offset exceeds its section")?;
    let end = rest
        .iter()
        .position(|&byte| byte == 0)
        .unwrap_or(rest.len());
    if !rest[..end].is_ascii() {
        return Err("ELF string is not ASCII".into());
    }
    Ok(std::str::from_utf8(&rest[..end])?.to_owned())
}

impl Elf {
    pub fn read(path: impl AsRef<Path>) -> Result<Self> {
        Self::parse(std::fs::read(path)?)
    }

    pub fn parse(data: Vec<u8>) -> Result<Self> {
        if data.get(..7) != Some(b"\x7fELF\x01\x02\x01") {
            return Err("Expected a big-endian ELF32 file.".into());
        }
        if u16be(&data, 18)? != 20 {
            return Err("Expected PowerPC machine code.".into());
        }
        let kind = u16be(&data, 16)?;
        let table = u32be(&data, 32)? as usize;
        let stride = u16be(&data, 46)? as usize;
        let count = u16be(&data, 48)? as usize;
        let names = u16be(&data, 50)? as usize;
        if stride < 40 || names >= count {
            return Err("Invalid ELF section table".into());
        }
        let entries: Vec<Vec<u32>> = (0..count)
            .map(|index| {
                (0..10)
                    .map(|field| u32be(&data, table + index * stride + field * 4))
                    .collect()
            })
            .collect::<Result<_>>()?;
        let strings = bytes(
            &data,
            entries[names][4] as usize,
            entries[names][5] as usize,
        )?;
        let sections = entries
            .iter()
            .map(|entry| {
                Ok(Section {
                    name: string(strings, entry[0] as usize)?,
                    kind: entry[1],
                    flags: entry[2],
                    address: entry[3],
                    offset: entry[4],
                    size: entry[5],
                    link: entry[6] as usize,
                    info: entry[7] as usize,
                    alignment: entry[8],
                    entry_size: entry[9],
                })
            })
            .collect::<Result<Vec<_>>>()?;
        let table = sections
            .iter()
            .find(|section| section.kind == 2)
            .ok_or("Missing ELF symbol table")?;
        let names = sections.get(table.link).ok_or("Invalid ELF string table")?;
        let strings = bytes(&data, names.offset as usize, names.size as usize)?;
        let symbols = bytes(&data, table.offset as usize, table.size as usize)?;
        if symbols.len() % 16 != 0 {
            return Err("Truncated ELF symbol table".into());
        }
        let symbols = symbols
            .as_chunks::<16>()
            .0
            .iter()
            .map(|entry| {
                Ok(Symbol {
                    name: string(strings, u32be(entry, 0)? as usize)?,
                    address: u32be(entry, 4)?,
                    size: u32be(entry, 8)?,
                    info: entry[12],
                    section: u16be(entry, 14)? as usize,
                })
            })
            .collect::<Result<Vec<_>>>()?;
        let mut relocations = Vec::new();
        for section in sections.iter().filter(|section| section.kind == 4) {
            let records = bytes(&data, section.offset as usize, section.size as usize)?;
            if records.len() % 12 != 0 {
                return Err("Truncated ELF relocation table".into());
            }
            for record in records.as_chunks::<12>().0 {
                let info = u32be(record, 4)?;
                if (info >> 8) as usize >= symbols.len() || section.info >= sections.len() {
                    return Err("Invalid ELF relocation index".into());
                }
                relocations.push(Relocation {
                    section: section.info,
                    address: u32be(record, 0)?,
                    kind: info & 255,
                    symbol: (info >> 8) as usize,
                    addend: u32be(record, 8)? as i32,
                });
            }
        }
        Ok(Self::indexed(
            Arc::new(data),
            kind,
            sections,
            symbols,
            relocations,
        ))
    }

    pub fn indexed(
        data: Arc<Vec<u8>>,
        kind: u16,
        sections: Vec<Section>,
        symbols: Vec<Symbol>,
        relocations: Vec<Relocation>,
    ) -> Self {
        let functions: Vec<_> = symbols
            .iter()
            .enumerate()
            .filter_map(|(index, symbol)| {
                (symbol.kind() == 2
                    && symbol.size != 0
                    && symbol.section != 0
                    && sections
                        .get(symbol.section)
                        .is_some_and(|section| section.flags & 4 != 0))
                .then_some(index)
            })
            .collect();
        let mut names: HashMap<String, Vec<usize>> = HashMap::new();
        let mut addresses: HashMap<u32, Vec<usize>> = HashMap::new();
        for (index, symbol) in symbols.iter().enumerate() {
            names.entry(symbol.name.clone()).or_default().push(index);
            addresses.entry(symbol.address).or_default().push(index);
        }
        let mut ranges: BTreeMap<usize, Vec<(u32, u64, usize)>> = BTreeMap::new();
        for &index in &functions {
            let symbol = &symbols[index];
            ranges
                .entry(symbol.section)
                .or_default()
                .push((symbol.address, 0, index));
        }
        for entries in ranges.values_mut() {
            entries.sort_by_key(|entry| entry.0);
            let mut end = 0;
            for entry in entries {
                end = end.max(u64::from(entry.0) + u64::from(symbols[entry.2].size));
                entry.1 = end;
            }
        }
        let mut relocation_index: BTreeMap<usize, Vec<usize>> = BTreeMap::new();
        for (index, relocation) in relocations.iter().enumerate() {
            relocation_index
                .entry(relocation.section)
                .or_default()
                .push(index);
        }
        for indices in relocation_index.values_mut() {
            indices.sort_by_key(|&index| relocations[index].address);
        }
        Self {
            data,
            kind,
            sections,
            symbols,
            relocations,
            functions,
            names,
            addresses,
            ranges,
            relocation_index,
            users: OnceLock::new(),
        }
    }

    pub fn reindex(&mut self) {
        *self = Self::indexed(
            self.data.clone(),
            self.kind,
            self.sections.clone(),
            self.symbols.clone(),
            self.relocations.clone(),
        );
    }

    pub fn section_data(&self, section: &Section) -> Result<&[u8]> {
        if section.kind == 8 {
            return Err("BSS has no file bytes.".into());
        }
        bytes(&self.data, section.offset as usize, section.size as usize)
    }

    pub fn symbol_data(&self, symbol: &Symbol) -> Result<&[u8]> {
        let section = self
            .sections
            .get(symbol.section)
            .ok_or("Invalid symbol section")?;
        let offset = if self.kind == 2 {
            symbol.address.checked_sub(section.address)
        } else {
            Some(symbol.address)
        };
        let offset =
            offset.ok_or_else(|| format!("Symbol exceeds its section: {}", symbol.name))?;
        if u64::from(offset) + u64::from(symbol.size) > u64::from(section.size) {
            return Err(format!("Symbol exceeds its section: {}", symbol.name).into());
        }
        bytes(
            self.section_data(section)?,
            offset as usize,
            symbol.size as usize,
        )
    }

    pub fn functions(&self) -> impl Iterator<Item = &Symbol> {
        self.functions.iter().map(|&index| &self.symbols[index])
    }

    pub fn named(&self, name: &str) -> Vec<&Symbol> {
        self.names
            .get(name)
            .into_iter()
            .flatten()
            .map(|&index| &self.symbols[index])
            .collect()
    }

    pub fn functions_named(&self, name: &str) -> Vec<&Symbol> {
        self.named(name)
            .into_iter()
            .filter(|symbol| {
                symbol.kind() == 2
                    && symbol.size != 0
                    && symbol.section != 0
                    && self
                        .sections
                        .get(symbol.section)
                        .is_some_and(|section| section.flags & 4 != 0)
            })
            .collect()
    }

    fn owner_indices(&self, section: usize, address: u32) -> Vec<usize> {
        let Some(ranges) = self.ranges.get(&section) else {
            return Vec::new();
        };
        let mut index = ranges.partition_point(|entry| entry.0 <= address);
        let mut owners = Vec::new();
        while index != 0 && ranges[index - 1].1 > u64::from(address) {
            index -= 1;
            let symbol = &self.symbols[ranges[index].2];
            if u64::from(address) < u64::from(symbol.address) + u64::from(symbol.size) {
                owners.push(ranges[index].2);
            }
        }
        owners.sort_unstable();
        owners
    }

    pub fn function_owners(&self, section: usize, address: u32) -> Vec<&Symbol> {
        self.owner_indices(section, address)
            .into_iter()
            .map(|index| &self.symbols[index])
            .collect()
    }

    pub fn function_users(&self, symbol: &Symbol) -> Vec<&Symbol> {
        let users = self.users.get_or_init(|| {
            let mut users: HashMap<Symbol, BTreeSet<usize>> = HashMap::new();
            for relocation in &self.relocations {
                users
                    .entry(self.symbols[relocation.symbol].clone())
                    .or_default()
                    .extend(self.owner_indices(relocation.section, relocation.address));
            }
            users
        });
        users
            .get(symbol)
            .into_iter()
            .flatten()
            .map(|&index| &self.symbols[index])
            .collect()
    }

    pub fn symbols_at(&self, address: u32) -> Vec<&Symbol> {
        self.addresses
            .get(&address)
            .into_iter()
            .flatten()
            .map(|&index| &self.symbols[index])
            .collect()
    }

    pub fn relocations_in(&self, section: usize, start: u32, end: u32) -> Vec<&Relocation> {
        let Some(indices) = self.relocation_index.get(&section) else {
            return Vec::new();
        };
        let first = indices.partition_point(|&index| self.relocations[index].address < start);
        let last = indices.partition_point(|&index| self.relocations[index].address < end);
        let mut indices = indices[first..last].to_vec();
        indices.sort_unstable();
        indices
            .into_iter()
            .map(|index| &self.relocations[index])
            .collect()
    }
}
