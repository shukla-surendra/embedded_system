# 02 — AMS1117 3.3V Power Supply Module

## What Is It?

The **AMS1117** is a low-dropout (LDO) linear voltage regulator IC made by Advanced Monolithic Systems. The **3.3V module** version packages this chip with input/output capacitors on a small PCB, giving you a ready-to-use 3.3V power supply.

**Input:** 4.5V–12V DC  
**Output:** Fixed 3.3V DC, up to **800 mA continuous**

**For this project:** Powers the NRF24L01+PA+LNA module, which needs clean 3.3V and can draw up to 250 mA — more than the ESP32's onboard 3.3V regulator can reliably supply.

---

## What "LDO" Means

A **Low-Dropout** regulator needs only a small voltage difference between input and output to work.

The AMS1117-3.3 needs input to be at least **~4.5V** (1.2V higher than 3.3V output = "dropout voltage").

```
Works fine:    5V in  → 3.3V out   (1.7V dropout — fine)
Works fine:    9V in  → 3.3V out   (5.7V dropout — fine)
Works fine:   12V in  → 3.3V out   (8.7V dropout — fine)
Does NOT work: 3.7V in → 3.3V out  (0.4V dropout — too small)
```

> A single LiPo cell (3.7V nominal) is too close to the output — use a 2S LiPo (7.4V) or a USB power bank (5V) as the input source.

---

## Hardware Overview

The AMS1117 module has 3 pins (or screw terminals on some versions):

```
AMS1117 Module

  IN  ── DC input voltage (4.5V to 12V)
  GND ── Common ground
  OUT ── 3.3V regulated output
```

Some modules have two sets of pins (input side and output side) — the ground is shared.

```
┌─────────────────────────┐
│  IN+  IN-    OUT+  OUT- │
│  (vin)(gnd)  (3.3V)(gnd)│
└─────────────────────────┘
```

---

## How a Linear Regulator Works (Plain Language)

A linear regulator is like a variable resistor in series with your power line.
It burns the excess voltage as heat.

```
5V input → [AMS1117 dissipates 1.7V as heat] → 3.3V output
```

If you draw 500 mA from the output:
- Power dissipated as heat = (5 - 3.3) × 0.5 = **0.85 Watts**
- At 12V input drawing 500 mA: (12 - 3.3) × 0.5 = **4.35 Watts** — the chip gets very hot

**Rule of thumb:** Keep input voltage as low as practical (5V is ideal when 3.3V output is needed). Higher input = more heat, lower efficiency.

---

## Heat Dissipation

The AMS1117 chip has a maximum junction temperature of 125°C. Without a heatsink:

| Input Voltage | Output Current | Heat Generated | Safe?              |
|---------------|---------------|----------------|--------------------|
| 5V            | 500 mA        | 0.85W          | Yes, barely warm   |
| 5V            | 800 mA        | 1.36W          | Warm, monitor it   |
| 9V            | 500 mA        | 2.85W          | Hot — add heatsink |
| 12V           | 500 mA        | 4.35W          | Too hot — use buck |

For powering the NRF24L01 (max 250 mA) from 5V:  
`(5 - 3.3) × 0.25 = 0.425W` — perfectly fine, barely warm.

---

## Wiring Diagram

### Powering NRF24L01 from USB/5V via AMS1117

```
USB 5V ──────────────────── AMS1117 IN+
GND   ──────────────────── AMS1117 IN- (GND)
                                │
                           AMS1117 OUT+ ──── NRF24L01 VCC
                           AMS1117 OUT- ──── NRF24L01 GND
                                             (also ──── ESP32 GND)
```

> Always connect ESP32 GND and AMS1117 GND together. A common ground is essential — without it, signals between the devices have no reference and communication fails.

### Powering Both ESP32 and NRF24L01 from a 9V Battery

```
9V Battery (+) ─────────────── AMS1117 IN+
9V Battery (-) ─────────────── AMS1117 GND ──── ESP32 GND

AMS1117 OUT (3.3V) ──┬──── NRF24L01 VCC
                     └──── ESP32 3V3 pin (or VIN if module accepts 3.3V)
```

> The ESP32 development board also has its own AMS1117 onboard for its 5V→3.3V conversion. If you feed 3.3V directly to the ESP32's 3V3 pin, bypass its regulator — check your specific board's pinout.

---

## Adding Decoupling Capacitors

Even with the module's built-in capacitors, add capacitors close to the load (the NRF24L01 module):

```
AMS1117 OUT ──┬──── NRF24L01 VCC
              │
             [100µF electrolytic, + toward OUT]
              │
             [100nF ceramic]
              │
             GND
```

- **100µF electrolytic:** handles slow current spikes (NRF24L01 bursts during transmit)
- **100nF ceramic:** filters high-frequency noise from the radio

This combination is the #1 fix for NRF24L01 disconnection problems.

---

## Code Example — Verifying Clean 3.3V on ESP32

Read the ESP32's internal reference voltage to confirm your supply is stable:

```cpp
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  // ESP32 has an internal 1.1V reference you can read on GPIO 34
  // But the easiest check: measure a voltage divider
  // Here we just confirm ADC reads look stable

  int reading1 = analogRead(34);
  delay(10);
  int reading2 = analogRead(34);
  delay(10);
  int reading3 = analogRead(34);

  Serial.print("ADC readings: ");
  Serial.print(reading1); Serial.print(", ");
  Serial.print(reading2); Serial.print(", ");
  Serial.println(reading3);

  // If readings vary by more than 50 counts with nothing connected,
  // your power supply is noisy. Add caps.

  delay(500);
}
```

For a precise measurement, use a multimeter across the AMS1117 output pins.  
You should read **3.28–3.32V** at no load, and it should not drop below **3.20V** under full NRF24L01 TX load.

---

## AMS1117 vs Other 3.3V Options

| Option             | Max Current | Efficiency | Cost   | Best For                         |
|--------------------|-------------|------------|--------|----------------------------------|
| AMS1117 module     | 800 mA      | ~66% (5V→3.3V) | Very low | NRF24L01, sensors, ESP32 backup |
| ESP32 onboard reg  | ~500 mA     | ~66%       | Built-in | ESP32 itself + small sensors     |
| MP1584 (buck)      | 3A          | ~90%       | Low    | Motors, servos, high current     |
| LM7805             | 1A          | ~33% (12V) | Very low | Not for 3.3V — use AMS1117     |

For low-current 3.3V loads (sensors, radio modules): AMS1117 is the right choice.  
For high-current loads (motors, servos): use a switching buck converter instead.

---

## Common Problems

| Problem                            | Likely Cause                         | Fix                                                   |
|------------------------------------|--------------------------------------|-------------------------------------------------------|
| Output is 0V                       | Input too low (< 4.5V)               | Use 5V or higher input                                |
| Output is correct but NRF drops    | Current spike exceeds filter caps    | Add 100µF + 100nF caps at NRF24L01 VCC               |
| Module gets very hot               | High input voltage + high current    | Lower input voltage or switch to a buck converter     |
| ESP32 resets when NRF transmits    | Ground loop / shared power rail      | Ensure common GND between all modules                 |
| Output voltage is 3.1V or lower    | Under-loaded OR damaged regulator    | Check input voltage, replace module (₹20–30 each)    |
| ADC reads fluctuate wildly         | Noisy power supply                   | Add ceramic cap (100nF) at ADC pin to GND            |

---

## Real Use Case: The NRF24L01 Power Problem Solved

The NRF24L01+PA+LNA can draw 115–250 mA during transmission. The ESP32's built-in 3.3V regulator is rated 500 mA total — shared with the ESP32 itself (~80–200 mA). The math leaves almost nothing for the radio.

**Symptom without AMS1117:** NRF24L01 initializes, sends 1–2 packets, then goes silent. Or initializes fine on the bench but fails during actual transmission bursts.

**Solution:**
```
USB 5V → AMS1117 module → NRF24L01 VCC + 100µF cap
USB 5V → ESP32 USB port (powers ESP32 via its own regulator)
ESP32 GND ←──────────────→ AMS1117 GND  (shared ground!)
```

This completely isolates the NRF24L01 power from the ESP32 power — problem solved.

---

## Safety Notes

- Do not exceed **12V input** — the chip is rated to 15V absolute maximum but the module capacitors may not be
- Do not draw more than **800 mA** continuously — the IC will thermal-shutdown
- Polarity matters — reverse polarity will destroy the module instantly (no protection built in)
- The output is **not short-circuit protected** on all module versions — don't short the output pins

---

## What to Try

1. Connect 5V input → measure output with multimeter → confirm 3.3V
2. Wire NRF24L01 to AMS1117 output instead of ESP32 3V3 pin → compare connection reliability
3. Add 100µF cap across NRF24L01 VCC/GND → run NRF24L01 at PA_MAX → check for stability
4. Touch the AMS1117 chip after 5 minutes of NRF TX → should be warm but not burn-your-finger hot

---

## Next Steps

- **04_communication_protocols/04_nrf24l01_wireless.md** — power the NRF24L01 properly with this module
- **06_power_systems/01_lipo_batteries.md** — drone battery fundamentals
- **06_power_systems/03_power_distribution.md** — how to distribute power across the full drone
