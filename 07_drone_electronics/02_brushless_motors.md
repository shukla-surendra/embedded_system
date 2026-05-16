# 02 — Brushless Motors

## Why Brushless?

Older motors used **brushes** — carbon contacts that physically rubbed against a rotating commutator to switch current direction. They wore out, created sparks, and lost efficiency.

**Brushless motors** use electronics (the ESC) to switch current direction. No physical contact = no wear, no sparks, higher efficiency, higher RPM, longer life.

Drones exclusively use brushless motors except for very small toys.

---

## How a Brushless Motor Works

Inside every brushless motor:
- **Stator** — the fixed part with copper wire coils wound around iron cores
- **Rotor (Bell)** — the spinning outer shell with permanent magnets

```
Cross-section view (simplified, 3 coils, 4 magnets):

        N   S
       ┌─────┐
    S  │  ●  │  N     ← rotor (spinning outer bell with magnets)
       │ ╱│╲ │
   N   │╱ │ ╲│  S
       │  │  │
       │A B C │       ← stator coils (fixed, connected to ESC)
       └─────┘
        S   N
```

The ESC energizes coils A, B, C in sequence (3-phase). Each energized coil creates a magnetic field. The permanent magnet rotor chases that rotating field — this is what makes the motor spin.

Changing the sequence speed changes the RPM. Changing the phase order reverses direction.

---

## Outrunner vs Inrunner

| Type | Description | Used For |
|------|-------------|----------|
| **Outrunner** | Outer shell (bell) spins, stator is inside and fixed | Drones, RC aircraft — high torque, direct drive |
| **Inrunner** | Inner shaft spins, stator is outside | RC cars — high RPM, used with gearbox |

Drone motors are **outrunners**. The bell is what you see spinning. Your propeller bolts to the bell.

```
Outrunner:
  [fixed stator core]
       │
  ═══════════   ← spinning bell (attach prop here)
       │
  [mounting base — bolts to frame]
```

---

## The Motor Naming Convention

Motors are named by stator dimensions: **XXYY**

- **XX** = stator diameter in mm
- **YY** = stator height in mm

Example: **2207**
- 22mm diameter stator
- 7mm tall stator

```
Common drone motor sizes:

  1306  → tiny, micro drones (< 3")
  2204  → 3–4" props
  2306  → 5" props (light builds)
  2207  → 5" props (most popular)
  2306  → 5–6" props
  2812  → 7–10" props (efficiency)
  4014  → heavy lifters (10"+)
```

Bigger stator = more copper = more torque = can spin bigger props and lift more weight.

---

## KV Rating — RPM per Volt

**KV** = RPM per volt (with no load).

```
RPM = KV × Battery Voltage

Example:
  Motor: 2207, 2300KV
  Battery: 4S = 16.8V (fully charged)

  RPM = 2300 × 16.8 = 38,640 RPM (no load)

Under load with a prop: roughly 70–80% of this
  ≈ 27,000–31,000 RPM actual
```

### High KV vs Low KV

| KV | Prop | Use |
|----|------|-----|
| 2400KV+ | Small props (3–5") | Racing — max speed |
| 1700–2400KV | 5" props | Sport flying, FPV freestyle |
| 900–1700KV | 5–7" props | Efficiency, long range |
| 300–900KV | 8–12" props | Heavy lifters, cinema drones |

**High KV** = spins faster → smaller props → more speed, less efficiency  
**Low KV** = spins slower → larger props → more thrust efficiency, heavier lifts

---

## Motor Sizing Guide

### Step 1 — Know Your Drone's All-Up Weight (AUW)

AUW = frame + motors + ESCs + FC + battery + camera + everything.

A typical 5" FPV quad: 500–750g AUW.

### Step 2 — Thrust-to-Weight Ratio (TWR)

For a quad to hover: each motor must produce at least **AUW/4** thrust.  
For sport flying, target **4:1 TWR** (each motor produces 4× its share of the total weight).

```
Example:
  AUW = 600g
  TWR target = 4:1
  Total thrust needed = 600g × 4 = 2400g
  Per motor = 2400g ÷ 4 = 600g per motor

→ Choose a motor rated for ≥600g thrust on your battery voltage
```

### Step 3 — Motor Thrust Tables

Motor manufacturers publish thrust tables. Example for a 2207 2450KV motor:

| Throttle | Thrust | Current | Efficiency |
|----------|--------|---------|------------|
| 25% | 210g | 3.2A | — |
| 50% | 590g | 11.8A | — |
| 75% | 980g | 26.1A | — |
| 100% | 1350g | 45.3A | — |

Hover at 50% throttle = good design. Hover at 80%+ = underpowered.

---

## Propeller and Motor Matching

Prop size and pitch affect how hard the motor works.

```
Prop notation: 5040  or  5x4.0
               │  │       │ │
               │  └───────┘ └── Pitch (inches per revolution)
               └────────────── Diameter (inches)
```

**Pitch** = how far the prop would move forward in one revolution if air were solid (like a screw).

Higher pitch = more speed, more current draw.
Lower pitch = more thrust efficiency, less top speed.

### Matching Rules of Thumb

| Motor Size | Recommended Prop Range |
|-----------|------------------------|
| 1306 | 3"–4" |
| 2204 | 4"–5" |
| 2207 | 4"–6" |
| 2306 | 5"–6" |
| 2812 | 6"–9" |

**Never run a prop larger than recommended** — the motor draws too much current and burns out.

---

## Motor Direction and Propeller Selection

```
Standard quad motor layout (Betaflight default):

        Front
    M1 (CW)  M2 (CCW)
       ●        ●
        \      /
         ──────
        /      \
       ●        ●
    M3 (CCW)  M4 (CW)
        Back
```

- CW motors use **CW propellers** (marked "R" or with a clockwise twist)
- CCW motors use **CCW propellers** (marked "N" or counter-clockwise twist)
- Running the wrong prop direction = drone flips immediately on takeoff

### Changing Motor Direction

Swap any **2 of the 3 phase wires** at the ESC. That's it.

Or in software: most ESC firmware (BLHeli_32, AM32) has a direction setting you can change without rewiring.

---

## Motor Poles

Inside the motor are magnets arranged in pairs (poles). Common configurations: 12N14P (12 stator teeth, 14 poles).

The number of poles affects the relationship between electrical cycles and actual shaft rotations:

```
Shaft RPM = Electrical RPM × (2 / Pole Pairs)
          = Electrical RPM × (2 / 7)   for 14-pole motor
```

You don't usually need this for basic builds, but it matters if you're writing your own ESC firmware.

---

## Motor Maintenance

Brushless motors are nearly maintenance-free. But watch for:

- **Loose screws** — vibration loosens motor mounting screws. Blue Loctite on motor screws.
- **Bent shaft** — crashes bend shafts. Symptoms: vibration at all throttle levels.
- **Damaged magnets** — strong crash can crack or dislodge magnets.
- **Burnt windings** — running oversized props or stalling. Symptoms: motor gets extremely hot, loses power.
- **Bearing wear** — spin motor by hand. Should feel smooth, no grinding.

---

## Reading a Motor Datasheet

When you see a motor listing, here's what to check:

```
Motor: EMAX ECO II 2207 1700KV

Stator: 22 × 7mm
KV: 1700
Max voltage: 6S (25.2V)
Max current: 47A
Motor weight: 34g
Shaft diameter: 3mm
Mount: 16×16mm or 19×19mm
Recommended prop: 5"–6"
Thrust (4S, 5" prop): 800g max
```

Always check:
1. KV matches your battery voltage and prop size
2. Max current the ESC can handle ≥ motor's max current
3. Mount pattern matches your frame

---

## Practice

1. A motor is labeled 2306. What does 23 mean? What does 06 mean?
2. You have a 2450KV motor and a 3S (12.6V fully charged) battery. What is the approximate max unloaded RPM?
3. Your drone's AUW is 800g. You want a 4:1 thrust-to-weight ratio. How much thrust does each motor need to produce (at maximum)?
4. A prop is labeled 6045. What is the diameter? What is the pitch?
5. You swap two of the three motor wires at the ESC. What changes?
6. Why do racing drones use high-KV motors while cargo drones use low-KV motors?
