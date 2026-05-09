# 01 — Electricity Basics

## The Water Analogy

The easiest way to understand electricity is to think about water flowing through pipes. This analogy isn't perfect, but it gets you 90% of the way there.

```
Water system          ←→   Electrical system
─────────────────────────────────────────────
Water                 ←→   Electric charge (electrons)
Water pressure        ←→   Voltage (Volts, V)
Flow rate             ←→   Current (Amperes / Amps, A)
Pipe diameter/friction ←→  Resistance (Ohms, Ω)
Pump                  ←→   Battery or power supply
```

---

## Voltage (V)

**Voltage is electrical pressure.** It's the force that pushes electrons through a circuit.

- Measured in **Volts (V)**
- A 9V battery has more "push" than a 1.5V AA battery
- Voltage is always measured *between two points* (it's a difference, not an absolute)

### Common voltages you'll encounter
| Source | Voltage |
|--------|---------|
| AA/AAA battery | 1.5V |
| Phone charger (USB) | 5V |
| ESP32 operating voltage | 3.3V |
| Arduino Uno operating voltage | 5V |
| Small drone LiPo battery | 3.7V per cell (7.4V for 2S, 11.1V for 3S) |
| Mains electricity (India) | 230V AC |

> **Safety note:** Anything above ~50V DC or 25V AC can be fatal. The voltages used in this course (3.3V–12V DC) are safe to touch, but always be careful with LiPo batteries — they can catch fire if mishandled.

---

## Current (A)

**Current is the flow of electrons.** It's how many electrons actually move through the wire per second.

- Measured in **Amperes (Amps, A)**
- Smaller amounts are measured in **milliamps (mA)** — 1A = 1000mA
- Current flows from the positive terminal of a battery, through the circuit, and back to the negative terminal

### Why current matters
- Too much current through a component destroys it
- An LED might survive only 20mA — exceed that and it burns out
- A motor might need 5A to run — a supply that can only give 1A will struggle

### Common current draws
| Device | Typical current |
|--------|----------------|
| LED (standard) | 10–20 mA |
| ESP32 (active, WiFi on) | 80–240 mA |
| Small servo motor | 200–500 mA |
| Brushless motor (drone) | 5–30 A |

---

## Resistance (Ω)

**Resistance is how much a material opposes the flow of current.** High resistance = current struggles to flow. Low resistance = current flows easily.

- Measured in **Ohms (Ω)**
- Larger values: kilohms (kΩ) = 1000Ω, megohms (MΩ) = 1,000,000Ω

### Good conductors vs insulators
| Material | Behavior |
|----------|----------|
| Copper wire | Very low resistance — current flows freely |
| Resistor (component) | Fixed, known resistance |
| Air | Extremely high resistance — current doesn't flow |
| Plastic (wire insulation) | Extremely high resistance |

---

## The Circuit: Making Everything Work Together

For current to flow, there must be a **complete loop** — from the positive terminal of the power source, through the components, back to the negative terminal. This loop is called a **circuit**.

```
         +-------[LED]-------[Resistor]-------+
         |                                    |
       + |                                  - |
      [Battery]                         [Battery]
         |                                    |
         +------------------------------------+
```

Break the loop anywhere (open switch, cut wire, burnt component) and current stops — the circuit is "open." Close it back up and current flows — the circuit is "closed."

### Short circuit
If positive connects directly to negative with very low resistance (like a bare wire touching both battery terminals), current rushes through with nothing to limit it. This causes:
- Massive heat
- Wire/battery damage
- Possible fire

Always have some resistance in your circuit.

---

## Direct Current (DC) vs Alternating Current (AC)

**DC (Direct Current):** Electrons flow in one direction. This is what batteries produce. All microcontrollers, sensors, and the electronics in this course run on DC.

**AC (Alternating Current):** Electrons reverse direction many times per second. This is what comes from wall outlets (50Hz in India = reverses 50 times/second). Your phone charger converts AC to DC.

You will work almost entirely with DC in this course.

---

## Practice

1. Look at a AA battery. Find the + and - terminals. The voltage is 1.5V — can you explain what that means in your own words?

2. Your ESP32 needs 3.3V at up to 240mA. If you're powering it from a USB power bank that outputs 5V at 2A, what device must sit between them to drop the voltage? (Answer: a voltage regulator — covered in Module 06)

3. Name two reasons why a short circuit is dangerous.

---

**Next:** [02 — Ohm's Law](02_ohms_law.md)
