# Stage 6 — The Regenerative Receiver: One Transistor, Maximum Sensitivity

---

## What You Will Learn

- Why the crystal set stops working beyond a few tens of kilometres
- What positive feedback is and how it is fundamentally different from negative feedback
- How Edwin Armstrong's 1912 insight turned a lossy tank circuit into a near-perfect filter
- The mathematics of regeneration and effective Q — derived step by step
- How to wind a tickler coil, why the direction matters, and how to test it
- Three operating regions of the regenerative receiver and what each one sounds and feels like
- How to build a complete single-transistor regenerative receiver for the Medium Wave band
- What stations you can expect to receive and how this compares to your crystal set

---

## 1. Why the Crystal Set Runs Out of Sensitivity

In Stage 2 you built a crystal set. It works. But it only works well for strong local stations. Why?

Two fundamental limits:

**Limit 1: No gain.** The crystal set has exactly one active signal path: antenna → tank circuit → diode detector → earphone. The diode does not amplify. It only rectifies. If the antenna delivers 10 microvolts of signal at 1 MHz, the earphone gets at most a few microvolts of audio. Distant stations deliver less than 1 microvolt. That is below the forward voltage of even the best germanium diode. The diode simply does not conduct at those signal levels and you hear nothing.

**Limit 2: Real coils have loss.** Your tank circuit coil has resistance — every metre of wire has resistance. A typical ferrite rod antenna coil might have 10 ohms of DC resistance. At resonance the tank circuit stores energy in the magnetic and electric fields, but the coil resistance dissipates some of that energy every cycle. The ratio of energy stored to energy dissipated per cycle is Q (Quality factor).

The Q of your crystal set coil sets the selectivity (how narrow the passband is) and also sets the effective "signal voltage multiplication" at resonance. A coil with Q = 100 at 1 MHz means the voltage across the tuned circuit is 100 times the voltage at the antenna terminals. That sounds useful, but once you add the diode load, the loading of the earphone, and the radiation resistance of a short antenna — the effective Q collapses to perhaps 20 or 30. The selectivity is poor and the voltage gain is modest.

The signal from a station 500 km away might be 0.5 microvolts at the antenna. After the tank circuit (effective Q = 25) the signal is 12.5 microvolts. A 1N34A germanium diode has a threshold around 150–200 mV. You are 10,000 times too small. Nothing.

**What we need:** gain before the detector, and a way to increase the effective Q of the tank circuit without using expensive high-Q coils. Armstrong gave us both with one idea.

---

## 2. Positive Feedback vs Negative Feedback

You have already met negative feedback in Stage 5 (the emitter resistor in a transistor amplifier). Let us be precise about what feedback means.

**Feedback** means taking a sample of the output of a system and adding it back to the input.

**Negative feedback:** the sample is added back in anti-phase — it opposes the original signal. Effect: gain is reduced, bandwidth increases, distortion is reduced, input impedance changes. Every good audio amplifier uses negative feedback. The emitter resistor in your BC547 amplifier is negative feedback.

**Positive feedback:** the sample is added back in phase — it reinforces the original signal. Effect: gain increases. If loop gain reaches exactly 1, the system sustains oscillation indefinitely without any input. If loop gain exceeds 1, the amplitude grows until clipping or some other non-linearity limits it.

Think of it physically:
- Negative feedback: a thermostat. Room gets too hot → heater turns off. Stabilising.
- Positive feedback: a microphone in front of a loudspeaker. The speaker plays the microphone signal, the microphone picks it up louder, the speaker plays it louder — the screech is positive feedback gone all the way to oscillation.

Armstrong's insight: positive feedback, carefully controlled to stay just below the oscillation threshold, dramatically increases gain AND selectivity simultaneously.

---

## 3. Armstrong's Regeneration Principle (1912)

Edwin Armstrong, a 21-year-old Columbia University student, was experimenting with the triode vacuum tube (the transistor of its day). He noticed that when he added a small coil near the plate (output) of the triode and brought it near the grid (input) coil, the signal got enormously stronger. With careful adjustment, he could increase the gain to hundreds of thousands before the circuit started to oscillate.

The mechanism is this:

Your tank circuit (inductor + capacitor) is resonant at frequency f. Every cycle, the coil resistance dissipates some energy. This loss is what limits Q. But what if you could pump a tiny bit of energy back into the tank circuit on every cycle — exactly in phase with the oscillation already there? You would be partially compensating for the resistance loss. The effective resistance of the coil decreases. Effective Q increases.

The transistor amplifies the signal in the tank. The amplified signal is fed back into the tank through a small coupling coil (the tickler coil) wound on the same former as the main coil. If the phase is correct (positive feedback), the fed-back signal reinforces the oscillation already in the tank. The tank "sees" less net resistance. Its Q rises.

As you increase the feedback (by coupling the tickler coil more tightly), the effective Q rises and rises. At a certain coupling, the energy pumped back in exactly equals the energy lost per cycle. Net resistance = zero. Q = infinity. This is the oscillation threshold.

---

## 4. Mathematical Treatment of Regeneration

This is the most important derivation in this module. Follow it step by step.

### 4.1 The Tank Circuit Quality Factor

For a series RLC circuit (or its parallel equivalent), Q is defined as:

```
        omega_0 × L       1
Q  =   -----------  =  -------
            R          omega_0 × R × C
```

Where:
- omega_0 = 2π × f_0 (angular resonant frequency)
- L = inductance of the coil
- R = series resistance of the coil (the loss)
- C = capacitance

Higher Q means:
- Narrower bandwidth: BW = f_0 / Q  (Hz)
- Higher voltage across the tank at resonance: V_tank = Q × V_input
- Slower ring-down if you flick it with a pulse

For a typical MW ferrite rod coil:
- L = 250 microhenries
- R = 15 ohms (wire resistance)
- f_0 = 1 MHz

omega_0 = 2π × 10^6 = 6.28 × 10^6 rad/s

```
Q = (6.28 × 10^6 × 250 × 10^-6) / 15 = 1570 / 15 = 105
```

But when you add the diode detector and earphone load, the effective parallel resistance of the tank drops enormously. Q_effective might be only 20–40.

### 4.2 The Effect of Regeneration on Effective Resistance

Let the tickler coil feed back a fraction β of the output voltage. The transistor amplifier has voltage gain A (negative, because common-emitter is inverting — but the tickler reversal restores phase so net feedback is positive).

The effective loop gain for the regeneration path is:

```
G_loop = β × |A|
```

Each time the tank circuit signal goes around the loop, it comes back multiplied by G_loop. If G_loop < 1, the circuit is stable. The tank signal decays but slowly — the Q is enhanced.

The effective resistance seen by the tank is reduced by the regeneration. Working through the algebra (which requires phasor analysis but I will give you the result):

```
R_effective = R_coil × (1 - G_loop)
```

Therefore the effective Q is:

```
Q_eff = Q_coil / (1 - G_loop)
```

This is the fundamental regeneration equation. Let us examine what it tells us:

| G_loop | Q_eff / Q_coil |
|--------|----------------|
| 0.0    | 1.0  (no regeneration, normal Q) |
| 0.5    | 2.0  |
| 0.9    | 10.0 |
| 0.99   | 100.0 |
| 0.999  | 1000.0 |
| 1.0    | infinity — the oscillation threshold |
| >1.0   | oscillating — generates its own signal |

Starting with Q_coil = 50 (loaded value), at G_loop = 0.99 you get Q_eff = 5000.

Bandwidth = f_0 / Q_eff = 1,000,000 / 5000 = **200 Hz**

That is an extraordinary selectivity. A broadcast AM station occupies ±5 kHz (10 kHz total). With Q_eff = 5000, the 3-dB bandwidth is only 200 Hz. You can resolve individual stations that are very close in frequency. You can even selectively listen to the upper or lower sideband.

### 4.3 Voltage Gain Enhancement

The voltage appearing across the tank at resonance is:

```
V_tank = Q_eff × V_antenna
```

With Q_eff = 5000 and a 1-microvolt signal at the antenna:

```
V_tank = 5000 × 1 microvolt = 5 millivolts
```

Five millivolts is well above the threshold of a germanium diode detector (typically 0.1–0.2 mV for audio detection). So a properly set regenerative receiver can detect signals that are **100 times smaller** than a crystal set can detect. That translates to stations that are roughly 10 times further away (signal strength falls as 1/r^2, so 100× better sensitivity → roughly 10× the distance).

---

## 5. The Tickler Coil

### 5.1 What It Is

The tickler coil is a small secondary winding on the same former as the main tank coil. It is connected to the collector (output) of the transistor. The signal at the collector, amplified, is coupled magnetically into the main tank coil.

### 5.2 Coupling Coefficient

The coupling coefficient k describes how tightly two coils interact:

```
k = M / sqrt(L1 × L2)
```

Where M is mutual inductance. k ranges from 0 (no coupling) to 1 (perfect coupling).

For a tickler wound on the same former as the main coil:
- If wound far apart: k ≈ 0.1 to 0.3
- If wound directly over the main coil: k ≈ 0.5 to 0.8
- If wound bifilar (twisted together): k ≈ 0.9 to 0.99

For the regenerative receiver, you want k adjustable — or you want a variable element (variable capacitor or resistor) in the feedback path to control the loop gain from below 1 to just above 1.

### 5.3 Why Winding Direction Matters

This is critical and is where most beginners' first attempts fail.

The transistor in common-emitter configuration inverts the signal: a positive-going signal at the base produces a negative-going signal at the collector (180 degree phase shift).

For positive feedback (regeneration), you need the signal returned to the tank to be in phase with the original signal. The tickler coil must therefore introduce another 180 degree shift to cancel the transistor's inversion. Whether this happens depends on which way you wind the tickler coil relative to the main coil.

There is no formula to predict which direction is correct without knowing your specific coil geometry. You must test it. The procedure is:

1. Wind the tickler coil as described below.
2. Connect the circuit.
3. Advance the regeneration control. If the receiver's signal gets louder and more sensitive — you have positive feedback. Correct.
4. If advancing the regeneration control makes the signal get quieter — you have negative feedback. The tickler polarity is wrong. Reverse either the tickler connections OR rewind the tickler in the opposite rotational direction.

**A simple test before connecting:** Wind the tickler. Touch the free ends to a 1.5V battery momentarily while watching the main coil on a compass needle or observing a nearby AM radio. Or just connect the circuit and try — it costs nothing to reverse the two tickler wires.

### 5.4 How to Wind the Tickler Coil

For the MW band receiver coil:

**Main coil:**
- 60 turns of 26 AWG (0.4 mm) enamelled copper wire
- Wound on a 10 mm diameter former (or a ferrite rod, 10 mm diameter, 100 mm long)
- Use a plastic former or a cardboard tube from kitchen roll — do NOT use a metal former
- Wind as a single layer, tightly butted turns, over 3 cm of the former
- Secure ends with a small piece of tape

**Tickler coil:**
- 15 turns of the same wire
- Wound directly over one end of the main winding — approximately over the last 15 turns of the main winding
- Wind in the SAME rotational direction as the main winding (start at the same end)
- Note: this may or may not produce the correct phase — you will check as described above
- Twist the tickler wires together (to prevent them picking up stray signals) and run them to the circuit

**Alternative — separate section:**
- Wind the tickler at the opposite end of the ferrite rod from the main coil
- Overlap a small amount
- Sliding the ferrite rod (or moving the tickler section) then varies the coupling
- This is the "variable coupling" method of regeneration control

---

## 6. The Regeneration Control

You must control how much feedback is applied. Too little and you get only modest enhancement. Too much and the circuit oscillates, generating its own signal (useful for CW reception but it radiates and disturbs nearby radios).

There are three common control methods:

### Method 1: Variable Resistor in Collector Circuit

A potentiometer in series between the collector and the tickler coil adjusts how much of the collector signal reaches the tickler. This is the simplest approach for a beginner.

```
Collector → [10kΩ pot, wiper to circuit] → Tickler coil → Ground
                     ↑
            Regeneration control
```

Turn the pot towards the collector end: more signal → more feedback → more regeneration.
Turn towards ground: less signal → less feedback → less regeneration.

**Problem:** The potentiometer's resistance, reflected through the coupling, also loads the tank circuit slightly. The Q changes with the regeneration setting. Not ideal but works for a first build.

### Method 2: Variable Capacitor in Feedback Path

A small variable capacitor (10–100 pF range) in series with the tickler circuit. At low capacitance, the series impedance is high and little signal passes. At maximum capacitance the impedance is lower and more signal passes.

This is less loading on the tank and gives a smoother control. Use a small trimmer capacitor and replace it with a proper variable cap once you find the required range.

### Method 3: Variable Coupling (Move the Tickler)

If the tickler is wound on a separate sleeve that can slide along the ferrite rod, moving it closer to the main coil increases coupling and therefore feedback. Moving it away decreases coupling.

This is mechanically elegant and is how many vintage receivers were designed. It requires a bit of construction skill. Build Method 1 first.

---

## 7. Three Operating Regions

This section is crucial. Spend time at each region. They behave very differently.

### Region 1: Below Threshold (G_loop < 1)

**How to get here:** Start with the regeneration control at minimum. Slowly advance it.

**What you hear:** Stations that were inaudible on the crystal set become audible. As you advance the control, stations get louder. The audio quality improves — background hash (noise) decreases relative to the signal. High frequency audio (the "crispness" of voice) becomes more distinct because the bandwidth is still wide enough to pass 5 kHz of audio sidebands.

**What is happening:** Q_eff is increasing. The antenna signal is being amplified before detection. The signal-to-noise ratio improves because the resonant circuit is rejecting off-frequency noise.

**Bandwidth:** At moderate regeneration (G_loop = 0.9, Q_eff = 500 on a Q_coil = 50 coil), bandwidth = 1 MHz / 500 = 2 kHz. Still too narrow for full audio fidelity (you want 10 kHz for full AM bandwidth) but much better than the crystal set.

**Usage:** This region gives you the best combination of sensitivity and audio quality for listening to speech and music on MW AM stations.

### Region 2: Near Threshold (G_loop approaching 1)

**How to get here:** Continue advancing the control past Region 1. There will be a point where you hear a slight rushing or hissing sound, or a faint heterodyne squeal on some stations.

**What you hear:** Signals become almost uncomfortably strong — loud with the slightest antenna. Audio quality degrades: the bandwidth has narrowed so much that bass is fine but treble gets cut (10 kHz sideband → 200 Hz bandwidth → you lose audio above 100 Hz). Voice sounds "hollow" or "underwater."

**Critical behaviour:** In this region the circuit is extremely sensitive to hand capacity. Moving your hand near the circuit shifts the frequency slightly (your body is a large capacitor to ground). This is called the "hand effect" and is a characteristic problem of regenerative receivers.

**Usage:** Maximum sensitivity. Use this region to find weak stations or DX (long-distance reception). Once found, back off slightly to Region 1 for comfortable listening.

**Important:** Stay just below threshold. Do not push over into Region 3 unless you want CW reception.

### Region 3: Above Threshold (G_loop > 1) — Oscillating

**How to get here:** Advance the regeneration control beyond the threshold. There is a definite click-over point where the circuit starts generating its own signal.

**What you hear:**
- On an AM radio nearby: you will hear a carrier appear — you have become a transmitter! (Low power, but real.)
- In your headphones: you hear a loud heterodyne whistle. Strong AM stations beat against your oscillation frequency to produce an audio tone.
- By tuning across the band in this mode: when the oscillation frequency crosses an AM carrier, you hear the characteristic zero-beat (the tone descends to zero as you pass through the carrier frequency).

**What is happening:** The tank circuit is now a self-sustaining oscillator (exactly like the Colpitts oscillator you will build in Stage 7). Any incoming signal at a nearby frequency mixes with your oscillation to produce a beat note at the difference frequency. This is heterodyne reception.

**Usage for CW (Morse Code):** A Morse code signal is just a carrier switched on and off. It has no audio content by itself. But if your receiver oscillates 1 kHz away from the Morse carrier, you hear a 1 kHz tone appearing and disappearing with the dots and dashes. This is how Morse was received for decades.

**Usage for SSB (Single Sideband):** An SSB signal is a suppressed-carrier AM signal. It sounds like Donald Duck if detected with an AM detector. But if your oscillating receiver's frequency is set exactly at the suppressed carrier frequency, it reinserts the carrier and the SSB signal becomes intelligible speech.

**Hazard:** In oscillating mode, your receiver is also a transmitter. It is radiating on the frequency it is oscillating on. This can cause interference to nearby radios. Particularly in apartment buildings. In the UK and most countries, intentional radiation on MW/HF requires a licence. Short experiments are generally tolerated; prolonged operation is not. A small shield or keeping the antenna short (10 cm) reduces the radiated power to negligible levels.

---

## 8. Full Circuit Schematic

This is the complete circuit. Build it exactly as shown before experimenting with variations.

```
         +9V
          |
          R1 (68kΩ)
          |
          +----------+-----> Base (B) of Q1 (BC547)
          |          |
         R2          C_in (10nF ceramic)
        (15kΩ)       |
          |          +---[Main tank coil L1]---+
          |          |                         |
         GND        VC (variable, 10-400pF)   GND
                                              
         +9V
          |
         RC (4.7kΩ)
          |
          +----------> Collector (C) of Q1
          |
         C_RF (10nF)  -- bypass, from collector to GND (keeps RF out of supply)
          |
         [Tickler coil L2, 15 turns]
          |
         [REGEN pot 10kΩ]
          |
         GND
          
         Q1 Emitter (E) --> RE (470Ω) --> GND
                       --> CE (100μF electrolytic) across RE (emitter bypass)
          
         Audio output: from Collector of Q1 through C_audio (100nF) to headphones or audio amp
         
         Headphones: 32-ohm type between audio output and GND
                     OR crystal earphone (high impedance) directly
```

**Full annotated schematic:**

```
         +9V rail
          |
    [R1 = 68kΩ] <--- voltage divider bias top
          |
          +---[C_in = 10nF]---+--- B (base) Q1
          |                   |
    [R2 = 15kΩ]          [L1 main coil]
          |                60 turns on ferrite
         GND                   |
                          [VC = 10-400pF variable]
                               |
                              GND
                              
    +9V rail
      |
   [RC = 4.7kΩ]
      |
      +-------------- C (collector) Q1 --------[C_RF_bypass = 10nF]--- GND
      |
   [L2 tickler, 15T]
      |
   [REGEN pot, 10kΩ, wiper to above, one end to collector, other to GND]
   
   Actually correct configuration:
   
   Collector --[L2 tickler 15T]--+--[REGEN pot 10kΩ]-- GND
                                 |
                               this junction to collector
                               side of pot via the tickler

   Simpler description: The tickler coil is in series between collector and the
   wiper of the REGEN pot. The other end of the pot goes to GND.
   
   Collector --> [L2 tickler] --> [REGEN 10kΩ wiper] --> GND
                                  (wiper at centre = half feedback)
                                  
   Q1 emitter:
   E --- [RE = 470Ω] --- GND
   E --- [CE = 100μF, 16V] --- GND   (bypass cap across RE)
   
   Audio output:
   Collector --- [C_audio = 100nF] --- [Headphone jack] --- GND
   
   Power supply decoupling:
   +9V rail --- [C_supply1 = 100μF, 16V] --- GND  (near battery connection)
   +9V rail --- [C_supply2 = 100nF ceramic] --- GND  (near Q1 VCC connection)
```

### Component List (complete)

| Ref | Value | Type | Notes |
|-----|-------|------|-------|
| Q1 | BC547 or 2N2222 | NPN transistor | |
| R1 | 68kΩ | 0.25W resistor | Bias divider top |
| R2 | 15kΩ | 0.25W resistor | Bias divider bottom |
| RC | 4.7kΩ | 0.25W resistor | Collector load |
| RE | 470Ω | 0.25W resistor | Emitter degeneration |
| REGEN | 10kΩ | Linear pot | Regeneration control |
| C_in | 10nF | Ceramic | RF coupling to base |
| CE | 100μF | Electrolytic, 16V | Emitter bypass |
| C_RF | 10nF | Ceramic | Collector RF bypass |
| C_audio | 100nF | Ceramic or film | Audio output coupling |
| C_supply1 | 100μF | Electrolytic, 16V | Power supply decoupling |
| C_supply2 | 100nF | Ceramic | Power supply HF decoupling |
| VC | 10-400pF | Variable air-spaced | Tuning capacitor |
| L1 | 60T 26AWG | Main tank coil | On 10mm ferrite rod |
| L2 | 15T 26AWG | Tickler coil | Over end of L1 |
| Earphone | 32Ω or crystal | | Headphones or crystal earphone |

### Bias Point Calculation

Q1 (BC547) with RC = 4.7kΩ, RE = 470Ω on 9V supply:

We want the collector sitting at roughly half the supply voltage (4.5V) for maximum headroom.

```
If V_collector = 4.5V:
I_C = (V_CC - V_C) / RC = (9 - 4.5) / 4700 = 4.5 / 4700 = 0.96 mA ≈ 1 mA

V_E = I_C × RE = 0.001 × 470 = 0.47V

V_B = V_E + V_BE = 0.47 + 0.65 = 1.12V

Voltage divider R1/R2 must give V_B = 1.12V from 9V:
V_B = 9V × R2 / (R1 + R2)
1.12 = 9 × R2 / (R1 + R2)

If R2 = 15kΩ: R1 = R2 × (9/1.12 - 1) = 15000 × 7.036 = 105kΩ

Hmm, 105kΩ is a bit high. Let us check with R1 = 68kΩ:
V_B = 9 × 15000 / (68000 + 15000) = 135000 / 83000 = 1.63V
V_E = 1.63 - 0.65 = 0.98V
I_C ≈ 0.98 / 470 = 2.1 mA
V_C = 9 - (2.1mA × 4700) = 9 - 9.87 = negative? Problem.

Let us recalculate with RC = 4.7kΩ and I_C = 1 mA (target):
V_C = 9 - 1mA × 4.7kΩ = 9 - 4.7 = 4.3V  ✓ (good, near half of 9V)

So use smaller divider current. Try R1 = 100kΩ, R2 = 22kΩ:
V_B = 9 × 22000 / 122000 = 1.62V → I_C too high.

Best approach: use R1 = 150kΩ, R2 = 27kΩ:
V_B = 9 × 27/(150+27) = 9 × 27/177 = 1.37V
V_E = 1.37 - 0.65 = 0.72V
I_C = 0.72/470 = 1.53 mA
V_C = 9 - 1.53 × 4.7 = 9 - 7.2 = 1.8V  (too low)

Use RC = 2.2kΩ:
V_C = 9 - 1.53 × 2.2 = 9 - 3.37 = 5.63V  ✓
```

**Revised component values (corrected bias):**

| Ref | Value |
|-----|-------|
| R1 | 150kΩ |
| R2 | 27kΩ |
| RC | 2.2kΩ |
| RE | 470Ω |

Re-check: V_B = 9 × 27/177 = 1.37V, V_E = 0.72V, I_C = 1.5mA, V_C = 5.7V. Excellent.

The 2.2kΩ collector resistor also means the RF gain is higher (higher Rc = higher voltage gain in common-emitter). For this particular circuit, higher gain is what we want.

---

## 9. Complete Coil Winding Guide

### What You Need

- One ferrite rod, approximately 10 mm diameter, 100 mm long (from a broken AM transistor radio, or purchased. Type: mix 61 or mix 43 for MW)
- 1 metre of 26 AWG (0.4 mm diameter) enamelled copper wire — get the type with solderable enamel (bonds wire)
- A short length of heat-shrink tubing or masking tape
- A thin plastic sleeve (from a ballpoint pen body or similar) to wind on

### Main Coil (L1) — 60 turns

1. Measure 1.5 cm from one end of the ferrite rod. Mark with a pen.
2. Cut a strip of masking tape, wrap it around the rod at the 1.5 cm mark to make a start anchor.
3. Anchor your wire end at the tape.
4. Wind turns tightly and evenly, working from left to right (or right to left — pick one direction and keep it).
5. Count every 10 turns. At turn 60 you should have covered approximately 3 cm.
6. Anchor the finish end with tape.
7. Leave 5 cm of wire free at each end as leads.
8. Scrape the enamel off the last 5 mm of each lead with a craft knife. Tin with solder.
9. Label these leads L1a (start) and L1b (finish). Note which end is which.

### Tickler Coil (L2) — 15 turns

1. Start at the L1b end (finish end of the main coil).
2. Wind directly over the last 15 turns of L1.
3. Wind in the SAME rotational direction as L1.
4. Use the same wire gauge.
5. Leave 5 cm leads, scrape and tin.
6. Label L2a (start, at top of main coil) and L2b (finish).

**Important note on direction:** "Same rotational direction" means: if you look at the coil from one end and wound L1 clockwise — wind L2 also clockwise. The coil start (L2a) should be at the same end as L1b.

### Testing Before Connecting

Use a multimeter on resistance range:
- L1: should read approximately 8–15 ohms. If open circuit, the enamel is not fully scraped.
- L2: should read approximately 2–4 ohms.
- L1 to L2: should be open circuit (infinite resistance). If shorted, the enamel between the layers has been damaged. Rewind.

Optional: measure inductance with an LC meter if you have one.
- L1 at 1 MHz should be approximately 200–300 μH.
- Resonant capacitance to hit 1 MHz: C = 1/(4π²f²L) = 1/(4π² × 10^12 × 250×10^-6) = 101 pF. This should be within your variable cap range.

---

## 10. Step-by-Step Build Sequence

### Step 1: Assemble the bias network

Place Q1 on the breadboard. Wire R1 from the +9V rail to the base. Wire R2 from the base to ground. Wire RE from the emitter to ground. Wire CE across RE. Apply power (no antenna connected). Measure:
- V_B should be approximately 1.3–1.5V
- V_E should be approximately 0.65–0.85V
- V_C should be approximately 5–6V

If V_C is near 9V: transistor is not conducting. Check R1, R2 values.
If V_C is near 0V: transistor is saturated. Check R1 value (too low).

### Step 2: Add the tank circuit

Connect L1 between the base and ground (via C_in coupling cap). Connect VC (variable cap) across L1. Do not connect the antenna yet.

### Step 3: Add the collector circuit

Connect RC from +9V to collector. Add C_RF_bypass from collector to ground. Connect C_audio from collector to earphone jack. Connect one earphone terminal to ground.

### Step 4: Add the tickler

Connect one end of L2 to the collector (after RC). Connect the other end of L2 to one end of the REGEN pot (10kΩ). Connect the wiper of the REGEN pot to the same junction. Connect the other end of the pot to ground.

Wait — this configuration does not work as written. The correct wiring:

The REGEN pot controls how much DC path (and RF path) there is through the tickler. Better configuration:

```
Collector ---[L2 tickler]--- [REGEN pot, full 10kΩ] --- GND
                             (wiper connects to GND side of pot)
```

Actually the simplest and most reliable method: put a 100nF coupling cap in series with L2, then the REGEN pot between that junction and GND. The cap stops DC flowing through the tickler.

```
Collector ---[100nF cap]--- [L2 tickler 15T] ---+--- GND
                                                |
                                          [REGEN pot 10kΩ, 
                                           between this point and GND]
```

As the pot resistance decreases (wiper moves toward GND end), more of the collector signal flows through L2, increasing coupling to L1.

### Step 5: Connect a short antenna

A 1-metre wire laid on the bench is sufficient to start. Connect it to the base through C_in.

### Step 6: Power up and tune

Set REGEN pot to minimum (clockwise if wiper moves toward high-resistance end).
Tune VC slowly across its range. You should hear at least one MW station.
If you hear nothing: check the tuning range (use an AM radio to verify your area has MW stations in the 530–1600 kHz range that your coil covers).

---

## 11. Tuning Procedure

This procedure lets you find stations and work the regeneration correctly.

1. **Set regeneration to zero.** Turn REGEN pot fully counter-clockwise (minimum feedback). The receiver should work like a crystal set at this point.

2. **Tune VC across its full range.** You should hear at least the strongest local station. This confirms the circuit is alive.

3. **Advance REGEN slowly.** Turn it clockwise, 1/8 turn at a time. After each increment, tune VC a little. You will find:
   - The station gets louder.
   - Background noise increases less than signal (signal-to-noise improves).
   - The tuning becomes sharper — you have to tune more precisely to hit the station.

4. **Watch for the threshold.** The threshold announces itself:
   - A faint rushing sound when you are near a station.
   - The station may seem to "pull" the tuning — this is the oscillator pulling effect.
   - If you advance further: a loud squeal breaks out.

5. **Back off.** When you hit the squeal, back off the REGEN pot by 1/8 turn. You are now just below threshold.

6. **This is your operating point.** Stay here for maximum sensitivity. Re-tune VC carefully for peak signal. The tuning is very sharp now — ±1 pF changes on the variable cap shift the frequency by tens of kHz at this Q level.

7. **For CW/SSB:** Advance past threshold. Tune until the squealing resolves into a steady tone on a station. For CW (Morse), the tone will appear and disappear with the dots and dashes.

---

## 12. What You Can Receive

### 12.1 MW AM Broadcast (530–1700 kHz)

This is the primary use of the receiver as built. Expected performance:
- **Local stations (< 50 km):** Loud and clear even at low regeneration.
- **Regional stations (50–300 km):** Audible at medium regeneration.
- **Night-time DX (300–3000 km):** Possible at near-threshold regeneration during evening and night hours (November–March best in northern hemisphere).

**Why night-time?** The ionosphere has two relevant layers for MW:
- The D layer (60–90 km altitude) absorbs MW during daytime. It dissipates at night.
- The E layer (100 km) reflects MW after dark.
- At night, MW signals from European stations bounce off the E layer and can travel 2000 km. This is called skywave or skip propagation.

### 12.2 Changing the Coil for Other Bands

Replace L1 and L2 with different coils:

**80-metre amateur band (3.5–4.0 MHz):**
- L1: 30 turns on same ferrite rod
- L2: 8 turns
- VC: 10–200 pF range (smaller than MW)
- Listen for amateur CW (Morse) and SSB voice stations

**40-metre amateur band (7.0–7.3 MHz):**
- L1: 18 turns on 10mm former (air core or ferrite)
- L2: 5 turns
- VC: 10–100 pF range

At these frequencies the hand effect is severe. Mount the coil and tuning cap inside a small cardboard box and use a long extension rod on the tuning knob.

**Coil formula** (approximate, for air core):
```
L (μH) ≈ (r² × N²) / (9r + 10l)
```
Where r = coil radius in inches, N = turns, l = coil length in inches.

For 7 MHz, you need L ≈ 2 μH with C = 150 pF:
```
f = 1/(2π√(LC)) = 1/(2π√(2×10^-6 × 150×10^-12))
  = 1/(2π × √(3×10^-16))
  = 1/(2π × 1.73×10^-8)
  = 1/(1.087×10^-7)
  = 9.2 MHz  (a bit high — try 3 μH)
```

With L = 3 μH and C = 150 pF: f = 7.5 MHz. Close enough for the 40m band with tuning capacitor trimming.

---

## 13. Crystal Set vs Regenerative: Comparison Table

| Parameter | Crystal Set | Regenerative (below threshold) | Regenerative (near threshold) |
|-----------|-------------|-------------------------------|-------------------------------|
| Components | Diode, LC | 1 transistor, LC, tickler | Same |
| Gain before detector | 0 dB | ~20–30 dB | ~50–60 dB |
| Effective Q (typical) | 20–50 | 200–500 | 1000–5000 |
| 3-dB bandwidth at 1 MHz | 20–50 kHz | 2–5 kHz | 200 Hz–1 kHz |
| Minimum detectable signal | ~1 mV at antenna | ~10 μV | ~0.1–1 μV |
| Typical local station range | 30 km | 200 km | 500 km |
| Night-time DX range | 100 km | 500 km | 2000+ km |
| Audio quality | Fair | Good | Bass-heavy (narrow bandwidth) |
| Stability | Perfect | Good below 50% regen | Sensitive to vibration, hand |
| Power required | Zero | 9V × 1.5mA = 13.5 mW | Same |
| Interference to others | None | Slight if oscillating | Noticeable if oscillating |

---

## 14. Why Regenerative Receivers Were Replaced

By 1930, the regenerative receiver was being superseded by the superheterodyne (superhet) receiver. Why?

**Problem 1: Instability near threshold.** The operating point (just below oscillation) is inherently unstable. Temperature changes, vibration, the operator's hand near the circuit — all shift the operating point. An automatic radio in a car bouncing over cobblestones would be unusable.

**Problem 2: Radiation when oscillating.** When operating above threshold for CW/SSB reception, the receiver is a transmitter. In a city with many receivers, every regen receiver tuned to the same station would cause mutual interference. Regulators in the 1930s effectively banned oscillating receivers in many countries.

**Problem 3: Tracking difficulty.** To cover the entire MW band, you tune the variable capacitor. But as you tune, the regeneration control needs readjustment because the coupling coefficient between tickler and tank changes with the tank circuit's resonant impedance. Every station required rebalancing.

**Problem 4: Selectivity vs audio quality trade-off.** As shown in the comparison table, the near-threshold selectivity (200 Hz bandwidth) destroys audio quality. For comfortable music listening you need ~10 kHz bandwidth — but then you lose the selectivity benefit.

The superheterodyne (Stages 8 and 9) solved all four problems by amplifying at a fixed lower intermediate frequency where stable, high-Q, fixed filters could be used.

---

## 15. The Modern Descendant

The phase-locked loop (PLL) receiver — used in every FM radio, every TV, every WiFi chip — is, at a conceptual level, a self-stabilising regenerative detector.

In a PLL:
- A voltage-controlled oscillator (VCO) is the equivalent of the regenerative receiver's oscillating tank.
- A phase detector compares the VCO output with the incoming signal.
- A loop filter smooths the comparison.
- The loop filter output drives the VCO — this is the "regeneration control" adjusted automatically.

The difference: in the PLL, the loop is always closed, always lock tracking the incoming signal. The gain never accidentally goes too high because the control loop corrects it 1000 times per second. What Armstrong had to do by hand with a knob, a PLL does automatically with transistors and a loop filter.

When you buy an FM radio IC or a WiFi chip, there is a PLL inside. It is Armstrong's regeneration with automatic gain control added.

---

## 16. Troubleshooting

| Symptom | Likely Cause | Remedy |
|---------|-------------|--------|
| No signal at all, even local stations | Bias wrong — V_C not near 4–6V | Measure V_C. If near 0V: RC too low or transistor shorted. If near 9V: transistor not conducting, check R1/R2. |
| Station heard but advancing REGEN makes it quieter | Tickler polarity wrong — negative feedback | Reverse L2 connections (swap L2a and L2b) |
| Squealing immediately when REGEN is advanced even slightly | Tickler coupling too tight | Fewer turns on L2, or separate L2 further from L1 |
| Will not oscillate even at maximum REGEN | Tickler coupling too loose | More turns on L2, or wind closer to L1. Check gain: V_C measured with meter should be ~5V; if very low, gain is low. |
| Motorboating (low-frequency pulsing, "put-put" sound) | Supply decoupling inadequate | Add 100μF + 100nF across the 9V supply near Q1. Check CE is properly connected across RE. |
| Instability — frequency wanders when hand approaches | Normal for regenerative receiver near threshold | Shield coil with cardboard box. Reduce regeneration slightly. |
| Works but audio is very faint | Earphone impedance mismatch or C_audio too small | Check C_audio = 100nF. Use high-impedance crystal earphone OR add a second common-emitter audio stage after C_audio. |
| Loud hum on all stations | Poor supply filtering OR RF pickup from mains | Add more supply filtering. Keep circuit away from mains cables. Use battery, not mains supply. |

---

## 17. What to Record in Your Notebook

For every listening session, record:

```
Date: ___________  Time (local): ___________  UTC time: ___________
Weather: ___________  (clouds/clear/rain — affects ionosphere slightly)
Season: ___________  (winter nights give best DX)

Station log:
| Freq (kHz) | Station Name | Location | Signal Quality (1-5) | Regen Setting (1-10) |
|------------|--------------|----------|----------------------|----------------------|
|            |              |          |                      |                      |

Antenna used: ___________  Length: ___________ 
Antenna orientation (N-S or E-W): ___________

Unusual events:
- Did you hear stations you don't normally hear? (Note possible ionospheric enhancement)
- Static crashes? (Lightning, thunderstorms — direction roughly indicated by S-meter)
- Any heterodyne interference between stations?

Circuit settings:
- Regeneration pot position (degrees from minimum): ___
- Tuning capacitor range covered: ___ pF to ___ pF
- Any component changes this session: ___
```

**Why record UTC time?** International stations and amateur radio operators use UTC (Coordinated Universal Time). When reporting a DX reception to others ("I heard a station from Brazil at 0300 UTC"), the time is meaningful worldwide. UTC = GMT = London winter time.

**Why weather?** Ionospheric propagation correlates with solar activity, season, and to a lesser extent local weather. After a few months of records you will see patterns.

---

## 18. Key Insight

The regenerative receiver demonstrates that **feedback is the most powerful tool in electronics engineering.**

Negative feedback (used in audio amplifiers, operational amplifiers, motor controllers, temperature regulators) stabilises, linearises, and makes circuits predictable at the cost of some gain.

Positive feedback (used in oscillators, triggers, and at the heart of radio reception) creates extraordinarily sensitive systems at the cost of stability.

Armstrong's insight — push positive feedback to the edge but not over — is exactly what the PLL does electronically. Every RF system you will encounter in your drone project: the WiFi chip (2.4 GHz), the GPS receiver, the RC link — all contain PLLs. All of them are Armstrong's regenerative detector, stabilised and miniaturised.

You have just built, with one transistor, a circuit that achieves a sensitivity within a factor of 2–3 of a modern communication-grade receiver. The difference between your single-transistor receiver and the chip inside a WiFi radio is not a different principle — it is engineering refinement of the same principle over 100 years.

---

*Next: Stage 7 — The Colpitts Oscillator. You will take the oscillating mode of the regenerative receiver and turn it into a controlled, stable transmitter.*
