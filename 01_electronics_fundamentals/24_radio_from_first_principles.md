# Radio From First Principles — The Marconi Path

Build your understanding of radio from absolute zero using only wire, coils, capacitors, and diodes — exactly the intellectual journey that Guglielmo Marconi and his contemporaries took, but with today's basic components available to you.

No ICs. No modules. No black boxes.

By the end of this document you will understand radio not as something that happens inside a chip but as a physical, touchable, buildable phenomenon that you discovered yourself.

---

## The One Idea That Makes All of Radio

Before any component, before any circuit — one physical fact:

> **A wire carrying a changing electric current radiates energy into space as an electromagnetic wave.**

That is radio. Everything else is engineering that flows from this single fact.

The inverse is also true:
> **A changing electromagnetic wave passing a wire induces a tiny current in that wire.**

Transmitter = make changing current → radiate wave.
Receiver = wave arrives → tiny induced current → detect it.

Marconi did not invent this idea. Heinrich Hertz proved it experimentally in 1887. James Clerk Maxwell predicted it mathematically in 1865. Marconi's genius was engineering — making it reliable, long-range, and useful.

---

## Stage 1: The Spark — How Hertz Made Radio Waves

### What Hertz Built

Hertz's transmitter was breathtakingly simple:

```
Battery → induction coil (step-up transformer) → spark gap
                                                     │
                                           Two metal rods with a gap
                                           between them
```

The induction coil charged the rods to thousands of volts. When the voltage exceeded the air gap's breakdown voltage — **crack** — a spark jumped across. The spark was a very rapid oscillating current — electrons bouncing back and forth across the gap at radio frequencies (hundreds of MHz).

This oscillating current radiated an EM wave. No oscillator circuit needed. The spark itself was the oscillation.

Hertz's receiver was equally simple:

```
A loop of wire with a tiny gap — a "resonator"

If the gap was small enough, a tiny spark appeared in it when a wave arrived from the transmitter.
```

Hertz could see the spark in his receiver from across the room. He had proven that EM waves existed and could travel through air. That was 1887.

### The Spark Gap in a Jar (Leyden Jar)

Before capacitors had the name "capacitor," physicists used Leyden jars — glass jars coated inside and outside with metal foil. The jar IS a capacitor.

**Marconi's early transmitter (1895) was essentially:**
```
                +HV
                 │
           [Induction coil]
                 │
          ───────┤├───────  Leyden jar (capacitor C)
                 │
         ────────┤────────  Tuning coil L (antenna circuit)
                 │
              ═══╪═══       Spark gap
                 │
                GND
```

When the Leyden jar charged to the spark breakdown voltage, a spark fired. The capacitor (C) and the inductor/antenna (L) formed an LC circuit. The spark excited it to oscillate at the resonant frequency `f = 1 / (2π√LC)`. These oscillations were what radiated from the antenna.

**The problem with spark transmitters:** The spark was damped — the oscillations died out within a few cycles. This produced a very wide "splat" of frequencies (broadband noise). You could not tune two spark transmitters near each other. This is why spark transmitters were banned internationally by 1927 — they polluted the entire radio spectrum.

But for Marconi in 1895, crossing the Bristol Channel (14 km) with a message — it was enough.

---

## Stage 2: Build the Simplest Detector — The Crystal Set

The crystal set requires no battery, no active components, no power at all. It is powered entirely by the radio waves it receives. This is your first build.

### How It Works (Every Part Explained)

**The Antenna:** A long wire (10–30 metres) strung outside as high as possible. The AM broadcast station's wave induces a tiny AC voltage in it (microvolts to millivolts).

**The Tuned Circuit (LC tank):** An inductor L and a variable capacitor C in parallel. This circuit resonates at `f = 1 / (2π√LC)`. At resonance it has very high impedance — the desired station's signal builds up to maximum voltage. All other frequencies are short-circuited. This is how you "tune" the radio.

**The Diode Detector:** The tuned circuit produces an AM signal — a high-frequency (MW: 530–1700 kHz) carrier with the audio modulated onto its amplitude (envelope). The diode removes one half of the AC cycle — this is called **rectification**. Only the envelope (audio shape) remains.

**The Audio Filter:** A small capacitor across the earphones removes the remaining RF carrier — only the audio frequency (300 Hz – 3 kHz for voice) passes through.

**The Crystal Earphone:** A piezoelectric earphone. The rectified audio current causes a crystal to physically vibrate — moving a diaphragm — making sound. Uses NO power — the crystal converts electrical energy to mechanical energy directly.

```
CRYSTAL SET SCHEMATIC

Antenna (long wire, high up)
    │
    ├──────────────────────────┐
    │                          │
   [L] (coil with tap)        [VC] (variable cap)
    │                          │
    ├──────────────────────────┤  ← this is the LC tank (parallel resonant)
    │
   [D1] (germanium diode OA91 or 1N34A)
    │
    ├────────────[C2 100pF]────┤
    │                          │
  [Crystal earphone]           │
    │                          │
   GND ────────────────────────┘
         (Earth ground — metal rod in soil, or cold water pipe)
```

### Components You Need — All Basic, No ICs

| Component | Value | What to buy / how to get |
|-----------|-------|--------------------------|
| Wire      | ~20 m | Any wire — thicker is better for antenna |
| Coil former | — | Toilet roll tube / 50mm PVC pipe |
| Enamelled copper wire | 0.3–0.5 mm | Buy as "magnet wire" / winding wire |
| Variable capacitor | 10–500 pF | Old AM radio — salvage the variable cap |
| Germanium diode | OA91, OA47, 1N34A | Germanium, NOT silicon — much lower forward voltage (~0.2V vs 0.6V for silicon) |
| Crystal earphone | 2000Ω impedance | Old-style piezo earphone, very cheap |
| Small capacitor | 100 pF ceramic | Any value 47pF–220pF |
| Earth wire | | Wire to metal water pipe or ground rod |

**Why germanium diode and NOT 1N4148 silicon?**
The AM signal can be millivolts. A silicon diode needs 0.6V before it conducts — the signal would never turn it on. Germanium diodes start conducting at 0.1–0.2V — they can detect these tiny signals.

### Winding the Coil

This is the most important skill in RF electronics — winding an inductor.

```
Former: 50 mm diameter PVC pipe or toilet roll tube
Wire:   0.3 mm enamelled copper wire (magnet wire)
Turns:  80 turns close-wound for medium wave (MW)

Method:
  1. Leave 10 cm of wire free at one end (this is Terminal A).
  2. Wind 80 turns of wire tightly in a single layer, keeping turns touching.
  3. Every 10 turns, bring out a small loop (tap) — these allow you to connect at different points.
  4. Leave 10 cm free at the end (Terminal B).
  5. Secure with tape or varnish.

The tap at turn 10–20 from the antenna end couples the antenna without loading the tank.
```

**Inductance of the coil (approximate):**
```
For a close-wound single-layer coil:
  L (μH) ≈ (d² × n²) / (450 × d + 1000 × l)

  d = diameter in metres
  n = number of turns
  l = length of winding in metres

50mm former, 80 turns, 0.3mm wire, ~24mm winding length:
  L ≈ (0.05² × 80²) / (450 × 0.05 + 1000 × 0.024)
  L ≈ (0.0025 × 6400) / (22.5 + 24)
  L ≈ 16 / 46.5 ≈ 344 μH
```

**Tuning range with variable capacitor:**
```
With L = 344 μH, C variable 10–500 pF:

  f_max = 1 / (2π × √(344μH × 10pF))  = 857 kHz
  f_min = 1 / (2π × √(344μH × 500pF)) = 121 kHz

To cover MW band (530–1700 kHz), you need a smaller coil (fewer turns).
Try 40 turns for 1 MHz centre frequency.
```

**Practical tuning:**
- Spread turns apart → lower inductance → higher frequency.
- Compress turns together → higher inductance → lower frequency.
- Adjust capacitor → fine tune.

### How to Test and Listen

1. Connect a 10-metre wire antenna as high as possible (roof, tree, upper floor window).
2. Connect a ground wire to a metal water pipe or a rod driven into damp soil.
3. Set variable capacitor to mid-position.
4. Put crystal earphone in ear.
5. Slowly rotate the variable capacitor.
6. You should hear stations — faint at first. No battery. No power. Pure physics.

**If nothing heard:**
- Longer antenna — every metre helps dramatically.
- Better earth connection — wet soil conducts better.
- Try at night — AM signals travel much further after dark (sky wave reflection from ionosphere).
- Try near a strong local MW transmitter.
- Check your germanium diode orientation — reverse it if nothing.

**What you just demonstrated:**
- Resonance: your LC circuit selected one frequency from the hundreds arriving at the antenna.
- Rectification: your diode detected the amplitude envelope of the AM signal.
- Energy harvesting: you powered a speaker with no battery — the radio station's transmitter powered your earphone.

This is exactly what Marconi's early receivers did, except he used a different detector (coherer — explained below).

---

## Stage 3: The Original Detector — The Coherer

Before the crystal detector, Marconi used a device invented by Édouard Branly in 1890: the **coherer**.

### How the Coherer Worked

```
A small glass tube filled with iron filings between two metal plugs.

At rest:  filings are loose → high resistance (no current flows) → circuit open.
RF arrives: the tiny RF currents cause the filings to clump (cohere) → resistance drops to near zero → current flows → activates a relay → records a dot or dash.
After each detection, a tapper (small motor) shakes the tube to de-cohere the filings → ready for next pulse.
```

The coherer was binary — either ON (cohered) or OFF (de-cohered). It could only detect the presence or absence of a signal, not the amplitude or audio content. This is why early radio was Morse code only — dots (short bursts) and dashes (long bursts).

**Build a coherer (demonstration):**
```
Take a short section of drinking straw.
Plug both ends with small bolts or metal screws.
Fill loosely with iron filings (from a file, or iron powder).
Connect to a battery and an LED in series.

Touch the tube to a charged Van de Graaff generator, a spark from a lighter, or transmitter antenna:
  → filings cohere → resistance drops → LED lights.
Tap the tube → LED goes out.
```

This demonstrates why early radio was so hard — a random electrostatic discharge (lightning, machinery) would trigger the coherer. The noise floor was terrible.

---

## Stage 4: Understanding Resonance — The LC Tank

The LC tank circuit is the most important circuit in radio. Build one and observe resonance physically.

### The LC Oscillation (Water Analogy)

```
Capacitor = bucket of water (stores charge / water)
Inductor = spinning flywheel connected to the pump (stores current / momentum)

Charge cap → release → current flows through inductor → inductor stores energy magnetically
Current peaks → cap now discharged → inductor's field collapses → forces current to continue → recharges cap (reversed polarity)
Cycle repeats at resonant frequency → perfect oscillation

Without resistance losses → oscillates forever.
With resistance losses → oscillations decay (damped oscillation — exactly like spark transmitter).
```

### Build and Observe LC Resonance

**Equipment:** Function generator (or use a 555 timer as an audio oscillator) + oscilloscope (or sound card oscilloscope + resistor voltage divider for safety).

```
CIRCUIT:

Function generator → [series resistor 1kΩ] → LC tank → GND

Scope probe: across the LC tank (parallel combination of L and C).

Sweep frequency from 10 kHz to 1 MHz slowly.
At resonant frequency: voltage across tank PEAKS dramatically.
Below and above resonance: voltage is low.

The peak voltage can be 5–50× the input voltage at resonance — this is the Q factor.
```

**Q factor (Quality factor):**
```
Q = (resonant frequency) / (bandwidth)
Q = (1/R) × √(L/C)

High Q = sharp, selective peak = good tuning (separates stations well)
Low Q = broad, flat peak = poor selectivity

Wire resistance in the coil reduces Q. Use thicker wire, silver wire, or Litz wire for high Q.
```

**Practical observation:**
- Use a 350 μH coil (your crystal set coil) and a 100 pF capacitor.
- Resonant frequency ≈ 1 / (2π × √(350μH × 100pF)) ≈ 850 kHz.
- If you have an AM broadcast station near 850 kHz, tune to it with your crystal set using this combination.

---

## Stage 5: From Detection to Amplification — The Cat's Whisker

Crystal radio gives you stations 50–200 km away on a good antenna and earth. But the volume is very low and you can only use a crystal earphone (high impedance). To drive a loudspeaker, you need amplification.

### The Cat's Whisker Detector (Galena Crystal)

Before germanium diodes existed (1940s), radio builders used a **galena crystal** (lead sulfide, PbS) with a thin copper wire ("cat's whisker") touching its surface. The wire–crystal contact was a natural Schottky junction — a primitive diode.

```
Galena crystal holder:
  Crystal in a small cup/holder.
  Copper wire bent to a fine point, mounted on a adjustable arm.
  Scratch the wire across the crystal surface until you find a sensitive spot.
  At the right spot: the junction works as a rectifying diode.

Problem: vibration or time shifts the wire → lose the spot → have to search again.
This is why the reliable germanium diode (same semiconductor physics, fixed contact) replaced it immediately after WWII.
```

**Galena crystal physics:** The metal-semiconductor contact formed naturally between the copper wire and the PbS crystal is a Schottky barrier diode. Exactly the same physics as a modern Schottky diode — except it is unstable and variable.

**You can buy galena crystals today** — they are available from mineral suppliers and electronics hobbyist stores. The historical experience is worth having.

---

## Stage 6: The First Amplifier — The Triode Valve (Vacuum Tube)

Before transistors, amplification came from vacuum tubes. Understanding the tube explains the transistor — they are the same idea in different materials.

### The Audion — Lee de Forest, 1906

De Forest added a third electrode (the "grid") between the cathode and the plate of a diode tube. A small voltage on the grid controlled a large current between cathode and plate. This was the triode — the first amplifier.

```
Vacuum tube triode:
  Cathode (heated filament) → emits electrons
  Grid → controls electron flow (small voltage → large current change)
  Plate (anode) → collects electrons
  All in an evacuated glass envelope

Exactly analogous to:
  Emitter → Base → Collector (BJT transistor)
  Source  → Gate → Drain   (MOSFET)
```

**The triode as an amplifier:**
```
Small AC signal on Grid → large AC current variation in plate circuit → 
voltage developed across plate resistor → amplified output

Voltage gain = gm × Rplate
gm (transconductance) = change in plate current per volt of grid voltage
```

The concept is identical to a transistor common-emitter amplifier.

---

## Stage 7: One Transistor Radio Receiver

Now build an AM receiver using a single transistor. No ICs. This is how transistor radios worked in the 1950s and 60s.

### The Regenerative Receiver

Invented by Edwin Armstrong in 1912 (using a triode valve). Regeneration (positive feedback) makes a single amplifying stage do the work of many.

**The key idea:** Take a fraction of the amplified output and feed it back to the input in phase (positive feedback). The circuit amplifies its own signal repeatedly — gain can be hundreds or thousands from one device. Just below the point of oscillation: extremely sensitive detector. At the oscillation threshold: also becomes a local oscillator (used in superheterodyne receivers).

```
ONE-TRANSISTOR REGENERATIVE RECEIVER

Antenna ─── [10T tickler winding, coupled to main coil]
                │
            [Main LC tank: 40-turn coil on 50mm former + variable capacitor]
                │
         Base of NPN transistor (BC547, 2N2222)
                │
         Collector ──── [4.7kΩ resistor] ──── 9V battery +
                │
         [Regeneration: 10pF capacitor from collector back to the tank]
                │
         [Audio: 0.01μF capacitor from collector] ──── [Crystal earphone]
                │
         Emitter ──── [1kΩ] ──── GND

Regeneration control: a variable resistor (10kΩ) in series with the feedback capacitor.
Turn it up slowly until you're just below the edge of oscillation — maximum sensitivity.
```

### How the Regenerative Detector Works

At sub-oscillation setting:
1. Station signal arrives at tank circuit → small voltage appears.
2. Transistor amplifies it.
3. Part of amplified signal fed back to tank → adds to incoming signal → amplified again.
4. This loop multiplies the effective Q of the tank enormously → extreme selectivity AND sensitivity.
5. The transistor also acts as a detector (non-linear device = rectification of AM envelope).

**At oscillation threshold:**
- The circuit starts generating its own RF signal.
- This mixes with incoming AM signals → produces audio as a beat frequency.
- Used for receiving CW (Morse code) and SSB (single sideband) transmissions.

**Why Armstrong's regenerative receiver was revolutionary:**
A crystal set needed a very strong local signal. Armstrong's circuit could receive stations across the Atlantic on a small indoor antenna. One transistor (originally one tube) replaced what would later require 20+ tubes in a non-regenerative design.

**The regenerative receiver is the most sensitive single-transistor RF circuit possible.** Radio amateurs still build them today for this reason.

---

## Stage 8: Building a Transmitter From Scratch

Understanding reception is half the story. Now generate radio waves yourself.

### The Colpitts Oscillator — One Transistor, Generates RF

The Colpitts oscillator uses a transistor's amplification to sustain oscillations in an LC tank:

```
COLPITTS OSCILLATOR (MW frequency, ~1 MHz)

        9V +
         │
        [R1 47kΩ]
         │
         ├──────────────────── Collector
         │                        │
        [R2 10kΩ]               [L] (antenna/coil, ~350 μH)
         │                        │
         ├──── Base               ├──────── RF Output (antenna)
         │
        [C1 100pF] ──┬── Emitter ──┬──── GND
                     │             │
                    [C2 100pF]   [RE 470Ω]
                     │
                    GND

Oscillation frequency: f = 1 / (2π × √(L × Ceq))
  where Ceq = C1 × C2 / (C1 + C2) = 50 pF
  f = 1 / (2π × √(350μH × 50pF)) = 1.2 MHz  (within MW band)
```

**How it self-oscillates:**
- C1 and C2 form a voltage divider — the junction is the feedback point.
- A fraction of the collector signal is fed back to the emitter (which connects to the C1-C2 junction).
- This is positive feedback — the signal at the emitter drives the base via the voltage developed across C2.
- The transistor amplifies → amplified signal feeds back → sustains the oscillation.

**To transmit (SHORT range, legal for very low power):**
- Connect a 30–100 cm wire antenna to the collector or directly to the coil.
- An AM radio tuned to the same frequency (set variable capacitor to match) should pick up your oscillator as a carrier.

**To modulate (make it transmit voice/audio):**
- Feed an audio signal (from a microphone amplifier) into the base via a coupling capacitor and small resistor.
- The audio varies the transistor's operating point → varies the oscillation amplitude → AM modulation.

```
AUDIO MODULATION:

Microphone → [2-transistor amplifier] → [coupling cap 0.1μF] → [1kΩ resistor] → Base
```

**Legal note:** Unlicensed transmission on MW band is legal at very low power in most countries (USA: Part 15, <100 mW, <3 metre antenna; UK: similar Ofcom provisions). At the power levels of a single transistor oscillator on a short wire, you will be heard only within a few metres. This is fine for experimentation.

---

## Stage 9: The Superheterodyne Principle — Edwin Armstrong, 1918

The crystal set and regenerative receiver struggle with selectivity and sensitivity together. Armstrong invented the solution that every radio uses today.

**The problem:**
- High Q tank = very selective but hard to make variable and maintain Q.
- Amplifying at the received frequency (RF) is unstable — easy to oscillate.
- Different stations at different frequencies need different filter/amplifier characteristics.

**Armstrong's solution — heterodyne:**
Convert the received RF signal to a fixed Intermediate Frequency (IF) where a sharp, fixed filter can be built permanently.

```
SUPERHETERODYNE PRINCIPLE:

Antenna → [RF amp / bandpass filter]
                │
                ↓
           [Mixer] ←───── [Local Oscillator (LO)]
                │
                ↓
          [IF Filter: fixed frequency, very sharp] ← fixed crystal filter
                │
                ↓
          [IF Amplifier] (multiple stages for high gain)
                │
                ↓
          [Detector / Demodulator]
                │
                ↓
          [Audio amplifier]
                │
              Speaker
```

**How mixing works:**
```
Mixer multiplies two signals:
  RF signal at f_rf
  × Local oscillator at f_lo
  = products at (f_rf + f_lo) and (f_rf − f_lo)

Choose the difference: f_if = f_rf − f_lo

Example (AM MW radio):
  Tuning to 1000 kHz station
  IF = 455 kHz (standard for AM)
  Local oscillator = 1000 + 455 = 1455 kHz
  Mixer output: 1455 + 1000 = 2455 kHz (filtered out) + 1455 − 1000 = 455 kHz ✓

Tune to 600 kHz station:
  Local oscillator = 600 + 455 = 1055 kHz
  Mixer output: 455 kHz ✓ (always the same IF)
```

The mixer translates any station to the same 455 kHz IF regardless of which station you tune to. The sharp 455 kHz filter is always correctly tuned.

**Image frequency problem:**
```
If station is at 1000 kHz, image is at 1000 + (2 × 455) = 1910 kHz.
A signal at 1910 kHz would also mix to 455 kHz (1910 − 1455 = 455 kHz).
Solution: RF bandpass filter before the mixer to reject the image.
Modern receivers use higher IF (10.7 MHz for FM) → image is further away → easier to filter.
```

**Build a superheterodyne front end (discrete components):**

**LO (Colpitts oscillator):**
```
Standard Colpitts oscillator (from Stage 8), tuned to f_station + 455 kHz.
Variable capacitor controls tuning. Dial reads station frequency.
```

**Mixer (passive double-balanced diode mixer):**
```
4 diodes in a ring + two transformers (bifilar wound on ferrite cores):

   RF IN ──── [Transformer T1] ──── [Diode ring D1-D4] ──── [Transformer T2] ──── IF OUT
                                    ↑
                              LO injected here

The diode ring: when LO is positive half-cycle, D1/D3 conduct.
                When LO is negative half-cycle, D2/D4 conduct.
This effectively multiplies (modulates) the RF signal by the LO signal.
Output is sum and difference frequencies.
```

**455 kHz IF filter (ceramic filter):**
A ceramic filter is a passive component — piezoelectric resonator that passes only a narrow band around 455 kHz. Buy CFW455 (455 kHz ceramic filter) — no ICs, just a passive component. Connect in series with the IF chain.

**IF amplifier (2 transistors):**
```
Standard common-emitter amplifier stages (BC547) with 455 kHz transformers (IF transformers — yellow/black/white cans, available cheaply) coupling between stages.

Standard IF transformer values: approximately 590 μH, designed to resonate at 455 kHz with a built-in trimmer capacitor.
```

**Detector (envelope detector — same as crystal set):**
```
IF amplifier output → germanium diode → RC filter → audio out
Simple 1N34A or OA91 diode, 10kΩ resistor, 4.7nF capacitor.
```

This is a complete AM superheterodyne receiver from first principles using only: transistors, diodes, coils, capacitors, resistors, and one ceramic filter.

---

## Stage 10: FM and the Discriminator

AM is easy to detect (envelope detector = diode + RC). FM encodes information in frequency changes. Detecting frequency changes requires different circuits.

### The Slope Detector (Simplest FM Detector)

Tune the IF tank slightly off resonance. Now the tank's frequency response has a slope — higher frequency means more output, lower frequency means less output. Frequency changes become amplitude changes. Then just use an AM envelope detector.

**Problem:** Works but gives poor audio quality and is sensitive to AM noise.

### The Foster-Seeley Discriminator (1936)

Uses the phase relationship between the IF signal and a reference to extract frequency information:

```
          IF in
           │
    [Primary coil L1]
           │
    ──────┤├──────  [C1 tuning cap]
           │
    [Secondary coil L2] (tightly coupled to L1)
           │
    ┌──────┴──────┐
    │             │
   [D1]          [D2]   (two diodes, back to back)
    │             │
  [C2, R1]      [C3, R2]  (RC filters)
    │             │
    └──────┬──────┘
           │
         Audio out

At f = IF: D1 and D2 conduct equally → output = 0
At f > IF: D1 conducts more → positive output voltage
At f < IF: D2 conducts more → negative output voltage
```

The output voltage is proportional to the frequency deviation — this IS the audio signal.

### The Ratio Detector (1944, simpler to build)

A modified discriminator that rejects AM noise:
```
Same topology as Foster-Seeley but with a large capacitor (electrolytic) stabilising the sum voltage.
The ratio of D1:D2 voltages gives audio, not the difference.
AM noise (amplitude changes) affects both diodes equally → ratio unchanged → noise rejected.
```

This is what most FM radios used before ICs existed.

---

## Stage 11: The Full Discrete Component Radio — Bill of Materials

Here is a complete component list to build a working MW AM superheterodyne radio with no ICs:

### Components

**Coils and transformers (wind yourself or buy vintage/NOS):**
| Part | Value | How to get |
|------|-------|------------|
| Antenna coil | 350 μH, ferrite rod | Wind 80 turns on 10mm ferrite rod |
| Local oscillator coil | 220 μH | Wind 50 turns on ferrite rod |
| IF transformer ×2 | 455 kHz | Buy CFB455, or old radio salvage |
| Audio output transformer | 1kΩ:8Ω | From old radio / intercom |

**Semiconductors (no ICs):**
| Part | Value | Notes |
|------|-------|-------|
| Transistor ×4 | BC547 / 2N2222 | RF amp, mixer, ×2 IF amp |
| Diode ×2 | OA91 / 1N34A | Mixer + detector (germanium) |
| Diode ×1 | 1N4148 | Power supply protection |

**Passive components:**
| Part | Value | Notes |
|------|-------|-------|
| Variable capacitor | 2-gang 10–365 pF | Old AM radio salvage |
| Ceramic filter | CFW455 | 455 kHz IF filter |
| Capacitors assorted | 10 pF – 100 μF | Standard values |
| Resistors assorted | 470Ω – 100kΩ | Standard ¼W |
| Crystal earphone or speaker | 2kΩ or 8Ω | Crystal earphone needs no amp |

**Total cost:** ~£5–15 / $6–20 from eBay/AliExpress salvage and component suppliers.

---

## Stage 12: The Learning Ladder — Experiments in Order

Work through these in sequence. Each experiment builds on the last.

```
Week 1: Observe resonance
  Components: coil, variable capacitor, LED, battery, function generator (or 555 timer)
  Goal: watch the LED brighten at resonance frequency as you vary the cap.
  Teaches: resonance is real and visible.

Week 2: Crystal set
  Components: coil, variable cap, germanium diode, crystal earphone, antenna wire, ground wire
  Goal: hear an AM station with NO battery.
  Teaches: resonance selects frequency, diode detects amplitude, energy comes from the station.

Week 3: Cat's whisker exploration
  Components: galena crystal, copper wire, mounting hardware
  Goal: find a detecting spot, compare to the diode.
  Teaches: the semiconductor junction is a natural phenomenon, not an invention.

Week 4: Diode detector analysis
  Components: oscilloscope (or audio amplifier + speaker), crystal set from Week 2
  Goal: probe the circuit at different points — see the RF waveform, see the detected audio.
  Teaches: what rectification looks like in practice.

Week 5: Colpitts oscillator
  Components: 1× BC547, resistors, capacitors, coil from Week 1
  Goal: generate an RF signal. Confirm with crystal set or AM receiver.
  Teaches: positive feedback sustains oscillation, LC sets frequency.

Week 6: Modulate the oscillator
  Components: Colpitts from Week 5, microphone, 2× BC547 preamp
  Goal: speak into microphone, hear your voice on an AM receiver.
  Teaches: amplitude modulation — varying bias changes oscillation amplitude.

Week 7: Regenerative receiver
  Components: 1× BC547, coil, variable cap, germanium diode, crystal earphone
  Goal: hear distant stations you could not get on the crystal set.
  Teaches: positive feedback multiplies gain enormously, sensitivity vs selectivity trade-off.

Week 8: Build the mixer
  Components: 4× OA91 germanium diodes, ferrite cores, wire for bifilar coils
  Goal: demonstrate that mixing two frequencies produces sum and difference.
  Teaches: the heart of the superheterodyne principle.

Week 9: Complete superheterodyne
  Components: all of the above + IF transformers + CFW455 filter + audio stage
  Goal: full AM radio with multiple stages — loudspeaker output.
  Teaches: how all stages combine, the role of each stage in the chain.

Week 10: Add SSB/CW reception
  Components: add a variable oscillator for the BFO (Beat Frequency Oscillator)
  Goal: receive amateur radio CW (Morse code) and SSB voice.
  Teaches: heterodyne demodulation, what SSB means in practice.
```

---

## Stage 13: Test Equipment From Basic Components

You need to measure and observe. Build this before or alongside your radio experiments.

### RF Probe for Multimeter

A multimeter cannot measure RF directly. Convert RF to DC first:

```
RF signal ──[1N34A germanium diode]──┬── DC output to multimeter
                                     │
                                   [47pF cap]
                                     │
                                    GND

The diode rectifies RF → capacitor smooths to DC → multimeter reads the peak RF voltage.
You can now measure signal levels across your radio circuits.
```

### Simple Field Strength Meter

```
Short wire antenna (30 cm) → [tuned LC tank] → [1N34A diode] → [100μA panel meter]

As you move the meter near your transmitter, the needle deflects.
Tune the tank to your transmitter frequency for maximum deflection.
Useful for antenna experiments and transmitter tests.
```

### Absorption Wavemeter (Frequency Measurement)

```
LC tank (with calibrated variable cap) + LED or meter across it.
Bring the wavemeter near a signal source.
When the tank resonates at the same frequency as the source → LED brightens (energy absorbed).
Read the frequency from the calibrated dial.

No batteries needed — passive absorption.
```

---

## Stage 14: Antenna Theory From First Principles

### Why Antenna Length Matters

The wire you transmit or receive with must be a significant fraction of the wavelength to be efficient:

```
Wavelength λ = c / f = 300,000,000 / frequency

MW radio 1 MHz:  λ = 300 m → half-wave = 150 m (Marconi used 60m vertical with ground plane)
FM radio 100 MHz: λ = 3 m → half-wave = 1.5 m
WiFi 2.4 GHz:     λ = 12.5 cm → half-wave = 6.25 cm
```

**Marconi's insight:** You cannot always use a half-wave antenna. For the long-wave frequencies of his day (500 kHz → λ = 600 m), a half-wave antenna would need to be 300 metres tall. Marconi used a quarter-wave vertical antenna with a large ground plane (counterpoise) — the ground acts as a mirror, making the image of the antenna below ground simulate the other half.

### Practical Wire Antennas You Can Build

**The Long Wire (crystal set standard):**
```
Any wire, longer is better.
Not resonant (too short for MW wavelengths) but works by brute force — captures more signal area.
Run as high as possible, outside, as far from buildings as possible.
```

**The Half-Wave Dipole (FM band, 88–108 MHz):**
```
λ/4 = 300,000,000 / (100×10⁶) / 4 = 0.75 m per half = 1.5 m total

Two pieces of wire each 0.75 m long:
    ─────────── [0.75m] ─────[feedpoint]─────── [0.75m] ───────────
                              │
                            coax (or twin feeder)
                              │
                            radio

Natural impedance at resonance: ~73 Ω.
```

**The Quarter-Wave Vertical with ground radials (VHF/UHF):**
```
One vertical wire λ/4 long + 4 radials (λ/4 each) pointing horizontally outward.
Impedance: ~35 Ω (use a balun or matching network to 50 Ω).
This is the basis of every rubber-duck antenna on a radio and every mobile phone antenna.
```

---

## Stage 15: What Marconi Would Make of Today's Components

The progression from Marconi's bench to a modern radio is a direct line:

| Marconi's component | Modern equivalent | Same function |
|---------------------|-------------------|---------------|
| Leyden jar | Electrolytic capacitor | Stores charge |
| Induction coil | Transformer | Voltage step-up |
| Spark gap oscillator | Crystal oscillator / Colpitts | Generates RF |
| Coherer | Schottky diode + comparator | Threshold detector |
| Galena + cat's whisker | Germanium / Schottky diode | Envelope detector |
| Triode vacuum tube | BJT transistor / MOSFET | Amplification |
| Variable condenser | Varactor diode | Tuning |
| IF transformer + capacitor | Ceramic / SAW filter | IF bandpass filter |
| Regenerative detector | PLL in FM chip | Sensitive locked detection |
| Heterodyne principle | Mixer in every radio chip | Frequency translation |

And inside every modern radio chip (which you started without) are just transistors, diodes, resistors, and capacitors arranged exactly as this document describes — but miniaturised to nanometres.

---

## Glossary — Terms from Marconi to Modern Radio

| Term | Meaning |
|------|---------|
| Carrier | The high-frequency wave that carries the modulated signal |
| Coherer | Branly's iron-filings detector — the first practical radio detector |
| Damped wave | An oscillation that decays — produced by spark gap transmitters |
| Dielectric | An insulating material between capacitor plates (air, paper, ceramic) |
| Feedpoint | Where the transmission line connects to an antenna |
| Galena | Lead sulfide crystal used as a natural diode in early radio |
| Ground plane | A flat conductor that acts as a reflector for a vertical antenna |
| Heterodyne | Mixing two frequencies to produce sum and difference frequencies |
| IF (Intermediate Frequency) | Fixed frequency in a superhet to which all received signals are converted |
| Image frequency | The unwanted frequency that also converts to IF (mirror image) |
| Litz wire | Multi-strand twisted wire for high-Q coils at RF frequencies |
| Local oscillator (LO) | The variable oscillator in a superhet that determines which station is received |
| Permeability | How well a core material concentrates magnetic flux — affects inductance |
| Q factor | Sharpness of resonance — ratio of resonant frequency to bandwidth |
| Reactance | Frequency-dependent resistance — capacitive (XC) or inductive (XL) |
| Regeneration | Positive feedback in a receiver to increase gain and selectivity |
| Resonance | The frequency at which LC reactances are equal and opposite — maximum response |
| Sky wave | Radio waves that reflect off the ionosphere — enables long-distance reception at night |
| Skin effect | At RF, current flows only on surface of conductor — use thin wire or Litz |
| Superheterodyne | Receiver that converts incoming RF to a fixed IF before detection |
| Tickler coil | Small coil coupled to tank coil to provide regenerative feedback |
| Transmission line | Cable that carries RF with controlled impedance (coax, twin feeder) |
| Tuned circuit | LC resonant circuit that selects a frequency |
| Varactor | A diode whose capacitance varies with reverse voltage — used for electronic tuning |
| Wavemeter | Instrument for measuring frequency by absorption — passive, no battery |

---

## Where to Get Components

### For Stage 1–6 (Crystal set and early experiments)

| Item | Source |
|------|--------|
| Germanium diodes OA91, 1N34A | eBay (search "germanium diode OA91"), Rapid Electronics, Mouser |
| Crystal earphone | eBay (search "crystal earphone"), old telephone accessories |
| Variable capacitor (AM radio type) | Salvage from old AM radio (charity shop, car boot sale), eBay "365pF variable capacitor" |
| Enamelled copper wire 0.3mm | eBay "magnet wire 0.3mm", craft suppliers |
| Ferrite rods | eBay "ferrite rod 10mm AM radio", old AM radio antenna |
| Galena crystal | Mineral specimen shops, eBay "galena crystal specimen" |

### For Stage 7–9 (Transistor receiver)

| Item | Source |
|------|--------|
| BC547, BC548, 2N2222 transistors | Mouser, Farnell, Rapid, eBay (buy 20 — cheap) |
| IF transformers (455 kHz) | eBay "IF transformer 455kHz yellow" or salvage from old radio |
| Ceramic filter CFW455 | eBay, Mouser (search "455kHz ceramic filter") |
| Ferrite beads and cores | eBay, Mouser |
| Trimmer capacitors 5–60 pF | eBay, Mouser, Farnell |

### Tools

| Tool | Use | Cost |
|------|-----|------|
| Multimeter | Voltage, continuity, resistance | £8–20 |
| Soldering iron | Assembly | £10–20 |
| Oscilloscope | Essential for RF work | Free: use PC soundcard + audioscope software (up to 20 kHz only) / DSO138 kit (~£12) / DS1054Z (~£100) |
| SDR receiver (RTL-SDR) | Visualise spectrum, see your oscillator on screen | £20 — after you understand basics |

---

## The Mindset — Think Like Marconi

Marconi did not understand all the mathematics. He was not primarily a theorist. He worked empirically:

1. **Build something simple.** The simplest thing that could possibly work.
2. **Observe carefully.** What happens? Not what should happen — what does happen?
3. **Change one thing.** More turns on the coil. Longer antenna. Different ground. One change at a time.
4. **Record results.** Distances achieved, conditions, component values.
5. **Let the physics teach you.** The circuit will show you things no book will.

When your crystal set picks up a voice from a station 100 km away, powered by nothing but the station's own transmitter — you will have understood something fundamental about the physical world that no amount of reading can fully replace.

That understanding — the physical intuition built through experimentation — is what makes all subsequent learning stick. An IC or module is no longer a mystery. It is just a miniaturised version of what you already built on a breadboard.

Start with the long wire antenna and the germanium diode. The rest follows naturally.
