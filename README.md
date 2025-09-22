# Demo Firmware Project

A simulated embedded firmware application built with CMake. Demonstrates realistic firmware patterns with hardware abstraction layers, device drivers, and continuous sensor monitoring.

## Building

```bash
make          # Debug build
make release  # Release build
```

## Testing

```bash
make test
```

## Running

```bash
make run
```

The application simulates firmware startup, runs self-diagnostics, then continuously blinks an LED and reads temperature/humidity sensors. Press Ctrl+C to exit.

## What it does

- Initializes simulated hardware (GPIO, UART, sensors)
- Runs hardware self-tests
- Continuously monitors sensor data and controls LED
- Outputs realistic telemetry data
- Handles graceful shutdown

