# Zoids: Battle Legends decompilation, clang build

This branch builds the Zoids: Battle Legends (GameCube, `GZSE70`) decompilation with a stock clang instead of the Metrowerks CodeWarrior compilers. The result, `build/main.dol`, is a non-matching executable: the code is compiled by clang, so its bytes differ from the original. The `battle-legends` branch keeps the byte-matching MWCC build, and `main` holds the Japanese Zoids VS. II version.

This repository contains no game data and no compilers.

## Requirements

- macOS or Linux, with Rust 1.88 or later.
- clang with the PowerPC backend. Any LLVM from 14 on works (tested with 14, 17, 20 and 23). No separate linker is needed.
  - macOS: `brew install llvm`. Apple's `/usr/bin/clang` has no PowerPC backend.
  - Debian or Ubuntu: `sudo apt install clang`. Fedora: `sudo dnf install clang`. Arch: `sudo pacman -S clang`.
- Your own copy of the game as a disc image, `game.iso` in the repository root (SHA-256 `22f0351430ab95ce75f7d90ebede70fffd2d9a4772de3af81bd301fb1455917c`). After the first build the tool keeps what it needs in `build/reference/`, and the image is no longer read.

## Build

```sh
cargo run
```

This writes `build/main.dol` and a symbol map, `build/main.map`. A clean build takes a few seconds; later builds only recompile changed files.

Options, passed after `--` (for example `cargo run -- --iso ~/games/zoids.iso`):

| Option | Default |
|---|---|
| `--iso PATH` | `game.iso`, or the only `.iso`/`.gcm` file in the repository root |
| `--clang PATH` | `$CLANG`, then the first clang on `PATH` or in the usual LLVM install folders that can target PowerPC |
| `--output PATH` | `build/main.dol` |
| `--jobs N` | number of CPU cores |

## How the build works

The disc contains `zoidR.elf`, the fully linked game with its symbol table and relocations. The tool in `tools/build/` uses it as follows:

1. Every C and C++ file in `src/` is compiled with clang for the Gekko CPU (`--target=powerpc-unknown-eabi -mcpu=750 -Os`), with the struct-return convention of the Metrowerks EABI.
2. The tool links the objects itself. The original data sections stay at their original addresses, and variables defined in the sources resolve to those addresses. Clang code goes into the original `.text` area. Pointers to code in the original data (callback tables, `.ctors`) are repointed at the new functions with the relocations from `zoidR.elf`.
3. Functions written as Metrowerks inline assembly cannot be compiled by clang. There are 96 of them: OS context switching and exception handlers, cache control, paired-single matrix math, the THP video decoder and the MetroTRK debugger. The tool copies these from the original executable and relocates them. They are marked `original` in `build/main.map`.
4. Data that exists only in the clang build (string literals, constants, a few function-local statics, about 41 KiB) is placed at the original start of the heap, and `__ArenaLo` moves up by the same amount.

The build has not yet been boot-tested on hardware or in an emulator.

## Layout

- `src/`: game code, Dolphin SDK, MusyX, MetroTRK and runtime sources.
- `include/`: headers. `include/clang_compat.h` maps the Metrowerks intrinsics to clang.
- `tools/build/`: the build tool and linker.

## License

MIT. See `LICENSE`. If you use this project's code or findings in your own work, see `NOTICE` for how to credit it.
