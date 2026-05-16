# 04 — Flight Controller

## What Is a Flight Controller?

A **Flight Controller (FC)** is a small computer that:
1. Reads sensor data (gyroscope, accelerometer, barometer, GPS)
2. Reads pilot commands (from RC receiver)
3. Runs a control algorithm (PID loop) to compute corrections
4. Sends motor speed commands to ESCs
5. Does this **hundreds to thousands of times per second**

Without the FC, a quadcopter is aerodynamically unstable. It would immediately flip and crash. The FC is what makes it flyable.

---

## Why Quads Are Inherently Unstable

A fixed-wing plane has passive stability — wings create restoring forces if it tilts. A quadcopter has **no passive stability**. If one motor produces slightly more thrust, it tilts, the tilt causes it to drift, the drift accelerates...

The FC must constantly — every millisecond — sense any tilt and correct it before it becomes uncontrollable. Human reflexes (~200ms) are 200× too slow. The FC at 1kHz is 200,000× faster.

---

## FC Hardware Architecture

```
┌──────────────────────────────────────────────────────────┐
│                 FLIGHT CONTROLLER BOARD                  │
│                                                          │
│  ┌──────────┐   ┌────────┐   ┌──────────┐               │
│  │ MCU      │   │  IMU   │   │Barometer │               │
│  │(STM32 or │◄──│MPU6050 │   │(BMP280 or│               │
│  │ ESP32)   │   │ICM42688│   │ MS5611)  │               │
│  │          │◄──└────────┘   └──────────┘               │
│  │          │◄──────────────────────────                 │
│  │          │                                            │
│  │          │   UART ports for:                          │
│  │          │◄──────── RC Receiver (SBUS/IBUS/ELRS)      │
│  │          │◄──────── GPS module                        │
│  │          │◄──────── ESC telemetry (bidir DSHOT)       │
│  │          │──────► ESC signal wires (DSHOT output)     │
│  │          │──────► USB (for Betaflight configurator)   │
│  └──────────┘                                            │
│                                                          │
│  ┌──────────┐                                            │
│  │OSD chip  │──────────────────────► video overlay       │
│  └──────────┘                                            │
└──────────────────────────────────────────────────────────┘
```

---

## Common FC Microcontrollers

| MCU | Used In | Clock Speed | Notes |
|-----|---------|-------------|-------|
| STM32F405 | Most FPV FCs | 168 MHz | Industry standard for FPV |
| STM32F7xx | High-end FCs | 216 MHz | More processing for filtering |
| STM32H7xx | Top-end FCs | 480 MHz | RPM filtering, HD builds |
| **ESP32** | DIY / learning | 240 MHz | Dual-core, Wi-Fi, our target |
| RP2040 | Some DIY FCs | 133 MHz | Raspberry Pi microcontroller |

ESP32 can absolutely run a basic flight controller — it has the speed, peripherals, and two cores. Commercial drones use STM32 because of mature ecosystem (Betaflight), not because ESP32 is insufficient.

---

## FC Loop Rates

The FC runs in loops. Two important rates:

### Gyro Loop (inner loop)
How fast the FC reads the IMU and runs the PID algorithm.

```
Common rates:
  1 kHz  = 1,000 times/sec  (entry level)
  4 kHz  = 4,000 times/sec  (standard)
  8 kHz  = 8,000 times/sec  (high performance)
  32 kHz = 32,000 times/sec (some IMUs support this, not always useful)
```

### RC Loop (outer loop)
How fast the FC reads new pilot commands from the receiver.

```
SBUS:  112 Hz
ELRS:  100–1000 Hz (configurable)
```

The gyro loop is always faster than the RC loop. Between RC updates, the gyro loop keeps stabilizing based on the last known RC command.

---

## The IMU — Heart of the FC

The IMU (Inertial Measurement Unit) is usually a chip like the **MPU6050**, **ICM-20689**, or **ICM-42688**.

It contains:
- **3-axis gyroscope** — measures angular velocity (deg/sec) on X, Y, Z axes
- **3-axis accelerometer** — measures linear acceleration (m/s² or G) on X, Y, Z axes

### Gyroscope vs Accelerometer for Stability

```
GYROSCOPE:
  Measures: how fast the drone is rotating right now
  Precision: very accurate, fast
  Problem: drifts over time (integrating noise builds up error)
  Primary use: main stabilization sensor

ACCELEROMETER:
  Measures: gravity direction (tells us "which way is down")
  Precision: accurate for slow movements, noisy for fast movements
  Problem: vibrations corrupt the reading
  Primary use: angle estimation, horizon lock (Angle mode)
```

The gyro is the primary sensor. The accelerometer is used to correct gyro drift in angle (self-leveling) mode.

### Why Vibration Is the Enemy

Motors spinning at 20,000+ RPM create vibrations. If these vibrations reach the IMU:
- Accelerometer reads false accelerations
- Gyro reads false rotation rates
- PID algorithm tries to correct non-existent tilts → oscillation → crash

**Solutions:**
- Mount FC on soft rubber standoffs
- Balance propellers
- Use software filters (Betaflight has notch filters, RPM filters)

---

## FC Coordinate System

Standard orientation (NED — North-East-Down):

```
        Pitch axis (Y)
              ↑
              │      Roll axis (X)
              │     /
              │    /
      Front   │   /
      ────────┼──/──────────────
             [FC]
              │
              │
              │
              ▼
         Yaw axis (Z) points DOWN

Roll:  rotation around X axis (tipping left/right)
Pitch: rotation around Y axis (tipping forward/back)
Yaw:   rotation around Z axis (spinning in place)
```

---

## Flight Modes

FCs support multiple flight modes that change how it responds to your input.

### Acro Mode (Rate Mode / Manual)

- **Sticks control angular velocity** (rotation rate), not angle
- Release stick → drone maintains current rotation rate (does NOT self-level)
- You are flying the gyro directly
- Requires skill. Used by all experienced FPV pilots.
- Maximum performance and maneuverability

```
Stick input → target rotation rate
  e.g., full roll stick right → spin right at 720°/sec
  release stick → stop spinning, hover in whatever angle you're at
```

### Angle Mode (Self-Level / Stabilized)

- **Sticks control angle** (tilt angle), not rotation rate
- Release stick → drone returns to level automatically
- Easiest to fly. Used by beginners and photography drones.
- Limited to ~45° tilt maximum

```
Stick input → target tilt angle
  e.g., half roll stick right → tilt 20° right
  release stick → automatically return to level (0°)
```

### Horizon Mode

- Hybrid: behaves like Angle mode at low stick deflection
- Allows flips/rolls at full stick deflection (switches to Acro briefly)
- Useful training mode

### GPS Modes (require GPS)

- **Position Hold** — drone holds GPS position and altitude automatically
- **Return to Home** — drone flies back to takeoff point
- **Follow Me** — drone follows pilot's GPS position

---

## FC Mounting

```
FC mounting options:
  ┌───────────────────────────────────────────┐
  │  Hard mount:    direct bolts to frame     │
  │  Problem:       vibrations transfer to FC │
  │                                           │
  │  Soft mount:    rubber standoffs          │
  │  How:  Frame ─[rubber]─ FC               │
  │  Benefit:       absorbs motor vibrations  │
  │  Most common choice for FPV builds        │
  └───────────────────────────────────────────┘

Rubber standoff sizes:
  M3 × 6mm rubber grommets are standard
```

**FC orientation matters!** If the FC is rotated on the frame (common to route wires), you must tell the firmware the rotation angle. In Betaflight: `Board Alignment` setting.

---

## FC Power

FCs run on 5V (from ESC BEC or onboard regulator).

```
Power path:
  LiPo → ESC BEC → 5V → FC 5V pin
                      → Receiver 5V pin

  OR:
  LiPo → FC onboard regulator → 5V internally
         (many FCs can accept direct battery voltage on VBAT pin)
```

FCs also measure battery voltage through a voltage divider on the VBAT pin — this lets Betaflight show your battery voltage and trigger low-voltage warnings.

---

## Popular FC Firmware

### Betaflight
- Most popular for FPV racing/freestyle
- Huge community, excellent configurator app
- Targets STM32
- Features: PID tuning, filters, OSD, blackbox logging

### iNav
- Fork of Betaflight focused on GPS navigation
- Long-range, waypoint missions, fixed-wing support

### ArduPilot
- Professional-grade, supports many vehicle types
- Very feature-rich (used in commercial/military drones)
- Runs on Pixhawk hardware

### Our Custom ESP32 FC
- What we'll build in file `07_diy_fc_on_esp32.md`
- Reads MPU6050 via I2C
- Runs simple PID loop
- Outputs DSHOT to ESCs
- Basic rate (acro) mode only

---

## FC Blackbox

Most FCs can log flight data to an onboard flash chip or SD card. This is called **blackbox logging**.

Logged data includes (at 2kHz+):
- Gyro rates on all 3 axes
- Accelerometer data
- PID setpoints and outputs
- Motor outputs
- RC commands

Used for PID tuning — you analyze the log to see if the drone is oscillating, sluggish, or bouncy, then adjust P, I, D values accordingly.

---

## OSD — On-Screen Display

Many FCs include an OSD chip that overlays flight data on the FPV camera video feed.

```
Camera feed + OSD overlay:
  ┌──────────────────────────────────┐
  │  14.8V  ████▒▒▒▒  75%           │
  │                                  │
  │          ──┼──                   │
  │         crosshair                │
  │                                  │
  │  ARMED   Acro    00:03:24        │
  └──────────────────────────────────┘
  Shows: voltage, mAh used, flight time, mode, GPS
```

---

## Practice

1. A quad has no FC — what happens when you apply throttle?
2. What is the difference between a gyro loop and an RC loop? Why does the gyro loop run faster?
3. In Angle mode, you push the roll stick to the right and release it. What does the drone do?
4. In Acro mode, you push the roll stick to the right and release it. What does the drone do?
5. Why are soft-mount rubber standoffs used for FC mounting?
6. What does the accelerometer measure? Why is it bad for measuring rapid movements?
7. Your FC is rotated 45° on the frame for better cable routing. What must you configure in firmware?
