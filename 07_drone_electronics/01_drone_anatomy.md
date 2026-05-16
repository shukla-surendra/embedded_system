# 01 — Drone Anatomy

## What Is a Quadcopter?

A quadcopter is a drone with **4 motors**. Each motor spins a propeller. By varying the speed of each motor independently, the flight controller can make the drone move in any direction, rotate, and hold position.

No moving parts except the motor shafts and propellers. No gearboxes. No servos for control surfaces. Simplicity is why quads dominate the DIY market.

---

## The Big Picture — Every Component

```
┌─────────────────────────────────────────────────────────────┐
│                    QUADCOPTER SYSTEM                        │
│                                                             │
│   Pilot (you)                                               │
│       │                                                     │
│       ▼                                                     │
│   TX (Transmitter / Radio Controller)                       │
│       │  2.4 GHz radio signal                               │
│       ▼                                                     │
│   RX (Receiver) ── mounted on drone                         │
│       │  SBUS / IBUS / PPM signal                           │
│       ▼                                                     │
│   FC (Flight Controller) ← IMU (gyro + accel)              │
│       │                  ← Barometer (altitude)             │
│       │                  ← GPS (optional)                   │
│       │  PWM / DSHOT signals (one per motor)                │
│       ▼                                                     │
│   ESC × 4 (Electronic Speed Controller)                     │
│       │  3-phase AC (simulated)                             │
│       ▼                                                     │
│   Brushless Motor × 4                                       │
│       │  shaft rotation                                      │
│       ▼                                                     │
│   Propeller × 4  →  Thrust                                  │
│                                                             │
│   LiPo Battery ──────► powers everything above             │
└─────────────────────────────────────────────────────────────┘
```

---

## The Frame

The skeleton everything mounts to.

```
        M1 (CW)          M2 (CCW)
          ●                ●
           \              /
            \            /
             ┌──────────┐
             │    FC    │
             │   BATT   │
             └──────────┘
            /            \
           /              \
          ●                ●
        M3 (CCW)         M4 (CW)
```

- **CW** = clockwise spinning propeller
- **CCW** = counter-clockwise spinning propeller
- Diagonal pairs spin the same direction
- This cancels torque — without it the drone would spin in place

### Frame Materials

| Material | Weight | Strength | Cost | Used For |
|----------|--------|----------|------|----------|
| Carbon Fiber | Very light | Very strong | Expensive | Racing, FPV |
| Aluminum | Medium | Medium | Cheap | Beginner builds |
| Plastic (nylon) | Light | Flexible | Cheap | Tiny whoops |
| Plywood | Heavy | OK | Very cheap | Prototyping |

### Frame Size = Motor-to-Motor Diagonal Distance

A "5-inch" frame means the propellers are 5 inches in diameter — the frame is sized to fit them. Frame diagonal is roughly 100–120mm larger than prop size.

| Prop Size | Frame Class | Frame Diagonal |
|-----------|-------------|----------------|
| 3" | Micro | ~130–160mm |
| 5" | Mini/Standard | ~210–250mm |
| 7" | Long range | ~300–350mm |
| 10"+ | Heavy lifter | ~450mm+ |

---

## Motors

Brushless DC motors. Each has **3 wires** (3-phase).

```
     Motor shaft (top)
          │
     ┌────┴────┐
     │ Magnets │  ← outer bell spins (outrunner)
     │  stator │  ← inner coils are fixed
     └────┬────┘
          │
     Motor mount (bolts to arm)

     A ─── B ─── C   (3 phase wires to ESC)
```

**Motor direction is reversed by swapping any 2 of the 3 wires.**

Key specs:
- **KV rating** — RPM per volt. A 2300KV motor on 4S (14.8V) → ~34,000 RPM unloaded.
- **Stator size** (e.g., 2207) — 22mm diameter, 7mm tall stator. Bigger = more torque.

---

## ESC — Electronic Speed Controller

The ESC converts a digital signal from the FC into 3-phase power the motor can use.

```
FC  ──(PWM/DSHOT)──►  ESC  ──(3-phase)──►  Motor
                       ▲
                  LiPo battery
```

Each motor needs its own ESC. Modern builds use a **4-in-1 ESC** — all four ESCs on one board, greatly reducing wiring.

ESC also usually contains a **BEC** (Battery Eliminator Circuit) — a 5V regulator that powers the flight controller from the main battery.

---

## Flight Controller (FC)

The brain. It does one job: **keep the drone stable and follow your stick commands**.

```
Inputs:
  ├── IMU (gyroscope + accelerometer) → knows tilt, spin rate
  ├── Barometer → knows altitude
  ├── RC receiver → knows what you want
  └── (optional) GPS, rangefinder, optical flow

Processing (runs 1000×/sec):
  └── PID algorithm → calculates motor corrections

Outputs:
  └── 4 motor speed commands (PWM or DSHOT)
```

Common FC chips: STM32F405, STM32F7xx. For our learning build: **ESP32**.

Popular FC firmware: Betaflight, iNav, ArduPilot. Our custom build: bare-metal ESP32 code.

---

## IMU — Inertial Measurement Unit

A small chip (usually MPU6050 or ICM-42688) that contains:
- **3-axis gyroscope** — measures rotation rate in deg/sec
- **3-axis accelerometer** — measures acceleration in m/s²

The gyro is the primary sensor for stability. The FC reads it up to 8000 times per second.

Axes:
```
        Z (yaw)
        │
        │     Y (pitch)
        │    /
        │   /
        └──────── X (roll)
```

---

## RC Receiver

Receives your radio signal and decodes it into channel values.

| Channel | Controls |
|---------|----------|
| CH1 | Roll (left/right tilt) |
| CH2 | Pitch (forward/back tilt) |
| CH3 | Throttle (up/down power) |
| CH4 | Yaw (spin left/right) |
| CH5+ | Arm/Disarm, Flight modes, etc. |

Common output protocols: **SBUS** (serial, inverted UART), **IBUS** (serial), **PPM** (legacy), **ELRS** (modern, low latency).

---

## LiPo Battery

Lithium Polymer. The power source.

Key specs:
- **Cell count (S)** — each cell = 3.7V nominal, 4.2V fully charged. 4S = 14.8V nominal.
- **Capacity (mAh)** — how much energy stored. 1500mAh = can deliver 1500mA for 1 hour.
- **C rating** — max continuous discharge. 1500mAh × 75C = 112.5A max.

```
Battery voltage at different states:
  4.20V/cell = 100% (fully charged)
  3.85V/cell = ~50%
  3.70V/cell = ~20%
  3.50V/cell = cutoff (land NOW or damage battery)
```

Never discharge below 3.5V/cell. LiPo fires are real — use a LiPo bag.

---

## How Altitude and Direction Are Controlled

The magic of a quad: **no mechanical linkages**. Only motor speed changes.

```
THROTTLE UP:  All 4 motors speed up equally
THROTTLE DOWN: All 4 motors slow down equally

ROLL RIGHT:   Left motors speed up, right motors slow down
ROLL LEFT:    Right motors speed up, left motors slow down

PITCH FORWARD: Rear motors speed up, front motors slow down
PITCH BACK:    Front motors speed up, rear motors slow down

YAW RIGHT:    CW motors (M1, M4) speed up, CCW motors slow down
YAW LEFT:     CCW motors (M2, M3) speed up, CW motors slow down
```

The flight controller is doing this math — mixing your stick commands with IMU corrections — hundreds of times per second.

---

## Component Interaction Summary

```
YOU → TX → [radio] → RX → FC
                           │
               IMU ────────┤
               BARO ───────┤
                           │
                    PID calculation
                           │
              ┌────┬───────┴────┬────┐
              ▼    ▼            ▼    ▼
             ESC1 ESC2        ESC3 ESC4
              │    │            │    │
              M1   M2          M3   M4
              │    │            │    │
             Prop Prop        Prop  Prop
                           │
                    FLIGHT ←─────────
```

---

## Practice

1. Draw your own drone block diagram from memory without looking at this file.
2. Name the 3 wires coming out of a brushless motor and explain what they connect to.
3. If a 4S LiPo reads 15.2V total, what is the per-cell voltage? Is it safe to fly?
4. Why does a quadcopter need motors spinning in opposite directions?
5. Which component reads the gyroscope data — the ESC, the FC, or the receiver?
