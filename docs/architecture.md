# Architecture

This firmware is organized in layers so sensor hardware can be swapped for mocks or replay without changing algorithms or the application entrypoint.

## Layers

| Layer | Role |
|--------|------|
| **Application** (`app/`) | Startup, configuration, glue code. Keeps `main()` thin. |
| **Services** (`src/services/`) | Sensor manager, scheduling, queues — Phase 2+. |
| **Algorithms** (`src/algorithms/`) | Heart rate, activity classification — Phase 3+. |
| **Drivers** (`src/drivers/`) | `IPpgDriver` / `IImuDriver` implementations (real chip vs CSV replay). |
| **HAL** (`src/hal/`) | Thin Zephyr wrappers (timers, I2C handles) to keep drivers testable. |

## Data flow (target state)

PPG and IMU drivers expose a common interface. A sensor manager pulls samples on a timer or interrupt-driven path, pushes into queues, and algorithm tasks consume windows of data. Logging uses Zephyr `LOG_*` macros so the pipeline is visible on the console (QEMU UART today; RTT-style transport on real hardware later).

## Build system

- **West** + **CMake**: `west.yml` pins Zephyr; `CMakeLists.txt` registers sources on the Zephyr `app` library.
- **Kconfig**: `prj.conf` enables logging and C++17.

## Phase 1 scope

Phase 1 only proves the toolchain, QEMU bring-up, and repository layout. Driver interfaces and algorithms land in later phases.
