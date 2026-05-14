# Silicon & Semiconductors — Complete Theory + Home Experiments

## Table of Contents

1. [The Big Picture](#1-the-big-picture)
2. [Atoms — The Foundation](#2-atoms--the-foundation)
3. [What Makes a Semiconductor](#3-what-makes-a-semiconductor)
4. [Silicon — The Element](#4-silicon--the-element)
5. [Crystal Structure](#5-crystal-structure)
6. [Energy Bands (How Electrons Actually Move)](#6-energy-bands-how-electrons-actually-move)
7. [Intrinsic Silicon — Pure Semiconductor](#7-intrinsic-silicon--pure-semiconductor)
8. [Doping — Engineering the Semiconductor](#8-doping--engineering-the-semiconductor)
9. [The PN Junction — Where the Magic Happens](#9-the-pn-junction--where-the-magic-happens)
10. [Diodes](#10-diodes)
11. [Bipolar Junction Transistors (BJT)](#11-bipolar-junction-transistors-bjt)
12. [MOSFETs](#12-mosfets)
13. [How Chips Are Made](#13-how-chips-are-made)
14. [Other Semiconductors Beyond Silicon](#14-other-semiconductors-beyond-silicon)
15. [Home Experiments](#15-home-experiments)
16. [Quick Reference](#16-quick-reference)

---

## 1. The Big Picture

Every electronic device you own — your phone, your ESP32, your LED — is built on one fundamental fact:

> **Some materials can be engineered to conduct electricity exactly as much as we want, when we want.**

These materials are called **semiconductors**. Silicon is the most important one.

Understanding silicon explains:
- Why diodes only let current flow one way
- Why transistors can amplify or switch
- Why LEDs glow
- Why solar cells generate voltage from light
- How a billion transistors fit on a chip smaller than your fingernail

---

## 2. Atoms — The Foundation

### What an Atom Looks Like

```
        Nucleus
       (protons +
       neutrons)
          |
          v
         (o)   <-- center, very tiny, very dense
        /   \
      /       \
   e-           e-    <-- electrons orbiting in shells
      \       /
        \   /
         e-
```

An atom has:
- **Protons** (positive charge) — in the nucleus
- **Neutrons** (no charge) — in the nucleus
- **Electrons** (negative charge) — orbiting the nucleus in shells

The number of protons = **atomic number** = defines the element.

Silicon has atomic number **14** → 14 protons → 14 electrons.

### Electron Shells

Electrons don't orbit randomly. They occupy **shells** (also called energy levels):
- Shell 1: holds up to 2 electrons
- Shell 2: holds up to 8 electrons
- Shell 3: holds up to 18 electrons (but wants 8 to be "happy")

Silicon's 14 electrons fill up like this:
```
Shell 1: 2 electrons   (full — stable)
Shell 2: 8 electrons   (full — stable)
Shell 3: 4 electrons   (HALF FULL — this is the key!)
```

That outermost shell is called the **valence shell**. Its electrons are called **valence electrons**.

Silicon has **4 valence electrons** — this is why silicon is special.

### Why 4 Valence Electrons Matters

Atoms "want" 8 electrons in their outer shell (the **octet rule**). Silicon with 4 is exactly halfway:
- It's not a conductor like copper (1 valence electron, loosely held)
- It's not an insulator like rubber (8 valence electrons, tightly held)
- It's right in the middle — a **semiconductor**

---

## 3. What Makes a Semiconductor

### The Three Categories of Materials

```
CONDUCTORS          SEMICONDUCTORS        INSULATORS
(Copper, Gold,     (Silicon, Germanium,  (Rubber, Glass,
 Aluminium)         GaAs, InP)            Plastic)

  e-  e-  e-           e-    e-              X    X
 e- e- e- e-          e-  [  ]  e-           X    X
  e-  e-  e-            e-    e-              X    X

Electrons flow      Electrons flow          No electrons
freely at room      only with help          flow at all
temperature         (energy, doping)
```

| Property           | Conductor    | Semiconductor    | Insulator     |
|--------------------|--------------|------------------|---------------|
| Valence electrons  | 1            | 4                | 8             |
| Conductivity       | Very high    | Medium (tunable) | Very low      |
| Band gap           | 0 eV         | 0.1 – 3.5 eV    | > 5 eV        |
| Example            | Copper       | Silicon          | Diamond       |

The magic of semiconductors: **their conductivity can be controlled** with:
1. Temperature
2. Light
3. Adding tiny amounts of impurity atoms (doping)
4. Electric fields

This controllability is why we can build transistors and diodes.

---

## 4. Silicon — The Element

### Basic Facts

| Property              | Value                          |
|-----------------------|--------------------------------|
| Symbol                | Si                             |
| Atomic number         | 14                             |
| Atomic mass           | 28.09 u                        |
| Valence electrons     | 4                              |
| Melting point         | 1414°C                         |
| Band gap              | 1.12 eV (at room temperature)  |
| Crystal structure     | Diamond cubic                  |
| Abundance in Earth's crust | 27.7% (second most abundant element) |

### Where Silicon Comes From

Sand (SiO₂ — silicon dioxide) is almost pure silicon dioxide. Computer chips start from beach sand:

```
Sand (SiO₂)
    |
    | Heat to 1500°C with carbon (coke)
    v
Metallurgical grade Si (98% pure)
    |
    | Siemens process (chlorosilane gas + H₂)
    v
Electronic grade Si (99.9999999% pure — "9N purity")
    |
    | Czochralski process (pull a crystal from melt)
    v
Silicon ingot (single crystal, 30cm diameter, 1m long)
    |
    | Diamond wire saw
    v
Silicon wafers (0.7mm thick discs)
    |
    | Photolithography + doping + etching (100+ steps)
    v
Chips (ESP32, CPU, etc.)
```

### Why Silicon Dominates (and Not Germanium)

Germanium was used in the first transistors (1947). Silicon replaced it because:

| Factor                | Silicon (Si)  | Germanium (Ge)  |
|-----------------------|---------------|-----------------|
| Band gap              | 1.12 eV       | 0.67 eV         |
| Max operating temp    | ~150°C        | ~75°C           |
| Forward voltage (diode)| ~0.7V        | ~0.3V           |
| Native oxide          | SiO₂ (excellent insulator) | GeO₂ (dissolves in water) |
| Abundance             | Very common   | Rare            |
| Cost                  | Cheap         | Expensive       |

Silicon's natural oxide (SiO₂) is the key: it's an almost perfect insulator and bonds perfectly to silicon, making it ideal for building transistor gates.

---

## 5. Crystal Structure

### Why Crystal Structure Matters

In a single crystal, every atom is in a perfectly ordered lattice. This regularity allows electrons to move predictably — which is what we need for reliable electronics.

### Diamond Cubic Lattice

Silicon forms a **diamond cubic** crystal structure. Each silicon atom bonds to exactly 4 neighbors, sharing one valence electron with each:

```
        Si
       /|\ \
      / | \ \
    Si  Si  Si  Si
      \ | / /
       \|/ /
        Si
```

In 2D (easier to draw):

```
  Si — Si — Si — Si
  |    |    |    |
  Si — Si — Si — Si
  |    |    |    |
  Si — Si — Si — Si
  |    |    |    |
  Si — Si — Si — Si

Each line (—) represents a shared electron pair = covalent bond
```

This is called **covalent bonding** — atoms share electrons rather than giving them away.

### Covalent Bonds in Detail

Each bond contains 2 electrons (one from each Si atom):

```
    4 bonds, one in each direction
    
         e e
         | |
    e e--Si--e e
         | |
         e e

Actually more accurately:
Si has 4 valence electrons.
Each is shared with an adjacent Si.
Result: each Si "sees" 8 electrons = full outer shell = stable.
```

At absolute zero (−273°C), all electrons are locked in bonds — silicon is a perfect insulator. At room temperature, thermal energy breaks some bonds — creating free electrons.

---

## 6. Energy Bands (How Electrons Actually Move)

### From Atoms to Crystals

A lone atom has discrete energy levels (like steps on a staircase). When billions of atoms bond into a crystal, those discrete levels **smear out** into continuous **energy bands**:

```
ISOLATED ATOM               CRYSTAL (billions of atoms)

   Energy                      Energy
     |                           |
     | --- n=3                   | ==== CONDUCTION BAND (empty or partly filled)
     |                           |
     | --- n=2                   | //// BAND GAP (forbidden zone)
     |                           |
     | --- n=1                   | ==== VALENCE BAND (filled at 0K)
     |                           |
    atom                       crystal
```

### The Three Key Bands

**Valence Band** — The highest band that is fully filled at 0K. Electrons here are "owned" by atoms. They cannot move freely and carry no current.

**Band Gap** — A forbidden zone. No electron can exist here. This is the key differentiator between materials:
- Conductor: 0 eV gap (bands overlap)
- Semiconductor: ~1 eV gap (crossable with thermal/light energy)
- Insulator: >5 eV gap (almost never crossable)

**Conduction Band** — The band above the gap. Electrons here are "free" — not bound to any atom. They can move through the crystal and carry current.

### How Silicon's Band Gap Works

Silicon's band gap = **1.12 eV** (electron-volt).

At room temperature (300K), thermal energy ≈ 0.026 eV per particle.

Most electrons don't have enough energy to jump the gap. But in a large crystal with ~10²³ atoms, a small fraction do:
- Pure silicon at room temp: ~1.5 × 10¹⁰ free electrons per cm³
- Compare: copper has ~8.5 × 10²² free electrons per cm³ (about 10 billion times more)

This tiny fraction of free electrons is what makes silicon a weak conductor at room temperature — and what we can dramatically increase through doping.

### Light and the Band Gap

A photon with energy ≥ 1.12 eV can excite an electron from valence band to conduction band.

Energy of a photon: **E = hf = hc/λ**

For silicon's 1.12 eV band gap:
```
λ = hc/E = (6.63×10⁻³⁴ × 3×10⁸) / (1.12 × 1.6×10⁻¹⁹)
  = 1.11 μm  (near-infrared)
```

Photons with wavelength < 1.11 μm (visible light and UV) have enough energy to be absorbed by silicon. This is why:
- Silicon solar cells work with visible light
- Silicon appears gray/metallic (it absorbs most visible light)

For LED emission, you need a **direct band gap** semiconductor (GaAs, GaN, InP) — silicon has an **indirect band gap** which makes it very inefficient at emitting light.

---

## 7. Intrinsic Silicon — Pure Semiconductor

### Electron-Hole Pairs

When a covalent bond breaks (from thermal energy or light), two things are created:

```
Before:
  Si — Si — Si
       |
       (bond intact, both electrons in place)

After (bond broken by thermal energy):
  Si — Si — Si
       |
   [hole]  [free electron]
       |         |
   positive    negative
   charge      charge
   carrier     carrier
```

The **hole** is not a real particle — it's the absence of an electron in the valence band. But it behaves like a positive charge carrier. When an adjacent electron fills the hole, the hole appears to move in the opposite direction.

```
Electron movement:  → → → → →
Hole movement:      ← ← ← ← ←

Both carry current, in opposite directions, which add together!
```

### Thermal Generation and Recombination

Generation (breaking bonds): increases with temperature
Recombination (electron fills a hole, both disappear): happens constantly

At equilibrium, **generation rate = recombination rate**. This gives us the thermal carrier concentration n_i.

For silicon at 300K:
```
n_i = 1.5 × 10¹⁰ carriers/cm³

(electrons = holes = n_i for pure silicon)
```

### Temperature Dependence

As temperature increases, more bonds break, more carriers, higher conductivity:

```
Temperature  |  Conductivity of Si
100K (−173°C)|  Nearly insulating
200K (−73°C) |  Very low
300K (27°C)  |  1.5×10¹⁰ carriers/cm³  (baseline)
400K (127°C) |  ~10× higher (problem for chips!)
500K (227°C) |  Breaks down completely
```

This is why chips overheat: conductivity rises with temperature → more leakage current → more heat → more conductivity → runaway. Heatsinks and thermal management are critical.

### Intrinsic vs Extrinsic

**Intrinsic semiconductor** = pure silicon, carrier concentration determined by temperature only.

**Extrinsic semiconductor** = doped silicon, carrier concentration dominated by dopant atoms (1000x to 10⁸x more carriers than intrinsic). This is what we actually use in devices.

---

## 8. Doping — Engineering the Semiconductor

Doping = adding a tiny, controlled amount of a foreign atom (dopant) to silicon.

Typical doping concentration: 10¹⁵ to 10¹⁸ atoms/cm³
Silicon atom density: 5 × 10²² atoms/cm³

So roughly 1 dopant per 10,000 to 10,000,000 silicon atoms. Incredibly small amount, yet transforms electrical behavior completely.

### N-Type Doping (Adding Electrons)

Use atoms with **5 valence electrons**: Phosphorus (P), Arsenic (As), Antimony (Sb).

These are called **donor atoms** — they donate a free electron.

```
SILICON LATTICE WITH PHOSPHORUS DOPANT:

  Si — Si — Si — Si
  |    |    |    |
  Si — Si — P  — Si      ← Phosphorus has 5 valence electrons
  |    |    |    |         4 form bonds with Si neighbors
  Si — Si — Si — Si        1 is LEFT OVER → free electron!
                            
                         P⁺  e⁻
                         (ionized donor, free electron)
```

The phosphorus atom becomes a fixed positive ion (P⁺). The extra electron is free to roam the crystal and carry current.

**Result:** Many free electrons, very few holes.
- Electrons are the **majority carriers**
- Holes are the **minority carriers**
- Material is called **N-type** (N for Negative charge carrier)

### P-Type Doping (Adding Holes)

Use atoms with **3 valence electrons**: Boron (B), Gallium (Ga), Indium (In), Aluminium (Al).

These are called **acceptor atoms** — they accept an electron, creating a hole.

```
SILICON LATTICE WITH BORON DOPANT:

  Si — Si — Si — Si
  |    |    |    |
  Si — Si — B  — Si      ← Boron has only 3 valence electrons
  |    |    |    |         3 form bonds with Si neighbors
  Si — Si — Si — Si        1 bond is INCOMPLETE → hole!
                            
                         B⁻  [hole]
                         (ionized acceptor, mobile hole)
```

The boron atom becomes a fixed negative ion (B⁻). The hole is free to move as adjacent electrons fill it.

**Result:** Many holes, very few electrons.
- Holes are the **majority carriers**
- Electrons are the **minority carriers**
- Material is called **P-type** (P for Positive charge carrier)

### Doping Summary

```
                N-TYPE              P-TYPE
Dopant:         Phosphorus (5e⁻)   Boron (3e⁻)
Majority carrier: Electrons         Holes
Minority carrier: Holes             Electrons
Fixed charges:  Positive ions (P⁺) Negative ions (B⁻)
Net charge:     NEUTRAL            NEUTRAL
Fermi level:    Near conduction band Near valence band
```

Important: **Both N-type and P-type silicon are electrically neutral overall.** The fixed ions and the mobile carriers cancel out. Current flows when an external voltage pushes the mobile carriers.

---

## 9. The PN Junction — Where the Magic Happens

Take a piece of N-type silicon and join it to P-type silicon. What happens at the boundary?

### Step 1: Diffusion

Electrons in N-type diffuse toward P-type (high concentration → low concentration). Holes in P-type diffuse toward N-type.

```
P-type     |    N-type
           |
[hole]→  → | → ←  ←[electron]
[hole]→  → | → ←  ←[electron]
           |
    Diffusion current
```

### Step 2: Depletion Region Forms

As electrons diffuse into P-type, they recombine with holes. As holes diffuse into N-type, they recombine with electrons. This leaves behind the fixed charged ions:

```
P-type        | Depletion | N-type
              | Region    |
  B⁻ B⁻ B⁻  | B⁻  P⁺  | P⁺ P⁺ P⁺
  B⁻ B⁻ B⁻  | B⁻  P⁺  | P⁺ P⁺ P⁺
  B⁻ B⁻ B⁻  | B⁻  P⁺  | P⁺ P⁺ P⁺
             |           |
  (negative) ←  E-field  → (positive)
```

The negative ions on P-side and positive ions on N-side create an **electric field** pointing from N to P.

### Step 3: Built-In Potential (Contact Potential)

The electric field opposes further diffusion. Equilibrium is reached when the drift current (from E-field) balances the diffusion current.

This creates a **built-in potential** across the junction:
```
Silicon PN junction built-in potential ≈ 0.6 – 0.7 V

(This is why silicon diodes have a 0.7V forward voltage drop!)
```

### Step 4: Forward Bias

Apply external voltage + on P-side, − on N-side:

```
   + Battery −
   |         |
   P  [jct]  N
   |         |
   |← V_ext →|
   
External field opposes built-in field.
If V_ext > 0.6V: net field collapses, carriers flow across junction.
Result: CURRENT FLOWS.
```

The depletion region narrows and eventually disappears. Current increases exponentially with voltage (Shockley diode equation).

### Step 5: Reverse Bias

Apply external voltage − on P-side, + on N-side:

```
   − Battery +
   |         |
   P  [jct]  N
   |         |
   External field ADDS to built-in field.
   Depletion region WIDENS.
   Almost no current flows.
   Result: BLOCKED.
```

Only tiny **reverse saturation current** (I₀) flows from thermally generated minority carriers — picoamps to nanoamps.

### The Shockley Diode Equation

```
I = I₀ × (e^(V/nVₜ) − 1)

Where:
  I  = diode current
  I₀ = reverse saturation current (10⁻¹² A for silicon)
  V  = voltage across diode
  n  = ideality factor (1-2, usually 1)
  Vₜ = thermal voltage = kT/q = 0.026V at 300K
```

At V = 0.65V:
```
I = 10⁻¹² × e^(0.65/0.026) = 10⁻¹² × e^25 = 10⁻¹² × 7.2×10¹⁰ = 0.072A = 72mA
```

Huge current for a small voltage change — the exponential I-V characteristic.

---

## 10. Diodes

### Structure

```
         Anode (+)    Cathode (−)
              |            |
    Metal contact          Metal contact
         |                      |
    P-type Si  |  junction  |  N-type Si
```

Diode symbol:
```
    Anode         Cathode
      |     ___      |
      |----▷|-------|
      |               |
   Current flows this direction →
```

### I-V Characteristic Curve

```
Current (mA)
    ^
 100|                          /
  80|                         /
  60|                        /
  40|                       /
  20|                      /
   0|_____________________/____________→ Voltage (V)
  -1|                  0.3 0.6 0.7
    |
    | (tiny leakage current in reverse)
    |
    | At reverse breakdown (−30 to −1000V):
    | current flows again (avalanche/Zener)
```

### Types of Diodes

**Rectifier diode (1N4007):** converts AC to DC. Forward drop ≈ 0.7V.

**Schottky diode (1N5817):** metal-semiconductor junction. Forward drop ≈ 0.3V. Very fast switching.

**Zener diode:** designed to operate in reverse breakdown at a precise voltage. Used for voltage regulation.

**LED (Light Emitting Diode):** made from direct-bandgap semiconductors (GaN, GaAs). When electrons recombine with holes, energy is released as photons.

```
LED color   Semiconductor   Forward voltage
Red         GaAsP           1.8 – 2.2 V
Yellow      GaAsP           2.0 – 2.1 V
Green       GaN             2.0 – 3.0 V
Blue        InGaN           2.5 – 3.5 V
White       InGaN + phosphor 3.0 – 3.5 V
IR          GaAs            1.2 – 1.7 V
UV          AlGaN           3.1 – 4.4 V
```

**Photodiode:** light generates electron-hole pairs → current. Used in light sensors, solar cells.

**Varactor diode:** reverse-biased diode whose capacitance changes with voltage. Used in tunable circuits.

---

## 11. Bipolar Junction Transistors (BJT)

### Structure

A BJT is two PN junctions sharing a thin middle region:

**NPN transistor:**
```
N emitter | P base | N collector
    E      |   B   |     C

  Metal    P thin   Metal
  contact  region   contact
    |     (1μm!)     |
    |        |        |
    N    ----P----    N
    |        |        |
    E        B        C
```

**Symbol:**
```
        C
        |
    B --[NPN arrow pointing out
        |
        E

Arrow direction: direction conventional current flows into emitter
```

### How the NPN Transistor Works

**Step 1:** Forward bias the Base-Emitter junction (B more positive than E by ~0.7V).

Electrons flow from Emitter into the base.

**Step 2:** The base is VERY THIN (1μm). Most electrons that enter don't recombine with holes — they diffuse straight through into the collector region.

**Step 3:** The Collector-Base junction is reverse biased (C more positive than B). The electric field sweeps the electrons that reach the collector into the collector terminal.

**Result:**
```
Large collector current (Ic) controlled by small base current (Ib)

Current gain: β (beta) = hFE = Ic / Ib

Typical β: 50 – 300

If Ib = 1mA, β = 100: Ic = 100mA

Small current controls big current = AMPLIFICATION
```

### Three Regions of Operation

```
                 Vc (collector voltage)
                 
Saturation   |  Active (linear)    |
(both jct    |  (amplifier mode)   |  Cutoff
forward      |  Ic = β × Ib       |  (both junctions
biased)      |                    |  reverse biased,
             |                    |  no current)
             |                    |
             0                 voltage
```

**Cutoff:** No base current → No collector current → Switch is OFF
**Saturation:** Base current large → Collector current maxed → Switch is ON
**Active:** Base current controls collector current linearly → Amplifier

### BJT as a Switch (Your ESP32 GPIO Example)

ESP32 GPIO can only source/sink ~12mA. To drive a motor or relay needing 500mA:

```
ESP32 GPIO (3.3V) →[1kΩ resistor]→ Base of NPN (2N2222)
                                     |
                              Collector → Motor/Relay → 12V
                                     |
                                   Emitter → GND
```

When GPIO goes HIGH: Ib = (3.3 - 0.7) / 1000 = 2.6mA
With β = 100: Ic can reach 260mA → motor gets full current.

---

## 12. MOSFETs

The transistor inside your ESP32 (and every modern chip) is a MOSFET, not a BJT.

### Structure (N-channel MOSFET)

```
                    Gate (G)
                       |
Source (S) ←------[insulator (SiO₂)]-------→ Drain (D)
     |         |                    |          |
     N+     P-substrate          N+
     |                                        |
     |←————————— channel ————————————→|
                (induced when V_GS > Vth)
```

### How It Works

The gate is separated from the semiconductor by an insulator (SiO₂ — about 5nm thick in modern chips).

**No gate voltage:** The P-type substrate between source and drain has no free electrons. No current flows.

**Gate voltage > threshold (Vth):** Electric field from gate attracts electrons from P-type substrate, forming an **inversion layer** (channel) of electrons beneath the gate. Now electrons can flow from source to drain!

```
V_GS < Vth:  OFF (no channel)
V_GS > Vth:  ON (channel forms, current flows)
```

### Why MOSFETs Beat BJTs for Chips

| Property          | BJT           | MOSFET               |
|-------------------|---------------|----------------------|
| Control signal    | Base current  | Gate voltage (no current!) |
| Power to switch   | Continuous    | Nearly zero (just charge gate capacitor) |
| Size              | Larger        | Scales to sub-nm     |
| Input impedance   | Low           | Extremely high       |
| Speed             | Good          | Better at small sizes |

No gate current → CMOS logic (complementary N+P MOSFET pairs) dissipates power only when switching, not when idle. This is why CPUs can have billions of transistors without melting.

### CMOS Inverter (The Heart of All Digital Logic)

```
         VDD (3.3V)
            |
           [P-MOSFET] ← (gate connected to Input)
            |
            +——→ Output
            |
           [N-MOSFET] ← (gate connected to Input)
            |
           GND

Input HIGH: N-MOSFET ON, P-MOSFET OFF → Output pulled to GND
Input LOW:  N-MOSFET OFF, P-MOSFET ON → Output pulled to VDD

Every NAND, NOR, XOR, flip-flop is built from this primitive.
```

---

## 13. How Chips Are Made

### The Whole Process (Simplified)

**Starting material:** Silicon wafer (300mm diameter, 0.7mm thick, single crystal, ultra-pure)

**Step 1 — Oxidation**
```
Wafer in furnace with O₂/H₂O at 1000°C
Si + O₂ → SiO₂

Grows a precise thickness of SiO₂ (2nm – 1μm depending on use)
```

**Step 2 — Photolithography (Printing the pattern)**
```
1. Spin photoresist (light-sensitive polymer) on wafer
2. Expose through a mask (like a photographic negative) with UV light
3. Develop: exposed resist washes away (positive resist)
4. Etch: acid or plasma removes exposed SiO₂
5. Strip resist
Result: SiO₂ pattern matches the mask
```

**Step 3 — Doping**
```
Ion implantation: shoot P or B ions at wafer using particle accelerator
Ions embed at precise depth and concentration
Anneal at 1000°C to repair crystal damage and activate dopants
```

**Step 4 — Deposition**
```
Chemical Vapor Deposition (CVD): deposit polysilicon, metals, insulators
Atomic Layer Deposition (ALD): one atomic layer at a time (for gate dielectric)
Physical Vapor Deposition (PVD): sputter metal from target
```

**Step 5 — Chemical Mechanical Polishing (CMP)**
```
Polish wafer surface flat (to within 1nm!) so next layer can be added on top
Modern chips have 15+ metal interconnect layers stacked on top of transistors
```

**Repeat steps 1-5 many times (~1000 steps total)**

**Step 6 — Test, Dice, Package**
```
Test each die on wafer (probe card touches thousands of contact pads)
Saw wafer into individual chips (diamond blade or laser)
Bond to package (wire bonds or flip-chip bumps)
Encapsulate in plastic or ceramic
Test finished package
```

### Modern Scale Numbers (2024)

| Parameter              | Value                    |
|------------------------|--------------------------|
| Smallest transistor    | 2nm (20 atoms wide)      |
| Transistors per chip   | ~100 billion (Apple M3 Ultra) |
| Wafer diameter         | 300mm (12 inches)        |
| Wafer cost             | $5,000 – $20,000         |
| Dies per wafer         | 200 – 1000+              |
| Fab cost               | $10 – $20 billion USD    |
| Defect density         | <0.01 defects/cm²        |

### Your ESP32 at Scale

The ESP32 uses a 40nm process (older, cheaper, sufficient for RF).
- Transistor size: ~40nm
- A human hair: ~70,000nm wide
- 1750 transistors fit across a human hair on the ESP32

---

## 14. Other Semiconductors Beyond Silicon

### Why Other Materials

Silicon is great but not perfect for everything:

| Material | Band Gap | Key Property | Application |
|----------|----------|--------------|-------------|
| Si       | 1.12 eV  | Cheap, stable, great oxide | CPUs, memory, power |
| Ge       | 0.67 eV  | Low forward voltage | Old transistors, some RF |
| GaAs     | 1.43 eV (direct) | Fast electrons, emits light | RF chips (your phone PA), IR LEDs |
| GaN      | 3.4 eV (direct)  | High voltage, emits blue/UV | LEDs, 5G power amplifiers, fast chargers |
| SiC      | 3.26 eV  | High temp, high voltage | EV power electronics |
| InP      | 1.35 eV (direct) | Ultra-fast electrons | Fiber optic transceivers |
| CdTe     | 1.45 eV  | Cheap thin-film solar | Solar panels |
| GaN on Si | — | Combines GaN+Si | Next-gen power ICs |

### Direct vs Indirect Band Gap

This determines if a semiconductor can emit light efficiently:

**Direct band gap (GaAs, GaN, InGaAs):**
- Electron falls from conduction band to valence band in one step
- Momentum is conserved → photon emitted directly
- Efficient light emitter → LEDs, lasers, solar cells

**Indirect band gap (Si, Ge):**
- Electron must change both energy AND momentum to recombine
- Requires a phonon (crystal vibration) to be involved
- Very inefficient light emitter → silicon LEDs barely work
- But fine for absorbing light (solar cells use silicon fine)

---

## 15. Home Experiments

All experiments use components available cheaply online (< ₹500 total).

---

### Experiment 1: Measure the Diode's "Fingerprint" (I-V Curve)

**Purpose:** Directly observe the PN junction's exponential I-V curve.

**Components:**
- 1N4007 rectifier diode
- 100Ω resistor
- Multimeter (voltage and current)
- 9V battery + connector

**Circuit:**
```
    (+)9V
     |
    [100Ω]
     |
    [A]←— ammeter —→[K] of 1N4007 diode (Anode-to-Cathode)
     |
    GND
```

Or use two multimeters — one in series (current) one in parallel with diode (voltage).

**Simpler method with one multimeter:**

Vary the resistor in series (use different resistors: 1kΩ, 470Ω, 220Ω, 100Ω, 47Ω):

| Resistor | Circuit current (I = V_bat/R total) | Diode voltage |
|----------|--------------------------------------|---------------|
| 1000Ω    | ≈ 8.3mA (subtract 0.7V drop)        | Measure it    |
| 470Ω     | ≈ 17.6mA                             | Measure it    |
| 100Ω     | ≈ 83mA                               | Measure it    |

You'll see: current varies hugely, but diode voltage stays near 0.6-0.7V. The exponential relationship holds.

**Reverse bias test:**
Flip the diode. Measure voltage across it with your meter set to 20V DC. You'll read close to the full battery voltage — almost no current flows, junction is blocking.

---

### Experiment 2: Compare Silicon vs Germanium Forward Voltage

**Components:**
- 1N4007 silicon diode (Vf ≈ 0.7V)
- 1N34A germanium diode (Vf ≈ 0.3V) — order online ₹20-30 each
- 1kΩ resistor
- 9V battery
- Multimeter

**Procedure:** Build the same series circuit with each diode. Measure the voltage across each diode.

**Expected results:**
```
Silicon (1N4007):  Vf = 0.65 – 0.72V
Germanium (1N34A): Vf = 0.25 – 0.35V
```

This directly shows the effect of band gap difference on junction potential.

**Bonus:** Germanium diodes are sensitive enough to detect AM radio signals. Silicon needs more signal. The crystal radio (stage_02 in your radio_stages folder) works best with germanium.

---

### Experiment 3: Temperature Changes Semiconductor Behavior

**Purpose:** See how thermal energy affects carrier generation.

**Components:**
- 1N4007 diode
- 1kΩ resistor
- 9V battery
- Multimeter (voltage mode, across diode)
- Hot water (or a heat gun / hair dryer)
- Ice water

**Procedure:**
1. Build: 9V → 1kΩ → diode → GND. Measure Vf across diode at room temperature.
2. Heat the diode with hot water (don't burn yourself — wrap in plastic, dip briefly) or hairdryer.
3. Measure Vf again.
4. Cool the diode in ice water.
5. Measure Vf again.

**Expected results:**
```
Hot diode (60°C):   Vf ≈ 0.55V  (lower — more thermal carriers)
Room temp (25°C):   Vf ≈ 0.67V
Cold diode (5°C):   Vf ≈ 0.73V  (higher — fewer thermal carriers)
```

Temperature coefficient of silicon junction: **−2 mV/°C**

This is why electronics datasheets always specify temperatures. It's also how temperature sensors like LM35 work — they output a voltage proportional to temperature using this exact effect.

---

### Experiment 4: The Transistor as a Switch

**Purpose:** Prove that a small base current controls a large collector current.

**Components:**
- BC547 NPN transistor (or 2N2222) — ₹5 each
- 5mm LED (any color)
- 1kΩ resistor (base)
- 330Ω resistor (LED current limit)
- 9V battery
- Jumper wires
- Breadboard

**Circuit:**
```
         9V
          |
        [330Ω]
          |
         LED
          |
        Collector (C)
        BC547
        Base (B) ←——[1kΩ]←—— Touch with finger to 3.3V/5V / connect to +
        Emitter (E)
          |
         GND
```

Pin order of BC547 (flat side facing you, pins down): EBC (left to right)

```
Flat face:    [ BC547 ]
Pins:          E  B  C
```

**Experiment steps:**

1. Wire the circuit. Keep base disconnected. LED should be OFF.
2. Touch base resistor to 5V (from an Arduino, or even through your finger to the positive terminal). LED lights up!
3. Use a 10kΩ, then 100kΩ resistor. Observe LED still lights (proving tiny base current switches large collector current).

**Calculating gain:**
```
If 5V source, 1kΩ base resistor:
Ib = (5 - 0.7) / 1000 = 4.3mA

If LED takes 20mA:
β = Ic/Ib = 20mA / 4.3mA ≈ 4.7  (in saturation)

But in active region: Ic = β × Ib = 200 × 4.3mA = 860mA → saturated (β appears low in saturation)
```

---

### Experiment 5: Build a Simple Photodetector

**Purpose:** Observe how light generates electron-hole pairs in a semiconductor.

**Option A: Any Rectifier Diode (1N4007)**

Bright sunlight or a flashlight can generate a tiny photocurrent in a normal diode, even in reverse bias. It acts as a crude photodiode.

**Circuit:**
```
[Ammeter in microamp range] in series with reverse-biased diode.
  − battery − diode (reverse biased) − ammeter − + battery

Cover diode completely: read ~0 μA (just dark current)
Shine bright light on diode: current may rise slightly.

Works MUCH better with a real photodiode (BPW34, ₹30-50).
```

**Option B: Solar Cell**

A solar cell IS a large photodiode! Use any small solar garden light cell (strip it out — usually GaAs or CdTe, 5×5cm).

```
Connect multimeter in voltage mode across solar cell terminals.
Indoors: ~0.2V (from room light)
Bright sunlight: 0.4 – 0.6V per cell

Stack two cells in series: ~1V in direct sun!
```

This is the photovoltaic effect — light creates electron-hole pairs that the built-in junction field separates into current.

---

### Experiment 6: Make a Cuprous Oxide Semiconductor (Real Home Semiconductor!)

This is the most spectacular experiment — make an actual semiconductor junction from copper at home.

**Materials:**
- Copper wire or a copper sheet (10cm × 10cm)
- Gas stove or electric hot plate
- Water
- Salt (NaCl)
- Sandpaper
- Crocodile clips
- Multimeter

**Process:**

1. **Sand the copper sheet** until it's bright and shiny.
2. **Heat it on the stove** for 5-10 minutes. It will oxidize:
   - First, it turns brown (CuO — cupric oxide, black)
   - Actually what you want forms underneath: Cu₂O (cuprous oxide — red/brick colored, P-type semiconductor!)
3. **Cool it slowly** at room temperature for 20-30 minutes. Do NOT quench in water.
4. **Gently rub off the black outer CuO layer.** The brick-red Cu₂O remains on the copper surface.

**Now you have a natural P-N junction:**
```
Bottom: Cu (conductor, effectively N-side)
Middle: Cu₂O (P-type semiconductor)
Top: Air/oxide interface
```

5. **Test it:**
   - Place the copper sheet in salt water.
   - Use copper wire + clip as one electrode, graphite from a pencil as the other (dip in water but don't touch the copper plate).
   - Shine a bright light on the Cu₂O surface.
   - Measure voltage: **you should get 0.1–0.3V in direct sunlight!**

This is literally the first working photoelectric/solar cell ever made — discovered by Bruno Lange in 1932. You're doing century-old semiconductor physics at home.

**Alternative simpler test:**
Use multimeter in diode mode — probe across the junction. You may measure a forward voltage of 0.2–0.5V (P-type Cu₂O to N-type Cu).

---

### Experiment 7: LED Reveals Its Band Gap (Measuring with a Multimeter)

**Purpose:** Calculate the band gap of an LED's semiconductor from its forward voltage.

**Theory:**
When an LED emits light, the photon energy ≈ the energy gap the electron fell across ≈ the forward voltage × electron charge.

```
Photon energy E = hf = hc/λ
Also:          E ≈ V_f × e   (V_f = forward voltage, e = electron charge)

So: λ ≈ hc / (V_f × e) = 1240 nm·eV / V_f (in eV)
```

**Procedure:**
1. Forward-bias each color LED (use 1kΩ resistor in series with 5V).
2. Measure V_f across each LED with multimeter.
3. Calculate the peak wavelength: λ = 1240 / V_f (where V_f is in volts = eV numerically).

**Expected results:**

| Color  | Measured V_f | Calculated λ     | Actual λ       |
|--------|-------------|------------------|----------------|
| Red    | ~1.9V       | 1240/1.9 = 653nm | 620–650nm ✓    |
| Yellow | ~2.1V       | 1240/2.1 = 590nm | 580–600nm ✓    |
| Green  | ~2.2V       | 1240/2.2 = 564nm | 520–560nm ~✓   |
| Blue   | ~3.0V       | 1240/3.0 = 413nm | 460–470nm ~✓   |
| White  | ~3.1V       | 1240/3.1 = 400nm | phosphor-shifted|

Note: There's some discrepancy because the simple model ignores series resistance and the LED emits a range of wavelengths, not a single one. But it's remarkably close!

**Bonus:** If you have an IR LED (TV remote), measure it — V_f ≈ 1.2V → λ = 1240/1.2 = 1033nm. Invisible to human eyes but your phone camera (which has no IR filter removed) should show it glowing purple.

---

### Experiment 8: Crystal Radio — Semiconductor Detecting Real Radio Signals

This uses the non-linear I-V curve of a diode (PN junction) to demodulate AM radio. No battery needed — it runs entirely on captured radio energy.

See your detailed guide at: `radio_stages/stage_02_crystal_set.md`

Key semiconductor concept demonstrated: The diode's non-linear response "rectifies" the AM carrier wave, allowing the audio envelope to be extracted. A linear component (resistor, capacitor) cannot do this — you NEED the semiconductor junction.

---

### Experiment 9: Transistor Curves with Arduino (Advanced)

**Purpose:** Plot the full I-V curves of a BJT using your ESP32 as a curve tracer.

**Components:**
- BC547 transistor
- ESP32 (or Arduino)
- Two 10kΩ resistors (DAC output protection)
- 100Ω resistor (collector current sense)

**Concept:**
Use the ESP32's DAC outputs to sweep base and collector voltages, measure resulting currents via ADC, plot Ic vs Vce for various Ib values (the classic transistor family of curves).

```
ESP32 DAC1 → 10kΩ → Base (controls Ib)
ESP32 DAC2 → 100Ω → Collector → sense voltage → ADC1
Emitter → GND
ADC2 → between DAC2 and 100Ω (Vce measurement)
```

This is effectively building a rudimentary semiconductor curve tracer that costs under ₹100 but replicates instruments costing ₹50,000+.

A full Arduino/ESP32 sketch for this would:
1. Set DAC1 to fixed voltage (fixed Ib)
2. Sweep DAC2 from 0 to 3.3V
3. At each step, read ADC and calculate Ic = (V_DAC2 - V_ADC2)/100
4. Repeat for several DAC1 voltages
5. Plot curves over Serial plotter

---

### Experiment 10: Thermal Runaway Demonstration

**Purpose:** See why transistor bias circuits must be temperature-compensated.

**Components:**
- BD139 power transistor (or 2N3055)
- 1kΩ base resistor
- 100Ω collector resistor
- 9V battery
- Multimeter (current mode)
- Heat gun / hair dryer

**Setup:** Simple common-emitter circuit, fixed base bias.

```
9V → 1kΩ → Base (BD139)
9V → 100Ω → Collector (BD139)
Emitter → GND
Measure Ic through the 100Ω (voltage across it / 100)
```

**Observation:**
1. Note collector current at room temperature.
2. Heat the transistor body with hair dryer.
3. Watch Ic increase as temperature rises.

**Explanation:** More heat → more thermal carriers → higher β → more Ic → more power dissipation → more heat. This is thermal runaway. Real amplifier designs use **emitter degeneration** (a resistor in the emitter leg) to provide negative feedback and prevent this.

---

## 16. Quick Reference

### Key Numbers to Remember

| Quantity                      | Silicon Value     |
|-------------------------------|-------------------|
| Band gap                      | 1.12 eV           |
| Intrinsic carrier density     | 1.5×10¹⁰ /cm³    |
| Diode forward voltage         | 0.6 – 0.7 V       |
| Temperature coefficient of Vf | −2 mV/°C          |
| Electron mobility             | 1400 cm²/V·s      |
| Hole mobility                 | 450 cm²/V·s       |
| Relative permittivity         | 11.7              |
| Melting point                 | 1414°C            |
| Breakdown field               | 3×10⁵ V/cm        |

### Semiconductor Hierarchy

```
Material
└── Semiconductor (4 valence electrons, tunable conductivity)
    ├── Elemental: Si, Ge
    └── Compound: GaAs, GaN, InP, SiC, CdTe
        ├── Direct band gap → Light emission → LEDs, Lasers
        └── Indirect band gap → No efficient light emission → Silicon
            └── Doped (Extrinsic)
                ├── N-type: Donor dopant (P, As) → Free electrons
                └── P-type: Acceptor dopant (B, Al) → Free holes
                    └── PN Junction
                        ├── Diode (rectification, light detection, emission)
                        ├── BJT (current-controlled current amplifier)
                        └── MOSFET (voltage-controlled, digital logic, power)
```

### Troubleshooting Your Experiments

| Problem | Likely cause | Fix |
|---------|-------------|-----|
| Diode shows no forward drop | Diode shorted | Replace |
| Diode blocks in both directions | Diode open/blown | Replace |
| LED doesn't light | Wrong polarity | Flip LED (longer leg = Anode) |
| Transistor always ON | Base-collector shorted | Replace |
| Transistor always OFF | Base not biased correctly | Check base resistor value |
| Cu₂O cell gives no voltage | Black CuO not removed | Rub more gently |

### Next Steps After These Experiments

1. Build a full-wave bridge rectifier (4 diodes + capacitor → DC from AC)
2. Build a simple audio amplifier using BC547 (see `09_transistors_bjt.md`)
3. Build a Zener voltage regulator
4. Combine a photodiode + transistor amplifier → light meter
5. Study your ESP32 datasheet: find the MOSFET gate threshold voltage in the specifications
6. Build a crystal set radio (see `radio_stages/stage_02_crystal_set.md`)

---

*This document is part of the embedded systems learning project. It is intentionally written for a complete beginner — if you find a section unclear, that is a documentation problem, not yours. The physics is deep but the intuition is buildable.*
