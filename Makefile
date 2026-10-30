ISO ?= game.iso
VERIFY_ARGS ?=
BUILD_ARGS ?=
MWPERM = build/tools/mwperm-target/release/mwperm

.PHONY: build verify mwperm

mwperm:
	cargo build --release --manifest-path tools/mwperm/Cargo.toml --target-dir build/tools/mwperm-target

build: mwperm
	$(MWPERM) build --iso "$(ISO)" $(BUILD_ARGS)

verify: mwperm
	$(MWPERM) verify --iso "$(ISO)" $(VERIFY_ARGS)
