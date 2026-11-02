use crate::{Result, elf::u32be};
use std::{
    fs,
    io::{self, BufWriter, Read, Seek, SeekFrom, Write},
    path::Path,
};

const DISC_SIZE: u64 = 0x5705_8000;
const SYSTEM_SIZE: usize = 0x2440;
const FILE_ALIGN: u64 = 0x8000;
const SYSTEM_ALIGN: u64 = 0x100;

fn read_at(file: &mut fs::File, offset: u64, size: usize) -> Result<Vec<u8>> {
    let mut data = vec![0; size];
    file.seek(SeekFrom::Start(offset))?;
    file.read_exact(&mut data)?;
    Ok(data)
}

fn pad(output: &mut impl Write, position: &mut u64, until: u64) -> Result<()> {
    io::copy(&mut io::repeat(0).take(until - *position), output)?;
    *position = until;
    Ok(())
}

pub fn rebuild(source: &Path, dol: &[u8], output: &Path) -> Result<()> {
    let mut input = fs::File::open(source)
        .map_err(|e| format!("Cannot open the disc image {}: {e}", source.display()))?;
    let mut system = read_at(&mut input, 0, SYSTEM_SIZE)?;
    if &system[..6] != b"GZSE70" {
        return Err("Expected the GZSE70 GameCube disc.".into());
    }
    let apploader_header = read_at(&mut input, SYSTEM_SIZE as u64, 0x20)?;
    let apploader_size = (0x20 + u32be(&apploader_header, 0x14)? + u32be(&apploader_header, 0x18)?)
        .next_multiple_of(32) as usize;
    let apploader = read_at(&mut input, SYSTEM_SIZE as u64, apploader_size)?;
    let fst_size = u32be(&system, 0x428)? as usize;
    let mut fst = read_at(&mut input, u32be(&system, 0x424)?.into(), fst_size)?;
    let count = u32be(&fst, 8)? as usize;
    if count * 12 > fst.len() {
        return Err("Invalid disc file table".into());
    }
    let mut files: Vec<(usize, u64, u64)> = (1..count)
        .filter(|&i| fst[i * 12] == 0)
        .map(|i| {
            Ok((
                i,
                u32be(&fst, i * 12 + 4)?.into(),
                u32be(&fst, i * 12 + 8)?.into(),
            ))
        })
        .collect::<Result<_>>()?;
    files.sort_by_key(|&(_, offset, _)| offset);

    let dol_offset = (SYSTEM_SIZE + apploader.len()) as u64;
    let dol_offset = dol_offset.next_multiple_of(SYSTEM_ALIGN);
    let fst_offset = (dol_offset + dol.len() as u64).next_multiple_of(SYSTEM_ALIGN);
    let mut next = (fst_offset + fst_size as u64).next_multiple_of(FILE_ALIGN);
    let mut layout = Vec::with_capacity(files.len());
    for &(index, offset, size) in &files {
        fst[index * 12 + 4..index * 12 + 8].copy_from_slice(&(next as u32).to_be_bytes());
        layout.push((offset, next, size));
        next = (next + size).next_multiple_of(FILE_ALIGN);
    }
    if next > DISC_SIZE {
        return Err(format!(
            "The rebuilt disc needs {next} bytes, more than the {DISC_SIZE} a GameCube disc holds."
        )
        .into());
    }
    system[0x420..0x424].copy_from_slice(&(dol_offset as u32).to_be_bytes());
    system[0x424..0x428].copy_from_slice(&(fst_offset as u32).to_be_bytes());
    system[0x42c..0x430].copy_from_slice(&(fst_size as u32).to_be_bytes());

    let file =
        fs::File::create(output).map_err(|e| format!("Cannot create {}: {e}", output.display()))?;
    let mut writer = BufWriter::with_capacity(4 << 20, file);
    let mut position = 0;
    for (offset, data) in [
        (0, &system[..]),
        (SYSTEM_SIZE as u64, &apploader[..]),
        (dol_offset, dol),
        (fst_offset, &fst[..]),
    ] {
        pad(&mut writer, &mut position, offset)?;
        writer.write_all(data)?;
        position += data.len() as u64;
    }
    for (source_offset, offset, size) in layout {
        pad(&mut writer, &mut position, offset)?;
        input.seek(SeekFrom::Start(source_offset))?;
        let copied = io::copy(&mut (&mut input).take(size), &mut writer)?;
        if copied != size {
            return Err("The disc image ended inside a file.".into());
        }
        position += size;
    }
    writer.flush()?;
    writer
        .into_inner()
        .map_err(|e| e.into_error())?
        .set_len(DISC_SIZE)?;
    Ok(())
}
