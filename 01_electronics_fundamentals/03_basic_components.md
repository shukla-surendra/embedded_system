# 03 — Basic Electronic Components

These are the building blocks you'll encounter in every circuit. Learn to recognize them, understand what they do, and know their key specifications.

---

## Resistor

**What it does:** Limits current flow. Converts electrical energy into heat.

**Symbol in schematics:**
```
    ──[/\/\/]──
```

**How to read the value — color bands:**

Most through-hole resistors have 4 colored bands. The first three give the value, the fourth is tolerance.

```
Color    Digit    Multiplier
Black      0        ×1
Brown      1        ×10
Red        2        ×100
Orange     3        ×1,000
Yellow     4        ×10,000
Green      5        ×100,000
Blue       6        ×1,000,000
Violet     7
Gray       8
White      9
Gold                ×0.1      (tolerance: ±5%)
Silver              ×0.01     (tolerance: ±10%)
```

**Example:** Brown–Black–Red–Gold = 1–0–×100–±5% = **1000Ω = 1kΩ**

**Key specs:**
- Resistance value (Ω, kΩ, MΩ)
- Power rating (¼W is standard; use 1W or more for high-current paths)
- Tolerance (±1%, ±5%, ±10%)

**Tip:** Resistors have no polarity — you can put them in either direction.

---

## Capacitor

**What it does:** Stores electrical charge and releases it quickly. Smooths out voltage fluctuations ("decoupling"). Blocks DC, passes AC.

**Symbol:**
```
    ──|  |──    (non-polarized)
    ──|+ |──    (polarized/electrolytic)
```

**Two main types:**

| Type | Typical range | Polarity? | Look |
|------|--------------|-----------|------|
| Ceramic disc | 1pF–100nF | No | Small yellow/brown disc |
| Electrolytic | 1µF–10,000µF | YES — must be correct | Cylindrical aluminum can |

**Units:** Farads (F), microfarads (µF), nanofarads (nF), picofarads (pF)
```
1F = 1,000,000 µF = 1,000,000,000 nF = 1,000,000,000,000 pF
```

> **IMPORTANT:** Electrolytic capacitors have a **+** and **–** leg. Connect them backwards and they can bulge, leak, or explode. The longer leg is + (positive). The side with the stripe is – (negative).

**Common use in this course:** 100nF ceramic capacitor near every power pin of every chip — this filters high-frequency noise.

---

## LED (Light Emitting Diode)

**What it does:** Emits light when current flows through it in the correct direction.

**Symbol:**
```
    ──|>|──→ (light rays)
      A  K
    Anode  Cathode
```

**Key properties:**
- **Polarity matters** — current can only flow from Anode (+) to Cathode (−)
- **Forward voltage (Vf):** voltage drop across the LED when on (typically 1.8V–3.5V depending on color)
- **Max current:** typically 20mA for standard LEDs; exceeding this burns it out

**Identifying legs:**
- Longer leg = Anode (+)
- Shorter leg = Cathode (−)
- Flat side on the body = Cathode (−)

**Forward voltage by color (approximate):**
| Color | Vf |
|-------|-----|
| Red | 1.8–2.2V |
| Yellow | 2.0–2.2V |
| Green | 2.0–3.5V |
| Blue | 3.0–3.5V |
| White | 3.0–3.5V |

Always use a series resistor with an LED. (You calculated how in the previous module.)

---

## Diode

**What it does:** Allows current to flow in only one direction. Acts like a one-way valve.

**Symbol:**
```
    ──|>|──
      A  K
```

**Key use cases:**
- Protect circuits from reverse power connection
- Flyback diode — protects against voltage spikes from motors/relays
- Rectifier — converts AC to DC

**Key spec — forward voltage:** 0.6–0.7V for silicon diodes (this voltage is "used up" across the diode).

The most common diode you'll use: **1N4007** (general purpose, handles up to 1A and 1000V).

---

## Transistor

**What it does:** An electrically controlled switch (or amplifier). A small current/voltage at the control pin allows a large current to flow through the other two pins.

**Two main types:**
- **BJT (Bipolar Junction Transistor):** current-controlled switch
- **MOSFET:** voltage-controlled switch — preferred for power applications

**Why you need transistors:** Your ESP32 GPIO pins output only ~3.3V and ~12mA. A motor or relay might need 12V and 2A. A transistor lets the ESP32's tiny signal control the large load.

**BJT (NPN type) — 3 pins:**
```
         Collector (C) → load connects here
         |
Base ────┤  NPN
         |
         Emitter (E) → GND
```

Small current into Base → large current flows Collector to Emitter.

**MOSFET (N-channel) — 3 pins:**
```
         Drain (D) → load connects here
         |
Gate ────┤  N-ch MOSFET
         |
         Source (S) → GND
```

Voltage on Gate → MOSFET turns on, current flows Drain to Source.

Common beginners' transistors:
- **2N2222 / BC547** — NPN BJT, small signals up to ~600mA
- **IRLZ44N** — N-channel MOSFET, handles up to 47A, logic-level gate (works with 3.3V ESP32)

---

## Switch & Button

**What it does:** Mechanically breaks or completes a circuit.

**Tactile push button (momentary):** Completes circuit only while pressed. Used for resets, user input.

**Key concept — pull-up and pull-down resistors:**

When a button is not pressed, the GPIO pin it connects to is "floating" — it might randomly read high or low. A resistor tied to VCC (pull-up) or GND (pull-down) gives it a defined default state.

```
Pull-up:                    Pull-down:
3.3V ──[10kΩ]──┬── GPIO    GPIO ──┬──[10kΩ]── GND
                │                  │
               [BTN]              [BTN]
                │                  │
               GND               3.3V

Default = HIGH                Default = LOW
Pressed = LOW                 Pressed = HIGH
```

The ESP32 has built-in pull-up resistors you can enable in software — so often you don't need an external resistor.

---

## Power Components

### Voltage Regulator
Converts a higher voltage to a fixed lower voltage.

- **LM7805** — converts up to 35V down to 5V (linear, gets hot, wastes energy as heat)
- **AMS1117-3.3** — converts 5V down to 3.3V (common on dev boards)
- **Buck converter** — efficient switching regulator (covered in Module 06)

### Crystal Oscillator
Provides a precise clock frequency to a microcontroller. Some dev boards include one; the ESP32 has an internal oscillator.

---

## Quick Reference — Polarity Summary

| Component | Has polarity? |
|-----------|--------------|
| Resistor | No |
| Ceramic capacitor | No |
| Electrolytic capacitor | **YES** |
| LED | **YES** |
| Diode | **YES** |
| Transistor | **YES** (3 distinct pins) |

---

## Practice

1. You have a resistor with bands: Orange–Orange–Brown–Gold. What is its value?

2. An electrolytic capacitor has a stripe on one side and one leg longer than the other. You need to connect it between 5V and GND. Which leg goes to 5V?

3. You want to use an ESP32 (3.3V GPIO, 12mA max) to switch on a 12V, 500mA motor. Which component do you need between the GPIO pin and the motor? (transistor / resistor / capacitor)

**Answers:**
1. Orange(3)–Orange(3)–Brown(×10)–Gold(±5%) = 330Ω ±5%
2. The longer leg (Anode/+) goes to 5V
3. Transistor (a MOSFET like IRLZ44N is ideal)

---

**Next:** [04 — Reading Circuits](04_reading_circuits.md)
