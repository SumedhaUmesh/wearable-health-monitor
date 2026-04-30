# Engineering overview

**Intent**, **constraints**, **what CI may claim**, and **what is out of scope**. Keep this aligned with [`CMakeLists.txt`](../CMakeLists.txt), [`tests/`](../tests/), and [`.github/workflows/ci.yml`](../.github/workflows/ci.yml).

## Purpose

Firmware for a wrist-style health monitor: **PPG heart-rate estimation** and **IMU activity** inference. The reference platform is **Zephyr** on **ARM Cortex-M3 in QEMU** so bring-up and CI do not depend on hardware.

## Repository hygiene

- Do **not** commit **Zephyr SDK** trees, **`.tar.xz` / `.tar.gz` SDK downloads**, or **`zephyr/`** checkouts; they belong on disk and in [`.gitignore`](../.gitignore) only.
- **West** fetches [`zephyr`](../west.yml) next to this app; CI does the same.

## Design principles

1. **Layered boundaries** — `IPpgDriver` / `IImuDriver`; algorithms avoid Zephyr types for host **GoogleTest**.
2. **Scheduler path** — `SensorManager` + `k_msgq`; `main` drains into `HeartRateEstimator` / `ActivityClassifier`.
3. **Observable pipeline** — log tags `[sensor]`, `[hr]`, `[act]`.

## Fixed parameters (see source for details)

| Item | Value / note |
|------|----------------|
| PPG rate (mock) | 25 Hz |
| HR filter | 0.5–4 Hz band, 2nd-order Butterworth SOS @ 25 Hz |
| Peak refractory | 300 ms |
| BPM window | 8 s |
| IMU rate (mock) | 50 Hz |
| Activity window | 2 s |

## Non-goals

- Bluetooth on QEMU CI — see [`ble_future.md`](ble_future.md).
- Measured power — [`power.md`](power.md) only.
- PhysioNet/WISDM **accuracy tables in CI** — offline artifacts under [`results/README.md`](results/README.md).

## Verification matrix

| Claim | Evidence |
|-------|----------|
| Firmware links `qemu_cortex_m3` | CI job **Firmware CI** |
| SOS impulse matches SciPy | Host test `IirSos.ImpulseMatchesScipyGolden` |
| BPM on synthetic input | `HeartRate.RoughBpmFromSinePpg` |
| Activity thresholds | `Activity.StillVsRunningByVariance` |
| QEMU smoke | Local `west build -t run` (optional CI later) |

Local:

```bash
west build -b qemu_cortex_m3 -d build .
cmake -S tests -B build-tests -G Ninja && cmake --build build-tests && ctest --test-dir build-tests
```

## Dependencies

- Zephyr revision: [`west.yml`](../west.yml).
- Toolchain: Zephyr SDK **0.16.x** (see Zephyr Getting Started).

## Related docs

| Doc | Role |
|-----|------|
| [`architecture.md`](architecture.md) | Layers |
| [`debugging.md`](debugging.md) | GDB + QEMU |
| [`power.md`](power.md) | Power strategy |
| [`ble_future.md`](ble_future.md) | BLE HR intent |
