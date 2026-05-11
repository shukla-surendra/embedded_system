# Stage 9 — The Complete Superheterodyne: Full AM Radio from Scratch

---

## What You Will Learn

- How all seven functional blocks of an AM receiver connect and depend on each other
- Why each stage exists and what fails if you remove it
- Exact component values and bias calculations for every transistor stage
- How to build a complete medium-wave AM radio using only discrete components
- Alignment procedure: how to tune up each stage for maximum performance
- What performance to expect and how to measure it
- How AGC (Automatic Gain Control) works and how to implement a basic version
- Common problems and systematic troubleshooting

---

## 1. The Complete Block Diagram

```
 [ANTENNA]
     |
     v
 [RF AMPLIFIER] --- 20dB gain at f_station, tuned tank
     |
     v
 [MIXER] + [LOCAL OSCILLATOR] --- translate f_station to 455kHz
     |
     v
 [IF FILTER] --- CFW455 ceramic + IF transformers, 455kHz, ~10kHz BW
     |
     v
 [IF AMPLIFIER] --- two stages, 40dB total gain, with AGC
     |
     v
 [DETECTOR] --- OA91 diode envelope detector
     |           audio output + AGC voltage
     v
 [AUDIO AMPLIFIER] --- two BC547 stages, drives earphone or small speaker
     |
     v
 [EARPHONE / SPEAKER]
```

Total signal path: weak RF at antenna → audio at speaker.

Approximate gain budget:
- Antenna to RF amp input: +3 dB (antenna gain, depending on length)
- RF amplifier: +20 dB
- Mixer (conversion gain): +10 dB
- IF filter (insertion loss): -3 dB
- IF amplifier (2 stages): +40 dB
- Detector: -6 dB (conversion loss, approximate)
- Audio amplifier: +30 dB
- Total: approximately +94 dB from antenna to speaker

A minimum detectable signal of 1 microvolt at the antenna delivers approximately:
- After 94 dB gain: 1μV × 10^(94/20) = 1μV × 50,000 = 50 mV RMS audio
- Into a 32Ω earphone: P = (50mV)²/32 = 78 μW — clearly audible

For comparison, the crystal set needed about 1 mV at the antenna for audible output. The superheterodyne is 1000 times more sensitive.

---

## 2. Why Each Stage Exists

**Remove the RF amplifier:** The mixer alone has enough conversion gain to work. But without RF preamplification, the noise figure degrades. Weak stations disappear into the noise. Image rejection also worsens because the pre-mixer filtering is less effective. Keep the RF amp.

**Remove the mixer/LO:** You now have a TRF (tuned radio frequency) receiver. Must tune every stage for each station. All the instability problems described in Stage 8. Very hard to build well. The mixer is the key innovation.

**Remove the IF filter:** The IF amplifier amplifies all frequencies passing through the mixer. Adjacent stations on the MW band (separated by 10 kHz) would all be amplified equally. No selectivity. Every station audible simultaneously. The IF filter is essential for selectivity.

**Remove the IF amplifier:** Only the mixer's conversion gain (about +10 dB) and the RF amp (+20 dB) are left. Total of about +30 dB before the detector. Weak signals below about 100 μV at the antenna would be undetectable. The IF amplifier is essential for sensitivity.

**Remove the AGC:** Strong local stations overload the IF amplifier causing loud distortion. Weak distant stations are barely audible. AGC automatically reduces gain for strong stations, maintaining comfortable listening volume regardless of signal strength.

**Remove the detector:** You have an amplified 455 kHz carrier but no audio extraction. Nothing reaches the speaker. The detector is the essential stage that extracts audio.

**Remove the audio amplifier:** The detector output (a few millivolts) is not enough to drive a speaker. Could drive a crystal earphone directly. Adding the audio amp gives speaker-level output.

---

## 3. Stage-by-Stage Build Guide

Build each stage separately, test it, then connect stages one at a time. Do not connect everything at once — it will be impossible to debug.

---

## 3.1 Stage A: RF Amplifier

### Function
Amplifies the received RF signal (530–1600 kHz) before the mixer. Provides ~20 dB gain. Includes the first tuned tank circuit for initial selectivity and image rejection.

### Schematic

```
ANTENNA (1-metre wire or ferrite rod antenna)
     |
[C_ant = 100pF]  <- antenna coupling cap, reduces antenna's effect on tuning
     |
[L_RF: ferrite rod antenna coil, 60 turns on 10mm rod]
     |
[VC1: variable capacitor, 10-400pF]  <- tuning capacitor for RF stage
     |
    GND

The top of L_RF (above VC1) connects to:
[C_in = 10nF] ---> [Base of Q1 (BC547)]

+9V
 |
[R1 = 100kΩ]
 |
[Base Q1]
 |
[R2 = 22kΩ]
 |
GND

+9V
 |
[RC1 = 4.7kΩ]
 |
[Collector Q1] ---[C_out = 47pF]---> [Mixer base]
                     (small coupling cap for impedance matching to mixer)
                     
[Emitter Q1] ---[RE1 = 470Ω]--- GND
[Emitter Q1] ---[CE1 = 100nF ceramic]--- GND  (RF bypass)

Supply decoupling for this stage:
+9V ---[C1 = 100μF]--- GND
+9V ---[C2 = 100nF ceramic]--- GND
```

### Bias Calculation

Target I_C = 1 mA, V_C = 4.5V on 9V supply:
```
RC1 = (9 - 4.5) / 1mA = 4.5kΩ → use 4.7kΩ
V_RE = 1mA × 470 = 0.47V
V_B = V_RE + V_BE = 0.47 + 0.65 = 1.12V

R1/(R1+R2) divider: 1.12/9 = 0.124
If R2 = 22kΩ: R1 = 22000 × (9 - 1.12)/1.12 = 22000 × 7.04 = 154.8kΩ → use 150kΩ

Verify: V_B = 9 × 22/(150+22) = 9 × 22/172 = 1.15V ✓
I_C = (1.15 - 0.65)/470 = 0.5/470 = 1.06 mA ✓
V_C = 9 - 1.06mA × 4.7kΩ = 9 - 4.98 = 4.02V ✓ (close to target)
```

Use R1 = 150kΩ, R2 = 22kΩ, RC1 = 4.7kΩ, RE1 = 470Ω.

### Voltage Gain

At 1 MHz with RC = 4.7kΩ and I_C = 1 mA:
```
gm = I_C / V_T = 1mA / 26mV = 38.5 mS
|A_v| = gm × RC = 38.5mS × 4.7kΩ = 181 ≈ +45 dB
```

In practice, parasitic capacitances and the 47pF coupling cap to the mixer reduce this to about 20–25 dB. That is acceptable.

### Test (Stand-alone)

Before connecting to mixer:
1. Apply 9V. Measure V_C: should be 4–5V.
2. Connect a 1-metre antenna wire.
3. Touch the collector through 47pF cap to a crystal earphone (earphone directly, no detector).
4. You will not hear AM audio this way, but you will hear a change in noise level when you tune the tank circuit to a station frequency (the noise level changes as you pass through a station's carrier).

Better test: connect a diode detector (OA91 + 10nF) between the collector and earphone. Tune VC1. You should hear MW stations — this is essentially a TRF set and proves the RF amp is working.

---

## 3.2 Stage B: Local Oscillator

### Function
Generates a continuous signal at f_station + 455 kHz. Must track with VC1 — as you tune to different stations, the LO frequency must stay 455 kHz above the signal frequency.

### Schematic (from Stage 7, adapted)

```
+9V
 |
[R_LO1 = 100kΩ]
 |
[Base Q2 (BC547)]
 |
[R_LO2 = 10kΩ]
 |
GND

+9V
 |
[RFC = 100μH]  <- keeps RF off supply
 |
[Collector Q2] ---[C_LO_out = 10pF]---> [Mixer: feeds to base of mixer via 100pF]
 
[Emitter Q2] ---[RE_LO = 100Ω]--- GND
[Emitter Q2] ---[CE_LO = 10nF ceramic]--- GND

Tank circuit:
[L_LO: 25μH air core coil] between collector and top of tank
[C_LO1 = 1500pF NP0] from top to junction
[C_LO2 = 4700pF NP0] from junction to GND
[VC2 = variable 10-200pF] in parallel with L_LO

Feedback: junction between C_LO1 and C_LO2 ---[C_fb = 100nF]---> Base Q2
```

### Tracking

The LO must always be 455 kHz above the RF tuned frequency. Both VC1 (RF tank) and VC2 (LO tank) must track together.

In a commercial radio, VC1 and VC2 are sections of a single "ganged" variable capacitor on a common shaft. As you turn the dial, both sections rotate together.

For our receiver, we have two options:

**Option A: Two separate variable capacitors, adjusted together.**
Tune VC1 for the station, then adjust VC2 for maximum signal. This is the manual two-knob approach. It works but is inconvenient.

**Option B: Gang two air-variable capacitors on a common shaft (or use a dual-gang variable cap).**
Dual-gang variable capacitors are available from component suppliers. They have two sections of identical capacitance range that rotate together. However, for the RF and LO tanks to both be in tune with a fixed 455 kHz offset across the entire MW band, the two tank circuits cannot have identical component values — they need "padding" capacitors to compensate.

### Padding Capacitors for Tracking

The tuning equation for the RF tank (signal frequency):
```
f_RF = 1 / (2π × √(L_RF × C_RF_total))
```

The tuning equation for the LO tank:
```
f_LO = 1 / (2π × √(L_LO × C_LO_total))
```

For tracking: f_LO = f_RF + 455 kHz at ALL dial positions.

This is a quadratic constraint — it cannot be perfectly satisfied at all frequencies with just a single ganged capacitor and fixed inductors. But it can be satisfied at three frequencies (using a technique called three-point tracking) and the error is small at all other frequencies.

Three-point tracking uses:
- A "padding" capacitor in series with the LO variable cap (reduces the LO cap's tuning range)
- A "trimmer" cap in parallel with the LO variable cap (adjusts the minimum LO frequency)

**Standard MW tracking values (for a 10–365 pF dual gang capacitor):**

For RF tank (L = 250 μH ferrite rod coil):
```
At 530 kHz: C_total = 1/(4π² × f² × L) = 1/(4π² × 530000² × 250×10^-6) = 360 pF
At 1600 kHz: C_total = 1/(4π² × 1600000² × 250×10^-6) = 40 pF
So VC1 range: 40 pF to 360 pF (330 pF range) — a standard 365 pF gang works.
```

For LO tank (f_LO = f_RF + 455 kHz):
```
At 530 kHz station: LO = 985 kHz
At 1600 kHz station: LO = 2055 kHz
LO tuning ratio: 2055/985 = 2.09 (frequency ratio)
Capacitance ratio needed: (2.09)² = 4.37
```

If we want C_LO to swing from 40 pF to 175 pF (ratio 4.4), but our gang capacitor gives the same range as VC1 (40–360 pF, ratio 9), we need to reduce the capacitance range. Adding a padding capacitor C_p in series with VC1_LO:

The effective capacitance becomes: C_eff = C_p × VC2 / (C_p + VC2)

At maximum VC2 (365 pF), if C_p = 200 pF:
```
C_eff_max = 200 × 365 / (200 + 365) = 73000/565 = 129 pF
```
At minimum VC2 (10 pF):
```
C_eff_min = 200 × 10 / (200 + 10) = 2000/210 = 9.5 pF
```
Ratio: 129/9.5 = 13.6 — too much. Try C_p = 100 pF:
```
C_eff_max = 100 × 365 / 465 = 78.5 pF
C_eff_min = 100 × 10 / 110 = 9.1 pF
Ratio: 78.5/9.1 = 8.6 — still too much.
```

For a 4.4 capacitance ratio: C_p = 47 pF:
```
C_eff_max = 47 × 365 / 412 = 41.6 pF
C_eff_min = 47 × 10 / 57 = 8.2 pF
Ratio: 41.6/8.2 = 5.1 — closer.
```

The LO inductor must then be chosen to resonate at 985 kHz with C = 41.6 pF:
```
L_LO = 1 / (4π² × 985000² × 41.6×10^-12) = 626 μH
```

That is a larger inductor than the RF coil — approximately 100 turns on the same ferrite rod.

**For the beginner's receiver, use Option A (two separate controls) for the first build.** Tracking is a refinement. Once you have the receiver working with two controls, you can add tracking as an upgrade project.

---

## 3.3 Stage C: Mixer

(Full circuit was derived in Stage 8. Reproduced here with complete values for integration.)

```
+9V
 |
[R_MX1 = 100kΩ]
 |
[Base Q3 (BC547)]   <- mixer transistor
 |
[R_MX2 = 10kΩ]
 |
GND

RF from RF amp --- [C_RF = 10nF] ---> Base Q3
LO from Stage B --- [C_LO = 100pF] ---> Base Q3

+9V
 |
[RC_MX = 4.7kΩ]
 |
[Collector Q3]---[T_IF1 primary]---GND
                 (T_IF1 = first 455kHz IF transformer, yellow can)
                 
T_IF1 secondary ----> IF filter stage

[Emitter Q3] ---[RE_MX = 470Ω]--- GND
[Emitter Q3] ---[CE_MX = 100nF ceramic]--- GND
```

---

## 3.4 Stage D: IF Filter

The IF filter defines the receiver's selectivity. It uses a combination of IF transformers and a ceramic filter.

```
T_IF1 secondary ---[C_series = 1nF]---[CFW455 pin 1: IN]
                                      [CFW455 pin 2: GND] --- GND
                                      [CFW455 pin 3: OUT] ---[R_out = 1kΩ]--- GND
                                                                |
                                                          [T_IF2 primary]
                                                                |
                                                               GND
                                                               
T_IF2 secondary ----> IF Amplifier Stage 1
```

The CFW455 ceramic filter:
- Input impedance: approximately 2 kΩ
- Output impedance: approximately 2 kΩ
- Bandwidth: 6–10 kHz at -3 dB (sufficient for AM audio)
- Insertion loss: approximately 3–6 dB

The two IF transformers (T_IF1 and T_IF2) provide additional selectivity and impedance matching.

---

## 3.5 Stage E: IF Amplifier with AGC

Two stages of common-emitter amplification at 455 kHz. Each stage provides about 20 dB gain. Total: 40 dB.

### IF Amplifier Stage 1

```
+9V
 |
[R_IF1_1 = 100kΩ]
 |  (modified by AGC voltage — see below)
[Base Q4 (BC547)]
 |
[R_IF1_2 = 22kΩ]
 |
GND

+9V
 |
[RC_IF1 = 4.7kΩ]
 |
[Collector Q4] ---[T_IF2 primary] (already connected in Stage D output)
                  (wait — T_IF2 is the output transformer of stage D)
                  (T_IF3 is the output transformer of IF amp stage 1)
                  
Actually the signal chain:

Mixer collector → T_IF1 (yellow) → CFW455 → T_IF2 (white) → IF amp 1 base
IF amp 1 collector → T_IF3 (second white or red) → IF amp 2 base
IF amp 2 collector → T_IF4 → Detector

Let us rename clearly:
T_IF_A = mixer output IF transformer (yellow)
T_IF_B = post-CFW455 transformer (white) = IF amp stage 1 input
T_IF_C = IF amp stage 1 output → IF amp stage 2 input (second white)
T_IF_D = IF amp stage 2 output → detector input
```

**Revised signal path:**

```
Mixer Q3 collector
     |
   T_IF_A (yellow, 455kHz IF transformer)
     |
   CFW455 ceramic filter
     |
   T_IF_B (white, 455kHz IF transformer)
     |
   Base of Q4 (IF amp stage 1)
     |
   Collector Q4
     |
   T_IF_C (second white, 455kHz IF transformer)
     |
   Base of Q5 (IF amp stage 2)
     |
   Collector Q5
     |
   T_IF_D (or just RC_IF2 load if using germanium diode detector directly)
     |
   OA91 detector diode
```

### IF Amplifier Stage 1 — Full Circuit

```
Input from T_IF_B secondary:

+9V
 |
[R4_1 = 150kΩ]   (modified by AGC)
 |
[Base Q4 (BC547)]
 |
[R4_2 = 22kΩ]
 |
GND

+9V
 |
[RC_IF1 = 4.7kΩ]
 |
[Collector Q4]---[T_IF_C primary]---GND

[Emitter Q4]---[RE_IF1 = 470Ω]---GND
[Emitter Q4]---[CE_IF1 = 100nF ceramic]---GND

Supply decoupling:
+9V rail to this stage: additional 100μF + 100nF to GND
```

### IF Amplifier Stage 2 — Full Circuit

```
Input from T_IF_C secondary:

+9V
 |
[R5_1 = 150kΩ]   (modified by AGC)
 |
[Base Q5 (BC547)]
 |
[R5_2 = 22kΩ]
 |
GND

+9V
 |
[RC_IF2 = 4.7kΩ]  (or 10kΩ if detector is directly connected to collector)
 |
[Collector Q5]---[Detector input]

[Emitter Q5]---[RE_IF2 = 470Ω]---GND
[Emitter Q5]---[CE_IF2 = 100nF ceramic]---GND
```

### Basic AGC (Automatic Gain Control)

The detector's output contains not just audio but also a DC component proportional to the carrier strength. A strong station produces a large DC voltage after the detector. This DC voltage can be used to reduce the gain of the IF stages — when the signal is strong, AGC turns down the gain; when weak, AGC lets the gain be maximum.

```
Detector output (DC + audio):
     |
[R_AGC = 100kΩ]
     |
[C_AGC = 10μF]  <- AGC filter (removes audio, passes only slow DC)
     |
[AGC voltage output]

This AGC voltage is fed back to R4_1 (top bias resistor of IF amp stage 1):

The AGC voltage (which is negative relative to +9V, since detector is a negative peak detector)
is applied to the base of Q4 through R4_1.

When signal is strong: AGC voltage rises → Q4 base voltage decreases → I_C decreases → gain decreases. ✓

When signal is weak: AGC voltage falls → Q4 base bias restores → gain returns to maximum. ✓
```

**Simple AGC implementation:**

Replace R4_1 (150kΩ fixed) with:
- 100kΩ fixed resistor from +9V to base Q4 (top half of original divider)
- 47kΩ from AGC voltage node to base Q4 (introduces AGC control)

The AGC filter time constant (R_AGC × C_AGC = 100kΩ × 10μF = 1 second) is slow enough to not affect audio (audio is at 300–3000 Hz, too fast to modulate the 1-second AGC) but fast enough to respond to signal level changes as you tune between stations.

---

## 3.6 Stage F: Detector

The detector extracts audio from the 455 kHz IF signal. This is identical to the diode detector in Stage 4, now at 455 kHz instead of the original carrier frequency.

```
IF signal (from Q5 collector or T_IF_D secondary)
     |
[OA91 or 1N34A diode, anode to input, cathode to output]
     |
[R_det = 10kΩ] to GND
[C_det = 10nF] to GND  (forms RC filter: RC = 10kΩ × 10nF = 0.1ms = 10kHz cutoff)
     |
[Audio output] ---[C_audio = 10μF]---> [Audio amplifier]
     |
[DC level (AGC)] ---[R_AGC = 100kΩ]---[C_AGC = 10μF]---> [AGC input to IF stages]
```

**Detector component values explanation:**

The RC filter time constant τ = R × C must satisfy:
- τ << 1/f_IF = 1/455kHz = 2.2 μs (to follow audio envelope, not RF)
- τ >> 1/f_audio_max = 1/5000Hz = 200 μs (to smooth RF ripple)

So we need: 2.2 μs << τ << 200 μs.

With R = 10kΩ and C = 10nF: τ = 100 μs ✓ (100 μs is between 2.2 μs and 200 μs).

**The AGC RC filter:**
- Must be much slower than audio to avoid affecting audio frequency response
- τ_AGC = 100kΩ × 10μF = 1 second — much slower than audio (0.1–100 ms) ✓

---

## 3.7 Stage G: Audio Amplifier

Two-stage common-emitter audio amplifier. Alternatively, a simple class-B push-pull using one BC547 and one BC557.

### Option 1: Two-Stage CE Audio Amplifier

```
Audio from detector ---[C_a1 = 10μF]---> [Base Q6 (BC547)]
                                          |
                                     [R6_1 = 1MΩ] to +9V
                                     [R6_2 = 100kΩ] to GND

+9V
 |
[RC_A1 = 22kΩ]
 |
[Collector Q6]---[C_a2 = 10μF]---> [Base Q7 (BC547)]
                                     |
                                [R7_1 = 470kΩ] to +9V
                                [R7_2 = 47kΩ] to GND

+9V
 |
[RC_A2 = 4.7kΩ]
 |
[Collector Q7]---[C_a3 = 100μF]---> [Output: crystal earphone or 1kΩ volume pot]
                                     (crystal earphone: high impedance, no output transformer needed)

[Emitter Q6]---[RE_A1 = 10kΩ]---GND  (no emitter bypass — for stability and lower distortion)
[Emitter Q7]---[RE_A2 = 470Ω]---GND
[Emitter Q7]---[CE_A2 = 100μF]---GND  (bypass for gain)
```

**Q6 bias (audio stage 1):**
Using R6_1 = 1MΩ, R6_2 = 100kΩ (audio stages use higher value resistors — audio signals are large, can drive base from high impedance unlike RF):
```
V_B = 9 × 100/(1000+100) = 9 × 100/1100 = 0.818V
V_E = 0.818 - 0.65 = 0.168V
I_C ≈ 0.168/10000 = 16.8 μA (very low current — audio stage)
V_C = 9 - 16.8μA × 22kΩ = 9 - 0.37 = 8.6V  (transistor nearly off)
```

Hmm, that is wrong. The high RE = 10kΩ means the emitter is at 0.168V and the collector is at 8.6V — the transistor is barely turned on. We need more current.

**Corrected audio amp bias:**
```
Target I_C = 0.5 mA for Q6:
V_C = 9 - 0.5mA × 22kΩ = 9 - 11 = -2V  (impossible — RC too large)

Use RC_A1 = 10kΩ:
V_C = 9 - 0.5mA × 10kΩ = 9 - 5 = 4V  ✓

RE_A1 = 1kΩ (not 10kΩ):
V_RE = 0.5mA × 1kΩ = 0.5V
V_B = 0.5 + 0.65 = 1.15V

R6_1 = 150kΩ, R6_2 = 22kΩ:
V_B = 9 × 22/(150+22) = 9 × 22/172 = 1.15V  ✓
```

**Corrected audio stage 1:**

| Ref | Value |
|-----|-------|
| Q6 | BC547 |
| R6_1 | 150kΩ |
| R6_2 | 22kΩ |
| RC_A1 | 10kΩ |
| RE_A1 | 1kΩ (no bypass cap — deliberate negative feedback for stability) |
| C_a1 | 10μF electrolytic |
| C_a2 | 10μF electrolytic |

Audio stage 2 (Q7) drives the output:

| Ref | Value |
|-----|-------|
| Q7 | BC547 |
| R7_1 | 100kΩ |
| R7_2 | 22kΩ |
| RC_A2 | 4.7kΩ |
| RE_A2 | 470Ω |
| CE_A2 | 100μF electrolytic |
| C_a3 | 100μF electrolytic (output coupling) |
| Output load | Crystal earphone (high-Z) OR 1kΩ volume pot → crystal earphone |

### Option 2: Direct Crystal Earphone Drive

The simplest audio output: connect the detector output directly to a crystal earphone through a 10μF capacitor. Crystal earphones have impedance of 50 kΩ or more — no amplifier needed if signal is strong enough.

This works for strong local stations (within 50 km) with the full IF chain providing plenty of gain. For weak stations or for loudspeaker output, use Option 1.

### Option 3: Complementary Push-Pull for Speaker Output

For an 8Ω speaker, add a complementary push-pull output stage after Q7:

```
Q7 collector ---[C = 100μF]---> [Base Q8 (BC547 NPN) and Base Q9 (BC557 PNP)]
                                [Q8 emitter and Q9 emitter joined] ---> Speaker (8Ω)
                                [Q8 collector] ---> +9V
                                [Q9 collector] ---> GND
                                
[R_bias = 470Ω between Q8 base and Q9 base to prevent crossover distortion]
  -- actually: two diodes (1N4148) in series between Q8 and Q9 bases, biased by Q7 collector
```

For speaker output, you also need the output power:
```
With Q9 driven to swing ±3V into 8Ω:
P = V_rms² / R = (3/√2)² / 8 = 4.5/8 = 562 mW
```

That requires the power supply to deliver 562 mW at 9V — about 62 mA. A 9V alkaline battery can do this for several hours.

---

## 4. Power Supply Decoupling

This is not glamorous but is critical. Multi-stage RF receivers fail or perform poorly due to inadequate decoupling.

### Why Decoupling Matters

Every transistor stage shares the same 9V supply rail. If a strong signal causes the IF amplifier to draw more current, the supply voltage dips slightly. This dip feeds into every other stage through the shared supply rail. The result is low-frequency oscillation (motorboating) or audio distortion.

The RF stages (RF amp, LO, mixer, IF amp) are particularly sensitive. Their supply must be stable to better than 1 mV.

### Required Decoupling

**Global supply decoupling:**
- 100μF electrolytic across the 9V supply (at the battery terminals)
- 10μF electrolytic near the last IF stage

**Per-stage decoupling:**
Every stage that amplifies RF must have:
- 100nF ceramic capacitor from its +9V supply pin to ground, placed within 1 cm of the transistor
- 1μF to 10μF electrolytic additionally, if space permits

**Supply sequence:** Run separate supply leads to each section rather than daisy-chaining. Audio section supply comes from main +9V via a 100Ω resistor + 100μF capacitor (creates an RC low-pass filter that isolates audio from RF sections):

```
+9V ----+------> RF stages (+9V, with local 100nF decoupling each)
        |
      [100Ω]
        |
      [100μF to GND]
        |
        +------> Audio stages (+9V_audio, isolated from RF)
```

---

## 5. Complete Component List (All Stages)

| Ref | Value | Stage |
|-----|-------|-------|
| Q1 | BC547 | RF amplifier |
| Q2 | BC547 | Local oscillator |
| Q3 | BC547 | Mixer |
| Q4 | BC547 | IF amplifier 1 |
| Q5 | BC547 | IF amplifier 2 |
| Q6 | BC547 | Audio amplifier 1 |
| Q7 | BC547 | Audio amplifier 2 |
| D1 | OA91 or 1N34A | Detector |
| R1, R_RF2 | 150kΩ, 22kΩ | RF amp bias |
| R_LO1, R_LO2 | 100kΩ, 10kΩ | LO bias |
| R_MX1, R_MX2 | 100kΩ, 10kΩ | Mixer bias |
| R_IF1_1, R_IF1_2 | 150kΩ, 22kΩ | IF amp 1 bias |
| R_IF2_1, R_IF2_2 | 150kΩ, 22kΩ | IF amp 2 bias |
| R6_1, R6_2 | 150kΩ, 22kΩ | Audio amp 1 bias |
| R7_1, R7_2 | 100kΩ, 22kΩ | Audio amp 2 bias |
| RC_RF | 4.7kΩ | RF amp collector |
| RC_LO | (via RFC) | LO collector |
| RC_MX | 4.7kΩ | Mixer collector |
| RC_IF1 | 4.7kΩ | IF amp 1 collector |
| RC_IF2 | 4.7kΩ | IF amp 2 collector |
| RC_A1 | 10kΩ | Audio amp 1 collector |
| RC_A2 | 4.7kΩ | Audio amp 2 collector |
| RE_RF | 470Ω | RF amp emitter |
| RE_LO | 100Ω | LO emitter |
| RE_MX | 470Ω | Mixer emitter |
| RE_IF1 | 470Ω | IF amp 1 emitter |
| RE_IF2 | 470Ω | IF amp 2 emitter |
| RE_A1 | 1kΩ | Audio amp 1 emitter |
| RE_A2 | 470Ω | Audio amp 2 emitter |
| R_det | 10kΩ | Detector load |
| R_AGC | 100kΩ | AGC filter resistor |
| C_ant | 100pF | Antenna coupling |
| VC1 | 10-400pF variable | RF tuning |
| VC2 | 10-200pF variable | LO tuning |
| C_LO1 | 1500pF NP0 | LO tank top |
| C_LO2 | 4700pF NP0 | LO tank bottom |
| C_fb_LO | 100nF | LO feedback |
| C_LO_out | 10pF | LO output coupling |
| C_RF | 10nF | RF to mixer coupling |
| C_LO_inj | 100pF | LO to mixer coupling |
| CE_RF, CE_MX, etc. | 100nF ceramic each | Emitter RF bypass |
| CE_A2 | 100μF | Audio amp 2 emitter bypass |
| C_det | 10nF | Detector filter |
| C_AGC | 10μF | AGC filter |
| C_a1, C_a2 | 10μF | Audio coupling |
| C_a3 | 100μF | Audio output coupling |
| C_power_each | 100nF ceramic | Per-stage supply bypass (×7 stages) |
| C_global | 100μF + 10μF | Global supply filtering |
| L_RF | 60T ferrite rod | RF tank coil |
| L_LO | 25μH air core | LO tank coil |
| RFC | 100μH | LO supply choke |
| CFW455 | 455kHz ceramic filter | IF filter |
| T_IF_A | 455kHz IF xfmr (yellow) | Mixer IF output |
| T_IF_B | 455kHz IF xfmr (white) | Post-CFW455 |
| T_IF_C | 455kHz IF xfmr (white) | IF amp 1 output |
| T_IF_D | 455kHz IF xfmr (white) | IF amp 2 output / detector input |
| Earphone | Crystal type | Audio output |

---

## 6. Complete Build Sequence

This is the most important section. Follow it exactly. Do not skip ahead.

### Step 1: Build and test the RF amplifier (Stage A)

Build Q1 and its biasing. Test DC operating point. Add tank coil L_RF and VC1. Test with a diode detector and earphone: tune VC1 across the MW band while holding an antenna wire. Verify you can hear at least one local MW station.

**Pass criteria:** At least one MW station audible with detector + crystal earphone directly at Q1 collector. V_C = 4–5V.

### Step 2: Build and test the Local Oscillator (Stage B)

Build Q2 and its Colpitts oscillator. Test with AM radio method (Stage 7 procedure). Verify oscillation across MW band + 455 kHz.

For the initial test, tune the LO to a fixed frequency — say 1000 kHz (so it can receive the 545 kHz end of the band... wait, that does not work for MW).

Better: tune the LO to approximately 1455 kHz. This covers the 1000 kHz station (1000 + 455 = 1455 ✓). That is BBC or AIR or another strong reference station to test with.

**Pass criteria:** LO oscillates. AM radio in "detector" mode picks up the LO signal when brought within 10 cm of the LO coil. VC2 tunes the LO from approximately 985 kHz (530 kHz station) to 2055 kHz (1600 kHz station).

### Step 3: Build and test the Mixer (Stage C)

Build Q3 and its biasing. Connect to LO output (via 100pF cap) and RF amp output (via 10nF cap). Connect a 4.7kΩ resistor as a temporary load at Q3 collector. Connect RF probe to this load.

**Test procedure:**
1. Apply power. Verify Q3 V_C = 4–5V.
2. Turn on LO (Stage B).
3. Bring antenna near RF amp.
4. Probe collector of Q3 with RF probe. You should see a DC voltage (indicating RF at the collector — mixing products). The exact frequency cannot be verified without a frequency counter, but the presence of signal is confirmed.

**Pass criteria:** RF probe reads > 0.1V DC at Q3 collector with LO running and antenna present. Without LO: near zero.

### Step 4: Add the IF Filter (Stage D)

Replace the temporary 4.7kΩ load at Q3 with T_IF_A (the yellow IF transformer). Connect CFW455 after T_IF_A secondary. Connect T_IF_B after CFW455. Connect RF probe to T_IF_B secondary.

**Test procedure:**
1. Connect all as above.
2. Apply power, LO running, antenna present.
3. The RF probe reading at T_IF_B secondary will be less than at Q3 collector (the IF transformer and CFW455 have insertion loss, and only the 455 kHz component passes through).
4. Adjust T_IF_A slug for maximum RF probe reading. Then adjust T_IF_B slug for maximum.

**Pass criteria:** RF probe reads some level (even very small — 0.01V is fine at this stage). The reading increases when IF transformer slugs are optimally positioned.

### Step 5: Build and test IF Amplifier (Stage E)

Build Q4 and Q5 with their biasing. Connect Q4 base to T_IF_B secondary. Add T_IF_C at Q4 collector. Connect Q5 base to T_IF_C secondary. Add T_IF_D at Q5 collector.

**Test:** RF probe at Q5 collector should read significantly higher than at the filter input. Each IF stage adds about 10–20 dB.

**Pass criteria:** RF probe at Q5 collector reads at least 10× the level at T_IF_B secondary (that is ≥ 20 dB gain from the two IF stages combined).

### Step 6: Build and test the Detector (Stage F)

Connect OA91 diode, R_det, C_det, and C_audio as described. Connect a crystal earphone to the audio output.

**Test:** With strong local station, you should hear audio. If no audio: verify the DC level at the detector output is changing with signal (measure with multimeter on DC 1V range at detector output cathode).

**Pass criteria:** At least one local MW station audible in the crystal earphone.

### Step 7: Build and test Audio Amplifier (Stage G)

Build Q6 and Q7. Connect Q6 base to detector audio output (through 10μF coupling cap). Test each stage independently: inject an audio signal (from a phone at low volume through a coupling cap) and verify amplification.

**Pass criteria:** Voice from a phone, injected into Q6 base through 10μF + 1kΩ, is audible and louder at Q7 collector output.

### Step 8: Connect all stages

Connect Q7 output to earphone.
Now you have a complete superheterodyne.
Tune VC1 (RF) and VC2 (LO) together — both tracking approximately 455 kHz apart.

### Step 9: Alignment

Alignment is the process of adjusting each IF transformer for maximum signal throughput at 455 kHz.

**Full alignment procedure:**

**Prerequisite:** You must have a MW signal available. Use a local station or the Colpitts oscillator from Stage 7 AM-modulated with audio.

1. Tune the receiver to the reference signal (a known MW station or your modulated test oscillator).
2. Adjust VC1 and VC2 until the signal is at maximum (even without alignment, you should hear something).
3. Now: one at a time, starting from the DETECTOR end and working BACK toward the mixer, adjust each IF transformer slug for maximum audio volume.
   - Adjust T_IF_D first (closest to detector)
   - Then T_IF_C
   - Then T_IF_B
   - Then T_IF_A
4. Go around the loop twice — adjusting one transformer changes the loading on the others slightly.
5. Final adjustment: tweak VC1 and VC2 for exact station tuning.

**Why adjust from detector end first?** The stages closest to the output show the clearest indication (audio volume). The earlier stages are harder to align because misalignment looks the same as "no signal."

---

## 7. Performance Expectations

### Sensitivity

With the antenna described (1-metre wire, or 10 cm ferrite rod from a broken transistor radio), typical performance:

| Station distance | Signal strength | Performance |
|-----------------|----------------|-------------|
| < 20 km | > 1 mV at antenna | Very loud, may need to reduce volume |
| 20–100 km | 10–100 μV | Clear audio, good quality |
| 100–500 km | 1–10 μV | Audible but some noise |
| 500–2000 km (night only) | 0.1–1 μV | Marginal — requires careful alignment and good antenna |

### Selectivity

With CFW455 ceramic filter: adjacent-channel selectivity approximately 40 dB at ±10 kHz.
This means: a station 10 kHz away (next channel on MW) is 40 dB weaker in the IF than the desired station. Stations 20 kHz away are nearly completely rejected.

### Image Rejection

With a single tuned RF stage (Q1 and L_RF/VC1):
Q = approximately 80 at 1 MHz (good ferrite rod coil loaded by transistor)
Image frequency for 1 MHz station = 1910 kHz
Separation from desired = 910 kHz
Image rejection ≈ 40–50 dB

This means: a very strong station at 1910 kHz would appear as a very faint ghost when tuned to 1000 kHz. For typical MW usage, this is acceptable.

---

## 8. What to Measure and Record

### 8.1 Stage Gain Measurements

For each amplifier stage, measure gain as follows:
1. Inject a known-amplitude signal at the stage input (use the modulated Colpitts from Stage 7 as a source).
2. Measure the output amplitude with an RF probe.
3. Calculate: Gain (dB) = 20 × log(V_out / V_in)

| Stage | Expected Gain | Your Measurement |
|-------|--------------|------------------|
| RF Amp | 20–25 dB | ___ dB |
| Mixer (conversion) | +10 dB | ___ dB |
| IF Filter (insertion loss) | -3 to -6 dB | ___ dB |
| IF Amp Stage 1 | 15–20 dB | ___ dB |
| IF Amp Stage 2 | 15–20 dB | ___ dB |
| Detector | -6 dB | ___ dB |
| Audio Amp (two stages) | 30–40 dB | ___ dB |
| TOTAL | ~80–90 dB | ___ dB |

### 8.2 Bandwidth Measurement

1. Tune to a MW station.
2. The station's carrier is at frequency f_0. Note the volume at this exact tuning.
3. Detune the LO slowly above f_0. Note the frequency at which the audio falls to half its original volume (that is the -6 dB point, approximately -3 dB in audio power terms).
4. Detune below f_0, same procedure.
5. Bandwidth = frequency at which audio drops to half above + frequency below.

Record:
```
f_0 = ___kHz
f_upper (-3dB) = ___kHz  (offset = ___kHz above)
f_lower (-3dB) = ___kHz  (offset = ___kHz below)
Bandwidth = ___kHz
```

### 8.3 Image Rejection Test

1. Tune to 1000 kHz station. Note its volume (level 10 on your subjective scale).
2. Calculate image frequency: 1000 + 2×455 = 1910 kHz.
3. Tune to 1910 kHz and check if any station there is audible in your receiver when your LO is set for 1000 kHz reception.
4. If a 1910 kHz station is present: note how loud it is relative to the desired 1000 kHz station.
5. Image rejection (dB) = 20 × log(V_desired / V_image).

### 8.4 AGC Performance

1. Note volume with a local station (within 20 km).
2. Connect a long antenna (10 metres of wire) — this increases the signal.
3. With AGC: note whether the volume increases dramatically or is relatively controlled.
4. Without AGC (disconnect the AGC line): note the change — strong signals should now overload and distort.

---

## 9. Troubleshooting — Full Receiver

| Symptom | Diagnosis | Check | Fix |
|---------|-----------|-------|-----|
| Completely silent | No signal through | Inject audio at Q6 base. If heard: problem is in RF/IF chain. If not: audio amp problem. | Work forward from audio amp. |
| Hiss and noise but no stations | IF chain working, no RF | Check LO oscillating. Check VC1/VC2 tracking. | Verify LO with AM radio test. |
| One or two stations, not others | IF or LO alignment wrong | Re-do alignment procedure. Check tracking across band. | Re-align; adjust padding caps. |
| Loud hum on all stations | Supply not decoupled; mains pickup | Check 100μF on supply. Battery? | Add decoupling. Use battery. |
| Motorboating (put-put sound) | Feedback through supply | Check all per-stage 100nF caps. | Add more decoupling. Add 100Ω in supply leads to IF stages. |
| Very strong stations distorted | AGC not working | Check AGC circuit. Check OA91 detector polarity. | Verify AGC voltage present at IF amp base (should vary with signal strength). |
| Receiver oscillates (screaming) | IF amp oscillating | Remove one IF stage at a time to find the culprit. | Add 100Ω series in IF transformer primary lead. Check supply decoupling. |
| Stations sound hollow / metallic | IF bandwidth too narrow | Transformer alignment peaked too sharply. | Re-align with broader approach — do not peak absolutely. |
| AM radio reception but no audio | Detector not working | Test detector with a 455kHz signal (from alignment oscillator) directly. | Check OA91 orientation. Check R_det, C_det values. |

---

## 10. What to Record in Your Notebook

```
Superheterodyne Build Log

Start date: ___________

Stages completed:
[  ] RF Amplifier — pass date: ___
[  ] Local Oscillator — pass date: ___
[  ] Mixer — pass date: ___
[  ] IF Filter — pass date: ___
[  ] IF Amplifier — pass date: ___
[  ] Detector — pass date: ___
[  ] Audio Amplifier — pass date: ___
[  ] Full receiver working — date: ___

Performance measurements (fill after alignment):
- Stations receivable (list): ___________
- Weakest station received: ___kHz, ___km distance
- Selectivity: bandwidth ___kHz at -3dB
- Image rejection: ___dB
- Total gain (estimated): ___dB

Alignment log:
- T_IF_A slug position (turns from full in): ___
- T_IF_B slug position: ___
- T_IF_C slug position: ___
- T_IF_D slug position: ___

Problems encountered and solutions:
___________

Power consumption: ___mA at 9V = ___mW = battery life ___hours (at 500mAh)

Comparison with crystal set (same conditions):
Crystal set strongest station: ___km
Superheterodyne strongest station: ___km
Improvement factor: ___x
```

---

## 11. Key Insight

You have built, from discrete components, a radio receiver that achieves genuine performance. It can receive stations that no crystal set could ever hear. It uses principles that were cutting-edge in 1920 and that remain unchanged in every radio device manufactured today.

The architecture you just built — RF amp, mixer, IF filter, IF amp, detector, audio amp — is called the superheterodyne and it is used in:
- Every AM/FM home radio sold in the last 80 years
- Every mobile phone (your 4G phone is a superhet receiver operating at 700 MHz–2.6 GHz IF ~ few hundred MHz)
- Every WiFi receiver (your ESP32's WiFi is a direct-conversion superhet, which is a superhet where f_IF = 0 Hz)
- Every GPS receiver (L1 frequency 1575.42 MHz, mixed down to baseband)
- Every radar system
- Every satellite set-top box

The ESP32's radio subsystem does the same thing you just built. But the IF stages are implemented as on-chip silicon amplifiers running at GHz rather than kHz, the IF filter is a digital filter running in software, and the detector is replaced by a digital baseband processor. The scale is different. The architecture is identical.

When you use your ESP32 for drone control — when it receives 2.4 GHz WiFi commands from your ground station — the signal passes through an RF amplifier, a mixer, an IF filter, an IF amplifier, and a detector, just as you have built here. The laws of physics have not changed. The engineer of 1920 and the engineer of today solve the same problem with the same tools, just with different materials.

---

*Next: Stage 10 — FM Detection and the Path Forward. You will learn why FM sounds better than AM, how to detect frequency-modulated signals, and how all of this connects to the modern digital radio chips used in your drone.*
