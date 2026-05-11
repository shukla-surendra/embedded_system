# Stage 10 — FM Detection and the Path Forward

---

## What You Will Learn

- Why FM is fundamentally more noise-resistant than AM, with the numbers
- The mathematics of FM: frequency deviation, modulation index, Carson's rule
- Why an AM detector fails completely on FM signals
- Four methods of FM detection: slope detector, Foster-Seeley, ratio detector, PLL demodulator
- How to build a slope detector and a Foster-Seeley discriminator
- How FM stereo multiplexing works
- How to receive amateur radio (CW and SSB) using a beat frequency oscillator
- What comes next — from discrete components to ICs you can actually understand
- Complete glossary of all terms introduced in Stages 1–10

---

## 1. Why FM Sounds Better Than AM — the Noise Argument

To understand why FM was invented, you must understand what "noise" means in a radio receiver.

### 1.1 What Is Receiver Noise?

At every point in the signal path, thermal noise is present. This is not man-made interference — it is fundamental physics. Every resistor, every transistor, every antenna radiates thermal noise with power:

```
P_noise = k × T × B
```

Where:
- k = Boltzmann's constant = 1.38 × 10^-23 J/K
- T = temperature in Kelvin (300K at room temperature)
- B = bandwidth in Hz

At room temperature and 10 kHz bandwidth (AM audio):
```
P_noise = 1.38 × 10^-23 × 300 × 10000 = 4.14 × 10^-17 watts = -134 dBm
```

That is incredibly small, but remember: the signal from a station 1000 km away might be only -120 dBm at the antenna. The noise floor is only 14 dB below the signal. The signal-to-noise ratio (SNR) is only 14 dB — barely adequate for intelligible audio.

### 1.2 AM and Noise

In an AM signal, the audio information is encoded in the amplitude of the carrier:

```
V_AM(t) = A_c × [1 + m × s(t)] × cos(2π × f_c × t)
```

Where m is the modulation index (0 to 1) and s(t) is the audio signal.

Noise in the channel adds to the amplitude of the signal (among other effects). An AM detector is essentially an envelope detector — it measures amplitude. Therefore, any amplitude noise directly corrupts the detected audio. 

The SNR at the output of an AM detector is approximately:

```
SNR_AM_output = (m² / 2) / (1 + m²/2) × SNR_input_carrier
```

At 100% modulation (m=1):
```
SNR_AM = (0.5) / (1.5) × SNR_input = 0.33 × SNR_input
```

So the output SNR is actually worse than the input SNR. The AM modulation process wastes power in the carrier (which carries no information) and sidebands. At 100% modulation, two-thirds of the transmitted power is in the carrier — wasted.

### 1.3 FM and Noise — the Improvement Factor

In an FM signal, the audio information is encoded in the instantaneous frequency:

```
V_FM(t) = A_c × cos(2π × f_c × t + 2π × k_f × ∫s(τ)dτ)
```

The amplitude A_c is constant. Noise that affects amplitude does not corrupt the frequency — and frequency is what carries the information.

An FM detector is a frequency detector. It ignores amplitude variations (an FM receiver's limiter stage removes amplitude variations entirely before the detector). Therefore, amplitude noise is rejected.

The FM improvement factor over AM (for the same channel SNR) is:

```
SNR_FM_output = 3 × β² × (β + 1) × SNR_input
```

Where β = modulation index = f_deviation / f_audio_max.

For FM broadcast: f_deviation = ±75 kHz, f_audio_max = 15 kHz:
```
β = 75000 / 15000 = 5

SNR_FM = 3 × 5² × (5+1) × SNR_input = 3 × 25 × 6 × SNR_input = 450 × SNR_input
```

FM broadcast gives **450 times better SNR** than the same carrier in AM, for the same transmitter power and same channel noise. In dB: 10 × log(450) = **+26.5 dB SNR improvement**.

This is why a distant FM station sounds nearly perfect (low noise), while an AM station at the same distance sounds hissy. The FM modulation index β = 5 provides a massive SNR bonus at the cost of bandwidth (Carson's rule: BW = 2(β+1) × f_audio = 2 × 6 × 15 = 180 kHz per FM channel — much wider than the 10 kHz AM channel).

### 1.4 The FM Threshold Effect

There is a critical caveat to FM's noise advantage: the FM capture effect (also called the FM threshold).

Below a certain input SNR (approximately 10 dB at the IF input), FM performance collapses catastrophically — the FM detector locks onto the noise instead of the carrier. The SNR at the output plummets below even AM performance.

Above this threshold, FM performs 450× better than AM. Below the threshold, it performs worse.

The practical result: FM works brilliantly when the signal is adequate and fails completely when it is too weak. AM degrades gracefully — it gets noisier as signal falls, but stays intelligible until very weak. This is why:
- Aircraft use AM (ATC frequencies 108–136 MHz) — graceful degradation is safer
- FM broadcast is used for music (high quality when signal is adequate)
- Walkie-talkies use FM (known coverage area, threshold is above minimum usable signal)

---

## 2. FM Signal Structure

### 2.1 What Frequency Deviation Looks Like

In AM, the carrier frequency is fixed. In FM, the carrier frequency swings above and below its centre (rest) frequency:

```
Instantaneous frequency = f_c + k_f × s(t)

For a 1 kHz tone at full deviation (s(t) = cos(2π × 1000 × t)):
f(t) = f_c + 75000 × cos(2π × 1000 × t)

At the peak of s(t): f = f_c + 75 kHz
At the trough: f = f_c - 75 kHz
At zero crossing: f = f_c
```

The carrier sweeps ±75 kHz at a rate of 1000 Hz. The amplitude remains constant throughout.

### 2.2 The Modulation Index

The FM modulation index β (also called m_f) is:

```
β = Δf / f_m
```

Where Δf = peak frequency deviation, f_m = modulating (audio) frequency.

Note that β changes with the audio frequency even at constant audio amplitude. A 100 Hz tone at full deviation gives β = 75000/100 = 750. A 15 kHz tone at full deviation gives β = 75000/15000 = 5.

This is why FM has a "pre-emphasis" and "de-emphasis" circuit — high-frequency audio tones are boosted before transmission (pre-emphasis) and attenuated after detection (de-emphasis) to normalise for this effect and to improve HF noise performance.

### 2.3 Carson's Rule for Bandwidth

The bandwidth occupied by an FM signal:

```
B_FM = 2 × (β + 1) × f_m_max = 2 × (Δf + f_m_max)
```

For FM broadcast: B = 2 × (75 kHz + 15 kHz) = 180 kHz.

Channel spacing on FM broadcast band = 200 kHz (100 kHz in some countries) — just enough for one FM station per 200 kHz slot.

---

## 3. Why the AM Detector Completely Fails for FM

Your crystal set, your superhet detector (Stage 4 and Stage 9) — these are envelope detectors. They output the amplitude envelope of the signal. For AM, this envelope contains the audio. For FM, the amplitude is constant — the envelope is flat. An envelope detector on an FM signal outputs nothing but a constant DC level. No audio.

You can verify this: tune your Stage 9 superhet receiver to an FM station. You will hear nothing. (Or rather: you will hear the IF amplifiers' noise, not the FM audio.)

To detect FM you must convert frequency changes to amplitude changes, then detect the amplitude. There are several ways to do this.

---

## 4. Method 1: The Slope Detector

### 4.1 Principle

The resonance curve of a tuned circuit has a slope on each side of the peak:

```
Amplitude
    |
    |        /\ 
    |       /  \     <- resonance peak at f_0
    |      /    \
    |     /      \
    |----/--------\--------- frequency
         f_0-BW/2  f_0+BW/2
         
         SLOPE    SLOPE
         region   region
```

On the slope region, frequency-to-amplitude conversion occurs: if the input frequency changes, the output amplitude changes according to the slope of the resonance curve.

**Slope detector principle:** Detune the IF tank circuit so that the FM carrier sits on the slope of the resonance curve, not at the peak. As the instantaneous FM frequency swings up and down (following the audio), the output amplitude swings up and down (following the slope). Apply an AM envelope detector to the output. Result: FM detection.

### 4.2 Circuit

For FM at 10.7 MHz IF, detune the IF transformer by adjusting the slug until the 10.7 MHz carrier is approximately halfway up the resonance slope.

```
FM IF input (10.7 MHz) ---[T_IF: 10.7MHz transformer, DETUNED 100-200kHz above centre]
                              |
                         [OA91 diode + 10nF + 10kΩ]  <- envelope detector
                              |
                         Audio output
```

The IF transformer should be adjusted to resonate at approximately 10.8–10.9 MHz (instead of 10.7 MHz). The 10.7 MHz FM carrier then sits on the lower slope. As frequency increases (positive deviation), amplitude increases → positive audio. As frequency decreases (negative deviation), amplitude decreases → negative audio.

### 4.3 Why the Slope Detector Is Poor Quality

The resonance curve is not linear — it is a bell curve. The slope is approximately linear near the centre but curves at the top and bottom. This means the frequency-to-amplitude conversion is non-linear → audio distortion.

Additionally, amplitude noise passes through the slope detector (it is still an envelope detector at heart). The advantage of FM's noise immunity is partially lost.

**The slope detector demonstrates the principle** but should not be used for high-quality reception.

### 4.4 Build and Observe

1. If you have a salvaged FM receiver board: find the 10.7 MHz IF transformer.
2. Desolder it and resolder it into a simple test circuit with the OA91 detector.
3. Adjust the slug approximately 1 full turn from optimum.
4. Connect to an FM radio IF strip (or inject 10.7 MHz FM-modulated signal from a signal generator).
5. Compare the audio quality to proper FM detection — the slope detector audio will be noticeably more distorted and noisier.

This is worth doing once for understanding. Do not use the slope detector in a receiver you want to listen to.

---

## 5. Method 2: The Foster-Seeley Discriminator (1936)

The Foster-Seeley discriminator is the classic circuit for FM detection. It converts frequency to phase, then phase to voltage. Understanding it requires careful reading.

### 5.1 Phase-to-Frequency Relationship

A key property of a tuned circuit (LC resonator): the phase shift through a tuned circuit depends on how far the signal frequency is from the resonant frequency.

At resonance (f = f_0): phase shift = 0°.
Above resonance (f > f_0): phase shift is positive (signal leads).
Below resonance (f < f_0): phase shift is negative (signal lags).

Near resonance, the phase shift is approximately proportional to the frequency offset:

```
φ ≈ 2Q × (f - f_0) / f_0 × (-90°)  ... approximately linear near f_0
```

For a Q = 50 IF transformer at 10.7 MHz, the phase shift is approximately:
```
φ = 90° × (f - 10.7MHz) / (f_0 / 2Q) = 90° × (f - 10.7MHz) / 107000Hz
```

At f = 10.775 MHz (75 kHz above centre, FM peak deviation):
```
φ = 90° × 75000 / 107000 = 63.1°
```

At f = 10.625 MHz (75 kHz below centre):
```
φ = 90° × (-75000) / 107000 = -63.1°
```

The phase changes ±63° across the full FM deviation. The Foster-Seeley detector converts this phase change to a voltage.

### 5.2 Circuit Description

```
FM IF signal input (10.7 MHz)
     |
[Primary of T1: centre-tapped]
     |
 left half          right half
of primary         of primary
     |                  |
[L_primary left]  [L_primary right]
     |                  |
[junction = V_ref, the RF reference voltage for both diodes]
     |
[Via C_RFC = 100pF] to the centre tap of the secondary

Meanwhile:

[Secondary of T1] (NOT centre-tapped — standard winding)
     |
[C_tune = variable, resonant at 10.7MHz]
     |
[two ends of secondary = V_A and V_B]

Now two diodes:
V_A ---[D1: anode to V_A]---[D1 cathode]---+---[R1 = 10kΩ]--- centre point (output)
                                            |
                                          [C1 = 10nF]
                                            |
                                           GND

V_B ---[D2: anode to V_B]---[D2 cathode]---+---[R2 = 10kΩ]--- centre point (output)
                                            |
                                          [C2 = 10nF]
                                            |
                                           GND

V_ref (from primary centre tap, via 100pF coupling cap) --- centre point (output)
```

Wait — this is not quite right. Let me describe the Foster-Seeley more carefully.

### 5.3 Foster-Seeley Proper Circuit Description

The transformer T1 has:
- Primary: a single coil, centre-tapped. Connected to the IF input.
- Secondary: a separate coil, also centre-tapped. Resonant at 10.7 MHz.
- The primary and secondary are coupled magnetically.
- The centre tap of the secondary goes to ground (for DC purposes).
- An additional coupling capacitor C_link (1–10 nF) connects the centre tap of the PRIMARY to the centre tap of the secondary. This provides the reference signal.

The voltages at the two ends of the secondary are V_A and V_B. At resonance, V_A and V_B are equal in magnitude and opposite in phase (since they are measured from the centre tap to each end of the secondary).

The primary centre tap provides a reference voltage V_ref. V_ref is phase-shifted 90° relative to the secondary voltages at resonance (because of the transformer coupling phase).

The four voltages in the circuit:
- V_ref (from primary, via C_link)
- V_A = V_secondary_top = half the secondary voltage
- V_B = V_secondary_bottom = -V_A (opposite phase)
- V_D1 = V_A + V_ref (vector sum applied to D1)
- V_D2 = V_B + V_ref = -V_A + V_ref (vector sum applied to D2)

**At resonance (f = f_0):**
V_A and V_ref are 90° out of phase.
|V_D1| = |√(V_A² + V_ref²)| = |√(V_A² + V_A²)| = V_A√2 (if |V_A| = |V_ref|)
|V_D2| = |√(V_B² + V_ref²)| = |-V_A + V_ref| = same magnitude as V_D1.
Both diodes conduct equally. The output is zero (equal voltages, symmetric circuit).

**Above resonance (f > f_0):**
The secondary phase shifts by more than 90°. Now V_A + V_ref is no longer symmetric.
|V_D1| becomes larger than |V_D2|. D1 conducts more. Output voltage is positive.

**Below resonance (f < f_0):**
The phase shifts the other way. |V_D2| > |V_D1|. D2 conducts more. Output is negative.

**The S-curve:** If you plot the output voltage vs input frequency, you get an S-shaped curve crossing zero at f_0. This is the discriminator characteristic. The audio output is this voltage, which tracks the FM instantaneous frequency. FM is detected.

### 5.4 Full Foster-Seeley Schematic — 10.7 MHz IF

```
          FM IF input (10.7 MHz from mixer)
                |
                |<---- R_load = 1kΩ in parallel (impedance matching)
                |
         [L_primary: 10 turns, wound on T50-6 ferrite toroid or similar]
         [centre-tapped: tap at turn 5]
                |
          top end ------[C_link = 1nF]------+
               |                            |
          bottom end                        |
               |                            |
              GND                           |
                          Centre tap of primary ----+---[C_link = 1nF]---+
                                                                         |
                                                                    (To centre of secondary -- see below)
                          
         [L_secondary: 12 turns on same toroid, close coupling]
         [Resonating capacitor C_res in parallel with secondary]
         C_res = 1/(4π² × f² × L_sec)
         
         If L_sec = 1.5 μH: C_res = 1/(4π² × (10.7MHz)² × 1.5μH) = 148pF ≈ 150pF
                
         [Secondary: top end = A, centre tap = GND, bottom end = B]
         
         A ---[D1 anode]---[D1 cathode]---+---[R1 = 10kΩ]---+---> Audio out
                                          |                  |
                                        [C1=10nF]            |
                                          |                 GND
         B ---[D2 cathode]---[D2 anode]--+---[R2 = 10kΩ]---+
                                         |
                                        [C2=10nF]
                                         |
                                        GND
                                        
         The C_link connects primary centre tap to the junction of D1 cathode + D2 cathode
         (or to A and B via equal impedance paths -- different textbooks show slight variations)
         
         Use D1 = D2 = OA91 or 1N34A (matched pair)
```

### 5.5 Component Selection

- L_primary: 8–12 turns of 26 AWG on T50-6 (yellow) toroid, or salvaged from FM IF strip
- L_secondary: 10–14 turns on same toroid, wound bifilar with primary for close coupling
- C_res: chosen to resonate secondary at exactly 10.7 MHz (measure inductance, calculate C)
- D1, D2: OA91 or 1N34A — must be matched (same forward voltage within 5 mV)
- R1, R2: 10kΩ matched pair
- C1, C2: 10nF matched pair (use same value from same batch)
- C_link: 1nF (connects primary centre tap to detector diodes)

**Symmetry is critical.** Any asymmetry between D1/R1/C1 and D2/R2/C2 creates a DC offset at the output (which shifts the zero-crossing away from f_0) and increases distortion.

### 5.6 Alignment

1. Feed a 10.7 MHz unmodulated carrier (from a Colpitts oscillator at 10.7 MHz — use 0.85 μH and 280 pF).
2. Set oscilloscope on the audio output.
3. Adjust C_res (or use a trimmer in parallel) until the audio output is zero (or minimised). This is the exact resonant frequency alignment.
4. Now feed the FM signal and check for audio.

Without an oscilloscope: use a multimeter on the audio output in DC mode. The DC level should be near zero at exact alignment. Adjust C_res for minimum DC offset.

---

## 6. Method 3: The Ratio Detector (Simpler, Rejects AM)

The ratio detector, developed by Seeley in 1939, is a modification of the Foster-Seeley that adds AM rejection — an important practical benefit.

### 6.1 The Key Difference

The ratio detector adds a large electrolytic capacitor (4.7–10 μF) across the entire detector output. This capacitor:
- Charges to the total instantaneous amplitude of the IF signal
- Has a time constant (R × C) much longer than one IF cycle
- Therefore, the total voltage across both R1 and R2 is held constant

If the IF amplitude suddenly jumps (due to an AM noise spike), the large cap holds the sum voltage steady. The individual diode currents adjust, but their sum (= total voltage across both R) stays the same. The AM variation cancels. Only the FM-related ratio of D1/D2 currents changes the output.

### 6.2 Ratio Detector Circuit

```
FM IF input
     |
[T1 transformer: same as Foster-Seeley]
     |
A ---[D1 cathode]---[D1 anode]---+---[R1=10kΩ]---+--- Audio output
                                  |               |
                                [C_stab=4.7μF]    +--- (midpoint between R1 and R2)
                                  |               |
B ---[D2 anode]---[D2 cathode]---+---[R2=10kΩ]---+

NOTE: In ratio detector, both diodes point the SAME direction (one is reversed vs Foster-Seeley)
```

Wait — the polarity distinction: in the Foster-Seeley, one diode points each way (D1: A → cathode, D2: B → anode). In the ratio detector, both diodes point the same way around the loop.

The full ratio detector description:

Both D1 and D2 are connected so that current flows in the same direction around the loop A → D1 → R1 → junction → R2 → D2 → B.

The stabilising capacitor C_stab is connected across the total output (from the top of R1 to the bottom of R2). Its large value (4.7μF) holds the total voltage constant, rejecting AM.

The audio output is taken from the midpoint between R1 and R2 to the centre of C_stab (which is the effective ground of the audio output). This midpoint voltage tracks the FM instantaneous frequency.

**Advantage over Foster-Seeley:** AM rejection means no limiter stage is needed before the detector. The ratio detector works reasonably well even with AM-modulated noise spikes.

**Disadvantage:** Slightly more complex to understand and align. Audio output is half that of Foster-Seeley. The C_stab introduces some phase shift at low audio frequencies.

**Historical usage:** Most portable FM radios from the 1960s through 1990s used ratio detectors. The Foster-Seeley is more common in high-fidelity applications.

---

## 7. Method 4: The PLL FM Demodulator

The phase-locked loop (PLL) FM demodulator is the most modern all-discrete approach. It is the basis for how digital FM demodulation works in chips.

### 7.1 The PLL Concept

A PLL consists of three blocks:

```
Input FM signal --> [PHASE DETECTOR] --> [LOOP FILTER] --> [VCO]
                           ^                                   |
                           |___________________________________|
                               feedback
```

- **VCO (Voltage-Controlled Oscillator):** generates a signal whose frequency depends on a control voltage. (This is the Colpitts + varactor circuit from Stage 7.)
- **Phase Detector:** compares the phase of the VCO output with the input signal. Outputs a voltage proportional to the phase difference.
- **Loop Filter:** a low-pass filter that removes high-frequency components from the phase detector output.

When the loop is locked (in equilibrium):
- The VCO frequency equals the input signal frequency (to within a small phase error)
- The VCO control voltage is exactly what is needed to make the VCO track the input

If the input FM signal's frequency increases (due to audio modulation), the VCO must increase its frequency to follow. The control voltage (VCO tuning voltage) must therefore increase.

**This control voltage is the audio signal.** The VCO control voltage tracks the instantaneous FM frequency — it IS the demodulated audio. No diodes or transformers needed. FM demodulation is achieved through phase locking.

### 7.2 Discrete PLL FM Demodulator

All three PLL blocks can be built from your component kit:

**VCO (Colpitts + varactor):** From Stage 7, replacing one tank capacitor with a varactor diode. The control voltage modulates the varactor capacitance, modulating the VCO frequency.

**Phase detector (product detector):** A diode ring mixer (from Stage 8) with the input FM signal on one port and the VCO output on the other. The difference-frequency output of the mixer is zero when the VCO is locked to the carrier (zero frequency difference = DC). When there is a phase difference, the output is a DC voltage proportional to sin(Δφ).

```
FM IF input ---[D-ring mixer port 1]
VCO output  ---[D-ring mixer port 2]
                   |
              [Output = V_phase_error]
```

**Loop filter:** A simple RC low-pass filter. The time constant must be chosen for:
- Slow enough to remove IF ripple from phase detector output
- Fast enough to follow FM audio (up to 15 kHz)
- Lock acquisition range and tracking range trade-off

```
Phase detector output ---[R_loop = 10kΩ]---+----> VCO control input
                                            |
                                        [C_loop = 10nF]
                                            |
                                           GND
                                           
Time constant: 10kΩ × 10nF = 100μs = 10kHz bandwidth ✓
```

### 7.3 The 4046 PLL IC — First Principles Understanding

The CD4046 or HEF4046 is a single IC containing:
- One VCO (pin 9 = control input, pins 4 and 5 = frequency range caps, pin 3 = output)
- Two phase comparators (PC1 and PC2, pins 2/3/13 area)
- Input buffers

It costs a few rupees and implements the exact PLL architecture above. After building the discrete version, using the 4046 is not "giving up on understanding" — it is using an integrated version of what you just built.

**The 4046 as FM demodulator:**
```
FM IF (10.7 MHz) ---> [4046 pin 14: signal input]
4046 pin 3 (VCO out) ---> [4046 pin 3: also phase comparator 2 input]
4046 pin 4 and 5: C1 = 150pF (sets VCO range)
4046 pin 12 (PC2 output) ---> [loop filter RC] ---> [4046 pin 9: VCO control]
                                                       |
                                                  [Audio output at pin 9]
```

The demodulated audio is taken from pin 9 (VCO control voltage). This is FM demodulation with zero discrete coils or transformers.

---

## 8. FM Stereo — The Pilot Tone

### 8.1 How Mono FM Works

In mono FM: the audio (L+R mixed together, monophonic) frequency-modulates the carrier. Bandwidth ±75 kHz. Received and detected as described above.

### 8.2 The Stereo Multiplex Baseband Signal

FM stereo is backward compatible with mono: a mono receiver gets the sum signal (L+R) and works normally. A stereo receiver extracts both L+R and L-R, then recovers L and R separately.

The FM stereo baseband signal (what is inside the ±75 kHz deviation) contains:

```
0–15 kHz:     L+R (sum signal — mono compatible)
19 kHz:       Pilot tone (38 kHz/2 = 19 kHz, used to reconstruct stereo subcarrier)
23–53 kHz:    L-R double-sideband suppressed carrier signal, centred at 38 kHz
53–75 kHz:    (usually unused, sometimes used for RDS data at 57 kHz)
```

The pilot tone at 19 kHz is the key. A frequency doubler (or PLL locked to 19 kHz) reconstructs the 38 kHz suppressed carrier. This is multiplied with the L-R DSB signal to demodulate the L-R audio. Then:

```
L = (L+R) + (L-R) = 2L → divide by 2
R = (L+R) - (L-R) = 2R → divide by 2
```

The stereo decoder uses another PLL locked to the 19 kHz pilot tone. This is why a weak FM signal causes stereo to "blend" to mono (the 19 kHz pilot disappears into noise, the stereo decoder loses lock and switches to mono).

---

## 9. Build Experiment: Simple FM Receiver Front End

### Overview

Build an FM receiver that covers 88–108 MHz using:
- One BC547 RF amplifier at 88–108 MHz
- One BC547 Colpitts oscillator at f_station + 10.7 MHz (LO for FM)
- One BC547 mixer → 10.7 MHz IF
- IF strip (either wound coils or salvaged FM IF transformer strip)
- Foster-Seeley detector
- Audio amplifier from Stage 9

### 9.1 RF Amplifier for FM (88–108 MHz)

At 88–108 MHz, parasitic capacitances dominate. The BC547's gain-bandwidth product is 300 MHz — at 100 MHz, available gain is only about 3 (10 dB). This is sufficient for one RF stage.

**Coil for 88–108 MHz:**

Target inductance for resonance at 98 MHz with a 10–50 pF variable cap:

```
At f = 98 MHz and C = 30 pF (mid-range):
L = 1 / (4π² × f² × C) = 1 / (4π² × (98×10^6)² × 30×10^-12)
  = 1 / (4π² × 9.6×10^15 × 30×10^-12)
  = 1 / (4π² × 2.88×10^5)
  = 1 / (11.37×10^6)
  = 88 nH ≈ 90 nH
```

Winding 90 nH air-core coil:
For a single-layer air-core coil, N turns on a 5mm diameter (r = 0.197 inch) former, 5mm length (l = 0.197 inch):
```
L = r² × N² / (9r + 10l) microhenries
0.090 = 0.0388 × N² / (1.773 + 1.97)
0.090 = 0.0388 × N² / 3.74
N² = 0.090 × 3.74 / 0.0388 = 8.67
N = 2.95 ≈ 3 turns
```

**FM RF coil: 3 turns of 22 AWG (0.6mm) bare or enamelled wire, 5mm diameter, 5mm long.**

This is a tiny coil — 3 turns on a former the diameter of a pencil. Spread the turns slightly so they are self-supporting (no former needed).

### 9.2 FM Local Oscillator (f_LO = f_station + 10.7 MHz)

For 88–108 MHz FM band: LO range = 98.7–118.7 MHz.

Same coil as RF section (3 turns, 5mm) but with different capacitor to shift frequency:

```
At 108 MHz (top of FM band + 10.7 MHz = 118.7 MHz):
C = 1/(4π² × f² × L) = 1/(4π² × (118.7×10^6)² × 90×10^-9)
  = 1/(4π² × 1.41×10^16 × 90×10^-9)
  = 1/(4π² × 1.27×10^6)
  = 20 pF
  
At 88 MHz (bottom + 10.7 MHz = 98.7 MHz):
C = 1/(4π² × (98.7×10^6)² × 90×10^-9) = 28.9 pF
```

So LO tunes from 20 to 29 pF. A 5–30 pF trimmer capacitor covers this range.

### 9.3 The Mixer at VHF

The transistor mixer from Stage 8 works at VHF with modification. At 100 MHz, lead inductance and stray capacitance are significant. Keep all leads very short (< 5mm).

Use the emitter-injection mixing topology at VHF:
- RF to base (through 1pF coupling cap — tiny coupling to avoid loading)
- LO to emitter (through 10pF)
- IF at collector through 10.7 MHz IF transformer

### 9.4 IF Strip at 10.7 MHz

Wind the 10.7 MHz IF coils or salvage from a broken FM radio:

For a salvaged FM IF transformer: remove from the board. Test for continuity (primary ~5 ohms, secondary ~3 ohms). Resolder into your circuit.

For wound coils: 10.7 MHz IF transformer on T25-6 toroid:
- Primary: 8 turns
- Secondary: 5 turns
- C_res across primary: 1/(4π² × (10.7MHz)² × L)

If L_primary ≈ 2.2 μH:
```
C_res = 1/(4π² × (10.7×10^6)² × 2.2×10^-6)
      = 1/(4π² × 1.145×10^14 × 2.2×10^-6)
      = 1/(4π² × 2.52×10^8)
      = 101 pF ≈ 100 pF
```

### 9.5 Connecting the FM Front End

```
[FM Antenna, 75cm wire] 
     |
[C_ant = 1pF]  <- very light coupling at VHF
     |
[RFC tank: 3T coil + 10-50pF variable]
     |
[C_in = 1pF] ---> [Base Q1: FM RF amp, BC547]
                   |
              [RC = 560Ω]  <- lower Rc at VHF for stability
                   |
              [Collector Q1] ---> [C_mix = 1pF] ---> [Base Q3: mixer]

[FM LO: Q2 BC547 Colpitts] ---> [C_LO = 10pF] ---> [Emitter Q3]

[Collector Q3] ---> [T_IF 10.7MHz] ---> [10.7MHz IF chain]
                                         (2 stages of IF amp at 10.7MHz)
                                               |
                                        [Foster-Seeley detector]
                                               |
                                        [Audio amp stages G from Stage 9]
                                               |
                                         [Speaker / earphone]
```

---

## 10. SSB and CW Reception with a Beat Frequency Oscillator

### 10.1 Why CW and SSB Need a BFO

**CW (Continuous Wave, i.e., Morse code):** A CW signal is a carrier switched on and off. At 7.025 MHz (40m amateur band), the transmitter simply turns the 7.025 MHz carrier on and off. In the IF of your receiver, this appears as 455 kHz (or whatever your IF is) switching on and off. An AM envelope detector hears... silence (the carrier is on) and silence (the carrier is off). No audio. You hear nothing.

To hear CW as dots and dashes, you need to hear the difference between "carrier on" and "carrier off." If you inject a tone into the detector, when the carrier is on you hear the tone (the two mix together), when the carrier is off you hear silence. Dots and dashes.

The Beat Frequency Oscillator (BFO) provides this injected tone.

**SSB (Single Sideband):** An SSB signal suppresses the carrier and transmits only one sideband. A 1 kHz audio tone on an SSB transmitter at 7.025 MHz appears only as the single frequency 7.026 MHz (upper sideband) or 7.024 MHz (lower sideband). No 7.025 MHz carrier. No other sideband.

An AM detector detects the envelope. An SSB signal's envelope varies rapidly at the sideband frequency (1 kHz in this example) — but detected without a carrier reference, the output is a mess: the diode distorts the single-sideband signal horribly.

The BFO reinserts the missing carrier. Now the detector has both the sideband (carrying audio information) AND the reinserted carrier from the BFO. The mix produces intelligible audio.

### 10.2 BFO Circuit

The BFO is a small Colpitts oscillator tuned near the IF frequency, with a trimmer capacitor for fine adjustment.

```
BFO target frequency: 455 kHz ± 1 kHz
(The ±1 kHz offset sets the pitch of the CW audio tone)

BFO circuit:
+9V
 |
[R_BFO1 = 100kΩ]
 |
[Base Q_BFO (BC547)]
 |
[R_BFO2 = 10kΩ]
 |
GND

+9V
 |
[RFC = 1mH]
 |
[Collector Q_BFO] ---[C_BFO_out = 1pF]---> BFO output to detector
 
[L_BFO: same size as Colpitts LO from Stage 7 but tuned to 455kHz]
[L_BFO = 100μH]  (same as used for the IF transformer]
[C_BFO1 = 1500pF NP0]
[C_BFO2 = 4700pF NP0]
[C_tune = 10-100pF trimmer in parallel with L_BFO] <- pitch control

Feedback: C1/C2 junction ---[100nF]---> base Q_BFO

[Emitter Q_BFO] ---[100Ω]--- GND
[Emitter Q_BFO] ---[10nF]--- GND (bypass)
```

### 10.3 Injecting the BFO

The BFO output is injected into the detector stage through a very small coupling capacitor (1–5 pF). This coupling must be very light — too much BFO injection overloads the detector and all you hear is the BFO.

```
BFO output ---[C_inject = 2pF]--- [Junction between IF transformer secondary and OA91 detector diode]
```

### 10.4 Using the BFO for CW

1. Turn on the BFO. Adjust the trimmer until you hear a 600–800 Hz tone when the BFO is on and the detector is connected (with no station signal, you will hear the BFO tone mixed with IF noise).
2. Tune the receiver to the amateur 40m CW sub-band (7.000–7.100 MHz). This requires changing the LO and RF tank to these frequencies (see Stage 6 for coil values at 7 MHz).
3. When a Morse station is present: you hear a tone appearing and disappearing. The dots and dashes of Morse code.
4. Adjust the trimmer for a comfortable tone pitch (conventionally 600–800 Hz for copying code).

### 10.5 Using the BFO for SSB

1. Enable the BFO.
2. Tune to the amateur 40m SSB portion (7.100–7.200 MHz for lower sideband in most of the world).
3. Tune the LO carefully (within ±100 Hz) to where a voice signal sounds natural — not chipmunk-squeaky and not too low-pitched.
4. The fine adjustment of the LO (or a separate product detector with the BFO adjustable) is needed for SSB: the BFO must be within a few hundred Hz of the suppressed carrier for natural-sounding voice.

This is the hardest part of SSB reception on a simple receiver. The correct BFO/LO setting requires patience.

---

## 11. The Path Forward — From Discrete to ICs You Understand

### 11.1 What You Have Mastered

After completing all 10 stages, you understand:
- LC resonance, Q factor, bandwidth
- Diode detection (AM)
- Transistor amplification (common-emitter, bias design, gain calculation)
- Positive feedback and regeneration
- Oscillator theory (Barkhausen criterion, Colpitts topology)
- Frequency mixing (heterodyne, product, sum/difference)
- The superheterodyne architecture (RF amp → mixer → IF filter → IF amp → detector → audio)
- FM signal structure and FM detection (slope, Foster-Seeley, ratio, PLL)
- BFO for CW/SSB
- Noise theory (thermal noise, SNR, AM vs FM comparison)

### 11.2 The First Valid IC: NE602/SA612 (Gilbert Cell)

The NE602 (manufactured as SA612, NE602AN, etc.) is a single IC combining:
- A Gilbert Cell double-balanced mixer (6 transistors in a specific cross-coupled configuration)
- A Colpitts oscillator
- Bias circuitry

The Gilbert Cell multiplier performs the exact cos(A) × cos(B) = ½cos(A-B) + ½cos(A+B) operation with:
- Conversion gain: +14 dB (much better than passive diode mixer)
- LO-RF isolation: 50 dB
- Noise figure: 5 dB
- Operating frequency: up to 500 MHz

You understand exactly what is inside this IC. You built the individual parts in Stages 7 and 8. The SA612 puts them on a single chip with better performance than achievable on a breadboard at VHF.

**SA612 connections for MW superhet mixer:**
```
Pin 1: RF input (via 1nF from antenna tank)
Pin 2: RF input complement (to GND via 1nF)
Pin 6: LO output (to external LO coil: 0.1μH to tune to f + 455kHz)
Pin 7: LO companion (capacitor to pin 6)
Pin 4: IF output to IF transformer
Pin 5: IF output complement to GND via decoupling
Pin 8: +5V supply (via 100nF bypass)
Pin 3: GND
```

One SA612 replaces the LO (Q2) and mixer (Q3) from Stage 9, with better performance.

### 11.3 The TDA7052 Audio Amplifier

A BTL (Bridge-Tied Load) audio amplifier IC. 1W output into 8Ω speaker from 5V supply. Replaces the Q6/Q7 audio stages. No output coupling capacitor needed.

You understand what is inside: it is several common-emitter stages followed by a push-pull output stage — exactly what you built in Stage 9 Option 3, integrated and optimised.

### 11.4 The TEA5767 FM Module

The TEA5767 is a complete FM receiver IC (88–108 MHz) with:
- Low-noise amplifier
- Colpitts VCO (or PLL LO)
- Gilbert Cell mixer
- 10.7 MHz IF strip (ceramic filters inside)
- FM quadrature detector (a variant of ratio detector)
- Stereo decoder
- I²C interface for frequency control from a microcontroller

Now you understand every stage inside it:
- LNA = your FM RF amplifier (Q1)
- VCO = your Colpitts oscillator (Q2)
- Mixer = your transistor mixer (Q3), or more precisely a Gilbert Cell
- IF strip = your IF transformers and CFW455
- FM detector = Foster-Seeley or ratio type
- Stereo decoder = PLL locked to 19 kHz pilot

The chip does not do anything you have not built. It does it at a higher integration level.

### 11.5 The ESP32 Radio Subsystem

The ESP32's WiFi/BT radio operates at 2.4 GHz. Inside the chip (the radio die, separate from the Xtensa processor die in a multi-chip module):

- **LNA (Low Noise Amplifier):** CMOS RF amplifier, same principle as Stage 9 RF amp. Noise figure approximately 3 dB.
- **Mixer:** Gilbert Cell or passive CMOS switching mixer. Converts 2.4 GHz to baseband (zero-IF architecture) or to an intermediate IF.
- **VCO:** LC oscillator on-chip with switched capacitor banks for channel selection, locked to a crystal reference via PLL.
- **PLL synthesiser:** exactly Armstrong's regenerative concept, auto-stabilised. The PLL locks the VCO to the crystal reference, divides down to the desired channel frequency.
- **ADC:** converts the baseband I/Q signal to digital samples.
- **Digital baseband:** software implements demodulation, equalization, FEC (Forward Error Correction), and the 802.11 protocol stack.

The RF portion — antenna to ADC — is the superheterodyne (or zero-IF variant) you just built. The baseband portion is digital signal processing.

**Channel tuning on the ESP32:** when your drone code calls `WiFi.begin("SSID", "password")` and the chip tunes to 2.4 GHz channel 6, it is setting the PLL synthesiser to generate the LO at 2.437 GHz + IF offset. The same operation as tuning VC1 and VC2 on your Stage 9 receiver, but done automatically in microseconds by the PLL.

---

## 12. Complete Glossary — All 10 Stages

This glossary covers every technical term introduced across Stages 1–10.

**AGC (Automatic Gain Control):** A feedback system that reduces receiver gain when the signal is strong and increases it when the signal is weak, maintaining approximately constant output volume.

**AM (Amplitude Modulation):** A modulation scheme where the audio signal varies the amplitude of a radio-frequency carrier.

**Antenna:** A conductor designed to efficiently radiate or receive electromagnetic waves. Length typically a quarter or half wavelength of the operating frequency.

**Barkhausen Criterion:** The condition for sustained oscillation: loop gain = 1 (magnitude), loop phase = 0° or 360°. Named after Heinrich Barkhausen (1921).

**Bandwidth:** The range of frequencies passed by a filter or stage. Measured at the -3 dB (half-power) points.

**Beat Frequency Oscillator (BFO):** A local oscillator injected into the detector stage to enable reception of CW and SSB signals.

**Carson's Rule:** The FM bandwidth approximation: B = 2(Δf + f_m_max), where Δf is peak deviation and f_m_max is the highest audio frequency.

**Ceramic Filter (CFW455):** A piezoelectric bandpass filter resonant at 455 kHz, used in the IF stage of AM receivers for selectivity.

**Clapp Oscillator:** A Colpitts variant with an additional series capacitor for improved frequency stability.

**Colpitts Oscillator:** An LC oscillator using a capacitive voltage divider (two capacitors) as the feedback network.

**Common-Emitter Amplifier:** A transistor amplifier configuration with input at the base and output at the collector. Provides voltage gain and 180° phase inversion.

**Conversion Gain:** The ratio of IF output power to RF input power in a mixer. Active mixers can have positive conversion gain; passive diode mixers have conversion loss.

**Crystal Oscillator:** An oscillator using a quartz crystal as the frequency-determining element. Extremely stable (±50 ppm or better).

**CW (Continuous Wave):** Morse code transmission, where the carrier is switched on and off to form dots and dashes.

**dB (Decibel):** Logarithmic ratio: Power = 10×log(P2/P1) dB; Voltage = 20×log(V2/V1) dB.

**Demodulation (Detection):** The process of extracting audio (or data) from a modulated radio-frequency signal.

**Diode:** A two-terminal semiconductor device that conducts current in one direction (forward bias) and blocks it in the other (reverse bias).

**Discriminator (Foster-Seeley):** An FM detector that converts frequency deviation to phase shift, then uses two diodes to convert phase shift to audio voltage.

**Emitter Bypass Capacitor:** A capacitor across the emitter resistor that allows AC (RF or audio) signals to bypass the emitter resistor, increasing AC gain.

**Emitter Follower (Common Collector):** A transistor configuration with high input impedance and low output impedance. Used as a buffer between stages.

**FM (Frequency Modulation):** A modulation scheme where the audio signal varies the instantaneous frequency of a radio-frequency carrier. More noise-immune than AM.

**Frequency:** Number of oscillation cycles per second. Unit: Hz (hertz). 1 MHz = 10^6 Hz. 1 GHz = 10^9 Hz.

**Gain:** The ratio of output signal to input signal, usually expressed in dB.

**Gilbert Cell:** A six-transistor double-balanced mixer topology that implements mathematical multiplication. Used inside ICs like SA612/NE602.

**Hartley Oscillator:** An LC oscillator using a tapped inductor as the feedback network.

**Heterodyne:** The process of mixing two frequencies to produce their sum and difference. Derived from Greek: hetero (different) + dyne (force).

**IF (Intermediate Frequency):** The fixed frequency to which all received signals are translated by the mixer in a superheterodyne receiver. Standard: 455 kHz (AM), 10.7 MHz (FM).

**Image Frequency:** A spurious signal that mixes with the LO to produce the same IF as the desired signal. Lies at 2×f_IF away from the desired frequency on the opposite side of the LO.

**Inductor (Coil):** A wound conductor that stores energy in a magnetic field. Opposes changes in current. Reactance: X_L = 2πfL.

**Ionosphere:** Layers of ionised gas in the upper atmosphere (60–600 km altitude) that reflect radio waves at MF/HF frequencies, enabling long-distance propagation.

**LC Tank Circuit:** An inductor (L) and capacitor (C) connected together (in series or parallel) forming a resonant circuit with a natural resonant frequency f_0 = 1/(2π√(LC)).

**LNA (Low Noise Amplifier):** A low-noise amplifier used as the first stage of a receiver to amplify weak signals without significantly degrading the signal-to-noise ratio.

**LO (Local Oscillator):** The oscillator in a superheterodyne receiver that mixes with the incoming signal to produce the IF. Tuned to f_station + f_IF.

**Modulation Index (FM):** β = peak frequency deviation / highest audio frequency. Determines FM bandwidth and SNR improvement.

**Motorboating:** Low-frequency oscillation in an audio amplifier or receiver, usually caused by insufficient power supply decoupling. Sounds like "put-put-put."

**Mixer:** A circuit that multiplies two signals to produce their sum and difference frequencies. Implements the trigonometric identity cos(A)×cos(B) = ½cos(A-B) + ½cos(A+B).

**Noise Figure:** A measure of how much a stage degrades the signal-to-noise ratio. Expressed in dB. Lower is better.

**Oscillator:** An amplifier with frequency-selective positive feedback that sustains continuous oscillation without external input.

**Padding Capacitor:** A capacitor added in series with a variable capacitor to reduce its tuning range, used to achieve LO tracking in superheterodyne receivers.

**Phase Detector:** A circuit that outputs a voltage proportional to the phase difference between two signals. Used in PLLs.

**PLL (Phase-Locked Loop):** A feedback system containing a VCO, phase detector, and loop filter that locks the VCO frequency and phase to an input reference signal.

**Positive Feedback:** Feedback that reinforces the input signal. Increases gain and can cause oscillation. Opposite of negative feedback.

**Pre-emphasis / De-emphasis:** In FM transmission, high-frequency audio is boosted before transmission (pre-emphasis, 50 μs or 75 μs time constant) and cut after reception (de-emphasis) to improve high-frequency SNR.

**Q Factor (Quality Factor):** A dimensionless measure of the "sharpness" of a resonator. Q = f_0 / bandwidth = energy stored / energy dissipated per cycle. Higher Q = narrower bandwidth = lower loss.

**Ratio Detector:** An FM detector similar to Foster-Seeley but with AM rejection, achieved by adding a stabilising capacitor to hold the total detector voltage constant.

**Reactance:** The opposition of a capacitor or inductor to alternating current. Capacitive: X_C = 1/(2πfC). Inductive: X_L = 2πfL. Unit: ohms. Unlike resistance, reactance stores rather than dissipates energy.

**Regenerative Receiver:** A receiver that uses positive feedback (through a tickler coil) to increase the effective Q of the antenna circuit, dramatically improving sensitivity and selectivity.

**Resonance:** The condition where a circuit's natural frequency matches the driving frequency. For an LC circuit: f_0 = 1/(2π√(LC)). At resonance, impedance is at an extreme (maximum or minimum depending on series/parallel configuration).

**RF Choke (RFC):** An inductor designed to block RF current while passing DC. Used to keep RF signals out of power supply rails.

**SA612 / NE602:** An IC containing a Gilbert Cell double-balanced mixer and Colpitts oscillator. First "acceptable" IC for entry into radio design after mastering discrete stages.

**Selectivity:** A receiver's ability to separate wanted from unwanted signals at nearby frequencies. Measured as attenuation at ±10 kHz or ±20 kHz from the centre frequency.

**Sensitivity:** The minimum signal that a receiver can detect at a specified signal-to-noise ratio. Measured in microvolts (μV) or dBm.

**Sideband:** The frequency components of a modulated signal that lie above or below the carrier frequency. AM: ±audio frequency. FM: multiple sidebands (Bessel functions).

**Signal-to-Noise Ratio (SNR):** The ratio of signal power to noise power, usually expressed in dB. Higher SNR = clearer audio.

**Skywave / Skip:** Propagation of HF/MF radio waves by reflection off the ionosphere, enabling long-distance (500–20,000 km) communication.

**Slope Detector:** A simple FM detector that detunes the IF tank off resonance so the resonance slope converts frequency to amplitude, which is then envelope detected. Poor performance.

**SSB (Single Sideband):** A bandwidth-efficient form of AM where the carrier and one sideband are suppressed, transmitting only one sideband. Requires BFO in the receiver.

**Superheterodyne (Superhet):** A receiver architecture that translates the received signal to a fixed intermediate frequency (IF) before amplification and detection. Invented by Armstrong in 1918.

**Thermal Noise:** Random noise generated by thermal agitation of charge carriers in any resistive material. Power = kTB. Fundamental limit to receiver sensitivity.

**Tickler Coil:** A small secondary coil coupled to the tank circuit in a regenerative receiver, used to feed amplified signal back to the tank with positive phase (positive feedback).

**Transconductance (gm):** The ratio of change in collector current to change in base-emitter voltage: gm = ΔI_C / ΔV_BE = I_C / V_T. Unit: siemens (A/V). Determines transistor gain.

**TRF (Tuned Radio Frequency):** A receiver architecture that amplifies at the received frequency, requiring all stages to be re-tuned for each station. Replaced by superheterodyne.

**Varactor Diode:** A diode operated in reverse bias, whose junction capacitance varies predictably with applied voltage. Used in VCOs for frequency modulation and PLL tuning.

**VCO (Voltage-Controlled Oscillator):** An oscillator whose frequency is controlled by an applied voltage. Usually a Colpitts oscillator with a varactor diode in the tank circuit.

**Wavelength (λ):** The physical length of one cycle of an electromagnetic wave. λ = c/f where c = 3×10^8 m/s. At 1 MHz: λ = 300m. At 2.4 GHz: λ = 12.5 cm.

---

## 13. What to Record in Your Notebook

```
FM Receiver and Final Stage Log

FM detection method attempted:
[  ] Slope detector  —  date: ___ Audio quality rating: ___/5
[  ] Foster-Seeley   —  date: ___ Audio quality rating: ___/5
[  ] Ratio detector  —  date: ___ Audio quality rating: ___/5
[  ] PLL demodulator —  date: ___ Audio quality rating: ___/5

FM front end test:
LO frequency range: ___MHz to ___MHz
Stations received: ___
Best station (distance): ___km
Stereo pilot tone heard? YES / NO

BFO test (CW/SSB):
BFO frequency: ___kHz
CW received: YES / NO  (frequency: ___MHz)
SSB received: YES / NO  (frequency: ___MHz)

Progression summary — compare all receivers built:
| Receiver | Components | Min signal heard | Max range | Power |
|----------|------------|-----------------|-----------|-------|
| Crystal set | Coil, cap, diode | ___mV | ___km | 0 |
| Regenerative | 1 transistor | ___μV | ___km | ___mW |
| Superhet (Stage 9) | 7 transistors | ___μV | ___km | ___mW |
| FM (Stage 10) | ~10 transistors | ___μV | ___km | ___mW |

Time invested in radio stages (honest estimate): ___hours

Concepts still unclear — write them here for future research:
___________

Connection to ESP32 drone project:
- RC link (2.4 GHz): uses _______________ architecture
- WiFi telemetry: uses _______________ architecture  
- GPS (1575 MHz): uses _______________ architecture
```

---

## 14. Troubleshooting Table — FM and BFO

| Symptom | Likely Cause | Remedy |
|---------|-------------|--------|
| No FM audio from slope detector | IF detuned too far, or wrong direction | Adjust slug ±1 turn each way, check for audio |
| FM audio but very distorted | Slope detector working but non-linear — normal | Try Foster-Seeley for better quality |
| Foster-Seeley output offset large DC | Diodes not matched OR R1 ≠ R2 OR C1 ≠ C2 | Measure diode forward voltage; use matched pairs |
| Foster-Seeley no output | Transformer coupling wrong | Check C_link capacitor. Verify primary centre tap connection |
| BFO pitch too high or too low | BFO frequency too far from IF | Adjust BFO trimmer capacitor |
| BFO causes severe distortion on AM | BFO coupling too strong | Reduce C_inject from 5pF to 1pF |
| CW audible but SSB sounds like Donald Duck | BFO frequency slightly wrong | Fine-tune BFO trimmer — must be within ±200Hz for SSB |
| FM stereo not stereo (both channels same) | Pilot tone too weak or stereo decoder not locking | Improve signal level; check 19kHz pilot with oscilloscope |
| Entire FM front end oscillates / squeals | VHF layout too loose — stray feedback | Keep all VHF leads under 5mm. Shield LO from RF section |

---

## 15. Key Insight — The Full Picture

You began Stage 1 by watching a spark jump a gap — an invisible electromagnetic disturbance propagating at the speed of light. You learned that this disturbance could be structured to carry information.

Across ten stages, you have built every functional block of a complete radio receiver and transmitter from first principles, using only the most basic components: wire, coils, capacitors, resistors, diodes, and transistors.

The path from spark to smartphone radio is a straight line of engineering refinements:
- The crystal set → regenerative receiver → TRF → superheterodyne. Each step improved sensitivity and selectivity while reducing the engineer's required skill.
- The vacuum tube → transistor → integrated circuit → system-on-chip. Each step shrunk the circuit, reduced power, and reduced cost. The physics did not change.
- The AM detection → FM detection → digital demodulation. Each step improved noise immunity. The underlying mathematics (Fourier transforms, trigonometric identities, Barkhausen criterion) did not change.

When your drone's ESP32 communicates at 2.4 GHz, it is running an LC oscillator (on-chip transmission line sections instead of coils), a mixer (CMOS switches implementing the same multiplication), and an IF filter (digital FIR filter implementing the same cos(ω_IF × t) frequency selectivity). Every principle is present. Every mathematical identity applies. The chip is smaller than your fingernail. The circuit is larger than a table. The physics is identical.

The world runs on radio. Every device that communicates without wires — your phone, your drone, your WiFi router, your GPS, the satellite thousands of kilometres above you — uses what you have built in these ten stages. Not as a historical curiosity. As living, active, fundamental technology.

You now understand it from first principles. That understanding does not expire.

---

*End of the Radio Fundamentals sequence. Proceed to the practical drone radio link modules: RC receiver implementation, telemetry design, GPS front-end integration.*
