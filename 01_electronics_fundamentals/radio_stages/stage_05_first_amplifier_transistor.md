# Stage 5 — The First Amplifier: One Transistor Changes Everything

---

## What You Will Learn

- Why the crystal set cannot drive a loudspeaker — the impedance mismatch explained quantitatively
- What amplification means physically — controlling a large energy source with a small signal
- How a transistor works from first principles — the NPN sandwich, carrier injection, and the β relationship
- The vacuum tube triode analogy — gate=base, cathode=emitter, plate=collector — and what this teaches you
- DC biasing — why the transistor needs a steady operating point, and what happens without one
- The four biasing methods — with voltage divider bias explained in full mathematical detail
- Complete design procedure for a common-emitter amplifier at 9V — step by step with all component values
- AC gain calculation and why the emitter resistor matters
- Two complete build experiments: the audio amplifier, and the full AM receiver with loudspeaker output
- How to cascade two amplifier stages
- Why high-gain amplifiers oscillate and how to prevent it
- A complete troubleshooting guide

**Prerequisites:** Stages 1–4. You should understand what a transistor is (base/emitter/collector), Ohm's law, and the basics of capacitors and resistors.

**Time required:** 3–4 hours reading + 4–6 hours building

**Cost:** ₹150–400 / $3–7 USD for all components

---

## Part 1 — Why the Crystal Set Cannot Drive a Speaker

### The Power Problem

From Stage 2: a typical crystal set receives somewhere between 10 nW and 1 µW of power from a distant transmitter. The crystal earphone requires only about 10 nW to produce audible sound — right at the edge of the received power.

A small 8Ω loudspeaker (the kind used in portable radios) requires at least 1 mW (1,000,000 nW) to produce audible sound at moderate volume. To produce loud, clear sound: 10–100 mW.

**The crystal set provides 10–1000 nW. The speaker needs 1,000,000–100,000,000 nW.**

The gap is 3 to 7 orders of magnitude. No passive circuit can close this gap — no transformer, no matching network, no clever arrangement of components can amplify power without an additional energy source. This is a thermodynamic law.

To drive a speaker, we need a power gain of at least 1,000 (30 dB). We need an amplifier.

### The Impedance Problem

But wait — even if we had the power, there is an impedance mismatch.

The crystal set detector output impedance is approximately Q × XL at the resonant frequency — from Part 4 of Stage 3, we calculated this to be around 150 kΩ for a typical circuit. 

The 8Ω speaker has an input impedance of 8Ω.

For maximum power transfer, the source impedance and load impedance must match. With 150 kΩ source and 8Ω load:

```
    Power transfer efficiency = 4 × R_load × R_source / (R_load + R_source)²

    = 4 × 8 × 150,000 / (8 + 150,000)²

    = 4,800,000 / (150,008)²

    = 4,800,000 / 2.25×10¹⁰

    = 2.13×10⁻⁴ = 0.021%
```

Only 0.021% of the available power reaches the speaker. The rest is wasted in the source impedance.

Even if a transformer could step down the impedance (it can, with N₁:N₂ = 137:1), the transformer has its own losses, and you are still limited by the 10 nW total power available.

**Conclusion:** We need amplification. We need a transistor.

---

## Part 2 — What Amplification Really Means

An amplifier does not create power from nothing. It takes power from a power supply (a battery, a mains adapter) and controls how much of that power is delivered to the load — using the small input signal as the control.

**The water tap analogy:**

Imagine a mains water pipe under high pressure (this is the power supply — the battery). The pipe has a tap (this is the transistor). The tap is controlled by a small lever (this is the input signal). A small force on the lever opens or closes the tap, controlling a large flow of water at high pressure. The small input force CONTROLS the large output power — it does not PROVIDE the output power.

The key is: the signal power needed to move the lever is much smaller than the water power flowing through the tap. The tap is an amplifier.

In the transistor:
- The small signal is applied to the base (the lever)
- The battery is the power supply (the mains water)
- The collector-emitter path (the tap) controls a large current from the battery
- The output power comes entirely from the battery, not from the input signal

**This is the central idea of amplification:**

```
    Output power = Input signal × Power from supply

    The transistor's job: let the input signal MODULATE the supply current.
```

---

## Part 3 — The Transistor from First Principles

### The NPN Sandwich

A transistor is made from three layers of semiconductor material:

```
    N ── P ── N
    │    │    │
Collector Base Emitter
    (C)  (B)  (E)
```

- N-type material: silicon doped with phosphorus or arsenic. Extra electrons are the majority carriers.
- P-type material: silicon doped with boron. Holes (missing electrons) are the majority carriers.

The NPN transistor has:
- Emitter (N): heavily doped N-type. Source of electrons.
- Base (P): very thin, lightly doped P-type. The control layer.
- Collector (N): moderately doped N-type. Collects electrons from the emitter via the base.

### What Happens in Operation

For an NPN transistor in active (amplifying) mode:

**Base-emitter junction (B-E): forward biased (V_BE ≈ 0.6V for silicon)**

When V_BE ≈ 0.6V (base positive relative to emitter), the B-E junction behaves like a forward-biased diode:
- Electrons flood from emitter into the base region
- This is "minority carrier injection" — electrons are minority carriers in the P-type base
- In the base, they are called "injected minority carriers"

**Base-collector junction (B-C): reverse biased (V_CB > 0)**

The collector is held positive relative to the base. This creates a strong electric field at the B-C junction, sweeping any minority carriers (electrons) in the base across into the collector.

**The key geometry: the base is very thin.**

Most of the injected electrons reach the B-C junction and are swept into the collector before they can recombine in the base. Typically 99%+ of the injected electrons end up as collector current.

Only the small fraction that recombine in the base need to be supplied as base current.

**The result:**

```
    IC = β × IB

Where:
  IC = collector current
  IB = base current
  β  = current gain (also called hFE)
       Typical values: 100–500 for BC547, 75–300 for 2N2222
```

Also:
```
    IE = IC + IB = (β+1) × IB ≈ IC   (since β >> 1)
```

A small base current controls a large collector current. For β = 200: a 10 µA base current controls a 2 mA collector current. The small input controls the large output. Amplification.

### The Physical Reason for β

β is high because:
1. The base is very thin (typically 0.5–5 µm in a small-signal transistor like BC547)
2. The base is lightly doped — few recombination sites
3. Most injected electrons traverse the thin base quickly, before finding a hole to recombine with

β is not a constant — it varies with collector current. At very low IC (< 1 µA), β drops because surface recombination becomes significant. At very high IC, β drops because of high-level injection effects. In the middle range (0.1 mA to 10 mA for a small-signal transistor), β is approximately constant.

### The Water Tap / Vacuum Tube Analogy

```
    Vacuum Tube Triode ↔ Transistor (NPN)
    ─────────────────────────────────────
    Grid voltage       ↔ Base current (IB)
    Cathode            ↔ Emitter (E)
    Plate (Anode)      ↔ Collector (C)
    Heater + cathode   ↔ Emitter (thermally generated carriers → injection)
    Vacuum between     ↔ Semiconductor base region
    Grid-cathode voltage ↔ Base-emitter voltage
    Plate current      ↔ Collector current
```

The key difference: in the vacuum tube, the grid controls current through a vacuum by electrostatic repulsion (voltage control — the grid needs essentially no current). In the transistor, the base controls current through a semiconductor by minority carrier injection (current control — base current is needed, but it is small compared to collector current).

Both are three-terminal devices where a small signal at one terminal (grid/base) controls a large signal at another terminal (plate/collector), with the third terminal (cathode/emitter) as the common reference.

---

## Part 4 — DC Biasing: Why the Transistor Needs a Bias Point

### The Problem Without Bias

Consider an NPN transistor with no DC bias — base connected to ground, emitter connected to ground, collector connected to supply via a resistor. The base-emitter voltage is 0V.

At V_BE = 0V: the transistor is OFF. IC = 0.

Now connect an AC input signal to the base via a coupling capacitor. The signal is a sine wave that swings between +10 mV and −10 mV.

- When the signal is positive: V_BE rises above 0. But it needs to reach 0.6V before any current flows. A +10 mV signal gives V_BE = 10 mV — still below threshold.
- When the signal is negative: V_BE goes negative. Still off.

Result: No output. The transistor never turns on because the signal is too small to reach threshold.

Now try a larger signal: ±0.8V swing.

- Positive half: V_BE rises to +0.8V. Base current flows. IC flows. Collector voltage drops.
- Negative half: V_BE drops to -0.8V. Transistor is off. No IC. Collector voltage returns to supply.

Result: Only the positive half-cycles produce output. The signal is half-wave rectified — severely distorted. This is called "cutoff clipping".

### The Solution: DC Bias

Apply a steady DC voltage to the base, holding V_BE at about 0.6–0.7V (just into conduction). This sets the "quiescent" or "Q-point" — the operating point with no signal applied.

With V_BE = 0.65V (mid-point of conduction):
- A +10 mV signal swings V_BE to 0.66V → IC increases slightly
- A −10 mV signal swings V_BE to 0.64V → IC decreases slightly
- The output (collector voltage) swings in proportion to the input signal
- Both positive and negative half-cycles produce proportional output
- This is linear amplification

The bias point must be chosen such that:
- IC has room to increase (collector voltage is not already at minimum)
- IC has room to decrease (transistor is not already cut off)
- The quiescent IC puts the collector voltage at approximately VCC/2 — this gives maximum symmetrical swing before clipping

### The Four Biasing Methods

**Method 1 — Fixed Bias (simplest, worst):**

```
    Vcc
     │
    [RB]
     │
     └── Base
     
     Vcc
      │
     [RC]
      │
      └── Collector
      
     Emitter ──── GND
```

A single resistor from VCC to base. VB = VCC − IB×RB. Simple but terrible — β varies from transistor to transistor and with temperature. Since IC = β × IB, and β might be anywhere from 100–500 for a given type, the operating point is completely unpredictable. A circuit that works with one BC547 might not work with another.

**Method 2 — Voltage Divider Bias (most stable, standard method):**

Two resistors form a voltage divider from VCC to GND. This divider sets VB at a stable voltage largely independent of β. An emitter resistor RE provides additional stability through negative feedback.

This is the method used in all practical audio amplifier stages. It is what you will build.

**Method 3 — Collector Feedback Bias:**

A resistor from collector to base. The collector voltage feeds back to set the base current. If IC rises (e.g. due to temperature), VCE falls, VB falls, IB falls, IC falls — self-correcting. Good stability, simpler than voltage divider, but gain is reduced due to the feedback.

**Method 4 — Emitter Bias (two-supply):**

Uses both a positive and negative supply rail. The emitter is biased with a negative supply. Provides excellent stability but requires two power supplies — inconvenient for battery-powered applications.

---

## Part 5 — Voltage Divider Bias: Full Design Procedure

### The Circuit

```
        Vcc (+9V)
         │
         ├───── [R1] ────────── Vcc (+9V)
         │                         │
        [R1]                      [R1]
         │                         │
         ├── Base (B)          Base (B)─── [Cin] ─── Input
         │
        [R2]
         │
        GND ─────────────────────────
         
         (simplified diagram showing the bias divider)
```

Full circuit:

```
        Vcc (+9V)
         │
       ──┴──
       │R1  │
       ──┬──
         │
         ├──────────────────[Cin]─── Signal input
         │  Base (B)
    ┌────┤
    │    │
  ──┴──  │NPN
  │R2  │ │ (BC547 or 2N2222)
  ──┬── │
    │   ├────────────────── Signal output ─── [Cout] ─── Load
    │   │  Collector (C)
    │   │
    │  [RC]
    │   │
    │  Vcc (+9V)
    │
    │  Emitter (E)
    │   │
    │  [RE]
    │   │
    └───┴─── GND
         │
        [Ce]
         │
        GND
```

Let me draw this more clearly:

```
          Vcc (+9V)
           │
           │
     ┌─────┴─────┐
     │           │
    [R1]        [RC]
     │           │
     │     ┌─────┘
     ├─────┤ B   C ├─── [Cout] ─── Output
     │     │ NPN   │
    [R2]   │       │ E
     │     └───────┘
     │           │
     │          [RE]
     │           │
     └───────────┴─── GND
                 │
                [Ce]
                 │
                GND
```

(Ce is the emitter bypass capacitor, in parallel with RE.)

And the coupling capacitors:

```
Input ─── [Cin] ─── Base (B)

[Cin]: blocks DC from the input source, passes AC signal
[Cout]: blocks DC from the output, passes AC signal to the load
[Ce]: short-circuits RE for AC signals, restoring full AC gain
```

### Step-by-Step Design Procedure

**Given:**
- Supply voltage: VCC = 9V
- Transistor: BC547, β = 200 (typical), V_BE = 0.65V

**Step 1: Choose the quiescent collector current IC.**

For an audio amplifier driving a crystal earphone or feeding a second stage: IC = 1 mA is a good choice.
- Low enough to be battery-efficient
- High enough for a reasonable transconductance (see Step 7)

**Step 2: Choose the collector-emitter voltage VCE.**

For maximum undistorted output swing, set VCE = VCC/2 = 9/2 = 4.5V.

**Step 3: Decide how much voltage to drop across RE.**

A voltage drop of 1/10 to 1/5 of VCC across RE provides good stability.

Choose V_RE = 1V (gives good thermal stability):
```
    RE = V_RE / IC = 1 / 0.001 = 1000 Ω = 1 kΩ
```
Use a standard value: RE = 1 kΩ.

**Step 4: Calculate RC.**

The total supply voltage must equal the sum of voltage drops around the collector-emitter loop:

```
    VCC = V_RC + VCE + V_RE

    V_RC = VCC − VCE − V_RE = 9 − 4.5 − 1 = 3.5V

    RC = V_RC / IC = 3.5 / 0.001 = 3500 Ω
```

Use the nearest standard value: RC = 3.3 kΩ or 3.9 kΩ. Let's use 3.9 kΩ.

Recalculate VCE with RC = 3.9 kΩ:
```
    V_RC = IC × RC = 0.001 × 3900 = 3.9V
    VCE = VCC − V_RC − V_RE = 9 − 3.9 − 1 = 4.1V
```

VCE = 4.1V. Close to VCC/2 — acceptable.

**Step 5: Calculate the base voltage VB.**

```
    VB = V_BE + V_RE = 0.65 + 1 = 1.65V
```

**Step 6: Calculate R1 and R2.**

The voltage divider rule:
```
    VB = VCC × R2 / (R1 + R2)

    1.65 = 9 × R2 / (R1 + R2)

    1.65 × (R1 + R2) = 9 × R2

    1.65 × R1 = 9 × R2 − 1.65 × R2 = 7.35 × R2

    R1/R2 = 7.35/1.65 = 4.45
```

To ensure the voltage divider current is large compared to IB (ensuring VB is stable regardless of β):
The divider current should be at least 10× IB.

```
    IB = IC / β = 0.001 / 200 = 5 µA

    Divider current target: 10 × IB = 50 µA

    R1 + R2 = VCC / I_divider = 9 / 0.000050 = 180,000 Ω = 180 kΩ
```

With R1/R2 = 4.45 and R1 + R2 = 180 kΩ:

```
    R2 = 180,000 / (1 + 4.45) = 180,000 / 5.45 = 33,028 Ω ≈ 33 kΩ (standard value)

    R1 = 180,000 − 33,000 = 147,000 Ω ≈ 150 kΩ (standard value)
```

**Verify VB with standard values:**
```
    VB = 9 × 33000 / (150000 + 33000) = 9 × 33000 / 183000 = 297000 / 183000 = 1.62V
```

1.62V is close to the desired 1.65V. Fine.

**Step 7: Calculate the AC voltage gain.**

For the common-emitter configuration with Ce bypassing RE:

```
    Av = −Rc_effective / re

Where:
    re = 26 mV / IC = 26 mV / 1 mA = 26 Ω
    (the intrinsic emitter resistance — a fundamental parameter)
    Rc_effective = RC || R_load (parallel combination of RC and any load)
```

If the load is a crystal earphone (very high impedance >> RC):
```
    Rc_effective ≈ RC = 3.9 kΩ

    |Av| = RC / re = 3900 / 26 = 150
```

Voltage gain of 150 (or 43.5 dB). A 1 mV input signal produces a 150 mV output signal.

The negative sign indicates the output is inverted (180° phase shift) relative to the input — this is normal for common-emitter amplifiers and does not affect audio quality.

**Step 8: Calculate capacitor values.**

**Cin (input coupling capacitor):**
Must present low impedance at audio frequencies (> 300 Hz) relative to the input resistance of the amplifier stage.

Input resistance of the amplifier: R1 || R2 || (β × re + RE) ≈ 150kΩ || 33kΩ || (200×26) = 150kΩ || 33kΩ || 5200 Ω ≈ 4.7 kΩ

At lowest audio frequency (300 Hz), XCin must be << 4.7 kΩ. Set XCin = 470 Ω at 300 Hz:
```
    Cin = 1 / (2π × 300 × 470) = 1 / 886,000 = 1.13 µF

    Use 2.2 µF or 4.7 µF electrolytic (polarity: + to the higher voltage side — the base)
```

**Cout (output coupling capacitor):**
Must present low impedance at audio frequencies relative to the load impedance.

For a crystal earphone (160 kΩ at 1 kHz): set XCout = 16 kΩ at 300 Hz:
```
    Cout = 1 / (2π × 300 × 16,000) = 1 / (30,000,000) = 33 nF
    Use 47 nF or 100 nF ceramic
```

For an 8Ω speaker (via a subsequent amplifier stage): set XCout << 8Ω at 300 Hz:
```
    Cout = 1 / (2π × 300 × 0.8) = 1 / 1508 = 663 µF
    Use 1000 µF electrolytic
```

**Ce (emitter bypass capacitor):**
Must present low impedance at audio frequencies relative to RE (1 kΩ).

At lowest audio frequency (300 Hz), XCe must be << RE = 1 kΩ. Set XCe = 100 Ω at 300 Hz:
```
    Ce = 1 / (2π × 300 × 100) = 1 / 188,400 = 5.3 µF
    Use 10 µF or 47 µF electrolytic (polarity: + to emitter side, which is positive relative to ground)
```

---

## Part 6 — Complete Component List

**For the Common-Emitter Audio Amplifier on a 9V Battery:**

| Component | Value | Type | Purpose |
|-----------|-------|------|---------|
| Q1 | BC547 or 2N2222 | NPN transistor | The amplifying element |
| R1 | 150 kΩ | Carbon film 0.25W | Upper bias divider resistor |
| R2 | 33 kΩ | Carbon film 0.25W | Lower bias divider resistor |
| RC | 3.9 kΩ | Carbon film 0.25W | Collector load resistor |
| RE | 1 kΩ | Carbon film 0.25W | Emitter stability resistor |
| Cin | 4.7 µF | Electrolytic, 16V+ | Input coupling |
| Cout | 47 nF | Ceramic film | Output coupling (earphone) |
| Ce | 47 µF | Electrolytic, 16V+ | Emitter bypass |
| Cdecoupling | 100 nF | Ceramic (NP0 or X7R) | Power supply decoupling |
| Battery | 9V | PP3 / 6F22 | Power supply |
| Battery clip | — | — | Connection to battery |

**BC547 pin identification (viewed from flat face, leads down):**

```
    ┌────────────────┐
    │  FLAT face     │
    └──┬──┬──┬───────┘
       │  │  │
       C  B  E
```

C = Collector (left)
B = Base (middle)
E = Emitter (right)

**2N2222 pin identification (metal can, viewed from bottom):**

```
    Bottom view:
    
         ┌───────┐
        /         \
       │  E  B  C  │
        \         /
         └───────┘
           (tab = Collector on metal can version)
```

Always verify the pinout on the datasheet for your specific package — plastic (TO-92) and metal (TO-18) packages have different pin arrangements for 2N2222.

---

## Part 7 — Complete Circuit Schematic

```
                        Vcc (+9V)
                         │
       ┌─────────────────┴──────────────────────────────────────┐
       │                                                        │
     [R1 150kΩ]                                              [RC 3.9kΩ]
       │                                                        │
       │                                 ┌──────────────────────┤
       ├─────── [Cin 4.7µF] ─── Input   │ Collector            │
       │        (+pole to base)          │                   [Cout 47nF]
       │ VB=1.6V                     ┌───┤ BC547/2N2222          │
     [R2 33kΩ]                  Base │   │                    Output ─── [Earphone]
       │                         IB  │   │ Emitter               │
       │                             └───┤                      GND
       │                                 │
      GND                             [RE 1kΩ]
                                         │
                                 ┌───────┤
                                [Ce 47µF]│
                                 (+ to   │
                                 emitter)│
                                         │
                                        GND

Power supply decoupling:
  Between Vcc line and GND, place [Cdecoupling 100nF] as close to the transistor as possible.
```

---

## Part 8 — Gain Calculation

### The re Model

At the operating point IC = 1 mA, the transistor's intrinsic emitter resistance re is:

```
    re = VT / IC = 26 mV / 1 mA = 26 Ω

Where:
  VT = thermal voltage ≈ 26 mV at room temperature
  IC = quiescent collector current
```

re is not a resistor you place in the circuit — it is the effective resistance of the forward-biased base-emitter junction.

### AC Gain (with Ce installed)

When Ce is installed (bypassing RE for AC signals):

```
    Av = − RC / re    (approximately, when RL >> RC)

    Av = − 3900 / 26 = −150
```

Magnitude: 150. This means a 1 mV input produces 150 mV output (inverted).

In decibels:
```
    Av(dB) = 20 × log₁₀(|Av|) = 20 × log₁₀(150) = 20 × 2.176 = 43.5 dB
```

### AC Gain (without Ce)

If Ce is removed, RE is in the gain formula:

```
    Av = − RC / (re + RE) = − 3900 / (26 + 1000) = − 3900 / 1026 = − 3.8
```

Magnitude: 3.8 — very low gain, but very stable (gain is set by RE, not β, so it doesn't depend on the specific transistor's β).

This is a key design trade-off:
- With Ce: high gain, but gain depends on β and re (which vary with temperature and transistor-to-transistor)
- Without Ce: low, stable gain, but much more predictable and less prone to distortion

For a signal stage driving a crystal earphone where gain is critical, include Ce.

### Worked Example: Complete Power Calculation

Input signal: 5 mV peak from the crystal set detector
Stage gain: 150

Output signal: 5 mV × 150 = 750 mV peak

Output power into crystal earphone (160 kΩ impedance):
```
    P = V² / R = (0.75/√2)² / 160,000 = (0.530)² / 160,000 = 0.281 / 160,000 = 1.76 µW
```

Compare to crystal set output power (estimated 100 nW for a good setup):
```
    Power gain = 1,760 nW / 100 nW = 17.6 times = 12.4 dB
```

With 1.76 µW into the earphone, the audio should be clearly audible and comfortable.

---

## Part 9 — Build Experiment 1: Common-Emitter Audio Amplifier

**Goal:** Build the complete common-emitter amplifier on a breadboard. Use an electret microphone as input. Verify amplification by measuring input and output voltages.

**Time: 2–3 hours**

### Materials

- 1× BC547 or 2N2222 NPN transistor
- R1 = 150 kΩ, R2 = 33 kΩ, RC = 3.9 kΩ, RE = 1 kΩ (carbon film 0.25W each)
- Cin = 4.7 µF electrolytic, 16V minimum
- Ce = 47 µF electrolytic, 16V minimum
- Cout = 47 nF ceramic
- Cdecoupling = 100 nF ceramic
- 1× electret microphone capsule (small, 2-pin, widely available — ₹15–30)
- 1× crystal/ceramic earphone or another BC547 for a second stage (for speaker output)
- 9V battery and battery clip
- Breadboard and jumper wires
- Multimeter

### The Electret Microphone Bias Circuit

An electret microphone capsule contains a FET preamplifier inside the capsule. It requires a DC supply voltage (typically 1–10V) through a bias resistor:

```
    Vcc (+9V) ──── [R_bias 10kΩ] ──── Microphone+ ──── [Cmicrophone] ──── Amplifier Input
                                       Microphone−
                                            │
                                           GND
```

The audio signal appears across a 10 kΩ bias resistor (or at the microphone output pin). Use a 10 µF coupling capacitor after the microphone to block the DC bias and pass the audio signal to the amplifier input.

**Microphone pin identification:**
Most electret capsules have two pins. One is connected to the case (this is the GND/negative terminal). The other is the output. Check with a multimeter in resistance mode: the case pin will show a different resistance to the other pin in each direction (due to the internal FET junction).

### Build Steps

1. Place the BC547 on the breadboard. Identify C, B, E (flat face facing you: left=C, middle=B, right=E for TO-92 package). Verify with continuity tester — between B and E there should be a diode junction (~0.6V in one direction).

2. Connect R1 from Vcc rail to the base pin. Connect R2 from the base pin to GND rail.

3. Connect RC from Vcc rail to the collector pin.

4. Connect RE from the emitter pin to GND rail.

5. Connect Ce from the emitter pin to GND rail (in parallel with RE). Observe polarity: + to emitter.

6. Connect Cin from the base to the signal input point. Observe polarity: + to the base side.

7. Connect Cout from the collector to the output point.

8. Add Cdecoupling (100nF ceramic, no polarity) directly across the Vcc and GND rails, as close to the transistor as possible.

9. Connect the microphone bias circuit to the Vcc rail and connect the microphone output through a 10µF capacitor to the Cin input.

10. Connect the crystal earphone from the Cout output to GND.

11. Connect the 9V battery.

### Static Bias Check (Before Connecting Signal)

Before connecting any signal, verify the DC bias point:

1. Set multimeter to DC voltage, 10V range.
2. Measure VB (base to GND): should be approximately 1.6V.
3. Measure VE (emitter to GND): should be approximately 0.95V (= VB − VBE = 1.6 − 0.65).
4. Measure VC (collector to GND): should be approximately 5V (= VCC − IC×RC = 9 − 0.001×3900 = 9 − 3.9 = 5.1V).
5. Measure VCE (collector to emitter) = VC − VE = 5.1 − 0.95 = 4.15V.

If the measurements are close (±0.5V) to these values, the bias is correct. If not:

| VB is wrong | Check R1 and R2 values and connections |
| VE is wrong | Check RE connection; verify VBE = 0.6V (if VBE is very high or low, transistor may be faulty) |
| VC is wrong | Check RC; if VC is near 0, transistor is in saturation (IC too high — wrong R values); if VC = Vcc, transistor is cut off (IC = 0 — check bias) |

### Gain Measurement

With the microphone connected and the circuit biased correctly:

1. Speak into the microphone normally at about 30 cm distance.
2. Listen on the earphone. You should hear your voice clearly, somewhat amplified compared to the direct sound.
3. To measure gain:
   - Set multimeter to AC voltage, most sensitive range (200 mV or 2000 mV AC).
   - Measure AC voltage at the base (before Cin, at the microphone capsule output). This is Vin.
   - Measure AC voltage at the collector (at the junction of RC and the collector pin). This is Vout.
   - Note: the multimeter measures RMS voltage, not peak.
   - Gain = Vout/Vin.

Expected values (speaking clearly at 30 cm):
- Microphone output: 1–5 mV RMS
- Amplifier output: 150–750 mV RMS
- Measured gain: 100–200 (depending on transistor β and exact component values)

**Record your measurements:**

| Measurement | Value |
|-------------|-------|
| VB (DC) | |
| VE (DC) | |
| VC (DC) | |
| VCE (DC) | |
| IC (calculated: (Vcc−VC)/RC) | |
| re (calculated: 26mV/IC) | |
| Vin AC (microphone output) | |
| Vout AC (collector) | |
| Measured gain (Vout/Vin) | |
| Calculated gain (RC/re) | |
| Difference | |

---

## Part 10 — Build Experiment 2: Crystal Set + Amplifier = AM Radio with Speaker Output

**Goal:** Connect the transistor amplifier after the crystal set detector to drive an 8Ω loudspeaker. This is a complete battery-powered AM receiver.

**Time: 2–3 hours (assuming crystal set is already built)**

### Circuit Modification

The crystal set crystal set output (after the germanium diode and RC filter, before the crystal earphone) connects to the input of the transistor amplifier. The output of the transistor amplifier connects to a speaker.

To drive an 8Ω speaker directly from a single common-emitter stage, you need a transformer or a second stage — the 8Ω speaker would load the output heavily. The simplest approach:

**Option A: Keep the crystal earphone at the stage 1 output, and add a second stage to drive the speaker.**

**Option B: Add an audio output transformer** (primary 2–10 kΩ, secondary 8Ω — widely available, these are standard audio output transformers sold for transistor radios, cost ₹30–100 in India).

**Option C: Add a second BC547 stage in common-emitter configuration, then connect an 8Ω speaker with a large coupling capacitor (1000µF).**

For simplicity, here is Option C — the two-stage amplifier:

### Two-Stage Amplifier Circuit

Stage 1: Collector of first BC547 → Cin2 (coupling cap) → Base of second BC547.

**Second stage component values (optimised for speaker output):**

- R1_2 = 47 kΩ (upper bias)
- R2_2 = 10 kΩ (lower bias)
- RC2 = 470 Ω (lower value to drive more current into speaker)
- RE2 = 100 Ω
- Ce2 = 100 µF
- Cin2 = 10 µF
- Cout2 = 1000 µF (large — needed for low-impedance speaker)

**Complete signal path:**

```
Antenna → LC Tank (coil + variable cap) → Germanium diode detector → 
RC filter (100kΩ + 4.7nF) → [Cin1 4.7µF] → Stage 1 Base →
Stage 1 Collector → [Cin2 10µF] → Stage 2 Base →
Stage 2 Collector → [Cout2 1000µF] → 8Ω Speaker → GND
```

**Expected total gain:** Stage 1 gain × Stage 2 gain.

Stage 1 (IC=1mA, RC=3.9kΩ): Av1 ≈ RC/(re) but the second stage loads stage 1. The load on stage 1 output is (R1_2 || R2_2 || β×re2).

For Stage 2: IC2 = VB2/RE2 approximately. Let's say IC2 = 2 mA (more current for speaker drive), then re2 = 26mV/2mA = 13Ω.

Input impedance of Stage 2 = 47k || 10k || (200×13) = 47k || 10k || 2600Ω ≈ 2.1 kΩ.

Stage 1 effective load = RC1 || Rin_stage2 = 3900 || 2100 ≈ 1360 Ω.
Stage 1 gain with loading = 1360 / 26 = 52.

Stage 2 effective load = RC2 || Rsp = 470 || 8 ≈ 7.87 Ω.
Stage 2 gain = 7.87 / 13 = 0.6 — less than 1? This seems wrong.

**Wait** — at 2 mA, re2 = 13Ω. With RC2 = 470Ω and speaker 8Ω in series with a coupling cap, the AC load on stage 2 is RC2 || Rspeaker = 470||8 ≈ 7.87Ω. Gain = 7.87/13 = 0.6.

This is voltage gain less than 1 for Stage 2. But the power gain is still positive:

```
Output current to speaker = Vout / 8Ω
Input current to Stage 2 = Vin / Rin_stage2 = Vin / 2100

Power gain = (Vout²/8) / (Vin²/2100) = (Av²) × (2100/8) = 0.6² × 262.5 = 94
```

Power gain of 94 (nearly 20 dB) even with voltage gain < 1. The stage delivers significant current to the speaker. This is a power amplifier stage — designed for current drive, not voltage gain.

**For cleaner design:** Use a dedicated power transistor (TIP31 or 2N3055) in an emitter follower (common collector) configuration for the output stage. But for a beginner project, two BC547 stages as described will work — expect low but audible volume from the speaker.

### Testing the Complete Receiver

1. Build both stages on the breadboard.
2. Connect the crystal set to Stage 1 input.
3. Connect the 8Ω speaker to Stage 2 output via the 1000µF coupling capacitor.
4. Tune the crystal set LC tank to a local AM station.
5. You should hear the station from the speaker.

**What to compare:** Note the signal level that was barely audible on the crystal earphone before. With the two-stage amplifier, the same signal should now drive the speaker at an audible volume — even across a quiet room.

---

## Part 11 — Stability, Oscillation, and Decoupling

### Why High-Gain Amplifiers Oscillate

A single-stage amplifier with gain of 150 can be unstable. Here is why:

Imagine a small noise signal at the input. The amplifier amplifies it by 150. If any of this amplified output signal finds its way back to the input — through the power supply wiring, through stray capacitance between input and output wires, through the circuit board — and if it arrives in phase, it reinforces the original noise. The noise grows on each pass. This is positive feedback. The result is oscillation — the circuit produces a steady tone (usually a high-pitched squeal) even with no input signal.

**The most common path for feedback in a multi-stage amplifier:**

The power supply wires. When Stage 2 draws a large collector current, the battery internal resistance causes the supply voltage to dip slightly. This dip appears at the Stage 1 collector via the shared Vcc wire. This can feed back to Stage 1's base and produce oscillation.

### The Fix: Decoupling Capacitors

Place a decoupling capacitor directly across the power supply pins, as close to each transistor as possible:

```
    Vcc ────┬──── [R_decouple 100Ω] ──── Vcc for Stage 1
            │
          [C_decouple 100µF]
            │
           GND
```

The capacitor acts as a local reservoir. When Stage 2 suddenly demands current, the capacitor supplies it from its local charge, preventing the Vcc rail from dipping. The large value (47µF–100µF) handles low-frequency (audio) current demands.

For RF oscillation suppression, add a small (100 nF) ceramic capacitor in parallel with the large electrolytic — the small cap handles high-frequency transients that the electrolytic is too slow to respond to.

**Minimum decoupling:**
- 100 µF electrolytic across the battery connector
- 100 nF ceramic across each transistor's collector to GND, as close to the collector pin as possible

### Layout Rules to Prevent Oscillation

1. Input and output wires/traces should not run parallel to each other for any significant length.
2. The ground return should be a low-impedance path. On stripboard: use a continuous ground rail along one edge.
3. Coupling capacitors (Cin, Cout) should be as close as possible to the transistor pins they connect to.
4. Keep all signal paths as short as possible.

If your amplifier oscillates (squeals without input signal):
1. First try: add a 100 nF ceramic cap directly across the collector to GND.
2. Second try: add a 100Ω resistor in series with the collector, between the junction of RC and the transistor collector pin.
3. Third try: re-layout the circuit with better separation between input and output.

---

## Part 12 — Introduction to the RF Amplifier

The common-emitter amplifier you built is an audio amplifier — designed for frequencies up to 10–20 kHz.

The same transistor (BC547 or 2N2222) can be used at radio frequencies (1 MHz for AM, 7 MHz for HF, up to about 500 MHz for the 2N2222 — its ft is 300 MHz).

A radio frequency amplifier (RF amplifier) differs from an audio amplifier in:

**1. Lower signal frequencies → smaller reactive components:**
At 1 MHz, inductances in the circuit are measured in microhenries (µH) not millihenries. Capacitors are picofarads (pF) not microfarads (µF).

**2. Matching networks instead of resistive loads:**
At RF, the collector load is an LC tank tuned to the signal frequency. The high impedance at resonance gives high gain at the tuned frequency and low gain off-tune — selectivity and gain in one component.

**3. Different bias current:**
For maximum power in an RF amplifier, the bias is often set for IC = 5–20 mA (higher than audio) to ensure sufficient transconductance at RF.

**4. Transmission line effects:**
At 100 MHz and above, the length of wires becomes comparable to the signal wavelength and must be treated as transmission lines (not just wires). At 1 MHz, wires shorter than 30 cm are fine.

**A teaser for the next stages:**

The crystal set + detector + audio amplifier combination you have built is a "TRF" (tuned radio frequency) receiver — an improvement over the crystal set. The next evolution is the "superheterodyne" (superhet) receiver, invented by Edwin Armstrong in 1918:

```
Crystal set:
  Antenna → LC tank → Detector → Audio amp → Speaker

TRF receiver:
  Antenna → RF amp → LC tank → Detector → Audio amp → Speaker

Superheterodyne receiver (every modern radio):
  Antenna → RF amp → Mixer (multiply signal with local oscillator) →
  IF filter at fixed frequency → IF amplifier → Detector → Audio amp → Speaker
```

The superhet shifts the incoming signal (at whatever frequency you are tuned to) down to a fixed intermediate frequency (IF — typically 455 kHz for AM, 10.7 MHz for FM). All the selectivity and gain is at this fixed IF, which can be optimised once and never changed. This is why modern radios can receive signals across a wide frequency range with consistent performance.

Your ESP32's radio uses a similar downconversion principle: the 2.4 GHz signal is mixed down to a baseband frequency (near DC), sampled by an ADC, and processed digitally.

---

## Part 13 — What to Record in Your Notebook

**DC Bias Measurements:**

| Component | Value used | Designed value | Measured VB | Measured VE | Measured VC | Measured IC |
|-----------|-----------|---------------|-------------|-------------|-------------|-------------|
| Stage 1 | R1=150k, R2=33k, RC=3.9k, RE=1k | 1.6V, 0.95V, 5.1V, 1mA | | | | |

**Gain Measurements:**

| Signal source | Input voltage (mV AC) | Output voltage (mV AC) | Measured gain | Calculated gain |
|--------------|----------------------|----------------------|---------------|-----------------|
| Microphone (voice) | | | | |
| Crystal set output (radio) | | | | |

**Speaker Output Test (Two-Stage):**

| Station | With crystal earphone only | With two-stage + speaker | Improvement |
|---------|--------------------------|-------------------------|-------------|

**Oscillation test:**
- Does the circuit oscillate (squeal) without input? Y/N
- If yes: what frequency (can you estimate from the pitch)?
- Fix applied (decoupling caps) and result

---

## Part 14 — Complete Troubleshooting Guide

| Symptom | Likely Cause | Test | Fix |
|---------|-------------|------|-----|
| No output at all, faint or nothing | Transistor not biased — VCE ≈ VCC | Measure VC: if ≈ VCC, transistor is cut off | Check R1, R2 values and connections; check VBE = 0.65V |
| Output severely clipped on one half | Transistor overdriven or wrong Q-point | Measure VCE: if < 1V, transistor is saturated | Reduce RC or increase R2 to lower IC |
| High-pitched squeal with no input | Oscillation due to feedback | Remove input — does squeal stop? | Add 100nF across collector-to-GND; improve layout |
| Very low gain (much less than 150) | Ce not connected or open-circuit | Check Ce in-circuit (should be low resistance to AC) | Replace Ce; check solder joints |
| Gain too high — output clips easily | Emitter bypass Ce is too large (not a problem) or transistor β is very high | Measure gain at small input | Add small resistor (100Ω) in series with emitter, in series with Ce, to reduce gain |
| Microphone produces no sound | Microphone not biased correctly | Measure DC at microphone output pin: should be 1–5V | Check 10kΩ bias resistor; check microphone capsule is not shorted |
| Loud hum on output | Power supply not adequately decoupled | Check for 50Hz/60Hz hum | Add 100µF electrolytic across battery terminals |
| Output signal inverted (oscilloscope) | Normal for common-emitter | Check oscilloscope polarity | This is normal — does not affect audio quality |
| Transistor hot to touch | IC too high — saturation or wrong bias | Measure VCE: if near 0, transistor is saturated and dissipating all supply power | Check RC value; check R1/R2 bias. Stop and fix before continued use. |
| VBE reads 0V (multimeter) | Base-emitter short inside transistor | Replace transistor | Use a new BC547/2N2222 |
| VBE reads > 1V | Base-emitter open inside transistor | Replace transistor | Use a new BC547/2N2222 |

---

## Part 15 — Key Insight: Connecting to Modern Radio Chips

You have now built every block of a real radio receiver — from the antenna all the way to the speaker. The connection to your ESP32:

**The BC547 common-emitter amplifier you built** is a single transistor performing the same function as the "LNA" (Low Noise Amplifier) at the input of the ESP32's radio front end. The LNA inside the ESP32 is built from RF transistors in a 90nm CMOS process — billions of times smaller, but doing exactly the same thing: amplifying a weak signal without adding too much noise.

**The bias circuit (R1, R2, RC, RE)** has an exact counterpart in the ESP32's LNA: the transistors in the LNA are biased by on-chip bias circuits that use the same voltage divider principle, implemented with transistors acting as precision resistors (a "bandgap reference" and current mirror, but the underlying purpose is identical to your R1/R2 divider).

**Cascading two stages** is exactly what every RF signal chain does. Your ESP32 chip has: LNA → filter → mixer → IF amplifier → ADC. Each stage amplifies or processes the signal, with coupling capacitors (or inductors) between stages, and decoupling capacitors everywhere on the supply rail.

**The layout rules** (keep input and output separated, use decoupling caps) are the same rules that RF PCB designers follow when laying out circuits at 2.4 GHz — at higher frequencies they are even more critical, but the principle is identical.

When a drone loses radio control — the RC link fails — it is almost always one of these: noise coupling into the LNA input, oscillation due to poor PCB layout, or insufficient supply decoupling causing the LNA to lose gain when the power amplifier transmits. Understanding the amplifier you built in this stage puts you in a position to diagnose and fix these problems.

---

## Part 16 — Where to Go Next

You have now worked through the complete signal chain of a radio receiver from first principles:

1. **Stage 1:** The spark — how electromagnetic waves are produced
2. **Stage 2:** The crystal set — passive reception from the LC tank
3. **Stage 3:** LC resonance — deep mathematical understanding
4. **Stage 4:** The detector — recovering audio from the RF signal
5. **Stage 5:** The amplifier — driving a speaker with a transistor

The natural next stages in a complete study of radio are:

**Stage 6 — The Regenerative Receiver:** Add feedback to the crystal set coil to make the transistor oscillate very gently, right at the threshold. At this threshold, the effective Q of the LC tank increases dramatically (by a factor of 10–100), and the sensitivity and selectivity both improve enormously. This is the most sensitive simple receiver circuit ever invented. Armstrong built the first one in 1912 while a student at Columbia University.

**Stage 7 — The Superheterodyne:** Learn about mixers, local oscillators, intermediate frequency (IF) stages, and the architecture that makes all modern radios work. Build a simple superhet for AM.

**Stage 8 — FM Demodulation:** FM radio uses frequency modulation instead of amplitude modulation. The detector is completely different (a discriminator or ratio detector). Understanding this covers the FM radio in your phone.

**Stage 9 — Digital Modulation:** BPSK, QPSK, QAM — how bits are encoded onto a carrier. This is the basis of WiFi, Bluetooth, and your ESP32's radio.

**Stage 10 — The ESP32 Radio Layer:** Armed with this knowledge, read the ESP32's 2.4 GHz radio documentation, understand the PHY layer of IEEE 802.11b/g/n, and understand how your drone's RC link works at the physical level.

---

*End of Stage 5. You now have a complete understanding of radio from the spark gap to the transistor amplifier. Every modern radio chip builds on these exact principles.*
