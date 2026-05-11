# Stage 2 — The Crystal Set: Radio With No Battery

---

## What You Will Learn

- Why a crystal set receives radio stations with no battery or power supply whatsoever (the energy comes from the transmitter, hundreds of kilometres away)
- How an antenna converts electromagnetic waves into electrical voltages — the physics of induction
- Why a ground connection is not optional — what it actually does electrically
- The LC parallel resonant tank in full mathematical detail — not just the formula, but where it comes from
- Q factor — what it means, how to calculate it, how to measure it, how it determines selectivity
- Why you must use a germanium diode and not a silicon diode — a calculation that makes the reason quantitative
- Envelope detection — step by step, with a diagram of the waveform at each point
- The crystal earphone — why it works here when a loudspeaker does not
- How to wind the inductor, tune the variable capacitor, and bring in radio stations
- Complete build instructions, troubleshooting, and measurement guide

**Prerequisites:** Stage 1 (spark gap and radio waves). You should understand what a capacitor and inductor are. Stage 3 (LC resonance) goes deeper into the maths — read this stage first, then Stage 3 for fuller understanding.

**Time required:** 3 hours reading + 4–6 hours building and tuning

**Cost:** ₹300–800 / $5–12 USD (depending on whether you salvage parts or buy new)

---

## Part 1 — Why a Crystal Set Needs No Battery

This seems impossible at first. A device that receives and plays radio stations with no power supply. Where does the energy come from?

The answer is that it comes from the transmitter — perhaps hundreds of kilometres away.

Consider a powerful AM broadcast transmitter. A typical medium-wave (AM) broadcast station in India or the UK transmits at 100 to 500 kilowatts. That is 100,000 to 500,000 watts of radio frequency energy, radiated in all directions.

At a distance of 100 km, that energy has spread out over a sphere of surface area 4π × (100,000)² ≈ 1.26 × 10¹¹ m². The power density is:

```
    Power density = Transmitter power / Surface area
    Power density = 100,000 W / 1.26 × 10¹¹ m²
    Power density = 7.9 × 10⁻⁷ W/m²  (0.79 µW per square metre)
```

An antenna that captures 0.1 m² of this wave (a modest antenna) receives:

```
    Received power = 7.9 × 10⁻⁷ × 0.1 = 79 nW (79 nanowatts)
```

79 nanowatts is extraordinarily small — about 12 million times less power than a single LED. But a crystal earphone requires only about 10–100 nanowatts to produce audible sound. So we are right on the edge of detectability — which is exactly why every part of the crystal set must be optimised, and why the earphone must be high-impedance and efficient.

**The key insight:** You are not creating energy. You are intercepting and converting energy that was transmitted by someone else's powerful transmitter. The crystal set is a passive energy harvester. Every milliwatt it wastes to heat or poor impedance matching is a milliwatt that cannot drive the earphone.

---

## Part 2 — The Antenna as a Transducer

### How the Antenna Receives

When a radio wave passes through a conductor (a wire), the oscillating electric field of the wave exerts a force on the free electrons in the conductor. The electrons accelerate — they are pushed back and forth along the wire at the frequency of the radio wave.

This motion of electrons IS an electric current. Specifically, it is an alternating current at the radio frequency. An alternating current flowing through a wire creates a voltage across any impedance connected to that wire.

This is the fundamental receiving process. The antenna is transducing electromagnetic energy (the radio wave) into electrical energy (an AC voltage).

### The Induced Voltage

The voltage induced in a wire antenna is approximately:

```
    V_induced = E_field × L_eff

    Where:
      V_induced = induced EMF in Volts
      E_field   = electric field strength of the radio wave in V/m
      L_eff     = effective length of the antenna in metres
```

The effective length is roughly half the physical length for a simple wire. A 10-metre wire has an effective length of about 5 metres.

For a 500 kW transmitter at 100 km distance, the electric field strength at the receiving location might be around 1–10 mV/m. With a 10-metre antenna (L_eff ≈ 5 m):

```
    V_induced = 5 mV/m × 5 m = 25 mV
```

25 millivolts of RF signal. This is the raw signal your crystal set must work with. After passing through the LC tank (which attenuates it somewhat due to matching losses), the signal arriving at the diode might be 5–20 mV peak. This is why the diode threshold matters so much — see Part 5.

### Why Longer = More Signal

Directly from the formula: a longer antenna intercepts a larger region of the passing wave, collecting more energy and producing a larger induced voltage. Doubling the antenna length roughly doubles the signal. Practical limits:

- **Too short (less than 5 m):** Very weak signal, hard to receive anything
- **Good range (8–20 m):** Excellent for a crystal set. More stations, louder audio.
- **Too long (more than 30–40 m):** Can overload the detector diode; self-resonance of the wire starts to matter; some selectivity is lost

For best results: 10–20 metres of wire, as high as possible, as straight as possible, with no sharp bends.

### Why Height Matters

A wire suspended high above the ground is far from lossy soil and nearby metal objects. Metal objects near the antenna couple to it, changing its resonant frequency and absorbing signal. The wire should be kept at least 1–2 metres from walls and metallic structures.

---

## Part 3 — The Earth Ground: Why It Is Essential

Every antenna circuit needs a reference — a return path for current. In a crystal set, this is the earth ground.

### The Antenna-Earth Capacitance

Your antenna wire, suspended in the air, acts as one plate of a capacitor. The earth (or a large metal object connected to earth) acts as the other plate. The air between them is the dielectric. This capacitance is typically 100–500 pF for a typical outdoor antenna.

This capacitance forms part of your tuning circuit. It is in parallel with your variable tuning capacitor. It means the actual capacitance in the tuning circuit is:

```
    C_total = C_variable + C_antenna
```

So your variable capacitor needs to accommodate this stray capacitance in its tuning range.

### The Return Path for RF Current

When the antenna captures a radio wave, electrons flow. They flow along the antenna wire — but then where do they go? They need a return path back to "ground" to complete the circuit. Without a proper ground, the current has nowhere to return and no signal is received.

The earth ground provides this return path. Electrons flow:
- Along the antenna wire
- Through the LC tuning circuit
- Through the diode detector
- Through the earphone
- Down to ground
- Through the earth itself (the earth conducts at RF because of its size and moisture content)
- Back up to the base of the antenna (there is capacitance between the bottom of the antenna wire and the earth)

This forms a complete circuit. Without earth, there is no circuit, no current, no signal.

### What Counts as a Good Ground

Best (lowest impedance, most current possible):
1. **A copper rod driven into moist earth** — a 1-metre copper rod, 15 mm diameter, driven into moist soil. Cold water pipes buried in the ground make excellent grounds too.
2. **A large metal sheet in or on the ground** — a 1 m² sheet of copper or aluminium laid on grass.

Acceptable:
3. **Household cold water pipe** (metallic, connected to buried pipework) — connect your ground wire to a cold water pipe under a sink
4. **Radiator/central heating pipe** — often connected to buried pipework
5. **A long wire lying on or near the ground** (a counterpoise) — 5–10 metres on the ground is much better than nothing

Poor (barely usable):
6. **A large metal object indoors** (filing cabinet, sink, even a heating radiator not connected to buried pipe) — gives some capacitive coupling to ground
7. Nothing at all — no ground, no working crystal set

### A Note on Indoor Operation

If you are in an upstairs flat without access to buried pipes, tape 5 metres of wire to the wall, baseboard, or floor. This is a "counterpoise" — not a true earth, but it provides enough capacitive coupling to ground to allow the circuit to function, albeit with reduced sensitivity.

---

## Part 4 — The LC Parallel Resonant Tank

### Purpose

The LC tank circuit in a crystal set performs one crucial job: it selects ONE station out of the many that are simultaneously inducing voltages in your antenna.

Without the LC tank, the detector would receive all stations at once — a cacophony of overlapping signals. The LC tank is a filter that passes only the frequency you are tuned to and rejects all others.

### The Parallel Resonant Circuit

```
        From antenna
              |
              |
        ┌─────┴─────┐
        │           │
       [L]        [C]
     (coil)     (variable
                capacitor)
        │           │
        └─────┬─────┘
              │
           To detector
              │
             GND
```

In a parallel LC circuit:
- At the resonant frequency, the impedance is VERY HIGH (theoretically infinite for a perfect circuit)
- Above and below the resonant frequency, the impedance drops

This means: at resonance, the RF voltage across the tank is maximum. Off resonance, the voltage drops. This is how it selects one frequency.

### Derivation of Resonant Frequency

At any frequency f, the reactance of the inductor is:

```
    XL = 2π × f × L     (in Ohms)
```

The reactance of the capacitor is:

```
    XC = 1 / (2π × f × C)     (in Ohms)
```

In a parallel circuit, resonance occurs when the current drawn by L equals the current drawn by C (they cancel each other in the parallel circuit, leaving only the resistance to pass current). The condition for this is XL = XC:

```
    2π × f × L = 1 / (2π × f × C)

Multiply both sides by f:

    2π × f² × L = 1 / (2π × C)

Multiply both sides by (2π × C):

    (2π)² × f² × L × C = 1

Divide both sides by (2π)² × L × C:

    f² = 1 / ((2π)² × L × C)

Take the square root:

              1
    f₀ = ──────────────     ← The resonant frequency formula
          2π × √(L × C)
```

This is the same formula as the spark circuit in Stage 1. It is THE fundamental formula of radio. Memorise it.

**Note what it tells you:**
- Increasing L or C decreases the resonant frequency (longer wavelength)
- Decreasing L or C increases the resonant frequency (shorter wavelength)
- To tune to a lower frequency (longer wavelength station), increase the capacitance of your variable capacitor
- To tune to a higher frequency, decrease the capacitance

### Typical AM Band Values

The AM medium wave broadcast band covers 530–1710 kHz in the Americas, or 531–1602 kHz (9 kHz spacing) in most of the rest of the world.

For a coil with L = 250 µH (a typical ferrite rod inductor for AM):

At f = 531 kHz:
```
              1
    C = ──────────────
         (2πf₀)² × L

              1
    C = ──────────────────────────────
         (2π × 531,000)² × 250×10⁻⁶

              1
    C = ──────────────────────────────
         (3,336,283)² × 250×10⁻⁶

              1
    C = ──────────────────────────────
         1.113×10¹³ × 250×10⁻⁶

              1
    C = ─────────────
         2.782×10⁹

    C = 3.6×10⁻¹⁰ F = 360 pF
```

At f = 1602 kHz:
```
              1
    C = ──────────────────────────────
         (2π × 1,602,000)² × 250×10⁻⁶

    C ≈ 39 pF
```

So a variable capacitor that goes from about 40 pF to 360 pF will cover the entire AM broadcast band with a 250 µH coil. This matches standard AM variable capacitors (typically 20–500 pF travel range).

---

## Part 5 — Q Factor

### What Q Factor Means Physically

Q (Quality factor) is one number that tells you how "sharp" a resonant circuit is. Physically, it means:

**Q = (Energy stored in the circuit) / (Energy lost per radian of oscillation)**

Or equivalently:

**Q = (Resonant frequency) / (Bandwidth)**

A circuit with high Q:
- Has a sharp, narrow resonance peak
- Stores oscillations for a long time without decay
- Rejects stations far from the tuned frequency
- Has a narrow bandwidth

A circuit with low Q:
- Has a broad, flat resonance
- Loses energy quickly
- Picks up multiple nearby stations (poor selectivity)
- Has a wide bandwidth

In radio terms: a low-Q crystal set hears several stations at once and they bleed into each other. A high-Q crystal set hears one station cleanly and neighbours are inaudible.

### Q Factor Formula

For a parallel LC circuit with parasitic resistance R (the resistance of the wire in the coil, plus dielectric losses in the capacitor):

```
              1      L
    Q = ──── × √───
              R      C

Where:
  Q = quality factor (dimensionless)
  R = equivalent series resistance of the inductor in Ohms
  L = inductance in Henries
  C = capacitance in Farads
```

Or expressed in terms of bandwidth:

```
    Q = f₀ / BW₃dB

Where:
  f₀  = resonant frequency
  BW₃dB = 3 dB bandwidth (the frequency range over which the response is within 3 dB of maximum)
```

### What Determines Q for a Coil?

For an air-core coil:

```
    Q = 2πf₀L / R
```

- R is the DC resistance of the wire plus "AC resistance" (skin effect at RF — more on this in Stage 3)
- L is the inductance
- Higher L for the same R → higher Q
- More turns → more L but also more R (proportional) → Q stays roughly constant unless you optimise the geometry
- **Thicker wire → lower R → higher Q**
- **Fewer turns of thicker wire at the right frequency → best Q**
- **Ferrite core → much higher L for same turns → much higher Q** (this is why ferrite rod antennas are standard in AM radios)

### Typical Q Values

| Coil type | Typical Q at 1 MHz |
|-----------|-------------------|
| Hand-wound coil, thin wire (0.2 mm), air core | 30–80 |
| Hand-wound coil, thick wire (0.5 mm), air core | 80–150 |
| Store-bought RF choke | 30–60 |
| Ferrite rod antenna (commercial AM radio) | 100–200 |
| Silver-plated coil, silver mica capacitor, optimised layout | 300–600 |

### Bandwidth Calculation

If your crystal set has Q = 50 and is tuned to 1 MHz:

```
    BW = f₀ / Q = 1,000,000 / 50 = 20,000 Hz = 20 kHz
```

AM stations are 9 kHz wide (in Europe/Asia). So with Q = 50, your bandwidth is 20 kHz — you might hear two adjacent stations overlapping. With Q = 100, bandwidth = 10 kHz — just about one station. With Q = 200, bandwidth = 5 kHz — a single station with some of the audio treble cut.

**Practical target for a good crystal set:** Q > 80 at the midpoint of the AM band (around 1 MHz).

### How to Measure Q

The simplest way with basic equipment:

1. Build the LC tank circuit.
2. If you have a function generator: inject a signal from the function generator into the tank via a large series resistor (10 kΩ, to avoid loading the tank). Monitor the voltage across the tank with a high-impedance voltmeter or oscilloscope.
3. Sweep the frequency. The voltage peaks at resonance. Find the peak.
4. Find the frequencies above and below resonance where the voltage drops to 0.707 × peak (that is the –3 dB point).
5. Measure the bandwidth: BW = f_upper – f_lower.
6. Q = f₀ / BW.

Without a function generator, you can estimate Q by connecting a small battery briefly to give the tank a kick (very briefly, via a large resistor), then measuring how many cycles the oscillation rings before dying down on an oscilloscope. Q ≈ π × N_cycles_to_half_amplitude. But this requires an oscilloscope to see.

For a complete beginner without instruments, proceed and assess Q qualitatively: if the crystal set receives one station at a time and adjacent stations are inaudible, Q is adequate. If you hear multiple stations bleeding together, Q is low and you should improve the coil (thicker wire, ferrite rod).

---

## Part 6 — Why You Must Use a Germanium Diode

### The Diode's Job

The diode's job in the crystal set is to detect the AM signal — to recover the audio (the voice or music) from the modulated RF carrier.

An AM signal looks like this: a high-frequency carrier (e.g. 1 MHz) whose amplitude (height) varies in time according to the audio being transmitted. The audio might be a 1 kHz tone, a voice, or music.

```
    Full AM signal at antenna:

    ↑
    │ ╭─╮  ╭──╮  ╭──╮  ╭──╮  ╭─╮  ╭╮
    │╭╯ ╰──╯  ╰──╯  ╰──╯  ╰──╯ ╰──╯╰─  ← carrier (1 MHz)
    │     (amplitude rises and falls at audio rate)
    │
    └──────────────────────────────────→ time
    (1 µs per cycle = 1 MHz carrier)
```

The diode must extract the envelope — the slowly-varying amplitude — from this high-frequency signal. It does this by rectifying: passing current only on positive half-cycles. After a low-pass filter (RC), what remains is the audio envelope.

### The Threshold Problem

A real diode does not conduct until its forward voltage is exceeded. Below the threshold voltage, the diode acts as an open circuit — no current flows, no detection happens.

Threshold voltages:
- Silicon diode (1N4148, 1N914): ~0.6 V (or 600 mV)
- Schottky diode (BAT43, BAT85): ~0.25 V (250 mV)
- Germanium diode (OA91, 1N34A, OA47): ~0.1–0.2 V (100–200 mV)
- Galena crystal (natural point contact): varies, typically 0.05–0.15 V

Recall from Part 2: the signal arriving at the detector from a typical AM station might be 5–25 mV peak.

**5 mV is 1/120th of the silicon diode threshold (600 mV).**

The silicon diode is completely blind to this signal. It never begins to conduct. The crystal set produces no output at all.

**5 mV is 1/20th to 1/40th of the germanium diode threshold (100–200 mV).**

Still below threshold, but the germanium diode has a much softer knee — it begins to conduct a tiny, measurable current even below its threshold, because of quantum mechanical tunnelling and the shape of the I-V curve. This tiny current IS enough to drive a crystal earphone.

**Quantitative comparison:**

Near the threshold, a diode can be modelled by the Shockley equation:

```
    I = IS × (e^(V/nVT) - 1)

Where:
  I  = diode current
  IS = reverse saturation current (very different for Ge vs Si)
  e  = Euler's number (2.718)
  V  = voltage across the diode
  n  = ideality factor (~1 for Ge, ~2 for Si in the low-current region)
  VT = thermal voltage = 26 mV at room temperature (300 K)
```

Key: IS for germanium is approximately 10⁻⁶ A (1 µA). IS for silicon is approximately 10⁻¹² A (1 pA) — a factor of ONE MILLION lower.

At V = 10 mV (a reasonable RF signal level at the diode):

For Germanium (IS = 1 µA, n = 1):
```
    I = 1×10⁻⁶ × (e^(10/26) - 1)
    I = 1×10⁻⁶ × (e^0.385 - 1)
    I = 1×10⁻⁶ × (1.47 - 1)
    I = 1×10⁻⁶ × 0.47
    I = 0.47 µA
```

For Silicon (IS = 1 pA, n = 2):
```
    I = 1×10⁻¹² × (e^(10/52) - 1)
    I = 1×10⁻¹² × (e^0.192 - 1)
    I = 1×10⁻¹² × (1.212 - 1)
    I = 1×10⁻¹² × 0.212
    I = 2.12×10⁻¹³ A = 0.000212 pA
```

The germanium diode passes 0.47 µA = 470,000 times more current than the silicon diode at 10 mV.

For driving a crystal earphone that requires ~10 nA to produce any audio, the germanium diode works; the silicon diode produces essentially nothing.

**This is not a matter of preference. It is physics. Use germanium.**

---

## Part 7 — The Envelope Detection Process, Step by Step

### Stage 1: The AM Signal at the Antenna

An AM signal has this structure:

```
    v(t) = Ac × [1 + m × audio(t)] × cos(2πfct)

Where:
  Ac = carrier amplitude
  m  = modulation index (0 to 1; 1 = 100% modulation)
  audio(t) = normalised audio signal (between -1 and +1)
  fc = carrier frequency
```

At 100% modulation (m=1) with a 1 kHz audio tone:

- When audio = +1: signal amplitude = 2 Ac (maximum)
- When audio = 0: signal amplitude = Ac (carrier only)
- When audio = -1: signal amplitude = 0 (complete cancellation)

### Stage 2: After the LC Tank

The LC tank selects this one station's signal. Assume 90% efficiency — we get 90% of the induced voltage across the tank. The waveform shape is unchanged. We have the full AM signal but now isolated from other stations.

### Stage 3: The Waveform at the Diode Anode

This is the full AM waveform. Both positive and negative halves of the carrier are present.

```
        ┌──Envelope──┐
     ↑  ╭─╮  ╭──╮    │
     │ ╭╯ ╰──╯  ╰╮   │  ← audio modulation: positive half
  0V─┼─────────────────────────
     │ ╰╮        ╭╰─╮│  ← audio modulation: negative half
     ↓  ╰╮  ╭──╮╯   ╰╯
         ╰──╯
          (1 µs cycles at 1 MHz)
```

### Stage 4: After the Diode (Half-Wave Rectified)

The diode passes only the positive half-cycles. Negative half-cycles are blocked.

```
     ↑  ╭─╮  ╭──╮
     │ ╭╯ ╰──╯  ╰╮        ← only positive half-cycles pass
  0V─┼─────────────────────────
     │                        ← negative half: diode blocks this
     ↓

    The peaks of the half-wave rectified signal
    trace the shape of the audio envelope.
```

### Stage 5: After the RC Low-Pass Filter

The RC filter (R and C in parallel across the output) smooths out the 1 MHz ripple, leaving only the slowly-varying audio envelope:

```
     ↑    ╭─────────────╮
     │   ╱               ╲    ← audio waveform
     │  ╱                 ╲
  DC─┼────────────────────────
     │ (DC level = carrier average)
     ↓
    This is the recovered audio signal.
    It can drive a crystal earphone.
```

The remaining DC level (the average value of the envelope) does not drive the earphone because the earphone is coupled through capacitor Cout, which blocks DC and passes only the AC audio variation.

### Choosing the RC Filter Values

The RC time constant must satisfy:

```
    1/fcarrier << RC << 1/faudio_max

    i.e.    τ_carrier << τ_RC << τ_audio

Where:
  fcarrier  = RF carrier frequency (e.g. 1 MHz)
  faudio_max = highest audio frequency you want to reproduce (e.g. 5 kHz)

Numerically:
  τ_carrier = 1/1,000,000 = 1 µs
  τ_audio   = 1/5,000    = 200 µs

So RC must be >> 1 µs and << 200 µs

A good choice: RC = 10 µs to 50 µs

Example: R = 10 kΩ, C = 1 nF → RC = 10,000 × 10⁻⁹ = 10 µs ✓
         R = 10 kΩ, C = 4.7 nF → RC = 47 µs ✓
```

For a crystal set, R is typically 47 kΩ to 470 kΩ (high impedance to not load the circuit), and C is 100 pF to 10 nF.

---

## Part 8 — The Crystal Earphone

### Why a Speaker Won't Work Here

A typical 8Ω loudspeaker:
- Has a very low impedance (8 Ω)
- Requires significant current to drive
- Loads the crystal set detector output and kills the signal
- Needs milliwatts of power to produce audible sound

The crystal set output impedance is very high — typically 100 kΩ to 1 MΩ. Connecting an 8Ω speaker to this creates a massive impedance mismatch. Almost no power is transferred to the speaker. It is like trying to force water through a garden hose using a pressure that can barely support a column of water one inch high — the impedance mismatch means the system is completely inefficient.

### The Piezoelectric Crystal Earphone

A crystal earphone works on the piezoelectric effect: a crystal of Rochelle salt (sodium potassium tartrate) or more commonly nowadays a ceramic piezoelectric element (lead zirconate titanate, PZT) produces a mechanical deformation when a voltage is applied across it.

A thin piece of this material, bonded to a metal diaphragm, flexes when voltage is applied. Flexing = sound.

**Critical property:** The crystal earphone is a capacitive element. Its impedance is:

```
    Z = 1 / (2π × f × C)

For a typical crystal earphone with C ≈ 1000 pF at audio frequencies (1 kHz):
    Z = 1 / (2π × 1000 × 1000×10⁻¹²)
    Z = 1 / (6.28 × 10⁻⁶)
    Z ≈ 159,000 Ω ≈ 160 kΩ
```

160 kΩ impedance. This closely matches the high output impedance of the crystal set detector. Good power transfer. This is why the crystal earphone is the correct load for a crystal set.

**Practical note:** Crystal earphones are sold as "ceramic earphones" in India and online. Search: "crystal earphone" or "ceramic earphone" or "2000 ohm earphone" or "high impedance earphone". They cost ₹30–100 ($0.50–$1.50). They look like old-fashioned single earpieces or small in-ear buds.

**Warning:** Do NOT use modern 32Ω stereo headphones. They will not work. Do NOT use phone earbuds. Do NOT use computer speakers. Only a crystal/ceramic earphone works without amplification.

---

## Part 9 — Full Circuit Schematic

```
CRYSTAL SET — COMPLETE CIRCUIT

                ANTENNA (10–20 m wire, as high as possible)
                    │
                    │
                   TAP (use coil tap, approximately 1/3 from top)
                    │
        ┌───────────┴──────────────────────────────────────────┐
        │                                                      │
        │ L (coil, ~250 µH,                                  [C_var]
        │  wound on ferrite rod                         (variable capacitor
        │  or 50mm former)                               ~40 to 500 pF)
        │                                                      │
        └───────────┬──────────────────────────────────────────┘
                    │
                    │ ← RF voltage appears here (high impedance point)
                    │
                   [D1]  ← Germanium diode OA91 or 1N34A
                    │     (anode toward tank, cathode toward output)
                    │
             ┌──────┴──────┐
             │             │
           [R1]          [C1]    ← Detector low-pass filter
         (100 kΩ)       (1 nF)    RC ≈ 100 µs
             │             │
             └──────┬──────┘
                    │
                [C_out]  ← output coupling cap, 10–47 nF
                    │      (blocks DC, passes audio)
                    │
               [EARPHONE]  ← crystal/ceramic earphone, high impedance
                    │
                   GND ───────────────────── EARTH
                                    (copper rod in ground,
                                     or cold water pipe)
```

**Component list (all values given):**

| Component | Value | Notes |
|-----------|-------|-------|
| L (coil) | 250 µH nominal | Wind your own — see Part 11 |
| C_var (variable cap) | 40–500 pF | Salvage from old AM radio |
| D1 (diode) | OA91 or 1N34A or OA47 | Germanium ONLY |
| R1 | 100 kΩ | Standard 0.25W carbon film |
| C1 | 1 nF (1000 pF) | Ceramic, any voltage rating |
| C_out | 47 nF | Ceramic or film, any voltage |
| Earphone | Crystal/ceramic high-impedance | NOT modern headphones |
| Antenna wire | 10–20 m | Any insulated wire, as high as possible |
| Earth connection | — | 1m copper rod in moist earth or cold water pipe |

---

## Part 10 — How to Obtain the Parts

### Germanium Diode

The critical part. Sources:

- **India:** "OA91 diode" on Robu.in, Electronicwings, or any electronics market. Also available as "1N34A" on Amazon India. Also look for OA47, OA70, OA79 — all are germanium point-contact diodes that work.
- **eBay (worldwide):** Search "1N34A germanium diode" or "OA91 diode" — buy a pack of 10, they are cheap (under $3 for 10).
- **Old equipment:** Germanium diodes were used extensively in radios and TVs from 1950s–1970s. If you have access to old radio equipment, check the circuit board for "OA91" or similar markings.
- **Test before use:** With a multimeter on diode mode, a germanium diode should read 150–250 mV forward voltage. Silicon reads 550–700 mV. If your "germanium" reads 600 mV, you have been sold silicon — return it.

### Variable Capacitor

- **Best source:** Salvage from a dead AM radio. Any old transistor radio, clock radio, or AM receiver has a variable capacitor for tuning. It is the component with the rotating shaft and meshing metal plates.
- **New:** Search "365 pF variable capacitor" or "air variable capacitor" on eBay or electronics suppliers. Cost: $2–5.
- **India:** Velleman or generic versions available in Mumbai, Delhi electronics markets. Also on Amazon India: "variable capacitor AM radio".

### Crystal Earphone

- **India:** "Crystal earphone" or "ceramic earphone" — widely available at electronics markets. Any old telephone spare parts shop will have them.
- **eBay:** "Crystal earphone" or "high impedance earphone" — packs of 5 available for $3.
- **Note:** If you can only get a modern 32Ω earphone, add a transistor amplifier stage (Stage 5 in this series) to drive it.

### Wire for the Coil

- **Enamelled copper wire 0.3 mm (30 AWG):** Available on eBay/Amazon India as "magnet wire" or "enamelled copper wire". A 100g spool is more than enough. Cost: ₹200–400.
- **Salvage:** Unspool from a dead transformer, motor winding, or relay. Measure the diameter with a micrometer or estimate by measuring 10 turns wrapped around a pencil.

### Ferrite Rod (Optional but Strongly Recommended)

A ferrite rod as the coil core multiplies the inductance dramatically, allowing fewer turns for the same inductance (lower resistance, higher Q).

- **Type:** Ferrite rod, 8–10 mm diameter, 100–200 mm long, permeability µr = 100–200. Often sold as "AM ferrite rod antenna" or "ferrite bar antenna core".
- **Sources:** Old AM radio antennas are ferrite rods. Salvage the rod from any dead AM radio or clock radio. The rod is the brown/grey bar inside the radio with the antenna coil wound on it.
- **eBay:** "Ferrite rod 10mm 100mm" — about $2 each.

---

## Part 11 — Step-by-Step Coil Winding Instructions

### Option A: Air-Core Coil on PVC Former

**Former:** 50 mm (2 inch) diameter PVC pipe, 100 mm long. Cut from standard plumbing pipe. Sand the surface lightly with 120-grit sandpaper to help the wire grip.

**Wire:** 0.3 mm enamelled copper wire.

**Target:** 250 µH (measured or calculated). Starting point: approximately 80–100 turns single layer.

**Inductance estimate for a single-layer coil (Wheeler's formula in metric units):**

```
         r² × n²
    L = ──────────       (in µH)
         9r + 10l

Where:
  r = coil radius in inches (NOT metres!)
  n = number of turns
  l = coil length in inches

For our 50mm diameter (r = 25mm = ~1 inch):
  r = 25mm / 25.4 = 0.984 inches ≈ 1 inch (close enough)

For 80 turns in 50mm length (l = 50mm / 25.4 ≈ 2 inches):

         1² × 80²
    L = ──────────
         9×1 + 10×2

         6400
    L = ──────
          29

    L ≈ 221 µH
```

For 100 turns in 60mm (l = 2.36 inches):

```
         1 × 10000
    L = ───────────
         9 + 23.6

    L ≈ 307 µH
```

So winding 80–100 turns will give approximately 220–310 µH — within range for covering the AM broadcast band with a 40–500 pF variable capacitor.

**Winding steps:**

1. Tape the free end of the wire to the former, leaving 15 cm free (this will be the end connection). Label this end A.
2. Begin winding. Hold the former in your non-dominant hand and guide the wire with your dominant hand.
3. Wind each turn immediately next to the previous — no gaps, no overlaps. Push each turn against the last.
4. Wind firmly but not so tight that the wire kinks.
5. After 10 turns, pause. Place a small piece of tape over the windings to hold them, then stick a small label: "10T".
6. Continue. Make taps (bring a loop of wire out) at every 10 turns: 10T, 20T, 30T... These taps allow you to experiment with connecting the antenna to different points on the coil (important for impedance matching).
7. Wind to approximately 80–100 turns total.
8. Tape the final turn firmly. Leave 15 cm free at the end (label this end B).
9. Make the antenna tap loop at the 20–30 turn point — this is typically the best antenna connection point. (The antenna impedance is lower than the tank impedance, so connecting at a tap partway down the coil is an impedance transformer — more on this in Stage 3.)

### Option B: Ferrite Rod Coil

**Core:** 8–10 mm diameter ferrite rod, 100–150 mm long.

**Wire:** 0.3 mm enamelled copper wire.

**Target inductance:** 250 µH. With a ferrite rod of permeability µr ≈ 200, you need far fewer turns.

**Approximate turns for ferrite rod coil:**

As a rough rule with a ferrite rod, the inductance scales approximately as:

```
    L_ferrite ≈ µr × L_air_core
```

So if 80 turns air-core gives 220 µH, then with ferrite (µr = 200) the inductance would be enormous. In practice, the effective permeability is much lower because the rod is not a closed magnetic circuit — flux leaks around the ends. Typical effective permeability for a rod is µeff ≈ 50–100.

**Practical recipe for ferrite rod:**
- Wind 60–80 turns on the rod, spread over 30–40 mm in the middle of the rod (not the ends)
- Measure with an inductance meter if you have one, or trial and tune
- Expected inductance: 200–400 µH — well within range

**Advantage of ferrite:** Much fewer turns means much lower DC resistance means much higher Q. A 60-turn ferrite rod coil at 1 MHz typically has Q = 100–200. The same inductance in air core requires 300+ turns of thin wire and Q drops to 20–50.

### Stripping the Wire Ends

For each end you need to connect:
1. Cut the wire cleanly.
2. Hold the end against 400-grit sandpaper.
3. Rub back and forth 15–20 times, rotating the wire between rubs.
4. The end should be bright shiny copper with no dark varnish coating.
5. Test: touch your multimeter probes (continuity mode) to two stripped ends of a short piece of wire. Should beep immediately. If it does not beep, the enamel is not stripped.

### Checking the Coil with a Multimeter

1. Set multimeter to Ohms (resistance) mode.
2. Measure between end A and end B (the full winding).
3. Expected DC resistance: 5–30 Ω for a properly wound coil. If you get open circuit (∞), either the enamel is not stripped or the wire has broken. If you get 0 Ω, you have a short (wire insulation failed at some point).

---

## Part 12 — Variable Capacitor Guide

### How It Works Mechanically

A variable capacitor consists of two sets of metal plates — one fixed, one on a rotating shaft. As you turn the shaft, the rotating plates mesh into the gaps between the fixed plates, increasing the overlapping area and therefore the capacitance.

```
    SIDE VIEW:
                shaft
                  |
    fixed plates: │
         ╠══╬══╬══╣ ← these don't move
                  │
    rotating plates (on shaft):
         ╠══╬══╬══╣ ← these rotate, meshing between fixed plates

    When fully meshed: maximum area = maximum capacitance
    When fully out:    minimum area = minimum capacitance
```

Typical values for an AM radio variable capacitor:
- Minimum capacitance (fully un-meshed): 10–20 pF (mostly stray capacitance of the plates and wiring)
- Maximum capacitance (fully meshed): 365–500 pF

### Salvaging from an Old Radio

1. Open the radio (four to six screws on the back).
2. Look for a component with multiple parallel metal plates on a shaft, connected to the tuning knob. This is it.
3. It will be soldered to the circuit board. Desolder the three or four connections carefully (two sections of the capacitor — ganged — are common in superheterodyne radios; you only need one section for a crystal set).
4. Test: connect the multimeter in capacitance mode (if it has one) and measure while rotating. Capacitance should change smoothly from minimum to maximum. No jumps or open circuits.

### Wiring the Variable Capacitor

One terminal goes to the top of the coil (end A or end B). The other terminal goes to ground. The shell/frame of the capacitor also connects to ground (it is the fixed plates in most designs — the frame IS the ground-connected plates).

---

## Part 13 — Complete Build Sequence

Work on a piece of cardboard or wooden board as your assembly platform. Use crocodile clip leads for initial assembly, then solder for the final version.

**Step 1 — Test all components individually before connecting anything.**

- Test the diode: multimeter in diode mode. Forward voltage (anode positive) should be 100–250 mV for germanium. If 550–700 mV, it is silicon — do not use it.
- Test the variable capacitor: rotate it through full range — should change smoothly in capacitance.
- Test the coil: DC resistance 5–30 Ω between end terminals.
- Test the earphone: connect a 9V battery briefly through a 1 MΩ resistor to the earphone. You should hear a click. This proves the piezo element is working.

**Step 2 — Lay out the circuit on your cardboard with crocodile clips.**

Do not solder yet. Connect:
- Coil L across variable capacitor C_var (this is the tank circuit)
- Antenna to the coil tap at ~20-30 turns from the grounded end
- Ground to the lower end of the coil and to one terminal of C_var
- Diode D1 from the hot end of the tank (junction of L and C_var, NOT the grounded end) to R1
- R1 from the diode cathode to ground
- C1 in parallel with R1 (across R1, from diode cathode to ground)
- C_out from diode cathode to earphone
- Earphone other terminal to ground
- Antenna wire: out the window, as high as possible
- Ground wire: to a cold water pipe, copper rod in earth, or counterpoise

**Step 3 — Run the antenna out the window.**

The longer and higher, the better. At minimum: 5 metres up on a wall or fence. Better: 10 metres strung between two upper-floor windows. Best: 15–20 metres on poles in the garden.

**Step 4 — Connect the ground.**

Run a wire from your circuit ground point to the best ground you have available: cold water pipe under a sink, copper rod in the garden, or failing those, a 5-metre counterpoise wire along the baseboard.

**Step 5 — Initial tuning.**

Put the earphone in your ear. Rotate the variable capacitor slowly from minimum to maximum capacitance. Listen for stations.

If nothing is heard:
- Check all connections with the multimeter (continuity between expected points)
- Try rotating the capacitor more slowly — you might have passed through a station quickly
- Try a different antenna (longer/higher)
- It is most productive to do this at night — see Part 14

**Step 6 — Optimise antenna connection point.**

Once you find one station, try moving the antenna connection to different taps on the coil:
- More turns from the bottom (higher up the coil) = higher impedance = better for stronger signals or longer antenna
- Fewer turns from the bottom (lower on the coil) = lower impedance = better match for shorter antennas

**Step 7 — Solder the final circuit.**

Once the circuit is working, solder it on a small piece of stripboard (Veroboard). This will improve connection reliability and reduce losses from clip lead resistance.

---

## Part 14 — Tuning Procedure

### What to Adjust

There are really only two things to tune in a crystal set:

1. **The variable capacitor** — this is your tuning control. Rotate it to change the resonant frequency of the LC tank. Turning it one way increases capacitance and tunes to lower frequencies (longer wavelengths); the other way tunes higher.

2. **The antenna connection tap** — determines impedance matching between antenna and tank. Not a real-time adjustment; set once and leave.

### Step-by-Step Tuning

1. Start with the earphone in your ear and the volume of the environment very quiet (night-time is best).
2. Rotate the variable capacitor very slowly — a full rotation from end to end might take 10–15 seconds. Listen for any change in the background noise level — a station will appear as either a carrier hiss (sound of the modulation) or actual audio.
3. When you find a station, fine-tune slowly around it. The LC tank bandwidth is narrow — the signal rises and falls sharply as you tune.
4. Try all the antenna taps to see which gives the loudest signal.
5. If you live in the southern hemisphere or outside the main AM broadcast regions, check your local AM broadcast frequencies and confirm the band covers them (adjust coil turns if needed).

### Best Time to Listen

AM radio propagation changes dramatically with the time of day:

- **During the day:** Ground wave propagation only. You can receive stations within 100–300 km. Strong local stations will be received.
- **At night:** The ionosphere (specifically the F-layer) reflects medium wave signals back to earth, enabling skywave propagation. You can receive stations from 500–3,000 km away. This is when a crystal set comes into its own — distant, exotic stations from other countries.

In India: on a good night with a decent antenna, you might hear All India Radio stations from across the country, and possibly stations from neighbouring countries. In Europe: BBC stations, European national broadcasters, possibly North African or Middle Eastern stations.

---

## Part 15 — Measurement Guide: What to Measure at Each Node

**Setup:** Use a high-impedance voltmeter or DMM set to AC voltage mode (highest sensitivity range).

**Warning:** Connecting a low-impedance voltmeter to any point in a crystal set will load the circuit and reduce (or kill) the signal. A standard DMM has 10 MΩ input impedance — this is acceptable for measurement but will reduce the signal by some amount. Note this in your measurements.

| Measurement Point | Expected Value | What It Tells You |
|------------------|----------------|-------------------|
| Across tank (L & C_var) | 5–50 mV AC at RF | Signal strength at that station; increases with antenna length and station power |
| Across R1 (after diode) | 1–10 mV DC | DC component = rectified carrier average; if 0, diode is not conducting |
| Across C_out (audio signal) | 0.5–5 mV AC at audio frequencies | What the earphone sees; if much lower than tank voltage, diode threshold is killing the signal |
| Antenna to ground | 5–100 mV AC RF | Raw signal before tuning; wider frequency spread than after the tank |

If the tank voltage is healthy (5 mV or more) but the voltage after the diode is negligible, your diode may be silicon or faulty — replace with a confirmed germanium diode.

---

## Part 16 — Troubleshooting Table

| Symptom | Likely Cause | Test | Fix |
|---------|-------------|------|-----|
| No sound at all | No ground connection | Multimeter: continuity from ground terminal to actual earth | Improve ground connection |
| No sound at all | Diode wrong way round | Multimeter: diode forward voltage. Anode is the wire end, cathode is the banded end | Reverse the diode |
| No sound at all | Silicon diode used | Multimeter diode mode: reads >550 mV | Replace with OA91 or 1N34A |
| No sound at all | Enamel not stripped from coil wire | Multimeter resistance across coil: should be 5–30 Ω | Re-strip wire ends |
| No sound at all | Variable capacitor fully meshed (max C) | Try rotating the entire range very slowly | Rotate capacitor |
| No sound at all | Antenna too short (<3 m) | Measure antenna length | Extend antenna |
| Lots of hiss, no station | Everything working but no strong stations nearby | Check AM station listings for your area | Move to time of night, or longer antenna |
| Multiple stations heard at once | Q too low — coil resistance too high | Measure coil DC resistance | Re-wind with thicker wire; use ferrite core |
| Sound cuts in and out | Loose connection in circuit | Wiggle each connection while listening | Solder all joints |
| Sound is very faint even when tuned | Earphone is wrong type (low impedance) | Check earphone: should read very high resistance (>100 kΩ) or be capacitive | Replace with crystal/ceramic earphone |
| Sound is buzzy/harsh | RC filter C1 too small | Check C1 value | Increase C1 to 4.7 nF or 10 nF |
| Stations received but audio is muffled | RC filter C1 too large | Check C1 value | Decrease C1 to 1 nF or 470 pF |
| Cannot tune to the high end of AM band | Tank inductance too high | Calculated resonant frequency at min capacitance | Remove 10–20 turns from coil |
| Cannot tune to the low end of AM band | Tank inductance too low | Calculated resonant frequency at max capacitance | Add 10–20 turns to coil |

---

## Part 17 — How to Improve the Crystal Set

Once you have the basic set working, here are improvements in order of impact:

### 1. Longer, Higher Antenna (Biggest Impact)
An antenna from 10 m to 20 m length roughly doubles the signal. Height of 5 m to 10 m also significantly improves it. This costs nothing — just more wire.

### 2. Better Ground Connection
A copper rod (15mm diameter, 1 metre long) driven into moist earth and connected with a short, thick wire is dramatically better than a counterpoise. If the soil is dry, water around the rod.

### 3. Ferrite Rod Core (Second Biggest Impact)
Replacing an air-core coil with a ferrite rod coil can increase Q from 50 to 150 — tripling selectivity and significantly increasing signal at the detector. It also allows fewer turns, further improving Q.

### 4. Better Variable Capacitor
A good-quality variable capacitor with ball bearings and solid construction has lower losses than a cheap or worn-out one. The capacitor also contributes to the overall circuit Q.

### 5. Silver Mica or Polystyrene Parallel Capacitor
In parallel with the variable capacitor, add a fixed silver mica or polystyrene capacitor. These have very low losses at RF, improving Q slightly. Use a value that brings the minimum tuning frequency to the bottom of the AM band.

### 6. Multiple Tap Positions
Add taps at every 10 turns. Try different taps for the antenna connection — an optimal tap point can increase signal by 3–6 dB.

### 7. Schottky Diode (If No Germanium Available)
If you genuinely cannot obtain a germanium diode, a BAT43 or BAT85 Schottky silicon diode (forward voltage 0.2–0.25 V) is the next best option. It will still miss weak signals that germanium would catch, but it will work on strong local stations.

---

## Part 18 — Expected Performance

What can you realistically receive with a properly built crystal set?

### With a 10 m antenna + good ground + ferrite coil + germanium diode:

| Location | Expected Reception |
|----------|--------------------|
| Within 50 km of a 10 kW+ AM transmitter | Strong, clear audio during day and night |
| 50–200 km from a 10 kW+ AM transmitter | Audible during day, stronger at night |
| 200–500 km (skywave at night) | Often audible at night, signal fades |
| International (1000+ km, skywave at night) | Occasionally audible at night during good propagation |

### With a 5 m indoor wire + counterpoise + air-core coil + germanium diode:

| Location | Expected Reception |
|----------|--------------------|
| Within 20 km of a 50 kW AM transmitter | Audible, possibly clear |
| 20–100 km | Very faint, at night only |
| Beyond 100 km | Unlikely without improvement |

### Best practices for a beginner:

- Start experimenting at night (21:00–02:00 local time)
- Check what AM stations broadcast in your area: use http://www.mwlist.org or similar
- In India: All India Radio broadcasts on multiple medium wave frequencies in most cities
- In the UK: BBC Radio 4 on 198 kHz (long wave — you'd need to retune your coil for this), or BBC local stations on 720–1602 kHz AM

---

## Part 19 — What to Record in Your Notebook

For this build, record:

**Construction log:**
- Date of each step
- Number of turns wound
- Wire diameter used
- Former diameter and length
- Calculated vs measured coil DC resistance
- Estimated inductance (from Wheeler's formula)
- Variable capacitor: minimum and maximum capacitance (if measured)
- Calculated resonant frequency at min and max capacitance

**Tuning log:**
(Create one row for each station received)

| Date/time | Capacitor position (% of rotation) | Estimated frequency | Station ID | Signal strength (1-5) | Audio quality (1-5) | Notes |
|-----------|-------------------------------------|--------------------|-----------|-----------------------|---------------------|-------|

**Improvement experiments:**
For each change you make (new antenna length, different tap, ferrite core), record the before and after signal quality on the same station to assess the improvement quantitatively.

---

## Part 20 — Key Insight: Connecting to Modern Radio Chips

The crystal set contains every functional block of a modern radio receiver, just in its simplest possible form:

| Crystal Set Component | Function | Modern Equivalent in ESP32 Radio |
|-----------------------|----------|-----------------------------------|
| Antenna wire | Captures EM wave, converts to voltage | PCB trace antenna or external antenna SMA connector |
| LC tank (L + C_var) | Bandpass filter — selects one frequency | Bandpass filter in the RF front end; automatic gain control for tuning |
| Germanium diode | Detector — recovers audio envelope | Mixer + ADC + DSP — digital detection |
| RC filter (R1, C1) | Low-pass filter after detection | Digital low-pass filter in firmware |
| Crystal earphone | Converts electrical signal to sound | DAC → amplifier → speaker driver |

The ESP32's 2.4 GHz radio has the exact same functional blocks — but implemented with transistors, ADCs, and digital signal processing instead of passive components. The incoming 2.4 GHz signal is filtered by an LC bandpass filter (integrated into the chip), downconverted by a mixer (a high-speed transistor circuit), detected by an ADC, and processed digitally.

Understanding the crystal set means you understand the functional architecture of every radio that will ever be built.

---

*End of Stage 2. Proceed to Stage 3: LC Resonance — The Heart of All Radio, for a deeper mathematical treatment of the tank circuit and Q factor.*
