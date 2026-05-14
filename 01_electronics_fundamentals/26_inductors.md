# Inductors — Complete Theory, Types, Circuits & Experiments

## Table of Contents

1. [What Is an Inductor](#1-what-is-an-inductor)
2. [The Physics — Magnetic Fields](#2-the-physics--magnetic-fields)
3. [Faraday's Law — Where the Voltage Comes From](#3-faradays-law--where-the-voltage-comes-from)
4. [Lenz's Law — Why Inductors Resist Change](#4-lenzs-law--why-inductors-resist-change)
5. [Inductance — The Fundamental Quantity](#5-inductance--the-fundamental-quantity)
6. [The Inductor Equation: V = L·dI/dt](#6-the-inductor-equation-v--ldidt)
7. [Energy Stored in an Inductor](#7-energy-stored-in-an-inductor)
8. [Inductors in DC Circuits — RL Circuits](#8-inductors-in-dc-circuits--rl-circuits)
9. [Inductors in AC Circuits — Inductive Reactance](#9-inductors-in-ac-circuits--inductive-reactance)
10. [Inductors and Capacitors Together — LC Circuits](#10-inductors-and-capacitors-together--lc-circuits)
11. [Mutual Inductance and Transformers](#11-mutual-inductance-and-transformers)
12. [Real Inductor Parameters](#12-real-inductor-parameters)
13. [Types of Inductors](#13-types-of-inductors)
14. [Reading Inductor Markings](#14-reading-inductor-markings)
15. [Inductors in Real Circuits](#15-inductors-in-real-circuits)
16. [Flyback Voltage — The Dangerous Spike](#16-flyback-voltage--the-dangerous-spike)
17. [Buck and Boost Converters](#17-buck-and-boost-converters)
18. [Home Experiments](#18-home-experiments)
19. [Quick Reference](#19-quick-reference)

---

## 1. What Is an Inductor

An inductor is a coil of wire. That's it physically.

```
Simple inductor:

    ~~~~
   /    \
  |      |   ← one loop of wire
   \    /
    ~~~~

Multi-turn coil:

   ___   ___   ___   ___
  /   | /   | /   | /   \
 |    |/    |/    |/     |
  \   |\    |\    |\    /
   ---|  ---|  ---|  ---

Symbol in schematics:

  ——UUUU——     (bumps represent the coil loops)

Or more modern:

  ——[    ]——   (rectangle, especially for chokes/ferrite)
```

When current flows through a coil, it creates a magnetic field. When that magnetic field changes, it creates a voltage. These two facts — plus the conservation of energy — explain everything an inductor does.

### Inductor vs Resistor vs Capacitor

| Component | Stores energy as | Resists change in | Opposes |
|-----------|-----------------|-------------------|---------|
| Resistor  | Nothing (dissipates) | Current (always) | Current proportional to voltage |
| Capacitor | Electric field   | Voltage           | Voltage changes |
| Inductor  | Magnetic field   | Current           | Current changes |

The capacitor and inductor are duals of each other:
- Capacitor: voltage can't change instantaneously, current can
- Inductor: current can't change instantaneously, voltage can

This duality appears everywhere in circuit analysis.

---

## 2. The Physics — Magnetic Fields

### What Creates a Magnetic Field

Moving electric charges (current) create magnetic fields. This is one of the fundamental laws of electromagnetism.

A single straight wire carrying current creates a magnetic field that circles around the wire:

```
Wire going INTO page (current away from you):
         ↑  B-field
    ←         →
         ↓

Right-hand rule:
Point thumb in direction of current.
Fingers curl in direction of B-field.

      I
      ↑
      |         B-field lines circle
    ←-+→        around the wire like
      |         rings on a rope
    
```

The field strength falls off with distance from the wire:
```
B = μ₀I / (2πr)

Where:
  μ₀ = 4π × 10⁻⁷ T·m/A  (permeability of free space)
  I  = current in amperes
  r  = distance from wire in meters
  B  = magnetic field strength in Tesla
```

### What a Coil Does

Wrapping the wire into a coil (solenoid) makes all the individual fields add together:

```
Current going around loops:

    ← ← ← ← ←
   ↑             ↓
N  |  =====>   | S
   ↑   B field ↓
    → → → → →

Each loop's B-field points the same direction inside the coil.
All fields add together → much stronger field than one wire.

The field looks exactly like a bar magnet:
  N pole at one end, S pole at other end.
```

The strength of the field inside a solenoid:
```
B = μ₀ × μᵣ × N × I / l

Where:
  μ₀ = 4π × 10⁻⁷ (free space permeability)
  μᵣ = relative permeability of core material
  N  = number of turns
  I  = current (Amperes)
  l  = length of coil (meters)
```

### Magnetic Permeability

The core material inside the coil dramatically affects field strength:

| Core material | Relative permeability (μᵣ) |
|---------------|---------------------------|
| Air / vacuum  | 1                         |
| Aluminum      | ~1.000022 (barely above 1)|
| Ferrite (MnZn)| 100 – 10,000              |
| Iron (silicon steel) | 1,000 – 10,000     |
| Permalloy (Ni-Fe) | up to 100,000          |

This is why inductors use ferrite or iron cores — they multiply the field strength by thousands, giving you a large inductance with few turns.

---

## 3. Faraday's Law — Where the Voltage Comes From

Michael Faraday discovered in 1831 that **a changing magnetic field creates a voltage** (EMF — electromotive force).

Precisely:

```
EMF = −N × dΦ/dt

Where:
  EMF = induced voltage (volts)
  N   = number of turns
  Φ   = magnetic flux through one loop (Webers = V·s)
  dΦ/dt = rate of change of flux

The minus sign is Lenz's law (see next section).
```

**Magnetic flux** is how much field passes through a loop:
```
Φ = B × A × cos(θ)

Where:
  B = field strength (Tesla)
  A = area of loop (m²)
  θ = angle between field and loop normal

Maximum flux when field is perpendicular to loop (θ=0, cos=1).
Zero flux when field is parallel to loop (θ=90°, cos=0).
```

### How This Creates Inductor Voltage

In an inductor, the current you pass through it creates the field. When that current changes → field changes → flux changes → voltage appears across the coil.

This is a self-generated voltage, which is why it's called **back-EMF** or **self-induced EMF**.

The sequence:
```
Current changes
      ↓
Magnetic field changes
      ↓
Flux through coil changes
      ↓
Faraday's law: EMF = −N × dΦ/dt
      ↓
Voltage appears across inductor terminals
```

---

## 4. Lenz's Law — Why Inductors Resist Change

The minus sign in Faraday's law has a physical meaning: **the induced voltage always opposes the change that caused it**.

This is **Lenz's law** — it's a consequence of energy conservation.

```
Scenario: You increase current through an inductor.

Step 1: Current I increases → Magnetic field B increases
Step 2: Increasing B → increasing flux Φ
Step 3: Faraday: dΦ/dt > 0 → EMF induced
Step 4: Lenz: EMF opposes the increase → induces a voltage
        that would push current BACKWARD (opposing the increase)

This opposing voltage is the "back-EMF"

Result: The inductor doesn't STOP the current from changing —
        it just slows down the change. You must "fight" the
        inductor to change its current.
```

Think of an inductor as **electromagnetic inertia**. Just as mass resists changes in velocity, an inductor resists changes in current. A heavy object is hard to start moving (and hard to stop). An inductor is hard to start with current (and hard to stop).

```
Mechanical analogy:

Mass (kg)          ↔   Inductance (H)
Velocity (m/s)     ↔   Current (A)
Force (N)          ↔   Voltage (V)
Momentum (kg·m/s)  ↔   Flux linkage (Wb = V·s)

F = m × dv/dt      ↔   V = L × dI/dt
```

---

## 5. Inductance — The Fundamental Quantity

**Inductance (L)** measures how strongly a coil resists changes in current.

Unit: **Henry (H)** — named after Joseph Henry, who independently discovered electromagnetic induction.

Common values:
```
1H    = very large (power transformers, audio chokes)
1mH   = 10⁻³ H  (common in RF circuits)
1μH   = 10⁻⁶ H  (common in switching power supplies)
1nH   = 10⁻⁹ H  (PCB traces, bond wires, RF)
```

### What Determines Inductance

```
L = μ₀ × μᵣ × N² × A / l

Where:
  L  = inductance (Henry)
  μ₀ = 4π × 10⁻⁷ H/m (permeability of free space)
  μᵣ = relative permeability of core (1 for air)
  N  = number of turns
  A  = cross-sectional area of core (m²)
  l  = length of coil (m)
```

Notice: **L ∝ N²** — inductance goes up as the SQUARE of turns. Double the turns → four times the inductance.

Also: **L ∝ μᵣ** — putting iron or ferrite core in multiplies L by μᵣ.

### Practical Example

Air-core coil: 20 turns, diameter 1cm, length 2cm:
```
A = π × (0.005)² = 7.85 × 10⁻⁵ m²
L = (4π × 10⁻⁷) × 1 × 20² × 7.85×10⁻⁵ / 0.02
L = (4π × 10⁻⁷) × 400 × 7.85×10⁻⁵ / 0.02
L ≈ 0.197 μH  (about 200nH)

Same coil with ferrite core (μᵣ = 200):
L = 0.197μH × 200 = 39.4 μH  (200x more!)
```

---

## 6. The Inductor Equation: V = L·dI/dt

This is the fundamental inductor equation:

```
V = L × dI/dt

Where:
  V    = voltage across inductor (Volts)
  L    = inductance (Henry)
  dI/dt = rate of change of current (Amperes per second)
```

**Reading this equation:**

| Situation | dI/dt | Voltage |
|-----------|-------|---------|
| Current increasing fast | Large positive | Large positive V (opposing increase) |
| Current increasing slow | Small positive | Small positive V |
| Constant current | Zero | Zero V (inductor = short circuit!) |
| Current decreasing fast | Large negative | Large negative V (now it's a source!) |
| Instantaneous change | Infinite | Infinite V (impossible — something breaks) |

### The Key Insight

- **At DC (steady state):** dI/dt = 0 → V = 0. An inductor with steady current acts like a piece of wire (just its resistance).

- **At the moment of switching:** The inductor creates whatever voltage is needed to try to keep current constant. This can be thousands of volts if you break the circuit suddenly — that's the flyback spike.

### Rearranged for current

```
I(t) = (1/L) × ∫V dt  +  I(0)

Current = (1/L) × integral of voltage over time, plus initial current.

This means:
  - You need sustained voltage to build up current in an inductor.
  - Current changes gradually, not instantly.
  - If you apply constant V across L:
    dI/dt = V/L = constant → current rises linearly
```

### Worked Example

A 10μH inductor has 1A flowing. You suddenly apply 5V across it.

How fast does current rise?
```
dI/dt = V/L = 5 / (10 × 10⁻⁶) = 500,000 A/s = 0.5 A/μs

After 1μs: current = 1A + 0.5A = 1.5A
After 2μs: current = 2A
```

What if you break the circuit when 1A is flowing and L = 10μH?

The current must stop (from 1A to 0A). If it takes 1μs:
```
dI/dt = −1A / 1μs = −1,000,000 A/s

V = L × dI/dt = 10×10⁻⁶ × 1,000,000 = 10V

If it takes 1ns (spark gap): V = 10×10⁻⁶ × 10⁹ = 10,000 V!
```

This is exactly why disconnecting a relay or motor without a flyback diode destroys transistors.

---

## 7. Energy Stored in an Inductor

An inductor stores energy in its magnetic field:

```
E = ½ × L × I²

Where:
  E = energy in Joules
  L = inductance in Henry
  I = current in Amperes
```

Compare to capacitor:
```
E_capacitor = ½ × C × V²
E_inductor  = ½ × L × I²

Perfect mathematical duality: C↔L, V↔I
```

### Example: Energy in a relay coil

Typical relay coil: L = 50mH, current = 100mA:
```
E = ½ × 0.05 × (0.1)² = ½ × 0.05 × 0.01 = 0.00025 J = 250 μJ

Seems tiny. But if released in 1μs (spark):
Power = 0.00025 / 0.000001 = 250 Watts!

In a nanosecond: 250,000 Watts. That's a spike.
```

This energy must go somewhere. If there's no path (no flyback diode), it goes into a voltage spike that destroys the switching transistor.

---

## 8. Inductors in DC Circuits — RL Circuits

### The RL Circuit (Resistor + Inductor + DC Source)

```
     Switch
      /
  ───/──────┬──[R]──┬───
  |          |       |
 [V]         |      [L]
  |          |       |
  ────────────┴───────┘
```

When the switch closes at t=0:

```
Current rise (charging):

I(t) = (V/R) × (1 − e^(−t/τ))

Where:
  τ = L/R  (time constant, in seconds)
  V/R = final steady-state current (just Ohm's law)

Voltage across inductor during rise:
V_L(t) = V × e^(−t/τ)
```

### Visualizing the Rise

```
I (current)
  ^
V/R|- - - - - - - - - - - - - ←  final value
   |              .......----
   |       .....--
   |    ...
   |  ..
   | .
   |.
   +-----|-----|-----|-----|--→ t
         τ    2τ    3τ    4τ

At t=τ:   I = 0.632 × (V/R)   [63.2% of final]
At t=2τ:  I = 0.865 × (V/R)
At t=3τ:  I = 0.950 × (V/R)
At t=5τ:  I = 0.993 × (V/R)  (considered "fully charged")
```

```
V_L (voltage across inductor)
  ^
V |. ← starts at full supply voltage
  | ..
  |   ..
  |     ...
  |         .....
  |               .......-------
   0|-----|-----|-----|------→ t
         τ    2τ    3τ

Inductor voltage starts high, decays exponentially.
At DC steady state: V_L = 0 (all voltage across R).
```

### Worked Example

L = 10mH, R = 100Ω, V = 5V:
```
τ = L/R = 0.01 / 100 = 0.1ms = 100μs

Final current = V/R = 5/100 = 50mA

At t=100μs: I = 50mA × 0.632 = 31.6mA
At t=500μs: I = 50mA × 0.993 ≈ 50mA (essentially done)
```

### When Switch Opens (Current Decay)

```
I(t) = I₀ × e^(−t/τ)

The inductor tries to maintain current.
If there's no path, it creates whatever voltage needed.
```

```
I
  ^
I₀|.
  | ..
  |   ..
  |     ....
  |          .....
  |               .........
   +-----|-----|-----|------→ t
         τ    2τ    3τ

Current decays exponentially, time constant still L/R
(but R is now whatever resistance is in the decay path)
```

---

## 9. Inductors in AC Circuits — Inductive Reactance

When AC flows through an inductor, the inductor opposes it — but differently from a resistor.

### Inductive Reactance

```
X_L = 2πfL = ωL

Where:
  X_L = inductive reactance (Ohms)
  f   = frequency (Hz)
  L   = inductance (Henry)
  ω   = 2πf = angular frequency (rad/s)
```

Like resistance, reactance is measured in Ohms. Unlike resistance, it **varies with frequency**.

| Frequency | X_L for 1mH | X_L for 100μH | X_L for 10μH |
|-----------|-------------|---------------|--------------|
| 50 Hz     | 0.314 Ω     | 0.031 Ω       | 0.003 Ω      |
| 1 kHz     | 6.28 Ω      | 0.628 Ω       | 0.063 Ω      |
| 100 kHz   | 628 Ω       | 62.8 Ω        | 6.28 Ω       |
| 1 MHz     | 6280 Ω      | 628 Ω         | 62.8 Ω       |
| 100 MHz   | 628 kΩ      | 62.8 kΩ       | 6.28 kΩ      |

**High frequency → high reactance → inductor blocks high frequencies**

This is the opposite of a capacitor:
```
Capacitor: X_C = 1/(2πfC) → blocks LOW frequencies, passes HIGH
Inductor:  X_L = 2πfL    → blocks HIGH frequencies, passes LOW
```

### Phase Shift — Current Lags Voltage

In a pure inductor, the voltage leads the current by 90°:

```
Voltage:  — — _____/‾‾‾‾‾‾\_____ /‾‾‾
Current:  — — — ___/‾‾‾‾‾‾\_____ /

Voltage peaks BEFORE current (V leads I by 90°, or "I lags V by 90°")
```

**Memory trick: ELI the ICE man**
- **E**LI: in an inductor (L), **E**MF (E) leads **I** current
- I**CE**: in a capacitor (C), **I** current leads **E**MF

### Impedance (Combined R and X_L)

In a real inductor (which has both inductance AND resistance):
```
Z = √(R² + X_L²)    (magnitude)

Phase angle: φ = arctan(X_L / R)

At low f: Z ≈ R (resistance dominates)
At high f: Z ≈ X_L (reactance dominates)
```

### Power in an Inductor

A pure inductor dissipates zero average power! It stores energy during one half-cycle and returns it during the next.

```
Instantaneous power: p(t) = V(t) × I(t)

Half cycle: p > 0 (energy going into magnetic field)
Next half:  p < 0 (energy returned from field)
Average:    P_avg = 0

This is called "reactive power" — it sloshes back and forth
but does no work and heats nothing.
```

Real inductors have winding resistance (DCR) which does dissipate power:
```
P_dissipated = I² × DCR
```

---

## 10. Inductors and Capacitors Together — LC Circuits

Put a capacitor and inductor together and you get one of the most important circuits in electronics: the **LC resonant circuit** (also called a tank circuit).

### How Resonance Works

```
Start: Capacitor fully charged to voltage V, inductor current = 0

Step 1: Capacitor discharges through inductor
        Current builds up in inductor
        Energy moves from E-field (cap) to B-field (inductor)

Step 2: Capacitor fully discharged, inductor at peak current
        Now inductor keeps current flowing, charges cap in OPPOSITE direction
        Energy moves from B-field (inductor) back to E-field (cap, reversed)

Step 3: Cap fully charged (reverse polarity), inductor = 0
        Process repeats in reverse

Step 4: Full oscillation — back to start

This cycle repeats forever (if no resistance to steal energy)
```

```
V_cap:   +A sin(ωt)          ← oscillates
I_ind:   +A cos(ωt)          ← 90° out of phase

Energy:
E_cap = ½CV²  oscillates between full and zero
E_ind = ½LI²  oscillates between zero and full
Total = constant (energy conserved, just moves back and forth)
```

### Resonant Frequency

```
f₀ = 1 / (2π√(LC))

ω₀ = 1 / √(LC)

Where:
  f₀ = resonant frequency (Hz)
  L  = inductance (Henry)
  C  = capacitance (Farads)
```

At this frequency: X_L = X_C (reactances cancel)

### Example: AM Radio Tuner

AM radio: 540 kHz to 1700 kHz. To tune to 1 MHz:

```
Pick L = 250μH (typical ferrite rod antenna)
Solve for C:

f₀ = 1 / (2π√(LC))
1×10⁶ = 1 / (2π × √(250×10⁻⁶ × C))
√(250×10⁻⁶ × C) = 1 / (2π × 10⁶) = 1.59 × 10⁻⁷
250×10⁻⁶ × C = 2.53 × 10⁻¹⁴
C = 101 pF

(This matches the range of typical AM variable capacitors: 10–365pF)
```

Tuning the capacitor shifts the resonant frequency — you're literally moving through AM stations.

### Parallel vs Series LC

```
SERIES LC:                    PARALLEL LC (tank circuit):

   ——[L]——[C]——               ——┬——[L]——┬——
                                |         |
                                └——[C]——┘

At resonance:                 At resonance:
Z is MINIMUM (≈ 0)            Z is MAXIMUM (→ ∞ for ideal)
Passes the resonant freq      Blocks the resonant frequency
→ Series resonant filter      → Parallel resonant filter / tank

Used in: bandpass filters,    Used in: oscillators, antenna
crystal oscillators           tuners, IF filters
```

### Q Factor (Quality Factor)

Q tells you how sharp the resonance peak is:
```
Q = f₀ / BW = X_L / R = (1/R) × √(L/C)

Where:
  f₀ = resonant frequency
  BW = bandwidth (−3dB points)
  R  = total resistance in circuit

High Q → sharp, narrow resonance → selective filter
Low Q  → broad, flat resonance → less selective
```

```
Frequency response:

Low Q (Q=1):          High Q (Q=10):
    |                      |
    |   /‾‾‾\              |      /\
    |  /     \             |     /  \
    |_/       \_           |____/    \____
    
    Broad peak            Sharp peak

Radio receivers want HIGH Q to select one station.
Power supplies want LOW Q to handle a range of frequencies.
```

---

## 11. Mutual Inductance and Transformers

### Mutual Inductance

When two coils are placed near each other, the changing magnetic field of one induces a voltage in the other. This is **mutual inductance (M)**:

```
V₂ = M × dI₁/dt

Where:
  V₂ = voltage induced in coil 2
  M  = mutual inductance (Henry)
  I₁ = current in coil 1
```

The coupling coefficient k (0 to 1) describes how tightly the two coils are coupled:
```
M = k × √(L₁ × L₂)

k = 0:   no coupling (coils far apart or perpendicular)
k = 0.5: loose coupling (typical air-core transformers)
k ≈ 1:   tight coupling (ferrite core transformers)
```

### The Transformer

A transformer is two (or more) coils wound on the same core — maximum coupling (k ≈ 1).

```
Primary coil         Core         Secondary coil
(N₁ turns)                       (N₂ turns)

     ||||||||||||         ||||||||||||
  ———||||||||||||—————————||||||||||||———
     ||||||||||||         ||||||||||||

Input voltage V₁                Output voltage V₂
Input current I₁                Output current I₂
```

### Transformer Equations

**Voltage ratio:**
```
V₂ / V₁ = N₂ / N₁

V₂ = V₁ × (N₂/N₁)

N₂ > N₁ → Step-UP transformer (V₂ > V₁)
N₂ < N₁ → Step-DOWN transformer (V₂ < V₁)
```

**Current ratio (ideal transformer, 100% efficiency):**
```
I₂ / I₁ = N₁ / N₂

Power in = Power out (for ideal):
V₁ × I₁ = V₂ × I₂

More voltage out → less current out (and vice versa)
```

**Impedance transformation:**
```
Z₁ / Z₂ = (N₁/N₂)²

A transformer can make any load look like any impedance.
Used in audio amplifiers to match speakers (8Ω) to amplifier output.
```

### Example: 230V to 5V Phone Charger Transformer

```
V₁ = 230V (mains), V₂ = 5V, N₁/N₂ = 230/5 = 46

If I₂ = 2A (10W load):
I₁ = I₂ × N₂/N₁ = 2 × (1/46) = 43mA from mains

Load impedance: Z₂ = 5V/2A = 2.5Ω
Reflected to primary: Z₁ = 2.5 × 46² = 5290Ω
(This is what the wall socket "sees")
```

### Core Saturation

Every magnetic core has a limit to how much flux it can carry. Push too hard and it **saturates**: the core can't hold more flux, inductance collapses, current skyrockets.

```
B (flux density)
  ^
Bs|. . . . . . . . . . . .←saturation
  |              ____----
  |         ----
  |    ----
  |---
  +------------------------→ H (field strength = N×I/l)

Linear region: B ∝ H (constant μ, predictable inductance)
Saturation: B stops increasing even as H increases
             → μ → 0 → L → 0 → current spikes
```

Saturation is usually destructive in power circuits. Inductors have a **saturation current** rating — never exceed it.

---

## 12. Real Inductor Parameters

When buying or using an inductor, you'll see these parameters on the datasheet:

### Inductance (L)

Nominal inductance in μH, mH, or H. Usually measured at a specific frequency (1kHz or 100kHz typically).

Standard values follow E12 or E24 series: 1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2, 10...

### Tolerance

How accurate the inductance is:
```
±5%   (J) — good
±10%  (K) — common
±20%  (M) — cheap parts
```

Unlike resistors (where 1% is easy), inductors with tight tolerance are expensive. Most designs are tolerant to ±20%.

### DC Resistance (DCR)

The resistance of the wire itself. Every inductor has it:
```
DCR causes: I²R heating, voltage drop, reduced Q
Typical: 0.01Ω (power inductors) to 100Ω (small signal)
Better inductors have lower DCR (thicker wire, fewer turns needed with better core)
```

### Rated Current / Saturation Current (I_sat)

Two different ratings — both matter:

**DC current rating (I_DCR):** current at which temperature rise reaches a limit (usually 40°C above ambient). Limited by DCR heating.

**Saturation current (I_sat):** current at which inductance drops by a certain amount (usually 10% or 20%). Limited by core saturating.

```
Always choose an inductor where your operating current < both ratings.
In switching power supplies, peak current must be < I_sat.
```

### Self-Resonant Frequency (SRF)

Every inductor has parasitic capacitance between its turns. This creates an internal LC circuit:

```
Ideal inductor:   ——[L]——

Real inductor:    ——[L]——
                      |
                     [C_parasitic]  (between turns)
                      |
```

At the **self-resonant frequency (SRF)**, the inductor acts like an LC tank circuit (very high impedance). Above the SRF, the component becomes capacitive — it no longer behaves like an inductor!

```
Rule: Use inductors well below their SRF.
Typically: operating frequency < SRF/10 for good inductor behavior.

SRF for common inductors:
  10μH power inductor: SRF ≈ 10-50 MHz
  100nH RF inductor:   SRF ≈ 500 MHz - 2 GHz
  1nH PCB trace:       SRF > 10 GHz
```

### Q Factor (Quality Factor)

```
Q = X_L / R_total = 2πfL / (DCR + R_core_loss)

High Q → efficient inductor, low loss
Low Q  → lossy, heats up

Typical Q values:
  Cheap power inductor:      Q = 20-50
  Good RF inductor:          Q = 50-200
  Air-core silver coil:      Q = 200-500
  Quartz crystal (resonator): Q = 10,000 – 1,000,000
```

### Impedance vs Frequency Plot

Every datasheet shows this — learn to read it:

```
Impedance
(Ω, log scale)
    |                      SRF
  1k|                       *
    |                    * | *
 100|              ***** |   *****  ← capacitive region
    |           ***      |         (don't use here!)
  10|         **         |
    |       **           |
   1|      *             |
    |     *              |
  0.1|   DCR             |
    +------|-------------|----------→ Frequency (log)
         100kHz         10MHz

Rising slope = inductive behavior (X_L = 2πfL)
Peak = SRF
Falling slope = capacitive behavior (avoid!)
Flat bottom = DCR dominates (low frequency)
```

---

## 13. Types of Inductors

### Air-Core Inductor

```
   ___   ___   ___
  /   \_/   \_/   \
 |                  |
  \___/‾\___/‾\___/

No magnetic core inside — just air.
```

| Property | Value |
|----------|-------|
| Inductance | Very low (nH range) |
| SRF | Very high (GHz range) |
| Q | High (no core losses) |
| Saturation | Never saturates (no core) |
| Size | Large for given inductance |
| Use | RF circuits, VHF/UHF, antenna matching |

Advantages: no saturation, no core loss, excellent at high frequencies
Disadvantages: needs many turns for large L, not shielded (radiates/picks up)

### Ferrite Core Inductor

Ferrite is a ceramic magnetic material (iron oxide + other oxides). Available in many grades for different frequency ranges:

```
MnZn ferrite: high permeability, good below 1MHz (power applications)
NiZn ferrite: lower permeability, good 1MHz – 1GHz (RF, EMI)
```

Common shapes:

```
TOROID:              E-CORE:              POT CORE:
  _____               ___   ___          _________
 /     \             |   | |   |        |         |
|   O   |           -|   |-|   |-       |    O    |
 \_____/             |___| |___|        |_________|

Self-shielding        Easy to wind,       Fully shielded,
(field stays inside)  common, gapped      high Q
Best for EMI          for power supplies
```

### Toroidal Inductor

The torus (donut) shape is special: the magnetic field stays almost entirely inside the core. Almost no external field → almost no electromagnetic interference (EMI).

```
Cross-section:
         N wires wound
         around torus
            ||||
    _____   ||||   _____
   /     \  ||||  /     \
  | inside| ← B-field stays inside
   \_____/  ||||  \_____/
            ||||

Benefits:
  - Very low EMI (great for audio, power)
  - High efficiency (no air gap leakage)
  - Compact for given inductance

Drawbacks:
  - Tedious to wind (thread wire through hole each turn)
  - Can't easily add air gap for saturation control
```

### Power Inductor (Shielded/Unshielded)

SMD power inductors for switching power supplies. They look like this on a PCB:

```
Unshielded:    Shielded:
  _____           _____
 |     |         |/////|
 |  ∪∪ |         |/////|   ← ferrite cage around winding
 |_____|         |_____|

Shielded: lower EMI, more expensive
Unshielded: cheaper, more EMI, but fine for many applications
```

Typical part numbers: CDRH6D28 (6mm×6mm×2.8mm), SRR1260

### Ferrite Bead (Chip Bead)

Looks like a resistor/capacitor on a PCB. It's an inductor optimized to be LOSSY at high frequencies — it converts high-frequency noise to heat.

```
Symbol:   ——[  ]——    (rectangle, like a resistor)
           Ferrite bead

Behavior:
  At low f: very low impedance (passes DC and low-freq signals)
  At high f: high impedance (absorbs/blocks RF noise)

Common use:
  On VCC power supply lines to ESP32, MCU
  On USB data lines
  On high-speed signal lines

Typical values: 100Ω @ 100MHz, 600Ω @ 100MHz, 1kΩ @ 100MHz
(specified at a frequency, not as inductance — because at that frequency it's mostly resistive)
```

Your ESP32 devkit almost certainly has ferrite beads on the USB power lines.

### Common Mode Choke

Two coils wound on the same core, in opposite directions:

```
              ┌──[L]──┐
Line A ───────┤       ├───────
              │ Core  │
Line B ───────┤       ├───────
              └──[L]──┘
```

- Differential current (signal): flows opposite in both windings → B-fields cancel → very low impedance (passes)
- Common mode current (noise): flows same direction in both → B-fields add → very high impedance (blocked)

Used on USB cables, HDMI cables, power lines — blocks EMI without affecting the signal.

### RF Choke (RFC)

An inductor specifically designed to block RF signals while passing DC. High inductance (μH to mH), high Q, high SRF.

```
Example use:
  DC power → RFC → transistor collector
  RF signal appears at collector but RFC blocks it from going back to power supply
```

### Coupled Inductor / Transformer

Multiple windings on same core. Used in:
- SEPIC converters (coupled inductors for efficiency)
- Flyback converters (transformer + energy storage)
- Forward converters
- LLC resonant converters

---

## 14. Reading Inductor Markings

### 3-Digit Code (like resistors)

```
Format: XXY
XX = first two significant digits
Y  = multiplier (number of zeros to add)
Unit: microhenries (μH)

101 = 10 × 10¹  = 100 μH
220 = 22 × 10⁰  = 22 μH
4R7 = 4.7 μH    (R = decimal point)
R47 = 0.47 μH
100 = 10 × 10⁰  = 10 μH
```

### 4-Digit Code

```
Format: XXYY
XX  = first two significant digits  
YY  = multiplier

1000 = 10 × 10⁰⁰ = 10 μH? No — depends on manufacturer.
Actually: 100μH is often marked as 101, never 1000 in this context.

Some manufacturers use:
1R0 = 1.0 μH
10R = 10 μH (R between digits = decimal point OR trailing zeros depending on context)
```

### Color Code (rare on modern parts)

Same as resistors — 4 or 5 bands — but value is in microhenries.

### SMD Marking

Tiny SMD inductors often just have:
- A dot or line (orientation mark, no value)
- A single letter/number code keyed to a reel label
- Full 3-digit code on larger parts

**Always check the reel label or datasheet — SMD markings are often not self-explanatory.**

---

## 15. Inductors in Real Circuits

### Low-Pass Filter (RL and LC)

**RL Low-Pass:**
```
Input ——[L]——┬—— Output
              |
             [R]
              |
             GND

Passes low frequencies (where X_L << R)
Blocks high frequencies (where X_L >> R)

Cutoff frequency: f_c = R / (2πL)
```

**LC Low-Pass (better, no power dissipation in ideal case):**
```
Input ——[L]——┬—— Output
              |
             [C]
              |
             GND

f_c = 1 / (2π√(LC))
```

Used in: power supply output filters, audio crossovers, EMI filters, switching supply output.

### High-Pass Filter (LC)

```
Input ——[C]——┬—— Output
              |
             [L]
              |
             GND

Passes high frequencies, blocks low.
Used in: high-frequency amplifier input, RF signal extraction.
```

### Band-Pass Filter

```
Series RLC:

Input ——[R]——[L]——[C]—— Output
                         |
                        GND

Maximum voltage at output when f = f₀ = 1/(2π√LC)
Width of peak determined by Q = f₀L/R
```

Used in: radio receivers (select one station), IF filters.

### Switching Power Supply Output Filter

Every buck converter output has an LC filter:
```
From switch ——[L]——┬—— Output (DC)
                    |
                   [C]
                    |
                   GND

L stores energy during ON time, releases during OFF time.
C smooths the ripple.
Together they convert the pulsing switch output to smooth DC.
```

### Relay Driver with Flyback Diode

```
ESP32 GPIO ——[1kΩ]——→ Base (NPN)
                        |
                   Collector ——→ Relay coil (inductor!) → VCC
                        |
                      Emitter → GND

Without flyback diode:
  When transistor turns off, relay coil (inductor) generates spike
  that can destroy the transistor.

WITH flyback diode:
  VCC ——┬—— Relay coil ——┬—— Collector
        |                |
       [D] (flyback)     |
        |                |
        └────────────────┘
        (diode clamps spike to VCC + 0.7V)
```

### Motor Driver with Inductor

Motors ARE inductors. A DC motor winding is typically 1-10mH.
Same flyback protection applies to every motor driver.
H-bridges (L298, DRV8833) have built-in flyback diodes for this reason.

---

## 16. Flyback Voltage — The Dangerous Spike

This is so important it deserves its own section.

### What Happens Without Protection

```
Before switch opens:
  Inductor carries current I
  Magnetic field stores energy E = ½LI²

Switch opens:
  Current path broken
  Inductor MUST continue current — it's a law of physics
  No path available → inductor generates whatever voltage needed
  V = L × dI/dt
  If dI/dt is fast (switch opens fast) → V is enormous

Example:
  L = 10mH (small relay coil)
  I = 50mA
  Switch opens in 1μs

  dI/dt = 50mA / 1μs = 50,000 A/s
  V = 0.01 × 50,000 = 500V!!

  Your transistor (rated 30V) sees 500V → immediately destroyed.
```

### The Flyback Diode Solution

```
           VCC
            |
         [Relay]  ← inductor
            |      \
           [D1]←——— + spike
            |      (diode clamps spike, conducts it back)
          [Q1] NPN
            |
           GND

When Q1 turns off:
  Spike appears at collector (+500V relative to GND)
  But D1's cathode is at VCC, so if spike > VCC+0.7V, D1 conducts
  Spike is clamped to VCC + 0.7V ← now Q1 only sees VCC+0.7V (safe!)
  Energy recirculates in relay coil loop until dissipated in coil resistance
```

The flyback diode must be:
- Fast recovery (1N4148 for small loads, 1N4007 for larger)
- Voltage rating > VCC
- Current rating > peak current

**Always use a flyback diode with any inductive load: relays, solenoids, motors, contactors.**

### Alternative Clamping Methods

```
TVS diode: clamps faster, lower clamping voltage
Snubber (RC): slows the spike rise, spreads energy over time
Zener + diode series: clamp to a specific voltage higher than VCC
                       (faster turn-off than simple flyback diode)
```

---

## 17. Buck and Boost Converters

The inductor is the heart of every switching power supply. This is how your 12V LiPo battery gets converted to 5V or 3.3V for your ESP32.

### Buck Converter (Step-Down)

Converts higher voltage → lower voltage.

```
                  D1
         Q1       ↑
VIN ——→|SWITCH|——┤—→——[L]——┬—→ VOUT
                 ↓         |
                GND        C  (output cap)
                           |
                          GND

Q1 = high-side switch (MOSFET)
D1 = freewheeling diode (or synchronous MOSFET)
L  = energy storage inductor
C  = output filter capacitor
```

**Operation:**

**Switch ON:**
```
VIN ——[L]——→ VOUT

Current through L rises: dI/dt = (VIN - VOUT) / L
Inductor stores energy
Capacitor supplies load
```

**Switch OFF:**
```
Energy in L must continue → current freewheels through D1
               D1
              ↗
GND ——→———[L]——→ VOUT

Current through L falls: dI/dt = −VOUT / L
Inductor releases energy to output
```

**Duty cycle relationship:**
```
VOUT = VIN × D

Where D = ton / (ton + toff) = duty cycle (0 to 1)

Example: VIN = 12V, VOUT = 5V
D = 5/12 = 0.417 → switch ON for 41.7% of each cycle

Inductor current ripple:
  ΔI = (VIN - VOUT) × D / (f × L)
  
  Where f = switching frequency
```

### Boost Converter (Step-Up)

Converts lower voltage → higher voltage.

```
         L        Q1
VIN ——[inductor]——┤SWITCH|—→ GND
          |        ↑
          └———→[D]—→ VOUT
                      |
                      C (output cap)
                      |
                     GND
```

**Switch ON:**
```
VIN charges L (current rises, builds up stored energy)
D1 is reverse biased → cap supplies load
dI/dt = VIN / L
```

**Switch OFF:**
```
L must continue current → voltage across L reverses and adds to VIN
Combined voltage (VIN + V_L) > VOUT → D1 conducts → charges output cap
```

**Duty cycle:**
```
VOUT = VIN / (1 − D)

Example: VIN = 3.7V (LiPo), VOUT = 5V (USB output)
1 − D = 3.7/5 = 0.74
D = 0.26 → switch ON for 26% of each cycle
```

**This is why LiPo power banks work: a boost converter takes the 3.7V cell up to 5V USB.**

### Why the Inductor Is Critical

In a switching supply, the inductor determines:
```
1. Ripple current (ΔI = (VIN−VOUT)×D / f×L)
   → Larger L = less ripple = smaller output capacitor needed

2. Efficiency (DCR losses = I² × DCR)
   → Lower DCR = more efficient

3. Peak current (must not saturate)
   → Peak I = average I + ΔI/2 < I_sat

4. Transient response (how fast output recovers from load steps)
   → Smaller L = faster response but more ripple (tradeoff)
```

---

## 18. Home Experiments

---

### Experiment 1: See the Magnetic Field with Iron Filings

**Purpose:** Make the invisible magnetic field visible.

**Materials:**
- Iron filings (scrape a steel nail with a file, or buy from a craft shop)
- White paper
- Any magnet (bar magnet shows field nicely)
- Or: make an electromagnet (20 turns of wire on a bolt, 9V battery)

**Procedure:**
1. Lay the paper over the magnet.
2. Sprinkle iron filings lightly over the paper.
3. Tap gently. The filings align with field lines.

**With electromagnet:**
1. Wrap ~20 turns of any copper wire around a large iron bolt.
2. Connect to 9V battery (briefly — it will get warm).
3. Sprinkle iron filings while current flows.
4. Disconnect battery — observe how filings collapse.

**What you see:** The classic dipole field pattern: lines leaving North pole, curving around, entering South pole. Inside the coil, field lines are parallel and dense (strong uniform field).

---

### Experiment 2: Build an Inductor and Measure It

**Purpose:** Wind a coil, measure its inductance, verify the formula.

**Materials:**
- Thin insulated wire (0.3mm or 0.5mm, "magnet wire" or stripped from an old transformer)
- Ferrite rod (from an old AM radio — the brown rod, ~10cm long, ~1cm diameter)
- Or: empty ballpoint pen tube (air core)
- Multimeter with inductance mode (if available) OR smartphone with inductor calculator app + oscilloscope/function generator

**Without inductance meter — estimate from the formula:**

Wind 50 turns on a ferrite rod (diameter 10mm, length 10cm):
```
μᵣ for typical radio ferrite ≈ 100–800 (say 200)
A = π × (0.005)² = 7.85 × 10⁻⁵ m²
l = 0.10m (winding length, spread evenly)
N = 50 turns

L = μ₀ × μᵣ × N² × A / l
L = (4π×10⁻⁷) × 200 × 2500 × 7.85×10⁻⁵ / 0.1
L ≈ 493 μH ≈ 500 μH
```

**Test it as an AM radio antenna:**
Connect a 100pF variable capacitor (or fixed 100pF capacitor) across the coil. You've made an LC tank circuit. If you connect a germanium diode and earphone, this is the antenna and tuner for a crystal radio!

Resonate at 1MHz: f = 1/(2π√LC) = 1/(2π√(500×10⁻⁶ × 100×10⁻¹²))
= 1/(2π × 2.24×10⁻⁷) = 711 kHz. Close to the middle of AM band!

---

### Experiment 3: Observe the Flyback Spike with an LED

**Purpose:** See the inductor voltage spike with your own eyes.

**Materials:**
- 9V battery
- Relay (any small 5V or 9V relay) OR motor OR solenoid
- NPN transistor (BC547 or 2N2222)
- 1kΩ resistor
- LED (any color)
- Switch or pushbutton
- Breadboard

**Circuit A — Without flyback diode (careful with transistor):**
```
9V ——→ Relay coil one end
Relay coil other end ——→ Collector (BC547)
9V ——→ 1kΩ ——→ Base (BC547)
Emitter ——→ GND

LED: connect from GND to collector (reverse biased normally)
     i.e., LED anode to GND, cathode to collector
```

When you break the base connection (disconnect the 1kΩ from 9V):
- Transistor turns OFF
- Relay coil generates spike
- Spike exceeds LED's reverse breakdown? **The LED might flash briefly!**
- More likely: the spike will kill the transistor within a few cycles. You'll see the transistor stop working.

**Circuit B — With flyback diode (safer):**
Add a 1N4007 diode across the relay coil: cathode to the + end, anode to collector side. Now the spike is clamped. Transistor survives.

**The LED trick for visualizing the spike safely:**
```
Replace the relay with an inductor (10mH coil).
Connect LED backward (cathode to VCC, anode to switching node).
Use a MOSFET as switch instead of BJT.

When MOSFET turns off, the negative flyback appears on anode.
LED (now forward biased by the spike) briefly flashes!
This is how optocouplers and flyback LEDs work in isolated PSUs.
```

---

### Experiment 4: Build an LC Resonant Circuit

**Purpose:** Observe resonance — the frequency where LC impedance is maximum.

**Materials:**
- Ferrite rod from AM radio (or wound coil from Experiment 2)
- Variable capacitor (from old AM radio, or 10pF–200pF fixed caps)
- Multimeter (AC voltage mode)
- Laptop/phone playing AM radio frequency (use a tone generator app)
- OR: Use a breadboard buzzer/oscillator as the AC source

**Simple resonance demo:**

1. Connect your wound coil (~500μH) in parallel with a 100pF capacitor.
2. Feed an AC signal through a 10kΩ resistor to the LC tank.
3. The tank circuit's voltage will peak at the resonant frequency.

Without a signal generator:
Use an AM radio station itself. The ferrite rod antenna (if from an AM radio) is already tuned with a variable cap to resonate at the station frequency. Listen to the actual radio — you're observing real LC resonance!

**Turn an old AM radio into a resonance demo:**
Open an old pocket AM radio (cheap, from e-waste). You'll see:
- Ferrite rod with coil (the inductor)
- Variable capacitor with two air-gap sections (the capacitor)
- As you turn the dial: you're changing C → changing f₀ → tuning to different stations

This is the oldest and most elegant application of LC resonance.

---

### Experiment 5: Measure RL Time Constant with Arduino/ESP32

**Purpose:** Record the exponential current rise in an RL circuit and extract L.

**Materials:**
- ESP32 or Arduino
- 10mH inductor (or your wound coil)
- 100Ω resistor
- Jumper wires

**Circuit:**
```
ESP32 GPIO (output) ——→ [100Ω] ——→ [L: 10mH] ——→ GND
                                          |
                                   ADC input of ESP32
                                   (measure voltage across L? 
                                   Actually easier: measure across R)

Better: measure voltage across R (which ∝ current):

GPIO → [L] → [R=100Ω] → GND
                |
              ADC (measures V_R = I×R, proportional to I)
```

**Arduino sketch:**
```cpp
const int OUTPUT_PIN = 2;
const int ADC_PIN = 34;

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {
  // Set GPIO low (discharge)
  digitalWrite(OUTPUT_PIN, LOW);
  delay(10);  // wait for full decay
  
  // Switch on and record rise
  digitalWrite(OUTPUT_PIN, HIGH);
  
  for (int i = 0; i < 200; i++) {
    int v = analogRead(ADC_PIN);
    Serial.println(v);
    delayMicroseconds(10);  // 10μs per sample
  }
  
  delay(1000);
}
```

**Analysis:**
Plot the ADC values. You should see an exponential rise. The time constant τ = L/R.

From the plot: find the time when V_R reaches 63.2% of final value. That time = τ.

```
L = τ × R

If τ ≈ 100μs, R = 100Ω:
L = 100×10⁻⁶ × 100 = 10mH ✓
```

This is how many digital LCR meters work — they apply a known voltage step and measure the time constant.

---

### Experiment 6: Make a Simple Boost Converter by Hand

**Purpose:** Actually build a switching power supply and step up voltage.

**Materials:**
- 1.5V AA battery (or two in series = 3V)
- Inductor ~100μH (from any dead circuit board or wound yourself)
- 1N4148 or 1N4007 diode
- 100μF capacitor
- NPN transistor (2N2222 or BC337)
- 10kΩ resistor
- 470Ω resistor
- Multimeter

**Manual boost (switch it yourself):**
```
1.5V+ ——→ [L: 100μH] ——→ ┬——→ [D] ——→ [C: 100μF] ——→ load ——→ GND
                          |                |
                         switch           (measures 2-4V!)
                          |
                         GND
```

Flip the switch ON and OFF rapidly (2-5 times per second). When ON: L charges. When OFF: L releases spike through D into C. C accumulates voltage. You should measure 2-4V across C from a 1.5V source!

**Automatic boost using transistor as oscillator (555-less):**
```
         1.5V
          |
         [L: 100μH]
          |
   ┌──────┤ Collector (2N2222)
   |      |
   | [470Ω] ← feedback resistor to base
   |      |
   └──→ Base
         |
       Emitter → GND

This is a simple blocking oscillator.
L resonates, transistor switches, generates pulses.
Add diode + cap at collector: output 5-10V from 1.5V!
```

Warning: output is unregulated and may be higher than expected. Use a high-voltage meter probe.

---

### Experiment 7: Motor as Inductor — Feel the Back-EMF

**Purpose:** Directly experience that a motor is an inductor.

**Materials:**
- Small DC motor (toy car motor, 3-6V)
- LED
- Multimeter

**Test 1 — Motor generates voltage when spun:**
Connect the LED directly across the motor terminals.
Spin the motor by hand rapidly.
**The LED lights up!** The spinning motor (generator) produces AC which the LED partially rectifies.

**Test 2 — Motor flyback:**
Connect motor to 9V battery through a switch.
Let it spin for 2 seconds.
Quickly disconnect one wire (open circuit the motor).
Look for/measure the voltage spike with multimeter on DC 200V range.

You should see a spike of 20-50V (even though you only supplied 9V) as the motor's inductance releases its stored energy.

**Test 3 — Inductance measurement:**
Motor winding inductance can be measured with the RL time constant method (Experiment 5). Typical small motors: 0.5mH to 10mH.

---

## 19. Quick Reference

### Key Formulas

```
Fundamental:         V = L × dI/dt
Energy:              E = ½ × L × I²
Inductance:          L = μ₀μᵣN²A/l
Reactance:           X_L = 2πfL = ωL
Impedance:           Z = √(R² + X_L²)
RL time constant:    τ = L/R
LC resonance:        f₀ = 1/(2π√LC)
Q factor:            Q = X_L/R = f₀/BW
Transformer voltage: V₂/V₁ = N₂/N₁
Transformer current: I₂/I₁ = N₁/N₂
Buck converter:      VOUT = VIN × D
Boost converter:     VOUT = VIN / (1-D)
```

### Inductor vs Capacitor — The Duality Table

| Property              | Capacitor                   | Inductor                  |
|-----------------------|-----------------------------|---------------------------|
| Stores energy as      | Electric field (voltage)    | Magnetic field (current)  |
| Fundamental equation  | I = C × dV/dt               | V = L × dI/dt             |
| Energy                | E = ½CV²                    | E = ½LI²                  |
| Blocks                | DC (at steady state, open)  | AC (high freq → high Z)   |
| Passes                | AC (especially high freq)   | DC (at steady state, short)|
| Cannot change instantly | Voltage                   | Current                   |
| Reactance             | X_C = 1/(2πfC) — decreases | X_L = 2πfL — increases    |
| Phase shift           | I leads V by 90°            | V leads I by 90°          |
| Danger                | Voltage spike on short      | Current spike on open      |

### Which Inductor to Use

```
Application              Type             Typical value
Power supply buck/boost  Shielded SMD     1μH – 100μH
EMI filter on power line Common mode choke 1mH – 100mH
RF signal filter         Air core / NiZn  1nH – 100nH
Audio crossover          Laminated iron   1mH – 10mH
Relay driver decoupling  Ferrite bead     100Ω @ 100MHz
AM radio antenna         Ferrite rod      200μH – 2mH
Flyback transformer      EE/EI ferrite    10μH – 10mH
```

### Debugging Inductors

| Symptom | Likely cause |
|---------|-------------|
| MOSFET dying in motor/relay circuit | Missing flyback diode |
| Switching supply output too high | Wrong L value (too small) |
| Switching supply runs hot | DCR too high, or saturation |
| RF noise from power supply | Unshielded inductor, wrong ferrite grade |
| Oscillator won't start | Q too low, inductor too lossy |
| LC filter not filtering | Operating above SRF of inductor |
| Inductor hums/squeals | Magnetostriction — core vibrating at switching frequency |

### The Hierarchy of Inductor Knowledge

```
Inductor (wire coil)
├── Creates magnetic field when current flows
├── Resists current changes (V = L dI/dt)
├── Stores energy in field (E = ½LI²)
├── Acts as short to DC, open to high-frequency AC
│
├── RL circuit → time constant τ = L/R
├── LC circuit → resonance f₀ = 1/2π√(LC)
├── Mutual inductance → Transformer
│
├── Practical types
│   ├── Air core (RF, no saturation)
│   ├── Ferrite (power, high permeability)
│   ├── Toroid (EMI-shielded)
│   └── Ferrite bead (EMI absorption)
│
└── Applications
    ├── Filters (LP, HP, BP)
    ├── Flyback protection
    ├── Switching converters (buck/boost)
    ├── RF tuning (with capacitor)
    └── Transformers (power, audio, RF isolation)
```

---

*Every time you plug in your ESP32 USB cable, switching converters (built around inductors) are running at hundreds of kHz, stepping voltages up and down, controlled by tiny MOSFETs. The inductor you just learned about is the energy backbone of every power supply on earth.*
