use crate::{Result, elf::u32be};

pub struct Segment {
    pub address: u32,
    pub text: bool,
    pub data: Vec<u8>,
}

pub fn sections(dol: &[u8]) -> Result<Vec<(u32, u32, u32)>> {
    let mut sections = Vec::new();
    for slot in 0..18 {
        let offset = u32be(dol, slot * 4)?;
        let address = u32be(dol, 0x48 + slot * 4)?;
        let size = u32be(dol, 0x90 + slot * 4)?;
        if size > 0 {
            sections.push((offset, address, size));
        }
    }
    Ok(sections)
}

pub fn is_text_slot(dol: &[u8], address: u32) -> Result<bool> {
    for slot in 0..7 {
        if u32be(dol, 0x48 + slot * 4)? == address && u32be(dol, 0x90 + slot * 4)? > 0 {
            return Ok(true);
        }
    }
    Ok(false)
}

pub fn write(original: &[u8], segments: &[Segment], entry: u32) -> Result<Vec<u8>> {
    let mut text: Vec<&Segment> = segments.iter().filter(|s| s.text).collect();
    let mut data: Vec<&Segment> = segments.iter().filter(|s| !s.text).collect();
    text.sort_by_key(|s| s.address);
    data.sort_by_key(|s| s.address);
    if text.len() > 7 || data.len() > 11 {
        return Err("Too many DOL sections".into());
    }
    let mut output = vec![0u8; 0x100];
    let put = |output: &mut Vec<u8>, slot: usize, segment: &Segment| {
        while !output.len().is_multiple_of(32) {
            output.push(0);
        }
        let offset = output.len() as u32;
        output.extend_from_slice(&segment.data);
        output[slot * 4..slot * 4 + 4].copy_from_slice(&offset.to_be_bytes());
        output[0x48 + slot * 4..0x48 + slot * 4 + 4]
            .copy_from_slice(&segment.address.to_be_bytes());
        output[0x90 + slot * 4..0x90 + slot * 4 + 4]
            .copy_from_slice(&(segment.data.len() as u32).to_be_bytes());
    };
    for (slot, segment) in text.iter().enumerate() {
        put(&mut output, slot, segment);
    }
    for (slot, segment) in data.iter().enumerate() {
        put(&mut output, 7 + slot, segment);
    }
    while !output.len().is_multiple_of(32) {
        output.push(0);
    }
    output[0xd8..0xe0].copy_from_slice(&original[0xd8..0xe0]);
    output[0xe0..0xe4].copy_from_slice(&entry.to_be_bytes());
    Ok(output)
}
