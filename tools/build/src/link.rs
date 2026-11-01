use crate::{
    Result,
    compile::Unit,
    dol::{self, Segment},
    elf::*,
    names,
};
use std::collections::{BTreeSet, HashMap};

const R_PPC_ADDR32: u32 = 1;
const R_PPC_ADDR16_LO: u32 = 4;
const R_PPC_ADDR16_HI: u32 = 5;
const R_PPC_ADDR16_HA: u32 = 6;
const R_PPC_REL24: u32 = 10;
const R_PPC_REL14: u32 = 11;
const R_PPC_PLTREL24: u32 = 18;
const R_PPC_LOCAL24PC: u32 = 23;
const R_PPC_REL32: u32 = 26;
const R_PPC_EMB_SDA21: u32 = 109;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Target {
    Clang {
        object: usize,
        section: usize,
        value: u32,
    },
    Original {
        function: usize,
        offset: u32,
    },
    Address(u32),
    ArenaLo,
}

#[derive(Clone, Copy, Debug)]
enum Implementation {
    Clang { object: usize, symbol: usize },
    Original,
}

struct Reference {
    elf: Elf,
    functions: Vec<usize>,
    group: Vec<Option<usize>>,
    globals: HashMap<String, usize>,
    locals: HashMap<String, Vec<usize>>,
    statics: HashMap<String, Vec<usize>>,
    code_relocations: Vec<usize>,
    text: (u32, u32),
}

impl Reference {
    fn new(elf: Elf) -> Result<Self> {
        let mut functions: Vec<usize> = elf
            .symbols
            .iter()
            .enumerate()
            .filter(|(_, s)| {
                s.kind() == 2
                    && s.size != 0
                    && elf.sections.get(s.section).is_some_and(Section::is_code)
            })
            .map(|(i, _)| i)
            .collect();
        functions.sort_by_key(|&i| elf.symbols[i].address);
        functions.dedup_by_key(|i| elf.symbols[*i].address);
        let mut group = vec![None; elf.symbols.len()];
        let mut current = None;
        let mut globals = HashMap::new();
        let mut locals: HashMap<String, Vec<usize>> = HashMap::new();
        let mut statics: HashMap<String, Vec<usize>> = HashMap::new();
        for (index, symbol) in elf.symbols.iter().enumerate() {
            if symbol.kind() == STT_FILE {
                current = Some(index);
                continue;
            }
            if symbol.binding() == STB_LOCAL {
                group[index] = current;
                if symbol.is_defined() && symbol.kind() != STT_SECTION && !symbol.name.is_empty() {
                    locals.entry(symbol.name.clone()).or_default().push(index);
                    if let Some((base, _)) = symbol.name.split_once('$') {
                        statics.entry(base.to_owned()).or_default().push(index);
                    }
                }
            } else if symbol.is_defined() {
                globals.entry(symbol.name.clone()).or_insert(index);
            }
        }
        let text = elf
            .sections
            .iter()
            .find(|s| s.name == ".text")
            .map(|s| (s.address, s.address + s.size))
            .ok_or("The reference executable has no .text section")?;
        let mut code_relocations: Vec<usize> = (0..elf.relocations.len())
            .filter(|&i| elf.sections[elf.relocations[i].section].is_code())
            .collect();
        code_relocations.sort_by_key(|&i| elf.relocations[i].address);
        Ok(Self {
            elf,
            functions,
            group,
            globals,
            locals,
            statics,
            code_relocations,
            text,
        })
    }

    fn symbol(&self, index: usize) -> &Symbol {
        &self.elf.symbols[index]
    }

    fn function(&self, index: usize) -> &Symbol {
        self.symbol(self.functions[index])
    }

    fn containing(&self, address: u32) -> Option<usize> {
        let index = self
            .functions
            .partition_point(|&i| self.elf.symbols[i].address <= address);
        let candidate = index.checked_sub(1)?;
        let function = self.function(candidate);
        (address < function.address + function.size).then_some(candidate)
    }

    fn is_code(&self, address: u32) -> bool {
        self.elf
            .sections
            .iter()
            .any(|s| s.is_code() && s.address <= address && address < s.address + s.size)
    }

    fn relocations_in(&self, start: u32, end: u32) -> &[usize] {
        let first = self
            .code_relocations
            .partition_point(|&i| self.elf.relocations[i].address < start);
        let last = self
            .code_relocations
            .partition_point(|&i| self.elf.relocations[i].address < end);
        &self.code_relocations[first..last]
    }
}

struct Object {
    source: String,
    elf: Elf,
    kept: Vec<bool>,
    group: Option<usize>,
    locals: HashMap<String, usize>,
    redirect: HashMap<usize, u32>,
    section_redirect: HashMap<usize, u32>,
}

struct Linker {
    reference: Reference,
    objects: Vec<Object>,
    globals: HashMap<String, (usize, usize)>,
    aliases: HashMap<String, (usize, usize)>,
    implementation: Vec<Implementation>,
    pulled: BTreeSet<usize>,
    owned: Vec<Vec<usize>>,
    errors: BTreeSet<String>,
}

fn is_ignored(section: &Section) -> bool {
    !section.is_alloc()
        || section.size == 0
        || !matches!(section.kind, SHT_PROGBITS | SHT_NOBITS | SHT_INIT_ARRAY)
        || section.name.starts_with(".eh_frame")
}

impl Linker {
    fn new(reference: Elf, units: &[Unit]) -> Result<Self> {
        let reference = Reference::new(reference)?;
        let mut objects = Vec::new();
        let mut signatures = BTreeSet::new();
        for unit in units {
            let elf = Elf::read(&unit.object)?;
            let mut kept: Vec<bool> = elf.sections.iter().map(|s| !is_ignored(s)).collect();
            for section in elf.sections.iter().filter(|s| s.kind == SHT_GROUP) {
                let (signature, members) = elf.group(section)?;
                if !signatures.insert(signature) {
                    for member in members {
                        if let Some(k) = kept.get_mut(member) {
                            *k = false;
                        }
                    }
                }
            }
            for (index, section) in elf.sections.iter().enumerate() {
                if kept[index]
                    && (section.name.starts_with(".ctors")
                        || section.name.starts_with(".init_array")
                        || section.name.starts_with(".dtors")
                        || section.name.starts_with(".fini_array"))
                {
                    return Err(format!(
                        "{}: C++ static constructors are not supported ({})",
                        unit.source.display(),
                        section.name
                    )
                    .into());
                }
            }
            objects.push(Object {
                source: unit.source.display().to_string(),
                elf,
                kept,
                group: None,
                locals: HashMap::new(),
                redirect: HashMap::new(),
                section_redirect: HashMap::new(),
            });
        }
        let mut linker = Self {
            reference,
            objects,
            globals: HashMap::new(),
            aliases: HashMap::new(),
            implementation: Vec::new(),
            pulled: BTreeSet::new(),
            owned: Vec::new(),
            errors: BTreeSet::new(),
        };
        linker.index_globals()?;
        linker.assign_groups();
        linker.map_implementations();
        linker.owned = vec![Vec::new(); linker.objects.len()];
        for (function, implementation) in linker.implementation.iter().enumerate() {
            if let Implementation::Clang { object, .. } = implementation {
                linker.owned[*object].push(function);
            }
        }
        linker.map_data();
        Ok(linker)
    }

    fn defined_in_kept(&self, object: usize, symbol: &Symbol) -> bool {
        match symbol.section {
            0 => false,
            SHN_ABS => true,
            index => self.objects[object]
                .kept
                .get(index)
                .copied()
                .unwrap_or(false),
        }
    }

    fn index_globals(&mut self) -> Result<()> {
        let mut duplicates = Vec::new();
        for (o, object) in self.objects.iter().enumerate() {
            for (s, symbol) in object.elf.symbols.iter().enumerate() {
                if symbol.binding() == STB_LOCAL || !self.defined_in_kept(o, symbol) {
                    continue;
                }
                if symbol.section == SHN_COMMON {
                    return Err(format!("{}: common symbol {}", object.source, symbol.name).into());
                }
                match self.globals.get(&symbol.name) {
                    Some(&(po, ps))
                        if self.objects[po].elf.symbols[ps].binding() != STB_WEAK
                            && symbol.binding() != STB_WEAK =>
                    {
                        duplicates.push(format!(
                            "{} ({} and {})",
                            symbol.name, self.objects[po].source, object.source
                        ));
                    }
                    Some(&(po, ps))
                        if self.objects[po].elf.symbols[ps].binding() != STB_WEAK
                            || symbol.binding() == STB_WEAK => {}
                    _ => {
                        self.globals.insert(symbol.name.clone(), (o, s));
                    }
                }
            }
        }
        if !duplicates.is_empty() {
            return Err(format!("Duplicate definitions:\n{}", duplicates.join("\n")).into());
        }
        for (name, &definition) in &self.globals {
            for alias in names::mwcc_names(name) {
                self.aliases.entry(alias).or_insert(definition);
            }
        }
        Ok(())
    }

    fn assign_groups(&mut self) {
        for object in &mut self.objects {
            let mut scores: HashMap<usize, usize> = HashMap::new();
            for (index, symbol) in object.elf.symbols.iter().enumerate() {
                if symbol.binding() != STB_LOCAL
                    || !matches!(symbol.kind(), 1 | 2)
                    || symbol.section == 0
                {
                    continue;
                }
                let name = names::local_name(&symbol.name);
                object.locals.insert(name.clone(), index);
                let groups: BTreeSet<usize> = self
                    .reference
                    .locals
                    .get(&name)
                    .into_iter()
                    .flatten()
                    .filter_map(|&r| self.reference.group[r])
                    .collect();
                for group in groups {
                    *scores.entry(group).or_default() += 1;
                }
            }
            let mut ranked: Vec<_> = scores.into_iter().collect();
            ranked.sort_by(|a, b| b.1.cmp(&a.1).then(a.0.cmp(&b.0)));
            object.group = match ranked.as_slice() {
                [(group, _)] => Some(*group),
                [(group, best), (_, next), ..] if best > next => Some(*group),
                _ => None,
            };
        }
    }

    fn clang_global(&self, name: &str) -> Option<(usize, usize)> {
        self.globals
            .get(name)
            .or_else(|| self.aliases.get(name))
            .copied()
    }

    fn is_clang_code(&self, object: usize, symbol: usize) -> bool {
        let symbol = &self.objects[object].elf.symbols[symbol];
        self.objects[object]
            .elf
            .sections
            .get(symbol.section)
            .is_some_and(Section::is_code)
            && symbol.section != SHN_ABS
    }

    fn map_implementations(&mut self) {
        let mut by_group: HashMap<usize, Vec<usize>> = HashMap::new();
        for (index, object) in self.objects.iter().enumerate() {
            if let Some(group) = object.group {
                by_group.entry(group).or_default().push(index);
            }
        }
        self.implementation = (0..self.reference.functions.len())
            .map(|f| {
                let index = self.reference.functions[f];
                let symbol = self.reference.symbol(index);
                if symbol.binding() != STB_LOCAL {
                    if let Some((o, s)) = self.clang_global(&symbol.name)
                        && self.is_clang_code(o, s)
                    {
                        return Implementation::Clang {
                            object: o,
                            symbol: s,
                        };
                    }
                } else if let Some(group) = self.reference.group[index] {
                    for &o in by_group.get(&group).into_iter().flatten() {
                        if let Some(&s) = self.objects[o].locals.get(&symbol.name)
                            && self.is_clang_code(o, s)
                        {
                            return Implementation::Clang {
                                object: o,
                                symbol: s,
                            };
                        }
                    }
                    if self
                        .reference
                        .locals
                        .get(&symbol.name)
                        .is_some_and(|l| l.len() == 1)
                        && !self.reference.globals.contains_key(&symbol.name)
                        && let Some((o, s)) = self.clang_global(&symbol.name)
                        && self.is_clang_code(o, s)
                    {
                        return Implementation::Clang {
                            object: o,
                            symbol: s,
                        };
                    }
                }
                Implementation::Original
            })
            .collect();
    }

    fn reference_data(&self, index: usize) -> Option<u32> {
        let symbol = self.reference.symbol(index);
        let section = self.reference.elf.sections.get(symbol.section)?;
        let kept_code = section.is_code()
            && self.reference.containing(symbol.address).is_none()
            && !(self.reference.text.0..self.reference.text.1).contains(&symbol.address);
        (symbol.section != SHN_ABS && section.is_alloc() && (!section.is_code() || kept_code))
            .then_some(symbol.address)
    }

    fn map_data(&mut self) {
        for o in 0..self.objects.len() {
            let mut redirect = HashMap::new();
            let object = &self.objects[o];
            for (s, symbol) in object.elf.symbols.iter().enumerate() {
                if symbol.kind() != 1 || !self.defined_in_kept(o, symbol) {
                    continue;
                }
                let address = self
                    .lookup_reference(o, &symbol.name, symbol.binding() != STB_LOCAL)
                    .and_then(|r| self.reference_data(r));
                if let Some(address) = address {
                    redirect.insert(s, address);
                }
            }
            let object = &self.objects[o];
            let mut bases: HashMap<usize, BTreeSet<u32>> = HashMap::new();
            for (s, symbol) in object.elf.symbols.iter().enumerate() {
                if symbol.kind() == 1 && self.defined_in_kept(o, symbol) {
                    let base = redirect
                        .get(&s)
                        .map(|a: &u32| a.wrapping_sub(symbol.address));
                    bases
                        .entry(symbol.section)
                        .or_default()
                        .insert(base.unwrap_or(u32::MAX));
                }
            }
            let section_redirect = bases
                .into_iter()
                .filter(|(_, b)| b.len() == 1 && !b.contains(&u32::MAX))
                .map(|(section, b)| (section, *b.iter().next().unwrap()))
                .collect();
            self.objects[o].redirect = redirect;
            self.objects[o].section_redirect = section_redirect;
        }
    }

    fn function_target(&mut self, function: usize, offset: u32) -> Option<Target> {
        match self.implementation[function] {
            Implementation::Clang { object, symbol } if offset == 0 => {
                let symbol = &self.objects[object].elf.symbols[symbol];
                Some(Target::Clang {
                    object,
                    section: symbol.section,
                    value: symbol.address,
                })
            }
            Implementation::Clang { .. } => None,
            Implementation::Original => {
                self.pulled.insert(function);
                Some(Target::Original { function, offset })
            }
        }
    }

    fn reference_address(&mut self, address: u32, label: bool) -> Option<Target> {
        if label
            && let Some(previous) = address
                .checked_sub(1)
                .and_then(|a| self.reference.containing(a))
            && self.reference.function(previous).address + self.reference.function(previous).size
                == address
            && matches!(self.implementation[previous], Implementation::Original)
            && self.pulled.contains(&previous)
        {
            let size = self.reference.function(previous).size;
            return Some(Target::Original {
                function: previous,
                offset: size,
            });
        }
        match self.reference.containing(address) {
            Some(function) => {
                let start = self.reference.function(function).address;
                self.function_target(function, address - start)
            }
            None if self.reference.text.0 <= address && address < self.reference.text.1 => None,
            None => Some(Target::Address(address)),
        }
    }

    fn reference_symbol(&mut self, index: usize, addend: i32) -> Option<Target> {
        let symbol = self.reference.symbol(index).clone();
        if symbol.name == "__ArenaLo" && addend == 0 {
            return Some(Target::ArenaLo);
        }
        if symbol.section == SHN_ABS {
            return Some(Target::Address(symbol.address.wrapping_add(addend as u32)));
        }
        let address = symbol.address.wrapping_add(addend as u32);
        let label = symbol.kind() == 0 && addend == 0;
        if self.reference.is_code(address) || label && self.reference.is_code(address - 1) {
            self.reference_address(address, label)
        } else {
            Some(Target::Address(address))
        }
    }

    fn lookup_reference(&self, object: usize, name: &str, global: bool) -> Option<usize> {
        let mut candidates: Vec<String> = vec![name.to_owned(), names::local_name(name)];
        candidates.extend(names::mwcc_names(name));
        candidates.dedup();
        if global {
            for candidate in &candidates {
                if let Some(&index) = self.reference.globals.get(candidate) {
                    return Some(index);
                }
            }
        }
        for candidate in &candidates {
            if let Some(locals) = self.reference.locals.get(candidate)
                && let Some(index) = self.choose(object, locals)
            {
                return Some(index);
            }
        }
        let (function, variable) = names::static_local(name)?;
        let statics = self.reference.statics.get(&variable)?;
        let mut used = BTreeSet::new();
        for &owned in &self.owned[object] {
            let symbol = self.reference.function(owned);
            if symbol.name != function {
                continue;
            }
            for &r in self
                .reference
                .relocations_in(symbol.address, symbol.address + symbol.size)
            {
                let target = self.reference.elf.relocations[r].symbol;
                if statics.contains(&target) {
                    used.insert(target);
                }
            }
        }
        if used.len() == 1 {
            return used.into_iter().next();
        }
        self.choose(object, statics)
    }

    fn choose(&self, object: usize, locals: &[usize]) -> Option<usize> {
        let group = self.objects[object].group;
        let matching: Vec<_> = locals
            .iter()
            .filter(|&&r| group.is_some() && self.reference.group[r] == group)
            .collect();
        if matching.len() == 1 {
            return Some(*matching[0]);
        }
        if locals.len() == 1 {
            return Some(locals[0]);
        }
        let mut used = BTreeSet::new();
        for &function in &self.owned[object] {
            let symbol = self.reference.function(function);
            for &r in self
                .reference
                .relocations_in(symbol.address, symbol.address + symbol.size)
            {
                let target = self.reference.elf.relocations[r].symbol;
                if locals.contains(&target) {
                    used.insert(target);
                }
            }
        }
        if used.len() == 1 {
            return used.into_iter().next();
        }
        None
    }

    fn clang_symbol(&mut self, object: usize, index: usize) -> Option<Target> {
        let symbol = self.objects[object].elf.symbols[index].clone();
        if self.defined_in_kept(object, &symbol) {
            if let Some(&address) = self.objects[object].redirect.get(&index) {
                return Some(Target::Address(address));
            }
            if symbol.section == SHN_ABS {
                return Some(Target::Address(symbol.address));
            }
            if symbol.kind() == STT_SECTION
                && let Some(&base) = self.objects[object].section_redirect.get(&symbol.section)
            {
                return Some(Target::Address(base));
            }
            if symbol.binding() == STB_LOCAL || symbol.kind() == STT_SECTION {
                return Some(Target::Clang {
                    object,
                    section: symbol.section,
                    value: symbol.address,
                });
            }
        }
        if symbol.kind() == STT_SECTION {
            return None;
        }
        if let Some((o, s)) = self.globals.get(&symbol.name).copied() {
            if let Some(&address) = self.objects[o].redirect.get(&s) {
                return Some(Target::Address(address));
            }
            let definition = &self.objects[o].elf.symbols[s];
            if definition.section == SHN_ABS {
                return Some(Target::Address(definition.address));
            }
            return Some(Target::Clang {
                object: o,
                section: definition.section,
                value: definition.address,
            });
        }
        let index = self.lookup_reference(object, &symbol.name, true)?;
        self.reference_symbol(index, 0)
    }

    fn resolve_clang(&mut self, object: usize, index: usize) -> Option<Target> {
        let target = self.clang_symbol(object, index);
        if target.is_none() {
            let name = &self.objects[object].elf.symbols[index].name;
            self.errors.insert(format!(
                "{}: undefined symbol {name}",
                self.objects[object].source
            ));
        }
        target
    }

    fn collect(&mut self) -> Vec<Vec<bool>> {
        let mut used: Vec<Vec<bool>> = self
            .objects
            .iter()
            .map(|o| {
                o.elf
                    .sections
                    .iter()
                    .enumerate()
                    .map(|(i, s)| o.kept[i] && s.is_code())
                    .collect()
            })
            .collect();
        let mut pending: Vec<(usize, usize)> = used
            .iter()
            .enumerate()
            .flat_map(|(o, sections)| {
                sections
                    .iter()
                    .enumerate()
                    .filter(|(_, u)| **u)
                    .map(move |(s, _)| (o, s))
            })
            .collect();
        while let Some((o, section)) = pending.pop() {
            let relocations: Vec<_> = self.objects[o]
                .elf
                .relocations
                .iter()
                .filter(|r| r.section == section)
                .map(|r| r.symbol)
                .collect();
            for symbol in relocations {
                if let Some(Target::Clang {
                    object, section, ..
                }) = self.resolve_clang(o, symbol)
                    && !used[object][section]
                {
                    used[object][section] = true;
                    pending.push((object, section));
                }
            }
        }
        for relocation in self.table_relocations() {
            if self
                .reference_symbol(relocation.symbol, relocation.addend)
                .is_none()
            {
                self.errors.insert(format!(
                    "original table at {:08X}: cannot relocate reference to {}",
                    relocation.address,
                    self.reference.symbol(relocation.symbol).name
                ));
            }
        }
        let mut done = BTreeSet::new();
        loop {
            let pending: Vec<_> = self.pulled.difference(&done).copied().collect();
            if pending.is_empty() {
                break;
            }
            for function in pending {
                done.insert(function);
                let symbol = self.reference.function(function).clone();
                let relocations = self
                    .reference
                    .relocations_in(symbol.address, symbol.address + symbol.size)
                    .to_vec();
                for index in relocations {
                    let relocation = self.reference.elf.relocations[index].clone();
                    if self
                        .reference_symbol(relocation.symbol, relocation.addend)
                        .is_none()
                    {
                        self.errors.insert(format!(
                            "original {}: cannot relocate reference to {}{:+}",
                            symbol.name,
                            self.reference.symbol(relocation.symbol).name,
                            relocation.addend
                        ));
                    }
                }
            }
        }
        used
    }

    fn table_relocations(&self) -> Vec<Relocation> {
        self.reference
            .code_relocations
            .iter()
            .map(|&i| self.reference.elf.relocations[i].clone())
            .filter(|r| self.reference.containing(r.address).is_none())
            .collect()
    }

    fn data_relocations(&self) -> Vec<Relocation> {
        self.reference
            .elf
            .relocations
            .iter()
            .filter(|r| {
                let section = &self.reference.elf.sections[r.section];
                section.is_alloc() && !section.is_code() && !section.name.starts_with("extab")
            })
            .cloned()
            .collect()
    }
}

fn align(value: u32, alignment: u32) -> u32 {
    value.div_ceil(alignment) * alignment
}

fn patch(buffer: &mut [u8], offset: usize, kind: u32, value: u32, place: u32) -> Result<()> {
    let word = |b: &[u8]| u32::from_be_bytes(b[offset..offset + 4].try_into().unwrap());
    match kind {
        R_PPC_ADDR32 => buffer[offset..offset + 4].copy_from_slice(&value.to_be_bytes()),
        R_PPC_REL32 => {
            buffer[offset..offset + 4].copy_from_slice(&value.wrapping_sub(place).to_be_bytes())
        }
        R_PPC_ADDR16_LO => {
            buffer[offset..offset + 2].copy_from_slice(&(value as u16).to_be_bytes())
        }
        R_PPC_ADDR16_HI => {
            buffer[offset..offset + 2].copy_from_slice(&((value >> 16) as u16).to_be_bytes())
        }
        R_PPC_ADDR16_HA => buffer[offset..offset + 2]
            .copy_from_slice(&((value.wrapping_add(0x8000) >> 16) as u16).to_be_bytes()),
        R_PPC_REL24 | R_PPC_PLTREL24 | R_PPC_LOCAL24PC => {
            let delta = value.wrapping_sub(place) as i32;
            if !(-0x0200_0000..0x0200_0000).contains(&delta) || delta & 3 != 0 {
                return Err(format!("Branch out of range at {place:08X}").into());
            }
            let instruction = (word(buffer) & 0xfc00_0003) | (delta as u32 & 0x03ff_fffc);
            buffer[offset..offset + 4].copy_from_slice(&instruction.to_be_bytes());
        }
        R_PPC_REL14 => {
            let delta = value.wrapping_sub(place) as i32;
            if !(-0x8000..0x8000).contains(&delta) || delta & 3 != 0 {
                return Err(format!("Conditional branch out of range at {place:08X}").into());
            }
            let instruction = (word(buffer) & 0xffff_0003) | (delta as u32 & 0xfffc);
            buffer[offset..offset + 4].copy_from_slice(&instruction.to_be_bytes());
        }
        _ => return Err(format!("Unsupported relocation type {kind} at {place:08X}").into()),
    }
    Ok(())
}

pub struct Report {
    pub compiled: usize,
    pub original: Vec<String>,
    pub code_size: u32,
    pub code_limit: u32,
    pub data_size: u32,
    pub data_address: u32,
    pub map: String,
}

pub fn link(dol_bytes: &[u8], reference: Elf, units: &[Unit]) -> Result<(Vec<u8>, Report)> {
    let mut linker = Linker::new(reference, units)?;
    let data_relocations = linker.data_relocations();
    for relocation in &data_relocations {
        let symbol = linker.reference.symbol(relocation.symbol).clone();
        let address = symbol.address.wrapping_add(relocation.addend as u32);
        if let Some(function) = linker.reference.containing(address)
            && linker.reference.function(function).address == address
        {
            linker.function_target(function, 0);
        }
    }
    let entry_name = "__start";
    let entry = match linker.clang_global(entry_name) {
        Some((o, s)) => {
            let symbol = &linker.objects[o].elf.symbols[s];
            Some(Target::Clang {
                object: o,
                section: symbol.section,
                value: symbol.address,
            })
        }
        None => linker
            .reference
            .globals
            .get(entry_name)
            .copied()
            .and_then(|i| linker.reference_symbol(i, 0)),
    };
    let used = linker.collect();
    if !linker.errors.is_empty() {
        let errors: Vec<_> = linker.errors.iter().take(40).cloned().collect();
        return Err(format!(
            "Link failed with {} errors:\n{}",
            linker.errors.len(),
            errors.join("\n")
        )
        .into());
    }

    let (start, limit) = linker.reference.text;
    let mut section_address: Vec<HashMap<usize, u32>> = vec![HashMap::new(); linker.objects.len()];
    let mut pulled_address = HashMap::new();
    let mut code_cursor = start;
    for (o, object) in linker.objects.iter().enumerate() {
        for (s, section) in object.elf.sections.iter().enumerate() {
            if used[o][s] && section.is_code() {
                code_cursor = align(code_cursor, section.alignment);
                section_address[o].insert(s, code_cursor);
                code_cursor += section.size;
            }
        }
    }
    for &function in &linker.pulled {
        code_cursor = align(code_cursor, 4);
        pulled_address.insert(function, code_cursor);
        code_cursor += linker.reference.function(function).size;
    }
    let arena = linker
        .reference
        .globals
        .get("__ArenaLo")
        .map(|&i| linker.reference.symbol(i).address)
        .ok_or("The reference executable has no __ArenaLo")?;
    let data_start = align(arena, 32);
    let mut cursor = data_start;
    for (o, object) in linker.objects.iter().enumerate() {
        for (s, section) in object.elf.sections.iter().enumerate() {
            if used[o][s] && !section.is_code() {
                cursor = align(cursor, section.alignment);
                section_address[o].insert(s, cursor);
                cursor += section.size;
            }
        }
    }
    if align(code_cursor, 32) > limit {
        return Err(format!(
            "The compiled code needs {} bytes but the original code area holds {}.",
            code_cursor - start,
            limit - start
        )
        .into());
    }
    let arena_lo = align(cursor, 32);
    let code_end = align(code_cursor, 32);
    let mut code = vec![0u8; (code_end - start) as usize];
    let mut data = vec![0u8; (align(cursor, 32) - data_start) as usize];

    let value_of = |target: Target| -> u32 {
        match target {
            Target::Clang {
                object,
                section,
                value,
            } => section_address[object][&section] + value,
            Target::Original { function, offset } => pulled_address[&function] + offset,
            Target::Address(address) => address,
            Target::ArenaLo => arena_lo,
        }
    };

    for (o, object) in linker.objects.iter().enumerate() {
        for (s, section) in object.elf.sections.iter().enumerate() {
            if !used[o][s] || section.kind == SHT_NOBITS {
                continue;
            }
            let address = section_address[o][&s];
            let (buffer, base) = if section.is_code() {
                (&mut code, start)
            } else {
                (&mut data, data_start)
            };
            let offset = (address - base) as usize;
            buffer[offset..offset + section.size as usize]
                .copy_from_slice(object.elf.section_data(section)?);
        }
    }
    for (&function, &address) in &pulled_address {
        let symbol = linker.reference.function(function);
        let section = &linker.reference.elf.sections[symbol.section];
        let source = (symbol.address - section.address + section.offset) as usize;
        let offset = (address - start) as usize;
        code[offset..offset + symbol.size as usize].copy_from_slice(bytes(
            &linker.reference.elf.data,
            source,
            symbol.size as usize,
        )?);
    }

    let mut resolved = Vec::new();
    for (o, sections) in used.iter().enumerate() {
        let relocations = linker.objects[o].elf.relocations.clone();
        for relocation in relocations {
            if !sections[relocation.section] {
                continue;
            }
            let target = linker
                .resolve_clang(o, relocation.symbol)
                .ok_or("Unresolved symbol after collection")?;
            resolved.push((o, relocation, target));
        }
    }
    let mut original = Vec::new();
    for &function in &pulled_address.keys().copied().collect::<Vec<_>>() {
        let symbol = linker.reference.function(function).clone();
        for index in linker
            .reference
            .relocations_in(symbol.address, symbol.address + symbol.size)
            .to_vec()
        {
            let relocation = linker.reference.elf.relocations[index].clone();
            let target = linker
                .reference_symbol(relocation.symbol, relocation.addend)
                .ok_or("Unresolved original relocation")?;
            original.push((function, symbol.address, relocation, target));
        }
    }
    let mut data_patches = Vec::new();
    for relocation in &data_relocations {
        let symbol = linker.reference.symbol(relocation.symbol).clone();
        let address = symbol.address.wrapping_add(relocation.addend as u32);
        if !linker.reference.is_code(address) {
            continue;
        }
        let Some(function) = linker.reference.containing(address) else {
            continue;
        };
        let begin = linker.reference.function(function).address;
        let target = match linker.implementation[function] {
            Implementation::Clang { .. } if address == begin => linker.function_target(function, 0),
            Implementation::Original if linker.pulled.contains(&function) => {
                linker.function_target(function, address - begin)
            }
            _ => None,
        };
        if let Some(target) = target {
            if relocation.kind != R_PPC_ADDR32 {
                return Err(format!(
                    "Unsupported data relocation type {} at {:08X}",
                    relocation.kind, relocation.address
                )
                .into());
            }
            data_patches.push((relocation.address, target));
        }
    }

    for (o, relocation, target) in resolved {
        let section = &linker.objects[o].elf.sections[relocation.section];
        let address = section_address[o][&relocation.section] + relocation.address;
        let value = value_of(target).wrapping_add(relocation.addend as u32);
        let (buffer, base) = if section.is_code() {
            (&mut code, start)
        } else {
            (&mut data, data_start)
        };
        patch(
            buffer,
            (address - base) as usize,
            relocation.kind,
            value,
            address,
        )
        .map_err(|e| format!("{}: {e}", linker.objects[o].source))?;
    }
    for (function, begin, relocation, target) in original {
        let address = pulled_address[&function] + (relocation.address - begin);
        let value = value_of(target);
        if relocation.kind == R_PPC_EMB_SDA21 {
            if target != Target::Address(value) {
                return Err(format!(
                    "Small-data reference at {:08X} points to moved data",
                    relocation.address
                )
                .into());
            }
            continue;
        }
        patch(
            &mut code,
            (address - start) as usize,
            relocation.kind,
            value,
            address,
        )?;
    }

    let mut segments = Vec::new();
    for (offset, address, size) in dol::sections(dol_bytes)? {
        if address == start {
            continue;
        }
        segments.push(Segment {
            address,
            text: dol::is_text_slot(dol_bytes, address)?,
            data: bytes(dol_bytes, offset as usize, size as usize)?.to_vec(),
        });
    }
    for relocation in linker.table_relocations() {
        let target = linker
            .reference_symbol(relocation.symbol, relocation.addend)
            .ok_or("Unresolved table relocation")?;
        if relocation.kind == R_PPC_EMB_SDA21 {
            continue;
        }
        let address = relocation.address;
        let segment = segments
            .iter_mut()
            .find(|s| s.address <= address && address + 4 <= s.address + s.data.len() as u32)
            .ok_or_else(|| format!("Table relocation outside the executable at {address:08X}"))?;
        let offset = (address - segment.address) as usize;
        patch(
            &mut segment.data,
            offset,
            relocation.kind,
            value_of(target),
            address,
        )?;
    }
    for (address, target) in data_patches {
        let value = value_of(target);
        let segment = segments
            .iter_mut()
            .find(|s| s.address <= address && address + 4 <= s.address + s.data.len() as u32)
            .ok_or_else(|| format!("Data relocation outside the executable at {address:08X}"))?;
        let offset = (address - segment.address) as usize;
        segment.data[offset..offset + 4].copy_from_slice(&value.to_be_bytes());
    }
    segments.push(Segment {
        address: start,
        text: true,
        data: code,
    });
    if !data.is_empty() {
        segments.push(Segment {
            address: data_start,
            text: false,
            data,
        });
    }
    let entry = value_of(entry.ok_or("The program has no __start entry point")?);
    let output = dol::write(dol_bytes, &segments, entry)?;

    let mut code_lines = Vec::new();
    let mut data_lines = Vec::new();
    let mut compiled = 0;
    for (o, object) in linker.objects.iter().enumerate() {
        for symbol in &object.elf.symbols {
            if matches!(symbol.kind(), 1 | 2)
                && let Some(&address) = section_address[o].get(&symbol.section)
            {
                let line = (
                    address + symbol.address,
                    symbol.size,
                    symbol.name.clone(),
                    object.source.clone(),
                );
                if object.elf.sections[symbol.section].is_code() {
                    compiled += 1;
                    code_lines.push(line);
                } else {
                    data_lines.push(line);
                }
            }
        }
    }
    let mut names_original = Vec::new();
    for (&function, &address) in &pulled_address {
        let symbol = linker.reference.function(function);
        names_original.push(symbol.name.clone());
        code_lines.push((
            address,
            symbol.size,
            symbol.name.clone(),
            "original".to_owned(),
        ));
    }
    names_original.sort();
    let mut map = String::new();
    for (title, mut lines, base) in [
        (".text", code_lines, start),
        (".data", data_lines, data_start),
    ] {
        lines.sort();
        map.push_str(&format!("{title} section layout\n"));
        for (address, size, name, source) in lines {
            map.push_str(&format!(
                "  {:08x} {size:06x} {address:08x}  4 {name} \t{source}\n",
                address - base
            ));
        }
        map.push('\n');
    }
    Ok((
        output,
        Report {
            compiled,
            original: names_original,
            code_size: code_cursor - start,
            code_limit: limit - start,
            data_size: cursor - data_start,
            data_address: data_start,
            map,
        },
    ))
}
