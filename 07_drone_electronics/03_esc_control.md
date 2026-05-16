# 03 — ESC Control

## What Is an ESC?

An **Electronic Speed Controller** converts a simple digital command from the flight controller into the complex 3-phase power a brushless motor needs.

```
Flight Controller             ESC                    Motor
     │                         │                       │
     │  "spin at 70%"          │   Phase A ────────────┤
     │ ──(PWM/DSHOT)──────────►│   Phase B ────────────┤
     │                         │   Phase C ────────────┘
     │                    LiPo battery
     │                    ─────────────►
```

Without an ESC, the FC cannot drive a brushless motor at all.

---

## Inside an ESC

```
┌─────────────────────────────────────────────────────┐
│                    ESC BOARD                        │
│                                                     │
│  ┌─────────┐    ┌──────────┐    ┌───────────────┐  │
│  │ MCU     │    │ Gate     │    │ MOSFETs       │  │
│  │(usually │───►│ Driver   │───►│ (6 of them,   │  │
│  │STM8 or  │    │          │    │  2 per phase) │  │
│  │EFM8)    │    └──────────┘    └───────┬───────┘  │
│  └────┬────┘                            │           │
│       │ receives signal                  │ 3-phase   │
│       │ from FC                          │ output    │
│  ┌────▼────┐    ┌──────────┐            │           │
│  │BEC      │    │ Current  │            │           │
│  │(5V reg) │    │ sensor   │            │           │
│  └─────────┘    └──────────┘            │           │
└─────────────────────────────────────────┼───────────┘
                                          ▼
                                       Motor
```

Key components:
- **MCU** — tiny microcontroller that runs the ESC firmware
- **MOSFETs** — power transistors that switch battery voltage to motor phases
- **Gate Driver** — amplifies MCU signal to drive MOSFETs
- **BEC** — Battery Eliminator Circuit, outputs regulated 5V for the FC

---

## ESC Signal Protocols

The FC talks to the ESC using one of these protocols. Newer = better.

### 1. Standard PWM (legacy)

```
Signal: 1ms–2ms pulse, 50Hz repeat rate

1ms  = 0% throttle (minimum)
1.5ms = 50% throttle
2ms  = 100% throttle

Timeline: ──┐  ┌──────────────────────────────┐  ┌──
            └──┘  (pulse width varies)          └──┘
            ↑ 1ms–2ms pulse every 20ms (50Hz)
```

Problems: slow (50Hz = 20ms update rate), analog precision, susceptible to noise.

### 2. Oneshot125

Same as PWM but pulse width is 125µs–250µs (8× shorter) and rate up to 2000Hz.  
Better than PWM but still analog timing.

### 3. Multishot

Pulse: 5µs–25µs. Even faster. Still analog timing.

### 4. DSHOT (Digital Shot) — Current Standard

**Fully digital.** No analog timing issues. The throttle value is encoded as a number sent over serial.

```
DSHOT frame: 16 bits total
  [11 bits throttle value] [1 bit telemetry request] [4 bits CRC]

Throttle range:
  0     = disarmed
  48    = minimum throttle (motor beeps)
  2047  = maximum throttle

CRC = checksum to detect errors
```

DSHOT variants:

| Protocol | Bitrate | Max Update Rate |
|----------|---------|-----------------|
| DSHOT150 | 150 kbps | 3.75 kHz |
| DSHOT300 | 300 kbps | 7.5 kHz |
| DSHOT600 | 600 kbps | 15 kHz |
| DSHOT1200 | 1.2 Mbps | 30 kHz |

**DSHOT300** is the most common choice. **DSHOT600** for high-performance builds.

Benefits of DSHOT:
- Error detection (CRC)
- No calibration needed
- Supports **bidirectional DSHOT** — ESC can send telemetry back (RPM, voltage, current, temperature)

---

## ESC Arming Sequence

For safety, ESCs won't spin motors unless they go through an arming sequence.

### Traditional PWM Arming

```
Power on ESC with throttle at ZERO (1ms pulse):
  1. ESC hears 1ms → enters "ready" mode
  2. FC sends arm command (via switch or stick combo)
  3. Motors play startup tones
  4. Throttle above 1ms → motors spin

If ESC powers on with throttle NOT at zero → enters calibration mode!
```

### DSHOT Arming

Cleaner. Throttle value 0 = disarmed. Throttle value 48+ = minimum spin.  
No calibration needed. FC sends a DSHOT "arm" command when the pilot arms.

### The Arming Stick Combo (typical Betaflight default)

```
Throttle: DOWN
Yaw: RIGHT
(hold for 1 second)
→ FC arms, ESCs start accepting throttle commands
```

---

## Throttle Calibration (PWM only)

Because PWM is analog, the ESC needs to learn what "min" and "max" look like.

```
Calibration procedure:
1. Power on FC with throttle at MAX (2ms pulse)
2. Plug in battery — ESC enters calibration mode (plays tones)
3. Lower throttle to MIN (1ms pulse)
4. ESC plays confirmation tones → calibration saved

Now ESC knows:
  1ms = 0%
  2ms = 100%
```

With DSHOT this is not needed — 0 and 2047 are hardcoded values.

---

## ESC Firmware

The ESC runs firmware just like your ESP32. Common open-source options:

| Firmware | Notes |
|----------|-------|
| **BLHeli_S** | Older, supports Multishot, many budget ESCs |
| **BLHeli_32** | 32-bit MCU, supports DSHOT, bidirectional DSHOT, telemetry |
| **AM32** | Open source, active development, DSHOT + bidir DSHOT |
| **Bluejay** | Fork of BLHeli_S, open source, DSHOT on 8-bit MCUs |

You can flash ESC firmware over the signal wire using your flight controller — no special programmer needed.

---

## 4-in-1 ESC vs Individual ESCs

### Individual ESCs (4 separate boards)

```
FC ──► ESC1 ──► Motor1
FC ──► ESC2 ──► Motor2
FC ──► ESC3 ──► Motor3
FC ──► ESC4 ──► Motor4
Battery ──► each ESC individually
```

Pros: replace one ESC if it fails, easier troubleshooting  
Cons: lots of wiring, more weight, more soldering

### 4-in-1 ESC (one board, all 4 ESCs)

```
FC ──(4 signal wires)──► [4in1 ESC] ──► 4 Motors
Battery ──► [4in1 ESC]
```

Pros: much less wiring, neat build, one power connection  
Cons: if the board fails, you lose all 4 ESCs

**4-in-1 is standard for modern 5" builds.**

---

## ESC Current Rating

The most important spec: **Amperage (A) rating**.

Every ESC has a continuous and a burst rating:
- **Continuous:** 35A — can sustain indefinitely
- **Burst:** 45A for 10 seconds — for brief throttle punches

Your motor at max throttle draws X amps. Your ESC continuous rating must be ≥ X.

```
Example:
  Motor max current (from datasheet): 45A
  ESC needed: 45A continuous minimum

  Safe choice: 55A ESC (some headroom)
```

**Rule of thumb:** ESC amperage ≥ 1.2× motor max current.

---

## BEC — The 5V Regulator

Most ESCs include a BEC that takes battery voltage (e.g., 16.8V on 4S) and outputs 5V to power the FC, receiver, and other 5V devices.

```
LiPo (16.8V) ──► [BEC inside ESC] ──► 5V ──► FC, RX, camera

Types:
  Linear BEC  — simple, wastes energy as heat, max ~1A
  Switching BEC (SBEC) — efficient, up to 3–5A, slight electrical noise
```

Most FCs also have a built-in voltage regulator but rely on the ESC's BEC for convenience.

---

## Motor Beep Codes

When you power up, ESCs play tones to tell you their status.

| Beep Pattern | Meaning |
|--------------|---------|
| 1 long beep | Battery detected |
| 2 short beeps | Battery cell count confirmed |
| 3 short beeps | Ready, no errors |
| Continuous melody | Throttle not at zero (danger!) |
| Rising tones | Calibration mode entered |
| Error melody | See ESC manual |

The "continuous melody" on arming means you powered up with throttle too high — **lower throttle immediately**.

---

## Connecting ESC to FC

```
ESC ──► FC connections:

Signal (white/yellow) ──► FC motor output pin
Ground (black/brown) ──► FC GND (optional if ESC and FC share power ground)

4-in-1 ESC often uses a JST connector:
  M1_SIG, M2_SIG, M3_SIG, M4_SIG, GND, TELEM
```

Betaflight motor numbering:

```
         Front
  M1  ●        ● M2
      \        /
       ────────
      /        \
  M3  ●        ● M4
         Back
```

Connect ESC1 to FC Motor 1 output, etc. Match this exactly or the drone will flip.

---

## Practice

1. What does an ESC do at a basic level? Why can't the FC directly drive a brushless motor?
2. What is the DSHOT throttle value range? What does value 0 mean?
3. Why doesn't DSHOT need calibration but PWM does?
4. Your motor draws 40A at full throttle. What is the minimum ESC amperage rating you should use?
5. Power up your ESC and hear a continuous melody. What does this mean and what should you do?
6. What is a BEC and why is it useful in a drone build?
7. Explain what happens inside the ESC when the FC sends a DSHOT packet.
