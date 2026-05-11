# Stage 8 — The Mixer: Shifting Frequency to Unlock Selectivity

---

## What You Will Learn

- Why trying to build a high-quality receiver at the received frequency is practically impossible
- The exact mathematics of frequency mixing — a trigonometric identity that creates new frequencies
- What the Intermediate Frequency (IF) is and why specific values were chosen historically
- The image frequency problem — the hidden vulnerability of every superheterodyne receiver
- Four types of mixer circuits and how they differ
- How to build a single-diode mixer and a double-balanced ring mixer from scratch
- How to build the most practical version: a transistor mixer for the superheterodyne receiver
- How IF transformers work and how to adjust them
- How to verify your mixer is working with no test equipment beyond an AM radio

---

## 1. The Problem That Mixing Solves

Imagine you want to build an AM radio that receives 1 MHz with high sensitivity and good selectivity. The natural approach: amplify the 1 MHz signal, then filter it, then amplify again, then detect.

This approach — called TRF (Tuned Radio Frequency) — was built in the early 1920s. It had four serious problems that make it impractical to build at home.

### Problem 1: Gain-Bandwidth Product

Every transistor (or vacuum tube triode in the 1920s) has a fixed gain-bandwidth product. The BC547, for example, has a gain-bandwidth product (f_T) of 300 MHz. At 1 MHz, the maximum available gain is approximately f_T / f = 300 MHz / 1 MHz = 300. But that is open-loop. With practical bias, parasitic capacitances, and stability margins, a single common-emitter stage at 1 MHz delivers about 40 dB (factor of 100) of gain. To get 100 dB of total gain (needed for weak signal reception), you need 2.5 stages. Sounds fine.

Now try to receive 20 MHz. Each stage at 20 MHz only delivers 300/20 = 15 gain maximum. You need far more stages for the same total gain.

At 100 MHz (FM band): each stage gives about 3 in gain — you need a dozen stages for 100 dB. This is where TRF breaks down.

### Problem 2: Everything Must Be Re-Tuned

Each gain stage has its own tuned circuit for selectivity. If you have four stages, all four must track together as you tune across the band. This requires four ganged variable capacitors rotating in perfect synchrony. Mechanical ganging is expensive and imperfect. Commercial TRF radios of the 1920s required three separate tuning controls that all had to be adjusted together — users called them "the three dials of death."

### Problem 3: Stray Capacitance Causes Instability

A high-gain amplifier at high frequency is on the edge of oscillating. The inter-electrode capacitance of the transistor (or tube) feeds a fraction of the output back to the input. This is unwanted positive feedback that becomes increasingly problematic as frequency rises.

In the 1920s, a TRF radio required a "neutralisation" adjustment for every stage — you had to add a small capacitor to cancel the inter-electrode feedback. Mis-adjustment caused oscillation. Correct adjustment required specialist skill.

### Problem 4: AGC Is Impossible to Implement Simply

Automatic Gain Control (AGC) — reducing gain for strong stations to prevent overload — is easy to implement in a fixed-frequency IF amplifier (because you just change the bias on fixed-tuned stages). In a TRF, each stage is at a different frequency and the AGC would need to adjust each differently. Very complex.

### The Solution: Heterodyne (Frequency Translation)

The solution was Edwin Armstrong's superheterodyne, invented in 1918. The insight:

**Translate the received signal to a fixed, lower frequency before amplification.** All the high-gain amplification happens at this fixed intermediate frequency (IF), where:
- Filters can be designed once and fixed
- AGC is straightforward
- Stability is easy to achieve
- The tuned stages are minimal (one in the RF section)

The translation is done by a **mixer**. Understanding the mixer is understanding every superheterodyne receiver ever built.

---

## 2. The Mathematics of Frequency Mixing

The mixer is a multiplier. It takes two signals and multiplies them together.

### 2.1 The Trigonometric Identity

This is not an approximation. It is an exact mathematical identity:

```
cos(A) × cos(B) = ½ × cos(A - B) + ½ × cos(A + B)
```

Proof (for completeness):
```
Using Euler's formula: cos(θ) = (e^jθ + e^-jθ) / 2

cos(A) × cos(B) = [(e^jA + e^-jA) / 2] × [(e^jB + e^-jB) / 2]
= (1/4) × [e^j(A+B) + e^j(A-B) + e^-j(A-B) + e^-j(A+B)]
= (1/4) × [2cos(A+B) + 2cos(A-B)]
= ½cos(A+B) + ½cos(A-B)   QED
```

**What this means physically:** Multiply two cosines at frequencies f1 and f2, and you get two new cosines at frequencies (f1 + f2) and (f1 - f2). The original frequencies f1 and f2 disappear (in a perfect multiplier). Two new frequencies appear.

### 2.2 Applying to Radio

Let:
- f_RF = 1000 kHz (the station you want to receive)
- f_LO = 1455 kHz (your Local Oscillator frequency)

Multiplying (mixing) these:
```
cos(2π × 1000kHz × t) × cos(2π × 1455kHz × t)
= ½ × cos(2π × (1455-1000) × t) + ½ × cos(2π × (1455+1000) × t)
= ½ × cos(2π × 455kHz × t) + ½ × cos(2π × 2455kHz × t)
```

Output frequencies: **455 kHz** (the difference) and **2455 kHz** (the sum).

Now put a filter after the mixer that passes 455 kHz and rejects 2455 kHz. What comes out? Only 455 kHz. You have taken the 1000 kHz signal and translated it to 455 kHz.

But crucially, the audio content of the 1000 kHz signal (the AM modulation) is preserved. The sidebands of the 1000 kHz AM signal — at 995 kHz and 1005 kHz for a 5 kHz audio tone — become:
- 1455 - 995 = 460 kHz
- 1455 - 1005 = 450 kHz

So the ±5 kHz sidebands around 1000 kHz become ±5 kHz sidebands around 455 kHz. The audio information is intact. Only the carrier frequency has changed.

This is frequency translation. The signal's information content is preserved. Only its centre frequency has moved.

### 2.3 The Role of the Filter

The 455 kHz IF filter removes:
- The 2455 kHz sum product (far from 455 kHz — easy to filter)
- All other stations on the MW band that were not translated to 455 kHz
- Noise above and below 455 kHz

The IF filter can be designed once, at a fixed frequency, with very high Q (narrow bandwidth), because it never needs to be tuned. In practice, a ceramic filter (CFW455) or two aligned IF transformers provide excellent selectivity at 455 kHz.

---

## 3. Why 455 kHz Was Chosen (and Why 10.7 MHz for FM)

### 3.1 The MW IF Frequency: 455 kHz

The choice of 455 kHz for AM was not made by a single engineer on a single day. It emerged in the 1920s and 1930s as a compromise between several competing requirements:

**Why not lower (e.g., 100 kHz)?**
- The MW band goes down to 530 kHz. An IF of 100 kHz means the image frequency (explained below) is only 200 kHz away from the desired station. The RF filter before the mixer cannot reject an image that close to the desired signal.
- Low IF transformers are physically large (larger at lower frequency).

**Why not higher (e.g., 2 MHz)?**
- The LO frequency would need to be 2000 + received_frequency. For a 1600 kHz station, LO = 3600 kHz. This is above the MW band. The LO might interfere with shortwave reception.
- Image rejection at 2 MHz IF: the image is 4 MHz away from the desired signal — much easier to filter, but the image falls in the shortwave band and real shortwave stations might get in.
- High Q at 2 MHz requires very precise small inductors — harder to manufacture in the 1930s.

**455 kHz hit the sweet spot:** below the MW band (530 kHz), giving good image separation, with tractable filter design using the technology of the era. It became an industry standard. Today, all MW AM receivers worldwide use 455 kHz IF, and the ceramic filter CFW455 is mass-produced for a few rupees.

**Trivia:** Some older Japanese designs use 450 kHz; some European military designs use 465 kHz. But 455 kHz dominates.

### 3.2 FM IF: 10.7 MHz

The FM broadcast band is 88–108 MHz. An IF of 10.7 MHz means:
- LO = 88 + 10.7 = 98.7 MHz for the bottom of the band
- LO = 108 + 10.7 = 118.7 MHz for the top

The image is 2 × 10.7 = 21.4 MHz above the LO, or at 88 + 21.4 = 109.4 MHz for the bottom of the band. This is just outside the FM band — the pre-mixer RF filter (which passes 88–108 MHz) easily rejects 109.4 MHz. Excellent image rejection.

At 10.7 MHz, very good ceramic filters (CFW10.7M) are available, and Foster-Seeley discriminators (for FM detection) can be built easily. The choice was made by the standards bodies in the 1940s and is still universal today.

---

## 4. The Image Frequency Problem — Fully Explained

This is the most misunderstood aspect of superheterodyne receivers. Read this carefully.

### 4.1 The Symmetry Problem

When the mixer multiplies f_RF × f_LO, it produces |f_LO ± f_RF|. The filter passes only |f_LO - f_RF| = f_IF.

But there are TWO frequencies that, when mixed with the LO, produce f_IF:
- f_RF = f_LO - f_IF (the desired signal, below the LO)
- f_image = f_LO + f_IF (the image, above the LO)

Both mix to the same IF. The IF filter cannot distinguish between them — both are at 455 kHz after mixing.

### 4.2 Numerical Example

```
f_IF = 455 kHz
f_station = 1000 kHz (BBC World Service, for example)
f_LO = f_station + f_IF = 1000 + 455 = 1455 kHz

f_image = f_LO + f_IF = 1455 + 455 = 1910 kHz

Check: 1910 kHz mixed with 1455 kHz LO:
|1455 - 1910| = 455 kHz  ✓  -- it does come through the IF filter
```

So: if you are tuned to 1000 kHz and there is a station on 1910 kHz, it will also appear in your receiver, superimposed on the 1000 kHz station you wanted. This is the image interference.

### 4.3 The Image Rejection Requirement

The only way to reject the image is with a filter BEFORE the mixer — the RF filter or antenna circuit.

The RF filter must:
- Pass 1000 kHz (desired) with low attenuation
- Reject 1910 kHz (image) with high attenuation

The frequency separation is: 1910 - 1000 = 910 kHz, which is 2 × f_IF = 910 kHz away.

A single LC tuned circuit centred at 1000 kHz has a bandwidth of approximately f_0/Q. If Q = 100 (a good antenna circuit), bandwidth = 1000/100 = 10 kHz. The image at 910 kHz away is 910/(10/2) = 182 half-bandwidths away — easily rejected.

With a Q = 100 circuit, image rejection ≈ 20 × log(Q × 2 × f_IF / f_0) dB approximately. For the numbers above: 20 × log(100 × 910/500) = 20 × log(182) = 45 dB of image rejection. Quite good.

### 4.4 Image Rejection vs IF Frequency Trade-off

If you used IF = 50 kHz (lower IF):
- Image = f_LO + 50 kHz = 1100 kHz (only 100 kHz from the desired 1000 kHz)
- A Q = 100 circuit gives attenuation at 100 kHz away: 20 × log(100 × 100/500) = 20 × log(20) = 26 dB. Much worse.

If you used IF = 2 MHz (higher IF):
- Image = f_LO + 2 MHz = 5 MHz (4 MHz from the desired 1000 kHz)
- Image rejection is excellent — the image is outside the MW band entirely.
- But now the IF filter at 2 MHz needs higher-Q components. And there are shortwave stations at 5 MHz that could image in.

455 kHz is the engineering compromise that worked for the MW band with the technology of the 1930s–present.

---

## 5. Types of Mixers

### 5.1 Single Diode Mixer

The simplest mixer: one diode.

A diode is not a linear multiplier. It is a non-linear device — its I-V curve is exponential. When you apply two frequencies simultaneously to a non-linear device, the output contains harmonics and mixing products. Among these products are the sum and difference frequencies.

The exponential diode can be Taylor expanded:
```
i = i_s × [exp(qV/kT) - 1] ≈ i_s × [1 + V/V_T + V²/2V_T² + ...]
```

The V² term produces the mixing products. It is not a pure multiplier (which is why the single-diode mixer has poor isolation and generates many spurious products), but it works.

```
Schematic:
                  RF Input
                     |
                  [L_in]  <- tuned to RF frequency
                     |
              [OA91 diode] -----> IF Output
              (cathode to output)
                     |
                  LO Input
                     |
                 [ground]
```

**Properties:**
- Conversion loss: approximately 6–10 dB (output is weaker than input — no gain)
- Isolation: poor — LO signal leaks into RF input and vice versa
- Spurious outputs: many harmonics and mixing products
- Simplicity: just one diode

Use: demonstration and understanding. Not suitable for good receiver performance.

### 5.2 Single-Balanced Mixer

Use two diodes in a specific arrangement so the LO signal cancels at the output but the mixing products add:

```
      RF ----+-------[D1]----+---- IF
              |               |
              +---[D2 (rev)]--+
              |
             LO
```

The LO feed is symmetric — it appears equally at both diodes. The LO signals cancel in the output (they are anti-phase from the two diodes). But the mixing products (which depend on RF × LO) add in phase.

Result: LO suppressed by 20–30 dB at the IF output. Better RF-to-LO isolation.

### 5.3 Double-Balanced Ring Mixer

The professional standard. Four diodes in a ring with two transformers.

```
         T1 (RF transformer)        T2 (IF transformer)
         
    RF --[primary]--[secondary]--+--D1--+--[primary T2]------- IF out
                                 |      |
                                D4    D2
                                 |      |
              LO --[LO xfmr]----+--D3--+--[secondary T2]------
```

More accurately (standard topology):

```
                   LO
                    |
              [LO transformer]
               |         |
              LO+        LO-
               |         |
    RF+ ---+--D1--+--D2--+--- RF-
           |          |
           +--D4--+--D3--+
               |         |
              IF+        IF-
               |         |
              [IF transformer]
                    |
                    IF out
```

**Properties:**
- Both LO-to-RF and LO-to-IF isolation: 30–40 dB
- RF-to-IF isolation: 30–40 dB
- Conversion loss: ~6 dB
- Very low spurious outputs
- Wide frequency range
- This topology (or its IC equivalent, the Gilbert Cell) is inside every professional radio

---

## 6. Build Experiment 1: Single Diode Mixer

This is for understanding. Build it, observe the mixing products, then move to the transistor mixer for the actual receiver.

### What You Need

- 1× OA91 or 1N34A germanium diode
- 2× small ferrite toroids (or ferrite rods) for RF transformers
- Various capacitors and resistors
- Two signal sources: use two small radios (one FM, one AM) as approximate signal sources, or wind two tank circuits with different tuning caps

### Schematic

```
          RF Signal (MW station at f_RF)
               |
          [L_RF = 20 turns on ferrite]
               |
      +--[C_RF = 100pF]--+
      |                  |
      |            [OA91 diode]   anode to this junction, cathode to IF output
      |                  |
      +------------------+------[IF Output]
      |                  |
      |            [R_load = 47kΩ to GND]  <- IF load
      |
 [LO signal injected here through 10pF cap]
      |
     GND
```

In practice, the single-diode mixer is built as part of the tank circuit:

```
Antenna ----[C_antenna 100pF]----+---- [L_antenna: 60T ferrite rod] ---- GND
                                 |
                            [Tuning cap VC1]
                                 |
                                GND
                                
Signal at top of L_antenna:
                                 |
                           [D1: OA91 anode]
                                 |
                           [D1: OA91 cathode] ----+---- IF output
                                                  |
                                            [C_IF: 1nF]
                                                  |
                                           [L_IF: IF transformer, 455kHz]
                                                  |
                                                 GND
                                                 
LO injected into antenna circuit via small capacitor:
[Colpitts LO output] ---[C_inject: 10pF]--- [junction at top of L_antenna]
```

### Procedure

1. Build the Colpitts oscillator from Stage 7 at approximately 1455 kHz (adjust the oscillator coil turns for this frequency).
2. Build the antenna tank circuit tuned to 1000 kHz (a target MW station).
3. Connect the OA91 diode as shown.
4. Connect a crystal earphone between the IF output and ground.
5. Tune an AM portable radio to 455 kHz (tune the portable to the lower end of the MW dial, around 500–530 kHz — some portables tune this low).

If you cannot find a portable that tunes below 530 kHz, listen at your crystal earphone directly. The 455 kHz signal will be too high for the earphone, but you can measure it with an RF probe.

### What You Should Observe

With the LO running at 1455 kHz and the antenna tuned to 1000 kHz:
- The diode mixes the two signals
- 455 kHz appears at the output (along with many other products)
- The IF transformer (455 kHz resonant) selects this frequency

The mixer is working if you can detect the 455 kHz output with an RF probe (as built in Stage 7 troubleshooting section).

---

## 7. Build Experiment 2: Double-Balanced Ring Mixer

This is the professional approach. Once built, this mixer is usable for any RF project up to about 100 MHz.

### What You Need

- 4× OA91 or 1N34A germanium diodes (must be matched — test them all with a multimeter's diode-test mode and select four with similar forward voltage)
- 2× ferrite toroids (FT37-43 or similar, or wind on small ferrite rod sections)
- 30-gauge enamelled copper wire

### The Transformer Construction

The double-balanced mixer requires two transformers, each wound as bifilar trifilar pairs on ferrite toroids:

**RF/IF transformer (T1 and T2 are identical):**

A bifilar transformer means two wires wound simultaneously (twisted together). They share the same magnetic path, giving close coupling (k ≈ 0.99).

```
Winding T1 (RF transformer):
1. Take 20 cm of 30 AWG enamelled wire, fold it in half (two parallel strands)
2. Twist the two strands together: 3–4 twists per cm
3. Wind the twisted pair 10 times around the ferrite toroid
4. You now have 4 wire ends: call them A, B (from one strand) and C, D (from other strand)
5. Connect B to C — this becomes the centre tap
6. A and D become the two ends of the primary
   OR A and D are the two ends of the secondary (the two windings are symmetrical)
```

Label the transformer:
- Primary: wires A and D (with B-C junction as centre tap = LO injection point)
- Secondary: split into two halves: A to B-C, and B-C to D (each half is 5 turns)

For the DBM, each transformer needs to be a centre-tapped winding. Both transformers are identical.

### Ring Mixer Assembly

```
              LO Input
               |
            [T_LO: 1:1 bifilar transformer on FT37-43]
            Primary: LO+ to LO-
            Secondary centre tap: to LO output of stage 7
               |
             LO+  |  LO-
               |  |  |
      RF+--[T_RF]--+---[D1 anode]--[D1 cathode]--+--[T_IF]--IF+
              |   |                               |
              |  [D4 cathode]               [D2 anode]
              |   [D4 anode]----LO+         [D2 cathode]---LO-
              |                                   |
      RF- --[T_RF]---[D3 cathode]--[D3 anode]----+--[T_IF]--IF-
```

This is getting complex in ASCII art. Let me describe it by connections instead:

**Ring connections (easier to follow):**

The four diodes D1, D2, D3, D4 are connected in a ring:
- D1 anode → D2 cathode (junction called "top")
- D2 anode → D3 cathode (junction called "right")
- D3 anode → D4 cathode (junction called "bottom")
- D4 anode → D1 cathode (junction called "left")

The four junctions connect to the transformers:
- "Top" and "Bottom" junctions → IF transformer (T2) ends
- "Left" and "Right" junctions → RF transformer (T1) ends
- LO transformer centre taps → IF transformer centre tap
- RF signal feeds T1 primary
- IF signal comes from T2 secondary

Rather than recreating this entirely in ASCII, here is the practical description:

**Step-by-step build:**
1. Wind two identical bifilar transformers on FT37-43 toroids: 10 bifilar turns = 20-turn equivalent with centre tap.
2. Arrange four OA91 diodes in a ring on a piece of breadboard, anode to cathode in a square.
3. Connect the RF transformer across one pair of opposite corners.
4. Connect the IF transformer across the other pair of corners.
5. Inject LO to the centre tap of one transformer.
6. Take output from the secondary of the other transformer.

A properly built DBM on FT37-43 toroids with OA91 diodes works from 100 kHz to about 50 MHz — covering the entire AM band, shortwave, and lower VHF.

---

## 8. The Transistor Mixer (Most Practical for Our Superheterodyne)

The transistor mixer provides conversion gain (the output is stronger than the input, unlike the passive diode mixers). It is simpler to build than the DBM and works well for the AM broadcast receiver.

### 8.1 Operating Principle

Use the transistor in a configuration where both the RF signal and the LO signal control the collector current:

- **RF signal:** applied to the base (through the antenna tank circuit and coupling cap). This modulates the base-emitter voltage.
- **LO signal:** applied to the emitter (through a low-impedance coupling). This modulates the transconductance gm of the transistor at the LO frequency.

Since the collector current = gm × V_be, and gm is varying at f_LO while V_be varies at f_RF, the collector current contains a product term at (f_RF × gm_LO) → mixing products at f_LO ± f_RF.

This is essentially the same mathematical product cos(A) × cos(B), but achieved through the non-linearity of the transistor's gm.

### 8.2 Full Transistor Mixer Schematic

```
                          +9V
                           |
                     [R1 = 100kΩ]
                           |
                     [Base Q1 (BC547)]
                           |
                     [R2 = 10kΩ]
                           |
                          GND
                          
RF input (from antenna tank circuit):
[Antenna tank] ---[C_RF = 10nF]--- [Base Q1]

+9V
 |
[RC = 4.7kΩ]
 |
[Collector Q1] ---[C_IF = 10nF]--- [IF transformer primary] --- GND
                                   (IF transformer = L_IF, resonant at 455kHz)
                                   
[Emitter Q1] ---[RE = 470Ω]--- GND

LO injection at emitter:
[Colpitts LO output] ---[C_LO = 100nF]--- [Emitter Q1]
                         (through coupling cap to avoid DC interaction)

Wait — injecting LO at the emitter affects both bias and mixer action.
Better: inject LO at base through a separate path with isolation:

[Colpitts LO] ---[R_isolate = 1kΩ]---[C_LO = 100pF]---[Base Q1]
[RF signal]   ---[C_RF = 10nF]---[Base Q1]  (separate path, both go to base)
```

**Revised — cleaner approach (LO and RF both at base):**

Both RF and LO are applied to the base. The transistor acts as a non-linear mixer because the transconductance gm is a function of the total instantaneous base-emitter voltage, and the V_BE is the sum of the DC bias + RF + LO. The product term in gm × V_BE creates the mixing output.

```
                      +9V
                       |
                  [R1 = 100kΩ]
                       |
        [RF via C_RF]--+--[LO via C_LO]--[Base Q1 (BC547)]
                       |
                  [R2 = 10kΩ]
                       |
                      GND

+9V
 |
[RC_mixer = 4.7kΩ]
 |
[Collector Q1]---[T_IF: 455kHz IF transformer primary]---GND
                 (T_IF secondary → next IF stage)
                 
[Emitter Q1]---[RE = 470Ω]---GND
[Emitter Q1]---[CE = 100nF ceramic]---GND  (RF bypass)

RF input:
[Antenna tank circuit, tuned to f_station] ---[C_RF = 10nF]---> Base Q1

LO input:
[Colpitts LO at f_station + 455kHz] ---[C_LO = 100pF]---> Base Q1
(small coupling cap reduces LO loading on the oscillator)

IF output:
T_IF secondary ---[C_IF_out = 10nF]---> IF amplifier
```

### 8.3 Full Component List — Transistor Mixer Stage

| Ref | Value | Notes |
|-----|-------|-------|
| Q1 | BC547 | NPN transistor |
| R1 | 100kΩ | Base bias top |
| R2 | 10kΩ | Base bias bottom |
| RC | 4.7kΩ | Collector load |
| RE | 470Ω | Emitter stability |
| CE | 100nF ceramic | Emitter RF bypass |
| C_RF | 10nF | RF coupling to base |
| C_LO | 100pF | LO coupling (light coupling to preserve LO) |
| T_IF | 455 kHz IF transformer | The yellow IF can from old AM radio |

### 8.4 Conversion Gain

The transistor mixer provides conversion gain — the 455 kHz IF output is stronger than the RF input. Typical conversion gain for this circuit is +10 to +15 dB (factor of 3 to 5 in voltage).

This compares to the passive diode mixer's conversion LOSS of 6–10 dB. The transistor mixer is preferred for low-signal applications (like receiving weak distant stations).

---

## 9. Practical IF Transformers

The IF transformer is a critical component. It is the bandpass filter of the receiver — it selects 455 kHz and rejects everything else. Understanding it well pays off in Stage 9 (the full superheterodyne).

### 9.1 What Is an IF Transformer Can?

An IF transformer "can" is a small metal-shielded inductor mounted in a square metal can (typically 10mm × 10mm × 12mm). Inside:
- A small ferrite core with a central threaded hole
- A primary winding and a secondary winding on the same former
- A plastic screw slug that can be raised or lowered to adjust inductance
- A parallel trimmer capacitor inside the can that, together with the coil, resonates at 455 kHz (or 10.7 MHz for FM)

The can shields the coil from external interference and prevents coupling to nearby cans.

### 9.2 Colour Coding

In old AM radios, IF transformer cans are colour coded:

| Colour | Purpose in AM receiver |
|--------|----------------------|
| Yellow | First IF transformer (between mixer output and first IF amp) |
| White | Second IF transformer (between first and second IF amp stages, or between IF amp and detector) |
| Black | Oscillator coil (the LO inductor — not an IF transformer but same package) |
| Red | Used in some designs for third IF stage |

The colour coding is not universal — Japanese radios sometimes use a different scheme. Always check the markings on the original PCB.

### 9.3 How to Test and Adjust an IF Transformer

**Equipment needed:** your RF probe from Stage 7, a multimeter, and an AM radio as a signal source.

**Test for continuity:**
- Primary winding: should read 5–20 ohms between the two primary pins
- Secondary winding: should read 2–10 ohms between the two secondary pins
- Primary to secondary: should be open circuit (no shorted turns)

**Adjusting the resonant frequency:**

The threaded slug adjusts inductance:
- Screwing the slug IN (toward the core centre): increases inductance → decreases resonant frequency
- Screwing slug OUT (away from core centre): decreases inductance → increases resonant frequency

**Use a non-metallic tool.** The slots in IF transformer slugs are designed for a thin plastic alignment tool (sold as an "IF alignment tool"). If you use a metal screwdriver, the metal changes the inductance while it is near the coil, making adjustment inaccurate. A sharpened wooden chopstick or plastic toothpick can work in a pinch.

**To align to 455 kHz:**
1. Connect the IF transformer between the transistor mixer collector and a 4.7kΩ load resistor (or the IF amp stage).
2. Inject a 455 kHz signal into the primary. (Source: a signal generator; or use the beat note from your oscillating regenerative receiver tuned to a station 455 kHz away from another reference.)
3. Connect the RF probe to the secondary.
4. Adjust the slug until the RF probe reads maximum. This is the resonant peak at 455 kHz.

If you have no signal generator, simply install the IF transformers per their original design (use the values from the salvaged radio's schematic) and they will be close enough to work. Fine alignment is a refinement, not a requirement for initial operation.

### 9.4 The 455 kHz Ceramic Filter (CFW455)

The CFW455 is a piezoelectric ceramic filter — a small disc (about 3mm) of barium titanate ceramic that resonates mechanically at exactly 455 kHz. Its Q is approximately 1000–2000, compared to about 50–100 for an LC IF transformer.

It has three terminals: input, output, and ground. It is used in place of (or in addition to) the LC IF transformers.

```
Mixer output ---[C = 1nF]---[CFW455 input]---[CFW455 output]---[IF amp input]
                                             |
                                        [CFW455 ground] --- GND
```

The ceramic filter's narrow bandwidth (typically 6–10 kHz for standard types) gives excellent selectivity. It cannot be adjusted — it is fixed at its manufacturing frequency.

**Important:** The ceramic filter requires specific input and output impedances (typically 2kΩ). Drive it with the correct impedance or performance degrades. The standard connection uses 1kΩ series resistor on the input and 1kΩ to ground on the output.

```
Mixer output ---[R_in = 1kΩ]---[CFW455 input]---[CFW455 output]---[IF amp input]
                                |                                   |
                           [C = 1nF to GND]                 [R_out = 1kΩ to GND]
                                               [CFW455 ground] --- GND
```

---

## 10. Testing the Mixer

### 10.1 Injection Test (No Test Equipment)

1. Build the mixer stage and connect the LO (Stage 7 Colpitts at 1455 kHz).
2. Connect an antenna (1-metre wire).
3. Connect a crystal earphone between the IF transformer secondary and ground.
4. If a 1000 kHz station is being received, the IF transformer should ring at 455 kHz and you should hear faint audio in the earphone.

The signal will be very faint because there is no IF amplification yet. You may need to be within 10 km of a strong MW station.

### 10.2 Two-Tone Test (Better — Requires Two Signal Sources)

Use the Colpitts oscillator from Stage 7 as the LO (at 1455 kHz). Build a second small Colpitts oscillator at 1000 kHz as a simulated station.

Connect both to the mixer inputs. Check for 455 kHz at the output with the RF probe.

```
LO oscillator (1455 kHz) ---[100pF]---> Base Q1 (mixer)
RF oscillator (1000 kHz) ---[10nF]---> Base Q1 (mixer)
                                        |
                                  Collector Q1 ---[T_IF]---> RF probe
```

If the RF probe reads a DC voltage (indicating RF at 455 kHz), the mixer is working. Adjust the IF transformer (or use the CFW455) for maximum reading.

### 10.3 Audio Detection Test

Connect the IF transformer secondary to an OA91 diode detector (as built in Stage 4), then to a crystal earphone. With both oscillators running, there is no audio content (the oscillators are pure sine waves, unmodulated). But you can observe the 455 kHz signal with the RF probe.

Now AM-modulate the 1000 kHz oscillator (as you did in Stage 7's experiment). With the audio modulation applied, you should hear the audio through the detector and earphone. This is a complete superheterodyne front-end: mixer → IF filter → detector → earphone.

---

## 11. Building the LO + Mixer as a Sub-Assembly

In Stage 9 you will integrate these into the full superheterodyne. Prepare now by building the LO and mixer as a combined module.

**Sub-assembly construction:**

Use a piece of cardboard (or a piece of veroboard) as the base. Mount components in this order:
1. The Colpitts LO in the upper half of the board
2. The mixer transistor and its biasing below the LO
3. The IF transformer at the output of the mixer
4. The CFW455 after the IF transformer (optional at this stage)

Keep the LO and mixer physically close (short LO coupling wire) but provide a degree of separation from the IF output (RF can couple back and cause instability).

**Shielding:** The LO especially must be shielded from the antenna and the IF output. A small piece of aluminium foil folded into an L-shape between the LO coil and the rest of the circuit is sufficient for initial testing.

---

## 12. Why the Mixer Principle Applies to Your ESP32

The ESP32's WiFi operates at 2.4 GHz. But the main processor runs at 240 MHz and the ADC runs at a few megahertz. How does a 240 MHz processor handle 2.4 GHz signals?

It does not. The WiFi chip inside the ESP32 (the RF subsystem) works exactly as described in this stage:

1. The antenna receives 2.4 GHz.
2. A low-noise amplifier (LNA) amplifies it.
3. A mixer translates 2.4 GHz to a lower IF (approximately 40 MHz in some implementations, or directly to baseband using a zero-IF architecture).
4. The IF is filtered and amplified.
5. An ADC converts the IF to digital samples.
6. Digital signal processing handles demodulation.

The zero-IF (homodyne) architecture is a variant where the LO is set equal to the carrier frequency, translating the signal to DC (zero Hz). This requires very good LO-RF isolation (to avoid DC offset) but eliminates the image frequency problem. Modern WiFi chips predominantly use this approach.

The Gilbert Cell (used in ICs like the NE602/SA612) is a double-balanced active mixer — the transistorised equivalent of your diode ring mixer, with conversion gain. It uses six transistors in a specific configuration that implements the mathematical multiplication cos(A) × cos(B). The SA612 costs a few rupees, integrates the Gilbert Cell and a Colpitts oscillator, and is the entry point to IC radio design. You now understand exactly what is inside it.

---

## 13. Troubleshooting

| Symptom | Likely Cause | Remedy |
|---------|-------------|--------|
| No output at all — RF probe reads zero | LO not running OR mixer transistor biased wrong | Check LO with AM radio test. Measure V_C of mixer transistor (should be 4–6V). |
| Output present but at wrong frequency | LO at wrong frequency | Adjust LO coil turns or capacitor values. Verify LO frequency with AM radio. |
| Strong LO signal at IF output but no mixing | RF signal too weak OR not reaching mixer base | Check antenna connection, C_RF value. Try a stronger local station. |
| Distorted audio from detector test | Mixer overloading (too much RF input) | Reduce RF signal level. Add 1kΩ series resistor in antenna lead. |
| IF transformer heats up | DC current flowing through it — wiring error | Check schematic. IF transformer should have no DC current. Add series capacitor if needed. |
| Mixer works with bench oscillator but not with real stations | LO frequency not tracking correctly | Verify LO is tuned to f_station + 455 kHz not f_station - 455 kHz. Recheck. |
| Image station audible on top of desired station | Insufficient RF filtering before mixer | Add a second tuned LC tank in the RF path before the mixer. Improve antenna circuit Q. |
| Oscillation / squealing from mixer stage | IF feedback through supply into LO | Add 100μF + 100nF decoupling on +9V rail. Separate LO and mixer supply rails. |

---

## 14. What to Record in Your Notebook

```
Mixer Experiment Log

Build date: ___________

LO frequency: ___kHz  (measured with AM radio or beat note method)
IF frequency: 455 kHz (target)
Test RF frequency: ___kHz
Expected IF output: LO - RF = ___kHz

Mixer transistor DC operating point:
V_B = ___V, V_E = ___V, V_C = ___V, I_C = ___mA

RF probe at IF transformer output:
Without LO: ___V DC (should be ~0)
With LO running: ___V DC (should be non-zero if mixer working)
With LO + RF signal: ___V DC (should be greater than LO-only reading)

IF transformer alignment:
Slug position (turns from fully in): ___
Maximum RF probe reading achieved: ___V DC at ___kHz

CFW455 in circuit:
RF probe reading with ceramic filter: ___V DC
Comparison with IF transformer only: ___dB difference

Two-oscillator test:
LO frequency: ___kHz  RF oscillator frequency: ___kHz
Difference (= IF): ___kHz
RF probe at output: ___V DC

Audio test (with modulated RF oscillator):
Audio audible through detector + earphone? YES / NO
Quality (1-5): ___

Notes:
___________
```

---

## 15. Key Insight

The trigonometric identity cos(A) × cos(B) = ½cos(A-B) + ½cos(A+B) is not a circuit design trick. It is a fundamental property of the mathematics of oscillating signals.

Any non-linear device — a diode, a transistor, an overdriven amplifier, even a stretched spring — generates mixing products when two frequencies are applied simultaneously. This happens in sound (beats between two musical notes), in mechanics (coupled pendulums), and in every radio receiver and transmitter ever built.

The superheterodyne was considered so powerful an idea that Armstrong's patent was the most litigated radio patent in history. RCA (Radio Corporation of America) spent decades in court fighting over it. Every AM radio, FM radio, TV tuner, satellite receiver, WiFi chip, GPS receiver, and mobile phone built in the last hundred years uses the frequency translation principle you just implemented with a transistor and a few resistors.

When your ESP32's WiFi connects to a router, it tunes to one of 14 channels (each 5 MHz apart in the 2.4 GHz band). "Tuning to channel 6" means setting the LO frequency to f_channel6 and running the mixer. It is the same operation you just performed manually. The chip does it automatically in microseconds. The physics is unchanged.

---

*Next: Stage 9 — The Complete Superheterodyne. You will connect all the stages built so far into a complete AM radio receiver, align it, and receive stations from hundreds of kilometres away.*
