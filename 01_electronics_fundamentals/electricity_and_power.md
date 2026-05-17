# Understanding Electrical Power
## A Beginner's Guide for Drone & Embedded Systems Hobbyists

---

## The Big Picture First

Before diving into formulas, let's agree on one thing:

> **Electricity is just energy moving through wires.** Your job as a hobbyist is to control *how much* energy moves, *where* it goes, and *how fast*.

Everything else — voltage, current, resistance, power — is just vocabulary to describe that one idea.

---

## 1. Voltage — The "Pressure" That Pushes Electricity

### The Water Tank Analogy

Imagine a water tank sitting high up on a tower. The higher the tank, the more pressure at the bottom tap. That pressure is what *pushes* water through your pipes.

**Voltage is electrical pressure.** It is the force that pushes electrons through a wire.

```
High Voltage (high tank)         Low Voltage (low tank)

    ┌──────────┐                      ┌──┐
    │  WATER   │  tall tower          │  │  short tower
    │  TANK    │                      │  │
    └────┬─────┘                      └─┬┘
         │ lots of pressure              │ little pressure
         ▼                              ▼
    ~~~~tap~~~~                    ~~~~tap~~~~
    strong flow                    weak flow
```

### In Real Life — Drone Battery Example

A drone LiPo battery is labeled **"3S 11.1V"**. That "11.1V" is voltage — the pressure available to push current through your ESCs and motors.

```
  3S LiPo Battery
  ┌─────────────────────────────────┐
  │  [Cell 1]  [Cell 2]  [Cell 3]  │
  │   3.7V  +   3.7V  +   3.7V    │  = 11.1V total
  └───────────────┬─────────────────┘
                  │ 11.1V pushes electricity
                  ▼
           [ ESC ] → [ Motor ] → Propeller spins!
```

### Key Facts About Voltage

| Voltage | Where you'll see it |
|---------|---------------------|
| 3.3V    | ESP32 logic, sensors |
| 5V      | USB power, servo signal |
| 7.4V    | 2S LiPo battery |
| 11.1V   | 3S LiPo (most common for mini drones) |
| 14.8V   | 4S LiPo (racing drones) |
| 22.2V   | 6S LiPo (large drones) |

**Unit:** Volt (V)
**Symbol:** V (in formulas)
**Measured with:** Voltmeter (red probe = +, black probe = −)

> **Rule of thumb:** Higher voltage → motors spin faster → drone flies more aggressively. But higher voltage also means components must be rated to handle it.

---

## 2. Current — The "Flow Rate" of Electricity

### Back to the Water Analogy

Voltage is the pressure. But pressure alone doesn't tell you how much water actually flows. You also need to know the *pipe size*. A wide pipe lets a lot of water through. A narrow pipe restricts it.

**Current is the actual flow of electrons per second through a wire.**

```
Same pressure (voltage), different pipe widths:

  Pressure = 11.1V               Pressure = 11.1V

  ┌────────┐                     ┌────────┐
  │ TANK   │                     │ TANK   │
  └───┬────┘                     └───┬────┘
      │  narrow pipe                 ║  wide pipe
      │  (high resistance)           ║  (low resistance)
      ▼                              ▼
   tiny flow                     huge flow
   low current                   high current
   (e.g. 0.1A)                   (e.g. 30A)
```

### In Real Life — Motors and ESCs

Your brushless motor might pull **20 Amps** at full throttle. Your USB charger might supply only **2 Amps**. That's why you can't power a drone motor from your phone charger — the pipe isn't wide enough!

```
  LiPo Battery (can supply 100A+)
         │
         │ thick wire (handles high current)
         ▼
       [ ESC ] ← Electronic Speed Controller
         │
         │ 3 wires to motor
         ▼
    [ Brushless Motor ]
    Peak current: ~20-30A
    
  ──────────────────────────────────
  
  USB Port (can supply ~0.5-2A)
         │
         │ thin USB cable
         ▼
    [ ESP32 development board ]
    Current draw: ~0.2-0.5A ✓ fits fine
```

**Unit:** Ampere, shortened to Amp (A)
**Symbol:** I (in formulas — from French word "intensité")
**Measured with:** Ammeter — placed IN SERIES with the circuit

> **Safety note:** Current is what actually kills you, not voltage. Even low voltages can drive lethal current through your body under the right conditions. Always respect electricity.

### Current Rating = "How Much Can This Handle?"

Every component has a maximum current rating:

```
  Component         Max Current     What happens if exceeded?
  ─────────────────────────────────────────────────────────
  ESP32 GPIO pin    ~12 mA          Pin damaged, ESP32 dies
  Small LED         20 mA           LED burns out
  Servo motor       1-3 A           Wire heats up, BEC overloads
  20A ESC           20 A continuous ESC overheats, fails
  LiPo 30C 2200mAh  66 A            Battery swells, fire risk!
```

**C-rating on LiPo batteries:** "30C" means it can discharge at 30 × its capacity.
- 2200mAh battery × 30C = **66 Amps maximum** safe discharge rate.

---

## 3. Resistance — The "Friction" That Slows Electrons Down

### The Analogy

Imagine water flowing through a pipe full of pebbles. The pebbles slow the water down. More pebbles = more resistance = less flow for the same pressure.

**Resistance is anything that opposes the flow of electricity.**

```
  No resistance:          With resistance:
  
  ──────────────          ──────[RRRR]──────
  ~~~~~~~~~~~~~~~~~~~     ~~~~~~   ~~~~~~~~~~
  electrons zoom          electrons slow down
  freely                  energy released as heat
```

### Why Does Resistance Matter in Drones?

1. **Wire resistance** — thick wires have less resistance, so less heat and voltage drop.
2. **Resistors** — intentionally slow current to protect components (e.g., LED current limiting).
3. **Motor windings** — the copper wire inside your brushless motor has resistance; that's why motors get warm.
4. **Battery internal resistance** — old or cold batteries have high internal resistance; they sag under load.

```
  Fresh LiPo (low internal resistance):
  
    ┌───[Ri = 5mΩ]───┐
    │    Battery      │
    └────────────────┘──── 11.1V at load  ✓ good
  
  
  Worn/cold LiPo (high internal resistance):
  
    ┌───[Ri = 80mΩ]──┐
    │    Battery      │
    └────────────────┘──── 9.2V at load   ✗ voltage sag!
```

**Unit:** Ohm (Ω)
**Symbol:** R
**Measured with:** Multimeter in resistance mode (with power OFF!)

### Resistor Color Codes (Quick Reference)

```
  Physical resistor looks like this:
  
    ──┤ [band1][band2][multiplier][tolerance] ├──
  
  Color → Digit:
  Black=0, Brown=1, Red=2, Orange=3, Yellow=4
  Green=5, Blue=6, Violet=7, Gray=8, White=9
  
  Example: Brown-Black-Red-Gold = 1, 0, ×100, ±5% = 1000Ω = 1kΩ
```

---

## 4. Ohm's Law — The One Formula That Rules Them All

Now that you know Voltage (V), Current (I), and Resistance (R), here's how they relate:

```
  ┌─────────────────────────────────┐
  │                                 │
  │         V = I × R               │
  │                                 │
  │    Voltage = Current × Resistance│
  │                                 │
  └─────────────────────────────────┘
```

### The Magic Triangle

Cover what you want to find, and the formula appears:

```
       ┌───────┐
       │   V   │
       ├───┬───┤
       │ I │ R │
       └───┴───┘
  
  Cover V → V = I × R
  Cover I → I = V / R
  Cover R → R = V / I
```

### Practical Examples

**Example 1: LED current limiting resistor**

You want to connect an LED (needs 2V, 20mA) to an ESP32 GPIO pin (outputs 3.3V).

```
  ESP32 pin (3.3V) ──[R]──┤LED├── GND
  
  Voltage across resistor = 3.3V - 2V = 1.3V
  Current needed = 20mA = 0.020A
  
  R = V / I = 1.3V / 0.020A = 65Ω
  
  → Use 68Ω (nearest standard value) ✓
```

**Example 2: Checking motor wire heating**

Your motor wire has 0.05Ω resistance. Motor pulls 20A.

```
  V = I × R = 20A × 0.05Ω = 1V drop
  
  → 1V is wasted as heat in the wire.
  → On a 3S (11.1V) battery, that's ~9% efficiency loss.
  → Use thicker wire (lower R) to reduce this.
```

---

## 5. Power — How Much Energy Per Second?

### The Concept

Power tells you how fast energy is being used (or supplied).

```
  ┌──────────────────────────────────────┐
  │                                      │
  │         P = V × I                    │
  │                                      │
  │    Power (Watts) = Voltage × Current  │
  │                                      │
  └──────────────────────────────────────┘
```

### Real-World Drone Power Math

**How long will your battery last?**

```
  Battery: 3S 11.1V, 2200mAh
  Drone average current draw: 15A (hovering)
  
  Power consumption = 11.1V × 15A = 166.5 Watts
  
  Flight time = Capacity / Current
             = 2200mAh / 15000mA
             = 0.147 hours
             = ~8.8 minutes
  
  (In practice: 6-7 min, don't fully discharge LiPo)
```

**Motor power ratings explained:**

```
  Motor labeled "2204 - 2300KV, max 3S"
  
  At full throttle on 3S (11.1V):
  
  RPM = KV × Voltage = 2300 × 11.1 = 25,530 RPM (no load)
  
  Typical current at full throttle: ~18A
  
  Power = 11.1V × 18A = 199.8 Watts  ← that's per motor!
  Quad has 4 motors: 4 × 199.8 = ~800 Watts total
  
  That's like running 8 incandescent light bulbs — from a palm-sized battery!
```

### Power Summary Table

```
  Component             Voltage    Current    Power
  ──────────────────────────────────────────────────
  ESP32 (idle)          3.3V       0.05A      0.16W
  ESP32 (WiFi active)   3.3V       0.25A      0.83W
  Small servo           5V         0.5A       2.5W
  20A ESC + motor       11.1V      15A        166.5W
  FPV camera            5V         0.3A       1.5W
  Flight controller     5V         0.1A       0.5W
```

---

## 6. Putting It All Together — The Drone Power System

```
  ┌─────────────────────────────────────────────────────┐
  │                                                     │
  │   ┌──────────────────────────────────┐              │
  │   │   3S LiPo Battery (11.1V)        │              │
  │   │   Capacity: 2200mAh              │              │
  │   │   Internal resistance: ~10mΩ     │              │
  │   └──────────┬───────────────────────┘              │
  │              │ thick 12AWG wire (low R)              │
  │              ▼                                      │
  │   ┌──────────────────────────────────┐              │
  │   │   Power Distribution Board (PDB) │              │
  │   │   Distributes power to all ESCs  │              │
  │   └──┬──────┬──────┬──────┬──────┬──┘              │
  │      │      │      │      │      │                  │
  │      ▼      ▼      ▼      ▼      ▼                  │
  │   [ESC1] [ESC2] [ESC3] [ESC4]  [5V BEC]            │
  │      │      │      │      │      │                  │
  │      ▼      ▼      ▼      ▼      ▼                  │
  │   [M1]   [M2]   [M3]   [M4]  [FC @ 5V]            │
  │   Motor  Motor  Motor  Motor  Flight Controller     │
  │                                    │                │
  │                                    ▼                │
  │                               [Receiver]            │
  │                               [GPS]                 │
  │                               [Other sensors]       │
  └─────────────────────────────────────────────────────┘
  
  BEC = Battery Eliminator Circuit (converts 11.1V → 5V or 3.3V)
  ESC = Electronic Speed Controller (converts DC battery power to 3-phase AC for brushless motor)
```

---

## Quick Reference Cheat Sheet

```
  ┌─────────────────────────────────────────────────────────┐
  │              ELECTRICAL FUNDAMENTALS CHEAT SHEET        │
  ├──────────┬────────┬────────┬──────────────────────────┤
  │ Quantity │ Symbol │  Unit  │ Analogy                   │
  ├──────────┼────────┼────────┼──────────────────────────┤
  │ Voltage  │   V    │  Volt  │ Water pressure            │
  │ Current  │   I    │  Amp   │ Water flow rate           │
  │Resistance│   R    │  Ohm Ω │ Pipe friction             │
  │ Power    │   P    │  Watt  │ How fast water does work  │
  ├──────────┴────────┴────────┴──────────────────────────┤
  │ FORMULAS:                                               │
  │   Ohm's Law:   V = I × R                               │
  │   Power:       P = V × I                               │
  │   Combined:    P = I² × R    (heat in a wire)          │
  │                P = V² / R                              │
  ├────────────────────────────────────────────────────────┤
  │ MEMORY TRICKS:                                          │
  │   V = IR  →  "Violet Is Really (nice)"                 │
  │   P = VI  →  "Pretty Volts Invited"                    │
  └────────────────────────────────────────────────────────┘
```

---

## Section Summary

| Concept    | What it is                        | Drone example                        |
|------------|-----------------------------------|--------------------------------------|
| Voltage    | Electrical pressure               | 11.1V LiPo drives motors             |
| Current    | Flow rate of electrons            | Motor draws 20A at full throttle     |
| Resistance | Opposition to current flow        | Thin wire gets hot, causing fire risk |
| Power      | Energy used per second (V × I)    | 4 motors × 200W = 800W total draw    |
| Ohm's Law  | V = I × R, links all three        | Size your resistors for LED safety   |

> **Next step:** Now that you understand power, let's learn how to control it using microcontrollers — starting with the ESP32.
