use crate::{
    Result,
    elf::{bytes, u32be},
    root,
};
use sha1::{Digest, Sha1};
use std::{fs, io::Read, path::Path};

pub const DOL_SHA1: &str = "873470a8dd7a38e22dbabb878d3dd11b3862375f";
pub const ELF_SHA1: &str = "7924c3347a522b5e1b7ea5eaaa913054d8815072";

pub fn reference_hash(data: &[u8], expected: &str, label: &str) -> Result<()> {
    if format!("{:x}", Sha1::digest(data)) != expected {
        return Err(
            format!("{label} hash does not match the US Zoids: Battle Legends disc.").into(),
        );
    }
    Ok(())
}
pub fn dol_sections(dol: &[u8]) -> Result<Vec<(u32, u32, u32)>> {
    (0..18)
        .filter_map(|i| {
            match (
                u32be(dol, i * 4),
                u32be(dol, 0x48 + i * 4),
                u32be(dol, 0x90 + i * 4),
            ) {
                (Ok(offset), Ok(address), Ok(size)) if size > 0 => {
                    Some(Ok((offset, address, size)))
                }
                (Ok(_), Ok(_), Ok(_)) => None,
                (a, b, c) => Some(Err(a
                    .err()
                    .or_else(|| b.err())
                    .or_else(|| c.err())
                    .unwrap())),
            }
        })
        .collect()
}
pub fn read_disc(reader: &mut impl Read) -> Result<(Vec<u8>, Vec<u8>)> {
    fn extend(reader: &mut impl Read, data: &mut Vec<u8>, end: usize) -> Result<()> {
        if end > data.len() {
            let start = data.len();
            data.resize(end, 0);
            reader
                .read_exact(&mut data[start..])
                .map_err(|_| "The game image ended before the executable.")?;
        }
        Ok(())
    }
    let mut data = Vec::new();
    extend(reader, &mut data, 0x440)?;
    if &data[..6] != b"GZSE70" {
        return Err("Expected the GZSE70 GameCube disc.".into());
    }
    let dol_offset = u32be(&data, 0x420)? as usize;
    let fst_offset = u32be(&data, 0x424)? as usize;
    let fst_size = u32be(&data, 0x428)? as usize;
    extend(
        reader,
        &mut data,
        (dol_offset + 256).max(fst_offset + fst_size),
    )?;
    let mut dol_size = 0;
    for i in 0..18 {
        dol_size = dol_size.max(
            u32be(&data, dol_offset + i * 4)? as usize
                + u32be(&data, dol_offset + 0x90 + i * 4)? as usize,
        );
    }
    extend(reader, &mut data, dol_offset + dol_size)?;
    let fst = bytes(&data, fst_offset, fst_size)?;
    let count = u32be(fst, 8)? as usize;
    let names = fst.get(count * 12..).ok_or("Invalid disc file table")?;
    let mut executables = Vec::new();
    for i in 1..count {
        let name = u32be(fst, i * 12)?;
        let rest = names
            .get((name & 0xffffff) as usize..)
            .ok_or("Invalid disc filename offset")?;
        let end = rest
            .iter()
            .position(|&c| c == 0)
            .ok_or("Unterminated disc filename")?;
        if name >> 24 == 0 && &rest[..end] == b"zoidR.elf" {
            executables.push((
                u32be(fst, i * 12 + 4)? as usize,
                u32be(fst, i * 12 + 8)? as usize,
            ));
        }
    }
    if executables.len() != 1 {
        return Err("Expected exactly one zoidR.elf on the disc.".into());
    }
    let (offset, size) = executables[0];
    let mut elf = Vec::new();
    if offset < data.len() {
        elf.extend_from_slice(&data[offset..data.len().min(offset + size)]);
    } else {
        let mut remaining = offset - data.len();
        let mut buffer = vec![0; 4 * 1024 * 1024];
        while remaining > 0 {
            let take = remaining.min(buffer.len());
            reader.read_exact(&mut buffer[..take])?;
            remaining -= take;
        }
    }
    if elf.len() < size {
        let start = elf.len();
        elf.resize(size, 0);
        reader.read_exact(&mut elf[start..])?;
    }
    Ok((data[dol_offset..dol_offset + dol_size].to_vec(), elf))
}
pub fn cached_reference(iso: Option<&Path>) -> Result<(Vec<u8>, Vec<u8>)> {
    let cache = root().join("build/reference");
    if let (Ok(dol), Ok(elf)) = (
        fs::read(cache.join("main.dol")),
        fs::read(cache.join("main.elf")),
    ) && reference_hash(&dol, DOL_SHA1, "DOL").is_ok()
        && reference_hash(&elf, ELF_SHA1, "ELF").is_ok()
    {
        return Ok((dol, elf));
    }
    let iso = iso.ok_or(
        "Put your Zoids: Battle Legends disc image at game.iso in the repository root, or pass --iso PATH.",
    )?;
    let file = fs::File::open(iso)
        .map_err(|e| format!("Cannot open the disc image {}: {e}", iso.display()))?;
    let (dol, elf) = read_disc(&mut std::io::BufReader::new(file))?;
    reference_hash(&dol, DOL_SHA1, "DOL")?;
    reference_hash(&elf, ELF_SHA1, "ELF")?;
    fs::create_dir_all(&cache)?;
    fs::write(cache.join("main.dol"), &dol)?;
    fs::write(cache.join("main.elf"), &elf)?;
    Ok((dol, elf))
}
