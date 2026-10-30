use crate::{Result, elf::Elf};
use regex::Regex;
use std::collections::{HashMap, HashSet};
use std::path::Path;
use std::sync::{Arc, LazyLock, RwLock};

static PATTERNS: LazyLock<RwLock<HashMap<String, Arc<Regex>>>> = LazyLock::new(RwLock::default);

pub fn rx(pattern: &str) -> Arc<Regex> {
    if let Some(value) = PATTERNS.read().unwrap().get(pattern) {
        return value.clone();
    }
    let value = Arc::new(Regex::new(pattern).expect("Invalid source-analysis pattern"));
    PATTERNS
        .write()
        .unwrap()
        .insert(pattern.to_owned(), value.clone());
    value
}

pub fn masked(source: &str) -> String {
    rx(r#"(?s)//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'"#)
        .replace_all(source, |capture: &regex::Captures<'_>| {
            capture[0]
                .bytes()
                .map(|byte| if byte == b'\n' { '\n' } else { ' ' })
                .collect::<String>()
        })
        .into_owned()
}

pub fn closing(text: &str, start: usize, left: u8, right: u8) -> Result<usize> {
    let mut depth = 0i64;
    for (offset, &byte) in text
        .as_bytes()
        .get(start..)
        .ok_or("Delimiter exceeds source")?
        .iter()
        .enumerate()
    {
        depth += i64::from(byte == left) - i64::from(byte == right);
        if depth == 0 {
            return Ok(start + offset + 1);
        }
    }
    Err(format!("Unclosed {}", char::from(left)).into())
}

pub fn function_span(source: &str, name: &str) -> Result<(usize, usize)> {
    let clean = masked(source);
    let pattern = rx(&format!(
        r"(?m)^[^\n;{{}}()=]*\b{}\s*\([^;{{}}]*\)\s*\{{",
        regex::escape(name)
    ));
    let matches: Vec<_> = pattern.find_iter(&clean).collect();
    if matches.len() != 1 {
        return Err(format!("Expected one source definition of {name}").into());
    }
    let start = matches[0].end() - 1;
    Ok((start, closing(&clean, start, b'{', b'}')?))
}

pub fn argument_spans(text: &str, start: usize, end: usize) -> Vec<(usize, usize)> {
    let mut result = Vec::new();
    let mut first = start;
    let mut depth = 0i64;
    for (index, &byte) in text.as_bytes().iter().enumerate().take(end).skip(start) {
        depth += i64::from(b"([".contains(&byte)) - i64::from(b")]".contains(&byte));
        if byte == b',' && depth == 0 {
            result.push((first, index));
            first = index + 1;
        }
    }
    result.push((first, end));
    result
}

pub fn has_entry_label(text: &str) -> bool {
    let mut depth = 0i64;
    for token in rx(r"[{}]|\b(?:case|default)\b").find_iter(text) {
        match token.as_str() {
            "{" => depth += 1,
            "}" => depth -= 1,
            _ if depth == 0 => return true,
            _ => (),
        }
    }
    false
}

const RAW_DATA: &str =
    r"\bopword\b|\.(?:long|word|short|byte|4byte|2byte|space|incbin)\b|\bentry\b";
const ASM_OP: &str = r"\basm\s*\{\s*(?:mfmsr|mtmsr|mfspr|mtspr|mftbu|mftb|mfsr|mtsr|mfsrin|mtsrin|icbi|dcbi|sc|mtfsb0|mtfsb1|mffs|mtfsf)\b[^{};\n]*\}";

pub fn require_c(source: &str, path: &Path, preprocessed: bool) -> Result<()> {
    let failure = || {
        format!(
            "C source contains embedded assembly or binary data: {}",
            path.display()
        )
    };
    let text = rx(r"(?s)/\*.*?\*/|//[^\n]*")
        .replace_all(source, "")
        .into_owned();
    let mut text = rx(r#""(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'"#)
        .replace_all(&text, "")
        .into_owned();
    let declarations = rx(r"\basm\s+[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{");
    while let Some(capture) = declarations.captures(&text) {
        if !ASM_LEAF_FUNCTIONS.contains(&&capture[1]) {
            break;
        }
        let whole = capture.get(0).unwrap();
        let end = closing(&text, whole.end() - 1, b'{', b'}')?;
        if rx(RAW_DATA).is_match(&text[whole.end()..end]) {
            return Err(failure().into());
        }
        text.replace_range(whole.start()..end, "");
    }
    if rx(r"\basm\s*\{[^{}]*\}")
        .find_iter(&text)
        .any(|block| rx(RAW_DATA).is_match(block.as_str()))
    {
        return Err(failure().into());
    }
    if preprocessed {
        text = rx(ASM_OP).replace_all(&text, "").into_owned();
        let instruction = rx(r"\basm\s*\{\s*[a-z][a-z0-9_.]*\b[^{};\n]*\}");
        for name in ASM_LEAF_FUNCTIONS {
            let pattern = rx(&format!(r"\b{}\s*\([^;{{}}]*\)\s*\{{", regex::escape(name)));
            let matches: Vec<_> = pattern
                .find_iter(&text)
                .map(|matched| matched.end())
                .collect();
            for start in matches.into_iter().rev() {
                let end = closing(&text, start - 1, b'{', b'}')?;
                let body = instruction.replace_all(&text[start..end], "").into_owned();
                text.replace_range(start..end, &body);
            }
        }
    }
    if rx(r"\b(?:asm|__asm|__asm__|INCBIN)\b").is_match(&text) {
        return Err(failure().into());
    }
    Ok(())
}

pub fn provisional_functions(source: &str, compiled: Option<&Elf>) -> Result<HashSet<String>> {
    let clean = masked(source);
    let tokens: Vec<_> = rx(r"[A-Za-z_]\w*|[^\s]")
        .find_iter(&clean)
        .map(|token| token.as_str())
        .collect();
    let markers = ["layout_padding", "frame_padding", "PAD_STACK"];
    let token_end = |start: usize, left: &str, right: &str| -> Result<usize> {
        let mut depth = 1i64;
        let mut end = start + 1;
        while end < tokens.len() && depth != 0 {
            depth += i64::from(tokens[end] == left) - i64::from(tokens[end] == right);
            end += 1;
        }
        if depth != 0 {
            return Err("Unbalanced source while checking source workarounds.".into());
        }
        Ok(end)
    };
    let mut bodies = HashMap::new();
    let mut index = 0;
    while index + 1 < tokens.len() {
        let name = tokens[index];
        if rx(r"^[A-Za-z_]\w*$").is_match(name)
            && tokens[index + 1] == "("
            && !["if", "for", "while", "switch", "sizeof"].contains(&name)
        {
            let end = token_end(index + 1, "(", ")")?;
            if tokens.get(end) == Some(&"{") {
                let stop = token_end(end, "{", "}")?;
                bodies.insert(name, &tokens[end + 1..stop - 1]);
                index = stop;
                continue;
            }
        }
        index += 1;
    }
    let mut provisional: HashSet<String> = bodies
        .iter()
        .filter(|(_, body)| {
            body.iter().any(|token| markers.contains(token)) || has_uninitialized_self_read(body)
        })
        .map(|(&name, _)| name.to_owned())
        .collect();
    if provisional.is_empty() {
        return Ok(provisional);
    }
    let calls: HashMap<_, Vec<_>> = bodies
        .iter()
        .map(|(&name, body)| {
            (
                name,
                body.windows(2)
                    .filter_map(|pair| {
                        (bodies.contains_key(pair[0]) && pair[1] == "(").then_some(pair[0])
                    })
                    .collect(),
            )
        })
        .collect();
    let mut direct: HashMap<&str, Vec<&str>> = HashMap::new();
    if let Some(compiled) = compiled {
        for function in compiled.functions() {
            direct.insert(
                &function.name,
                compiled
                    .relocations_in(
                        function.section,
                        function.address,
                        function.address + function.size,
                    )
                    .iter()
                    .filter(|item| item.kind == 10)
                    .map(|item| compiled.symbols[item.symbol].name.as_str())
                    .collect(),
            );
        }
    }
    loop {
        let consumers: Vec<_> = calls
            .iter()
            .filter(|&(&name, targets)| {
                targets.iter().any(|target| {
                    provisional.contains(*target)
                        && targets.iter().filter(|item| *item == target).count()
                            > direct
                                .get(name)
                                .into_iter()
                                .flatten()
                                .filter(|item| *item == target)
                                .count()
                })
            })
            .map(|(&name, _)| name.to_owned())
            .collect();
        let previous = provisional.len();
        provisional.extend(consumers);
        if provisional.len() == previous {
            return Ok(provisional);
        }
    }
}

fn has_uninitialized_self_read(body: &[&str]) -> bool {
    let scalar_types = [
        "s8", "s16", "s32", "s64", "u8", "u16", "u32", "u64", "f32", "f64", "int", "float",
        "double",
    ];
    let mut assignments: HashMap<&str, usize> = HashMap::new();
    for pair in body.windows(4) {
        if pair[1] == "=" && pair[2] == pair[0] && pair[3] == ";" {
            *assignments.entry(pair[0]).or_default() += 1;
        }
    }
    let mut declarations = HashMap::new();
    let mut occurrences = HashMap::new();
    let mut depth = 0i64;
    for (index, &token) in body.iter().enumerate() {
        *occurrences.entry(token).or_insert(0) += 1;
        if depth == 0 && scalar_types.contains(&token) && body.get(index + 2) == Some(&";") {
            *declarations.entry(body[index + 1]).or_insert(0) += 1;
        }
        depth += i64::from(token == "{") - i64::from(token == "}");
    }
    assignments.iter().any(|(name, count)| {
        declarations.get(name) == Some(&1) && occurrences.get(name) == Some(&(1 + count * 2))
    })
}

const ASM_LEAF_FUNCTIONS: &[&str] = &[
    "Config24MB",
    "Config48MB",
    "DCFlushRange",
    "DCFlushRangeNoSync",
    "DCInvalidateRange",
    "DCStoreRange",
    "DCStoreRangeNoSync",
    "DCZeroRange",
    "DecrementerExceptionHandler",
    "ExternalInterruptHandler",
    "GetR2",
    "HSD_ByteCodeEval",
    "HSD_SaveContext",
    "ICInvalidateRange",
    "InitMetroTRK",
    "LCDisable",
    "LCQueueWait",
    "LCStoreBlocks",
    "OSDefaultExceptionHandler",
    "OSDisableInterrupts",
    "OSEnableInterrupts",
    "OSExceptionVector",
    "OSFillFPUContext",
    "OSGetStackPointer",
    "OSGetTime",
    "OSInitContext",
    "OSLoadContext",
    "OSRestoreInterrupts",
    "OSSaveContext",
    "OSSetCurrentContext",
    "OSSwitchFPUContext",
    "PPCHalt",
    "PSMTXConcat",
    "PSMTXCopy",
    "PSMTXIdentity",
    "PSMTXInvXpose",
    "PSMTXInverse",
    "PSMTXMultVec",
    "PSMTXMultVecArray",
    "PSMTXMultVecSR",
    "PSMTXQuat",
    "PSMTXRotAxisRad",
    "PSMTXRotRad",
    "PSMTXRotTrig",
    "PSMTXScale",
    "PSMTXTrans",
    "PSMTXTranspose",
    "PSVECAdd",
    "PSVECCrossProduct",
    "PSVECDotProduct",
    "PSVECMag",
    "PSVECNormalize",
    "PSVECScale",
    "PSVECSubtract",
    "RealMode",
    "Reset",
    "Run",
    "SystemCallVector",
    "THPInit",
    "TRKExceptionHandler",
    "TRKInterruptHandler",
    "TRKInterruptHandlerEnableInterrupts",
    "TRKLoadContext",
    "TRKRestoreExtended1Block",
    "TRKSaveExtended1Block",
    "TRKSwapAndGo",
    "TRK_flush_cache",
    "TRK_ppc_memcpy",
    "WriteMTXPS3x3from3x4",
    "WriteMTXPS4x2",
    "WriteMTXPS4x3",
    "__AICallbackStackSwitch",
    "__DBExceptionDestination",
    "__LCEnable",
    "__OSDBIntegrator",
    "__OSDBJump",
    "__OSLoadFPUContext",
    "__OSPSInit",
    "__OSSaveFPUContext",
    "__THPDecompressYUV",
    "__THPDecompressiMCURow512x448",
    "__THPDecompressiMCURow640x480",
    "__THPDecompressiMCURowNxN",
    "__THPHuffDecodeDCTCompU",
    "__THPHuffDecodeDCTCompV",
    "__THPHuffDecodeDCTCompY",
    "__TRK_vector_100",
    "__TRK_vector_1000",
    "__TRK_vector_1100",
    "__TRK_vector_1200",
    "__TRK_vector_1300",
    "__TRK_vector_1400",
    "__TRK_vector_1600",
    "__TRK_vector_1700",
    "__TRK_vector_1C00",
    "__TRK_vector_1D00",
    "__TRK_vector_1E00",
    "__TRK_vector_1F00",
    "__TRK_vector_200",
    "__TRK_vector_300",
    "__TRK_vector_400",
    "__TRK_vector_500",
    "__TRK_vector_600",
    "__TRK_vector_700",
    "__TRK_vector_800",
    "__TRK_vector_900",
    "__TRK_vector_C00",
    "__TRK_vector_D00",
    "__TRK_vector_E00",
    "__TRK_vector_F00",
    "__TRK_vector_F20",
    "__TRK_vector_F54",
    "__cvt_dbl_usll",
    "__cvt_fp2unsigned",
    "__cvt_sll_flt",
    "__div2i",
    "__div2u",
    "__flush_cache",
    "__init_hardware",
    "__init_registers",
    "__mod2i",
    "__mod2u",
    "__shl2i",
    "__shr2i",
    "__shr2u",
    "__start",
];

pub fn is_assembly_leaf(name: &str) -> bool {
    ASM_LEAF_FUNCTIONS.contains(&name)
}
