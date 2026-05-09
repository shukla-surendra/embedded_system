# 07 — Resistors: Complete Reference

## What a Resistor Actually Does

A resistor opposes the flow of current. Every electron moving through the resistive material collides with atoms and loses energy — that energy becomes heat. This is not a side effect; it is the mechanism.

Three uses in almost every circuit:

1. **Current limiting** — protect an LED or GPIO pin from too much current
2. **Voltage dividing** — produce a fraction of a voltage (sensor reading, ADC input scaling)
3. **Pull-up / pull-down** — set a definite logic state on a floating pin

---

## Ohm's Law Refresher

```
V = I × R

V = voltage across the resistor (Volts)
I = current through the resistor (Amps)
R = resistance (Ohms, Ω)

Rearranged:
I = V / R     ← how much current flows
R = V / I     ← what value you need
```

**Example:** ESP32 GPIO outputs 3.3V. You want 10mA through an LED.
LED forward voltage = 2.0V. Voltage across resistor = 3.3 − 2.0 = 1.3V.
`R = V / I = 1.3 / 0.010 = 130Ω` → use the nearest standard value: **150Ω**

---

## Power Dissipation — The Critical Calculation

Every resistor has a maximum power rating. Exceed it → resistor overheats → burns, changes value, or fails open.

```
P = V × I = I² × R = V² / R

P = power in Watts
```

**Example:** 1kΩ resistor with 5V across it:
`P = 5² / 1000 = 25 / 1000 = 0.025W = 25mW`

A ¼W (250mW) resistor handles this with a 10× safety margin. Fine.

**Dangerous example:** 100Ω resistor with 12V across it:
`P = 12² / 100 = 144 / 100 = 1.44W`

A ¼W resistor would dissipate 5.76× its rating → instant failure. Need at least a **2W** resistor.

**Safety rule:** derate to 50% of rated power in continuous use.
A ¼W resistor should dissipate no more than 125mW continuously.

---

## Standard Power Ratings

| Package | Power Rating | Physical size | When to use |
|---------|-------------|---------------|-------------|
| 0402 SMD | 1/16W (62.5mW) | 1.0×0.5mm | PCB, low power signal circuits |
| 0603 SMD | 1/10W (100mW) | 1.6×0.8mm | PCB, general purpose |
| 0805 SMD | 1/8W (125mW) | 2.0×1.25mm | PCB, slightly higher current |
| 1206 SMD | 1/4W (250mW) | 3.2×1.6mm | PCB, standard |
| 1/4W through-hole | 250mW | ~6mm body | Breadboard, perfboard — standard |
| 1/2W through-hole | 500mW | ~8mm body | Moderate power |
| 1W through-hole | 1W | ~11mm body | Power circuits, motor drivers |
| 2W through-hole | 2W | ~15mm body | High power paths |
| 5W wire-wound | 5W | Large ceramic body | Shunt resistors, heater elements |

For breadboard and perfboard work: **1/4W** is the default. Use 1/2W or 1W when power dissipation is above 100mW.

---

## Resistor Types

### Carbon Film (most common, coloured bands)

- Tolerance: ±5% (gold band) or ±1% (brown band, "E24/E96")
- Noise: moderate
- Temperature coefficient: ~200 ppm/°C
- Cost: very low
- Use: general purpose, pull-ups, LED current limiting, voltage dividers

### Metal Film (precision, five colour bands)

- Tolerance: ±1% or ±0.1%
- Noise: low
- Temperature coefficient: ~50–100 ppm/°C
- Cost: slightly higher
- Use: precision voltage dividers, sensor circuits, analog signal paths

### Wire-Wound

- Power rating: 1W–1000W
- Tolerance: ±1% available
- Inductance: **significant** — do NOT use in high-frequency circuits
- Use: current sensing shunts, power supplies, heater controls

### SMD Thick Film

- All ratings above, now in chip form
- Markings: 3-digit (like caps) or 4-digit for 1% types
- Use: PCB assembly — same function as through-hole

---

## Reading Resistor Values

### 4-Band (±5% carbon film)

```
Band 1 — 1st digit
Band 2 — 2nd digit
Band 3 — multiplier (×10^n)
Band 4 — tolerance (gold=±5%, silver=±10%)
```

### 5-Band (±1% metal film)

```
Band 1 — 1st digit
Band 2 — 2nd digit
Band 3 — 3rd digit
Band 4 — multiplier
Band 5 — tolerance (brown=±1%, red=±2%)
```

### Colour Code Table

| Colour | Digit | Multiplier | Tolerance |
|--------|-------|------------|-----------|
| Black | 0 | ×1 | — |
| Brown | 1 | ×10 | ±1% |
| Red | 2 | ×100 | ±2% |
| Orange | 3 | ×1,000 | — |
| Yellow | 4 | ×10,000 | — |
| Green | 5 | ×100,000 | ±0.5% |
| Blue | 6 | ×1,000,000 | ±0.25% |
| Violet | 7 | — | ±0.1% |
| Grey | 8 | — | — |
| White | 9 | — | — |
| Gold | — | ×0.1 | ±5% |
| Silver | — | ×0.01 | ±10% |

**Examples:**

```
Brown–Black–Red–Gold (4-band):
  1 – 0 – ×100 – ±5% = 1000Ω = 1kΩ ±5%

Red–Red–Black–Black–Brown (5-band):
  2 – 2 – 0 – ×1 – ±1% = 220Ω ±1%

Orange–Orange–Brown–Gold (4-band):
  3 – 3 – ×10 – ±5% = 330Ω ±5%

Yellow–Violet–Orange–Gold (4-band):
  4 – 7 – ×1000 – ±5% = 47,000Ω = 47kΩ ±5%
```

### SMD Marking (3-digit)

```
XYZ = XY × 10^Z Ω

102 = 10 × 10² = 1000Ω = 1kΩ
472 = 47 × 10² = 4700Ω = 4.7kΩ
103 = 10 × 10³ = 10,000Ω = 10kΩ
000 = 0Ω (jumper/wire)
```

### SMD Marking (4-digit, 1%)

```
XYZW = XYZ × 10^W Ω

1001 = 100 × 10¹ = 1000Ω = 1kΩ
4702 = 470 × 10² = 47,000Ω = 47kΩ
```

---

## Standard Value Series (E-Series)

Resistors are not made in every value — they follow geometric series:

| Series | Tolerance | Values per decade | Use |
|--------|-----------|------------------|-----|
| E6 | ±20% | 6 | Old stock |
| E12 | ±10% | 12 | Common, cheap |
| E24 | ±5% | 24 | Standard ±5% |
| E96 | ±1% | 96 | Precision ±1% |

**E12 values:** 10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82 (then ×10, ×100...)
**E24 adds:** 11, 13, 16, 20, 24, 30, 36, 43, 51, 62, 75, 91

When your calculation gives 130Ω, the nearest E24 value is **130Ω** (available). Nearest E12 is **120Ω** or **150Ω**. Pick the one that keeps current within safe limits.

---

## The Voltage Divider

Two resistors in series divide voltage proportionally:

```
VCC ──[R1]──┬──[R2]── GND
            │
           Vout

Vout = VCC × R2 / (R1 + R2)
```

**Example:** 5V to 3.3V for an ESP32 ADC input:
```
Vout = 3.3V, VCC = 5V
R2 / (R1 + R2) = 3.3 / 5 = 0.66

Choose R2 = 10kΩ:
R1 + R2 = 10k / 0.66 = 15.15kΩ → R1 = 5.15kΩ → nearest E24: 5.1kΩ

Verify: 5V × (10,000 / 15,100) = 5V × 0.662 = 3.31V ✓
```

**Important:** The voltage divider only works accurately when the load (the ADC input, etc.) has much higher resistance than R2. If the load resistance is close to R2, it forms a parallel combination and changes the output voltage. ESP32 ADC input impedance is ~1MΩ, so a 10kΩ divider is fine.

**HC-SR04 ECHO voltage divider** (5V → 3.3V):
```
5V ──[1kΩ]──┬── GPIO (3.3V max)
            │
          [2kΩ]
            │
           GND

Vout = 5 × (2000 / 3000) = 3.33V ✓
```

---

## Pull-Up and Pull-Down Resistors

When a digital input pin has nothing driving it, it floats — reads unpredictably. A resistor ties it to a known voltage.

```
Pull-up (default HIGH):        Pull-down (default LOW):

3.3V                           Signal ──[10kΩ]── GND
  │
[10kΩ]
  │
GPIO ──────── Button ── GND    GPIO ──────── Button ── 3.3V

When button open: GPIO reads HIGH    When button open: GPIO reads LOW
When button pressed: GPIO reads LOW  When button pressed: GPIO reads HIGH
```

**Typical values:**
- **10kΩ** — standard, works for most digital signals
- **4.7kΩ** — I2C pull-up (SDA/SCL lines require this)
- **1kΩ** — strong pull-up for fast signals or long cables
- **100kΩ** — weak pull-up, used when the pin must also sense leakage current

The ESP32 has **internal pull-up resistors (~45kΩ)** you activate in software:
```cpp
pinMode(PIN, INPUT_PULLUP);   // no external resistor needed for buttons
```

For I2C: use external **4.7kΩ pull-ups** on SDA and SCL — the internal ones are too weak.

---

## Resistors in Series and Parallel

### Series (add values)
```
──[R1]──[R2]──[R3]──

Rtotal = R1 + R2 + R3

Example: 100Ω + 100Ω + 100Ω = 300Ω
```

### Parallel (reciprocal formula)
```
──┬──[R1]──┬──
  ├──[R2]──┤
  └──[R3]──┘

1/Rtotal = 1/R1 + 1/R2 + 1/R3

Two equal resistors in parallel = half value:
Two 10kΩ in parallel = 5kΩ

General two-resistor shortcut:
Rtotal = (R1 × R2) / (R1 + R2)
```

**Why this matters:** the I2C SDA/SCL lines may already have a 10kΩ pull-up from one module. Adding a second sensor with another 10kΩ pull-up creates two 10kΩ in parallel = 5kΩ. That's fine. Three sensors = 3.3kΩ. Still fine for I2C at 400kHz.

---

## Temperature Coefficient (Tempco)

Resistors change value with temperature. Expressed in ppm/°C (parts per million per degree).

```
ΔR = R × tempco × ΔT

Carbon film (200 ppm/°C) at 10°C temperature change:
ΔR = 1000Ω × 200×10⁻⁶ × 10 = 2Ω change (0.2%)

Metal film (50 ppm/°C) at 10°C:
ΔR = 1000Ω × 50×10⁻⁶ × 10 = 0.5Ω change (0.05%)
```

For most digital circuits this is irrelevant. For precision analog (sensor calibration, voltage reference dividers) use metal film ±1% with low tempco.

---

## Resistor Noise

Resistors generate thermal noise (Johnson-Nyquist noise):

```
Vnoise = √(4 × k × T × R × BW)

k = Boltzmann's constant (1.38×10⁻²³)
T = temperature in Kelvin
R = resistance in Ohms
BW = bandwidth in Hz
```

At room temperature, a 10kΩ resistor in a 20kHz audio bandwidth:
`Vnoise = √(4 × 1.38×10⁻²³ × 293 × 10,000 × 20,000) ≈ 1.8µV`

Negligible for digital circuits. For precision ADC or microphone circuits, use lower resistance values to reduce noise.

---

## Common Resistor Values for This Project

| Value | Use |
|-------|-----|
| 220Ω | LED current limiter from 5V (22mA) |
| 330Ω | LED current limiter from 5V (15mA) |
| 1kΩ | HC-SR04 voltage divider top half; LED from 3.3V |
| 2kΩ | HC-SR04 voltage divider bottom half |
| 4.7kΩ | I2C pull-ups (SDA, SCL) |
| 10kΩ | General pull-up/pull-down, voltage divider |
| 100kΩ | Weak pull-up, high-impedance input protection |

---

## Testing Resistors with a Multimeter

1. Set multimeter to Ω (resistance) mode
2. Touch probes to both ends of resistor (out of circuit)
3. Read value — should be within tolerance of marked value

If you read very high (MΩ) when expecting 1kΩ: resistor is burned open or probes not making contact.
If you read near zero: resistor is shorted — replace it.

**Never measure resistance in a powered circuit** — you'll read the parallel combination of the resistor and the rest of the circuit, and potentially damage the meter.

---

## Resistor Failure Modes

| Failure | Cause | Sign |
|---------|-------|------|
| Open circuit (infinite resistance) | Overheated, physical break | Circuit stops working |
| Changed value (drifted) | Prolonged heat stress | Subtle — measure to confirm |
| Body cracked / burned | Severely over-rated | Visible scorch marks, smell |
| Short (rare) | Manufacturing defect | Reads ~0Ω |

Burn marks or discolouration on the body → replace the resistor and investigate why it overheated (usually wrong value or missing series component).

---

## Practice Questions

1. A resistor has bands: Yellow–Violet–Red–Gold. What is its value and tolerance?

2. You want to light an LED from the ESP32's 3.3V GPIO (max 12mA). The LED's forward voltage is 2.1V. What resistor value do you need? What power is dissipated?

3. Two 22kΩ resistors are connected in parallel. What is the combined resistance?

4. You have a 5V sensor output connected to an ESP32 ADC (3.3V max). Design a voltage divider using values from the E12 series.

**Answers:**

1. Yellow(4)–Violet(7)–Red(×100)–Gold(±5%) = 4700Ω = **4.7kΩ ±5%**

2. V across resistor = 3.3 − 2.1 = 1.2V. R = 1.2 / 0.012 = **100Ω**. P = 1.2 × 0.012 = **14.4mW** (a ¼W resistor is fine).

3. Rtotal = (22k × 22k) / (22k + 22k) = 484M / 44k = **11kΩ**

4. Target: 5V → 3.3V. Ratio = 3.3/5 = 0.66. Using 10kΩ for R2: R1 = (5−3.3)/3.3 × 10kΩ = 5.15kΩ → nearest E12 = **4.7kΩ**. Verify: 5 × (10k / 14.7k) = 3.40V. Slightly high — use **5.6kΩ** for R1: 5 × (10k/15.6k) = 3.21V. Good enough for ADC input.
