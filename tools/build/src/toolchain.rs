use crate::Result;
use std::{
    fs,
    path::{Path, PathBuf},
    process::{Command, Stdio},
};

pub const TARGET: &str = "powerpc-unknown-eabi";

pub struct Clang {
    pub path: PathBuf,
    pub version: String,
}

fn candidates() -> Vec<PathBuf> {
    let mut list = Vec::new();
    if let Some(path) = std::env::var_os("CLANG") {
        list.push(PathBuf::from(path));
    }
    let mut versioned: Vec<(u32, PathBuf)> = Vec::new();
    for directory in std::env::var_os("PATH")
        .map(|p| std::env::split_paths(&p).collect::<Vec<_>>())
        .unwrap_or_default()
    {
        list.push(directory.join("clang"));
        if let Ok(entries) = fs::read_dir(&directory) {
            for entry in entries.flatten() {
                let name = entry.file_name().to_string_lossy().into_owned();
                if let Some(version) = name.strip_prefix("clang-")
                    && let Ok(version) = version.parse()
                {
                    versioned.push((version, entry.path()));
                }
            }
        }
    }
    for prefix in ["/opt/homebrew/opt", "/usr/local/opt"] {
        list.push(Path::new(prefix).join("llvm/bin/clang"));
        if let Ok(entries) = fs::read_dir(prefix) {
            for entry in entries.flatten() {
                let name = entry.file_name().to_string_lossy().into_owned();
                if let Some(version) = name.strip_prefix("llvm@")
                    && let Ok(version) = version.parse()
                {
                    versioned.push((version, entry.path().join("bin/clang")));
                }
            }
        }
    }
    for prefix in ["/usr/lib", "/usr/lib64"] {
        if let Ok(entries) = fs::read_dir(prefix) {
            for entry in entries.flatten() {
                let name = entry.file_name().to_string_lossy().into_owned();
                if let Some(version) = name.strip_prefix("llvm-").or(name.strip_prefix("llvm"))
                    && let Ok(version) = version.parse()
                {
                    versioned.push((version, entry.path().join("bin/clang")));
                }
            }
        }
    }
    versioned.sort_by_key(|entry| std::cmp::Reverse(entry.0));
    list.extend(versioned.into_iter().map(|(_, path)| path));
    list
}

fn probe(path: &Path) -> Option<String> {
    if !path.is_file() {
        return None;
    }
    let status = Command::new(path)
        .args([
            format!("--target={TARGET}").as_str(),
            "-mcpu=750",
            "-c",
            "-x",
            "c",
            "-",
            "-o",
            if cfg!(windows) { "NUL" } else { "/dev/null" },
        ])
        .stdin(Stdio::null())
        .stdout(Stdio::null())
        .stderr(Stdio::null())
        .status()
        .ok()?;
    if !status.success() {
        return None;
    }
    let output = Command::new(path).arg("--version").output().ok()?;
    Some(
        String::from_utf8_lossy(&output.stdout)
            .lines()
            .next()
            .unwrap_or_default()
            .to_owned(),
    )
}

pub fn find(explicit: Option<&Path>) -> Result<Clang> {
    let list = match explicit {
        Some(path) => vec![path.to_path_buf()],
        None => candidates(),
    };
    for path in list {
        if let Some(version) = probe(&path) {
            return Ok(Clang { path, version });
        }
    }
    Err(concat!(
        "No clang with the PowerPC backend was found.\n",
        "Install one, then run cargo run again:\n",
        "  macOS:          brew install llvm   (Apple's /usr/bin/clang has no PowerPC backend)\n",
        "  Debian/Ubuntu:  sudo apt install clang\n",
        "  Fedora:         sudo dnf install clang\n",
        "  Arch:           sudo pacman -S clang\n",
        "Or point CLANG=/path/to/clang at a clang that supports --target=powerpc-unknown-eabi."
    )
    .into())
}
