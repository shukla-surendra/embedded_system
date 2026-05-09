# 05 — Capacitors: Complete Reference

## What a Capacitor Actually Does

A capacitor stores electrical charge between two conductive plates separated by an insulating material called the **dielectric**. When voltage rises, it absorbs charge. When voltage dips, it releases charge to fill the gap. This makes it the fundamental tool for:

- **Decoupling / bypass** — absorbs sudden current spikes from ICs and radio modules
- **Filtering** — smooths ripple from power supplies
- **Timing** — charges and discharges at a known rate (RC circuits)
- **Coupling / DC-blocking** — passes AC signals but blocks DC

Think of a capacitor as a tiny, fast rechargeable battery. It can't store much energy, but it responds in microseconds — far faster than any real battery.

---

## The Fundamental Formula

```
Q = C × V

Q = charge stored (Coulombs)
C = capacitance (Farads)
V = voltage across the capacitor
```

And the defining behaviour — current through a capacitor is proportional to how fast voltage changes:

```
I = C × (dV/dt)

A 100nF cap with voltage changing at 1V/µs:
I = 100×10⁻⁹ × 1×10⁶ = 0.1A (100mA)
```

This is why capacitors are so effective at absorbing switching current spikes — they supply current instantly when voltage tries to change.

---

## Units and Scale

```
1 F (Farad)       — huge, rare (supercapacitors)
1 mF (millifarad) = 0.001 F
1 µF (microfarad) = 0.000001 F        ← electrolytic range
1 nF (nanofarad)  = 0.000000001 F     ← ceramic disc range
1 pF (picofarad)  = 0.000000000001 F  ← small SMD ceramic

Conversions:
1 µF = 1000 nF = 1,000,000 pF
100 nF = 0.1 µF = 100,000 pF
22 pF = 0.022 nF = 0.000022 µF
```

---

## The Three Capacitors You Bought

### 1 — 100nF 50V Disc Capacitor (Ceramic)

**What it is:** A small ceramic disc, typically yellow or brown, marked "104" (= 10 × 10⁴ pF = 100,000 pF = 100 nF = 0.1 µF).

**The most important capacitor in electronics.** Used on literally every power supply pin of every IC.

**What it does:** Absorbs the high-frequency current spikes that ICs create when their internal logic switches (hundreds of millions of times per second). Without it, these spikes travel down the power rail and cause other chips to malfunction.

**No polarity** — can be wired either direction.

**Key specs:**
- Capacitance: 100 nF (0.1 µF)
- Voltage rating: 50V (your supply is 3.3V–5V — huge safety margin)
- Type: Class 2 ceramic (X7R or Y5V typically for disc capacitors)
- Temperature: degrades at extremes but fine for room temperature

**Where to use it:**

```
3.3V ──┬── IC VCC pin
       │
     [100nF]    ← placed as close to IC as physically possible
       │
      GND

Rule: one 100nF cap per IC power pin, touching the IC.
```

**Reading the marking:**
```
Disc cap markings use 3-digit code: XYZ = XY × 10^Z picofarads
104 → 10 × 10⁴ = 100,000 pF = 100 nF ✓
103 → 10 × 10³ = 10,000 pF = 10 nF
472 → 47 × 10² = 4,700 pF = 4.7 nF
```

---

### 2 — 10µF 50V Electrolytic Capacitor (Two Variants)

You have two different brands/types of the same value: **10µF, 50V**.

**What it is:** A cylindrical aluminum can with two leads, one positive and one negative.

**AISHI ERS1HM100D11OT** and **HYNCDZ HY1H100M050110CD288** — both are standard aluminum electrolytic capacitors. The HYNCDZ variant lists ESR of 1.82Ω at 100kHz and ripple current 120mA — these are quality specs relevant for power supply design.

**What it does:** Handles slower, larger current surges that the 100nF ceramic can't absorb. The electrolytic responds in microseconds, the ceramic in nanoseconds — they work together.

**HAS POLARITY — critical:**

```
Electrolytic capacitor:

    Longer lead = POSITIVE (+)
    Shorter lead = NEGATIVE (−)

    The stripe printed on the body = NEGATIVE side

    + lead → higher voltage (e.g. 3.3V, 5V, VCC)
    − lead → lower voltage (GND)
```

Connect backwards → capacitor heats up, bulges, leaks, or explodes. This is not theoretical — it happens within seconds.

**Voltage rating (50V):** Always use a capacitor rated for more than your circuit voltage. With 5V supplies, 50V gives a 10× margin — correct. Never use a 10V cap on a 5V supply; the capacitor degrades faster and is more likely to fail.

**Key specs of the HYNCDZ variant:**
- ESR: 1.82Ω at 100kHz — ESR is the internal resistance; lower is better for fast switching
- Ripple current: 120mA — maximum AC current it can handle continuously before overheating
- Temperature range: typically −40°C to +85°C

**Where to use it:**

```
5V ──┬── (to load)
     │
   [10µF]  +lead to 5V, −lead to GND
     │
    GND

Typical placement: at the input and output of every voltage regulator,
and at the ESP32 VIN pin when running from a battery.
```

---

### 3 — 22pF 50V C0G/NP0 SMD Ceramic (0402 / 1005 Metric)

**What it is:** A tiny surface-mount ceramic capacitor, 1.0mm × 0.5mm. Requires soldering under magnification.

**C0G / NP0** is the dielectric type — the most stable ceramic capacitor class. "NP0" means Negative Positive Zero — its capacitance changes essentially zero with temperature, voltage, and age.

**What it does:** Precision timing, oscillator load capacitors, RF filtering. The 22pF value at this precision is specifically used as a **crystal load capacitor** — required on each leg of an 8 MHz or 16 MHz crystal oscillator.

**Key specs:**
- Capacitance: 22 pF ±5% (extremely tight tolerance)
- Voltage: 50V
- Dielectric: C0G (NP0)
- Size: 0402 imperial (1.0mm × 0.5mm)
- Capacitance drift: ±30 ppm/°C — essentially zero over the temperature range

**Where to use it:**

```
Crystal oscillator load capacitor circuit:

ESP32 XTAL_IN ──[22pF]── GND
ESP32 XTAL_OUT ──[22pF]── GND
```

Note: the ESP32 has an internal oscillator and does not require external crystals in most projects. The 22pF caps are useful for any external oscillator or precision timing circuit.

---

## All Capacitor Types — Complete Reference

### Class 1 Ceramics (C0G / NP0, U2J)

| Property | Value |
|----------|-------|
| Typical range | 1pF – 1nF |
| Tolerance | ±5% or better |
| Temperature coefficient | Near zero (±30 ppm/°C) |
| Voltage effect on capacitance | None |
| Aging | None |
| Best for | Precision timing, RF, oscillators |
| Your part | 22pF C0G 0402 |

### Class 2 Ceramics (X7R, X5R, Y5V, Z5U)

| Code | Temp range | Capacitance change | Best for |
|------|-----------|-------------------|----------|
| X7R | −55°C to +125°C | ±15% | General decoupling, good stability |
| X5R | −55°C to +85°C | ±15% | Same, lower temp rating |
| Y5V | −30°C to +85°C | +22% / −82% | High capacitance, poor stability |
| Z5U | +10°C to +85°C | +22% / −56% | Avoid for precision work |

Your 100nF disc cap is likely X7R or Y5V. For decoupling (its primary use) the stability doesn't matter much.

**Critical: Class 2 ceramic loses capacitance with applied voltage:**
```
100nF X5R cap rated 10V:
  At 0V:  reads 100nF (full value)
  At 5V:  reads ~60nF (40% loss)
  At 9V:  reads ~30nF (70% loss!)
```
This is called DC bias effect. Always use a cap with voltage rating 2–3× your actual voltage. Your 50V rated cap at 5V is fine.

### Aluminum Electrolytic

| Property | Value |
|----------|-------|
| Typical range | 1µF – 10,000µF |
| Tolerance | ±20% typical |
| Polarity | YES — critical |
| ESR | 0.1Ω – 10Ω (lower is better) |
| Lifespan | Limited — electrolyte evaporates over years |
| Temperature effect | Significant at extremes |
| Best for | Bulk power supply filtering, decoupling at moderate frequencies |
| Your part | 10µF 50V (both variants) |

**Lifespan formula:** electrolytic capacitors last ~1000–5000 hours at rated temperature. Every 10°C cooler doubles the lifespan. At room temperature (25°C, 40°C below the 85°C rating): lifespan × 2^4 = ×16 — essentially unlimited for your usage.

### Tantalum Electrolytic

| Property | Value |
|----------|-------|
| Typical range | 0.1µF – 470µF |
| Polarity | YES |
| ESR | Very low (0.01Ω – 2Ω) |
| Failure mode | **Can catch fire if reverse-biased or overvoltaged** |
| Best for | Low ESR bulk decoupling in tight spaces |
| Note | Always derate voltage to 50% (use 10V cap at ≤5V) |

You don't have these, but know they exist and are dangerous if misused.

### Film Capacitors (Polyester, Polypropylene)

| Property | Value |
|----------|-------|
| Typical range | 1nF – 10µF |
| Polarity | No |
| Stability | Excellent |
| ESR | Very low |
| Best for | Audio circuits, motor drives, precision filters |
| Note | Larger physical size than ceramic |

---

## Voltage Rating — The Rules

Never operate a capacitor above its voltage rating. The dielectric breaks down → short circuit → component destruction.

**Safe derating by type:**

| Type | Recommended operating voltage |
|------|-------------------------------|
| Ceramic (Class 1) | Up to rated voltage |
| Ceramic (Class 2) | 50–80% of rated (capacitance drops with voltage) |
| Aluminum electrolytic | 80% of rated |
| Tantalum | **50% of rated** (safety critical) |

Your 50V rated caps on 5V circuits: all well within safe limits. ✓

---

## ESR — Equivalent Series Resistance

Every real capacitor has a small internal resistance called **ESR** (Equivalent Series Resistance).

```
Real capacitor = ideal capacitor + ESR resistor in series

Low ESR = better at handling high-frequency ripple current
High ESR = generates more heat = less effective at filtering
```

ESR matters most in:
- Switching power supplies (the output cap sees high ripple current)
- NRF24L01 decoupling (fast current bursts at radio frequency)
- Motor driver output filtering

Your HYNCDZ 10µF cap: ESR = 1.82Ω at 100kHz — acceptable for general use.
For comparison, a low-ESR electrolytic might be 0.1Ω. A ceramic is 0.01–0.1Ω.

For the NRF24L01 specifically: **pair 10µF electrolytic (handles bulk) with 100nF ceramic (handles high-frequency)**. The ceramic has far lower ESR and handles the radio-frequency switching; the electrolytic handles the larger, slower current burst.

---

## Capacitors in RC Timing Circuits

Capacitors charge and discharge through resistors at a predictable rate. The **time constant**:

```
τ = R × C  (in seconds, when R is in Ohms and C in Farads)

After 1τ: capacitor is 63.2% charged
After 2τ: 86.5% charged
After 5τ: 99.3% charged (considered "fully" charged)
```

**Example:** 10kΩ resistor + 100nF capacitor:
```
τ = 10,000 × 0.0000001 = 0.001 seconds = 1ms
Fully charged in ~5ms
```

This is used in debounce circuits, delay timers, and sensor filtering.

---

## How to Read Capacitor Markings

### Through-Hole Ceramic (3-digit code)
```
XYZ = XY × 10^Z picofarads

104 = 10 × 10⁴ = 100,000 pF = 100 nF = 0.1 µF
103 = 10 × 10³ = 10,000 pF = 10 nF
222 = 22 × 10² = 2,200 pF = 2.2 nF
220 = 22 × 10⁰ = 22 pF
```

### Electrolytic (direct printing)
```
10 µF 50V — straightforward: 10 microfarads, 50 volt rating
May show: 10/50 or 10µ50
```

### SMD Ceramic (0402, 0603 etc.)
```
Usually no marking (too small)
Value must be tracked from the reel/bag label
Your 22pF C0G: confirm from the bag — do not trust unmarked SMD caps
```

---

## Standard Capacitor Values (E12 series)

Capacitors (like resistors) come in standard values:
```
...1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2, 10, 12, 15...
(each decade multiplied: 10pF, 12pF, 15pF... 100pF, 120pF... 1nF, 1.2nF...)
```

Your values:
- 22 pF — standard E12 value ✓
- 100 nF — standard (10 × E12 = 100) ✓
- 10 µF — standard E12 value ✓

---

## Correct Placement on a Circuit

### Rule 1: Decoupling caps as close to IC as possible

```
WRONG:                    CORRECT:
                          
IC ──────[long wire]──── cap    cap ──[short trace]── IC power pin
         ↑                               ↑
   inductance negates                1mm or less
   the capacitor
```

Every centimetre of wire adds inductance that reduces the capacitor's effectiveness at high frequency. On a breadboard: press the 100nF directly into the row next to the chip's power pin.

### Rule 2: Use both values together at power supply pins

```
Power in ──┬─── (load)
           │
         [10µF]   ← bulk storage, slower response
           │
         [100nF]  ← fast decoupling, high frequency
           │
          GND
```

### Rule 3: Electrolytic always correct polarity

```
+ leg → VCC (higher voltage)
− leg (stripe side) → GND (lower voltage)
```

---

## Capacitor Failure Modes

| Failure | Cause | Visible sign |
|---------|-------|-------------|
| Electrolytic bulge | Reverse bias, overvoltage, heat | Dome on top of cap |
| Electrolytic leak | Age, overvoltage | Brown sticky residue at base |
| Ceramic crack | Physical stress, thermal shock | Invisible — cap reads open or short |
| Electrolytic explosion | Severe reverse bias | Loud pop, smell, damage to PCB |
| Ceramic short | Overvoltage | Circuit stops working, warm PCB trace |

If an electrolytic looks bulged — replace it immediately. It will fail soon and may damage surrounding components.

---

## Quick Selection Guide

| Need | Use |
|------|-----|
| Decouple every IC power pin | 100nF ceramic (your disc cap) |
| Bulk decoupling at power supply | 10µF electrolytic |
| NRF24L01 power stabilization | 10µF electrolytic + 100nF ceramic in parallel |
| Crystal oscillator load | 22pF C0G ceramic (your SMD cap) |
| High-frequency RF filtering | C0G ceramic |
| Audio / signal coupling | Film capacitor |
| Large energy storage (motor start) | Large electrolytic (470µF+) |

---

## The Exact Setup for Your NRF24L01

You bought these parts specifically to fix the NRF24L01 power problem. Here is the exact circuit:

```
AMS1117-5V OUT (5V) ───── NRF24L01 Adapter VCC
                     │
                   [10µF electrolytic]   + lead to 5V rail
                     │                  − lead to GND
                    GND

NRF24L01 Adapter VCC (3.3V out) ───── NRF24L01 module VCC
                                  │
                                [100nF ceramic]   ← right at module pins
                                  │
                                 GND
```

The 10µF handles the 115–250mA bulk current burst when the radio transmits.
The 100nF handles the high-frequency (2.4 GHz) switching noise.
Together: the NRF24L01 gets clean, stable power and stops dropping connections.

---

## Practice Questions

1. A capacitor is marked "472". What is its capacitance in nF?

2. You connect a 10µF 16V electrolytic capacitor to a 12V supply. Is this safe?

3. You need to decouple the MPU-6050's VCC pin. What capacitor do you place there and how close?

4. Why does a 100nF ceramic cap outperform a 10µF electrolytic at radio frequencies?

**Answers:**
1. 47 × 10² = 4700 pF = 4.7 nF
2. No — 12V exceeds the 16V rating by only 25% with no derating margin. Use a 25V or 50V rated cap.
3. 100nF ceramic, placed within 1–2mm of the VCC pin directly on the breadboard row
4. Lower ESR — the ceramic's internal resistance is 10–100× lower, so it responds faster to high-frequency current spikes
