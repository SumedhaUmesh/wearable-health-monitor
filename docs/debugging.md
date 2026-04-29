# Debugging (QEMU + GDB)

Phase 1 runs on the `qemu_cortex_m3` board. Logs go to the QEMU UART; GDB attaches to QEMU’s remote stub.

## Prerequisites

- Firmware built successfully: `west build -b qemu_cortex_m3 -d build .`
- ARM GDB from the Zephyr SDK on `PATH`, typically `arm-zephyr-eabi-gdb` (installed with the SDK used for this project).

## Two-terminal workflow

**Terminal A — start GDB server (QEMU paused)**

From the repository root (same place you run `west build`):

```bash
west build -d build -t debugserver
```

This starts QEMU with the firmware and listens for GDB (default TCP port **1234** on localhost).

**Terminal B — attach GDB**

```bash
arm-zephyr-eabi-gdb build/zephyr/zephyr.elf
```

Inside GDB:

```text
target remote localhost:1234
break main
continue
```

Step with `step` / `next`, backtrace with `bt`, inspect locals as usual.

To exit GDB without leaving QEMU confused, prefer `quit` from GDB and stop Terminal A with Ctrl+C when you are done.

## If `arm-zephyr-eabi-gdb` is not found

Use the exact GDB shipped with your Zephyr SDK, for example (adjust SDK path):

```bash
~/zephyr-sdk-0.16.8/arm-zephyr-eabi/bin/arm-zephyr-eabi-gdb build/zephyr/zephyr.elf
```

## RTT-style logging (later)

On hardware, many teams route structured logs through SEGGER RTT instead of UART. Here we use Zephyr logging to the console for QEMU; the same `LOG_*` calls remain when you add an RTT backend later.

## Common issues

| Symptom | What to check |
|---------|----------------|
| `Connection refused` on `:1234` | Run `debugserver` first; ensure no other QEMU/GDB server holds the port. |
| Breakpoints never hit | Confirm you attached to `zephyr.elf` from this build tree (`build/zephyr/zephyr.elf`). |
| Wrong source lines | Rebuild after edits (`west build`); stale ELF paths confuse GDB. |
