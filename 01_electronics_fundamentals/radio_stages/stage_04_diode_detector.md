# Stage 4 — The Diode Detector: Seeing Inside the Signal

---

## What You Will Learn

- The complete I-V characteristic of a diode — how it really behaves, not just "passes current one way"
- The exact threshold voltage of silicon, Schottky, germanium, and galena crystal diodes — and why the difference matters
- A quantitative calculation showing why 1N4148 fails on a crystal set signal and OA91 works
- Envelope detection process explained in full — waveforms drawn at every point in the circuit
- How to choose the RC filter values after the detector — the maths behind the right choice
- How to build and use a galena crystal detector (the original radio detector, 1904–1940s)
- Three build experiments that make these concepts directly observable
- A comparison of every diode type relevant to radio detection
- The same circuit inside old television sets — connecting to a familiar application

**Prerequisites:** Stage 2 (crystal set). You should understand the basic LC tank circuit. Stage 3 is helpful for the Q factor and selectivity concepts but is not required for this stage.

**Time required:** 2–3 hours reading + 3–5 hours for experiments (finding galena adds time)

**Cost:** ₹200–600 / $3–10 USD. Most cost is in the galena crystal if purchased (or free if found as iron pyrite or galena mineral).

---

## Part 1 — The Diode as a One-Way Valve: Full I-V Characteristic

### The Simple Model (and Why It's Wrong)

The standard introduction to diodes says: "A diode passes current in the forward direction (anode positive) and blocks it in the reverse direction." This is a useful simplification for audio and power electronics. For radio detection, it is dangerously wrong.

The real diode does not switch from "off" to "on" instantly. It transitions gradually. The exact shape of this transition determines whether the diode can detect small signals or not.

### The Shockley Diode Equation

The exact relationship between current and voltage for an ideal p-n junction diode is:

```
    I = IS × (e^(V/nVT) - 1)

Where:
  I  = current through the diode (Amperes, positive = forward)
  IS = reverse saturation current (Amperes) — a property of the specific diode
  V  = voltage applied to the diode (Volts, positive = forward bias)
  e  = Euler's number = 2.71828...
  n  = ideality factor: 1 for an ideal junction, 1–2 in practice
  VT = thermal voltage = kT/q = 0.02585 V ≈ 26 mV at room temperature (300 K)
```

The thermal voltage VT = kT/q = (Boltzmann constant × temperature) / electron charge. At room temperature (T = 300 K):

```
    VT = (1.381×10⁻²³ × 300) / (1.602×10⁻¹⁹)
       = 4.143×10⁻²¹ / 1.602×10⁻¹⁹
       = 0.02587 V ≈ 26 mV
```

**The two critical parameters that vary between diode types are IS and n.**

### IS Values for Different Diodes

IS is the reverse saturation current — the tiny current that flows when the diode is reverse-biased (by a few volts). It is set by the semiconductor physics:

| Diode Type | IS (typical) | Physical reason |
|-----------|-------------|-----------------|
| Silicon p-n junction (1N4148) | 10⁻¹² to 10⁻¹⁴ A (1 pA) | Wide bandgap (~1.1 eV) — few thermally generated carriers |
| Schottky (BAT43) | 10⁻⁷ to 10⁻⁹ A (100 nA) | Metal-semiconductor junction, lower barrier |
| Germanium p-n junction (OA91) | 10⁻⁶ to 10⁻⁷ A (1 µA) | Narrow bandgap (~0.67 eV) — many more thermally generated carriers |
| Galena crystal (PbS) | 10⁻⁷ to 10⁻⁵ A | Point contact junction, variable — depends on contact spot |

**Why IS matters:** The larger IS is, the more current flows at small forward voltages. This is exactly what we need for small signal detection.

### The I-V Curve, Plotted in Words

**For Silicon (1N4148):**
- At V = 0: I = 0
- At V = 100 mV: I ≈ IS × e^(100/52) = 10⁻¹² × e^1.923 = 10⁻¹² × 6.84 = 6.84 pA (negligible)
- At V = 300 mV: I ≈ 10⁻¹² × e^(300/52) = 10⁻¹² × e^5.77 = 10⁻¹² × 319 = 0.32 nA (still negligible)
- At V = 500 mV: I ≈ 10⁻¹² × e^(500/52) = 10⁻¹² × e^9.62 = 10⁻¹² × 14,990 = 15 nA (just barely something)
- At V = 600 mV: I ≈ 10⁻¹² × e^(600/52) = 10⁻¹² × e^11.54 = 10⁻¹² × 103,000 = 103 nA = 0.1 µA
- At V = 700 mV: I ≈ 0.7 mA (significant current — "fully on" region)

**For Germanium (OA91), using IS = 1 µA, n = 1:**
- At V = 0: I = 0
- At V = 10 mV: I = 1×10⁻⁶ × e^(10/26) = 1×10⁻⁶ × e^0.385 = 1×10⁻⁶ × 1.47 = 0.47 µA
- At V = 50 mV: I = 1×10⁻⁶ × e^(50/26) = 1×10⁻⁶ × e^1.923 = 1×10⁻⁶ × 6.84 = 6.84 µA
- At V = 100 mV: I = 1×10⁻⁶ × e^(100/26) = 1×10⁻⁶ × e^3.846 = 1×10⁻⁶ × 46.8 = 46.8 µA (significant — "turning on")
- At V = 200 mV: I = 1×10⁻⁶ × e^(200/26) = 1×10⁻⁶ × e^7.69 = 1×10⁻⁶ × 2191 = 2.19 mA (fully on)

**The visual picture:**

```
Current (I)
    ↑
    │                                Si (1N4148)
    │                              /
    │                            /   ← steep on/off, threshold ~0.6V
    │                          /
    │                        /
    │    Ge (OA91)          /
    │      ╱             /
    │    ╱              /  ← Ge turns on at lower voltage, more gradually
    │  ╱              /
    │╱              /
────┼────────────────────────────→ Voltage (V)
   0V  0.1V  0.2V  0.3V  0.5V  0.7V

The germanium curve is shifted LEFT relative to silicon.
At any given small voltage, germanium passes much more current.
```

### The Threshold Voltage — What It Really Means

"Threshold voltage" is not a single number — it is a convention. By convention it means the voltage at which "significant" current flows. "Significant" usually means 1 mA for a power circuit.

For a crystal set detector, the signal voltages are 5–50 mV. The question is: how much current flows at 10 mV?

**At 10 mV:**
- Silicon (1N4148): I ≈ 0.2 fA (0.2 × 10⁻¹⁵ A) — essentially zero, undetectable
- Germanium (OA91): I ≈ 0.47 µA — 2.3 million times larger than silicon, detectable

The germanium diode "works" at 10 mV not because it has "overcome" any threshold — but because IS is a million times larger, and so even in the very nonlinear, low-current region of the curve, the current is large enough to do something useful.

---

## Part 2 — Complete Diode Type Comparison Table

| Diode | Type | IS (typical) | Vf at 1 mA | Vf at 1 µA | RF performance | Availability |
|-------|------|-------------|-----------|-----------|----------------|-------------|
| 1N4148 | Silicon p-n | ~1 pA | 0.6 V | 0.39 V | Poor — misses weak signals | Ubiquitous, cheap |
| 1N914 | Silicon p-n | ~1 pA | 0.6 V | 0.39 V | Poor (same as 1N4148) | Ubiquitous |
| BAT43 | Schottky | ~100 nA | 0.25 V | 0.15 V | Good — moderate IS | Common, eBay |
| BAT85 | Schottky | ~100 nA | 0.25 V | 0.15 V | Good | Common |
| 1N5711 | Schottky | ~30 nA | 0.30 V | 0.20 V | Good | Common |
| OA91 | Germanium p-n | ~1 µA | 0.18 V | 0.09 V | Excellent — large IS | eBay, electronics markets |
| OA47 | Germanium p-n | ~2–5 µA | 0.15 V | 0.07 V | Excellent | eBay |
| OA70 | Germanium p-n | ~1 µA | 0.18 V | 0.09 V | Excellent | eBay |
| 1N34A | Germanium p-c | ~1 µA | 0.20 V | 0.10 V | Excellent | eBay |
| Galena (PbS) | Point contact | Varies | Varies 0.1–0.3V | Varies | Excellent when tuned | Mineral shops, eBay |
| Iron pyrite (FeS₂) | Point contact | Varies | Varies | Varies | Good | Mineral shops |
| Silicon carbide (SiC) | Point contact | Varies | Varies | Variable | Historically used, unpredictable | Abrasive paper, Carborundum |

**Recommendation:**
- First choice: OA91 or any germanium diode
- Second choice: BAT43 or BAT85 Schottky (if no germanium)
- Third choice: 1N5711 Schottky
- Never: 1N4148 or any standard silicon diode for a crystal set
- Special project: Galena crystal (see Part 6)

### How to Test Your Diode Type

With a multimeter in diode-test mode (the symbol ▷| on the dial):

1. Connect red probe to anode, black probe to cathode.
2. The display shows forward voltage in millivolts (some meters) or in units.

| Reading | Diode type |
|---------|-----------|
| 550–700 mV | Silicon (1N4148, 1N4001, etc.) |
| 200–300 mV | Schottky (BAT43, BAT85) |
| 100–250 mV | Germanium (OA91, 1N34A, OA47) |
| Open circuit (∞) | Diode is blown, or probes on wrong way |
| 0 V (beep) | Diode is shorted |

---

## Part 3 — The Rectification Process: What Happens at Each Circuit Node

This section traces the signal from the antenna all the way to the earphone, with a drawing of what the waveform looks like at each point. This is the most important conceptual picture in AM radio.

### The Signal Before the Tank Circuit

At the antenna terminal, before the LC tank, you have a mixture of all radio signals present — every AM station in your area, every noise source, overlapping in time. The waveform looks like random noise (it IS the sum of many sine waves at different frequencies with random phases).

```
Voltage at antenna (very schematic):

↑ +50mV
│ ╭╮ ╭─╮ ╭╮╮ ╭╮   ╭╮ ╭─╮
│╭╯╰─╯ ╰─╯╰╯╰─╯╰──╯╰─╯ ╰─...  ← complex mixture of all signals
│
0V─────────────────────────────
│
│╰╮         ╭╮      ╭╮
↓ ╰─────────╯╰──────╯    -50mV (approx)
```

### At the Output of the LC Tank (Before the Diode)

The LC tank is a bandpass filter centred on one station's carrier frequency. Everything else is attenuated. What remains is the AM signal from that one station:

```
AM signal at tank output for a station at 1 MHz carrying audio:

        ←————— one audio half-cycle ————→
     ↑
+20mV│    ╭╮ ╭╮ ╭──╮ ╭──╮ ╭──╮ ╭╮ ╭╮         ← RF carrier (1 MHz)
     │   ╭╯╰─╯╰─╯  ╰─╯  ╰─╯  ╰─╯╰─╯╰╮        ← amplitude rises and falls
     │  ╭╯                             ╰╮       ← at audio rate (e.g. 1 kHz)
  0V─┼──────────────────────────────────────
     │  ╰╮                             ╭╯
     │   ╰╮ ╮╭ ╮╭   ╭╮   ╭╮   ╭╮ ╭╮ ╭╯
-20mV│    ╰╯ ╰╯ ╰───╯╰───╯╰───╯╰─╯╰─╯
     ↓

This is the amplitude-modulated signal.
The audio information is contained in the ENVELOPE — the slowly-varying
outer boundary of the rapidly oscillating carrier.
```

**Key numbers:**
- RF carrier: 1 MHz oscillations, 1 µs period
- Audio modulation: 1 kHz (for a single tone), 1 ms period
- The envelope traces out the audio waveform

### At the Cathode of the Diode (After Half-Wave Rectification)

The diode passes only the positive half-cycles. Negative half-cycles are blocked (cathode-side is the output, anode-side connects to the tank).

```
After the diode (half-wave rectified):

     ↑
+20mV│    ╭╮ ╭╮ ╭──╮ ╭──╮ ╭──╮ ╭╮ ╭╮
     │   ╭╯╰─╯╰─╯  ╰─╯  ╰─╯  ╰─╯╰─╯╰╮
     │  ╭╯                             ╰╮
  0V─┼──────────────────────────────────────
     │  (flat — negative halves blocked)
     ↓

The positive envelope is now visible.
The negative half-cycles are gone.
```

The peaks of these rectified pulses trace the audio envelope. Between the peaks, the voltage drops toward zero (falls to ground through R1 between pulses). The 1 MHz ripple is still very much present — we haven't filtered it yet.

### After the RC Low-Pass Filter

The RC filter (R1 in parallel with C1) smooths out the 1 MHz ripple. Here is what happens:

- The capacitor C1 charges up rapidly on each positive peak
- Between peaks (for 0.5 µs — half the RF period), the capacitor discharges slowly through R1
- If RC >> 0.5 µs, the capacitor barely discharges between pulses — the voltage stays close to the peak value
- If RC << 0.5 µs, the capacitor fully discharges between pulses — no smoothing

With R1 = 100 kΩ and C1 = 1 nF:
```
    RC = 100,000 × 1×10⁻⁹ = 100 µs
```
100 µs >> 0.5 µs (the RF half-period). The ripple is largely smoothed.

```
After the RC filter:

     ↑
+15mV│   ╭────────────────────────╮
     │  ╱                          ╲
     │ ╱                            ╲   ← audio waveform (smooth)
     │╱                              ╲
  DC─┼──────────────────────────────────  ← DC level (average of rectified signal)
     ↓

This is the recovered audio, with a DC offset.
The DC offset is removed by C_out (the output coupling capacitor).
```

### After C_out (Output Coupling Capacitor)

C_out is typically 47 nF. It blocks DC (infinite impedance at DC) and passes audio (low impedance at 1 kHz: XC = 1/(2π×1000×47×10⁻⁹) = 3380 Ω).

The waveform after C_out is the pure audio signal with no DC offset — ready for the earphone.

```
After C_out:

     ↑
+5mV │   ╭───────────────╮
     │  ╱                 ╲
     │ ╱                   ╲
  0V─┼───────────────────────────────  ← zero line (no DC offset now)
     │                       ╲   ╱
     │                        ╲ ╱
-5mV ↓                         ╰

This is the audio signal ready for the earphone.
```

---

## Part 4 — Choosing the RC Filter Values

### The Two Constraints

The RC filter must satisfy two inequalities simultaneously:

**Constraint 1 — Filter out the RF carrier:**
The RC time constant must be much longer than the RF period:

```
    RC >> T_RF = 1/f_carrier

    For f_carrier = 1 MHz:  T_RF = 1 µs
    Therefore: RC >> 1 µs
```

**Constraint 2 — Pass the audio frequencies:**
The RC time constant must be much shorter than the audio period:

```
    RC << T_audio_min = 1/f_audio_max

    For f_audio_max = 5 kHz:  T_audio_min = 200 µs
    Therefore: RC << 200 µs
```

**Combined constraint:**
```
    1 µs << RC << 200 µs

    A good target: RC somewhere in the middle, approximately 10–50 µs
```

### Worked Example

Choose RC = 47 µs:

Option 1: R = 10 kΩ, C = 4.7 nF:
```
    RC = 10,000 × 4.7×10⁻⁹ = 47×10⁻⁶ s = 47 µs ✓
    RF filtering: 47 µs >> 1 µs ✓ (47× margin)
    Audio passing: 47 µs << 200 µs ✓ (4.3× margin)
```

Option 2: R = 47 kΩ, C = 1 nF:
```
    RC = 47,000 × 1×10⁻⁹ = 47×10⁻⁶ s = 47 µs ✓ (same)
```

Higher R is better for the crystal set because it loads the detector less (higher impedance = draws less current from the detector, wastes less signal). Use R = 47 kΩ to 470 kΩ.

### The Effect of Wrong Values

**RC too small (e.g., R = 1 kΩ, C = 1 nF → RC = 1 µs):**
- RF barely filtered — the output still has 1 MHz ripple
- Earphone hears buzzing, harsh sound at the RF frequency
- Actually, 1 MHz is above the earphone's frequency range, so you'd mainly hear distortion
- In practice: audio sounds harsh and buzzy

**RC too large (e.g., R = 1 MΩ, C = 100 nF → RC = 100 ms):**
- Audio frequencies are also filtered! The high-frequency components of speech (consonants, sibilants) are removed.
- Audio sounds muffled, like hearing through a pillow
- Intelligibility drops — you can hear vowels but not consonants

**Summary:**

| RC Value | RF filtered? | Audio preserved? | Sound quality |
|----------|-------------|-----------------|---------------|
| 1 µs | Poor | Yes | Buzzy/harsh |
| 10 µs | Yes | Yes | Good |
| 50 µs | Yes | Yes | Good |
| 200 µs | Yes | Partial (treble cut) | Muffled |
| 10 ms | Yes | Poor | Very muffled |

---

## Part 5 — Build Experiment 1: Observe Rectification on an Oscilloscope (or Estimate Without One)

**Goal:** Make the difference between silicon and germanium diode threshold directly observable. See the waveform before and after the diode.

**What this proves:** The I-V curve is not just theory — you can see its effect on the waveform.

### Method A: With an Oscilloscope

**Materials:**
- Function generator (or 555 timer oscillator — see schematic below)
- Oscilloscope (or if not available, proceed to Method B)
- 1N4148 silicon diode
- OA91 (or any germanium) diode
- BAT43 Schottky diode (if available)
- 10 kΩ resistor
- Breadboard

**Circuit:**

```
Function generator → [signal lead] → ┬─── [diode] ─── [R 10kΩ] → GND
(1 kHz, 50–200 mV pp)               │                     │
                                     └── Oscilloscope Ch1  └── Oscilloscope Ch2

Ch1: measures input signal (before diode)
Ch2: measures output signal (across R, after diode)
```

**Procedure:**

1. Set function generator to 1 kHz sine wave, 50 mV peak-to-peak (this is a small signal representative of what a crystal set sees).
2. Connect Ch1 to the input of the diode. Observe and sketch the waveform.
3. Connect Ch2 to the junction of diode and resistor. Observe and sketch the output with the 1N4148 installed.
4. Replace the 1N4148 with the OA91 germanium diode. Observe the output again.
5. (If available) replace with BAT43. Observe.

**What you should see:**

With 1N4148 at 50 mV input:
- Output: essentially zero — the signal is far below threshold, the diode barely conducts
- You might see a tiny lump on the positive peaks, but signal amplitude is in microvolts

With OA91 at 50 mV input:
- Output: half-wave rectified signal — positive half-cycles of the input appear at the output
- The peaks are somewhat reduced (Vf ≈ 0.1 V), but the signal is clearly rectified and detectable

**Now increase the amplitude to 2V peak-to-peak and repeat:**

With 1N4148 at 2V input:
- Output: clear half-wave rectification — positive half-cycles, with about 0.6 V drop from peak

With OA91 at 2V input:
- Output: similar half-wave rectification, but with only about 0.15 V drop from peak

**The visual demonstration:** At small signals (50 mV), only the germanium works. At large signals (2V), both work but germanium has less voltage loss. The threshold voltage difference is made visible.

### Method B: Without an Oscilloscope — Using an Audio Crystal Set

You already built a crystal set in Stage 2. This is your test instrument.

**Procedure:**

1. Find a strong AM station (tune in, check you can hear it with the OA91 diode).
2. Note the audio volume. Call this "100%".
3. Replace the OA91 with a 1N4148 silicon diode (same circuit position).
4. Re-tune slightly (the different diode capacitance may shift the resonance slightly).
5. Note the audio volume. Call this "X%".

Expected result:
- For a strong local station (within 20 km of the transmitter): the 1N4148 may actually produce some audio — the signal may be large enough to exceed the 0.6V threshold on strong peaks
- For a medium-distance station: the 1N4148 produces nothing, the OA91 produces clear audio
- For a weak or distant station: the 1N4148 produces nothing, the OA91 may just barely work

**Result table:**

| Diode | Station 1 (strong, nearby) | Station 2 (medium) | Station 3 (weak, distant) |
|-------|---------------------------|-------------------|--------------------------|
| OA91 (germanium) | | | |
| 1N4148 (silicon) | | | |
| BAT43 (Schottky, if available) | | | |

### 555 Timer Signal Generator (if No Function Generator)

If you don't have a function generator, build this simple oscillator for Experiment 1:

```
9V supply

        ┌─────────────────────────────┐
        │                             │
        R1 (1kΩ)                      │
        │                             │
        R2 (10kΩ)                     │
        │           ┌─────────┐       │
        ├───────────┤2,6     3├─ Out  │
        │           │         │       │
        │   C1      │  555    │       │
        ├─┤10nF├────┤         │       │
        │           │  GND  4 ├─ to + │
        GND         └───┬─────┘       │
                        │             │
                       GND           GND
```

This generates a square wave at approximately:
```
    f = 1.44 / ((R1 + 2R2) × C1) = 1.44 / (21000 × 10⁻⁸) = 6857 Hz ≈ 7 kHz
```

Not 1 kHz, but close enough for demonstration. A square wave is not a pure sine, so the rectification waveform will look somewhat different — but the threshold effect of different diodes will still be clearly visible.

---

## Part 6 — Build Experiment 2: The Detector Stage Alone

**Goal:** Build and test just the detector (diode + RC filter) as a standalone circuit. Measure and compare output with different diodes.

**This is important:** A complete crystal set has many variables. Isolating the detector stage lets you characterise the diode directly.

### Circuit

```
DETECTOR TEST CIRCUIT:

Input signal ─────────────── [D1] ─────────────── Output
(from signal generator          │
or crystal set output)          ├── [C1 1nF] ──┐
                                │               │
                              [R1 100kΩ]       GND
                                │
                               GND

Measure: Vin (AC, before diode) with multimeter on AC mode, 200mV range
         Vout (DC, after filter) with multimeter on DC mode, 200mV range
```

### Procedure

1. Connect a signal source to the input. Start with your crystal set output (the hot terminal of the LC tank).
2. Measure Vin (AC voltage at the diode anode — before the diode).
3. Measure Vout (DC voltage across R1 — after the filter).
4. Calculate the "detection efficiency": η = Vout / Vin × 100%

Repeat with: OA91, BAT43, 1N4148.

**For a strong station you might find:**
- OA91: Vin = 15 mV AC, Vout = 8 mV DC → η = 53%
- BAT43: Vin = 15 mV AC, Vout = 4 mV DC → η = 27%
- 1N4148: Vin = 15 mV AC, Vout ≈ 0 mV DC → η ≈ 0%

**Note:** The multimeter is a lousy instrument for this — it cannot easily measure mV-level audio signals or RF. If you have access to an oscilloscope, use it instead. If you have only a multimeter, the result is qualitative but still informative.

### Measuring Detection Efficiency More Practically

The most practical measurement: put the earphone in your ear and rate the volume on a scale of 1–5 for each diode. This is a direct subjective test of detection efficiency that requires no instruments.

| Diode | Station 1 volume (1–5) | Station 2 volume | Station 3 volume |
|-------|----------------------|-----------------|-----------------|
| OA91 | | | |
| BAT43 | | | |
| 1N4148 | | | |
| Galena (if built) | | | |

---

## Part 7 — The Galena Crystal Detector: Build One

The galena crystal detector was the first practical radio detector, invented around 1904. It predates the germanium diode by 45 years. It is made from galena (lead sulphide, PbS) — a natural semiconductor mineral.

The detector works because galena is a semiconductor. At the point where a fine metallic wire (the "cat's whisker") presses against the crystal surface, a metal-semiconductor (Schottky-like) junction forms. This junction has rectifying properties similar to a germanium diode — low forward voltage, usable at small signal levels.

### Why Build One?

1. It is a direct connection to radio history — this is what Marconi's receivers used.
2. It teaches you about naturally occurring semiconductor junctions.
3. It works. A good galena contact is actually slightly better than a germanium diode for very weak signals because its forward voltage can be as low as 0.05V.
4. It is a fascinating piece of hands-on historical reconstruction.

### Where to Get Galena

- **Mineral/crystal shops:** Galena is a common and cheap mineral. Ask for "galena" or "lead sulphide crystal". A small 20 mm × 20 mm × 10 mm piece is more than enough. Cost: ₹50–200 / $2–5. Galena looks like bright metallic grey cubic crystals with perfect cleavage.
- **Online:** eBay "galena crystal radio detector mineral" — often available as crystal radio specimens.
- **Iron pyrite (fool's gold):** FeS₂, also works as a crystal detector but is less sensitive than galena. Available in the same mineral shops. Harder to find a sensitive spot.
- **Carborundum (Silicon Carbide, SiC):** Historically used (invented 1904, same year as galena). Found in abrasive grinding wheels and abrasive paper. Works, but typically requires a bias voltage (0.5–2V forward bias) for best sensitivity — not ideal for an unpowered crystal set.

### Building the Crystal Detector Holder

The key challenge is making a reproducible, adjustable contact between the cat's whisker wire and the crystal.

**Materials:**
- Galena crystal piece (or iron pyrite)
- Small metal cup or tray to hold the crystal: a bottle cap, a small nut or washer, a small coin soldered to a wire
- Fine copper or phosphor-bronze wire: 0.1–0.3 mm diameter (from enamelled wire or a fine spring from a ballpoint pen)
- Small bolt (M3 or M4), nut, and a short thin metal strip or small piece of printed circuit board
- Small pliers and thin-nose pliers

### The Holder Design

```
TOP VIEW:

  ┌─────────────────────────────┐
  │                             │
  │  [Crystal in cup]           │   ← Crystal soldered or hot-glued
  │                             │       into a small bottle cap
  │  ↑                          │
  │  │ Cat's whisker             │
  │  │ (fine wire bent           │
  │  │  to a point)              │
  │  │                          │
  │  [Adjustment arm]           │   ← A thin metal strip, bent to form
  │  (pivots up and down,        │       a cantilever that presses the wire
  │   controlled by pressing     │       gently against the crystal
  │   the wire against crystal)  │
  │                             │
  │  [Binding post] [Binding post] │   ← Crystal and cat's whisker
  └─────────────────────────────┘       each connected to a terminal
```

### Step-by-Step Construction

**Step 1: Mount the crystal.**

Take a bottle cap (metal, from a bottle) or a small washer. Hot-glue or Blu-Tack the galena crystal into the centre of the cap. The crystal should be held securely but not chemically damaged (galena is lead sulphide — it is inert, but don't use strong solvents).

The cap becomes one terminal of the detector. Solder or clamp a wire to the outside of the cap. This wire goes to one terminal in your crystal set circuit.

**Step 2: Make the cat's whisker.**

Take a piece of fine wire — 0.1 to 0.3 mm diameter. Copper from enamelled wire works. Phosphor-bronze wire (available from model engineering suppliers) is more spring-like and holds its shape better.

Bend a 5 cm piece of wire into an L-shape. The short end (about 5 mm) points downward and is bent to a fine point. This is the cat's whisker.

**Step 3: Make the adjustment arm.**

The cat's whisker needs to be held against the crystal at a controlled, adjustable pressure. Too light: no contact, no detection. Too heavy: the spring effect is lost and the contact is not a true point — sensitivity drops.

Simple design: Clip the long end of the cat's whisker wire into a small bulldog clip or clothespeg mounted on the base, with the short end pointing down toward the crystal. Height adjustment: bend the wire to aim.

Better design: Mount the long end of the wire on a small M3 bolt passing through the base. The bolt threads through the base and the height of the tip is adjusted by threading the bolt in or out.

**Step 4: Connecting**

Two connections come out of the detector:
1. From the crystal cup (this is one pole of the junction)
2. From the cat's whisker wire (this is the other pole)

These are connected to the circuit in the same way as the anode and cathode of a diode. But unlike a diode, the galena detector has no polarity marking — you try both orientations.

### Using the Galena Detector

This is the most time-consuming and skilled part. Finding a sensitive spot on the galena is a tactile skill that requires practice.

**How to find a sensitive spot:**

1. Connect the galena detector in your crystal set circuit in place of the diode.
2. Tune the radio to a known strong station.
3. With the cat's whisker lightly resting on the crystal, gently move it across the crystal surface. Small movements — 0.5 mm at a time.
4. At each position, listen for a moment. If you hear audio (even faint), you have found a sensitive spot.
5. Once you find a spot with audio, try tiny adjustments of pressure and position to maximise the signal.
6. Mark the spot with a tiny pencil mark on the crystal if you want to return to it.

**Characteristics of a sensitive spot:**
- Usually on a clean, smooth crystal face
- Often at or near a crystal grain boundary
- Sensitive to vibration — do not bump the table while using it
- The contact requires fresh finding every time the cat's whisker is disturbed
- Different spots on the same crystal give different sensitivity — some spots are excellent, many are poor

**Why it works better at some spots:** The rectifying junction forms where the metal wire presses against the semiconductor. The junction quality depends on the local crystal structure, surface cleanliness, and the oxide layer (galena oxidises in air, forming a thin PbO layer which can be beneficial or not). The best spots are those where a good Schottky-like junction forms.

**Historical context:** Before reliable semiconductor diodes, radio operators would spend considerable time "wiggling the cat's whisker" to find a good spot. Galena detectors were temperamental and not suited for mobile use. Vacuum tube diodes (thermionic valves) replaced them for reliability, until semiconductor diodes (first germanium, then silicon) took over.

### Storing the Detector

Once you find a good spot, do not disturb the crystal. Store the detector where it won't be vibrated. Cover it to keep dust off the crystal surface.

---

## Part 8 — Schottky Diode Physics

### The Metal-Semiconductor Junction

A Schottky diode is formed by direct contact between a metal and a semiconductor (usually N-type silicon or gallium arsenide). Unlike a p-n junction (which uses two semiconductor regions), a Schottky junction uses:

- Metal: tungsten, platinum, or gold (depending on the application)
- N-type semiconductor: silicon or GaAs

When these materials touch, electrons from the semiconductor flow into the metal until an equilibrium is established. A depletion region forms in the semiconductor (similar to a p-n junction's depletion region), creating a potential barrier.

### Why the Forward Voltage Is Lower

In a p-n junction, the potential barrier is related to the bandgap of the semiconductor and the doping levels. For silicon, this is about 0.6–0.7 eV.

In a Schottky junction, the barrier height depends on the metal-semiconductor combination. For platinum on N-silicon, the barrier is about 0.85 eV (higher than p-n — used for power rectifiers). For gold on GaAs, it can be 0.8 eV. For tungsten on N-silicon, about 0.6 eV. For certain metal combinations on silicon, the barrier can be as low as 0.2–0.3 eV.

This is why Schottky diodes can have lower forward voltages than p-n junctions.

### Why Schottky Diodes Are Faster

In a p-n junction, minority carriers are injected across the junction. When you reverse the voltage, these carriers must first be swept out before the junction becomes blocking — this is the "reverse recovery time", typically 50–1000 ns for standard silicon diodes. This limits how fast the diode can switch.

In a Schottky junction, there are no minority carriers — it is a majority carrier device. When the voltage is reversed, the depletion region widens immediately and the junction blocks almost instantly. Schottky diodes can switch in picoseconds. This makes them excellent for RF detection and high-frequency rectification.

---

## Part 9 — Point Contact Diodes

The cat's whisker on galena is an example of a point contact device. Early semiconductor diodes (before 1950s planar technology) were made this way — a fine metal wire pressed or welded to a semiconductor surface.

The advantage of point contact: the contact area is extremely small (1–10 µm diameter). Capacitance is proportional to area (C = εA/d). Very small area = very small junction capacitance = very fast switching = usable at very high frequencies.

The disadvantage: fragile, variable, not reproducible, temperature sensitive.

The first silicon transistor (Bell Labs, 1947) was a point contact transistor — two cat's whiskers on a piece of germanium. Point contact devices were soon replaced by planar diffusion technology, but the Schottky diode can be seen as the modern equivalent of the point contact rectifier.

---

## Part 10 — The Video Detector in Old Television Sets

AM detection appears inside every analogue television set — for the video signal.

In analogue PAL/NTSC television:
- The video information is transmitted as AM modulation on a carrier at around 40 MHz (for the IF stage, after downconversion from the broadcast frequency)
- The detector circuit at the output of the IF amplifiers is exactly the crystal set detector: a diode followed by an RC filter

The difference from an AM radio detector:
- The carrier frequency is 40 MHz (not 1 MHz) — the RC filter must be faster
- The "audio" is actually video at up to 5 MHz bandwidth (much higher than speech)
- A silicon diode (1N4148 or specialised video detector diode) works here because the signal has been amplified to volts by the IF amplifier stages before reaching the detector

This means: every old television made before about 2000 contains an AM envelope detector — the same circuit as your crystal set, just amplified first.

When you build the transistor amplifier in Stage 5 and add it between the LC tank and the detector, you are replicating exactly the superheterodyne radio architecture: amplify the RF, then detect with a diode.

---

## Part 11 — Summary: The Complete Detector Circuit

For reference, here is the complete detector circuit with all values optimised:

```
From LC tank
(hot node, high Z)
     │
     │
    [D1] OA91 or 1N34A germanium
     │   (anode to tank, cathode to output)
     │
     ├──────────────── [C1 4.7nF NP0 ceramic] ──── GND
     │                 (RF bypass)
     │
     ├──────────────── [R1 100kΩ carbon film] ───── GND
     │                 (detector load / RF bypass)
     │
    [C_out 47nF] ─────────────────────────────── to earphone
                 (audio coupling — blocks DC, passes audio)
                                                 other earphone terminal → GND
```

**Component value summary:**

| Component | Value | Type | Purpose |
|-----------|-------|------|---------|
| D1 | OA91 or 1N34A | Germanium diode | Rectifier — envelope detection |
| C1 | 4.7 nF | NP0 ceramic or film | RF bypass filter (with R1) |
| R1 | 100 kΩ | Carbon film 0.25 W | Detector load — provides current path for detected signal |
| C_out | 47 nF | Ceramic or film | Audio coupling — blocks DC, passes audio signal |

---

## Part 12 — What to Record in Your Notebook

For the diode comparison experiments:

**For each diode tested, record:**
- Diode type and marking on body
- Forward voltage measured with multimeter diode mode (mV)
- Classification (silicon/germanium/Schottky) based on measured Vf
- Relative signal volume on each of 3 stations (1–5 scale)
- Any anomalies or observations

**For the galena detector:**
- Date, crystal description (source, size, colour, crystal faces visible)
- Number of spots tried before finding a sensitive one
- Estimated sensitivity compared to OA91 (better/equal/worse)
- Notes on physical setup (what helped, what didn't)
- Sketch of the cat's whisker holder design

**For the RC filter experiment:**
- Values of R and C used
- Calculated RC time constant
- Observed audio quality (4 descriptors: clear, muffled, buzzy, good)

---

## Part 13 — Troubleshooting Table

| Symptom | Likely Cause | Diagnosis | Fix |
|---------|-------------|-----------|-----|
| Diode seems not to work at all | Silicon diode installed | Check Vf with multimeter diode mode | Replace with germanium (OA91/1N34A) |
| Diode installed backwards | Anode and cathode reversed | Reverse the diode — try it both ways | Correct orientation: anode to tank, cathode to RC filter |
| Detection works but audio is harsh/buzzy | C1 too small — RF not filtered | Increase C1 | Increase C1 to 4.7 nF or 10 nF |
| Detection works but audio is muffled | C1 too large, or RC too large | Decrease C1 | Decrease C1 to 470 pF, or decrease R1 to 47 kΩ |
| Galena detector: no signal on any crystal spot | Cat's whisker not making contact | Very light pressure — can you hear a crackle when moving it? | Increase contact pressure slightly; clean crystal face with IPA |
| Galena detector: signal but very poor compared to germanium diode | Not finding best spot | Continue searching | Move cat's whisker systematically across the entire crystal face |
| Oscilloscope shows input but no output from diode | Signal too small for silicon — use germanium | Measure Vf | Use OA91 or lower-threshold diode |
| RC filter test: can't hear difference between RC values | Only trying on very strong stations | Try on a weaker, more distant station | Use a weak station where the threshold effect matters more |

---

## Part 14 — Key Insight: Connecting to Modern Radio Chips

The diode detector in a crystal set is the direct ancestor of every digital radio receiver:

**Crystal Set (1904–1940s):**
```
RF signal → LC tank → diode detector → audio
```

**Superheterodyne AM Radio (1920s–present):**
```
RF signal → RF amplifier → mixer → IF amplifier → diode detector → audio amplifier → speaker
```
Still using a diode detector — just amplified first.

**Modern SDR (Software Defined Radio):**
```
RF signal → RF amplifier → ADC (samples RF directly) → DSP (digital detection in software) → audio
```
The diode is replaced by a high-speed ADC and digital signal processing. The mathematical function performed by the DSP (Hilbert transform and envelope extraction) is exactly equivalent to what the diode and RC filter do physically.

**Your ESP32's WiFi/BLE chip:**
```
2.4 GHz RF → LNA → mixer → ADC → DSP → digital packet
```
The "detector" is the ADC — it samples the signal at hundreds of megasamples per second and the envelope detection (or in this case, I/Q demodulation of OFDM) is done in digital hardware on the chip.

The principle has not changed. The implementation has become digital. Understanding the crystal set detector means you understand what the silicon DSP block inside your ESP32 is mathematically computing on every packet it receives.

---

*End of Stage 4. Proceed to Stage 5: The First Amplifier — One Transistor Changes Everything.*
