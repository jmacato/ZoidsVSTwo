# Zoids: Battle Legends decompilation

A matching decompilation of Zoids: Battle Legends for the GameCube (game ID `GZSE70`), the US release of Zoids VS. II. All 3,243 functions of the game (1,126,796 bytes) compile from the C and C++ sources in this repository to the same bytes as the original executable, and the rebuilt `main.dol` matches it. The `main` branch holds the Japanese Zoids VS. II version.

This repository contains no game data and no compilers.

## Requirements

- macOS or Linux, with Rust 1.88 or later.
- Your own copy of the game as a disc image, `game.iso` in the repository root (SHA-256 `22f0351430ab95ce75f7d90ebede70fffd2d9a4772de3af81bd301fb1455917c`).
- Your own copy of the Metrowerks CodeWarrior for GameCube compilers, versions 1.2.5, 1.2.5n and 1.1p1.

Place each compiler, as an executable your system can run, at:

| Version | Path |
|---|---|
| 1.2.5 | `build/tools/mwcceppc-native` |
| 1.2.5n | `build/tools/mwcceppc-1.2.5n-native` |
| 1.1p1 | `build/tools/mwcceppc-1.1p1-native` |

To keep them elsewhere, pass `--compiler PATH` or set `MWCC=PATH` for the 1.2.5 compiler; the other two are looked up in the same directory under the names above.

## Usage

```sh
make verify   # compile every source file and compare each function with the original
make build    # write build/main.dol and check its SHA-1
```

Use a disc image at another path with `make verify ISO=path/to/game.iso`. A complete run ends with `PASS 3243 C/C++ functions, 1126796 bytes; complete DOL reconstruction matches 873470a8dd7a38e22dbabb878d3dd11b3862375f.`

## Layout

- `src/`: game code, Dolphin SDK, MusyX, MetroTRK and runtime sources.
- `include/`: headers.
- `tools/mwperm/`: the build and verify tool.

## License

MIT. See `LICENSE`. If you use this project's code or findings in your own work, see `NOTICE` for how to credit it.
