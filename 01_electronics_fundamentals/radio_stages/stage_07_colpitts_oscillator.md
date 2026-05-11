# Stage 7 — The Colpitts Oscillator: Generate Your Own Radio Signal

---

## What You Will Learn

- What an oscillator is at a fundamental level and why it sustains itself
- The Barkhausen criterion — the exact conditions for sustained oscillation, derived from first principles
- The four main families of LC oscillator and why Colpitts is the best choice for a beginner
- Full mathematical analysis of the Colpitts circuit: feedback network, frequency, component ratios
- How to build a 1 MHz Colpitts oscillator from a BC547 and calculate every component value
- How to verify you have RF oscillation without an oscilloscope
- How to modulate your oscillator with audio and transmit your voice to a nearby AM radio
- What frequency stability means and how to improve it
- The voltage-controlled oscillator (VCO) — the link between Colpitts and modern radio ICs

---

## 1. What Is an Oscillator?

An amplifier takes an input signal and produces a larger output signal. If you remove the input and feed the output back to the input, you get nothing useful — the amplifier simply amplifies its own noise, which grows until the amplifier clips. That is not an oscillator. It is a saturated amplifier.

An oscillator is something more specific:

**Definition:** An oscillator is an amplifier with a frequency-selective feedback network where, at one specific frequency, the feedback is both:
1. In phase (positive feedback — the output reinforces the input)
2. Of exactly sufficient amplitude (loop gain = exactly 1)

At that one frequency, the circuit sustains a continuous sinusoidal oscillation without any external input. At all other frequencies the feedback is either out of phase (negative) or less than unity gain, so oscillation cannot be sustained.

The frequency-selective network in an LC oscillator is the inductor-capacitor tank circuit. It resonates at one frequency and presents a very different impedance at all other frequencies. This selectivity is what makes LC oscillators useful for radio work.

### The Physical Picture

Imagine a child on a swing. You give one push and the swing oscillates, gradually slowing down as air resistance and friction dissipate the energy. Eventually it stops.

Now imagine you push the swing every time it reaches the back of its arc — exactly when it is ready to swing forward. You are adding energy at exactly the right phase. The swing sustains its oscillation indefinitely. The energy you add exactly replaces the energy lost to friction.

Your pushing hand is the transistor. The swing is the LC tank circuit. The timing of your push — in exact phase with the swing — is positive feedback.

If you push at the wrong time (out of phase with the swing), you slow it down or stop it. That is negative feedback. The phase of the feedback determines everything.

---

## 2. The Barkhausen Criterion — Derived Simply

Heinrich Barkhausen stated in 1921 the exact conditions for oscillation. Let us derive this rather than just state it.

### 2.1 The Open-Loop System

Consider an amplifier with voltage gain A (a complex number — it has both magnitude and phase). A feedback network samples the output and returns a fraction β of it to the input (also complex).

The closed-loop gain of this system is:

```
G_closed = A / (1 - Aβ)
```

This is the standard feedback equation. Note the denominator: (1 - Aβ).

### 2.2 When Does It Oscillate?

If the denominator (1 - Aβ) approaches zero, the closed-loop gain approaches infinity. An infinite gain amplifier produces an output without any input. That is an oscillator.

For the denominator to equal zero:

```
1 - Aβ = 0
Therefore: Aβ = 1
```

This is the Barkhausen criterion: **the loop gain Aβ must equal exactly 1.**

But Aβ is a complex number. For a complex number to equal 1 (not -1, not j, but +1), it must have:
- **Magnitude: |Aβ| = 1** (the signal comes back at the same amplitude it left)
- **Phase: ∠(Aβ) = 0° or 360°** (the signal comes back in phase with itself)

### 2.3 The Steady State

In practice, circuits do not start at exactly Aβ = 1. They start with Aβ slightly greater than 1. The oscillation grows from noise, gets larger cycle by cycle. As the amplitude grows, the transistor starts to enter saturation and cut-off, and the effective gain A decreases. When A has decreased enough that |Aβ| = 1, the amplitude stops growing. This is the stable operating point.

This self-limiting mechanism means real oscillators always settle to a steady amplitude, even though they "start" with loop gain > 1.

**Implication:** the Barkhausen criterion |Aβ| = 1 is the steady-state condition. To start oscillating reliably, you want |Aβ| slightly greater than 1 at startup — typically 2 to 5.

### 2.4 Why the Phase Must Be Zero Degrees

If the phase of Aβ is not zero (or a multiple of 360°), the signal comes back shifted. On the next cycle, it arrives at the wrong time and partially cancels the oscillation already in the tank. Oscillation cannot be sustained.

In a Colpitts oscillator:
- The transistor (common-emitter) contributes 180° phase shift
- The capacitive voltage divider (feedback network) contributes another 180° phase shift
- Total: 360° = 0° ✓

---

## 3. Types of LC Oscillators

There are four main families. Understanding why we choose Colpitts requires knowing the others.

### 3.1 The Hartley Oscillator

```
      +V
       |
      [RC]
       |
       +---------> Output
       |
      [C]  (tank capacitor, single)
       |
   [L1 | L2]  (tapped inductor — winding with centre tap)
       |
   [emitter]--[tap point]
   [base]--[one end of L]
   [collector]--[other end via RC]
```

The Hartley uses a tapped inductor as the feedback network. The tap divides the inductor into two sections, giving a voltage divider that provides feedback.

**Advantages:** Only one capacitor (easy to make variable). Easy to tune over a wide range.

**Disadvantages:** Winding a tapped inductor is more work than using two capacitors. The inductance ratio sets the feedback; changing it requires rewinding. Less stable than Colpitts because the inductance of a tapped coil changes more with temperature and nearby conductors.

### 3.2 The Colpitts Oscillator

```
      +V
       |
      [RC]
       |
       +---------> Output
       |
      [L]  (single inductor)
       |
     C1  +  C2  (voltage divider using two capacitors)
```

The Colpitts uses a single inductor and two capacitors in series as the tank. The junction between the two capacitors is the feedback tap.

**Advantages:** Capacitors are stable (especially NP0/C0G types), easy to obtain in many values, and behave predictably. The feedback ratio is set by C1/C2, which is stable. Standard choice for most RF oscillator work.

**Disadvantages:** Two capacitors instead of one. If you want to tune it with a single variable cap, you need to add a third capacitor in parallel with the LC (the Clapp modification).

### 3.3 The Clapp Oscillator (Colpitts Variant)

Add a capacitor C3 in series with the inductor L. Now the series combination of L and C3 forms the inductive element, with C1 and C2 as the capacitive divider.

```
      [L]---[C3]   series combination
           |
          C1
           |
          C2
```

Because C3 is much smaller than C1 or C2, the oscillation frequency is dominated by L and C3 (the smaller capacitor in a series combination dominates). C1 and C2 mainly set the feedback ratio and have little effect on frequency.

**Advantage:** C3 can be a small air-variable trimmer, giving precise frequency control. C1 and C2 are large fixed capacitors that do not drift. Excellent stability.

**Use:** Standard for VFO (variable frequency oscillator) applications where stability matters.

### 3.4 Crystal Oscillator

Replace the inductor with a quartz crystal. A quartz crystal is a mechanical resonator — it vibrates at an exact frequency determined by its cut dimensions. Its equivalent Q is 10,000 to 100,000 (versus 50–200 for an LC tank). Frequency stability is ±50 parts per million or better.

**Advantage:** Extremely stable. A 10 MHz crystal oscillator drifts less than 1 Hz per degree Celsius. This is why every clock, every microcontroller, every radio IC uses a crystal reference.

**Disadvantage:** Fixed frequency (you cannot tune it). Can be pulled ±100 ppm with a variable capacitor in series, but that is a very small range.

**Why we do not build this now:** Crystal oscillators are almost trivial to build (same circuit as Colpitts, swap L for crystal) once you understand Colpitts. We will add one in the superheterodyne stage.

---

## 4. Colpitts Oscillator — Full Analysis

### 4.1 The Circuit Topology

```
        +9V
         |
     [RC = 2.2kΩ]
         |
         +----+-----------> Output (RF)
         |    |
         |   [C_out = 10pF]
         |    |
         | Collector (Q1)
         |
        [L = 25μH]
         |
         +---- Collector and top of tank
         |
        [C1 = 330pF]
         |
         +---- Base of Q1 (through C_coupling = 100nF)
         |
        [C2 = 1000pF]
         |
        GND (also emitter, via 100Ω emitter resistor for stability)
```

Wait — let me draw this more carefully and correctly.

The Colpitts oscillator in the common-emitter configuration:

```
        +9V
         |
    [R1 = 100kΩ]   <- bias top
         |
         +-----[Base of Q1]
         |
    [R2 = 22kΩ]    <- bias bottom
         |
        GND

        +9V
         |
    [RC = 2.2kΩ]   <- collector load
         |
    [Collector Q1]--[C_out 10pF]---> RF Output
         |
         |      <- RF choke path (inductor in tank)
         |
    [L = 25μH coil]
         |
    [C1 = 330pF]   <- top capacitor of divider
         |
    [Junction A]----[C_fb = 10nF]---[Base of Q1]   <- feedback path
         |
    [C2 = 1000pF]  <- bottom capacitor of divider
         |
        GND

    [Emitter Q1]---[RE = 100Ω]---GND
    [Emitter Q1]---[CE = 10nF]---GND  (RF bypass only, not audio bypass)
```

This is the correct topology. The tank circuit is formed by L in parallel with the series combination of C1 and C2.

### 4.2 Feedback Network Analysis

The series combination of C1 and C2 forms a capacitive voltage divider. When there is a voltage V across the full tank (across L, across C1+C2 in series), the voltage at the Junction A between C1 and C2 is:

```
V_A = V × C1 / (C1 + C2)
```

(Note: for capacitors in series as a divider, the larger capacitor has less voltage across it — opposite to a resistor divider. The voltage at the junction is V × C1/(C1+C2) where C1 is the top capacitor.)

This is the fraction fed back to the base. The feedback fraction is:

```
β = C1 / (C1 + C2)
```

With C1 = 330 pF and C2 = 1000 pF:

```
β = 330 / (330 + 1000) = 330 / 1330 = 0.248 ≈ 0.25 (about 25%)
```

### 4.3 Loop Gain Calculation

The transistor's transconductance is:

```
gm = I_C / V_T
```

Where V_T = 26 mV at room temperature (thermal voltage, kT/q).

With I_C = 1 mA:

```
gm = 1 mA / 26 mV = 38.5 mA/V = 38.5 mS
```

The voltage gain at the collector into load RC = 2.2kΩ:

```
|A| = gm × RC = 38.5 × 10^-3 × 2200 = 84.7 ≈ 85
```

The loop gain magnitude:

```
|Aβ| = 85 × 0.25 = 21
```

This is much greater than 1, which means the oscillation will start reliably and grow quickly from noise to full amplitude. Once oscillating, the transistor's effective gain drops (due to saturation/cutoff clipping) until |Aβ| settles to 1.

**Design rule:** want |Aβ| at startup between 3 and 10 for reliable oscillation with acceptable distortion. 21 is somewhat high — the output waveform will be somewhat clipped (more square than sine). This is fine for most radio work. For a cleaner sine wave, reduce C1 to 100 pF (reduces β to 0.09, loop gain ≈ 7.5).

### 4.4 Oscillation Frequency

The equivalent capacitance of C1 and C2 in series:

```
Ceq = (C1 × C2) / (C1 + C2) = (330 × 1000) / (330 + 1000) = 330000 / 1330 = 248 pF
```

Resonant frequency:

```
f = 1 / (2π × √(L × Ceq))
  = 1 / (2π × √(25 × 10^-6 × 248 × 10^-12))
  = 1 / (2π × √(6.2 × 10^-15))
  = 1 / (2π × 7.87 × 10^-8)
  = 1 / (4.944 × 10^-7)
  = 2.02 MHz
```

Hmm, that comes out near 2 MHz, not 1 MHz. To get 1 MHz, we need to either double the inductance or double the capacitance.

**Revised for 1 MHz target:**

Option A: Keep C values, increase L to 100 μH:
```
f = 1 / (2π × √(100 × 10^-6 × 248 × 10^-12))
  = 1 / (2π × √(24.8 × 10^-15))
  = 1 / (2π × 1.575 × 10^-7)
  = 1 / (9.9 × 10^-7)
  = 1.01 MHz  ✓
```

Option B: Keep L = 25 μH, use C1 = 680 pF, C2 = 2200 pF:
```
Ceq = (680 × 2200) / (680 + 2200) = 1496000 / 2880 = 520 pF
f = 1 / (2π × √(25 × 10^-6 × 520 × 10^-12))
  = 1 / (2π × √(13 × 10^-15))
  = 1 / (2π × 1.14 × 10^-7)
  = 1 / (7.16 × 10^-7)
  = 1.4 MHz
```

Not quite. Let us work backwards from 1 MHz with L = 25 μH:

```
f = 1 MHz → ω = 2π × 10^6 = 6.28 × 10^6 rad/s

L × Ceq = 1/ω² = 1/(6.28 × 10^6)² = 1/(3.94 × 10^13) = 2.54 × 10^-14

Ceq = 2.54 × 10^-14 / (25 × 10^-6) = 1.01 × 10^-9 F = 1010 pF ≈ 1000 pF
```

So for L = 25 μH and 1 MHz: Ceq = 1000 pF.

With C1/C2 = 1/3 ratio (for β = 0.25):
```
C1 = 4 × Ceq / 3 (working backwards from Ceq = C1C2/(C1+C2) with C2 = 3×C1):
Ceq = C1 × 3C1 / (C1 + 3C1) = 3C1² / 4C1 = 3C1/4
Therefore C1 = 4Ceq/3 = 4×1000/3 = 1333 pF ≈ 1500 pF
C2 = 3 × C1 = 4500 pF ≈ 4700 pF
```

Check: Ceq = (1500 × 4700)/(1500+4700) = 7050000/6200 = 1137 pF. Close enough.

f = 1/(2π√(25e-6 × 1137e-12)) = 1/(2π × 1.69e-7) = 942 kHz ≈ 1 MHz. Good.

**Final component values for 1 MHz Colpitts on BC547:**

---

## 5. Full Schematic — 1 MHz Colpitts Oscillator

```
                      +9V
                       |
               +-------+-------+
               |               |
          [R1 = 100kΩ]    [RFC = 100μH]  <- RF choke (keeps RF out of +9V rail)
               |               |
               |         [C_pwr = 10nF] to GND  <- RF bypass on supply
               |
          [Base]---Q1 (BC547)---[Collector]
               |                     |
          [R2 = 22kΩ]          [RC = 2.2kΩ]---[+9V via RFC]
               |                     |
              GND             [C_out = 10pF]----> RF Output (to antenna or probe)
                                     |
                            [TANK CIRCUIT below]
                            
TANK CIRCUIT (between collector and ground):

Collector ----[L = 25μH]----+---[C1 = 1500pF]---+--- GND
                             |                   |
                             |              [C2 = 4700pF]
                             |                   |
                             +-------------------+--- GND
                             |
                        Junction A (feedback tap)
                             |
                        [C_fb = 100nF]  <- coupling cap (blocks DC)
                             |
                         [Base of Q1]

Emitter of Q1:
     Emitter ---[RE = 100Ω]---GND
     Emitter ---[CE = 10nF ceramic]---GND  (RF bypass for emitter)
     
Full supply decoupling:
     +9V ---[C_bulk = 100μF electrolytic]---GND
     +9V ---[C_bypass = 100nF ceramic]---GND (near Q1 power pin)
```

### Complete Component List (1 MHz Colpitts)

| Ref | Value | Type | Notes |
|-----|-------|------|-------|
| Q1 | BC547 or 2N2222 | NPN transistor | |
| R1 | 100kΩ | 0.25W | Bias top |
| R2 | 22kΩ | 0.25W | Bias bottom |
| RC | 2.2kΩ | 0.25W | Collector load |
| RE | 100Ω | 0.25W | Emitter stability resistor |
| RFC | 100μH | Choke | Keeps RF off +9V rail |
| L | 25μH | Wound coil | See winding guide below |
| C1 | 1500pF | NP0/C0G ceramic | Top tank capacitor |
| C2 | 4700pF | NP0/C0G ceramic | Bottom tank capacitor |
| C_fb | 100nF | Ceramic | Feedback coupling cap |
| C_out | 10pF | NP0 ceramic | Low-power output coupling |
| C_pwr | 10nF | Ceramic | RF bypass on +9V rail |
| C_bypass | 100nF | Ceramic | Supply HF bypass at Q1 |
| C_bulk | 100μF | Electrolytic 16V | Supply bulk decoupling |
| CE | 10nF | Ceramic | Emitter RF bypass |

### Coil Winding Guide for L = 25μH

**Air core coil for 1 MHz:**

Using the formula for a single-layer air-core coil:
```
L (μH) = (r² × N²) / (9r + 10l)
```
Where r = radius in inches, l = length in inches.

Target: L = 25 μH on a 15mm diameter former (r = 0.30 inch):

Try 30 turns, length = 25mm (1 inch):
```
L = (0.3² × 30²) / (9×0.3 + 10×1)
  = (0.09 × 900) / (2.7 + 10)
  = 81 / 12.7
  = 6.4 μH  (too low)
```

For 25 μH on 15mm former, need more turns or tighter winding:

Try 60 turns, 25mm length:
```
L = (0.09 × 3600) / (2.7 + 10) = 324 / 12.7 = 25.5 μH  ✓
```

**Winding specification:**
- 60 turns of 28 AWG (0.32 mm) enamelled copper wire
- On a 15 mm diameter former (PVC pipe section, or cardboard tube)
- Wound in a single layer over 25 mm
- Secured with tape at each end

Alternatively: use a 100μH ferrite choke (the kind sold for EMI suppression) as the inductor — these are available cheaply and have a stable inductance. The oscillation frequency will be different from 1 MHz and will need adjustment with component values.

---

## 6. Bias Point Calculation (Step by Step)

Target: I_C = 2 mA, V_C = 4.5V on 9V supply.

**Step 1:** Collector voltage drop across RC:
```
V_RC = I_C × RC = 2mA × 2200 = 4.4V
V_C = 9 - 4.4 = 4.6V  ✓ (close to 4.5V target)
```

**Step 2:** Emitter voltage:
```
V_E = I_C × RE = 2mA × 100 = 0.2V
```

**Step 3:** Base voltage:
```
V_B = V_E + V_BE = 0.2 + 0.65 = 0.85V
```

**Step 4:** Current divider for bias:
Use divider current ≈ 10× base current:
```
I_B = I_C / hFE = 2mA / 200 = 10 μA
I_divider = 10 × I_B = 100 μA
```

**Step 5:** R2:
```
R2 = V_B / I_divider = 0.85 / 100×10^-6 = 8500Ω → use 8.2kΩ
```

**Step 6:** R1:
```
Voltage across R1 = 9 - 0.85 = 8.15V
R1 = 8.15 / 100μA = 81.5kΩ → use 82kΩ (or 100kΩ in practice)
```

**Updated component values (corrected bias):**

| Ref | Value |
|-----|-------|
| R1 | 82kΩ (use 100kΩ if 82kΩ unavailable) |
| R2 | 8.2kΩ (use 10kΩ if unavailable) |
| RC | 2.2kΩ |
| RE | 100Ω |

**Verify:**
With R1=100kΩ, R2=10kΩ:
```
V_B = 9 × 10/(100+10) = 9 × 10/110 = 0.818V
V_E = 0.818 - 0.65 = 0.168V
I_C ≈ 0.168/100 = 1.68 mA
V_C = 9 - 1.68mA × 2.2kΩ = 9 - 3.7 = 5.3V  (good)
```

---

## 7. Verifying Oscillation

You need to know whether the circuit is oscillating before you spend time on alignment. Three methods, from simplest to most accurate.

### Method 1: The AM Radio Test (No Equipment Needed)

An AM radio is a sensitive detector of RF signals in the 530–1700 kHz range.

1. Build the oscillator.
2. Apply power.
3. Tune your AM radio to the oscillator's calculated frequency (1 MHz = 1000 kHz on the dial, so tune between 990 kHz and 1010 kHz).
4. Bring the AM radio within 30 cm of your oscillator.
5. If the oscillator is running, you will hear a dead carrier — a loud hiss or silence compared to the normal station background noise. The station you were tuned to disappears (your oscillator's carrier suppresses it).
6. Tune the AM radio slowly around 1 MHz. You will find the exact dead-carrier point.

If you hear nothing unusual: the oscillator is not running. Debug first before proceeding.

### Method 2: RF Probe + Multimeter

Build this simple RF probe:

```
Input (clip to oscillator collector) ---[1N4148 or OA91]---+--- Output to multimeter
                                                            |
                                                       [100pF cap to GND]
                                                       [100kΩ to GND]
```

Connect the RF probe to the collector of Q1. Set the multimeter to DC volts, 1V range.

With no oscillation: reads approximately 0V (or the DC bias voltage).
With oscillation: the diode rectifies the RF. The cap smooths it. You will read a DC voltage proportional to the RF amplitude. A 1V peak-to-peak RF signal gives approximately 0.3–0.5V DC on the probe output.

**Build the probe before you need it** — it takes 5 minutes and is useful for every RF circuit you will ever build.

### Method 3: Oscilloscope

Connect the oscilloscope to the collector through a 10pF capacitor (to avoid loading the circuit). Set timebase to 1 μs/division. You should see a sine wave (or clipped sine) at 1 MHz — one complete cycle every 1 μs.

---

## 8. Frequency Adjustment

### 8.1 Variable Capacitor in Parallel

Add a small air variable capacitor (10–100 pF) in parallel with either C1 or with the whole tank (across L). As you increase this capacitance, the frequency decreases.

```
f_new = 1 / (2π × √(L × (Ceq + C_var)))
```

With Ceq = 1000 pF and C_var ranging 0–100 pF:
- C_var = 0: f = 1 MHz (calculated earlier)
- C_var = 100 pF: Ceq_new = 1100 pF → f = 1/(2π√(25μH × 1100pF)) = 957 kHz

So a 100 pF variable cap tunes the oscillator from 957 kHz to 1 MHz — a range of 43 kHz, or about ±2%.

### 8.2 Ferrite Core Insertion

Wind the coil on a former with a slight gap into which a ferrite rod can be inserted (or use a coil on a form with a screw-adjust ferrite slug — these are sold as adjustable inductors, or you can salvage them from old radios).

Inserting ferrite increases the inductance → decreases frequency.
Pulling ferrite out decreases inductance → increases frequency.

This is the usual method of fine-tuning coils after construction.

### 8.3 Swap the Entire Coil

For a totally different frequency range, wind a different coil. A table of coil values:

| Target Frequency | L (μH) | C1 (pF) | C2 (pF) | Ceq (pF) | Turns (15mm air) |
|-----------------|--------|---------|---------|----------|-----------------|
| 500 kHz | 100 | 1500 | 4700 | 1137 | — (ferrite rod) |
| 1 MHz | 25 | 1500 | 4700 | 1137 | 60 turns |
| 3.5 MHz (80m) | 2 | 1000 | 3300 | 767 | 15 turns |
| 7 MHz (40m) | 1 | 470 | 1500 | 364 | 10 turns |
| 14 MHz (20m) | 0.5 | 220 | 680 | 168 | 7 turns |

---

## 9. Frequency Stability

### 9.1 Why Frequency Drifts

Temperature changes affect every component:
- Inductors: the wire expands with heat, changing the turns spacing and therefore the inductance. A typical air-core coil drifts +50 to +200 ppm/°C.
- Capacitors: depends strongly on type. See table below.
- Transistors: V_BE changes by approximately -2 mV/°C. This shifts the operating current, which shifts the effective input capacitance of the transistor, which changes the frequency.

Temperature coefficient of common capacitor types:

| Type | Temp Coefficient | Drift over 40°C range |
|------|-----------------|----------------------|
| NP0 / C0G | 0 ±30 ppm/°C | ±1.2 ppm total — excellent |
| X5R, X7R | ±15% over range | 15,000 ppm — terrible for tuned circuits |
| Electrolytic | Not rated | Useless for RF tuning |
| Polypropylene film | ±100 ppm/°C | Acceptable |
| Silver mica | ±50 ppm/°C | Good |

**Rule:** Always use NP0/C0G ceramic capacitors for C1 and C2 in the Colpitts oscillator. Never use X5R or X7R (the small blue discs often in parts kits).

### 9.2 Improving Stability: Practical Steps

1. **Use NP0/C0G capacitors** for all tank capacitors. This is the single most important step.
2. **Rigid coil construction.** Varnish the coil with nail varnish or Araldite after winding. Movement of turns shifts inductance.
3. **Shield the coil.** A metal can around the coil (with ventilation holes) prevents nearby conductors from coupling to the coil and shifting frequency.
4. **Buffer the output.** Add an emitter follower (common-collector amplifier) between the oscillator and the load. This prevents load changes from pulling the oscillator frequency.
5. **Temperature-compensate.** If the coil has a positive temperature coefficient, use a capacitor with a negative temperature coefficient to partially cancel. N750 capacitors have -750 ppm/°C and can be used to compensate a positive-drift coil.

### 9.3 Why Crystal Oscillators Are Used in Real Radios

A crystal oscillator's frequency depends on the mechanical dimensions of a quartz crystal — which are stable to better than 1 ppm over a wide temperature range. The drift of an LC oscillator is hundreds or thousands of times worse. 

Your ESP32's 40 MHz clock crystal drifts by less than ±20 ppm over its operating temperature range. Your LC Colpitts oscillator will drift by several thousand ppm just from warm-up. For a broadcast transmitter, this translates to drifting off the assigned frequency. For a receiver's local oscillator, this translates to stations drifting off the station frequency as the receiver warms up.

We will add a crystal oscillator in Stage 9 (the superheterodyne). For now, the LC Colpitts is the foundation to understand.

---

## 10. Build Experiment: AM Transmission

This is the most satisfying experiment in the radio stages. You will transmit your voice on the AM band and receive it on any nearby radio.

### Warning: Legal Note

In most countries, the ISM (Industrial, Scientific, Medical) bands allow very low power unlicensed operation. In India (where this project is based), the Wireless Planning & Coordination Wing permits low-power AM transmission on certain frequencies for experimental use. A single transistor at 9V transmits approximately 1–5 mW — this gives a range of approximately 1–5 metres and is considered incidental radiation at this power level. Keep the antenna very short (10 cm or less). This is a bench experiment, not a broadcast station.

### 10.1 What AM Modulation Is

In an AM (Amplitude Modulated) signal, the frequency of the carrier is fixed but the amplitude varies with the audio signal.

```
AM signal = [1 + m × audio(t)] × cos(2π × f_carrier × t)
```

Where m is the modulation index (0 to 1, or 0% to 100%).

At 100% modulation (m=1), the carrier amplitude swings from zero to double its unmodulated value. This is the maximum amount of AM modulation before the signal becomes overmodulated (distorted).

### 10.2 How to Add AM Modulation to Your Oscillator

The simplest method: vary the supply voltage to the oscillator at the audio frequency. When the supply voltage increases, the oscillation amplitude increases. When it decreases, the amplitude decreases. This is collector modulation.

```
MICROPHONE ---[mic capsule, electret]---[R = 2.2kΩ to +9V]---
                                         |
                                        Audio signal (0.5 - 2V peak-peak)
                                         |
                              [Audio coupling cap 10μF]
                                         |
                              [Junction point on oscillator +9V]

Oscillator supply connects to this audio-modulated +9V line.
```

More precisely:

```
9V Battery (+) ---[100μH RFC]---+--- Oscillator collector supply
                                |
                        [Modulation injection point]
                                |
                     [10μF cap from audio amp output]
                                |
                    [Audio amplifier stage (one BC547)]
                                |
                    [Microphone (electret capsule)]
```

**Audio amplifier circuit for the mic:**

```
                +9V
                 |
            [R_load = 10kΩ]
                 |
             [Collector Q2 (BC547)]
                 |
             [C_out = 10μF]-------> [10μH choke] ---> [oscillator supply junction]
                 
            [Base Q2]---[C_in = 10μF]---[mic signal]
            
            [R_bias1 = 100kΩ] from +9V to base Q2
            [R_bias2 = 22kΩ] from base Q2 to GND
            [RE2 = 1kΩ] from emitter Q2 to GND (no bypass — for stability)
```

The electret microphone capsule is a small two-terminal device. It contains a FET amplifier. Connect it:
- One terminal to +9V through 2.2kΩ (power supply for the FET)
- Other terminal to GND
- Take the audio from the +9V side (high side) through a 10μF coupling capacitor

### 10.3 Procedure

1. Build the oscillator from previous section.
2. Verify it oscillates (AM radio test).
3. Tune the AM radio to the oscillator frequency.
4. Add the microphone amplifier circuit.
5. Couple the audio amplifier output to the oscillator supply through the 10μH choke + 10μF capacitor.
6. Speak into the microphone while listening on the AM radio.

**What you should hear:** Your voice, with some distortion, from the AM radio. The distortion comes from the simplicity of the modulation scheme (collector modulation is not perfectly linear) and the limited audio bandwidth of your microphone amplifier. But it will be recognisably your voice.

**This is AM radio transmission from first principles.** Every AM broadcast transmitter in the world — including the 500,000-watt towers in India that you received on the crystal set — uses exactly this principle: an oscillator + audio modulation. The difference is scale and engineering refinement.

### 10.4 Calculating Transmit Power

The RF power delivered to a load (antenna) is:

```
P = V²_rms / R_load
```

For an oscillator with a 4V peak-to-peak collector swing (measure with RF probe), the peak voltage is 2V, and V_rms = 2/√2 = 1.41V.

If the antenna is connected through a 100Ω series resistor (to limit current):

```
P = (1.41)² / 100 = 1.99 / 100 = 20 mW
```

In practice, the collector voltage swing into the tank circuit will be less than the supply, and the output coupling cap (10 pF) presents a high impedance at 1 MHz:

```
X_C = 1/(2π × 10^6 × 10 × 10^-12) = 15,900Ω
```

So the output through the 10pF cap is quite small — a fraction of a milliwatt. This is intentional. We are using this for bench experimentation only.

For a range of 1 metre in free space, you need approximately 0.1 mW. For 5 metres, you need about 2.5 mW. Your oscillator comfortably covers a bench-top experiment.

---

## 11. The Voltage-Controlled Oscillator (VCO)

So far, the Colpitts oscillator frequency is set by the LC component values. What if you could change the frequency by applying a voltage?

This is the **Voltage-Controlled Oscillator (VCO)** and it is the foundation of:
- FM modulation (audio voltage → frequency change → FM signal)
- Phase-Locked Loops (PLL) — the frequency control system inside every radio, WiFi, and GPS chip
- Synthesised tuning in modern radios

### 11.1 The Varactor Diode

A reverse-biased PN junction diode has capacitance — the depletion region acts as a dielectric between two conducting regions (the P-type and N-type semiconductor). This capacitance decreases as reverse voltage increases (the depletion region widens, increasing the effective plate separation).

```
C_varactor = C_0 / (1 + V_R / V_0)^n
```

Where:
- C_0 = capacitance at zero bias
- V_R = applied reverse voltage (positive number)
- V_0 ≈ 0.6 to 0.7V (built-in potential)
- n ≈ 0.33 to 0.5 (doping profile dependent)

A cheap BB105 or BB112 varactor diode changes from about 100 pF at 1V reverse bias to 20 pF at 10V reverse bias. That is a 5:1 capacitance range.

**Important:** Even an ordinary 1N4148 signal diode, reverse-biased, has a few picofarads of junction capacitance that changes with voltage. It is a poor varactor but it works for demonstration.

### 11.2 Adding VCO Control to the Colpitts

Replace C2 (the lower tank capacitor) with the series combination of a fixed capacitor and a varactor diode:

```
Old: [C2 = 4700pF fixed]

New: [C_series = 10nF fixed] ---[varactor diode, cathode to control voltage]
         |
     [R_control = 100kΩ] --- [Control voltage 0 to 9V]
         |
        GND
```

The 10nF series cap blocks DC from entering the tank. The varactor's capacitance changes with the control voltage, changing the effective tank capacitance, changing the oscillation frequency.

**FM modulation:** Replace the DC control voltage with audio. The audio voltage modulates the varactor capacitance, which modulates the frequency. This is how FM modulation works at its most fundamental level.

### 11.3 Demonstration Experiment

1. Build the standard Colpitts oscillator.
2. Replace C2 with the varactor + series cap combination.
3. Connect a 10kΩ potentiometer between 9V and GND. The wiper goes to the varactor control voltage.
4. Place your AM radio near the oscillator and tune to the carrier.
5. Turn the potentiometer. The frequency changes — you can hear the AM radio needs to be retuned to follow the drifting carrier.
6. Now connect a sine wave generator (or even the audio signal from a phone at low level) to the control voltage through a 1μF coupling capacitor.
7. The oscillator frequency wobbles with the audio. You have FM modulation.

The FM broadcast band is 88–108 MHz. Your oscillator is at 1 MHz. But the principle is identical.

---

## 12. Troubleshooting

| Symptom | Likely Cause | Diagnosis | Remedy |
|---------|-------------|-----------|--------|
| No oscillation — AM radio test shows nothing | Loop gain < 1 | Measure V_C. If correct (4–6V), gain is OK but feedback is wrong. | Check L and tank caps are correct values. Check C1/C2 ratio. Increase C1/C2 ratio to increase β. |
| No oscillation — V_C is near 0V or 9V | Bias wrong | Measure V_B, V_E, V_C | Recalculate bias resistors |
| Oscillating at wrong frequency | L or C values wrong | Use AM radio to find actual frequency | Adjust component values using frequency formula |
| Frequency very unstable — wanders visibly on AM radio | Component quality poor OR insufficient supply decoupling | Check cap types (must be NP0) | Replace X5R/X7R caps with NP0. Add more supply decoupling. |
| Output frequency changes when you touch the circuit | Circuit loading through stray capacitance (hand effect) | Normal if output not buffered | Add emitter follower buffer between collector and output. Shield the coil. |
| AM modulation works but voice is very distorted | Modulation depth too high (overmodulation) | Reduce audio level | Add 10kΩ series resistor between audio amp output and modulation injection point |
| Oscillates but at multiple frequencies simultaneously | Parasitic oscillation | Measure at different points | Add 100Ω series resistor in collector lead. Add ferrite bead on collector. |

---

## 13. What to Record in Your Notebook

```
Colpitts Oscillator Experiment Log

Build date: ___________

Component values used:
L = ___μH  (measured or calculated)
C1 = ___pF  (type: NP0 / X7R / other)
C2 = ___pF  (type: NP0 / X7R / other)
RC = ___Ω
RE = ___Ω
R1 = ___kΩ
R2 = ___kΩ

DC operating point:
V_B = ___V  (measured)
V_E = ___V  (measured)
V_C = ___V  (measured)
I_C = (V_CC - V_C)/RC = ___mA

Oscillation confirmed? (AM radio test): YES / NO
If yes, frequency found at: ___kHz on AM radio

Calculated frequency: ___kHz
Measured frequency: ___kHz
Difference: ___kHz (___%)

RF probe voltage at collector: ___V DC
Estimated RF voltage: ___V peak

Frequency adjustment experiments:
C_var added in parallel: ___pF → new freq: ___kHz
Ferrite inserted into coil: freq changes by ___kHz

AM modulation experiment:
Voice audible on AM radio? YES / NO
Modulation quality (1-5): ___
Distance at which received: ___cm

VCO experiment:
Varactor type: ___
Control voltage range tested: ___V to ___V
Frequency range achieved: ___kHz to ___kHz
Total tuning range: ___kHz (___%)

Notes and observations:
___________
```

---

## 14. Key Insight

You have just built a signal generator that produces a continuous radio wave.

Look at the WiFi antenna on your ESP32 module. The 2.4 GHz signal it broadcasts is produced by a circuit that is conceptually identical to your Colpitts oscillator — an LC (or more precisely, an on-chip transmission-line) resonator with a transistor (a CMOS FET, but same principle) and positive feedback.

The difference between your 1 MHz oscillator and the ESP32's 2.4 GHz oscillator:
- Frequency: 2400× higher (requires much smaller L and C, using on-chip structures)
- Stability: crystal reference with PLL locks the frequency to ±20 ppm
- Power: the ESP32 produces 100 mW; your oscillator produces about 1 mW
- Miniaturisation: the entire oscillator in the ESP32 occupies less than 1 mm² of silicon

But the physics — Barkhausen criterion, loop gain = 1, LC resonance — is exactly the same. The chip does not do anything fundamentally different from what you have on your breadboard. It does it faster, smaller, and more stably.

The Colpitts oscillator is not historical curiosity. It is what is inside the radio chip in your drone right now.

---

*Next: Stage 8 — The Mixer. You will take two radio frequencies and create a third, unlocking the principle behind every superheterodyne receiver, every software-defined radio, and every frequency-hopping communication link.*
