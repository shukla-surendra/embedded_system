# 04 — Reading Circuits: Schematics & Breadboards

Before you can build anything, you need to read the blueprint. In electronics, that blueprint is called a **schematic**.

---

## What Is a Schematic?

A schematic is a diagram that shows how components connect electrically — not how they look physically. It uses standardized symbols for each component and lines to show connections.

Schematic symbols you already know:
```
Resistor:        ──[/\/\/]──
Capacitor:       ──|  |──
LED:             ──|>|──→
Battery/power:   + side = VCC line, – side = GND line
Wire junction:   ──●──   (dot means wires connect)
Wire crossing:   ──┼──   (NO dot = wires cross but DON'T connect)
Ground (GND):    ──┴──  (triangle or three horizontal lines)
VCC/Power:       ──┬──  (arrow pointing up, or labeled +3.3V, +5V, VCC)
```

---

## Reading a Simple Schematic

**Example: LED connected to ESP32 GPIO**

```
3.3V ───────────────────────────────── VCC line

GPIO4 ──[220Ω]──|>|── GND

                ↑    ↑
           Resistor  LED (anode left, cathode right)
```

Reading this left-to-right:
1. GPIO4 outputs 3.3V when we tell it to in code
2. Current flows through the 220Ω resistor (limits current)
3. Current flows through the LED (produces light)
4. Current reaches GND (completes the circuit)

---

## The Breadboard

A breadboard lets you build circuits without soldering. It has rows of holes connected internally by metal clips.

### Internal connections of a breadboard

```
  a b c d e     f g h i j
  ─ ─ ─ ─ ─     ─ ─ ─ ─ ─
1 ●─●─●─●─●     ●─●─●─●─●  1   ← Row 1: all holes in same row are connected
2 ●─●─●─●─●     ●─●─●─●─●  2   ← Row 2: all holes in same row are connected
3 ●─●─●─●─●     ●─●─●─●─●  3
...

+ ●─●─●─●─●─●─●─●─●─●─●─●  ← Power rail: entire horizontal strip connected
- ●─●─●─●─●─●─●─●─●─●─●─●  ← GND rail: entire horizontal strip connected
```

**Key rules:**
- Holes in the same **row** (same number, left or right of center gap) are connected to each other
- Holes on opposite sides of the **center gap** are NOT connected
- The two long rails on each edge (+/−) run the full length of the board
- The center gap separates the board into two halves — this is where you straddle ICs (chips)

### Wiring the LED example on a breadboard

```
Step 1: Connect ESP32 3.3V pin  → + power rail (red wire)
Step 2: Connect ESP32 GND pin   → – power rail (black wire)
Step 3: Place 220Ω resistor     → one leg in row 10a, other in row 10c (jumps gap)
Step 4: Place LED               → anode (long leg) row 10e, cathode row 10g
Step 5: Wire                    → GPIO4 → row 10a (same row as resistor leg)
Step 6: Wire                    → row 10g → – power rail
```

---

## Color Conventions for Wires

Not a rule, but a universal convention:

| Color | Meaning |
|-------|---------|
| Red | Positive voltage (VCC, 3.3V, 5V) |
| Black | Ground (GND) |
| Other colors | Signal wires (data, GPIO, etc.) |

Following this convention makes debugging much easier — you can instantly see if power and ground are correct.

---

## Schematic Shorthand

### VCC and GND symbols

Instead of drawing a long wire from the battery to every component, schematics use shorthand:

```
    ↑          
   VCC   ← any component connected to this symbol gets +voltage

    ⊥          
   GND   ← any component connected to this symbol is connected to ground
```

All VCC symbols in a schematic are connected together. All GND symbols are connected together.

### Net names / labels

Sometimes wires are labeled instead of drawn:

```
[ESP32 GPIO4] ─── DATA_LINE ─── [Sensor]
```

If two wires have the same label, they are connected — even if there's no line drawn between them.

---

## Common Mistakes to Avoid

| Mistake | What happens |
|---------|-------------|
| Plugging into wrong breadboard row | Component not connected where you think |
| Bridging the center gap accidentally | Short circuit between two rows |
| LED in backwards | LED doesn't light, no harm done — just flip it |
| Electrolytic cap backwards | Can damage or destroy the cap |
| No current-limiting resistor for LED | LED burns out (might not be immediate) |
| Two GPIO outputs connected together | May damage the ESP32 if they output different voltages |

---

## Multimeter Basics

A multimeter measures voltage, current, and resistance. You'll use it constantly to debug circuits.

**Voltage measurement (most common):**
1. Set dial to **DC Voltage** (V with a straight line) — pick a range higher than what you expect
2. Put the **red probe** on the point you want to measure
3. Put the **black probe** on GND
4. Read the display

**Continuity test (checking if two points are connected):**
1. Set dial to the **continuity/beep symbol** (looks like a sound wave)
2. Touch probes to two points
3. Beep = connected, No beep = not connected

Use continuity to check:
- If a wire is broken
- If two breadboard rows are actually connected
- If you accidentally shorted two rails

---

## Practice

1. Draw a simple schematic (on paper) for: a 9V battery, a 470Ω resistor, and an LED in series. Label the + and – of the battery, and the A and K of the LED.

2. On a breadboard, which holes are connected? Row 5a–5e (same side)? Row 5a and 5f (across the gap)? The entire + power rail?

3. If you measure 0V between GPIO4 and GND but expect 3.3V, what are two possible causes?

**Answers:**
1. Battery + → Resistor → LED anode → LED cathode → Battery −. Current flows clockwise through all three in series.
2. 5a–5e are connected. 5a and 5f are NOT connected (gap). Entire + rail is connected.
3. Possible causes: (a) GPIO4 is set to LOW in code, (b) the wire from GPIO4 to the breadboard is not making contact, (c) the ESP32 is not powered.

---

**Module 01 complete.** You now have the foundation to understand every circuit in this course.

**Next module:** [02 — Microcontrollers](../02_microcontrollers/README.md)
