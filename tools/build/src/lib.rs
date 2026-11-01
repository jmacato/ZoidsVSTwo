pub mod compile;
pub mod disc;
pub mod dol;
pub mod elf;
pub mod link;
pub mod names;
pub mod strip;
pub mod toolchain;

use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, Box<dyn std::error::Error + Send + Sync>>;

pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("../..")
        .canonicalize()
        .expect("Repository root")
}
