# Zoids VS. II decompilation

A matching decompilation of Zoids VS. II for the GameCube (Japan, game ID `GZSJDA`). All 3,233 functions of the game (1,121,480 bytes) compile from the C and C++ sources in this repository to the same bytes as the original executable, and the rebuilt `main.dol` is identical to the original.

This repository contains no game data and no compilers.

## Requirements

- macOS or Linux, with Rust 1.88 or later.
- Your own copy of the game as a disc image, `game.iso` in the repository root (SHA-256 `c050d3dc9b53dae81c53b2908edc5bc6a106c9a8a2bf695a888a14a995b9f008`).
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

Use a disc image at another path with `make verify ISO=path/to/game.iso`. A complete run ends with `PASS 3233 C/C++ functions, 1121480 bytes; complete DOL reconstruction matches b5d0a39d22ea9e59716b4748c5840c0e4be27c79.`

## Layout

- `src/`: game code, Dolphin SDK, MusyX, MetroTRK and runtime sources.
- `include/`: headers.
- `tools/mwperm/`: the build and verify tool.

## License

MIT. See `LICENSE`. If you use this project's code or findings in your own work, see `NOTICE` for how to credit it.
