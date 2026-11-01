use clap::Parser;
use std::{
    fs,
    path::{Path, PathBuf},
    time::Instant,
};
use zoids_build::{Result, compile, disc, elf::Elf, link, root, toolchain};

#[derive(Parser)]
#[command(about = "Build the Zoids: Battle Legends decompilation with clang")]
struct Cli {
    /// Disc image; defaults to game.iso (or the only .iso/.gcm) in the repository root
    #[arg(long)]
    iso: Option<PathBuf>,
    /// Output executable
    #[arg(long, default_value = "build/main.dol")]
    output: PathBuf,
    /// clang to use; defaults to $CLANG or the first clang with a PowerPC backend
    #[arg(long)]
    clang: Option<PathBuf>,
    #[arg(long, default_value_t = std::thread::available_parallelism().map_or(1, usize::from))]
    jobs: usize,
}

fn find_iso(root: &Path) -> Option<PathBuf> {
    let default = root.join("game.iso");
    if default.is_file() {
        return Some(default);
    }
    let images: Vec<_> = fs::read_dir(root)
        .ok()?
        .flatten()
        .map(|e| e.path())
        .filter(|p| {
            p.extension()
                .is_some_and(|e| e.eq_ignore_ascii_case("iso") || e.eq_ignore_ascii_case("gcm"))
        })
        .collect();
    (images.len() == 1).then(|| images[0].clone())
}

fn run(cli: Cli) -> Result<()> {
    let started = Instant::now();
    let root = root();
    let iso = cli.iso.or_else(|| find_iso(&root));
    let (dol, elf) = disc::cached_reference(iso.as_deref())?;
    let clang = toolchain::find(cli.clang.as_deref())?;
    println!("Using {} ({})", clang.path.display(), clang.version);
    let units = compile::compile_all(&clang, cli.jobs)?;
    println!("Compiled {} source files.", units.len());
    let (output, report) = link::link(&dol, Elf::parse(elf)?, &units)?;
    let path = root.join(&cli.output);
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent)?;
    }
    fs::write(&path, &output)?;
    let map = path.with_extension("map");
    fs::write(&map, &report.map)?;
    println!(
        "Linked {} compiled functions and {} original MWCC assembly functions (listed in {}): {} of {} KiB code, {} KiB data at {:08X}.",
        report.compiled,
        report.original.len(),
        map.display(),
        report.code_size / 1024,
        report.code_limit / 1024,
        report.data_size / 1024,
        report.data_address
    );
    println!(
        "Wrote {} in {:.1}s.",
        path.display(),
        started.elapsed().as_secs_f64()
    );
    Ok(())
}

fn main() {
    if let Err(error) = run(Cli::parse()) {
        eprintln!("error: {error}");
        std::process::exit(1);
    }
}
