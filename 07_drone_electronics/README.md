# Module 07 — Drone Electronics

Bringing everything together into a flying machine.

## Topics (Coming Soon)

| File | What You'll Learn |
|------|-------------------|
| 01 — Drone Anatomy | Frame, motors, ESCs, FC, receiver — how it all connects |
| 02 — Brushless Motors | KV rating, motor sizing for your frame |
| 03 — ESC Control | Arming sequence, throttle calibration, DSHOT protocol |
| 04 — Flight Controller | FC role, PID control basics |
| 05 — PID Tuning | What P, I, D mean and how to tune them |
| 06 — Radio Control | RC receivers, SBUS/IBUS protocols |
| 07 — DIY FC on ESP32 | Building a minimal flight controller |

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
