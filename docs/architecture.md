# Architecture

This firmware is organized in layers so sensor hardware can be swapped for mocks or replay without changing algorithms or the application entrypoint.

## Layers

| Layer | Role |
|--------|------|
| **Application** (`app/`) | Startup, configuration, glue code. Keeps `main()` thin. |
| **Services** (`src/services/`) | `SensorManager`: periodic reads from `IPpgDriver` / `IImuDriver`, `k_msgq` fan-in. |
| **Algorithms** (`src/algorithms/`) | Heart rate, activity classification — Phase 3+. |
| **Drivers** (`src/drivers/`) | `IPpgDriver` / `IImuDriver` implementations (real chip vs CSV replay). |
| **HAL** (`src/hal/`) | Thin Zephyr wrappers (timers, I2C handles) to keep drivers testable. |

## Data flow (Phase 2)

PPG and IMU drivers implement `IPpgDriver` / `IImuDriver`. **`MockPpgDriver`** and **`MockImuDriver`** replay small waveforms (same numbers as [`data/ppg_sample.csv`](../data/ppg_sample.csv) and [`data/imu_sample.csv`](../data/imu_sample.csv)) so QEMU runs without a filesystem. **`Max30102Driver`** and **`ImuSkeletonDriver`** are register-map skeletons with I2C left unimplemented for a later HAL pass. **`SensorManager`** uses Zephyr `k_work_delayable` on the system workqueue to read each driver at its nominal rate and enqueue raw samples. Phase 3 algorithms will consume these queues (or a service boundary like a ring buffer) without touching driver code.

## Build system

- **West** + **CMake**: `west.yml` pins Zephyr; `CMakeLists.txt` registers sources on the Zephyr `app` library.
- **Kconfig**: `prj.conf` enables logging and C++17.

## Phase scope

- **Phase 1**: Toolchain, QEMU, CI, repo layout.
- **Phase 2** (current): Driver abstractions, mock replay, sensor manager + message queues; real I2C still stubbed.
- **Phase 3** (current): `HeartRateEstimator` (Butterworth bandpass SOS @ 25 Hz, refractory peaks, BPM window), `ActivityClassifier` (magnitude variance windows + step heuristic), GoogleTest host suite under [`tests/`](../tests/), small [`tools/validate.py`](../tools/validate.py) for CSV sanity.
- **Phase 4+**: Dataset-scale validation, plots, demo polish.
