# 03 — AMS1117 5V Step-Down Power Module

## What Is It?

The **AMS1117-5.0** is a fixed 5V low-dropout linear voltage regulator — the same AMS1117 chip family as the 3.3V module, but configured for a **5V output** instead of 3.3V.

- **Input:** 6.5V–12V DC
- **Output:** Fixed 5V DC, up to **800 mA continuous**
- **Use:** Step down a 9V battery, 12V adapter, or 2S LiPo (8.4V) to clean 5V

**For this project:** powers the ESP32 (which accepts 5V on its VIN/USB pin), the HC-SR04 ultrasonic sensor (needs 5V), and the NRF24L01 adapter board (which accepts 5V and internally steps to 3.3V).

---

## AMS1117-5V vs AMS1117-3.3V — At a Glance

| | AMS1117-3.3V | AMS1117-5V |
|-|-------------|-----------|
| Output | 3.3V | 5.0V |
| Min input | ~4.5V | ~6.5V |
| Powers | NRF24L01, sensors | ESP32 (VIN), HC-SR04, servo |
| From 9V battery | Works | Works |
| From USB (5V) | Works | **Does NOT work** — 5V in cannot produce 5V out |
| From 1S LiPo (3.7V) | Does NOT work | Does NOT work |
| From 2S LiPo (8.4V) | Works | Works |

The 5V module **cannot** be fed from a USB power bank (5V in, 5V out = 0V dropout — impossible for a linear regulator). Use a 9V or higher source.

---

## Dropout Voltage

The AMS1117-5.0 needs at least **1.3V headroom** above the output:

```
Minimum input for 5V out: 5.0 + 1.3 = 6.3V (use 6.5V+ to be safe)

Works fine:   9V → 5V   (4V dropout — no problem)
Works fine:  12V → 5V   (7V dropout — regulator gets hot)
Does NOT:     6V → 5V   (only 1V dropout — below spec)
Does NOT:     5V → 5V   (0V dropout — physically impossible)
```

---

## Hardware Overview

```
AMS1117-5V Module Pins:

  IN   ── DC input (6.5V to 12V)
  GND  ── Common ground
  OUT  ── 5V regulated output
```

Some modules have screw terminals; others have 3-pin headers or pads. All variants have the same 3-terminal function.

---

## Wiring Diagram

### Powering ESP32 from a 9V Battery

```
9V Battery (+) ──── AMS1117-5V IN
9V Battery (-) ──── AMS1117-5V GND ──── ESP32 GND

AMS1117-5V OUT (5V) ──── ESP32 VIN pin
```

> The ESP32 VIN pin expects 5V. It passes through the ESP32's own AMS1117-3.3V onboard regulator to power the ESP32 at 3.3V.

### Powering ESP32 + HC-SR04 + NRF24L01 Adapter from 9V

```
9V Battery (+) ─────────────── AMS1117-5V IN
9V Battery (-) ─────────────── AMS1117-5V GND
                                      │
AMS1117-5V OUT (5V) ──┬──── ESP32 VIN
                      ├──── HC-SR04 VCC
                      └──── NRF24L01 Adapter VCC
                                      │
                              Common GND for all
```

All three devices run from the same 5V rail. The NRF24L01 adapter steps 5V → 3.3V internally. The ESP32 steps 5V → 3.3V internally.

---

## Heat Dissipation

A linear regulator wastes voltage as heat. At 9V in, 5V out:

```
Power wasted = (Vin - Vout) × Current = (9 - 5) × I = 4 × I watts
```

| Load Current | Heat Generated | Hot? |
|-------------|---------------|------|
| 100 mA | 0.4W | Warm |
| 300 mA | 1.2W | Hot — may need heatsink |
| 500 mA | 2.0W | Very hot — needs heatsink |
| 800 mA (max) | 3.2W | Thermal shutdown risk |

At 12V input the heat doubles:
```
(12 - 5) × 0.3A = 2.1W — hot even at moderate current
```

**Use 9V input rather than 12V when possible.**
For loads above 300 mA from 9V, consider a buck converter (MP1584, LM2596) instead — far more efficient.

---

## Code Example — Checking 5V Supply Stability on ESP32

The ESP32 cannot directly measure its own 5V VIN pin (the ADC only sees 0–3.3V). But you can use a voltage divider to bring 5V down to a safe level for the ADC:

```
5V rail ──[10kΩ]──┬── GPIO34
                  │
                [22kΩ]
                  │
                GND
```

ADC reading = 5V × (22 / (10 + 22)) = 5V × 0.6875 = 3.44V → still slightly above 3.3V.
Use a 100kΩ + 100kΩ divider instead for a clean 2.5V at the ADC pin:

```
5V ──[100kΩ]──┬── GPIO34
              │
           [100kΩ]
              │
             GND
```

```cpp
#define V5_SENSE_PIN 34

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);   // 0–3.3V range
}

float read5VRail() {
  int raw = analogRead(V5_SENSE_PIN);
  float vADC = raw * (3.3 / 4095.0);
  float v5   = vADC * 2.0;    // ×2 because 100k/100k divider halves the voltage
  return v5;
}

void loop() {
  float v = read5VRail();
  Serial.print("5V rail: ");
  Serial.print(v, 2);
  Serial.println(" V");

  if (v < 4.7) Serial.println("WARNING: 5V low — check supply or battery");
  if (v > 5.3) Serial.println("WARNING: 5V high — check regulator");

  delay(1000);
}
```

Expected: 4.90–5.10V from a healthy AMS1117-5V module.

---

## When to Use 5V vs 3.3V Regulator

| Need | Use |
|------|-----|
| Power ESP32 from 9V battery | AMS1117-5V (output to VIN) |
| Power HC-SR04 from 9V battery | AMS1117-5V |
| Power NRF24L01 adapter from 9V | AMS1117-5V (adapter takes 5V) |
| Power NRF24L01 directly (no adapter) | AMS1117-3.3V |
| Power sensors (MPU, BMP, OLED) | 3.3V from ESP32 or AMS1117-3.3V |
| Power servos (drone gimbal) | AMS1117-5V or dedicated UBEC |

---

## Using Both AMS1117 Modules Together

In a complete drone build:

```
2S LiPo (8.4V) ─────────────────────────┐
                                         │
              ┌──── AMS1117-5V ─────────►│ ESP32 VIN
              │                           │ HC-SR04 VCC
              │                           │ NRF24L01 Adapter VCC
              │
              └──── AMS1117-3.3V ───────►│ MPU-6050 VCC (if separate from ESP32)
                                          │ BMP280 VCC
                                          │ OLED VCC
                                          │ NRF24L01 (if using without adapter)
```

Or more simply — let the ESP32's onboard 3.3V regulator power the 3.3V sensors:

```
2S LiPo → AMS1117-5V → ESP32 VIN
                         └── ESP32 3.3V pin → all sensors
```

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| Output is 0V | Input below 6.5V | Use 9V+ input |
| Output is 3.5V instead of 5V | Bought 3.3V version by mistake | Check chip marking — 1117-5.0 vs 1117-3.3 |
| Module very hot | High input voltage + high current | Lower input to 9V, add heatsink |
| ESP32 resets randomly | 5V dips during NRF TX burst | Add 100µF cap at ESP32 VIN |
| USB connected + 5V module connected | Two 5V sources fighting | Disconnect USB when using external 5V — or only use one at a time |

---

## Polarity Warning

The AMS1117 has **no reverse-polarity protection**. Connecting the input backwards will destroy the module instantly.

Always verify + and − before connecting a battery for the first time. Use a multimeter.

---

## What to Try

1. Connect 9V battery → measure output with multimeter → confirm 5V ±0.1V
2. Connect ESP32 VIN to output → upload a sketch → confirm it runs from the battery
3. Connect HC-SR04 VCC to the 5V output → run the ultrasonic example
4. Add the voltage divider circuit → run the monitoring sketch → watch voltage on Serial Monitor
5. Load up 3 devices (ESP32 + HC-SR04 + NRF24L01 adapter) → touch the AMS1117 chip → should be warm, not burning hot

---

## Next Steps

- **02_ams1117_power_module.md** — the 3.3V companion module
- **06_power_systems/01_lipo_batteries.md** — choosing the right battery for the whole drone
- **07_drone_electronics** — complete drone power wiring diagram
