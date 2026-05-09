# P08 — ESP32 Drone Remote Control

Build a complete 4-channel radio transmitter (remote control) for your ESP32 drone.

**What you end up with:**
- A handheld controller with two joysticks (throttle, yaw, pitch, roll)
- 50 Hz wireless link via NRF24L01 with up to ~300m range
- OLED screen showing live channel values, link status, and arm state
- Hardware arm/disarm safety (2-second hold + throttle at zero to arm)
- Automatic failsafe on signal loss — drone disarms if link drops for 500ms

---

## Hardware You Need

| Qty | Component | Where it is in your kit |
|-----|-----------|-------------------------|
| 2 | ESP32 (one per end) | Module 03 |
| 2 | NRF24L01+PA+LNA | Module 04 |
| 2 | PS2 Joystick modules | Module 05 |
| 1 | SSD1306 0.96" OLED | Module 05 |
| 2 | 100µF electrolytic capacitor | Module 06 |
| 1 | AMS1117 3.3V power module (recommended) | Module 06 |
| 2 | Push buttons | from kit |
| — | Breadboard + jumper wires | — |

One NRF24L01 goes in the transmitter (remote), one goes in the drone (receiver). They both use the same wiring.

---

## System Overview

```
┌─────────────────────────────────────┐     radio link (NRF24L01)     ┌──────────────────────┐
│         TRANSMITTER (remote)        │ ─────── 2.4GHz 50Hz ──────▶  │   DRONE (receiver)   │
│                                     │                               │                      │
│  Left Stick   → Throttle + Yaw      │     DronePacket struct:       │  Reads packet        │
│  Right Stick  → Pitch + Roll        │     throttle  0–1000          │  Feeds into PID FC   │
│  ARM button   → arm/disarm          │     roll    -500..+500        │  Failsafe on loss    │
│  MODE button  → flight mode 0/1/2   │     pitch   -500..+500        │                      │
│  OLED         → live status         │     yaw     -500..+500        │                      │
│                                     │     armed   0 or 1            │                      │
│                                     │     mode    0/1/2             │                      │
└─────────────────────────────────────┘                               └──────────────────────┘
```

### Mode 2 RC layout (standard)

```
  LEFT STICK            RIGHT STICK
  ┌─────┐               ┌─────┐
  │  ↑  │               │  ↑  │
  │ THR │               │ PIT │
  │←YAW→│               │←ROL→│
  │  ↓  │               │  ↓  │
  └─────┘               └─────┘
  Throttle/Yaw          Pitch/Roll
```

---

## Transmitter Wiring

### NRF24L01+PA+LNA → ESP32

```
NRF24L01        ESP32
─────────       ──────────────────────
GND      ───▶   GND
VCC      ───▶   3.3V from AMS1117 module (NOT from ESP32's 3.3V pin)
CE       ───▶   GPIO 4
CSN      ───▶   GPIO 5
SCK      ───▶   GPIO 18   (hardware SPI VSPI)
MOSI     ───▶   GPIO 23   (hardware SPI VSPI)
MISO     ───▶   GPIO 19   (hardware SPI VSPI)
IRQ      ───▶   leave disconnected
```

**Add a 100µF electrolytic cap across NRF24L01 VCC and GND.**
The PA+LNA version draws up to 250mA in bursts — this cap prevents voltage
drops that cause random disconnects. This is the #1 cause of radio problems.
See `06_power_systems/02_ams1117_power_module.md` for the AMS1117 setup.

### Left Joystick (Throttle / Yaw) → ESP32

```
Left PS2 Module     ESP32
───────────────     ──────────
GND          ───▶   GND
VCC          ───▶   3.3V
VRx          ───▶   GPIO 34   (Yaw)
VRy          ───▶   GPIO 35   (Throttle)
SW           ───▶   GPIO 25
```

### Right Joystick (Pitch / Roll) → ESP32

```
Right PS2 Module    ESP32
────────────────    ──────────
GND          ───▶   GND
VCC          ───▶   3.3V
VRx          ───▶   GPIO 32   (Roll)
VRy          ───▶   GPIO 33   (Pitch)
SW           ───▶   GPIO 26
```

### OLED Display → ESP32

```
OLED Pin    ESP32
────────    ──────────
GND    ───▶ GND
VCC    ───▶ 3.3V
SCL    ───▶ GPIO 22
SDA    ───▶ GPIO 21
```

### Buttons → ESP32

```
ARM button  one leg → GPIO 27, other leg → GND
MODE button one leg → GPIO 14, other leg → GND
```

The code uses INPUT_PULLUP so no external resistors are needed.

---

## Full Breadboard Layout

```
                       ESP32 (38-pin)
                  ┌────────────────────┐
            3.3V ─┤3V3              GND├─ GND (all grounds tie together)
             GND ─┤GND              EN ├─
                  ┤VP (36)         SVP ├─
                  ┤VN (39)         SVN ├─
    Left VRx(Yaw)─┤GPIO34         IO23├─ NRF MOSI
    Left VRy(Thr)─┤GPIO35         IO22├─ OLED SCL
                  ┤GPIO32(Roll)   IO21├─ OLED SDA
                  ┤GPIO33(Pitch)  IO19├─ NRF MISO
       Left SW   ─┤GPIO25         IO18├─ NRF SCK
       Right SW  ─┤GPIO26          IO5├─ NRF CSN
       ARM button─┤GPIO27          IO4├─ NRF CE
                  ┤GPIO14          IO0├─
                  └────────────────────┘
```

> GPIO 34/35 are **input-only** on ESP32 — they have no internal pullup, which is fine
> since they're connected to pot outputs that always drive a voltage.

---

## Drone Side (Receiver) Wiring

The receiver ESP32 uses the **same NRF24L01 wiring** as above.
There are no joysticks or OLED on the drone side — just the radio module.

---

## Files

| File | Purpose |
|------|---------|
| `transmitter/transmitter.ino` | Upload to your handheld remote |
| `receiver_test/receiver_test.ino` | Upload to the drone ESP32 to test the link |

---

## Step-by-Step Build Guide

### Step 1 — Install Libraries

In Arduino IDE → Sketch → Include Library → Manage Libraries:

| Library | Author |
|---------|--------|
| RF24 | TMRh20 |
| Adafruit SSD1306 | Adafruit |
| Adafruit GFX Library | Adafruit |

### Step 2 — Upload the Receiver Test Sketch First

Upload `receiver_test.ino` to the drone ESP32 and keep it connected via USB.
Open Serial Monitor (115200 baud). It will print `Receiver ready — waiting...`
You need this running to confirm the link before testing the transmitter.

### Step 3 — Calibrate the Joysticks

In `transmitter.ino`, set:
```cpp
const bool CALIBRATION_MODE = true;
```

Upload, open Serial Monitor. Move **both sticks to every corner and back to center**
slowly a few times. The output shows min/center/max for each axis live.

Copy those values into the `StickCal` structs:
```cpp
StickCal calLeftX  = {minLX, centerLX, maxLX};
StickCal calLeftY  = {minLY, centerLY, maxLY};
StickCal calRightX = {minRX, centerRX, maxRX};
StickCal calRightY = {minRY, centerRY, maxRY};
```

Then set `CALIBRATION_MODE = false` and re-upload.

### Step 4 — Test the Link

Upload the calibrated `transmitter.ino`. On the OLED you should see `LINKED`
within a second. On the receiver's Serial Monitor you'll see channel values
updating as you move the sticks.

### Step 5 — Arm and Test

1. **Throttle stick all the way DOWN** (full bottom position)
2. **Hold ARM button for 2 full seconds** — OLED switches from `[SAFE]` to `[ARMED]`
3. Move sticks and watch the values change in Serial Monitor
4. To disarm: hold ARM button 2 seconds again

### Step 6 — Connect to Your Flight Controller

In `receiver_test.ino`, replace the `Serial.printf` call with your PID
controller function, passing in the received throttle/roll/pitch/yaw values.

---

## Channel Ranges Reference

| Channel  | Range      | Stick position meaning |
|----------|-----------|------------------------|
| Throttle | 0 – 1000  | 0 = stick bottom (no thrust), 1000 = full up |
| Roll     | -500 – +500 | -500 = full left, 0 = center, +500 = full right |
| Pitch    | -500 – +500 | -500 = nose down (stick fwd), +500 = nose up |
| Yaw      | -500 – +500 | -500 = rotate left, +500 = rotate right |

---

## Safety Notes

**Always test without propellers attached first.**

- **Never arm with throttle above zero.** The code prevents this but physically check.
- **Failsafe is active** — if the transmitter is turned off or goes out of range,
  the drone disarms within 500ms. This is a software failsafe only; a real
  flight controller should also have a hardware failsafe.
- **Spring-return throttle:** the PS2 joystick modules have springs — releasing
  the left stick returns throttle to 50% (value 500), not zero. This is fine
  for testing but means the drone will hover (not fall) if you let go.
  For advanced flying, you'd want a non-spring throttle stick.
- **NRF24L01 channel 108** (2.508 GHz) is above the WiFi band (2.412–2.484 GHz),
  which reduces interference. Do not fly near other 2.4 GHz devices in use.

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---------|-------------|-----|
| OLED shows `NO LINK` | NRF24L01 not connecting | Check power (100µF cap? AMS1117?) and wiring |
| NRF24L01 not found at startup | Wiring wrong or no power | Check CE/CSN/SCK/MOSI/MISO pins |
| Sticks read wrong direction | Calibration inverted | Swap `minVal`/`maxVal` in the StickCal struct |
| Throttle jumps to 500 when released | Spring-return stick | Expected behavior — see safety note above |
| Values drift around zero when stick is centered | ADC noise | Increase `STICK_DEADZONE` to 50–80 |
| OLED shows nothing | Wrong I2C address | Try `0x3D` instead of `0x3C` in `OLED_ADDR` |
| Random disconnects at range | NRF24L01 brown-out | Larger cap (220µF), or dedicated AMS1117 supply |

---

## What's Next

- **P07 — Flight Controller** — connect the receiver output to a PID loop that
  drives 4 ESCs. This is the drone flight controller itself.
- **Module 07** — covers brushless motors, ESC control, and PID tuning.
- **Telemetry** — add a second pair of NRF24L01 modules running in the other
  direction to send battery voltage and altitude back to the remote's OLED.
