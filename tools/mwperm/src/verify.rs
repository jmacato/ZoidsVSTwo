use crate::Result;
use crate::elf::{Elf, Symbol, u32be};
use crate::source::rx;
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};

type Placements = BTreeMap<usize, (usize, i64)>;
pub type Matches = BTreeMap<Symbol, Vec<u8>>;

fn ensure(condition: bool, message: impl Into<String>) -> Result<()> {
    if condition {
        Ok(())
    } else {
        Err(message.into().into())
    }
}
fn unique<T>(mut values: Vec<T>, message: impl Into<String>) -> Result<T> {
    ensure(values.len() == 1, message)?;
    Ok(values.remove(0))
}
fn offset(elf: &Elf, symbol: &Symbol) -> i64 {
    i64::from(symbol.address)
        - if elf.kind == 2 {
            i64::from(elf.sections[symbol.section].address)
        } else {
            0
        }
}
fn valid_section(elf: &Elf, symbol: &Symbol) -> bool {
    symbol.section > 0 && symbol.section < elf.sections.len()
}
fn local_name(name: &str) -> Option<&str> {
    let (base, suffix) = name.rsplit_once('$')?;
    (!base.is_empty() && !suffix.is_empty() && suffix.bytes().all(|c| c.is_ascii_digit()))
        .then_some(base)
}
fn same_local(name: &str, base: &str) -> bool {
    local_name(name) == Some(base)
}
fn placeholder_original(name: &str) -> Option<String> {
    let digits = |s: &str| !s.is_empty() && s.bytes().all(|c| c.is_ascii_digit());
    let name = name
        .split_once("__")
        .filter(|(_, rest)| rest.ends_with("@unnamed@"))
        .map_or(name, |(base, _)| base);
    if let Some(number) = name.strip_prefix("lit_")
        && digits(number)
    {
        return Some(format!("@{number}"));
    }
    let (base, number) = name.rsplit_once('_')?;
    (!base.is_empty() && digits(number)).then(|| format!("{base}${number}"))
}
fn put32(data: &mut [u8], offset: usize, value: u32) -> Result<()> {
    data.get_mut(offset..offset + 4)
        .ok_or("Relocation exceeds its data")?
        .copy_from_slice(&value.to_be_bytes());
    Ok(())
}
fn put16(data: &mut [u8], offset: usize, value: u16) -> Result<()> {
    data.get_mut(offset..offset + 2)
        .ok_or("Relocation exceeds its data")?
        .copy_from_slice(&value.to_be_bytes());
    Ok(())
}
fn insert_mapping(
    sections: &mut Placements,
    index: usize,
    mapping: (usize, i64),
    message: &str,
) -> Result<()> {
    ensure(
        sections.get(&index).is_none_or(|old| *old == mapping),
        message,
    )?;
    sections.insert(index, mapping);
    Ok(())
}
fn referenced<'a>(reference: &'a Elf, function: &Symbol) -> Vec<&'a Symbol> {
    reference
        .relocations_in(
            function.section,
            function.address,
            function.address + function.size,
        )
        .iter()
        .map(|rel| &reference.symbols[rel.symbol])
        .collect()
}
fn mismatch(actual: &[u8], expected: &[u8], address: i64, label: &str) -> Result<()> {
    if actual == expected {
        return Ok(());
    }
    ensure(
        actual.len() == expected.len(),
        format!("{label}: byte length differs"),
    )?;
    let index = actual
        .iter()
        .zip(expected)
        .position(|(a, b)| a != b)
        .ok_or("Missing mismatch")?;
    Err(format!(
        "{label}: byte mismatch at {:08X}: {:02X} != {:02X}",
        address + index as i64,
        actual[index],
        expected[index]
    )
    .into())
}

pub fn initialization_table_regions(reference: &Elf) -> Vec<(usize, u32, u32)> {
    fn regions(reference: &Elf) -> Result<Vec<(usize, u32, u32)>> {
        let consumer = unique(reference.named("__init_data"), "init consumer")?;
        ensure(
            consumer.info == 2 && consumer.size == 192 && valid_section(reference, consumer),
            "init consumer type",
        )?;
        let section = &reference.sections[consumer.section];
        ensure(
            section.name == ".init" && section.kind == 1 && section.flags & 4 != 0,
            "init section",
        )?;
        let mut output = Vec::new();
        for (name, kind, offsets) in [
            ("_rom_copy_info", 1, [(0x1a, 6), (0x1e, 4)]),
            ("_bss_init_info", 8, [(0x6e, 6), (0x72, 4)]),
        ] {
            let symbol = unique(reference.named(name), "init table")?;
            let mut expected = Vec::new();
            for item in &reference.sections {
                if item.kind == kind && item.flags & 2 != 0 && item.size != 0 {
                    expected.extend(item.address.to_be_bytes());
                    if kind == 1 {
                        expected.extend(item.address.to_be_bytes());
                    }
                    expected.extend(item.size.to_be_bytes());
                }
            }
            expected.extend(vec![0; if kind == 1 { 12 } else { 8 }]);
            ensure(
                symbol.info == 17
                    && symbol.section == consumer.section
                    && symbol.size as usize == expected.len()
                    && symbol.address % 4 == 0,
                "init table shape",
            )?;
            let references: Vec<_> = reference
                .relocations
                .iter()
                .filter(|r| &reference.symbols[r.symbol] == symbol)
                .collect();
            let actual: BTreeSet<_> = references
                .iter()
                .filter(|r| r.section == consumer.section && r.addend == 0)
                .map(|r| (i64::from(r.address) - i64::from(consumer.address), r.kind))
                .collect();
            ensure(
                references.len() == 2 && actual == offsets.into_iter().collect(),
                "init references",
            )?;
            let start = symbol.address;
            let end = start.checked_add(symbol.size).ok_or("init overflow")?;
            ensure(
                section.address <= start
                    && start < end
                    && u64::from(end) <= u64::from(section.address) + u64::from(section.size),
                "init bounds",
            )?;
            ensure(
                !reference.functions().any(|f| {
                    f.section == symbol.section && f.address < end && start < f.address + f.size
                }),
                "init overlaps function",
            )?;
            ensure(
                reference
                    .relocations_in(symbol.section, start, end)
                    .is_empty(),
                "init relocations",
            )?;
            ensure(reference.symbol_data(symbol)? == expected, "init records")?;
            output.push((symbol.section, start, end));
        }
        ensure(output[0].2 == output[1].1, "init tables not adjacent")?;
        Ok(output)
    }
    regions(reference).unwrap_or_default()
}

pub fn unclassified_executable_ranges(reference: &Elf) -> Vec<(String, u32, u32)> {
    let tables = initialization_table_regions(reference);
    let mut result = Vec::new();
    for (index, section) in reference
        .sections
        .iter()
        .enumerate()
        .filter(|(_, s)| s.kind == 1 && s.flags & 4 != 0)
    {
        let mut cursor = section.address;
        let mut ranges: Vec<_> = reference
            .functions()
            .filter(|f| f.section == index)
            .map(|f| (f.address, f.address + f.size))
            .collect();
        ranges.extend(tables.iter().filter(|t| t.0 == index).map(|t| (t.1, t.2)));
        ranges.sort_unstable();
        for (start, end) in ranges {
            if start > cursor {
                result.push((section.name.clone(), cursor, start));
            }
            cursor = cursor.max(end);
        }
        if cursor < section.address + section.size {
            result.push((section.name.clone(), cursor, section.address + section.size));
        }
    }
    result
}

pub fn constant_data(
    compiled: &Elf,
    symbol: &Symbol,
    function: &Symbol,
    reference: &Elf,
) -> Result<Vec<u8>> {
    let mut data = compiled.symbol_data(symbol)?.to_vec();
    let source = unique(
        compiled.functions_named(&function.name),
        format!("Expected one compiled function named {}.", function.name),
    )?;
    let mut covered = HashSet::new();
    for relocation in
        compiled.relocations_in(symbol.section, symbol.address, symbol.address + symbol.size)
    {
        let position = (relocation.address - symbol.address) as usize;
        ensure(
            relocation.kind == 1,
            format!("Unsupported constant relocation: {}", relocation.kind),
        )?;
        ensure(
            position.is_multiple_of(4) && position + 4 <= data.len() && covered.insert(position),
            "Constant relocation overlaps or exceeds its data.",
        )?;
        let target = &compiled.symbols[relocation.symbol];
        let address = i64::from(target.address) + i64::from(relocation.addend);
        let value = if target.section == 0 && target.kind() == 0 && relocation.addend == 0 {
            unique(
                reference.functions_named(&target.name),
                format!(
                    "Constant callback has no unique original function: {}",
                    target.name
                ),
            )?
            .address
        } else if target.kind() == 1
            && target.name.starts_with('@')
            && relocation.addend == 0
            && valid_section(compiled, target)
        {
            let section = &compiled.sections[target.section];
            let start = offset(compiled, target);
            ensure(
                section.kind == 1
                    && section.flags & 4 == 0
                    && target.size != 0
                    && start >= 0
                    && start + i64::from(target.size) <= i64::from(section.size)
                    && compiled
                        .relocations_in(
                            target.section,
                            target.address,
                            target.address + target.size,
                        )
                        .is_empty(),
                "Constant literal pointer has invalid initialized storage.",
            )?;
            let mut originals = BTreeSet::new();
            for table in referenced(reference, function) {
                if table.kind() != 1
                    || table.size != symbol.size
                    || !valid_section(reference, table)
                    || reference.sections[table.section].kind != 1
                    || reference.sections[table.section].flags & 4 != 0
                {
                    continue;
                }
                let address = u32be(reference.symbol_data(table)?, position)?;
                for literal in reference.symbols_at(address) {
                    if literal.size != target.size
                        || literal.info != target.info
                        || !valid_section(reference, literal)
                    {
                        continue;
                    }
                    let original = &reference.sections[literal.section];
                    let start = i64::from(literal.address) - i64::from(original.address);
                    if original.kind == 1
                        && original.flags & 4 == 0
                        && original.name == section.name
                        && start >= 0
                        && start + i64::from(literal.size) <= i64::from(original.size)
                        && reference.symbol_data(literal)? == compiled.symbol_data(target)?
                    {
                        originals.insert((literal.address, literal.section));
                    }
                }
            }
            unique(
                originals.into_iter().collect(),
                "Constant literal pointer has no unique original table entry.",
            )?
            .0
        } else {
            ensure(
                target.section == source.section
                    && address % 4 == 0
                    && i64::from(source.address) <= address
                    && address < i64::from(source.address) + i64::from(source.size),
                "Constant relocation leaves its compiled function.",
            )?;
            (i64::from(function.address) + address - i64::from(source.address)).try_into()?
        };
        put32(&mut data, position, value)?;
    }
    Ok(data)
}

pub fn local_file_candidates<'a>(
    compiled: &Elf,
    reference: &'a Elf,
    candidates: Vec<&'a Symbol>,
) -> Vec<&'a Symbol> {
    let mut files = HashMap::new();
    let mut owner = None;
    for (index, symbol) in reference.symbols.iter().enumerate() {
        if symbol.kind() == 4 {
            owner = Some(index);
        } else if symbol.info >> 4 == 0
            && let Some(owner) = owner
        {
            files.insert(symbol, owner);
        }
    }
    let mut anchors = BTreeSet::new();
    for source in compiled.functions() {
        let originals: Vec<_> = reference
            .functions_named(&source.name)
            .into_iter()
            .filter(|s| s.info == source.info)
            .collect();
        if originals.len() != 1 || originals[0].size != source.size {
            continue;
        }
        let original = originals[0];
        if let Some(owner) = files.get(original) {
            anchors.insert(*owner);
        }
        for rel in
            compiled.relocations_in(source.section, source.address, source.address + source.size)
        {
            if rel.addend != 0 || ![4, 6, 10].contains(&rel.kind) {
                continue;
            }
            let callback = &compiled.symbols[rel.symbol];
            if callback.info != 2 || !compiled.functions().any(|s| s == callback) {
                continue;
            }
            let site = original.address + rel.address - source.address;
            let targets: Vec<_> = reference
                .relocations_in(original.section, site, site + 1)
                .into_iter()
                .filter(|r| r.kind == rel.kind && r.addend == 0)
                .map(|r| &reference.symbols[r.symbol])
                .collect();
            if targets.len() == 1 {
                let target = targets[0];
                if target.info == callback.info
                    && target.name == callback.name
                    && target.size == callback.size
                    && let Some(owner) = files.get(target)
                {
                    anchors.insert(*owner);
                }
            }
        }
    }
    if anchors.len() != 1 {
        return candidates;
    }
    let owner = anchors.first().unwrap();
    candidates
        .into_iter()
        .filter(|target| files.get(target) == Some(owner))
        .collect()
}

pub fn resolve_symbol(
    compiled: &Elf,
    symbol: &Symbol,
    reference: &Elf,
    function: &Symbol,
    sections: Option<&Placements>,
    site: Option<u32>,
) -> Result<Symbol> {
    if let Some(&(target_section, base)) = sections.and_then(|s| s.get(&symbol.section)) {
        let start = offset(compiled, symbol);
        ensure(
            start >= 0
                && start + i64::from(symbol.size)
                    <= i64::from(compiled.sections[symbol.section].size),
            format!("Data symbol exceeds its verified section: {}", symbol.name),
        )?;
        return Ok(Symbol {
            address: (base + start).try_into()?,
            section: target_section,
            ..symbol.clone()
        });
    }
    let referenced = referenced(reference, function);
    let local = if symbol.kind() == 1 && symbol.info >> 4 == 0 {
        local_name(&symbol.name)
    } else {
        None
    };
    let candidates =
        if symbol.name.starts_with('@') && valid_section(compiled, symbol) {
            let data = constant_data(compiled, symbol, function, reference)?;
            let identical: Vec<_> = referenced
                .iter()
                .copied()
                .filter(|t| {
                    t.size as usize == data.len()
                        && valid_section(reference, t)
                        && reference.sections[t.section].kind != 8
                })
                .filter_map(|t| match reference.symbol_data(t) {
                    Ok(d) if d == data => Some(Ok(t)),
                    Ok(_) => None,
                    Err(e) => Some(Err(e)),
                })
                .collect::<Result<_>>()?;
            let mut candidates = identical.clone();
            if candidates
                .iter()
                .map(|s| (s.address, s.section))
                .collect::<BTreeSet<_>>()
                .len()
                > 1
            {
                let section = &compiled.sections[symbol.section];
                if section.kind == 1
                    && section.flags & 4 == 0
                    && !compiled
                        .relocations
                        .iter()
                        .any(|r| r.section == symbol.section)
                {
                    let pool = compiled.section_data(section)?;
                    candidates.retain(|t| {
                        let original = &reference.sections[t.section];
                        let base = i64::from(t.address) - offset(compiled, symbol);
                        let start = base - i64::from(original.address);
                        original.kind == 1
                            && original.flags & 4 == 0
                            && original.name == section.name
                            && base % i64::from(section.alignment.max(1)) == 0
                            && start >= 0
                            && section.size > 0
                            && start + i64::from(section.size) <= i64::from(original.size)
                            && pool.len() == section.size as usize
                            && reference.section_data(original).ok().and_then(|data| {
                                data.get(start as usize..start as usize + pool.len())
                            }) == Some(pool)
                    });
                }
                if candidates.len() != 1
                    && let Some(site) = site
                {
                    let sited: BTreeSet<_> = reference
                        .relocations_in(
                            function.section,
                            function.address + site,
                            function.address + site + 1,
                        )
                        .into_iter()
                        .map(|r| &reference.symbols[r.symbol])
                        .collect();
                    candidates = identical
                        .into_iter()
                        .filter(|t| sited.contains(t))
                        .collect();
                }
            }
            candidates
        } else if let Some(base) = local {
            referenced
                .into_iter()
                .filter(|t| t.kind() == 1 && t.info >> 4 == 0 && same_local(&t.name, base))
                .collect()
        } else if let Some(target) = placeholder_target(compiled, reference, symbol) {
            vec![target]
        } else {
            let mut candidates: Vec<_> = reference
                .named(&symbol.name)
                .into_iter()
                .filter(|t| t.section != 0)
                .collect();
            let local: Vec<_> = candidates
                .iter()
                .copied()
                .filter(|t| referenced.contains(t))
                .collect();
            if !local.is_empty() {
                candidates = local;
            } else if candidates.len() > 1 && symbol.info >> 4 == 0 && symbol.section != 0 {
                candidates = local_file_candidates(compiled, reference, candidates);
            } else if candidates.len() > 1
                && symbol.kind() == 1
                && symbol.info >> 4 != 0
                && symbol.section != 0
            {
                candidates.retain(|t| t.info == symbol.info);
            }
            candidates
        };
    let candidates: BTreeMap<_, _> = candidates
        .into_iter()
        .map(|t| ((t.address, t.section), t))
        .collect();
    Ok(unique(
        candidates.into_values().collect(),
        format!(
            "Unresolved or ambiguous symbol in {}: {}",
            function.name, symbol.name
        ),
    )?
    .clone())
}

pub fn original_function<'a>(
    name: &str,
    reference: &'a Elf,
    primary: Option<&'a Symbol>,
    compiled: Option<&Elf>,
) -> Result<&'a Symbol> {
    if let Some(primary) = primary
        && name == primary.name
    {
        return Ok(primary);
    }
    let mut originals = reference.functions_named(name);
    if originals.len() > 1
        && let Some(compiled) = compiled
    {
        let sources = compiled.functions_named(name);
        if sources.len() == 1 && sources[0].info >> 4 == 0 {
            originals = local_file_candidates(compiled, reference, originals);
        }
    }
    unique(
        originals,
        format!("Emitted function has no unique original: {name}"),
    )
}

fn data_pool_bases(
    compiled: &Elf,
    reference: &Elf,
    primary: &Symbol,
    kind: u32,
) -> Result<Placements> {
    let mut sections = Placements::new();
    let names: &[&str] = if kind == 8 {
        &["...bss.0"]
    } else {
        &["...data.0", "...rodata.0"]
    };
    for (index, root) in compiled.symbols.iter().enumerate() {
        if !names.contains(&root.name.as_str())
            || root.kind() != 0
            || root.size != 0
            || !valid_section(compiled, root)
        {
            continue;
        }
        let section = &compiled.sections[root.section];
        ensure(
            section.kind == kind && section.flags & 4 == 0,
            "Compiler data-pool base has the wrong section type.",
        )?;
        ensure(
            root.name != "...rodata.0" || (section.name == ".rodata" && section.flags == 2),
            "Compiler read-only pool has the wrong section type.",
        )?;
        let start = offset(compiled, root);
        ensure(
            start >= 0 && start < i64::from(section.size),
            "Compiler data-pool base exceeds its section.",
        )?;
        for rel in compiled
            .relocations
            .iter()
            .filter(|r| r.symbol == index && compiled.sections[r.section].flags & 4 != 0)
        {
            let source = unique(
                compiled.function_owners(rel.section, rel.address),
                "Data-pool reference has no unique compiled function owner.",
            )?;
            let owner = original_function(&source.name, reference, Some(primary), Some(compiled))?;
            let originals: BTreeSet<_> = reference
                .relocations_in(owner.section, owner.address, owner.address + owner.size)
                .into_iter()
                .filter(|r| {
                    reference.symbols[r.symbol].name == root.name
                        && (root.name != "...rodata.0"
                            || (r.address - owner.address == rel.address - source.address
                                && r.kind == rel.kind
                                && r.addend == rel.addend))
                })
                .map(|r| &reference.symbols[r.symbol])
                .collect();
            let target = unique(
                originals.into_iter().collect(),
                format!(
                    "Data-pool reference has no unique original base: {}",
                    owner.name
                ),
            )?;
            ensure(
                target.kind() == 0 && target.size == 0 && valid_section(reference, target),
                "Original data-pool base is invalid.",
            )?;
            let original = &reference.sections[target.section];
            ensure(
                original.kind == kind && original.flags & 4 == 0,
                "Original data-pool base has the wrong section type.",
            )?;
            ensure(
                root.name != "...rodata.0" || (original.name == ".rodata" && original.flags == 2),
                "Original read-only pool has the wrong section type.",
            )?;
            insert_mapping(
                &mut sections,
                root.section,
                (target.section, i64::from(target.address) - start),
                "Compiled functions disagree on their data-pool base.",
            )?;
        }
    }
    Ok(sections)
}

fn shared_target(
    compiled: &Elf,
    symbol: &Symbol,
    reference: &Elf,
    function: &Symbol,
    owners: &[&Symbol],
    message: String,
) -> Result<Symbol> {
    let mut candidates = BTreeSet::new();
    for owner in owners {
        let original = original_function(&owner.name, reference, Some(function), Some(compiled))?;
        candidates.insert(resolve_symbol(
            compiled, symbol, reference, original, None, None,
        )?);
    }
    unique(candidates.into_iter().collect(), message)
}
fn add_owned(
    owned: &mut BTreeMap<usize, Vec<(i64, i64)>>,
    symbol: &Symbol,
    start: i64,
    label: &str,
) -> Result<()> {
    let owners = owned.entry(symbol.section).or_default();
    let end = start + i64::from(symbol.size);
    ensure(
        !owners.iter().any(|&(a, b)| start < b && end > a),
        format!("Overlapping {label} data objects: {}", symbol.name),
    )?;
    owners.push((start, end));
    Ok(())
}

pub fn verify_bss_data(compiled: &Elf, reference: &Elf, function: &Symbol) -> Result<Placements> {
    let mut sections = data_pool_bases(compiled, reference, function, 8)?;
    let pools = sections.clone();
    let mut owned = BTreeMap::new();
    let mut targets: BTreeMap<usize, BTreeSet<Symbol>> = BTreeMap::new();
    let mut objects: Vec<_> = compiled
        .symbols
        .iter()
        .filter(|s| {
            s.kind() == 1
                && valid_section(compiled, s)
                && compiled.sections[s.section].kind == 8
                && !s.name.starts_with("gap_")
        })
        .collect();
    objects.sort_by_key(|s| {
        s.info >> 4 == 0
            && reference
                .named(&s.name)
                .iter()
                .map(|t| (t.address, t.section))
                .collect::<BTreeSet<_>>()
                .len()
                > 1
    });
    for symbol in objects {
        let section = &compiled.sections[symbol.section];
        let start = offset(compiled, symbol);
        let mut context = function;
        let mut target = None;
        let local = local_name(&symbol.name);
        if let Some(base_name) = local
            && symbol.info >> 4 == 0
        {
            let owners = compiled.function_users(symbol);
            if owners.is_empty()
                && let Some(&(index, base)) = pools.get(&symbol.section)
            {
                let candidates = reference
                    .symbols_at((base + start).try_into()?)
                    .into_iter()
                    .filter(|t| {
                        t.section == index
                            && t.kind() == symbol.kind()
                            && t.info == symbol.info
                            && t.size == symbol.size
                            && same_local(&t.name, base_name)
                    })
                    .collect();
                target = Some(
                    unique(
                        candidates,
                        format!(
                            "Local zero-initialized data has no unique pool object: {}",
                            symbol.name
                        ),
                    )?
                    .clone(),
                );
            } else if owners.len() != 1 {
                target = Some(shared_target(
                    compiled,
                    symbol,
                    reference,
                    function,
                    &owners,
                    format!(
                        "Local zero-initialized data has no unique function owner: {}",
                        symbol.name
                    ),
                )?);
            } else {
                context =
                    original_function(&owners[0].name, reference, Some(function), Some(compiled))?;
            }
        }
        let target = if let Some(target) = target {
            target
        } else if symbol.info >> 4 == 0
            && local.is_none()
            && reference
                .named(&symbol.name)
                .iter()
                .filter(|t| t.section != 0)
                .map(|t| (t.address, t.section))
                .collect::<BTreeSet<_>>()
                .len()
                > 1
        {
            let owners = compiled.function_users(symbol);
            if owners.is_empty()
                && let Some(&(index, base)) = sections.get(&symbol.section)
            {
                unique(
                    reference
                        .named(&symbol.name)
                        .into_iter()
                        .filter(|t| t.section == index && i64::from(t.address) == base + start)
                        .collect(),
                    format!(
                        "Private zero-initialized data has no pool object: {}",
                        symbol.name
                    ),
                )?
                .clone()
            } else if owners.is_empty() {
                let candidates = local_file_candidates(
                    compiled,
                    reference,
                    reference
                        .named(&symbol.name)
                        .into_iter()
                        .filter(|t| t.section != 0)
                        .collect(),
                );
                let target = unique(
                    candidates,
                    format!(
                        "Private zero-initialized data has no function owner: {}",
                        symbol.name
                    ),
                )?;
                ensure(
                    !reference
                        .relocations
                        .iter()
                        .any(|r| &reference.symbols[r.symbol] == target),
                    format!(
                        "Private zero-initialized data has no function owner: {}",
                        symbol.name
                    ),
                )?;
                target.clone()
            } else {
                shared_target(
                    compiled,
                    symbol,
                    reference,
                    function,
                    &owners,
                    format!(
                        "Compiled functions disagree on private data: {}",
                        symbol.name
                    ),
                )?
            }
        } else {
            resolve_symbol(compiled, symbol, reference, context, None, None)?
        };
        ensure(
            valid_section(reference, &target),
            format!(
                "Zero-initialized data has no original section: {}",
                symbol.name
            ),
        )?;
        let original = &reference.sections[target.section];
        ensure(
            section.flags & 4 == 0
                && original.kind == 8
                && section.flags == original.flags
                && section.name == original.name
                && symbol.info == target.info,
            format!(
                "Zero-initialized data type or binding differs: {}",
                symbol.name
            ),
        )?;
        ensure(
            symbol.size != 0 && symbol.size == target.size,
            format!("Zero-initialized data size differs: {}", symbol.name),
        )?;
        ensure(
            start >= 0 && start + i64::from(symbol.size) <= i64::from(section.size),
            format!("Zero-initialized data exceeds its section: {}", symbol.name),
        )?;
        insert_mapping(
            &mut sections,
            symbol.section,
            (target.section, i64::from(target.address) - start),
            "Zero-initialized data objects disagree on their section placement.",
        )?;
        add_owned(&mut owned, symbol, start, "zero-initialized")?;
        targets.entry(symbol.section).or_default().insert(target);
    }
    for (&index, &(target_index, base)) in &sections {
        let section = &compiled.sections[index];
        let original = &reference.sections[target_index];
        let start = base - i64::from(original.address);
        ensure(
            start >= 0
                && section.size > 0
                && start + i64::from(section.size) <= i64::from(original.size)
                && base % i64::from(section.alignment.max(1)) == 0,
            "Zero-initialized data pool exceeds its original section or alignment.",
        )?;
        ensure(
            !compiled.relocations.iter().any(|r| r.section == index),
            "Zero-initialized data cannot contain relocations.",
        )?;
        let originals: BTreeSet<_> = reference
            .symbols
            .iter()
            .filter(|s| {
                s.kind() == 1
                    && s.size > 0
                    && s.section == target_index
                    && i64::from(s.address) < base + i64::from(section.size)
                    && base < i64::from(s.address) + i64::from(s.size)
            })
            .cloned()
            .collect();
        ensure(
            originals == targets.get(&index).cloned().unwrap_or_default(),
            "Zero-initialized data pool does not cover its original objects exactly.",
        )?;
    }
    ensure(
        !compiled
            .sections
            .iter()
            .enumerate()
            .any(|(i, s)| s.kind == 8 && s.size > 0 && !sections.contains_key(&i)),
        "Zero-initialized data section has no verified original placement.",
    )?;
    Ok(sections)
}

fn site_data_bases(
    compiled: &Elf,
    reference: &Elf,
    primary: &Symbol,
    sections: &mut Placements,
) -> Result<()> {
    for (index, symbol) in compiled.symbols.iter().enumerate() {
        if symbol.kind() != 1
            || symbol.info >> 4 != 0
            || symbol.name.starts_with('@')
            || !valid_section(compiled, symbol)
            || sections.contains_key(&symbol.section)
            || !reference.named(&symbol.name).is_empty()
        {
            continue;
        }
        let start = offset(compiled, symbol);
        for rel in compiled
            .relocations
            .iter()
            .filter(|r| r.symbol == index && compiled.sections[r.section].flags & 4 != 0)
        {
            let owners = compiled.function_owners(rel.section, rel.address);
            if owners.len() != 1 || !crate::source::is_assembly_leaf(&owners[0].name) {
                continue;
            }
            let original =
                original_function(&owners[0].name, reference, Some(primary), Some(compiled))?;
            let site = original.address + rel.address - owners[0].address;
            let matches: Vec<_> = reference
                .relocations_in(original.section, site, site + 1)
                .into_iter()
                .filter(|r| r.kind == rel.kind)
                .collect();
            if matches.len() != 1 {
                continue;
            }
            let target = &reference.symbols[matches[0].symbol];
            if !valid_section(reference, target) {
                continue;
            }
            let base = i64::from(target.address) + i64::from(matches[0].addend)
                - i64::from(rel.addend)
                - start;
            insert_mapping(
                sections,
                symbol.section,
                (target.section, base),
                "Compiled functions disagree on their data-pool base.",
            )?;
        }
    }
    Ok(())
}
fn placeholder_target<'a>(
    compiled: &Elf,
    reference: &'a Elf,
    symbol: &Symbol,
) -> Option<&'a Symbol> {
    if symbol.kind() != 1
        || symbol.info >> 4 != 0
        || !valid_section(compiled, symbol)
        || !matches!(compiled.sections[symbol.section].kind, 1 | 8)
        || !reference.named(&symbol.name).is_empty()
    {
        return None;
    }
    let name = placeholder_original(&symbol.name)?;
    let candidates: Vec<_> = reference
        .named(&name)
        .into_iter()
        .filter(|t| {
            t.kind() == 1
                && t.info >> 4 == 0
                && t.size == symbol.size
                && valid_section(reference, t)
                && reference.sections[t.section].name == compiled.sections[symbol.section].name
        })
        .collect();
    match local_file_candidates(compiled, reference, candidates)[..] {
        [target] => Some(target),
        _ => None,
    }
}
fn placeholder_bases(compiled: &Elf, reference: &Elf, sections: &mut Placements) -> Result<()> {
    for symbol in &compiled.symbols {
        let Some(target) = placeholder_target(compiled, reference, symbol) else {
            continue;
        };
        if compiled.sections[symbol.section].kind != 1 {
            continue;
        }
        insert_mapping(
            sections,
            symbol.section,
            (
                target.section,
                i64::from(target.address) - offset(compiled, symbol),
            ),
            "Data placeholders disagree on their section placement.",
        )?;
    }
    Ok(())
}
fn literal_data_bases(
    compiled: &Elf,
    reference: &Elf,
    primary: &Symbol,
    sections: &mut Placements,
) -> Result<()> {
    let mut found: BTreeMap<usize, BTreeSet<(usize, i64)>> = BTreeMap::new();
    for rel in compiled
        .relocations
        .iter()
        .filter(|r| compiled.sections[r.section].flags & 4 != 0)
    {
        let symbol = &compiled.symbols[rel.symbol];
        if !symbol.name.starts_with('@')
            || symbol.kind() != 1
            || !valid_section(compiled, symbol)
            || sections.contains_key(&symbol.section)
        {
            continue;
        }
        let section = &compiled.sections[symbol.section];
        if section.kind != 1
            || section.flags & 4 != 0
            || ![".data", ".rodata"].contains(&section.name.as_str())
        {
            continue;
        }
        let owners = compiled.function_owners(rel.section, rel.address);
        if owners.len() != 1 {
            continue;
        }
        let original =
            original_function(&owners[0].name, reference, Some(primary), Some(compiled))?;
        let Ok(target) = resolve_symbol(
            compiled,
            symbol,
            reference,
            original,
            None,
            Some(rel.address - owners[0].address),
        ) else {
            continue;
        };
        if reference.sections[target.section].name != section.name {
            continue;
        }
        found.entry(symbol.section).or_default().insert((
            target.section,
            i64::from(target.address) - offset(compiled, symbol),
        ));
    }
    for (index, mappings) in found {
        if mappings.len() == 1 {
            sections.insert(index, *mappings.first().unwrap());
        }
    }
    Ok(())
}

fn pooled_literal_bases(compiled: &Elf, reference: &Elf, sections: &mut Placements) -> Result<()> {
    let mut discovered = Vec::new();
    for rel in &compiled.relocations {
        let Some(&(target_index, base)) = sections.get(&rel.section) else {
            continue;
        };
        let literal = &compiled.symbols[rel.symbol];
        if rel.kind != 1
            || rel.addend != 0
            || !literal.name.starts_with('@')
            || literal.kind() != 1
            || literal.size == 0
            || !valid_section(compiled, literal)
        {
            continue;
        }
        let section = &compiled.sections[literal.section];
        if section.kind != 1
            || section.flags & 4 != 0
            || !compiled
                .relocations_in(
                    literal.section,
                    literal.address,
                    literal.address + literal.size,
                )
                .is_empty()
        {
            continue;
        }
        let pool = &compiled.sections[rel.section];
        let position = i64::from(rel.address)
            - if compiled.kind == 2 {
                i64::from(pool.address)
            } else {
                0
            };
        ensure(
            position >= 0 && position % 4 == 0 && position + 4 <= i64::from(pool.size),
            "Literal pointer exceeds its initialized pool.",
        )?;
        let original = &reference.sections[target_index];
        let pointer_offset: usize = (base + position - i64::from(original.address)).try_into()?;
        let address = u32be(reference.section_data(original)?, pointer_offset)?;
        let candidates = reference.symbols_at(address).into_iter().filter(|target| {
            target.kind() == literal.kind()
                && target.info == literal.info
                && target.size == literal.size
                && valid_section(reference, target)
                && reference.sections[target.section].kind == 1
                && reference.sections[target.section].flags & 4 == 0
                && reference.sections[target.section].name == section.name
                && matches!((reference.symbol_data(target), compiled.symbol_data(literal)), (Ok(a), Ok(b)) if a == b)
        }).collect();
        let target = unique(
            candidates,
            "Pooled literal pointer has no unique matching original object.",
        )?;
        // Recovered literal pools can begin inside the original compilation unit's pool.
        let literal_base = i64::from(target.address) - offset(compiled, literal);
        discovered.push((literal.section, (target.section, literal_base)));
    }
    for (index, mapping) in discovered {
        insert_mapping(
            sections,
            index,
            mapping,
            "Pooled literal pointers disagree on their section placement.",
        )?;
    }
    Ok(())
}

pub fn verify_initialized_data(
    compiled: &Elf,
    reference: &Elf,
    function: &Symbol,
) -> Result<Placements> {
    let mut sections = data_pool_bases(compiled, reference, function, 1)?;
    site_data_bases(compiled, reference, function, &mut sections)?;
    placeholder_bases(compiled, reference, &mut sections)?;
    let alias = |s: &Symbol| {
        s.info >> 4 == 0 && reference.named(&s.name).is_empty() && local_name(&s.name).is_none()
    };
    let needed: BTreeSet<_> = compiled
        .symbols
        .iter()
        .filter(|s| {
            s.kind() == 1
                && alias(s)
                && !sections.contains_key(&s.section)
                && valid_section(compiled, s)
                && compiled.sections[s.section].kind == 1
                && !s.name.starts_with('@')
        })
        .map(|s| s.section)
        .collect();
    if !needed.is_empty() {
        let mut found = sections.clone();
        literal_data_bases(compiled, reference, function, &mut found)?;
        for index in needed {
            if let Some(&mapping) = found.get(&index) {
                sections.insert(index, mapping);
            }
        }
    }
    let mut owned = BTreeMap::new();
    let mut objects: Vec<_> = compiled
        .symbols
        .iter()
        .filter(|s| {
            s.kind() == 1
                && s.section != 0
                && !(valid_section(compiled, s) && compiled.sections[s.section].kind == 8)
                && !s.name.starts_with('@')
                && !s.name.starts_with("gap_")
        })
        .collect();
    objects.sort_by_key(|s| alias(s));
    for symbol in objects {
        ensure(
            valid_section(compiled, symbol),
            format!(
                "Defined game data needs separate verification: {}; the object has no initialized section.",
                symbol.name
            ),
        )?;
        let mut context = function;
        let mut shared = None;
        let local = if symbol.info >> 4 == 0 {
            local_name(&symbol.name)
        } else {
            None
        };
        if local.is_some() {
            let owners = compiled.function_users(symbol);
            if owners.len() == 1 {
                context =
                    original_function(&owners[0].name, reference, Some(function), Some(compiled))?;
            } else if !sections.contains_key(&symbol.section) {
                shared = Some(shared_target(
                    compiled,
                    symbol,
                    reference,
                    function,
                    &owners,
                    format!("Local data has no unique function owner: {}", symbol.name),
                )?);
            }
        }
        let pooled_local = local.filter(|_| sections.contains_key(&symbol.section));
        let target = if alias(symbol) || pooled_local.is_some() {
            let &(index, base) = sections.get(&symbol.section).ok_or_else(|| {
                format!(
                    "Local initialized data has no verified pool: {}",
                    symbol.name
                )
            })?;
            let candidates = reference
                .symbols_at((base + offset(compiled, symbol)).try_into()?)
                .into_iter()
                .filter(|t| {
                    t.kind() == 1
                        && t.info >> 4 == 0
                        && t.section == index
                        && t.size == symbol.size
                        && if let Some(name) = pooled_local {
                            same_local(&t.name, name)
                        } else {
                            rx(r"^@[0-9]+$").is_match(&t.name) || local_name(&t.name).is_some()
                        }
                })
                .collect();
            unique(
                candidates,
                format!(
                    "Local initialized data has no original pool object: {}",
                    symbol.name
                ),
            )?
            .clone()
        } else if let Some(shared) = shared {
            shared
        } else {
            resolve_symbol(compiled, symbol, reference, context, None, None)?
        };
        let section = &compiled.sections[symbol.section];
        ensure(
            valid_section(reference, &target),
            format!("Initialized data has no original section: {}", symbol.name),
        )?;
        let original = &reference.sections[target.section];
        ensure(
            section.kind == 1
                && section.flags & 4 == 0
                && original.kind == 1
                && original.flags & 4 == 0
                && target.kind() == 1,
            format!(
                "Defined game data needs separate verification: {}; only initialized data sections are supported.",
                symbol.name
            ),
        )?;
        ensure(
            symbol.size > 0 && symbol.size == target.size,
            format!("Initialized data size differs: {}", symbol.name),
        )?;
        let start = offset(compiled, symbol);
        ensure(
            start >= 0 && start + i64::from(symbol.size) <= i64::from(section.size),
            format!("Initialized data exceeds its section: {}", symbol.name),
        )?;
        insert_mapping(
            &mut sections,
            symbol.section,
            (target.section, i64::from(target.address) - start),
            "Initialized data objects disagree on their section placement.",
        )?;
        add_owned(&mut owned, symbol, start, "initialized")?;
    }
    pooled_literal_bases(compiled, reference, &mut sections)?;
    for (&index, &(target_index, base)) in &sections {
        let section = &compiled.sections[index];
        let original = &reference.sections[target_index];
        let start = base - i64::from(original.address);
        ensure(
            start >= 0
                && section.size > 0
                && start + i64::from(section.size) <= i64::from(original.size),
            "Initialized data pool exceeds its original section.",
        )?;
        let mut actual = compiled.section_data(section)?.to_vec();
        ensure(
            actual.len() == section.size as usize,
            "Initialized data pool is truncated.",
        )?;
        let mut covered = BTreeSet::new();
        for rel in compiled.relocations.iter().filter(|r| r.section == index) {
            let position = i64::from(rel.address)
                - if compiled.kind == 2 {
                    i64::from(section.address)
                } else {
                    0
                };
            ensure(
                rel.kind == 1,
                format!("Unsupported initialized data relocation: {}", rel.kind),
            )?;
            ensure(
                position >= 0
                    && position % 4 == 0
                    && position + 4 <= i64::from(section.size)
                    && covered.insert(position),
                "Initialized data relocation overlaps or exceeds its pool.",
            )?;
            let source = &compiled.symbols[rel.symbol];
            let value = if valid_section(compiled, source)
                && compiled.sections[source.section].flags & 4 != 0
            {
                let address = i64::from(source.address) + i64::from(rel.addend);
                ensure(
                    address % 4 == 0,
                    "Initialized data target leaves its compiled functions.",
                )?;
                let owner = unique(
                    compiled.function_owners(source.section, address.try_into()?),
                    "Initialized data target leaves its compiled functions.",
                )?;
                let target =
                    original_function(&owner.name, reference, Some(function), Some(compiled))?;
                i64::from(target.address) + address - i64::from(owner.address)
            } else {
                let bss;
                let mappings = if valid_section(compiled, source)
                    && compiled.sections[source.section].kind == 8
                {
                    bss = verify_bss_data(compiled, reference, function)?;
                    &bss
                } else {
                    &sections
                };
                let target =
                    resolve_symbol(compiled, source, reference, function, Some(mappings), None)?;
                i64::from(target.address) + i64::from(rel.addend)
            };
            put32(&mut actual, position as usize, value as u32)?;
        }
        let expected = &reference.section_data(original)?
            [start as usize..start as usize + section.size as usize];
        mismatch(&actual, expected, base, "Initialized data pool")?;
    }
    Ok(sections)
}

pub fn relocate(compiled: &Elf, reference: &Elf, function: &Symbol) -> Result<Vec<u8>> {
    let mut sections = verify_initialized_data(compiled, reference, function)?;
    sections.extend(verify_bss_data(compiled, reference, function)?);
    let source = unique(
        compiled.functions_named(&function.name),
        format!("Expected one compiled function named {}.", function.name),
    )?;
    let mut output = compiled.symbol_data(source)?.to_vec();
    ensure(
        output.len() == function.size as usize,
        format!(
            "{}: generated {} bytes; expected {}.",
            function.name,
            output.len(),
            function.size
        ),
    )?;
    for rel in compiled
        .relocations
        .iter()
        .filter(|r| r.section == source.section)
    {
        if !(source.address <= rel.address && rel.address < source.address + source.size) {
            ensure(
                compiled.function_owners(rel.section, rel.address).len() == 1,
                "Relocation lies outside the compiled functions.",
            )?;
            continue;
        }
        let target = resolve_symbol(
            compiled,
            &compiled.symbols[rel.symbol],
            reference,
            function,
            Some(&sections),
            Some(rel.address - source.address),
        )?;
        let value = i64::from(target.address) + i64::from(rel.addend);
        let position = (rel.address - source.address) as usize;
        let word_offset = position & !3;
        let word = u32be(&output, word_offset)?;
        match rel.kind {
            10 => {
                let displacement = value - i64::from(function.address) - word_offset as i64;
                ensure(
                    displacement % 4 == 0 && (-(1 << 25)..(1 << 25)).contains(&displacement),
                    "PowerPC branch exceeds its signed range.",
                )?;
                put32(
                    &mut output,
                    word_offset,
                    (word & 0xfc000003) | (displacement as u32 & 0x03fffffc),
                )?;
            }
            109 => {
                let name = &reference.sections[target.section].name;
                let (base, register) = match name.as_str() {
                    ".sdata2" | ".sbss2" => ("_SDA2_BASE_", 2),
                    ".sdata" | ".sbss" => ("_SDA_BASE_", 13),
                    _ => return Err(format!("SDA relocation refers to {name}.").into()),
                };
                let displacement = value
                    - i64::from(unique(reference.named(base), "SDA base is ambiguous")?.address);
                ensure(
                    (-32768..32768).contains(&displacement),
                    "SDA relocation exceeds its signed range.",
                )?;
                put32(
                    &mut output,
                    word_offset,
                    (word & 0xffe00000) | (register << 16) | (displacement as u32 & 0xffff),
                )?;
            }
            4..=6 => {
                let half = match rel.kind {
                    4 => value,
                    5 => value >> 16,
                    _ => (value + 0x8000) >> 16,
                };
                put16(&mut output, position, half as u16)?;
            }
            1 => put32(&mut output, position, value as u32)?,
            _ => return Err(format!("Unsupported PowerPC relocation: {}", rel.kind).into()),
        }
    }
    Ok(output)
}

pub fn verify_vector_table(compiled: &Elf, reference: &Elf) -> Result<Matches> {
    let start = unique(
        reference.named("gTRKInterruptVectorTable"),
        "The TRK vector table has no unique original region.",
    )?;
    let end = unique(
        reference.named("gTRKInterruptVectorTableEnd"),
        "The TRK vector table has no unique original region.",
    )?;
    ensure(
        start.kind() == 0
            && end.kind() == 0
            && start.size == 0
            && end.size == 0
            && start.section == end.section,
        "The TRK vector table has no unique original region.",
    )?;
    let table = unique(
        compiled
            .named("gTRKInterruptVectorTable")
            .into_iter()
            .filter(|s| s.section != 0)
            .collect(),
        "Expected one compiled TRK vector table.",
    )?;
    ensure(
        valid_section(compiled, table),
        "Expected one compiled TRK vector table.",
    )?;
    let index = table.section;
    let section = &compiled.sections[index];
    let base = if compiled.kind == 2 {
        section.address
    } else {
        0
    };
    ensure(
        section.name == ".init"
            && section.kind == 1
            && section.flags & 4 != 0
            && reference.sections[start.section].name == ".init"
            && table.address == base
            && end.address.checked_sub(start.address) == Some(section.size),
        "The compiled TRK vector table does not span its original region.",
    )?;
    ensure(
        !compiled
            .sections
            .iter()
            .enumerate()
            .any(|(i, s)| s.flags & 2 != 0 && s.size > 0 && i != index),
        "The TRK vector table unit emits other allocated sections.",
    )?;
    let mut output = compiled.section_data(section)?.to_vec();
    let mut expected: Vec<_> = reference
        .relocations_in(start.section, start.address, end.address)
        .into_iter()
        .map(|r| {
            (
                r.address,
                r.kind,
                reference.symbols[r.symbol].name.clone(),
                r.addend,
            )
        })
        .collect();
    expected.sort();
    let mut actual = Vec::new();
    for rel in compiled.relocations.iter().filter(|r| r.section == index) {
        let position = rel
            .address
            .checked_sub(base)
            .ok_or("Vector relocation precedes its region")?;
        let symbol = &compiled.symbols[rel.symbol];
        let value = if symbol.section == index {
            i64::from(start.address) + i64::from(symbol.address) - i64::from(base)
                + i64::from(rel.addend)
        } else if symbol.section != 0 {
            return Err(format!(
                "TRK vector table relocation leaves its region: {}",
                symbol.name
            )
            .into());
        } else {
            let target = unique(
                reference.functions_named(&symbol.name),
                format!(
                    "Unresolved or ambiguous symbol in the TRK vector table: {}",
                    symbol.name
                ),
            )?;
            actual.push((
                start.address + position,
                rel.kind,
                symbol.name.clone(),
                rel.addend,
            ));
            i64::from(target.address) + i64::from(rel.addend)
        };
        match rel.kind {
            10 => {
                let word = u32be(&output, position as usize)?;
                let displacement = value - i64::from(start.address) - i64::from(position);
                ensure(
                    position % 4 == 0
                        && displacement % 4 == 0
                        && (-(1 << 25)..(1 << 25)).contains(&displacement),
                    "PowerPC branch exceeds its signed range.",
                )?;
                put32(
                    &mut output,
                    position as usize,
                    (word & 0xfc000003) | (displacement as u32 & 0x03fffffc),
                )?;
            }
            4..=6 => {
                let half = match rel.kind {
                    4 => value,
                    5 => value >> 16,
                    _ => (value + 0x8000) >> 16,
                };
                put16(&mut output, position as usize, half as u16)?;
            }
            _ => return Err(format!("Unsupported PowerPC relocation: {}", rel.kind).into()),
        }
    }
    actual.sort();
    ensure(
        actual == expected,
        "TRK vector table relocations differ from the original region.",
    )?;
    let region = Symbol {
        size: end.address - start.address,
        ..start.clone()
    };
    mismatch(
        &output,
        reference.symbol_data(&region)?,
        i64::from(start.address),
        &start.name,
    )?;
    Ok(BTreeMap::from([(region, output)]))
}

pub fn verify_object(compiled: &Elf, reference: &Elf, primary: Option<&Symbol>) -> Result<Matches> {
    if primary.is_none()
        && compiled
            .named("gTRKInterruptVectorTable")
            .iter()
            .any(|s| s.section != 0)
    {
        return verify_vector_table(compiled, reference);
    }
    let mut targets = Vec::new();
    let mut names = BTreeSet::new();
    for source in compiled.functions() {
        ensure(
            names.insert(&source.name),
            format!("Duplicate emitted function: {}", source.name),
        )?;
        targets.push((
            source,
            original_function(&source.name, reference, primary, Some(compiled))?,
        ));
    }
    ensure(
        !targets.is_empty(),
        "Compiled unit has no native functions.",
    )?;
    if let Some(primary) = primary {
        ensure(
            names.contains(&primary.name),
            format!("Expected one compiled function named {}.", primary.name),
        )?;
    }
    let mut ordered = targets.clone();
    ordered.sort_by_key(|(s, _)| (s.section, s.address));
    for pair in ordered.windows(2) {
        let (a, b) = (pair[0].0, pair[1].0);
        ensure(
            a.section != b.section || a.address + a.size <= b.address,
            format!("Overlapping emitted functions: {}", b.name),
        )?;
    }
    let mut matched = Matches::new();
    for (_, function) in targets {
        let actual = relocate(compiled, reference, function)?;
        mismatch(
            &actual,
            reference.symbol_data(function)?,
            i64::from(function.address),
            &function.name,
        )?;
        matched.insert(function.clone(), actual);
    }
    Ok(matched)
}
