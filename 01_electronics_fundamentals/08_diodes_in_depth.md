# 08 — Diodes: Complete Reference

## What a Diode Actually Does

A diode is a one-way valve for current. When voltage is applied in the forward direction (anode + to cathode −), current flows. When reversed, current is blocked.

```
Schematic symbol:

  Anode (A) ──|>|── Cathode (K)
              ↑
        current flows this way (A to K)

The triangle points in the direction of conventional current flow.
The bar at the tip is the cathode.
```

A perfect diode would have zero resistance forward and infinite resistance reverse. Real diodes have a **forward voltage drop** (typically 0.6–0.7V for silicon) and a small **reverse leakage current**.

---

## The VI Characteristic Curve

A diode does not obey Ohm's law. Its current-voltage relationship is exponential:

```
I = Is × (e^(V/Vt) − 1)

Is = reverse saturation current (tiny, ~10⁻¹⁴ A)
Vt = thermal voltage (~26mV at room temperature)
```

In practice this means:

```
Forward bias (A more positive than K):
  V < 0.5V: tiny current
  V ≈ 0.6V: current begins to flow noticeably
  V ≈ 0.7V: diode "fully on" — acts like a short with 0.7V drop
  V > 0.8V: current increases rapidly → damages diode

Reverse bias (K more positive than A):
  Current = essentially zero (leakage current only, µA range)
  V > reverse breakdown voltage: current increases rapidly → damage
  (Zener diodes exploit this breakdown region deliberately)
```

---

## Diode Types — Complete Reference

### 1. Rectifier Diode (1N4007, 1N4001 series)

**Purpose:** Convert AC to DC, reverse polarity protection

| Spec | 1N4007 | 1N4001 |
|------|--------|--------|
| Max current | 1A | 1A |
| Peak reverse voltage | 1000V | 50V |
| Forward voltage | ~0.7V | ~0.7V |
| Recovery time | Slow (~30µs) | Slow |
| Package | DO-41 through-hole | DO-41 |

**Use in this project:**
- Flyback protection for relay coils and motor drivers
- Reverse polarity protection on battery input

```
Reverse polarity protection:

Battery + ──[1N4007]──► VCC of circuit
           (cathode toward VCC)

If battery is connected backwards, diode blocks.
If connected correctly, diode conducts with 0.7V loss.
```

### 2. Schottky Diode (1N5819, BAT42, BAT85, SS14)

**Purpose:** Fast switching, low voltage drop, battery charging circuits

| Spec | 1N5819 | SS14 |
|------|--------|------|
| Max current | 1A | 1A |
| Peak reverse voltage | 40V | 40V |
| Forward voltage | **~0.3V** | ~0.3V |
| Recovery time | **Very fast (<1ns)** | Very fast |
| Package | DO-41 through-hole | SMA SMD |

**Key advantages over silicon diodes:**
- Forward voltage 0.3V vs 0.7V — wastes less power, less voltage drop on the supply rail
- Extremely fast switching — works in RF and high-frequency circuits where 1N4007 would be too slow
- No minority carrier storage — can switch in nanoseconds

**Use in this project:**
- Battery input protection with less voltage loss (0.3V drop instead of 0.7V)
- Freewheeling diode across motor drivers (the speed matters)
- Any circuit powered from a 3.3V LiPo where 0.7V loss would be unacceptable

### 3. Zener Diode

**Purpose:** Voltage regulation, reference, clamping

A Zener diode operates in **reverse breakdown** at a controlled voltage (its Zener voltage). This makes it a simple shunt voltage regulator.

```
Symbol:
  ──|>|── (but with bent ends on the bar)
  
  The bent bar indicates reverse-breakdown operation
```

```
Zener voltage clamp:

Input (variable) ──[R]──┬── Output (clamped)
                        │
                    [Zener, eg 3.3V]
                    (cathode to output)
                        │
                       GND

When input > 3.3V: Zener conducts, clamps output to 3.3V
When input < 3.3V: Zener blocks, R has no current, output = input
```

Common values: 2.7V, 3.3V, 3.6V, 5.1V, 6.2V, 12V

**Power rating critical:** Unlike a regular diode, a Zener in regulation dissipates power continuously.
`P = Vz × I` — a 5.1V Zener with 50mA through it dissipates 0.255W — use a ½W or 1W Zener.

### 4. TVS Diode (Transient Voltage Suppressor)

**Purpose:** Protect circuits from voltage spikes (ESD, inductive kickback)

A TVS diode is like a very fast Zener — it clamps a spike to a safe voltage within picoseconds.

```
Stand-off voltage: normal circuit voltage (e.g. 5V)
Clamping voltage: spike is clamped to ~6.4V for a 5V TVS

Placement: across power rails and signal inputs
```

Used on: USB inputs, motor driver outputs, radio module power pins.

### 5. Signal Diode (1N4148, 1N914)

**Purpose:** Small-signal switching, logic circuits, signal routing

| Spec | 1N4148 |
|------|--------|
| Max current | 200mA |
| Reverse voltage | 100V |
| Forward voltage | ~0.7V |
| Recovery time | **4ns** — very fast |
| Package | DO-35 (small glass body) |

**Use:** waveform clamping, simple logic gates, OR-ing two signals, steering diode networks.

### 6. LED (Light Emitting Diode)

A diode where the forward-biased recombination energy is emitted as photons instead of heat.

- Forward voltage: 1.8V–3.5V depending on colour (much higher than silicon diodes)
- Max current: typically 20mA (exceeding this permanently reduces brightness)
- Always needs a series current-limiting resistor

Forward voltages by colour:

| Colour | Vf | Wavelength |
|--------|-----|------------|
| Infrared | 1.2–1.8V | >700nm |
| Red | 1.8–2.2V | 620–750nm |
| Orange | 2.0–2.2V | 590–620nm |
| Yellow | 2.0–2.2V | 570–590nm |
| Green | 2.0–3.5V | 500–570nm |
| Blue | 3.0–3.5V | 450–500nm |
| White | 3.0–3.5V | Broad spectrum |
| UV | 3.0–4.0V | <400nm |

### 7. Photodiode

Reverse-biased diode where reverse current is proportional to light intensity. Used in optical sensors, remote control receivers.

### 8. Rectifier Bridge (4 diodes in a bridge package)

Four diodes arranged to convert AC to pulsating DC. Common in power supplies.

```
AC ──┬─── D1 ──┬─── DC+
     │         │
     └─── D2 ──┘
     │         │
     ┌─── D3 ──┐
     │         │
AC ──┴─── D4 ──┴─── DC−
```

---

## Flyback / Freewheeling Diode — Critical for Motors and Relays

When current through an inductor (motor coil, relay coil) is suddenly interrupted, the collapsing magnetic field generates a large voltage spike — potentially 100V+ from a 5V circuit.

Without protection, this spike destroys transistors, MOSFETs, and microcontrollers.

```
Without protection:
                      [Relay/motor]
ESP32 GPIO ──[transistor]────┤├──── VCC
                              │
              Voltage spike here → 100V+ → destroys transistor

With flyback diode:
                      [Relay/motor]
ESP32 GPIO ──[transistor]────┤├──── VCC
                              │
                           [1N4007]   ← cathode to VCC
                           (anode to drain/collector)
                              │
                             GND

Spike current flows through diode instead of through transistor.
```

**Rule:** Every relay, motor, solenoid, or any inductive load must have a flyback diode.
- Through-hole: 1N4007
- Fast switching: 1N5819 Schottky (better for fast PWM)

---

## Diode Polarity Identification

### Through-hole

```
Band (stripe) on body = CATHODE (−)

[Anode end]──[grey body]──|stripe|  ← Cathode end

Current flows: Anode (+) → Cathode (−)
```

### LED

```
Longer lead = Anode (+)
Shorter lead = Cathode (−)
Flat side on package = Cathode (−)
```

### SMD Diodes

```
Usually marked with a line or "K" near cathode.
SOD-123: small bar on one end = cathode
```

---

## Forward Voltage and Voltage Budgets

Every diode in series with your power supply costs you its forward voltage. Track these in your circuit:

```
Example: 9V battery → through 1N4007 protection diode → AMS1117-5V

9V − 0.7V (diode) = 8.3V into regulator
AMS1117 minimum input for 5V out = 6.5V
8.3V > 6.5V ✓ — works fine

Using Schottky (1N5819) instead:
9V − 0.3V = 8.7V — even better, less heat in regulator
```

```
Example: 3.7V LiPo → through 1N4007 → circuit needing 3.3V

3.7V − 0.7V = 3.0V — NOT enough for AMS1117-3.3V (needs 4.5V min)
3.7V − 0.3V (Schottky) = 3.4V — still not enough for AMS1117

Conclusion: direct LiPo connection or no protection diode on 3.7V systems
```

---

## Measuring Diodes with a Multimeter

**Diode test mode** (diode symbol on meter):

1. Red probe → Anode, Black probe → Cathode
2. Forward biased: meter reads **0.5–0.7V** (silicon) or **0.2–0.4V** (Schottky)
3. Reverse probe direction → meter reads **OL** (overload, open circuit)

LED in diode mode: reads 1.8–3.5V depending on colour and may faintly glow.

**Open diode** (burned out): reads OL in both directions.
**Shorted diode**: reads ~0V in both directions.

---

## Diode Packages

| Package | Size | Through-hole or SMD | Common types |
|---------|------|---------------------|-------------|
| DO-41 | 5mm body | Through-hole | 1N4007, 1N4001 |
| DO-35 | 3.5mm glass | Through-hole | 1N4148 |
| DO-27 | Larger | Through-hole | Higher current rectifiers |
| SMA (DO-214AC) | SMD | SMD | SS14, US1M |
| SOD-123 | SMD | SMD | BAT42, small signal |
| SOT-23 | SMD | SMD | Dual diodes |

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| Diode installed backwards | Polarity confusion | Identify stripe = cathode; remeasure with multimeter |
| Circuit works but voltage is lower than expected | Forgot to account for 0.7V drop | Use Schottky or remove series diode if not needed |
| Transistor / MOSFET keeps failing | No flyback diode on inductive load | Add 1N4007 across every relay/motor coil |
| Diode gets hot | Exceeds current rating or insufficient cooling | Use higher-rated diode or heatsink |
| Zener doesn't clamp | Wrong orientation (cathode and anode swapped) | Zener must be reverse-biased to regulate |

---

## Quick Selection Guide

| Need | Diode |
|------|-------|
| Reverse polarity protection (>5V circuit) | 1N4007 |
| Reverse polarity protection (3.3V circuit) | 1N5819 Schottky (low drop) |
| Flyback on relay/motor | 1N4007 |
| Flyback on PWM motor driver | 1N5819 Schottky (fast recovery) |
| Fast signal switching | 1N4148 |
| Voltage clamp/reference | Zener (choose voltage = clamp voltage) |
| Spike protection on inputs | TVS diode |
| Visual indicator | LED + current resistor |

---

## Practice Questions

1. Which end of a 1N4007 is the cathode, and how do you identify it physically?

2. You have a relay coil connected to an ESP32 GPIO via a transistor. The relay runs on 5V. What protection diode do you add, where do you place it, and in which orientation?

3. A Schottky diode has a forward voltage of 0.3V and your circuit draws 500mA through it. How much power is wasted in the diode?

4. You measure a diode with a multimeter: forward reads 0.28V, reverse reads OL. What type of diode is it?

**Answers:**

1. The **stripe (band)** printed on the body marks the **cathode**. Current flows from unmarked end (anode) toward the striped end (cathode).

2. Place 1N4007 **across the relay coil** — anode to the transistor's collector (drain), cathode to VCC (5V). When the transistor switches off, the inductive spike drives the cathode above VCC, the diode conducts, and the energy is safely dissipated.

3. P = Vf × I = 0.3 × 0.5 = **0.15W** (150mW). A standard 1A Schottky handles this easily.

4. 0.28V forward drop indicates a **Schottky diode** (silicon would be 0.6–0.7V).
