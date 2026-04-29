# Power strategy (future hardware)

This simulation build does not measure current. On a real wrist device you would align software structure with hardware constraints early:

1. **Wake frequency** — batch sensor reads via FIFO interrupts instead of polling at full rate when the UI is idle.
2. **Sleep policy** — between algorithm windows, call Zephyr `k_sleep()` / tickless idle with wake sources from IMU motion interrupt or timer.
3. **Radio** — BLE advertising and connection intervals dominate consumption; narrow bandwidth when HR is stable (trade latency vs battery).
4. **Verification** — Nordic-style power profiler or ammeter on the 3.3 V rail during scripted scenarios (still vs streaming).

Nothing here replaces measurement; it documents where hooks would live relative to this repo’s layered design.
