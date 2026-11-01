use crate::{Result, root, strip, toolchain::Clang, toolchain::TARGET};
use rayon::prelude::*;
use std::{
    fs,
    path::{Path, PathBuf},
    process::Command,
    time::SystemTime,
};

pub struct Unit {
    pub source: PathBuf,
    pub object: PathBuf,
}

const PIN_GLOBALS: &str = "#pragma clang attribute push (__attribute__((annotate(\"zoids\"))), apply_to = variable(is_global))";

const CODEGEN: &[&str] = &[
    "-mcpu=750",
    "-Os",
    "-fno-pic",
    "-maix-struct-return",
    "-fsigned-char",
    "-ffreestanding",
    "-fno-builtin",
    "-fno-exceptions",
    "-fno-rtti",
    "-fno-asynchronous-unwind-tables",
    "-fno-unwind-tables",
    "-fno-strict-aliasing",
    "-fwrapv",
    "-fno-delete-null-pointer-checks",
    "-ffunction-sections",
    "-fdata-sections",
    "-fno-common",
    "-fno-use-init-array",
    "-mllvm",
    "-enable-global-merge=false",
    "-w",
    "-ferror-limit=0",
    "-Wno-error=implicit-function-declaration",
    "-Wno-error=implicit-int",
    "-Wno-error=int-conversion",
    "-Wno-error=incompatible-pointer-types",
    "-Wno-error=incompatible-function-pointer-types",
    "-Wno-error=return-type",
    "-Wno-register",
    "-Wno-c++11-narrowing",
];

fn language(cpp: bool) -> [&'static str; 3] {
    if cpp {
        ["-std=gnu++14", "c++", "c++-cpp-output"]
    } else {
        ["-std=gnu11", "c", "cpp-output"]
    }
}

fn is_cpp(source: &Path, text: &str) -> bool {
    source.extension().is_some_and(|e| e == "cpp") || text.contains("#pragma cplusplus on")
}

fn base_flags(source: &Path, cpp: bool) -> Vec<String> {
    let include = root().join("include");
    let mut flags = vec![format!("--target={TARGET}")];
    flags.extend(CODEGEN.iter().map(|s| (*s).to_owned()));
    flags.push(language(cpp)[0].to_owned());
    flags.extend([
        "-nostdinc".to_owned(),
        "-include".to_owned(),
        include
            .join("clang_compat.h")
            .to_string_lossy()
            .into_owned(),
        "-I".to_owned(),
        include.to_string_lossy().into_owned(),
        "-I".to_owned(),
        source
            .parent()
            .unwrap_or(Path::new("."))
            .to_string_lossy()
            .into_owned(),
    ]);
    flags
}

pub fn signature(clang: &Clang) -> String {
    format!(
        "{}\n{}\n{}\n{}\n{PIN_GLOBALS}\n",
        env!("CARGO_PKG_VERSION"),
        clang.path.display(),
        clang.version,
        CODEGEN.join(" ")
    )
}

fn run(clang: &Clang, arguments: &[String], source: &Path) -> Result<()> {
    let output = Command::new(&clang.path)
        .args(arguments)
        .current_dir(root())
        .output()
        .map_err(|e| format!("Cannot run {}: {e}", clang.path.display()))?;
    if !output.status.success() {
        return Err(format!(
            "{}: clang failed\n{}{}",
            source.display(),
            String::from_utf8_lossy(&output.stdout),
            String::from_utf8_lossy(&output.stderr)
        )
        .into());
    }
    Ok(())
}

fn modified(path: &Path) -> Option<SystemTime> {
    fs::metadata(path).and_then(|m| m.modified()).ok()
}

fn up_to_date(object: &Path, depfile: &Path) -> bool {
    let Some(built) = modified(object) else {
        return false;
    };
    let Ok(text) = fs::read_to_string(depfile) else {
        return false;
    };
    let text = text.replace("\\\n", " ");
    let Some((_, inputs)) = text.split_once(": ") else {
        return false;
    };
    let mut paths = Vec::new();
    let mut current = String::new();
    let mut chars = inputs.chars().peekable();
    while let Some(c) = chars.next() {
        if c == '\\' && chars.peek() == Some(&' ') {
            current.push(' ');
            chars.next();
        } else if c.is_whitespace() {
            if !current.is_empty() {
                paths.push(std::mem::take(&mut current));
            }
        } else {
            current.push(c);
        }
    }
    if !current.is_empty() {
        paths.push(current);
    }
    !paths.is_empty()
        && paths
            .iter()
            .all(|p| modified(&root().join(p)).is_some_and(|m| m <= built))
}

fn compile_unit(clang: &Clang, source: &Path, object: &Path) -> Result<()> {
    let depfile = object.with_extension("d");
    if up_to_date(object, &depfile) {
        return Ok(());
    }
    fs::create_dir_all(object.parent().ok_or("Object has no parent")?)?;
    let text = fs::read_to_string(source)?;
    let cpp = is_cpp(source, &text);
    let wrapper = object.with_extension(if cpp { "wrap.cpp" } else { "wrap.c" });
    fs::write(
        &wrapper,
        format!(
            "{PIN_GLOBALS}\n#include \"{}\"\n#pragma clang attribute pop\n",
            root().join(source).display()
        ),
    )?;
    let mut flags = base_flags(source, cpp);
    flags.extend([
        "-MMD".to_owned(),
        "-MF".to_owned(),
        depfile.to_string_lossy().into_owned(),
        "-MT".to_owned(),
        object.to_string_lossy().into_owned(),
    ]);
    if !strip::mentions_mwcc_asm(&text) {
        flags.extend([
            "-c".to_owned(),
            "-x".to_owned(),
            language(cpp)[1].to_owned(),
            wrapper.to_string_lossy().into_owned(),
            "-o".to_owned(),
            object.to_string_lossy().into_owned(),
        ]);
        return run(clang, &flags, source);
    }
    let preprocessed = object.with_extension("pp");
    flags.extend([
        "-E".to_owned(),
        "-x".to_owned(),
        language(cpp)[1].to_owned(),
        wrapper.to_string_lossy().into_owned(),
        "-o".to_owned(),
        preprocessed.to_string_lossy().into_owned(),
    ]);
    run(clang, &flags, source)?;
    let text = strip::strip_mwcc_asm(&fs::read_to_string(&preprocessed)?);
    let intermediate = object.with_extension("i");
    fs::write(&intermediate, text)?;
    let mut flags = vec![format!("--target={TARGET}")];
    flags.extend(CODEGEN.iter().map(|s| (*s).to_owned()));
    flags.extend([
        language(cpp)[0].to_owned(),
        "-x".to_owned(),
        language(cpp)[2].to_owned(),
        "-c".to_owned(),
        intermediate.to_string_lossy().into_owned(),
        "-o".to_owned(),
        object.to_string_lossy().into_owned(),
    ]);
    run(clang, &flags, source)
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

pub fn compile_all(clang: &Clang, jobs: usize) -> Result<Vec<Unit>> {
    let root = root();
    let objects = root.join("build/obj");
    fs::create_dir_all(&objects)?;
    let stamp = objects.join(".signature");
    let signature = signature(clang);
    if fs::read_to_string(&stamp).ok().as_deref() != Some(signature.as_str()) {
        fs::remove_dir_all(&objects)?;
        fs::create_dir_all(&objects)?;
        fs::write(&stamp, &signature)?;
    }
    let sources = source_paths(&root.join("src"))?;
    let pool = rayon::ThreadPoolBuilder::new().num_threads(jobs).build()?;
    let results: Vec<Result<Unit>> = pool.install(|| {
        sources
            .par_iter()
            .map(|source| {
                let relative = source.strip_prefix(&root)?;
                let object = objects
                    .join(relative.strip_prefix("src")?)
                    .with_extension(format!(
                        "{}.o",
                        source.extension().unwrap_or_default().to_string_lossy()
                    ));
                compile_unit(clang, relative, &object)?;
                Ok(Unit {
                    source: relative.to_path_buf(),
                    object,
                })
            })
            .collect()
    });
    let mut units = Vec::new();
    let mut errors = Vec::new();
    for result in results {
        match result {
            Ok(unit) => units.push(unit),
            Err(error) => errors.push(error.to_string()),
        }
    }
    if !errors.is_empty() {
        let shown: Vec<_> = errors.iter().take(20).cloned().collect();
        return Err(format!(
            "{} of {} source files failed to compile:\n{}",
            errors.len(),
            sources.len(),
            shown.join("\n")
        )
        .into());
    }
    Ok(units)
}
