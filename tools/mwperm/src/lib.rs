pub mod elf;
#[cfg(target_os = "macos")]
pub mod process;
pub mod project;
pub mod source;
pub mod verify;

pub type Result<T> = std::result::Result<T, Box<dyn std::error::Error + Send + Sync>>;
