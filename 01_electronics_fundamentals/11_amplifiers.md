# Amplifiers — Which Components Do It and How

An amplifier takes a weak input signal and produces a stronger output signal.
The key idea: a small controlling signal controls a larger energy source.

---

## 1. Transistors (BJT — Bipolar Junction Transistor)

### What it is
A three-terminal semiconductor device: **Base (B)**, **Collector (C)**, **Emitter (E)**.
Two types: **NPN** and **PNP**.

### How it amplifies
- A small current injected at the **Base** controls a much larger current flowing from **Collector to Emitter**.
- Current gain is called **hFE** or **β (beta)** — typically 20 to 500.
- Formula: `IC = β × IB`
  - `IB` = small base current (input)
  - `IC` = large collector current (output)

### Visual analogy
Think of the Base as a water tap handle. A tiny twist (small base current) opens a valve that lets a large amount of water (collector current) flow through.

### Common use in circuits
- Audio amplifiers (microphone → speaker)
- Signal boosting in radio receivers
- Switching + amplification together

### Example: NPN Common-Emitter Amplifier
```
       +VCC
        |
       [RC]  ← collector resistor (load)
        |
        C
Vin → B    NPN  → Vout (amplified, inverted)
        E
        |
       [RE]
        |
       GND
```
- Input signal goes into Base through a resistor.
- Output is taken from Collector.
- Signal is amplified but **phase is inverted** (180° flip).

---

## 2. MOSFETs (Metal-Oxide-Semiconductor Field-Effect Transistor)

### What it is
Three terminals: **Gate (G)**, **Drain (D)**, **Source (S)**.
Two common types: **N-channel** and **P-channel**.

### How it amplifies
- A voltage at the **Gate** controls current flowing from **Drain to Source**.
- No current flows into the Gate — it is controlled by **voltage**, not current.
- This makes MOSFETs extremely efficient (less input power wasted).
- Gain is described as **transconductance (gm)**: how much drain current changes per volt of gate input.

### Visual analogy
Gate voltage is like squeezing a garden hose. More voltage = more current flows through the channel.

### Common use in circuits
- High-efficiency audio amplifiers (Class D)
- RF amplifiers in radio modules
- Motor speed control (PWM amplification)
- Power amplifiers in drones (ESC circuits use MOSFETs)

### Example: N-channel MOSFET Common-Source Amplifier
```
       +VDD
        |
       [RD]
        |
        D
Vin → G    MOSFET → Vout
        S
        |
       [RS]
        |
       GND
```
- Input voltage at Gate, output at Drain.
- Like BJT common-emitter — output is also **inverted**.

---

## 3. Operational Amplifiers (Op-Amps)

### What it is
An integrated circuit (IC) that contains many transistors inside.
Has two inputs: **Inverting (−)** and **Non-Inverting (+)**, and one output.
Common ICs: **LM741**, **LM358**, **TL071**, **NE5532** (audio).

### How it amplifies
- Amplifies the **difference** between the two input voltages.
- Open-loop gain is enormous (100,000× or more) — too much to use directly.
- In practice, **feedback resistors** set the exact gain you want.

### Two basic configurations

#### Non-Inverting Amplifier
```
         R2
        ┌──┤├──┐
        │      │
Vin+ ──→(+)    │
         Op-Amp ├──→ Vout
Vin− ──→(−)    │
        └──┤├──┘
            R1 to GND
```
- Gain = `1 + (R2 / R1)`
- Output is **in phase** with input.

#### Inverting Amplifier
```
Vin ──[Rin]──→ (−)
                Op-Amp ──→ Vout
          (+ ) → GND
               ↑
              [Rf] (feedback from Vout to −)
```
- Gain = `-(Rf / Rin)`
- Output is **inverted**.

### Common use in circuits
- Microphone pre-amplifiers
- Sensor signal conditioning (temperature, pressure)
- Audio equalizers and filters
- Comparators

---

## 4. JFETs (Junction Field-Effect Transistors)

### What it is
Three terminals: **Gate (G)**, **Drain (D)**, **Source (S)**.
Voltage-controlled like a MOSFET but with a physical p-n junction at the Gate.

### How it amplifies
- Gate voltage controls the width of a conducting channel between Drain and Source.
- Reverse-biasing the Gate pinches the channel, reducing current.
- Very high input impedance — almost no current drawn from the signal source.

### Common use
- Low-noise pre-amplifiers (microphones, guitar pickups)
- RF front-end amplifiers

---

## 5. Vacuum Tubes (Triodes, Pentodes)

### What it is
Old technology, still used in high-end audio. A glass tube containing electrodes in a vacuum.
Terminals: **Cathode**, **Grid**, **Plate (Anode)**.

### How it amplifies
- A small voltage on the **Grid** controls a large electron flow from Cathode to Plate.
- Exactly the same principle as a transistor — small signal controls large current — but done with electrons moving through vacuum instead of semiconductor material.

### Common use
- Vintage/audiophile amplifiers (guitar amps, HiFi)
- High-power RF transmitters

---

## Summary Table

| Component    | Control Method     | Input Terminal | Output Terminal | Typical Gain       | Best For                      |
|--------------|--------------------|----------------|-----------------|--------------------|-------------------------------|
| BJT          | Current (at Base)  | Base           | Collector       | 20–500× (current)  | General amplification         |
| MOSFET       | Voltage (at Gate)  | Gate           | Drain           | High (voltage)     | Power, efficiency, RF         |
| Op-Amp (IC)  | Voltage difference | +/− inputs     | Output pin      | Set by resistors   | Precision, sensors, audio     |
| JFET         | Voltage (at Gate)  | Gate           | Drain           | Moderate           | Low-noise, high-impedance     |
| Vacuum Tube  | Voltage (at Grid)  | Grid           | Plate           | 10–100× (voltage)  | Audio, high-power RF          |

---

## Key Concepts to Remember

| Term            | Meaning                                                              |
|-----------------|----------------------------------------------------------------------|
| Gain            | How much bigger the output is compared to input (e.g., 100× = 40dB) |
| β (beta / hFE)  | Current gain of a BJT transistor                                     |
| gm              | Transconductance — gain of a FET or tube                             |
| Feedback        | Sending part of output back to input — used to set precise gain      |
| Saturation      | When transistor is fully ON — no longer amplifying, just switching   |
| Biasing         | Setting up DC operating point so AC signal can swing cleanly         |

---

## Relevance to Your Drone Project

- **ESCs (Electronic Speed Controllers)** use MOSFETs to amplify PWM signals into high-current motor drive.
- **Radio receiver modules** use transistor/MOSFET RF amplifiers to boost weak signals from the transmitter.
- **Microphone/audio modules** on ESP32 boards use op-amps to bring microphone output to ADC-readable levels.
- Understanding amplifier gain helps you interpret sensor datasheets (signal conditioning stages).
