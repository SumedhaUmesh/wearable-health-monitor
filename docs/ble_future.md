# BLE heart-rate service (future)

Zephyr supports standard **Heart Rate Service** (UUID `0x180D`) with **Heart Rate Measurement** (`0x2A37`). A credible port would:

1. Enable `CONFIG_BT` and a controller suited to the SoC (e.g. nRF52 softdevice controller).
2. Add a small module under `src/services/` that formats BPM as GATT notifications when `HeartRateEstimator` updates.
3. Keep GATT serialization separate from DSP so unit tests stay host-side.

The QEMU `qemu_cortex_m3` target used here has **no Bluetooth radio**; this file records intent for interviews and a hardware branch without pulling BT into the simulated CI build.
