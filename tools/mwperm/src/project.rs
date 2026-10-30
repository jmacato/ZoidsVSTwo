use crate::{
    Result,
    elf::{Elf, Symbol, bytes, u32be},
    source, verify,
};
use clap::Args;
use rayon::prelude::*;
use sha1::{Digest, Sha1};
use std::{
    collections::BTreeSet,
    fs,
    io::Read,
    path::{Path, PathBuf},
};

pub const DOL_SHA1: &str = "b5d0a39d22ea9e59716b4748c5840c0e4be27c79";
pub const ELF_SHA1: &str = "b3e0c300afa9b36ca8b8e6e7466cca83c75a630c";
pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("../..")
        .canonicalize()
        .expect("Repository root")
}
pub fn jobs() -> usize {
    std::thread::available_parallelism().map_or(1, usize::from)
}
pub fn compiler_name(version: &str) -> Result<&'static str> {
    match version {
        "1.2.5" => Ok("mwcceppc-native"),
        "1.2.5n" => Ok("mwcceppc-1.2.5n-native"),
        "1.1p1" => Ok("mwcceppc-1.1p1-native"),
        _ => Err(format!("Unsupported compiler version: {version}").into()),
    }
}
pub fn toolchain(compiler: Option<&Path>, version: &str) -> Result<PathBuf> {
    use std::os::unix::fs::PermissionsExt;
    let path = compiler
        .map(Path::to_path_buf)
        .or_else(|| std::env::var_os("MWCC").map(PathBuf::from))
        .unwrap_or(root().join("build/tools").join(compiler_name(version)?));
    if path
        .extension()
        .is_some_and(|s| s.eq_ignore_ascii_case("exe"))
    {
        return Err("Use a native MWCC executable for --compiler or MWCC.".into());
    }
    if !path.is_file() || path.metadata()?.permissions().mode() & 0o111 == 0 {
        return Err(format!("Native MWCC {version} is unavailable: {}", path.display()).into());
    }
    Ok(path.canonicalize()?)
}
pub fn source_path(path: &Path) -> Result<PathBuf> {
    let absolute = std::path::absolute(path)?;
    match absolute.canonicalize() {
        Ok(path) => Ok(path),
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => {
            let parent = absolute.parent().ok_or("Source path has no parent")?;
            let name = absolute.file_name().ok_or("Source path has no filename")?;
            Ok(source_path(parent)?.join(name))
        }
        Err(error) => Err(error.into()),
    }
}

pub fn compiler_for_source(path: &Path, compiler: &Path) -> Result<PathBuf> {
    let absolute = source_path(path)?;
    let root = root();
    let Ok(relative) = absolute.strip_prefix(&root) else {
        return Ok(compiler.to_owned());
    };
    let name = relative.to_string_lossy();
    let version = if name.starts_with("src/sdk/trk/") {
        "1.1p1"
    } else if [
        "gx", "dsp", "ai", "ar", "exi", "si", "pad", "dvd", "card", "vi", "musyx",
    ]
    .iter()
    .any(|part| name.starts_with(&format!("src/sdk/{part}/")))
        || name == "src/runtime/bootstrap.c"
        || [
            "alarm",
            "interrupt",
            "message",
            "memory_protection",
            "mutex",
            "reboot",
            "thread",
            "time",
            "kernel",
            "cache",
            "error",
            "font",
            "sram",
            "reset_switch",
            "audio_system",
        ]
        .iter()
        .any(|file| name == format!("src/sdk/os/{file}.c"))
    {
        "1.2.5n"
    } else {
        return Ok(compiler.to_owned());
    };
    toolchain(
        Some(
            &compiler
                .parent()
                .ok_or("Compiler has no parent")?
                .join(compiler_name(version)?),
        ),
        version,
    )
}
pub fn flags_for_source(path: &Path) -> Result<Vec<String>> {
    let mut flags: Vec<String> = [
        "-c",
        "-O4,p",
        "-inline",
        "auto",
        "-fp",
        "hardware",
        "-fp_contract",
        "on",
        "-enum",
        "int",
        "-proc",
        "gekko",
        "-Cpp_exceptions",
        "off",
        "-RTTI",
        "off",
        "-nodefaults",
        "-nosyspath",
        "-align",
        "powerpc",
        "-i",
    ]
    .into_iter()
    .map(str::to_owned)
    .collect();
    flags.push(root().join("include").to_string_lossy().into_owned());
    let absolute = source_path(path)?;
    let root = root();
    let Ok(relative) = absolute.strip_prefix(&root) else {
        return Ok(flags);
    };
    let name = relative.to_string_lossy();
    if name.starts_with("src/runtime/math/") && path.extension().is_some_and(|s| s == "c") {
        flags[3] = "off".into();
    } else if name.starts_with("src/")
        && !name.starts_with("src/sdk/")
        && !name.starts_with("src/runtime/")
        && ![
            "src/camera/beSide.c",
            "src/thp/audio_codec.c",
            "src/thp/thp_decompress.c",
            "src/thp/video_tables.c",
            "src/zoid/resolveIKJoint1.c",
        ]
        .contains(&name.as_ref())
    {
        flags.extend(["-opt".into(), "level=0,schedule,functions,peephole".into()]);
    } else if name.starts_with("src/sdk/trk/") {
        flags[3] = "deferred".into();
        flags.extend(
            ["-sdata", "0", "-sdata2", "0"]
                .into_iter()
                .map(str::to_owned),
        );
        if path
            .file_name()
            .is_some_and(|s| s == "targimpl.c" || s == "dolphin_trk_glue.c")
        {
            flags.extend(["-opt".into(), "nopeephole".into()]);
        }
    }
    Ok(flags)
}
pub fn source_function(path: &Path, reference: &Elf) -> Result<Option<Symbol>> {
    let name = path
        .file_stem()
        .ok_or("Source has no filename")?
        .to_string_lossy();
    let capture = source::rx(r"^(.+)__([0-9A-Fa-f]{8})$");
    let suffix = capture.captures(&name);
    let (name, address) = if let Some(ref suffix) = suffix {
        (&suffix[1], Some(u32::from_str_radix(&suffix[2], 16)?))
    } else {
        (name.as_ref(), None)
    };
    source::require_c(&fs::read_to_string(path)?, path, false)?;
    let functions: Vec<_> = reference
        .functions_named(name)
        .into_iter()
        .filter(|s| address.is_none_or(|a| s.address == a))
        .collect();
    if functions.is_empty() && address.is_none() {
        return Ok(None);
    }
    if functions.len() != 1 {
        return Err(format!("Source has no unique original function: {}", path.display()).into());
    }
    Ok(Some(functions[0].clone()))
}
pub fn reference_hash(data: &[u8], expected: &str, label: &str) -> Result<()> {
    if format!("{:x}", Sha1::digest(data)) != expected {
        return Err(format!("{label} hash does not match the Japanese Zoids VS. II disc.").into());
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
pub fn reconstruct(dol: &[u8], reference: &Elf) -> Result<Vec<u8>> {
    let mut output = vec![0; dol.len()];
    output
        .get_mut(..256)
        .ok_or("Truncated DOL")?
        .copy_from_slice(bytes(dol, 0, 256)?);
    for (offset, address, size) in dol_sections(dol)? {
        let sections: Vec<_> = reference
            .sections
            .iter()
            .filter(|s| s.kind == 1 && s.address == address)
            .collect();
        if sections.len() != 1 || sections[0].size > size {
            return Err(format!("ELF section does not fit DOL address {address:08X}.").into());
        }
        let data = reference.section_data(sections[0])?;
        output
            .get_mut(offset as usize..offset as usize + data.len())
            .ok_or("DOL section exceeds file")?
            .copy_from_slice(data);
    }
    if output != dol {
        return Err("ELF reconstruction differs from the original DOL.".into());
    }
    Ok(output)
}
pub fn place(output: &mut [u8], dol: &[u8], function: &Symbol, data: &[u8]) -> Result<()> {
    for (offset, address, size) in dol_sections(dol)? {
        if address <= function.address
            && u64::from(function.address) + u64::from(function.size)
                <= u64::from(address) + u64::from(size)
        {
            let start = offset as usize + (function.address - address) as usize;
            output
                .get_mut(start..start + data.len())
                .ok_or("Replacement exceeds DOL")?
                .copy_from_slice(data);
            return Ok(());
        }
    }
    Err(format!("Function has no DOL section: {}", function.name).into())
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
    if &data[..6] != b"GZSJDA" {
        return Err("Expected the GZSJDA GameCube disc.".into());
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
pub fn cached_reference(iso: &Path) -> Result<(Vec<u8>, Vec<u8>)> {
    let cache = root().join("build/reference");
    if let (Ok(dol), Ok(elf)) = (
        fs::read(cache.join("main.dol")),
        fs::read(cache.join("main.elf")),
    ) && reference_hash(&dol, DOL_SHA1, "DOL").is_ok()
        && reference_hash(&elf, ELF_SHA1, "ELF").is_ok()
    {
        return Ok((dol, elf));
    }
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
pub fn source_paths(directory: &Path) -> Result<Vec<PathBuf>> {
    fn visit(directory: &Path, paths: &mut Vec<PathBuf>) -> Result<()> {
        for entry in fs::read_dir(directory)? {
            let entry = entry?;
            let path = entry.path();
            if entry.file_type()?.is_dir() {
                visit(&path, paths)?;
            } else if path.extension().is_some_and(|e| e == "c" || e == "cpp") {
                paths.push(path);
            }
        }
        Ok(())
    }
    let mut paths = Vec::new();
    visit(directory, &mut paths)?;
    paths.sort();
    Ok(paths)
}
#[derive(Debug, serde::Deserialize)]
pub struct Output {
    pub returncode: i32,
    pub stdout: Vec<u8>,
    pub stderr: Vec<u8>,
    pub timed_out: bool,
}
pub fn run(compiler: &Path, arguments: &[String], timeout_ms: u64) -> Result<Output> {
    #[cfg(target_os = "macos")]
    {
        let request = serde_json::from_value(
            serde_json::json!({"program":compiler,"arguments":arguments,"root":root(),"timeout_ms":timeout_ms}),
        )?;
        Ok(serde_json::from_value(crate::process::run(request)?)?)
    }
    #[cfg(not(target_os = "macos"))]
    {
        use std::time::{Duration, Instant};
        let mut stdout = tempfile::tempfile()?;
        let mut stderr = tempfile::tempfile()?;
        let mut child = std::process::Command::new(compiler)
            .args(arguments)
            .current_dir(root())
            .stdout(stdout.try_clone()?)
            .stderr(stderr.try_clone()?)
            .spawn()?;
        let started = Instant::now();
        let mut timed_out = false;
        let status = loop {
            if let Some(status) = child.try_wait()? {
                break status;
            }
            if started.elapsed() >= Duration::from_millis(timeout_ms) {
                timed_out = true;
                child.kill()?;
                break child.wait()?;
            }
            std::thread::sleep(Duration::from_millis(1));
        };
        use std::io::{Seek, SeekFrom};
        use std::os::unix::process::ExitStatusExt;
        stdout.seek(SeekFrom::Start(0))?;
        stderr.seek(SeekFrom::Start(0))?;
        let mut out = Vec::new();
        let mut err = Vec::new();
        stdout.read_to_end(&mut out)?;
        stderr.read_to_end(&mut err)?;
        Ok(Output {
            returncode: status
                .code()
                .unwrap_or_else(|| -status.signal().unwrap_or(1)),
            stdout: out,
            stderr: err,
            timed_out,
        })
    }
}
pub fn compile(
    path: &Path,
    canonical: &Path,
    compiler: &Path,
    output: &Path,
    preprocess: bool,
) -> Result<Output> {
    let mut flags = flags_for_source(canonical)?;
    if preprocess {
        flags[0] = "-E".into();
    }
    flags.extend([
        "-i".into(),
        canonical
            .parent()
            .ok_or("Missing source parent")?
            .to_string_lossy()
            .into_owned(),
        "-i".into(),
        path.parent()
            .ok_or("Missing candidate parent")?
            .to_string_lossy()
            .into_owned(),
        path.to_string_lossy().into_owned(),
        "-o".into(),
        output.to_string_lossy().into_owned(),
    ]);
    let result = run(compiler, &flags, 60_000)?;
    if result.returncode != 0 || result.timed_out {
        return Err(format!(
            "{}: {} failed\n{}{}",
            path.display(),
            if preprocess {
                "preprocessing"
            } else {
                "compiler"
            },
            String::from_utf8_lossy(&result.stdout),
            String::from_utf8_lossy(&result.stderr)
        )
        .into());
    }
    if preprocess {
        source::require_c(&String::from_utf8_lossy(&fs::read(output)?), path, true)?;
    }
    Ok(result)
}
pub fn compile_function(
    path: &Path,
    function: Option<&Symbol>,
    reference: &Elf,
    compiler: &Path,
    work: &Path,
) -> Result<verify::Matches> {
    let compiler = compiler_for_source(path, compiler)?;
    compile(path, path, &compiler, &work.join("function.i"), true)?;
    compile(path, path, &compiler, &work.join("function.o"), false)?;
    verify::verify_object(&Elf::read(work.join("function.o"))?, reference, function)
}
#[derive(Args, Clone, Debug)]
pub struct ReferenceArgs {
    #[arg(long, default_value = "game.iso")]
    pub iso: PathBuf,
    #[arg(long)]
    pub compiler: Option<PathBuf>,
    #[arg(long,default_value_t=jobs())]
    pub jobs: usize,
}
impl ReferenceArgs {
    pub fn read(&self) -> Result<(Vec<u8>, Elf)> {
        let (dol, elf) = cached_reference(&self.iso)?;
        reference_hash(&dol, DOL_SHA1, "DOL")?;
        reference_hash(&elf, ELF_SHA1, "ELF")?;
        Ok((dol, Elf::parse(elf)?))
    }
}
#[derive(Args, Clone, Debug)]
pub struct VerifyArgs {
    #[command(flatten)]
    pub reference: ReferenceArgs,
}
pub fn verify(args: &VerifyArgs) -> Result<()> {
    let (dol, reference) = args.reference.read()?;
    let mut output = reconstruct(&dol, &reference)?;
    let inventory: Vec<_> = reference.functions().collect();
    let paths = source_paths(&root().join("src"))?;
    let compiler = toolchain(args.reference.compiler.as_deref(), "1.2.5")?;
    let work = tempfile::Builder::new()
        .prefix("zoids-vs-verify-")
        .tempdir()?;
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(args.reference.jobs)
        .build()?;
    let results: Vec<Result<_>> = pool.install(|| {
        paths
            .par_iter()
            .enumerate()
            .map(|(index, path)| {
                let primary = source_function(path, &reference)?;
                let unit = work.path().join(index.to_string());
                fs::create_dir(&unit)?;
                let matched =
                    compile_function(path, primary.as_ref(), &reference, &compiler, &unit)
                        .map_err(|e| format!("{}: {e}", path.display()))?;
                let pending = source::provisional_functions(
                    &fs::read_to_string(path)?,
                    Some(&Elf::read(unit.join("function.o"))?),
                )?;
                Ok((matched, pending))
            })
            .collect()
    });
    let mut sources = Vec::new();
    let mut provisional = BTreeSet::new();
    for (path, result) in paths.iter().zip(results) {
        let (matched, pending) = result?;
        for (function, data) in matched {
            if pending.contains(&function.name) {
                provisional.insert(function.clone());
            }
            sources.push((path, function, data));
        }
    }
    let mut ordered: Vec<_> = sources.iter().collect();
    ordered.sort_by_key(|(_, f, _)| f.address);
    let mut covered = BTreeSet::new();
    let mut end = 0;
    for (_, function, _) in ordered {
        if !covered.insert(function.address) {
            return Err(format!("Duplicate source region: {}", function.name).into());
        }
        if function.address < end {
            return Err(format!("Overlapping source regions: {}", function.name).into());
        }
        end = function.address + function.size;
    }
    let recovered: BTreeSet<_> = sources
        .iter()
        .filter(|(_, f, _)| f.kind() != 2)
        .map(|(_, f, _)| (f.address, f.address + f.size))
        .collect();
    let unclassified: Vec<_> = verify::unclassified_executable_ranges(&reference)
        .into_iter()
        .filter(|(_, start, end)| !recovered.contains(&(*start, *end)))
        .collect();
    let missing: Vec<_> = inventory
        .iter()
        .copied()
        .filter(|f| !covered.contains(&f.address) || provisional.contains(f))
        .collect();
    for (path, function, data) in &sources {
        place(&mut output, &dol, function, data)?;
        println!(
            "{} {:08X} {:5} {}",
            if provisional.contains(function) {
                "PROVISIONAL"
            } else {
                "PASS"
            },
            function.address,
            function.size,
            path.display()
        );
    }
    if output != dol {
        return Err("The reconstructed DOL differs after source replacements.".into());
    }
    if !missing.is_empty() {
        return Err(format!(
            "C coverage is incomplete: {} named functions remain.",
            missing.len()
        )
        .into());
    }
    if !unclassified.is_empty() {
        return Err(
            "Executable-section coverage is incomplete: unclassified ranges remain.".into(),
        );
    }
    let verified: Vec<_> = sources.iter().filter(|(_, f, _)| f.kind() == 2).collect();
    let matched_bytes: u32 = verified.iter().map(|(_, f, _)| f.size).sum();
    println!(
        "PASS {} C/C++ functions, {matched_bytes} bytes; complete DOL reconstruction matches {DOL_SHA1}.",
        verified.len()
    );
    Ok(())
}
#[derive(Args, Clone, Debug)]
pub struct BuildArgs {
    #[command(flatten)]
    pub reference: ReferenceArgs,
    #[arg(long, default_value = "build/main.dol")]
    pub output: PathBuf,
}
pub fn build(args: &BuildArgs) -> Result<()> {
    let (dol, reference) = args.reference.read()?;
    let compiler = toolchain(args.reference.compiler.as_deref(), "1.2.5")?;
    let paths = source_paths(&root().join("src"))?;
    println!(
        "Compiling {} source files with {} jobs.",
        paths.len(),
        args.reference.jobs
    );
    let pool = rayon::ThreadPoolBuilder::new()
        .num_threads(args.reference.jobs)
        .build()?;
    let objects: Vec<Result<_>> = pool.install(|| {
        paths
            .par_iter()
            .map(|path| {
                let output = root()
                    .join("build/obj")
                    .join(path.strip_prefix(root().join("src"))?)
                    .with_extension("o");
                fs::create_dir_all(output.parent().ok_or("Object has no parent")?)?;
                let compiler = compiler_for_source(path, &compiler)?;
                compile(path, path, &compiler, &output, false)?;
                Ok(output)
            })
            .collect()
    });
    let mut regions = Vec::new();
    for (path, obj) in paths.iter().zip(objects) {
        let primary = source_function(path, &reference)?;
        let compiled = Elf::read(obj?)?;
        for (target, data) in verify::verify_object(&compiled, &reference, primary.as_ref())? {
            regions.push((target, data));
        }
    }
    write_build(&args.output, &dol, &reference, &regions)?;
    println!(
        "Wrote {} from {} compiled functions, SHA-1 {DOL_SHA1}.",
        args.output.display(),
        regions.len()
    );
    Ok(())
}
pub fn write_build(
    path: &Path,
    dol: &[u8],
    reference: &Elf,
    regions: &[(Symbol, Vec<u8>)],
) -> Result<()> {
    let mut output = reconstruct(dol, reference)?;
    for function in reference.functions() {
        place(&mut output, dol, function, &vec![0; function.size as usize])?;
    }
    let mut covered = BTreeSet::new();
    for (target, data) in regions {
        if !covered.insert(target.address) {
            return Err(format!("Duplicate compiled function: {}", target.name).into());
        }
        place(&mut output, dol, target, data)?;
    }
    let missing = reference
        .functions()
        .filter(|f| !covered.contains(&f.address))
        .count();
    if missing > 0 {
        return Err(
            format!("Build is incomplete: {missing} functions have no compiled source.").into(),
        );
    }
    reference_hash(&output, DOL_SHA1, "main.dol")?;
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent)?;
    }
    fs::write(path, &output)?;
    Ok(())
}
