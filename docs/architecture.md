# Architecture

See [**engineering_overview.md**](engineering_overview.md) for assumptions, non-goals, and what CI proves.

This firmware is organized in layers so sensor hardware can be swapped for mocks or replay without changing algorithms or the application entrypoint.

## Layers

| Layer | Role |
|--------|------|
| **Application** (`app/`) | Startup, configuration, glue code. Keeps `main()` thin. |
| **Services** (`src/services/`) | `SensorManager`: periodic reads from `IPpgDriver` / `IImuDriver`, `k_msgq` fan-in. |
| **Algorithms** (`src/algorithms/`) | `HeartRateEstimator`, `ActivityClassifier` (host-tested DSP core). |
| **Drivers** (`src/drivers/`) | `IPpgDriver` / `IImuDriver` implementations (real chip vs CSV replay). |
| **HAL** (`src/hal/`) | Thin Zephyr wrappers (timers, I2C handles) to keep drivers testable. |

## Data flow

PPG and IMU drivers implement `IPpgDriver` / `IImuDriver`. **`MockPpgDriver`** and **`MockImuDriver`** replay fixed sequences (same numbers as [`data/ppg_sample.csv`](../data/ppg_sample.csv) and [`data/imu_sample.csv`](../data/imu_sample.csv); PPG is a long ~72 BPM sine for visible `[hr]` in QEMU) so QEMU runs without a filesystem. **`Max30102Driver`** and **`ImuSkeletonDriver`** are register-map skeletons with I2C left unimplemented for a later HAL pass. **`SensorManager`** uses Zephyr `k_work_delayable` on the system workqueue to read each driver at its nominal rate and enqueue raw samples. **`main`** drains queues into **`HeartRateEstimator`** and **`ActivityClassifier`** while tagging logs with `[sensor]` / `[hr]` / `[act]` so reviewers can follow the pipeline in QEMU output.

## Build system

- **West** + **CMake**: `west.yml` pins Zephyr; `CMakeLists.txt` registers sources on the Zephyr `app` library.
- **Kconfig**: `prj.conf` enables logging and C++17.

## Phase scope

- **Phase 1**: Toolchain, QEMU, CI, repo layout.
- **Phase 2**: Driver abstractions, mock replay, sensor manager + message queues; real I2C stubbed.
- **Phase 3**: `HeartRateEstimator`, `ActivityClassifier`, GoogleTest in [`tests/`](../tests/), [`tools/validate.py`](../tools/validate.py).
- **Phase 4**: Portfolio polish — [`docs/power.md`](power.md), [`docs/ble_future.md`](ble_future.md), [`docs/results/`](results/README.md), [`tools/plot_results.py`](../tools/plot_results.py); dataset-scale metrics live under **`docs/results/`** when you add PhysioNet/WISDM evaluation runs.
