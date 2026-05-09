# Project P08 — ESP32 Drone Remote Control
### 4-Channel NRF24L01 Radio Transmitter with OLED Display

---

## Project Summary

| Field | Detail |
|-------|--------|
| **Project name** | ESP32 Drone Remote Control |
| **Difficulty** | Intermediate |
| **Modules covered** | 03 (ESP32), 04 (NRF24L01), 05 (Joysticks, OLED), 06 (Power) |
| **Radio protocol** | NRF24L01 2.4 GHz, 250 kbps, 50 Hz |
| **Control channels** | 4 analog (Throttle, Roll, Pitch, Yaw) + Arm + Mode |
| **Wireless range** | Up to ~300 m line-of-sight (PA+LNA module) |
| **Files** | `transmitter/transmitter.ino`, `receiver_test/receiver_test.ino` |

---

## What This Project Builds

Two ESP32 boards talking to each other over 2.4 GHz radio.

```
┌──────────────────────────────────────┐              ┌────────────────────────────┐
│          TRANSMITTER (remote)        │              │     RECEIVER (drone)       │
│                                      │              │                            │
│  ┌──────────┐   ┌──────────┐         │              │  ┌────────────────────┐    │
│  │Left Stick│   │Right Stick│        │  ~2.4 GHz   │  │   NRF24L01+PA+LNA  │    │
│  │ THR / YAW│   │PITCH / ROL│        │  50 packets │  └────────┬───────────┘    │
│  └────┬─────┘   └─────┬─────┘        │  per second │           │                │
│       │               │              │  ──────────▶ │           ▼                │
│  ┌────▼───────────────▼────┐         │              │  ┌────────────────────┐    │
│  │         ESP32           │         │              │  │       ESP32        │    │
│  └─────────────┬───────────┘         │              │  │  (flight computer) │    │
│  ┌─────────────▼───────────┐         │              │  └────────────────────┘    │
│  │    NRF24L01+PA+LNA      ├─────────┘              └────────────────────────────┘
│  └─────────────────────────┘         │
│  ┌──────────┐  ┌──────┐  ┌────┐      │
│  │OLED 128×64│ │ ARM  │  │MODE│      │
│  │  display  │ │ btn  │  │ btn│      │
│  └──────────┘  └──────┘  └────┘      │
└──────────────────────────────────────┘
```

---

## Modules and Components Required

### Transmitter Side (your handheld remote)

| # | Component | Spec | Purpose | From module |
|---|-----------|------|---------|-------------|
| 1 | ESP32 38-pin | Dual-core 240 MHz, 3.3V | Main processor | Module 03 |
| 1 | NRF24L01+PA+LNA | 2.4 GHz, SMA antenna | Radio transmitter | Module 04 |
| 1 | NRF24L01 Adapter Board *(optional but recommended)* | Built-in LDO + caps | Clean power for radio | Module 04 |
| 2 | PS2 Joystick Module | Analog X/Y + button | Throttle/Yaw + Pitch/Roll | Module 05 |
| 1 | SSD1306 OLED | 0.96″, 128×64 px, I2C | Status display | Module 05 |
| 1 | AMS1117 3.3V module | If not using adapter board | Powers NRF24L01 | Module 06 |
| 1 | 100µF electrolytic capacitor | 16V or higher | Filters NRF24L01 power spikes | Module 06 |
| 2 | Push button | Momentary NO | ARM + MODE buttons | any |
| — | Breadboard + jumper wires | — | Prototyping | — |

### Drone Side (receiver, mounted on drone)

| # | Component | Purpose | From module |
|---|-----------|---------|-------------|
| 1 | ESP32 38-pin | Receives commands, runs flight controller | Module 03 |
| 1 | NRF24L01+PA+LNA | Radio receiver | Module 04 |
| 1 | 100µF electrolytic capacitor | Power filtering | Module 06 |

---

## Pin Connection Tables

### TRANSMITTER — Complete Pin Map

#### NRF24L01 (+PA+LNA) → ESP32

> If using the **adapter board**: connect VCC to **5V (VIN)**, not 3.3V.
> If wiring the NRF24L01 **directly** (no adapter): connect VCC to the **AMS1117 3.3V output**, add 100µF cap across VCC/GND.

| NRF24L01 Pin | ESP32 Pin | Wire colour (suggested) | Notes |
|---|---|---|---|
| GND | GND | Black | Common ground |
| VCC | 3.3V (via AMS1117) *or* 5V (via adapter) | Red | See note above |
| CE | GPIO **4** | Orange | Chip Enable — TX/RX mode |
| CSN | GPIO **5** | Yellow | SPI Chip Select |
| SCK | GPIO **18** | Blue | SPI Clock (VSPI) |
| MOSI | GPIO **23** | Green | SPI Data out → radio |
| MISO | GPIO **19** | Purple | SPI Data in ← radio |
| IRQ | — | — | Leave disconnected |

#### Left Joystick (Throttle + Yaw) → ESP32

| PS2 Module Pin | ESP32 Pin | Channel | ADC range |
|---|---|---|---|
| GND | GND | — | — |
| VCC | 3.3V | — | — |
| VRx | GPIO **34** | Yaw | 0 – 4095 |
| VRy | GPIO **35** | Throttle | 0 – 4095 |
| SW | GPIO **25** | Left stick press | Digital (INPUT_PULLUP) |

#### Right Joystick (Pitch + Roll) → ESP32

| PS2 Module Pin | ESP32 Pin | Channel | ADC range |
|---|---|---|---|
| GND | GND | — | — |
| VCC | 3.3V | — | — |
| VRx | GPIO **32** | Roll | 0 – 4095 |
| VRy | GPIO **33** | Pitch | 0 – 4095 |
| SW | GPIO **26** | Right stick press | Digital (INPUT_PULLUP) |

#### SSD1306 OLED Display → ESP32

| OLED Pin | ESP32 Pin | Protocol | Notes |
|---|---|---|---|
| GND | GND | — | — |
| VCC | 3.3V | — | — |
| SCL | GPIO **22** | I2C clock | Shared bus |
| SDA | GPIO **21** | I2C data | Shared bus |

> I2C address: `0x3C` (default on most modules). If OLED is blank, try `0x3D`.

#### Buttons → ESP32

| Button | ESP32 Pin | Wiring | Behaviour |
|---|---|---|---|
| ARM | GPIO **27** | One leg → GPIO 27, other leg → GND | Hold 2 s to arm/disarm |
| MODE | GPIO **14** | One leg → GPIO 14, other leg → GND | Press to cycle mode 0→1→2 |

> Both use `INPUT_PULLUP` — no external resistors needed.

---

### RECEIVER — Pin Map (Drone Side)

#### NRF24L01 → ESP32 (same as transmitter)

| NRF24L01 Pin | ESP32 Pin |
|---|---|
| GND | GND |
| VCC | 3.3V (AMS1117) |
| CE | GPIO **4** |
| CSN | GPIO **5** |
| SCK | GPIO **18** |
| MOSI | GPIO **23** |
| MISO | GPIO **19** |
| IRQ | — |

---

## ESP32 GPIO Quick Reference (Transmitter)

```
                       ESP32 38-Pin
                  ┌───────────────────┐
             3.3V ┤ 3V3         GND   ├ GND ──── all grounds
              GND ┤ GND          EN   ├
                  ┤ GPIO36       GPIO23├──── NRF24L01 MOSI
                  ┤ GPIO39       GPIO22├──── OLED SCL
  Left VRx (Yaw)  ┤ GPIO34       GPIO21├──── OLED SDA
  Left VRy (Thr)  ┤ GPIO35       GPIO19├──── NRF24L01 MISO
  Right VRx (Roll)┤ GPIO32       GPIO18├──── NRF24L01 SCK
  Right VRy (Pitch┤ GPIO33        GPIO5├──── NRF24L01 CSN
  Left stick SW   ┤ GPIO25        GPIO4├──── NRF24L01 CE
  Right stick SW  ┤ GPIO26        GPIO0├
  ARM button      ┤ GPIO27        GPIO2├
                  ┤ GPIO14       GPIO15├
                  ┤ GPIO12       GPIO8 ├
                  ┤ GPIO13       GPIO7 ├
                  └───────────────────┘
```

> **GPIO 34 and 35 are input-only** — no internal pullup. This is fine; they are driven
> by the joystick potentiometers at all times.

---

## Radio Packet Structure

Both sketches must use the exact same struct — changing one without changing the other breaks the link.

```cpp
struct DronePacket {
  uint16_t throttle;   // 0 to 1000  — 0=motors off, 1000=full thrust
  int16_t  roll;       // -500 to +500
  int16_t  pitch;      // -500 to +500
  int16_t  yaw;        // -500 to +500
  uint8_t  armed;      // 0=disarmed  1=armed
  uint8_t  mode;       // 0=stabilize  1=alt-hold  2=manual
};                     // total: 10 bytes
```

| Channel | Type | Range | Stick neutral |
|---|---|---|---|
| Throttle | unsigned 16-bit | 0 – 1000 | 500 (spring center) |
| Roll | signed 16-bit | -500 – +500 | 0 |
| Pitch | signed 16-bit | -500 – +500 | 0 |
| Yaw | signed 16-bit | -500 – +500 | 0 |
| Armed | uint8 | 0 or 1 | 0 |
| Mode | uint8 | 0, 1, or 2 | 0 |

### Radio Settings

| Parameter | Value | Reason |
|---|---|---|
| Frequency channel | 108 (2.508 GHz) | Above WiFi band (2.412–2.484 GHz) |
| Data rate | 250 kbps | Longest range at lowest data rate |
| PA level | HIGH | Good balance of range and current draw |
| Pipe address | `"DRONE"` | 5-byte address, must match TX and RX |
| Packet rate | 50 Hz (every 20 ms) | Standard RC transmitter rate |

---

## Standard RC Layout — Mode 2

```
        LEFT STICK                    RIGHT STICK
   ┌───────────────────┐         ┌───────────────────┐
   │                   │         │                   │
   │       UP          │         │       UP          │
   │   (Throttle +)    │         │    (Pitch fwd)    │
   │                   │         │                   │
   │  LEFT      RIGHT  │         │  LEFT      RIGHT  │
   │  (Yaw L)  (Yaw R) │         │ (Roll L)  (Roll R)│
   │                   │         │                   │
   │      DOWN         │         │      DOWN         │
   │   (Throttle -)    │         │   (Pitch back)    │
   │                   │         │                   │
   └───────────────────┘         └───────────────────┘
    Throttle + Yaw                    Pitch + Roll
```

---

## OLED Screen Layout

What is displayed on the 128×64 OLED during operation:

```
┌────────────────────────────┐
│ LINKED   [ARMED]           │  ← Link status + arm state
│────────────────────────────│
│ THR: 650   YAW:  -45       │  ← Live channel values
│ PIT:  120  ROL:   80       │
│ T [████████████░░░░░░░░░] │  ← Throttle bar
│ MODE:1  OK:1842            │  ← Flight mode + packets sent
│ FAIL:0  50Hz CH108         │  ← Fail count + rate + channel
└────────────────────────────┘
```

| Indicator | Meaning |
|---|---|
| `LINKED` | Last packet was ACK'd by receiver |
| `NO LINK` | Last send failed or no radio found |
| `[ARMED]` | Armed — throttle commands are live |
| `[SAFE]` | Disarmed — throttle forced to 0 |
| Throttle bar | Visual 0–100% of throttle |
| `OK` count | Packets successfully delivered |
| `FAIL` count | Packets with no ACK (link quality indicator) |

---

## Software Architecture

```
loop() runs every 20 ms (50 Hz)
│
├── handleArmButton()
│     Track how long ARM is held
│     Toggle armed if held ≥ 2 s AND throttle < 10%
│
├── Read MODE button
│     Cycle flightMode 0 → 1 → 2 → 0
│
├── Read all 4 ADC channels
│     mapThrottle()  →  0 to 1000
│     mapAxis()      →  -500 to +500 (with dead zone)
│
├── Build DronePacket struct
│     Zero all channels if disarmed
│
├── radio.write(&pkt, sizeof(pkt))
│     Returns true = receiver ACK'd
│     Returns false = no ACK (link lost or receiver not ready)
│
└── updateDisplay()
      Redraw OLED with current values
      Called every packet, so display updates at 50 Hz
```

---

## Arming Sequence (Safety)

```
To ARM:
  1. Turn on transmitter
  2. Push LEFT stick fully DOWN (throttle = 0)
  3. Hold ARM button for 2 seconds
  4. OLED changes [SAFE] → [ARMED]
  5. Radio now sends live throttle values

To DISARM:
  1. Hold ARM button for 2 seconds at any time
  4. OLED changes [ARMED] → [SAFE]
  5. Throttle instantly forced to 0, all channels zeroed
```

Arming is blocked if throttle is above 10% to prevent the drone from spinning up unexpectedly.

---

## Failsafe Behaviour

| Condition | What happens |
|---|---|
| Transmitter turned off | Receiver detects no packet for 500 ms → sets `armed=0`, `throttle=0` |
| Radio link interrupted (obstacle / range) | Same — 500 ms timeout triggers failsafe |
| Transmitter battery dies mid-flight | Failsafe activates within 500 ms |
| Signal resumes after failsafe | Drone stays disarmed — operator must re-arm manually |

---

## Libraries to Install

Open Arduino IDE → **Sketch → Include Library → Manage Libraries** and install:

| Library name | Author | Used in |
|---|---|---|
| **RF24** | TMRh20 | Both sketches — radio communication |
| **Adafruit SSD1306** | Adafruit | Transmitter — OLED driver |
| **Adafruit GFX Library** | Adafruit | Transmitter — graphics primitives |

---

## Project Files

```
P08_esp32_remote_control/
│
├── PROJECT_DESCRIPTION.md      ← this file
├── README.md                   ← build guide and troubleshooting
│
├── transmitter/
│   └── transmitter.ino         ← upload to handheld remote ESP32
│
└── receiver_test/
    └── receiver_test.ino       ← upload to drone ESP32 to test link
```

---

## Build Checklist

- [ ] Both ESP32 boards set up and blink sketch verified (Module 03)
- [ ] Libraries installed (RF24, Adafruit SSD1306, Adafruit GFX)
- [ ] NRF24L01 wired with 100µF cap (or adapter board used)
- [ ] `receiver_test.ino` uploaded to drone ESP32 — Serial Monitor open
- [ ] `transmitter.ino` uploaded with `CALIBRATION_MODE = true`
- [ ] Joysticks calibrated — min/center/max values filled in
- [ ] `CALIBRATION_MODE = false` — transmitter re-uploaded
- [ ] OLED shows `LINKED` within 1 second of power-on
- [ ] Receiver Serial Monitor shows values matching stick positions
- [ ] ARM test: throttle down → hold ARM 2 s → OLED shows `[ARMED]`
- [ ] Disarm test: hold ARM 2 s again → OLED shows `[SAFE]`
- [ ] **Test everything without propellers attached**

---

## Connections to Other Modules and Projects

| Next step | Where to find it |
|---|---|
| Brushless motors and ESC control | `07_drone_electronics/03_esc_control.md` *(coming soon)* |
| IMU for stabilisation (MPU6050) | `05_sensors_and_modules/01_mpu6050_imu.md` |
| Altitude hold (BMP280 barometer) | `05_sensors_and_modules/02_bmp280_barometer.md` |
| Full flight controller build | `08_projects/P07_flight_controller/` *(coming soon)* |
| Battery monitor on OLED | `08_projects/P02_battery_monitor/` *(coming soon)* |
