# 06 — Prototyping Hardware: PCB Boards, Breadboards, and Assembly

## What You Bought

| Item | Size | Holes | Best Use |
|------|------|-------|----------|
| Universal PCB (single-sided) | 6×8 cm | ~576 | Compact permanent circuits |
| Universal PCB (single-sided) | 9×15 cm | ~2025 | Full project builds |

These are **perfboards** — single-sided FR4 fibreglass boards with copper pads at each hole, on a 2.54mm (0.1") grid. The same spacing as DIP ICs, through-hole components, and standard pin headers.

---

## Breadboard vs Perfboard vs PCB — When to Use Each

```
Stage          Tool                  Why
─────────────────────────────────────────────────────────
Experimenting  Solderless breadboard  Plug/unplug freely, no soldering
Prototyping    Perfboard              Semi-permanent, reliable connections
Final build    Custom PCB / perfboard Clean, compact, survives vibration
Production     Manufactured PCB       Professional, repeatable
```

For a DIY drone: build on breadboard first, then move to perfboard once the circuit is proven.

---

## Understanding the Universal PCB

### Single-Sided vs Double-Sided

**Single-sided** (what you have): copper pads only on the bottom. Each hole has one isolated copper ring. You solder component leads to the bottom and run wires or solder bridges to connect them.

**Double-sided**: copper on both sides, often with through-hole plating. More expensive, better for complex circuits.

### Hole Grid and Spacing

```
2.54mm grid = 0.1" grid

Holes per cm = 1 / 0.254 ≈ 4 holes per cm

6×8 cm board:
  6 cm ÷ 0.254 cm = ~24 holes wide
  8 cm ÷ 0.254 cm = ~31 holes tall
  Total: ~744 holes (some at edges may be unusable)

9×15 cm board:
  ~35 holes wide × ~59 holes tall ≈ 2065 holes
```

The 2.54mm spacing means:
- Standard DIP ICs (like 555 timer, ATmega328) fit directly across the centre gap
- 0.1" pin headers fit perfectly
- ESP32 and other modules with 0.1" pitch headers plug straight in

---

## Soldering on Perfboard — Technique

### Tools needed
- Soldering iron, 25–40W, with fine tip
- 60/40 or 63/37 solder, 0.6–0.8mm diameter
- Flux (optional but helps)
- Solder wick or desoldering pump (for mistakes)
- Tweezers
- Wire cutters / flush cutters

### Step-by-step

**1. Plan before soldering**

Sketch the layout on paper first. Once soldered, mistakes are hard to fix.

```
Example layout sketch for a voltage regulator circuit:

[ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
[ ] [+] [IN] [ ] [OUT] [-] [ ] [ ]
[ ] [C1+] [ ] [C2+] [ ] [ ] [ ]
[ ] [GND] [ ] [GND] [ ] [ ] [ ]
```

**2. Insert components from the top (non-copper side)**

Bend leads slightly outward to hold the component in place before soldering:
```
Component body (top)
      │
   ───┤├───     ← bend leads ~30° outward
      │
   copper pad (bottom)
```

**3. Solder from the bottom**

- Touch iron tip to the junction of lead + copper pad (not just the lead)
- Apply solder to the pad — it should flow and form a shiny cone
- Remove iron — joint should solidify in 2–3 seconds
- Good joint: shiny, cone-shaped, wets both lead and pad
- Bad joint: dull/grainy (cold joint), or balled-up not wetting pad

**4. Trim leads**

Use flush cutters to trim excess lead length after soldering. Cut close to the solder joint, not flush with the board.

**5. Connect pads with wire or solder bridges**

Pads are isolated. To connect two adjacent pads: run a short wire or melt a solder bridge between them.

For longer runs: use 24–26 AWG solid-core wire. Strip 3–4mm, thread through pad hole, solder.

---

## Layout Strategies

### Power rails first

Always lay out the power rails (VCC and GND) before placing components.

```
9×15 board layout example:

Top row: ── GND ── GND ── GND ── GND ── GND ── GND ──
Bottom row: ── 3.3V ── 3.3V ── 3.3V ── 3.3V ──

Connect all GND points to top rail.
Connect all VCC points to bottom rail.
```

### Keep signal wires short

Long wires act as antennas and pick up noise. I2C and SPI lines should be as short as possible — especially important for the NRF24L01 SPI connection.

### Group related components

```
[Power section]     [ESP32]     [Sensor section]
AMS1117 + caps  |   module   |   MPU + BMP + OLED
```

### Leave space for rework

Don't pack components too tight. Leave one empty row between component groups for later modifications.

---

## Component Placement for Your Drone Circuit

### 6×8 cm board — transmitter (ground station)

Fits comfortably:
- LOLIN32 D1 board (via pin headers)
- NRF24L01 adapter (via pin headers)
- 2× decoupling caps (100nF + 10µF)
- Power input header (2 pins)
- 2× PS2 joystick connectors (5-pin headers each)

### 9×15 cm board — drone flight controller

Fits comfortably:
- 38-pin ESP32 NodeMCU (via pin headers)
- MPU-6050 (via 4-pin I2C header)
- BMP280 (via 4-pin I2C header)
- NRF24L01 adapter (via 8-pin header)
- HC-SR04 connector (via 4-pin header)
- ESC signal output headers (4× 3-pin servo headers)
- AMS1117-5V module (via pin header or direct solder)
- Decoupling caps throughout

---

## Using Pin Headers — The Right Way

Pin headers let you mount and unmount modules without desoldering. Critical for a project you'll modify.

**Female headers** (socket): solder to the board, modules plug in.
**Male headers** (pins): solder to the module, plug into female headers or breadboard.

```
Female header soldered to board:
    ┌─┐ ┌─┐ ┌─┐ ┌─┐  ← module plugs in here
    │ │ │ │ │ │ │ │
────┴─┴─┴─┴─┴─┴─┴─┴──  ← perfboard
       (solder here)
```

**Tip:** When soldering headers, insert the mating header first to keep alignment:
1. Plug male header into female header
2. Insert both into the board
3. Solder — they're perfectly aligned
4. Remove mating header after soldering cools

---

## Common Perfboard Mistakes

| Mistake | Consequence | Fix |
|---------|-------------|-----|
| Cold solder joint (dull, grainy) | Intermittent connection — worst possible failure in a flying drone | Reheat with iron, add a little fresh solder |
| Solder bridge between adjacent pads | Short circuit | Use solder wick to remove excess |
| Component in wrong orientation (electrolytic cap) | Immediate destruction on power-up | Test polarity before soldering |
| No decoupling caps near ICs | Random resets, interference | Add 100nF between VCC and GND of every IC |
| Power rails not connected | Nothing works | Check continuity with multimeter |
| SPI/I2C wires too long | Communication errors, data corruption | Keep signal traces under 5cm on the board |

---

## Testing Before Power-On Checklist

Before applying power to a freshly soldered board:

```
□ Visual inspection — no solder bridges between adjacent pads
□ Measure GND–VCC resistance (should be >1kΩ, not near 0)
□ Confirm all polarised components (electrolytic caps, LEDs) are correct orientation
□ Check all module headers are fully seated
□ Apply power with current-limited supply or with a 1A fuse in line
□ Measure VCC pin voltages before connecting modules
```

Using a multimeter in continuity mode, verify:
- All GND pads connect to GND rail (beeps)
- All VCC pads connect to VCC rail (beeps)
- VCC and GND rails do NOT connect to each other (should not beep)

---

## Marking Up Your Board

Label your boards with a permanent marker or paint pen:

```
Mark on the board:
- "VCC 3.3V" and "GND" on the power rails
- Component reference designators (C1, R1, U1)
- Input/output labels on edge connectors
- Board name and date
```

---

## Wire Gauge Guide for Perfboard

| Wire | Use case |
|------|----------|
| 30 AWG wire-wrap wire | I2C, SPI, digital signal jumpers |
| 26 AWG solid core | General signal wires |
| 24 AWG | Low-current power (3.3V sensors) |
| 22 AWG | 5V power distribution, up to ~1A |
| 20 AWG | Up to ~2A (motor signal lines) |
| 18 AWG | Battery wires, ESC power (not on the perfboard — these go to external terminals) |

For the perfboard circuit: 26 AWG (0.4mm) for signals, 22 AWG (0.6mm) for power rails.

---

## Drone Assembly Note: Vibration

A flying drone vibrates intensely. Solder joints and connections that work fine on a bench can fail in flight.

**Rules for drone perfboards:**
1. All solder joints must be smooth and shiny — no cold joints
2. Use socket headers for all modules — not direct solder
3. Add hot glue or epoxy to reinforce tall components and headers after testing is complete
4. Use zip ties or foam tape to secure wires — no loose wire hanging near propellers
5. Test by physically shaking the board hard — recheck all connections

---

## What to Try

1. Lay out your AMS1117 + cap circuit on paper before soldering
2. Solder a 2-pin header onto the 6×8 board — inspect the joint under a phone camera
3. Use a multimeter in continuity mode to verify two pads are connected by your solder bridge
4. Practice desoldering one lead — clean with solder wick
5. Lay out the full transmitter circuit on the 6×8 board with pin headers (no active soldering yet) — confirm everything fits

---

## Next Steps

- **06_power_systems/02_ams1117_power_module.md** — first circuit to build on the 6×8 board
- **07_drone_electronics** — full build layout diagrams for both boards
