use crate::Result;
use std::path::Path;

pub const SHT_PROGBITS: u32 = 1;
pub const SHT_SYMTAB: u32 = 2;
pub const SHT_RELA: u32 = 4;
pub const SHT_NOBITS: u32 = 8;
pub const SHT_INIT_ARRAY: u32 = 14;
pub const SHT_GROUP: u32 = 17;
pub const SHF_ALLOC: u32 = 2;
pub const SHF_EXECINSTR: u32 = 4;
pub const SHN_ABS: usize = 0xfff1;
pub const SHN_COMMON: usize = 0xfff2;
pub const STT_SECTION: u8 = 3;
pub const STT_FILE: u8 = 4;
pub const STB_LOCAL: u8 = 0;
pub const STB_WEAK: u8 = 2;

#[derive(Clone, Debug)]
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
}

impl Section {
    pub fn is_alloc(&self) -> bool {
        self.flags & SHF_ALLOC != 0
    }
    pub fn is_code(&self) -> bool {
        self.flags & SHF_EXECINSTR != 0
    }
}

#[derive(Clone, Debug)]
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
    pub fn binding(&self) -> u8 {
        self.info >> 4
    }
    pub fn is_defined(&self) -> bool {
        self.section != 0
    }
}

#[derive(Clone, Debug)]
pub struct Relocation {
    pub section: usize,
    pub address: u32,
    pub kind: u32,
    pub symbol: usize,
    pub addend: i32,
}

#[derive(Clone, Debug)]
pub struct Elf {
    pub data: Vec<u8>,
    pub sections: Vec<Section>,
    pub symbols: Vec<Symbol>,
    pub relocations: Vec<Relocation>,
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

fn string(data: &[u8], offset: usize) -> Result<String> {
    let rest = data
        .get(offset..)
        .ok_or("String offset exceeds its section")?;
    let end = rest
        .iter()
        .position(|&byte| byte == 0)
        .unwrap_or(rest.len());
    Ok(String::from_utf8_lossy(&rest[..end]).into_owned())
}

impl Elf {
    pub fn read(path: impl AsRef<Path>) -> Result<Self> {
        let path = path.as_ref();
        Self::parse(std::fs::read(path)?).map_err(|e| format!("{}: {e}", path.display()).into())
    }

    pub fn parse(data: Vec<u8>) -> Result<Self> {
        if data.get(..7) != Some(b"\x7fELF\x01\x02\x01") {
            return Err("Expected a big-endian ELF32 file.".into());
        }
        if u16be(&data, 18)? != 20 {
            return Err("Expected PowerPC machine code.".into());
        }
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
                    alignment: entry[8].max(1),
                })
            })
            .collect::<Result<Vec<_>>>()?;
        let table = sections
            .iter()
            .find(|section| section.kind == SHT_SYMTAB)
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
        for section in sections.iter().filter(|section| section.kind == SHT_RELA) {
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
        Ok(Self {
            data,
            sections,
            symbols,
            relocations,
        })
    }

    pub fn section_data(&self, section: &Section) -> Result<&[u8]> {
        if section.kind == SHT_NOBITS {
            return Err("BSS has no file bytes.".into());
        }
        bytes(&self.data, section.offset as usize, section.size as usize)
    }

    pub fn group(&self, section: &Section) -> Result<(String, Vec<usize>)> {
        let words = self.section_data(section)?;
        let signature = self
            .symbols
            .get(section.info)
            .ok_or("Invalid ELF group signature")?
            .name
            .clone();
        let members = (4..words.len())
            .step_by(4)
            .map(|offset| u32be(words, offset).map(|index| index as usize))
            .collect::<Result<_>>()?;
        Ok((signature, members))
    }
}
