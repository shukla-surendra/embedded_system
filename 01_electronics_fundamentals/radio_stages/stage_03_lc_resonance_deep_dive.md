# Stage 3 — LC Resonance: The Heart of All Radio

---

## What You Will Learn

- Why an LC circuit oscillates — the water tank analogy developed fully
- How the resonant frequency formula is derived from energy balance, not just handed to you
- The impedance of L and C at different frequencies — XL and XC — and why they cancel at resonance
- The crucial difference between series and parallel resonance and when to use each
- Q factor in full mathematical depth — what each component of the formula contributes
- How to measure Q experimentally with basic equipment
- Practical coil design: diameter, turns, core material, wire thickness, skin effect, toroids
- Practical capacitor guide: which types are good for RF and why — and which are terrible
- Three build experiments: measuring resonant frequency, changing core material, comparing Q
- Worked design examples for 1 MHz, 7 MHz, and 100 MHz
- How coupling between coils works — mutual inductance and why it matters for transformers and matching networks

**Prerequisites:** Stage 1 (spark gap) and Stage 2 (crystal set). You should be comfortable with Ohm's law and know what inductance and capacitance are.

**Time required:** 3–4 hours reading + 4–5 hours for experiments

---

## Part 1 — The Water Tank Analogy for LC Oscillation

Before doing any maths, you need to feel physically what an LC circuit is doing. The analogy with a water system is exact and worth developing in full detail.

### The Components in the Analogy

**Capacitor = a tank of water (a water reservoir)**

Imagine a large tank with a piston in the middle — like a syringe. The piston can be pushed to one side, storing water (and thus pressure) on one side. When you let go, the pressure pushes the piston back.

- Water volume in the tank = charge on the capacitor (Coulombs)
- Water pressure (height difference between two sides) = voltage across the capacitor
- The spring constant of the piston (how hard it pushes back per unit compression) = 1/C (inverse of capacitance)
- More compliant (bigger C) = lower pressure for the same charge = lower voltage

**Inductor = a heavy flywheel coupled to a pump in the pipe**

Imagine the pipe connecting both sides of the tank contains a heavy flywheel pump. The pump can push water through the pipe, but because of the flywheel's inertia, it cannot start or stop quickly.

- Water flow rate through the pipe = electric current (Amperes)
- Flywheel inertia (resistance to change in speed) = inductance (Henries)
- More inertia (bigger L) = harder to start or stop current = larger inductance

**Resistance = friction in the pipe**

Water flowing through a narrow pipe loses energy to friction. Electrical current flowing through a resistor loses energy to heat. Both cause energy loss per unit time = power dissipation.

### The Oscillation

Now, imagine the piston is pushed over to one side (the capacitor is charged). Then you let go.

What happens?

1. **The pressure drives the pump.** The water pressure in the tank pushes water through the pipe, starting the flywheel spinning. This is analogous to the capacitor voltage driving current through the inductor.

2. **The flywheel accelerates.** Water flow increases (current increases). The pressure in the tank decreases as water transfers from one side to the other.

3. **The tank empties.** When both sides of the tank are at equal level, the pressure is zero. But the flywheel is spinning at maximum speed — it has maximum kinetic energy (maximum current in the inductor). It cannot stop instantly because of inertia.

4. **The flywheel overshoots.** Even with zero pressure driving it, the flywheel keeps pumping water. This water flows to the other side of the tank, now building up pressure there.

5. **The flywheel slows.** As the second side of the tank fills, back-pressure increases, opposing the pump. The flywheel slows.

6. **Maximum reverse pressure.** When the flywheel finally stops, all the water is on the second side. The tank is at maximum pressure again — but reversed.

7. **The cycle repeats in the opposite direction.** Water flows back, flywheel spins the other way, and so on.

This is exactly what the LC circuit does:

| Mechanical | Electrical |
|-----------|-----------|
| Water flows from high to low pressure | Current flows from high to low potential |
| Flywheel spinning = kinetic energy | Current in inductor = magnetic field energy |
| Water filling one side of tank = potential energy | Charge on capacitor = electric field energy |
| Flywheel can't stop instantly | Inductor opposes change in current |
| Tank can't empty instantly | Capacitor opposes change in voltage |
| Friction dissipates energy, oscillation decays | Resistance dissipates energy, oscillation decays |
| Oscillation frequency depends on flywheel inertia and tank compliance | Resonant frequency depends on L and C |

The key point of the analogy: **the energy transfers back and forth between two storage forms** — potential energy (pressure/voltage) stored in the tank/capacitor, and kinetic energy (flow/current) stored in the flywheel/inductor. The frequency is determined by how quickly this transfer happens, which depends on the sizes of the tank and flywheel.

---

## Part 2 — Derivation of Resonant Frequency from Energy Balance

This is where the formula comes from. It is not magic — it follows from the laws governing capacitors and inductors.

### The Laws

For a capacitor:

```
    V_C = Q / C     (voltage = charge / capacitance)
    I_C = C × dV_C/dt     (current = capacitance × rate of change of voltage)
```

For an inductor:

```
    V_L = L × dI/dt     (voltage = inductance × rate of change of current)
```

For a series LC circuit with no resistance, applying Kirchhoff's voltage law:

```
    V_L + V_C = 0     (voltages sum to zero around the loop)

    L × dI/dt + Q/C = 0     (substituting the inductor and capacitor equations)
```

Since I = dQ/dt (current is rate of change of charge):

```
    L × d²Q/dt² + Q/C = 0
```

This is a second-order differential equation. The standard form of this equation (for a harmonic oscillator) is:

```
    d²Q/dt² + (1/LC) × Q = 0
```

The solution to this equation is sinusoidal oscillation:

```
    Q(t) = Q₀ × cos(ω₀ × t)
```

Where ω₀ is the angular frequency of oscillation. Substituting back:

```
    d²/dt² [Q₀ × cos(ω₀t)] + (1/LC) × Q₀ × cos(ω₀t) = 0

    -ω₀² × Q₀ × cos(ω₀t) + (1/LC) × Q₀ × cos(ω₀t) = 0

    (-ω₀² + 1/LC) × Q₀ × cos(ω₀t) = 0
```

This must be true for all values of t (not just when cosine = 0), so the bracket must equal zero:

```
    -ω₀² + 1/LC = 0

    ω₀² = 1/LC

    ω₀ = 1/√(LC)     (angular resonant frequency in radians per second)
```

Converting to ordinary frequency (cycles per second, Hz):

```
    ω₀ = 2πf₀

    2πf₀ = 1/√(LC)

    f₀ = 1 / (2π√(LC))     ← the resonant frequency formula
```

**This formula was not invented arbitrarily. It follows directly from the differential equation of the LC circuit, which follows directly from the definitions of inductance and capacitance. You have now seen where it comes from.**

### The Energy View

At any instant in the LC oscillation, the total energy is constant (assuming no resistance):

```
    E_total = E_capacitor + E_inductor

    E_total = (1/2)CV² + (1/2)LI²
```

At maximum voltage (I = 0):
```
    E_total = (1/2)CV_max²
```

At maximum current (V = 0):
```
    E_total = (1/2)LI_max²
```

Since these are equal (energy is conserved):

```
    (1/2)CV_max² = (1/2)LI_max²

    CV_max² = LI_max²

    V_max / I_max = √(L/C)
```

The ratio V/I is an impedance. The quantity √(L/C) is called the **characteristic impedance** of the LC circuit, measured in Ohms. It appears in the Q factor formula.

---

## Part 3 — Impedance of L and C vs Frequency

### Inductive Reactance

An inductor opposes change in current. At high frequencies, the current is changing rapidly — the inductor presents high resistance-like opposition. At low frequencies, the current changes slowly — the inductor barely resists.

This frequency-dependent "resistance" is called reactance:

```
    XL = 2π × f × L = ω × L     (in Ohms)

Where:
  XL = inductive reactance in Ohms
  f  = frequency in Hz
  L  = inductance in Henries
  ω  = angular frequency = 2πf
```

XL increases linearly with frequency. At DC (f = 0): XL = 0 (inductor is a short circuit for DC). At high frequency: XL → ∞ (inductor is an open circuit at very high frequency).

**For L = 250 µH:**

| Frequency | XL |
|-----------|----|
| 100 Hz | 0.157 Ω |
| 10 kHz | 15.7 Ω |
| 1 MHz (resonance) | 1570 Ω |
| 10 MHz | 15,700 Ω |
| 100 MHz | 157,000 Ω |

### Capacitive Reactance

A capacitor opposes change in voltage. At high frequencies, the voltage changes rapidly — the capacitor presents little opposition (lets current through freely). At low frequencies, the voltage barely changes — the capacitor presents high opposition.

```
        1           1
    XC = ──────── = ────     (in Ohms)
         2πfC      ωC

Where:
  XC = capacitive reactance in Ohms
  f  = frequency in Hz
  C  = capacitance in Farads
```

XC decreases with increasing frequency. At DC (f = 0): XC = ∞ (capacitor is an open circuit for DC). At very high frequency: XC → 0 (capacitor is a short circuit at very high frequency).

**For C = 100 pF:**

| Frequency | XC |
|-----------|----|
| 100 Hz | 15,900,000 Ω |
| 10 kHz | 159,000 Ω |
| 1 MHz (resonance) | 1590 Ω |
| 10 MHz | 159 Ω |
| 100 MHz | 15.9 Ω |

### Why They Cancel at Resonance

In a series LC circuit, the inductor and capacitor voltages have opposite signs (they are 180° out of phase). At resonance, XL = XC, so the two voltages are equal and opposite — they cancel perfectly. The only remaining impedance is the resistance R. The circuit appears to the source as a pure resistance R at resonance — minimum impedance.

In a parallel LC circuit, the currents through L and C have opposite signs. At resonance, they cancel. The only current drawn from the source is the small current through R (losses). The circuit appears to the source as a very high impedance at resonance — maximum impedance.

### The XL = XC Condition

At resonance:

```
    XL = XC

    2πf₀L = 1/(2πf₀C)

    (2πf₀)² = 1/LC

    2πf₀ = 1/√(LC)

    f₀ = 1/(2π√(LC))
```

The same formula, derived a second way — this time from the impedance view rather than the energy view.

---

## Part 4 — Series Resonance vs Parallel Resonance

### Series Resonance

```
    Input ───[R]───[L]───[C]─── Input
              (series LC circuit)
```

Behaviour at resonance:
- XL = XC → they cancel
- Total impedance = R (minimum impedance)
- Current is maximum (I = V/R)
- Voltage across L is very high (XL × I)
- Voltage across C is very high and equal to voltage across L but opposite phase
- Voltage across R = supply voltage

**Analogy:** A series resonant circuit is like a crowbar that shunts all current at the resonant frequency. It is a SERIES PATH with LOW impedance.

**Use cases:**
- In series with the signal path, a series LC circuit is a **notch filter** — it passes all frequencies except the one it resonates at (which sees only R, the small series resistance)
- Actually, for a series LC, it's a **bandpass** for the source — maximum current flows at resonance
- Used in series filter paths to bypass a specific frequency directly to load

### Parallel Resonance

```
    Input ─┬────[L]────┬─ Output
           │           │
          [C]         [R]   ← load
           │           │
    GND  ──┴───────────┴─── GND
```

(Resistance R here represents the losses in the inductor, not a deliberate component. In the tank circuit of a crystal set, no separate R is added — R is the wire resistance of the coil.)

Behaviour at resonance:
- XL = XC → circulating current inside the tank is large, but current from the source is minimum
- Total impedance = very HIGH (approaches R_parallel = Q² × R_series)
- Voltage across the tank is maximum
- Circulating current inside the tank = Q × (supply current)

**Analogy:** A parallel resonant circuit is a reservoir that resonates at one frequency, storing energy at that frequency and presenting an impedance roadblock to everything else.

**Use cases:**
- As a **tank circuit** in a transmitter or receiver — the RF signal builds up across the high impedance at resonance
- As a **bandpass filter** in a signal path — the high impedance at resonance allows a large voltage to develop across the tank at the tuned frequency; off-resonance the tank is low impedance and shorts the signal
- As a **trap** or **filter** to block a specific frequency — the tank in series with the signal path blocks the resonant frequency by presenting high impedance

### Parallel Impedance at Resonance

For a parallel LC circuit with series resistance R in the inductor arm:

```
    Z_parallel(at resonance) ≈ Q × XL = Q × 2πf₀L = Q²R

Where:
  Q = quality factor of the tank
  R = series resistance of the inductor
```

Example: Q=100, f₀=1 MHz, L=250 µH:
```
    XL = 2π × 10⁶ × 250×10⁻⁶ = 1570 Ω
    Z_parallel = 100 × 1570 = 157,000 Ω = 157 kΩ
```

A 157 kΩ impedance at resonance. This is the signal source impedance in the crystal set. This is why the crystal earphone (at 160 kΩ impedance at 1 kHz — see Stage 2) is the right load.

---

## Part 5 — Q Factor: Full Mathematical Treatment

### Definition

```
                   Peak energy stored in the circuit
    Q = 2π × ──────────────────────────────────────────
                   Energy dissipated per cycle
```

This is the fundamental definition. All other forms of the Q formula are derived from this.

### For a Series RLC Circuit

At resonance, the energy oscillates between L and C. At the moment of maximum current (all energy in L):

```
    E_stored = (1/2) × L × I_max²
```

Energy dissipated per cycle (T = 1/f₀ seconds per cycle):

```
    E_per_cycle = I_rms² × R × T = (I_max/√2)² × R × (1/f₀) = I_max² × R / (2f₀)
```

Therefore:

```
             (1/2) × L × I_max²
    Q = 2π × ────────────────────────
              I_max² × R / (2f₀)

             (1/2) × L
    Q = 2π × ─────────────
              R / (2f₀)

          2π × f₀ × L      ω₀ × L     XL
    Q =   ──────────────  = ──────── = ────
               R               R        R
```

Also:

```
          1               XC      1
    Q = ──────── = ────── = ────────────────
         ω₀CR        R      ω₀ × C × R
```

And combining both:

```
          1     √(L/C)     Characteristic impedance
    Q =  ─── × ──────── = ───────────────────────────
          R      1            Series resistance
```

Or written cleanly:

```
         √(L/C)
    Q = ─────────
            R
```

### What Each Term Contributes

- **Increasing L** (more turns, larger coil, or ferrite core): increases Q because the characteristic impedance √(L/C) increases. But more turns also means more wire and higher R — net effect depends on geometry.
- **Decreasing C** (smaller capacitor): increases Q. But the frequency must stay the same, so you must also decrease L proportionally (fewer turns, or remove core). Typically in tuned circuits, the ratio L/C is a design variable.
- **Decreasing R**: directly increases Q. This is the main lever. Use thicker wire (lower DC resistance), use low-loss conductors (silver plating), minimise wire length, avoid materials with dielectric loss.

### Q and Bandwidth

At resonance, the response is at maximum. At frequencies above and below resonance, it falls. The –3 dB bandwidth is:

```
    BW₃dB = f₀ / Q
```

The selectivity (selectance?) is sometimes expressed as a "shape factor" — how much the response drops one channel away. A higher Q gives a steeper skirt.

### Q and Stored Cycles

Another way to think about Q: if you give an LC circuit a kick of energy and let it ring freely, it oscillates sinusoidally while decaying. The number of cycles before the amplitude falls to 1/e (37%) of its original value is Q/π.

```
    N_1/e = Q / π
```

So Q = 50 → 16 cycles before decay to 37%. Q = 200 → 63 cycles. Q = 1000 → 318 cycles.

This is directly observable on an oscilloscope if you can drive a tank circuit briefly and then disconnect the source.

---

## Part 6 — How to Measure Q Experimentally

### Method 1: Bandwidth Method (Requires a Function Generator)

This is the most accurate and straightforward method.

```
SETUP:

Function generator → [R_source (10kΩ)] → LC tank → Oscilloscope

The large source resistor (10kΩ) ensures the generator does not load the tank.
The oscilloscope (or AC voltmeter) measures voltage across the tank.
```

**Procedure:**

1. Set the function generator to a sine wave and sweep the frequency slowly while watching the oscilloscope.
2. Find the resonant frequency f₀ where the voltage across the tank is maximum.
3. Record V_max.
4. Calculate 0.707 × V_max (this is the –3 dB level).
5. Find the frequency above resonance (f_upper) where voltage drops to 0.707 × V_max.
6. Find the frequency below resonance (f_lower) where voltage drops to 0.707 × V_max.
7. BW = f_upper – f_lower.
8. Q = f₀ / BW.

**Worked example:**
- f₀ = 1.00 MHz (resonance)
- f_upper = 1.010 MHz (–3 dB above)
- f_lower = 0.990 MHz (–3 dB below)
- BW = 1.010 – 0.990 = 0.020 MHz = 20 kHz
- Q = 1.00 MHz / 20 kHz = 50

### Method 2: Ring-Down Method (Requires an Oscilloscope)

1. Apply a brief DC pulse to the LC circuit (via a resistor and switch, or a 555 timer one-shot).
2. The circuit rings — you see a decaying sinusoid on the oscilloscope.
3. Count the number of cycles for the amplitude to drop to 37% of its initial value.
4. Q = π × number of cycles.

### Method 3: AM Radio Tuning Sharpness Test (No Instruments)

A qualitative Q test for a crystal set coil:

1. Tune in a station.
2. Record the variable capacitor position.
3. Rotate the capacitor 5–10% above and below the resonant position.
4. Note how far you have to rotate before the station audio drops by half (becomes noticeably quieter).
5. A sharper cutoff = higher Q. If the station remains audible over a wide rotation, Q is low.

---

## Part 7 — Practical Coil Design Guide

### How Diameter Affects Inductance

For a single-layer air-core coil of fixed number of turns and fixed wire gauge, a larger diameter gives higher inductance. This is because:

- Larger diameter = larger cross-sectional area for magnetic flux
- Inductance is proportional to the area enclosed: L ∝ r²n² (from Wheeler's formula)

But a larger coil also has more wire (more resistance) per turn, which tends to keep Q approximately constant. For maximum Q at a given frequency, coil design involves optimising the ratio of diameter to length (optimal ratio is approximately 1:0.9 to 1:1 for maximum Q in a single-layer coil).

### Wheeler's Formula (for Single-Layer Air-Core Coil)

```
         r² × n²
    L = ──────────   (µH, r and l in inches)
         9r + 10l

Where:
  r = coil radius (half the diameter) in inches
  n = total number of turns
  l = coil length (winding length, not coil diameter) in inches
```

This formula is accurate to about 1% for single-layer coils where l > 0.4r.

**Example:** 50mm diameter coil (r = 25mm = 0.984 inch), 80 turns, 50mm long (l = 50mm = 1.97 inch):

```
         0.984² × 80²
    L = ─────────────────
         9×0.984 + 10×1.97

         0.968 × 6400
    L = ─────────────────
              8.856 + 19.7

         6195
    L = ──────
          28.6

    L = 217 µH
```

### Single Layer vs Multi-Layer

**Single layer** (all turns in one row):
- Higher Q at RF — shorter wire for the same inductance (less resistance)
- Lower parasitic capacitance between turns — better at high frequencies
- Preferred for all RF work

**Multi-layer** (turns stacked in multiple rows):
- Higher inductance per unit volume
- More inter-winding capacitance — poor at RF, the "self-resonant frequency" is lower
- Used for audio transformers, power inductors, RF chokes at low frequencies
- Do not use multi-layer for a tuned RF coil

### Effect of Core Material

An air core has relative permeability µr = 1 by definition. Inserting a ferromagnetic core concentrates the magnetic flux and increases the inductance without adding more turns:

```
    L_with_core ≈ µeff × L_air_core

Where µeff is the effective permeability (always less than µr because the core is not a closed loop)
```

**Core materials and their properties:**

| Core Material | µr (typical) | µeff (rod/slug, typical) | Loss at 1 MHz | Best use |
|---------------|-------------|--------------------------|---------------|----------|
| Air | 1 | 1 | Negligible | Any frequency |
| Ferrite (Mn-Zn, grey) | 500–5000 | 50–200 | Low at 100 kHz–10 MHz | AM/SW coils |
| Ferrite (Ni-Zn, green) | 20–2000 | 5–100 | Low at 1 MHz–500 MHz | FM, VHF |
| Powdered iron (red) | 3–75 | 3–50 | Very low up to 100 MHz | HF, FM, VHF |
| Soft iron (steel bolt) | 100–1000 | 5–30 | HIGH (lossy) | Do not use for RF |
| Air (rod) | 1 | 1 | Negligible | VHF and above |

**Key rule:** For AM broadcast band (0.5–1.7 MHz), use Mn-Zn ferrite (grey or black rod). For FM (88–108 MHz), use Ni-Zn ferrite or powdered iron. For VHF/UHF, air core only.

**Why a steel bolt as a core reduces Q:** Steel has high permeability but also high conductivity. At RF frequencies, eddy currents flow in the solid steel, generating heat and consuming energy. This greatly increases the effective resistance of the circuit and collapses the Q. A ferrite core is a poor conductor — eddy currents are negligible.

### Skin Effect and Why Litz Wire Helps at RF

At DC, current flows uniformly throughout a wire's cross-section. At AC, the alternating magnetic field induces opposing currents toward the centre of the wire, pushing current toward the surface. The depth at which current density falls to 1/e of its surface value is the "skin depth":

```
         1
    δ = ─────────────────
         √(π × f × µ × σ)

Where:
  δ = skin depth in metres
  f = frequency in Hz
  µ = permeability of the conductor (≈ µ₀ = 4π × 10⁻⁷ for copper)
  σ = conductivity (copper ≈ 5.8 × 10⁷ S/m)
```

For copper at different frequencies:

```
At f = 1 kHz:   δ = 2.09 mm
At f = 100 kHz: δ = 0.209 mm
At f = 1 MHz:   δ = 0.066 mm = 66 µm
At f = 10 MHz:  δ = 21 µm
At f = 100 MHz: δ = 6.6 µm
```

At 1 MHz, current only flows in the outer 66 µm of the wire. A 0.3 mm wire has a radius of 150 µm — so at 1 MHz, only the outer 66/150 = 44% of the wire carries current. The inner 56% is wasted copper, contributing weight and DC resistance but carrying no AC current.

At 10 MHz, only the outer 21 µm carries current. A 1 mm wire has 500 µm radius — only the outer 4% carries current!

**Consequence:** At HF and above, thick wire has very little advantage over thin wire. The effective "AC resistance" of a thick wire approaches the resistance of a thin tube.

**Litz wire** (from German "Litzendraht" = braided wire): Litz wire consists of many very thin individually-insulated strands braided together. Each strand is thin enough that its diameter is comparable to the skin depth, so current flows throughout the strand. The strands are braided to ensure each spends equal time at the inside and outside of the bundle, equalising current distribution.

Result: lower AC resistance at RF, higher Q coils.

Litz wire is worth using for coils at 100 kHz to 10 MHz. Above 10 MHz the strand diameter for Litz would need to be impractically thin, and air-core coils are used instead.

### How to Wind a Toroidal Inductor

A toroid is a donut-shaped core. A coil wound on a toroid has almost no external magnetic field — all the flux is contained within the core. This means:

- Low coupling to nearby components
- Higher Q than a rod (less flux escapes, less energy lost to the environment)
- More predictable inductance (less sensitive to nearby metal objects)

Toroids are standard in RF circuits and drone ESC motor controllers.

**How to wind:**

1. Get a ferrite toroid core. Standard sizes for HF work: T68 (17 mm diameter), T50 (12.7 mm), T37 (9.4 mm). Type 61 material (Ni-Zn) is good for 1–100 MHz.
2. Pass the wire through the centre hole and back around the outside: this is one turn.
3. Count turns carefully. Write the count in marker on the core.
4. The inductance of a toroid:

```
         AL × n²
    L = ─────────── (in µH, when AL is in µH/100 turns²)
          10,000

Or in terms of AL in nH/turn²:
    L = AL × n²  (in nH)
```

AL is the core's inductance constant, specified on the datasheet. For example, a T50-61 core has AL = 79 nH/turn². To get L = 250 µH = 250,000 nH:

```
         L         250,000
    n = √──── = √──────── = √3,164 = 56.2 ≈ 56 turns
         AL            79
```

---

## Part 8 — Practical Capacitor Guide for RF

### Which Capacitor Types Are Good for RF

The critical parameter is the loss tangent (tan δ), which measures how much energy is wasted as heat per cycle. Low loss tangent = good for RF.

**NP0/C0G ceramic capacitors:**
- Temperature coefficient: nearly zero (NP0 = Negative-Positive-Zero)
- Loss tangent: 0.0001–0.001 (excellent)
- Value stability: very high — value does not change with temperature or voltage
- Available values: 1 pF to 1 µF (most useful range for RF: 1 pF to 10 nF)
- Cost: slightly more than ordinary ceramics
- Identification: marked "NP0" or "C0G" on the package or in the part number
- Use for: tuning capacitors, RF bypass, crystal oscillator load capacitors, filter capacitors

**Silver mica capacitors:**
- Loss tangent: extremely low (0.00001–0.0001) — the lowest of any capacitor type
- Value stability: superb — military-grade circuits use them
- Available values: 1 pF to 1000 pF (mainly small values)
- Identification: pink, red, or cream coloured oval body, often with silver leads
- Use for: high-Q tank circuits, precision filters, crystal oscillator circuits

**Polystyrene capacitors:**
- Loss tangent: very low (0.0001–0.001), nearly as good as silver mica
- Temperature coefficient: slightly negative (~−120 ppm/°C) — can be used to compensate positive-TC inductors
- Available values: 10 pF to 10 nF
- Identification: usually cylindrical, often pale blue or white, small
- Use for: RF tuned circuits, where silver mica values are too large

**Air variable capacitors (the type in a crystal set):**
- Dielectric is air — essentially zero loss
- The limiting loss is the mechanical friction and contact resistance of the wiper
- This is the best possible capacitor for an RF tank circuit

### Which Capacitor Types Are BAD for RF

**Electrolytic capacitors (aluminium electrolytic, tantalum):**
- High loss — the electrolyte has significant resistivity
- Polarised — only work with DC bias
- Value varies with frequency (actual capacitance at 1 MHz is much less than rated value)
- Internal inductance — they resonate at low frequencies and become inductive above that
- NEVER use an electrolytic in a tuned RF circuit. Only use for power supply bypassing (below 1 MHz) and audio coupling.

**X5R, X7R, Y5V ceramic capacitors (multi-layer ceramic, standard grade):**
- These are very common, cheap, and terrible for RF precision
- Value changes dramatically with temperature: X7R ±15%, Y5V +22%/−82% (!!!)
- Value changes with applied DC voltage (piezoelectric effect works in reverse — voltage deforms the ceramic, changing permittivity)
- Loss tangent: higher than NP0 (0.01–0.1)
- Marked "100nF" but actual capacitance varies by 20–50% depending on temperature and voltage
- They are fine for bypass capacitors and audio coupling, where exact value doesn't matter
- NEVER use X7R or Y5V in a tuned RF circuit — the value drift will throw off your resonant frequency

**Disc ceramic (unlabeled):**
- Very common, cheap, found in many hobbyist packs
- Temperature coefficient and tolerance are unknown unless tested
- Loss tangent varies widely
- Avoid in tuned circuits. Use for uncritical bypassing only.

### Stray Capacitance

Every wire, every component lead, every PCB trace has some capacitance to nearby conductors. This "stray capacitance" is typically 1–5 pF per cm of wire. At low frequencies, stray capacitance is negligible. At RF, it matters.

A typical coil has several picofarads of distributed capacitance between turns. When this self-capacitance resonates with the coil's own inductance, you get a "self-resonant frequency" (SRF) — above this frequency the coil behaves as a capacitor, not an inductor.

For a 250 µH RF choke with 10 pF stray capacitance, SRF is:

```
                   1
    f_SRF = ──────────────────────
              2π√(250×10⁻⁶ × 10×10⁻¹²)

                   1
    f_SRF = ──────────────────────
              2π√(2.5×10⁻¹⁵)

                   1
    f_SRF = ──────────────────────
              2π × 5×10⁻⁸

    f_SRF = 1 / (3.14×10⁻⁷) = 3.18 MHz
```

Above 3.18 MHz, this coil resonates with its own stray capacitance and becomes unpredictable. Design rule: use the coil well below its SRF.

---

## Part 9 — Build Experiment 1: Measure Resonant Frequency

**Goal:** Measure the resonant frequency of several LC combinations. Compare measured values to calculated values. This makes the formula real and testable.

**Materials:**
- Your wound coil from Stage 2 (approximately 250 µH)
- Several fixed capacitors: 100 pF, 470 pF, 1 nF, 10 nF (NP0/C0G preferred, ordinary ceramic acceptable for this demonstration)
- A portable AM radio (your detection instrument)
- Optionally: a function generator and oscilloscope

### Method A: Using an AM Radio as the Detector

This is the simplest method and requires no test equipment beyond the AM radio you already have.

**Principle:** You will build an oscillator using your LC circuit and a transistor or 555 timer (or even just the ringing of the coil excited by a brief pulse). The AM radio, tuned to the oscillator frequency, will detect when the LC circuit's resonant frequency matches the radio's tuned frequency.

Alternatively, a simpler method: Connect the LC circuit to a 9V battery through a push button. When you press and release the button quickly, the LC circuit receives a brief energy impulse and rings at its resonant frequency. The damped oscillation radiates a weak signal. The AM radio, placed 30 cm away, will pick this up as a short burst of noise when tuned to f₀.

**Procedure:**

1. Build the LC tank: coil across capacitor, both terminals easily accessible.
2. Connect a wire from one terminal of the tank to a 9V battery via a momentary push button. Connect the other terminal to the battery negative (and also to the radio's vicinity as a "ground" plane).
3. Tune the AM radio to approximately the expected resonant frequency (calculate first — see below).
4. Hold the radio near the LC circuit (within 30 cm).
5. Click the push button once. Listen for a brief click or burst in the radio.
6. Tune the radio up and down while clicking. When the radio is tuned to f₀, the received burst will be loudest.
7. Note the radio dial frequency at maximum received signal = measured f₀.

**Calculations to do before the experiment:**

For each capacitor value C and your measured coil inductance L:

```
              1
    f₀ = ──────────────
          2π × √(L × C)
```

Example with L = 250 µH, C = 100 pF:

```
              1
    f₀ = ──────────────────────────────────────
          2π × √(250×10⁻⁶ × 100×10⁻¹²)

              1
    f₀ = ──────────────────────────────────────
          2π × √(2.5×10⁻¹⁴)

              1
    f₀ = ──────────────────────────────────────
          2π × 1.581×10⁻⁷

              1
    f₀ = ────────────
          9.935×10⁻⁷

    f₀ = 1,006,500 Hz ≈ 1.006 MHz
```

(Within the AM broadcast band — the radio can detect it.)

**Table to fill in:**

| C (pF) | L (µH) | Calculated f₀ (kHz) | Measured f₀ (kHz) | Error (%) |
|--------|--------|--------------------|--------------------|-----------|
| 100 | 250 | 1006 | | |
| 220 | 250 | 678 | | |
| 470 | 250 | 464 | | |
| 1000 | 250 | 318 | | |

Note: 318 kHz and 464 kHz are below the standard AM broadcast band minimum of 530 kHz in most regions. You won't be able to detect these with an AM radio. This is fine — record the result as "below band, not detectable with AM radio."

### Method B: Using a Function Generator and Oscilloscope

**Setup:**

```
Function generator → [R_inject (10kΩ)] → ┬── [L_coil] ──┬ → Oscilloscope Ch1
                                          └── [C_fixed] ──┘
                                                   |
                                                  GND
```

**Procedure:**

1. Set function generator to 500 kHz, 1V peak-to-peak sine wave.
2. Monitor the voltage across the LC tank on the oscilloscope.
3. Slowly increase the frequency while watching the oscilloscope.
4. The voltage across the tank rises to a maximum at f₀ and falls on either side.
5. Record f₀ for each capacitor value.
6. Also observe the 3 dB bandwidth — this gives you Q = f₀/BW.

---

## Part 10 — Build Experiment 2: Changing the Core Material

**Goal:** Observe how inserting different materials into the coil changes the resonant frequency.

**Materials:**
- Your wound coil from Stage 2 (air-core or loosely wound on a former)
- Various materials to insert: air (remove core), pencil (carbon/graphite), wooden rod, iron bolt (M8 or similar), ferrite rod from an old AM radio
- AM radio as detector (using the method from Experiment 1)

**Setup:**
Wind your coil on a former that allows you to slide materials in and out of the coil's centre. A cardboard toilet roll tube is ideal — you can slide rods through the middle.

**Procedure:**

1. Measure f₀ with nothing in the coil (air core). Record this as the reference.
2. Slide a pencil (graphite core) into the centre. Re-measure f₀. Note whether f₀ shifts up or down.
3. Try a wooden rod.
4. Try a steel bolt.
5. Try a ferrite rod (from a dead AM radio or bought separately).
6. For each material, note:
   - Does f₀ shift up or down?
   - Is the shift large or small?
   - Does the signal received on the AM radio get louder or quieter? (Louder = higher Q = less loss)

**Expected Results:**

| Core material | µr (approx) | Effect on inductance | Effect on f₀ | Effect on Q |
|--------------|------------|---------------------|-------------|-------------|
| Air | 1.0 | Baseline | Baseline | Baseline |
| Wood | ~1.0 | Negligible change | Negligible | Negligible |
| Graphite (pencil) | ~1.0 | Negligible | Small downshift (eddy current effect) | Slight decrease |
| Iron bolt (steel) | ~200 | Large increase | Large downshift | Large decrease (eddy current losses) |
| Ferrite rod | ~50–200 (effective) | Moderate to large increase | Moderate to large downshift | Increase (better Q than air for same L) |

**The key observation:** Inserting a ferrite rod decreases f₀ (increases L). This is how tuning slug inductors work — a threaded ferrite core screws in and out of the coil to fine-tune the inductance. Many LC filters in radio equipment use this technique for manufacturing calibration.

**Record in your notebook:**

| Core material | AM radio frequency of maximum signal (kHz) | Relative signal loudness (1-5) |
|--------------|---------------------------------------------|-------------------------------|
| Air | | |
| Wood | | |
| Pencil (graphite) | | |
| Steel bolt | | |
| Ferrite rod | | |

---

## Part 11 — Build Experiment 3: Demonstrating Q Factor

**Goal:** Show that a coil wound with thick wire has higher Q than the same coil wound with thin wire.

**Materials:**
- Two identical formers (two toilet roll tubes or two 50mm PVC pipe sections)
- Thin wire (enamelled, 0.1 mm or 0.15 mm) — 8 metres
- Thick wire (enamelled, 0.5 mm or 0.6 mm) — 8 metres
- Two identical capacitors (any equal values, 100–470 pF)
- Multimeter
- AM radio as detector

**Procedure:**

**Step 1: Wind two identical coils.**
Wind approximately 60–80 turns of thin wire on one former, and 60–80 turns of thick wire on the other former. Aim for the same number of turns and the same winding geometry.

**Step 2: Measure DC resistance.**
Measure the DC resistance of each coil with the multimeter. The thin-wire coil should have significantly higher resistance.

Expected (60 turns, 50mm coil):
- 0.1 mm wire: resistivity calculation → R ≈ 10–20 Ω
- 0.5 mm wire: resistivity calculation → R ≈ 0.4–0.8 Ω

Resistance of copper wire:

```
         ρ × l
    R = ───────
           A

Where:
  ρ = resistivity of copper = 1.72×10⁻⁸ Ω·m
  l = length of wire in metres
  A = cross-sectional area = π × r²

For 60 turns on 50mm diameter coil:
  Circumference per turn = π × 0.05 = 0.157 m
  Total length = 60 × 0.157 = 9.4 m

For 0.1mm wire (r = 0.05mm = 0.00005m):
  A = π × (0.00005)² = 7.85×10⁻⁹ m²
  R = (1.72×10⁻⁸ × 9.4) / (7.85×10⁻⁹) = 1.617×10⁻⁷ / 7.85×10⁻⁹ = 20.6 Ω

For 0.5mm wire (r = 0.25mm = 0.00025m):
  A = π × (0.00025)² = 1.963×10⁻⁷ m²
  R = (1.72×10⁻⁸ × 9.4) / (1.963×10⁻⁷) = 1.617×10⁻⁷ / 1.963×10⁻⁷ = 0.82 Ω
```

**Step 3: Calculate expected Q for each coil.**

Both coils have approximately the same inductance (same turns, same geometry). Let's say L ≈ 150 µH (for 60 turns on 50mm former).

With C = 100 pF:
```
         √(L/C)       √(150×10⁻⁶ / 100×10⁻¹²)
    Q = ─────── = ─────────────────────────────
            R                 R

         √(1.5×10⁶)       1224.7
    Q = ──────────── = ──────────
              R              R

Thin wire (R = 20.6 Ω): Q = 1224.7 / 20.6 = 59.5 ≈ 60
Thick wire (R = 0.82 Ω): Q = 1224.7 / 0.82 = 1493 ← theoretical
```

Wait — 1493 for thick wire? In practice, Q is limited by other losses: skin effect, dielectric losses in the former, radiation resistance. A more realistic Q for the 0.5mm coil at 1 MHz is about 100–200.

**Step 4: Qualitative comparison.**

Build two simple LC tanks: one with the thin-wire coil, one with the thick-wire coil, both with the same 100 pF capacitor. Using the AM radio detection method from Experiment 1, compare:

- The bandwidth of the received signal (how wide is the resonance peak as you tune the radio)
- The signal strength (how loud the received signal is — louder = better Q = more voltage across the tank)

**Record in your notebook:**

| Coil | Wire diameter | DC Resistance | Estimated L (µH) | Calculated Q | Received signal loudness (1–5) | Perceived tuning sharpness |
|------|--------------|--------------|------------------|-------------|-------------------------------|---------------------------|
| Thin wire | 0.1 mm | Ω | | | | |
| Thick wire | 0.5 mm | Ω | | | | |

---

## Part 12 — Coupling Between Coils: Mutual Inductance

### Why This Matters

In a crystal set, the antenna connects to a tap on the coil — this is a form of coupling. Transformers couple energy between circuits. Matching networks use coupled coils to transform impedances. Understanding coupling is essential for these applications.

### Mutual Inductance

When two coils are near each other, the magnetic flux produced by one partially links with the other. The degree of linkage is described by the mutual inductance M:

```
    V₂ = M × dI₁/dt

Where:
  V₂ = voltage induced in coil 2
  M  = mutual inductance in Henries
  dI₁/dt = rate of change of current in coil 1
```

### Coupling Coefficient k

The coupling coefficient k relates M to the individual inductances:

```
    k = M / √(L₁ × L₂)

Where:
  0 ≤ k ≤ 1
  k = 0: no coupling (coils far apart, perpendicular, or shielded)
  k = 1: perfect coupling (theoretical maximum — all flux links both coils)
```

For a transformer wound on a ferrite toroid core: k ≈ 0.95–0.99.
For two adjacent coils on the same rod: k ≈ 0.5–0.9 depending on spacing.
For two coils 10 cm apart: k ≈ 0.01–0.1.
For two coils 1 m apart: k ≈ 0.001 or less.

### Why Distance Matters

k falls off approximately as:

```
    k ∝ 1/r³   (for small coils at distance r >> coil size)
```

Doubling the distance between coils reduces coupling by a factor of 8. This is why antennas need to be far from interfering objects — at 10 wavelengths away, coupling is negligible.

### Reflected Impedance

When coil 2 is connected to a load Z_L, this load appears (partially) in coil 1's circuit as:

```
         (ω × M)²
    Z_reflected = ───────────────
                    Z₂ + Z_L

Where Z₂ is the impedance of coil 2 itself.
```

This is how transformers work: the load on the secondary "reflects" back to the primary, and by choosing the turns ratio (which determines M), you can transform impedances.

---

## Part 13 — Designed Examples for Specific Frequencies

### Design 1: AM Broadcast Band (1 MHz)

**Target:** Resonant frequency f₀ = 1 MHz. Variable capacitor range 40–500 pF.

Find L to place 1 MHz in the middle of the tuning range, say at C = 200 pF (middle of the range):

```
              1
    L = ──────────────
         (2πf₀)² × C

              1
    L = ────────────────────────────────
         (2π × 10⁶)² × 200×10⁻¹²

              1
    L = ────────────────────────────────
         3.948×10¹³ × 2×10⁻¹⁰

              1
    L = ────────────────
         7.896×10³

    L = 1.266×10⁻⁴ H = 126.6 µH ≈ 127 µH
```

Verify tuning range:

At C_min = 40 pF:
```
    f_max = 1/(2π√(127×10⁻⁶ × 40×10⁻¹²)) = 1/(2π × 2.25×10⁻⁸) = 7.07 MHz? 
```

Wait — that's too high. Let me recalculate:

```
    f_max = 1/(2π√(127×10⁻⁶ × 40×10⁻¹²))
          = 1/(2π√(5.08×10⁻¹⁵))
          = 1/(2π × 7.13×10⁻⁸)
          = 1/(4.48×10⁻⁷)
          = 2.23 MHz
```

At C_max = 500 pF:
```
    f_min = 1/(2π√(127×10⁻⁶ × 500×10⁻¹²))
          = 1/(2π√(6.35×10⁻¹⁴))
          = 1/(2π × 2.52×10⁻⁷)
          = 1/(1.583×10⁻⁶)
          = 632 kHz
```

Range: 632 kHz to 2.23 MHz. This covers the full AM broadcast band (530–1710 kHz) with room to spare.

**To wind this coil on a ferrite rod (µeff = 100):**
Effective turns needed is much fewer than air core. Rough guide: with ferrite, use 1/√µeff fewer turns = 1/√100 = 1/10 fewer turns than air core.

For a 127 µH air-core coil on 50mm diameter, Wheeler gives approximately 60 turns.
On ferrite rod (µeff = 100): approximately 6 turns.

In practice, start with 10–15 turns on the ferrite rod and measure/adjust.

### Design 2: Amateur HF Band (7 MHz, 40m band)

**Target:** f₀ = 7 MHz. Typical variable capacitor 10–100 pF.

At C = 50 pF (mid-range):

```
              1
    L = ──────────────────────────────
         (2π × 7×10⁶)² × 50×10⁻¹²

              1
    L = ──────────────────────────────
         (4.398×10⁷)² × 50×10⁻¹²

              1
    L = ──────────────────────────────
         1.934×10¹⁵ × 5×10⁻¹¹

              1
    L = ────────────
         96,700

    L = 1.035×10⁻⁵ H = 10.35 µH ≈ 10 µH
```

For a 10 µH air-core coil on 25mm diameter, Wheeler gives approximately 20 turns.

Tuning range (10 pF to 100 pF):
- f at 10 pF: 1/(2π√(10×10⁻⁶ × 10×10⁻¹²)) = 15.9 MHz
- f at 100 pF: 1/(2π√(10×10⁻⁶ × 100×10⁻¹²)) = 5.03 MHz

Range: 5 to 15.9 MHz — covers the 7 MHz band.

### Design 3: FM Broadcast Band (100 MHz)

**Target:** f₀ = 100 MHz. Typical variable capacitor 5–30 pF.

At C = 10 pF (mid-range):

```
              1
    L = ──────────────────────────────
         (2π × 100×10⁶)² × 10×10⁻¹²

              1
    L = ──────────────────────────────
         (6.283×10⁸)² × 10⁻¹¹

              1
    L = ──────────────────────────────
         3.948×10¹⁷ × 10⁻¹¹

              1
    L = ────────────────
         3.948×10⁶

    L = 2.53×10⁻⁷ H = 253 nH ≈ 250 nH
```

250 nH is a VERY small inductance. At 100 MHz, use an air-core coil. No ferrite (ferrite becomes lossy at 100 MHz).

For 250 nH on a 10mm air-core former (l = 10mm):
Wheeler formula: r = 5mm = 0.197 inch, l = 10mm = 0.394 inch.

```
         r² × n²       0.197² × n²
    L = ────────── = ──────────────────
         9r + 10l     9×0.197 + 10×0.394

         0.0388 × n²
    L = ──────────────
           1.774 + 3.94

         0.0388 × n²
    L = ──────────────
              5.714

    0.250 µH = 0.0388 × n² / 5.714

    n² = 0.250 × 5.714 / 0.0388 = 36.8

    n = 6.1 ≈ 6 turns
```

**A 6-turn air-core coil on a 10mm former gives approximately 250 nH for FM band oscillators.**

This is a tiny coil — 6 turns of 0.5mm silver wire on a 10mm form, winding length about 10mm. This is what FM radio coils look like.

---

## Part 14 — Complete Formulas Sheet

```
RESONANT FREQUENCY:
                  1
    f₀ = ──────────────────    (Hz, L in H, C in F)
          2π × √(L × C)

INDUCTIVE REACTANCE:
    XL = 2πfL = ωL     (Ω)

CAPACITIVE REACTANCE:
           1         1
    XC = ───── = ────────     (Ω)
          2πfC      ωC

CHARACTERISTIC IMPEDANCE OF LC:
    Z₀ = √(L/C)     (Ω)

Q FACTOR (series RLC):
         ωL      1           √(L/C)
    Q = ──── = ────── = ───────────
          R      ωCR           R

Q FACTOR (from bandwidth):
    Q = f₀ / BW₃dB

BANDWIDTH FROM Q:
    BW₃dB = f₀ / Q     (Hz)

PARALLEL IMPEDANCE AT RESONANCE:
    Z_parallel = Q² × R = Q × XL     (Ω)

WHEELER'S FORMULA (single-layer air-core coil):
         r² × n²
    L = ──────────    (µH, r and l in inches)
         9r + 10l

SKIN DEPTH:
         1
    δ = ────────────────     (m, µ in H/m, σ in S/m)
         √(πfµσ)

    For copper at room temperature: δ ≈ 0.066 / √f   (m, f in MHz)

MUTUAL INDUCTANCE:
    M = k × √(L₁ × L₂)     (H)

TRANSFORMER VOLTAGE RATIO:
    V₂/V₁ = N₂/N₁

ENERGY IN CAPACITOR:
    E = ½CV²     (J)

ENERGY IN INDUCTOR:
    E = ½LI²     (J)

COIL RESISTANCE:
         ρ × l
    R = ──────     (Ω, ρ in Ω·m, l in m, A in m²)
           A

    For copper: ρ = 1.72×10⁻⁸ Ω·m

DC RESISTANCE CALCULATION FOR COIL:
    Wire length l = n × π × D    (n = turns, D = coil diameter)
    Cross section A = π × (d/2)² (d = wire diameter)
```

---

## Part 15 — What to Record in Your Notebook

**For each LC tank you build or test:**

1. Coil construction: former diameter, wire diameter, number of turns, winding length, whether air or ferrite core
2. Measured DC resistance of the coil
3. Capacitor used: type, rated value, tolerance
4. Calculated resonant frequency (show full working)
5. Measured resonant frequency (method used)
6. Percentage error between calculated and measured
7. Estimated or measured Q
8. Calculated bandwidth
9. Any anomalies observed

**Experiment tables:** (one per variation)

| Experiment | Variable changed | f₀ calculated | f₀ measured | Q (if measured) | Notes |
|------------|-----------------|--------------|-------------|-----------------|-------|

---

## Part 16 — Troubleshooting Table

| Problem | Likely Cause | Diagnosis | Fix |
|---------|-------------|-----------|-----|
| Measured f₀ much lower than calculated | Stray capacitance or higher-than-expected L | Check coil turns count; measure capacitance of variable cap at "minimum" position — may not be truly zero | Reduce turns, or adjust capacitor range |
| Measured f₀ much higher than calculated | Lower-than-expected L or lower-than-expected C | Recount turns; re-check capacitor value | Add more turns to coil |
| f₀ drifts as you handle the circuit | Components not mechanically stable; hand capacitance effect | Note whether touching the circuit changes the frequency; shield with grounded foil | Secure coil and capacitor; add grounded shield |
| Q seems very low (wide resonance) | Lossy core material; thin wire; or lossy capacitor | Check coil DC resistance; replace with thicker wire or ferrite rod | Improve coil; use NP0 capacitor |
| Coil self-resonates below design frequency | Distributed capacitance between turns too large | Use wider-spaced winding; shorter coil length; use single-layer winding | Re-wind with larger pitch |
| Can't find resonance with AM radio detector | f₀ outside AM band (below 530 kHz) | Calculate f₀ for your LC values | Change C or L to bring f₀ into AM band |

---

## Part 17 — Key Insight: Connecting to Modern Radio Chips

The LC resonator is the core of every radio system, even today. In your ESP32's 2.4 GHz radio:

- The LC tank oscillator in the PLL (phase-locked loop) is built in silicon — the inductors are metal spirals on the chip (square or octagonal spirals in the metal layers), and the capacitors are varactors (voltage-controlled capacitors). The Q of on-chip inductors is typically only 5–15, far less than a hand-wound coil. To compensate, the PLL uses active circuitry to synthesise a low-noise stable frequency.

- In the RF matching network at the antenna connector, discrete LC components are used — chip inductors (wound on tiny ceramic formers, 0402 or 0603 package, sub-nH to 100 nH values) and chip capacitors (NP0 ceramic, 0.5 pF to 10 pF). These match the antenna impedance (50 Ω for a coaxial antenna) to the chip's internal impedance.

- When you add an external antenna to your ESP32 drone, you are extending the effective antenna and changing the capacitive loading. The matching network must be adjusted accordingly.

- The bandpass filters in 2.4 GHz WiFi/Bluetooth chips use LC resonators with Q values of 20–100 at 2.4 GHz to separate the desired band from adjacent interference (5.8 GHz FPV video, 2.4 GHz microwave ovens, other WiFi systems).

Your crystal set experiments are the foundation for understanding every one of these systems.

---

*End of Stage 3. Proceed to Stage 4: The Diode Detector — Seeing Inside the Signal, for a deep look at detection physics and how to build a galena crystal detector.*
