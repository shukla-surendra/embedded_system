# 09 — BJT Transistors: Complete Reference

## What a BJT Transistor Actually Does

A **Bipolar Junction Transistor (BJT)** is a current-controlled switch and amplifier. A small current flowing into (or out of) the **Base** pin controls a much larger current flowing between **Collector** and **Emitter**.

```
Three terminals:
  B — Base     (control input)
  C — Collector (large current path, high side)
  E — Emitter  (large current path, low side)

NPN (most common):
         C
         │
    B ───┤  → small current into B
         │     → large current C to E
         E

PNP:
         E
         │
    B ───┤  → small current out of B
         │     → large current E to C
         C
```

The BJT does not turn on with voltage alone — it needs **current** at the base. This distinguishes it from a MOSFET (which is voltage-controlled).

---

## NPN vs PNP

| Property | NPN | PNP |
|----------|-----|-----|
| Current flow | C → E (when on) | E → C (when on) |
| Base activation | Current INTO base | Current OUT OF base |
| Switch position | **Low-side switch** (between load and GND) | High-side switch |
| Most common | **Yes** — use NPN for most switching | Less common |
| ESP32 compatible | Yes — 3.3V base drive works | Yes, with base resistor to VCC |

For switching loads with a microcontroller: **use NPN** — much simpler to drive.

---

## The Key Parameters

### hFE (DC Current Gain / Beta β)

The ratio of collector current to base current:

```
hFE = β = IC / IB

IC = hFE × IB

Example: hFE = 100, IB = 1mA → IC = 100mA
```

hFE varies significantly between devices of the same type (often 2:1 range), with temperature, and with collector current. **Never rely on a specific hFE value** — always design with the minimum datasheet value, then verify.

### VCE(sat) — Saturation Voltage

When fully switched on (saturated), there is still a voltage drop between collector and emitter — typically **0.2–0.4V**. This is wasted power and a heat source in power applications.

For comparison, a MOSFET's RDS(on) creates much less drop at the same current.

### VBE — Base-Emitter Voltage

The voltage across base and emitter when the transistor is conducting — approximately **0.6–0.7V** for silicon BJTs. This must be present for current to flow.

### VCEO — Maximum Collector-Emitter Voltage

The maximum voltage the transistor can block when off. Exceeding this causes breakdown. Common values: 30V–100V for general purpose BJTs.

### IC(max) — Maximum Collector Current

The maximum continuous current through the collector. Exceeding it causes thermal failure.

---

## Common BJT Transistors

### 2N2222 / PN2222 (NPN)

| Spec | Value |
|------|-------|
| IC max | 600mA |
| VCEO | 40V |
| hFE | 75–300 (varies widely) |
| VCE(sat) | ~0.3V at IC=150mA |
| Package | TO-92 (through-hole), SOT-23 (SMD) |

**Use:** Switching small loads — LEDs, small relays (up to 200mA coil), buzzers, logic level conversion.

### BC547 / BC548 (NPN)

| Spec | Value |
|------|-------|
| IC max | 100mA |
| VCEO | 45V |
| hFE | 110–800 |
| VCE(sat) | ~0.9V at IC=100mA |
| Package | TO-92 |

**Use:** Signal amplification, small LED drivers, logic circuits.

### BC557 / BC558 (PNP complement to BC547)

| Spec | Value |
|------|-------|
| IC max | 100mA |
| VCEO | −45V |
| hFE | 110–800 |
| Package | TO-92 |

**Use:** High-side switching, PNP switch driven from NPN.

### 2N3904 (NPN) / 2N3906 (PNP)

| Spec | Value |
|------|-------|
| IC max | 200mA |
| VCEO | 40V |
| hFE | 100–300 |
| Package | TO-92 |

**Use:** General purpose, very widely available.

### TIP31C (NPN, medium power)

| Spec | Value |
|------|-------|
| IC max | 3A |
| VCEO | 100V |
| hFE | 10–50 |
| VCE(sat) | ~1.2V at IC=3A |
| Package | TO-220 |

**Use:** Motor driving, relay coil driving, 12V load switching.

### TIP120 / TIP122 (NPN Darlington)

| Spec | Value |
|------|-------|
| IC max | 5A |
| VCEO | 60V / 100V |
| hFE | **1000 min** (Darlington pair) |
| VCE(sat) | ~2.0V (high — see Darlington section) |
| Package | TO-220 |

---

## TO-92 Package Pin Identification

The most common through-hole package for small BJTs:

```
Flat face toward you:

  BC547 / 2N2222 / 2N3904:
  ┌─────────┐  (flat side)
  │         │
  C    B    E   ← pins from left to right (flat face toward you)

  (varies by manufacturer — ALWAYS check the datasheet)
```

> **The pin order varies between manufacturers and part numbers.** A 2N2222 in a TO-92 package has a different pinout than a BC547 in the same package. Always verify with the datasheet or measure with a multimeter in diode mode.

### Identifying pins with a multimeter (diode mode):

NPN transistor:
- B to E (red on B, black on E): reads ~0.6V
- B to C (red on B, black on C): reads ~0.6V
- All other combinations: reads OL
- The pin that reads 0.6V to BOTH others = **Base**
- Of the remaining two: the one that reads ~0.6V from Base with red probe = same junction, but you determine E vs C by relative current — usually E is the longer lead

---

## Switching Circuit Design

### Step 1: Know your load

```
Load voltage: VCC_load (e.g. 5V or 12V)
Load current: IC_load (e.g. 200mA for a relay)
```

### Step 2: Choose a transistor

Pick a transistor where:
- IC(max) > IC_load × 2 (safety factor)
- VCEO > VCC_load × 2
- Package can dissipate: P = VCE(sat) × IC = 0.3V × 0.2A = 60mW (fine for TO-92)

### Step 3: Calculate base resistor

The transistor needs enough base current to saturate (fully on). Use a forced beta much lower than hFE:

```
IB_required = IC_load / hFE_forced

Use hFE_forced = 10 (conservative, guarantees saturation even with component variation)

IB = IC / 10 = 200mA / 10 = 20mA

Base resistor:
RB = (V_GPIO − VBE) / IB
RB = (3.3 − 0.7) / 0.020 = 2.6 / 0.020 = 130Ω

Use next lower standard value: 100Ω
(going lower = more base current = more saturated = lower VCE(sat))
```

### Complete NPN switch circuit

```
                    +5V or +12V
                       │
                    [Load]      ← relay coil, LED array, motor
                       │
                   Collector
ESP32 GPIO ──[RB]──── Base      NPN transistor (e.g. 2N2222)
                   Emitter
                       │
                      GND
```

### With flyback diode (inductive load):

```
+VCC
  │
[Relay coil]──┬── Collector
              │
           [1N4007]  ← cathode to +VCC, anode to collector
              │
           Collector
ESP32 ──[RB]── Base
           Emitter
              │
             GND
```

---

## The Darlington Pair

Two BJTs cascaded so the first transistor's emitter drives the base of the second. Combined hFE = hFE1 × hFE2.

```
       IC ←─── Collector ────────────────────────── C out
                                │
               Q1 Collector     │ Q2 Collector
       IB →─── Q1 Base         │
               Q1 Emitter ──── Q2 Base
                                Q2 Emitter ──── E out

Combined hFE ≈ 100 × 100 = 10,000
```

**Advantage:** Can be driven from a very small base current (even the leakage current of another transistor). A 1mA input controls 10A.

**Disadvantage:**
- VCE(sat) = ~2V (two VBE drops) — significant power loss
- Slow compared to a single transistor
- The TIP120/TIP122 Darlington dissipates: 2V × 5A = 10W at full current

For high-current switching with low gate drive current: consider a MOSFET instead — lower saturation voltage, faster.

---

## BJT as Amplifier (Linear Region)

When not fully saturated, the BJT amplifies:

```
Small signal on base → amplified signal at collector

Common-emitter amplifier:
         +VCC
           │
          [RC]      ← collector resistor
           │
           ├──── Vout (amplified, inverted)
           │
      Collector
Vin ──[RB]── Base
      Emitter
           │
          [RE]      ← emitter resistor (stabilizes gain)
           │
          GND

Voltage gain ≈ −RC / RE (minus = inverted)
```

**Why you care:** The ESP32 reads sensors that output small signals. Understanding amplifiers helps when sensor signals are too weak for the ADC.

For this drone project the most relevant use is **switching**, not amplification.

---

## BJT vs MOSFET — When to Use Each

| Factor | BJT | MOSFET |
|--------|-----|--------|
| Control type | Current (needs IB) | Voltage (no gate current) |
| Drive from 3.3V GPIO | Needs base resistor | Logic-level MOSFETs work directly |
| Saturation voltage | 0.2–2V (VCE_sat) | Much lower (RDS_on × I) |
| Switching speed | Moderate | Faster |
| High current | Needs Darlington or multiple devices | Single device handles 10–100A |
| Cost | Very low | Low to moderate |
| Simpler to use | Yes (no gate charge concerns) | No (but not difficult) |

**Rule of thumb:**
- Load < 500mA, 5V–12V, don't want to think about it: use BJT (2N2222)
- Load > 500mA, or PWM motor control, or need low dropout: use MOSFET

---

## Thermal Considerations

Power dissipated in a BJT = VCE(sat) × IC

```
2N2222 switching 200mA:
P = 0.3V × 0.2A = 60mW — fine, TO-92 can handle 625mW

TIP31C switching 2A:
P = 1.2V × 2A = 2.4W — TO-220 needs heatsink (max 2W without)

TIP120 Darlington at 3A:
P = 2.0V × 3A = 6W — definitely needs heatsink
```

Thermal resistance of TO-92 (no heatsink): ~200°C/W
At 60mW: temperature rise = 60mW × 200°C/W = 12°C above ambient → fine

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| Load won't switch on | Base resistor too large → not enough IB | Lower RB value; verify GPIO actually toggles HIGH |
| Transistor hot even when "off" | Leakage current or partial conduction | Check VBE is truly 0 when off |
| Transistor heats up and fails | VCE(sat) × IC exceeds package rating | Use MOSFET for lower VCE, or add heatsink |
| Wrong pins identified | Pinout varies between packages | Check datasheet for your exact part number |
| Works on bench, fails in circuit | hFE variation between units | Design with hFE_forced = 10, not datasheet typical |
| Relay clicks but load doesn't switch | No flyback diode, transistor damaged | Replace transistor, add 1N4007 across coil |

---

## Practice Questions

1. A 2N2222 (hFE min = 75) switches a 12V relay with a 150mA coil. What is the minimum base current needed to saturate it? Using a forced beta of 10, what base current should you design for?

2. The ESP32 GPIO outputs 3.3V and you want 20mA base current. What is the correct base resistor value (VBE = 0.7V)?

3. A TIP120 Darlington switches 4A at VCE(sat) = 2V. How much power is dissipated? Is a heatsink needed?

4. Why is a MOSFET usually preferred over a BJT for PWM motor control?

**Answers:**

1. Minimum IB = IC / hFE = 150mA / 75 = 2mA. Forced beta of 10: IB = 150mA / 10 = **15mA**.

2. RB = (3.3 − 0.7) / 0.020 = 2.6 / 0.020 = **130Ω** → use 120Ω or 100Ω.

3. P = 2V × 4A = **8W**. Yes, heatsink required — TO-220 without heatsink is rated ~2W.

4. MOSFETs switch faster (no minority carrier storage), have lower on-resistance (less heat), and require no gate current (GPIO drives the gate directly with no resistor calculation needed for saturation).
