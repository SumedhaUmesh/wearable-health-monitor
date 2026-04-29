# Phase 1 checklist

Use this before calling Phase 1 “done” or merging documentation-only PRs.

## Repository

- [ ] `west.yml` pins a known Zephyr revision (currently **v3.7.0**).
- [ ] `.gitignore` excludes `.west/`, `zephyr/`, `modules/`, `build/`, `.venv/`.
- [ ] No accidental commits of Zephyr checkout or build artifacts.

## Local build and run

Run from the repository root after `west init -l .` and `west update`:

```bash
west build -b qemu_cortex_m3 -d build .
west build -d build -t run
```

- [ ] Build completes without errors.
- [ ] QEMU console shows a log line containing **Hello.**

## Debug

- [ ] `west build -d build -t debugserver` then GDB attach works (see [debugging.md](debugging.md)).
- [ ] Breakpoint on `main` is hit.

## CI

- [ ] GitHub Actions workflow **Firmware CI** passes on `main` / `develop`.
- [ ] Workflow badge in README matches the real repo path.

## Documentation

- [ ] README build/run steps were followed once on a clean machine (or CI substitutes for Linux).
- [ ] Architecture diagram (Mermaid in README) matches intended layering.

## Stop line for Phase 1

Do **not** implement sensor replay, drivers beyond stubs, or algorithms until Phase 2/3 — keep Phase 1 boundaries clear for reviewers.
