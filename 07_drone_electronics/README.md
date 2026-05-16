# Module 07 — Drone Electronics

Bringing everything together into a flying machine.

## Topics

| File | What You'll Learn |
|------|-------------------|
| [01 — Drone Anatomy](01_drone_anatomy.md) | Frame, motors, ESCs, FC, receiver — how it all connects |
| [02 — Brushless Motors](02_brushless_motors.md) | KV rating, motor sizing, propeller matching |
| [03 — ESC Control](03_esc_control.md) | Arming sequence, throttle calibration, DSHOT protocol |
| [04 — Flight Controller](04_flight_controller.md) | FC role, loop rates, IMU, flight modes |
| [05 — PID Tuning](05_pid_tuning.md) | What P, I, D mean and how to tune them |
| [06 — Radio Control](06_radio_control.md) | TX/RX systems, SBUS/IBUS/CRSF protocols, ESP32 code |
| [07 — DIY FC on ESP32](07_diy_fc_on_esp32.md) | Complete working flight controller from scratch |

## The Drone System

```
Remote Controller (TX)
        │ radio link
        ▼
RC Receiver (RX)
        │ SBUS/IBUS/PPM
        ▼
Flight Controller (ESP32 + IMU + Barometer)
  ├── reads: IMU (roll/pitch/yaw), RC commands, baro (altitude)
  ├── runs: PID algorithm 400–1000 times/second
  └── outputs: PWM/DSHOT to 4 ESCs
        │
        ▼
4× ESC (Electronic Speed Controller)
        │ 3-phase power
        ▼
4× Brushless Motor + Propeller
        │
        ▼
Thrust → Flight
```

## Frame Sizes

| Size | Class | Use |
|------|-------|-----|
| 100–150mm | Tiny Whoop | Indoor, beginner |
| 180–250mm | Mini quad | Sport/racing |
| 300–450mm | 5-inch class | Most popular, our target |
| 500–600mm | 7-inch | Efficiency, range |
| 800mm+ | Heavy lifter | Aerial photography, cargo |
