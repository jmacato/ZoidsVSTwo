use clap::{Parser, Subcommand};
use mwperm::project;

#[derive(Parser)]
#[command(about = "Build and verify the Zoids VS. II decompilation")]
struct Cli {
    #[command(subcommand)]
    command: Operation,
}
#[derive(Subcommand)]
enum Operation {
    Verify(project::VerifyArgs),
    Build(project::BuildArgs),
}
fn main() {
    let result = match Cli::parse().command {
        Operation::Verify(args) => project::verify(&args),
        Operation::Build(args) => project::build(&args),
    };
    if let Err(error) = result {
        eprintln!("FAIL {error}");
        std::process::exit(1);
    }
}
