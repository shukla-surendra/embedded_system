# Complete Hobbyist Reference — Every Term, Concept, and Skill You Need

A single-document master reference for anyone learning electronics and embedded systems. Covers everything from the first week to advanced builds. Use it as a dictionary, a checklist, and a mental model builder.

---

## Chapter 1: Units, Prefixes, and Numbers

Before anything else — you must be comfortable reading values on datasheets and component labels.

### SI Prefixes

| Prefix | Symbol | Multiplier    | Example                          |
|--------|--------|---------------|----------------------------------|
| Giga   | G      | × 10⁹         | 2.4 GHz = 2,400,000,000 Hz      |
| Mega   | M      | × 10⁶         | 1 MHz = 1,000,000 Hz            |
| Kilo   | k      | × 10³         | 10 kΩ = 10,000 ohms             |
| milli  | m      | × 10⁻³        | 100 mA = 0.1 A                  |
| micro  | μ      | × 10⁻⁶        | 100 μF = 0.0001 F               |
| nano   | n      | × 10⁻⁹        | 10 nF = 0.00000001 F            |
| pico   | p      | × 10⁻¹²       | 22 pF = 0.000000000022 F        |

**Common confusion:** 1 mA (milliamp) ≠ 1 MA (megaamp). Case matters — m is milli, M is mega.

### Core Electrical Units

| Quantity      | Unit    | Symbol | What It Measures                         |
|---------------|---------|--------|------------------------------------------|
| Voltage       | Volt    | V      | Electrical pressure / potential difference |
| Current       | Ampere  | A      | Flow of charge (electrons per second)   |
| Resistance    | Ohm     | Ω      | Opposition to current flow              |
| Power         | Watt    | W      | Energy used per second (V × A)          |
| Energy        | Joule   | J      | Total energy (W × seconds)              |
| Capacitance   | Farad   | F      | Ability to store charge                 |
| Inductance    | Henry   | H      | Ability to oppose changes in current    |
| Frequency     | Hertz   | Hz     | Cycles per second                       |
| Charge        | Coulomb | C      | Amount of electric charge (1 A × 1 sec)|
| Impedance     | Ohm     | Ω      | AC resistance (includes reactance)      |

### Decibels (dB)

Decibels are a logarithmic ratio — used everywhere in electronics (signal levels, gain, attenuation, SNR).

```
Power ratio:    dB = 10 × log10(P2 / P1)
Voltage ratio:  dB = 20 × log10(V2 / V1)

Key values to memorise:
  +3 dB  ≈ 2× power, √2 × voltage
  +6 dB  ≈ 2× voltage, 4× power
  +10 dB = 10× power
  +20 dB = 10× voltage, 100× power
  −3 dB  ≈ half power (the "−3 dB point" = half power bandwidth)
  −20 dB = 1/10 voltage
  0 dB   = no change (ratio of 1)

Absolute levels:
  dBm = dB relative to 1 milliwatt
    0 dBm = 1 mW
    10 dBm = 10 mW
    30 dBm = 1 W
    −100 dBm = 0.1 picowatt (GPS signal level)
```

---

## Chapter 2: Fundamental Electrical Laws

### Ohm's Law
```
V = I × R
I = V / R
R = V / I

V = voltage (volts)
I = current (amperes)
R = resistance (ohms)
```

**The most-used formula in all electronics.** If you know any two values, you can find the third.

### Power Law
```
P = V × I
P = I² × R
P = V² / R

P = power (watts)
```

**Power = heat.** If a resistor dissipates more power than its rating, it overheats and fails.

### Kirchhoff's Voltage Law (KVL)
The sum of all voltages around any closed loop equals zero.
```
Practical meaning:
  Battery supplies 9V → resistors in series must drop a total of 9V.
  3V across R1 + 6V across R2 = 9V total ✓
```

### Kirchhoff's Current Law (KCL)
The sum of all currents entering a node equals the sum leaving.
```
Practical meaning:
  All current that flows into a junction must flow out.
  100 mA into node → 60 mA + 40 mA out ✓
```

### Series and Parallel Rules

**Resistors in series:**
```
Rtotal = R1 + R2 + R3 + ...
Current is the same through all. Voltage divides.
```

**Resistors in parallel:**
```
1/Rtotal = 1/R1 + 1/R2 + 1/R3 + ...
Two equal resistors in parallel = half the value of one.
Voltage is the same across all. Current divides.
```

**Capacitors in series:**
```
1/Ctotal = 1/C1 + 1/C2 + ...  (opposite of resistors)
```

**Capacitors in parallel:**
```
Ctotal = C1 + C2 + C3 + ...
```

**Inductors in series:**
```
Ltotal = L1 + L2 + L3 + ...
```

### Voltage Divider
Two resistors in series, output taken from the junction:
```
       +V
        │
       [R1]
        │
        ├──── Vout = V × R2 / (R1 + R2)
       [R2]
        │
       GND

Example: V = 5V, R1 = 10kΩ, R2 = 10kΩ
  Vout = 5 × 10k / (10k + 10k) = 2.5V

Use: Level shifting 5V → 3.3V, reading potentiometers, biasing.
Warning: Output sags when you draw current from it (loading effect).
         Use a buffer op-amp if the load varies.
```

---

## Chapter 3: Electronic Components

### Resistor

**What it does:** Limits current flow, drops voltage, converts electrical energy to heat.

**Reading resistor values:**
- Colour code (4-band): First two bands = digits, third = multiplier, fourth = tolerance.
- Common colours: Black=0, Brown=1, Red=2, Orange=3, Yellow=4, Green=5, Blue=6, Violet=7, Grey=8, White=9.
- Multipliers: Gold=×0.1, Silver=×0.01, Brown=×10, Red=×100, Orange=×1000.
- SMD marking: e.g., "103" = 10 × 10³ = 10 kΩ. "4R7" = 4.7 Ω.

**Power rating:** Resistors come in ¼W, ½W, 1W, 2W. Choose a power rating > calculated dissipation.
```
Dissipated power = I² × R = V² / R = V × I
Safe rule: derate to 50% — a ¼W resistor should dissipate < 125 mW in practice.
```

**Types:**
| Type         | Properties                                | Use                          |
|--------------|-------------------------------------------|------------------------------|
| Carbon film  | Cheap, noisy, ±5%                         | General non-critical use     |
| Metal film   | Low noise, ±1%, ±0.1%                     | Precision, audio, measurement|
| Wirewound    | Very precise, handles high power          | Current sensing, power apps  |
| SMD (0402–2512) | Surface mount, tiny to medium         | PCB designs                  |
| NTC thermistor| Resistance decreases with temperature    | Temperature sensing          |
| PTC thermistor| Resistance increases with temperature   | Self-resetting fuse, sensing |
| LDR          | Resistance decreases with light           | Light sensing, auto night-light |
| Potentiometer| Variable resistor (mechanical)            | Volume control, position sensing |
| Trimpot      | Small adjustable resistor (screwdriver)   | Calibration, bias setting    |

---

### Capacitor

**What it does:** Stores charge (energy in electric field), blocks DC, passes AC. Acts as a low-pass filter with a resistor.

**Charging behaviour:**
```
Time constant τ = R × C (seconds)
  After 1τ: 63% charged
  After 5τ: 99.3% charged (considered fully charged)

Example: R = 10kΩ, C = 100μF → τ = 1 second
```

**Capacitive reactance (AC impedance):**
```
Xc = 1 / (2π × f × C)
  Higher frequency → lower impedance (capacitor passes AC more easily)
  Xc = 0 at DC (fully blocks) → ∞ at f=0 → 0 at f=∞
```

**Types and use:**

| Type              | Range        | Polarised | Properties                           | Use                             |
|-------------------|--------------|-----------|--------------------------------------|---------------------------------|
| Ceramic (MLCC)    | 1 pF – 100 μF | No       | Low ESR, small, stable               | Decoupling, filtering           |
| Electrolytic (Al) | 1 μF – 100,000 μF | Yes  | Large value, cheap, high ESR        | Bulk power supply filtering     |
| Tantalum          | 1 μF – 1000 μF | Yes    | Low ESR, compact, expensive          | Precision filtering             |
| Film (polyester)  | 1 nF – 100 μF | No      | Stable over temperature, no polarity | Audio, timing, precision        |
| Supercapacitor    | 0.1 F – 3000 F | No     | Huge capacitance, low voltage (2.7V) | Energy storage, backup power   |

**Critical warnings:**
- Electrolytic caps are polarised — reverse voltage will destroy them (can explode).
- Voltage rating must exceed the circuit voltage — use 2× margin minimum.
- C0G/NP0 ceramics are stable — X5R/X7R change value with voltage and temperature.
- Electrolytic caps have a lifespan — dry out at high temperature over years.

---

### Inductor

**What it does:** Stores energy in a magnetic field, opposes changes in current. With a capacitor, forms tuned (resonant) circuits.

**Inductive reactance:**
```
XL = 2π × f × L
  Higher frequency → higher impedance (inductor blocks AC more)
  XL = 0 at DC (wire, fully conducts) → ∞ at f=∞
```

**Flyback voltage:** When current through an inductor is suddenly interrupted, it generates a large voltage spike (can be hundreds of volts) to maintain current flow. This destroys transistors and MOSFETs.

**Flyback diode:** Place a diode in reverse across any inductive load (motor, relay, solenoid) to safely absorb the flyback voltage spike.

```
    +V ─────┬──────────
            │[motor / relay coil]
            ├──[diode, cathode to +V]──  (flyback diode)
            │
          [switch/MOSFET]
            │
          GND
```

**Types:**
| Type          | Core           | Use                                    |
|---------------|----------------|----------------------------------------|
| Air core      | Air            | RF circuits, VHF/UHF, no saturation   |
| Ferrite core  | Ferrite bead/rod | General filtering, switching PSU     |
| Toroidal      | Donut ferrite  | Low EMI (field contained), PSU chokes |
| Power inductor| Ferrite        | Buck/boost converter energy storage   |
| Ferrite bead  | Ferrite sleeve | HF noise suppression on power lines   |

---

### Diode

**What it does:** Allows current to flow in one direction only. Acts as a one-way valve.

**Forward voltage (Vf):** The voltage drop across a forward-biased diode:
```
Silicon:  ~0.6–0.7V
Schottky: ~0.2–0.4V (lower drop — better for power circuits)
LED:      ~1.8–3.6V (varies by colour)
Zener:    ~2.4–200V (reverse breakdown — controlled)
```

**Types:**

| Type       | Key Property                          | Use                                        |
|------------|---------------------------------------|--------------------------------------------|
| Rectifier  | Handles high current                  | AC to DC power supplies, reverse protection |
| Schottky   | Low Vf, very fast switching           | Flyback protection, power circuits, RF     |
| Zener      | Conducts in reverse at precise voltage| Voltage reference, overvoltage clamp       |
| TVS        | Very fast Zener — handles large spikes | ESD protection, transient suppression     |
| LED        | Emits light when forward biased       | Indicators, displays, IR remotes           |
| Photodiode | Generates current from light          | Light sensing, optical communication       |
| Schottky pair | Two Schottky in one package        | Ideal diode circuits, OR-ing power rails   |

**LED current limiting resistor:**
```
R = (Vsupply − Vf) / Iled

Example: 5V supply, red LED Vf=2V, Iled=20 mA
  R = (5 − 2) / 0.020 = 150 Ω
  Use 150Ω or 220Ω (standard value, safer)
```

---

### Transistor (BJT)

**What it does:** Current-controlled switch or amplifier. Small base current controls large collector current.

**NPN transistor as switch:**
```
       +V
        │
       [Rc] ← collector resistor (load)
        │
        C (collector)
Vin → [Rb] → B (base)
        E (emitter)
        │
       GND

Rb = (Vin − 0.7) / (Ic / β × 10)  ← 10× overdrive for solid saturation
```

**States:**
- **Cut-off**: Ib = 0, transistor OFF, Ic ≈ 0.
- **Active**: Ic = β × Ib, amplifying.
- **Saturation**: Fully ON, Vce ≈ 0.2V, Ic limited by circuit not β.

**β (hFE):** Current gain. Varies widely even between same part number — design for minimum β.

---

### MOSFET

**What it does:** Voltage-controlled switch or amplifier. Gate voltage controls drain-source current. No gate current needed (very high input impedance).

**N-channel MOSFET as switch (most common):**
```
       +V
        │
      [Load]
        │
        D (drain)
Vin → G (gate) [MOSFET]
        S (source)
        │
       GND

Gate voltage > Vth → MOSFET ON
Gate voltage = 0  → MOSFET OFF
Always add pull-down resistor (10kΩ) from Gate to GND — prevents floating gate from randomly switching.
```

**Logic-level MOSFET:** Vgs threshold < 3.3V — can be driven directly by ESP32/Arduino GPIO (3.3V or 5V). Standard MOSFETs need 10V gate drive.

**RDS(on):** Drain-source resistance when ON — determines heat dissipation. Choose low RDS(on) for high-current switching.

---

### Relay

**What it does:** Electrically controlled mechanical switch — isolates control circuit from load circuit.

```
Control side: 5V coil → electromagnet pulls switch contact
Load side: Completely separate — can switch 240V AC mains

Use flyback diode across coil — always.
Typical coil: 5V, 70–100 mA.
Contacts: rated in A at V (e.g., 10A at 250VAC).
```

**Solid-State Relay (SSR):** Electronic equivalent — optocoupler + TRIAC or MOSFET, no moving parts, longer life, faster switching, no click.

---

### Crystal Oscillator

**What it does:** Provides a stable, precise clock frequency using piezoelectric resonance.
- Crystals are specified by frequency (MHz), load capacitance (pF), and accuracy (ppm).
- Typical XTAL for Arduino/ESP32: 16 MHz or 26 MHz.
- Always use the two load capacitors specified in the datasheet (usually 12–22 pF to GND).
- Most MCU modules have the crystal already fitted.

---

## Chapter 4: Circuit Concepts

### Ground (GND)

The reference point — all voltages are measured relative to it. **Not the same as Earth ground.** In a battery-powered circuit, ground is just the negative terminal.

**Common ground:** All parts of a circuit must share the same ground. If two sub-circuits have different grounds, signals between them will be measured incorrectly.

**Ground loops:** When two devices are connected and have slightly different ground potentials — causes noise (hum in audio, offset in ADC readings). Use a single-point ground or differential signalling.

---

### Pull-Up and Pull-Down Resistors

Used to define the default state of a signal line when nothing is actively driving it.

```
Pull-up resistor:                  Pull-down resistor:
    +V ──[R]──┬── Signal            Signal ──┬──[R]── GND
               │                             │
          (switch to GND)              (switch to +V)

Default state when switch open:    Default state when switch open:
  Signal = HIGH                      Signal = LOW
```

**Typical values:** 1kΩ–100kΩ. Lower = stronger pull (less noise susceptible), more current. Higher = weaker pull (more noise susceptible), less current.

**I2C requires pull-up resistors** — both SDA and SCL must be pulled up to VCC (typically 4.7kΩ for standard mode, 2.2kΩ for fast mode).

**Internal pull-ups:** Most MCU GPIO pins have a built-in software-configurable pull-up resistor (~20–50 kΩ). Use `INPUT_PULLUP` in Arduino.

---

### Decoupling Capacitors (Bypass Capacitors)

Every IC power pin needs a capacitor from VCC to GND as close as possible to the chip.

**Why:** Digital ICs draw sudden bursts of current when switching — the inductance of PCB traces and power wires cannot supply this instantly. A local capacitor acts as a charge reservoir, preventing the supply voltage from drooping momentarily.

**Best practice:**
```
100 nF ceramic (0.1 μF): for HF switching noise (place < 1 mm from IC pin)
10 μF electrolytic or tantalum: for LF bulk decoupling (nearby)
```

**You should add these to every IC in your circuit.** Circuits without decoupling caps will behave erratically, especially at higher frequencies.

---

### RC Filters

A resistor and capacitor together form a first-order filter.

**Low-pass filter (LPF):**
```
Input ──[R]──┬── Output
              │
             [C]
              │
             GND

Cutoff frequency: fc = 1 / (2π × R × C)
  Below fc: signal passes
  Above fc: signal attenuated

Use: Remove HF noise from ADC input, smooth PWM to analog
```

**High-pass filter (HPF):**
```
Input ──[C]──┬── Output
              │
             [R]
              │
             GND

Same formula, but:
  Above fc: signal passes
  Below fc: signal attenuated

Use: Remove DC offset from AC signal, audio coupling
```

**LC filters:** Using inductors and capacitors — steeper roll-off, used in power supply filters and RF circuits.

---

### Impedance Matching

When a source drives a load, maximum power transfer occurs when source impedance = load impedance. Mismatches cause reflections in RF systems.

```
RF standard: 50Ω everywhere — coax cables, antenna connectors, RF chip ports.
Audio: Low impedance source (< 100Ω) → High impedance load (> 10kΩ) for best results.
Speaker: 4Ω or 8Ω — amplifier output impedance must be < 1Ω.
```

---

### Voltage Regulator Concepts

**Linear regulator:** Drops excess voltage as heat. Vout = Vin − Vdrop. Efficiency = Vout/Vin.
```
Example: 12V in, 5V out linear regulator
  Efficiency = 5/12 = 42% — 58% wasted as heat.
  Heat = (Vin − Vout) × Iout = 7V × 1A = 7W → needs heatsink!
```

**LDO (Low Drop-Out):** Linear regulator that works with very small Vin−Vout difference (< 0.3V). Needed when input is close to output (e.g., 3.6V LiPo → 3.3V output).

**Switching regulator (buck):** Efficiency 80–95%. Stores energy in inductor, switches at high frequency. Generates switching noise — needs output filtering.

**Buck:** Step down (Vout < Vin).
**Boost:** Step up (Vout > Vin).
**Buck-boost:** Either direction.
**SEPIC:** Non-inverting buck-boost.

---

### PWM — Pulse Width Modulation

A digital signal that simulates an analog voltage by switching between HIGH and LOW at high speed:

```
100% duty cycle: ████████████  always HIGH = Vcc
 50% duty cycle: ██░░██░░██░░  average = Vcc/2
 25% duty cycle: █░░░█░░░█░░░  average = Vcc/4
  0% duty cycle: ░░░░░░░░░░░░  always LOW = 0V

Average voltage = Duty Cycle % × Vcc
```

**Resolution:** 8-bit PWM = 256 steps (0–255). 10-bit = 1024 steps. 16-bit = 65536 steps.

**Frequency:** How fast the PWM switches. Higher frequency = smoother output (easier to filter) but harder for MCU to generate.
- LED dimming: 200 Hz+ (below 50 Hz flickers visibly).
- Motor speed control: 1–20 kHz typical.
- Servo control: 50 Hz standard (1–2 ms pulse width).
- Audio: 20–200 kHz.

**RC low-pass filter converts PWM to analog voltage:**
```
PWM pin ──[R 1kΩ]──┬── Analog out (0–Vcc)
                   [C 1μF]
                    │
                   GND
fc = 1/(2π × 1000 × 0.000001) = 159 Hz → smooths 1 kHz PWM well
```

---

### ADC — Analog to Digital Converter

Converts a real-world voltage into a digital number.

```
10-bit ADC, 3.3V reference:
  Resolution = 3.3V / 2^10 = 3.3 / 1024 = 3.22 mV per step
  ADC output 0 = 0V
  ADC output 512 = 1.65V
  ADC output 1023 = 3.297V

Formula: Voltage = (ADC_value / (2^bits − 1)) × Vref
```

**ESP32 ADC caveats:**
- 12-bit (0–4095) on 18 channels.
- Non-linear at the extremes (0–0.1V and 3.1–3.3V are compressed).
- Changes when WiFi is active (WiFi shares power rail).
- Use external ADS1115 for precise measurements.

**Sampling rate (Nyquist theorem):** To accurately digitise a signal of frequency f, you must sample at **at least 2f** (Nyquist rate). Sample at less → aliasing (higher frequencies appear as lower false frequencies).

---

### DAC — Digital to Analog Converter

Converts a digital number back to a voltage.

- ESP32 has two 8-bit DAC channels (GPIO25, GPIO26) — 256 voltage levels.
- MCP4725: 12-bit I2C DAC — 4096 levels, much better resolution.
- Use for: audio output, setting reference voltages, analog control signals.

---

## Chapter 5: Microcontroller Concepts

### GPIO — General Purpose Input/Output

The digital pins on your microcontroller. Each pin can be configured as:
- **Input**: reads HIGH (logic 1) or LOW (logic 0).
- **Output**: drives HIGH or LOW.
- **Input with pull-up/pull-down**: uses internal resistor.
- **Analog input (ADC)**: reads voltage level.
- **Special function**: UART TX/RX, SPI CLK/MOSI/MISO, I2C SDA/SCL, PWM output.

**Logic levels:**
| Family | LOW (0) | HIGH (1) | Max current per pin |
|--------|---------|----------|---------------------|
| 3.3V (ESP32, STM32) | < 0.8V | > 2.0V | 12–40 mA |
| 5V (Arduino Uno) | < 1.5V | > 3.0V | 40 mA |

**Never connect 5V signal directly to 3.3V GPIO — use a level shifter.**

**Never draw more than the rated current from a GPIO** — use a transistor/MOSFET to drive higher currents (LEDs > 10 mA, motors, relays, buzzers).

---

### Interrupts

An interrupt stops the main program instantly to run a special function (ISR — Interrupt Service Routine) when an event occurs.

```
Types:
  RISING  — triggers when pin goes LOW → HIGH
  FALLING — triggers when pin goes HIGH → LOW
  CHANGE  — triggers on either transition
  LOW     — triggers continuously while pin is LOW (level sensitive)

Arduino example:
  attachInterrupt(digitalPinToInterrupt(pin), myISR, FALLING);

void myISR() {
  // Keep this SHORT — no Serial.print, no delay, no long loops
  // Set a flag, increment a counter — handle the rest in loop()
}
```

**ISR rules:**
- Must be very short — every microsecond counts.
- Variables shared with ISR must be `volatile`.
- No `delay()`, no `Serial.print()` inside ISR.
- On ESP32: ISR must be in IRAM (`IRAM_ATTR` attribute).

---

### Timers

Hardware counters in the MCU that count clock cycles independently of the main program. Used for:
- Generating precise PWM.
- Measuring time intervals (ultrasonic sensor pulse width).
- Triggering events at exact intervals.
- Generating baud rates for UART.

Most MCUs have multiple independent timers (ESP32 has 4 hardware timers + 2 timer groups).

---

### Watchdog Timer (WDT)

A special timer that resets the MCU if the program stops responding (gets stuck in an infinite loop or crashes).

```
Program must "pet/feed" the watchdog at regular intervals.
If program freezes → watchdog expires → automatic reset.

Arduino/ESP32: yield() or delay() automatically reset the watchdog.
For long operations: wdt_reset() manually.
```

---

### Clock Speed and Timing

**Clock speed:** How many instruction cycles per second (MHz). ESP32 = 240 MHz = 240 million cycles/sec.

**One clock cycle ≠ one instruction** — complex instructions take multiple cycles (varies by MCU architecture).

**Timing accuracy:**
```
millis() — milliseconds since boot. Overflows after ~49 days.
micros() — microseconds since boot. Overflows after ~70 minutes.
delayMicroseconds() — blocking delay in μs. Accurate to ~1 μs.
```

**Non-blocking timing:**
```cpp
unsigned long previousTime = 0;
const long interval = 1000;  // 1 second

void loop() {
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval) {
        previousTime = currentTime;
        // do something every 1 second
    }
    // rest of loop runs freely
}
```
This is better than `delay()` because it does not block the rest of the program.

---

### Memory Types in MCUs

| Type    | Abbreviation | Volatile? | What lives here                        |
|---------|--------------|-----------|----------------------------------------|
| Flash   | FLASH        | No        | Program code, constants (PROGMEM)     |
| SRAM    | RAM          | Yes       | Variables, stack, heap                |
| EEPROM  | EEPROM       | No        | User settings, calibration (limited writes ~100k) |
| Registers | —          | Yes       | CPU working variables, peripheral config |

**ESP32:** 4 MB flash, 520 KB SRAM, no EEPROM (uses NVS partition in flash instead).

**Stack vs heap:**
- Stack: local function variables, automatic allocation. Overflows if too many nested calls or large local arrays.
- Heap: dynamic allocation (`malloc`/`new`). Fragments over time in long-running programs — prefer stack or static allocation in embedded.

---

## Chapter 6: Communication Protocols

### UART — Universal Asynchronous Receiver Transmitter

The simplest serial protocol — two wires (TX and RX), no clock signal.

```
Wiring: Device A TX → Device B RX
        Device A RX ← Device B TX
        Common GND (must be shared)

Frame: [Start bit][8 data bits][Optional parity][Stop bit(s)]
       1 start bit (always LOW) + 8 bits + 1 stop bit = 10 bits per byte

Baud rate: bits per second. Common: 9600, 115200, 230400, 460800.
Time per bit at 115200 baud = 1/115200 = 8.68 μs
```

**Key settings (must match on both ends):** Baud rate, data bits (usually 8), parity (usually None), stop bits (usually 1). This is "8N1" — the standard.

**Use for:** GPS modules (NMEA at 9600 baud), Bluetooth modules (HC-05 at 9600/38400), debug output to Serial monitor, talking to any external module with a UART.

---

### SPI — Serial Peripheral Interface

Synchronous full-duplex serial bus — one master, one or more slaves.

```
4 wires:
  SCLK / SCK: clock — master generates, slaves follow
  MOSI: Master Out Slave In — data from master to slave
  MISO: Master In Slave Out — data from slave to master
  CS / SS: Chip Select — master pulls LOW to select a specific slave

Timing: Data shifts on clock edge. SPI has 4 modes (CPOL × CPHA).
  Mode 0 (CPOL=0, CPHA=0): most common — data sampled on rising clock edge.
  Check datasheet for correct mode.

Speed: Up to 80 MHz on ESP32. Typical: 1–40 MHz.
```

**Multiple slaves:** Each slave has its own CS pin. All share MOSI, MISO, SCLK. Master pulls CS LOW for the target slave, keeps others HIGH.

**Use for:** nRF24L01 (RF module), SD cards, TFT displays (ST7789), MAX31855 (thermocouple), flash memory (W25Q32).

---

### I2C — Inter-Integrated Circuit

Two-wire synchronous bus — one master, many slaves on the same two wires.

```
2 wires:
  SDA: Serial Data (bidirectional)
  SCL: Serial Clock

Both lines need pull-up resistors to VCC (2.2kΩ–10kΩ).
Each device has a 7-bit address (0x00–0x7F → up to 127 devices).
Some devices have selectable addresses (ADDR pin to GND or VCC).

Speed: 100 kHz (Standard), 400 kHz (Fast), 1 MHz (Fast+), 3.4 MHz (High-speed).

Transaction:
  Master sends: [START][7-bit address + R/W bit][ACK][data bytes][STOP]
```

**I2C address conflicts:** Two devices with the same address on the same bus → collision. Use I2C multiplexer (TCA9548A) to separate them, or choose devices with different addresses.

**Scanning for I2C devices:**
```cpp
for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0)
        Serial.println(addr, HEX);  // found a device
}
```

**Use for:** BME280, MPU-6050, SSD1306 display, ADS1115, PCA9685, RTC modules, almost every sensor module.

---

### 1-Wire

Single wire carries power and data. Dallas/Maxim protocol.

```
Single pin (+ GND). Parasitic power: device steals power from the data line itself.
Strong pull-up resistor (4.7kΩ) required.
Each device has a unique 64-bit ID → many devices on one wire.
```

**Use for:** DS18B20 temperature sensor, iButton security tokens.

---

### PWM as a Protocol (Servo Control)

RC servos use PWM at 50 Hz:
```
1.0 ms pulse = full left (0°)
1.5 ms pulse = centre (90°)
2.0 ms pulse = full right (180°)
Period = 20 ms (50 Hz)
```

**ESC control:** Same protocol — 1000 μs = minimum throttle, 2000 μs = full throttle.

---

### DSHOT (Digital Protocol for ESCs)

Replaces analog PWM for ESC control — digital packet:
```
DSHOT150, DSHOT300, DSHOT600, DSHOT1200 (numbers = kbps)
11-bit throttle value + 1-bit direction + 4-bit CRC
No calibration needed, no noise susceptibility, supports telemetry.
DSHOT300 is standard for most builds.
```

---

### CAN Bus (Controller Area Network)

Differential two-wire bus used in automotive and industrial applications:
```
Two wires: CAN_H and CAN_L (differential → immune to noise)
Multi-master: any node can transmit, collision avoidance via priority
Speed: up to 1 Mbps (CAN) or 5–8 Mbps (CAN FD)
Max nodes: 110+ on one bus
Use in drones: DroneCAN/UAVCAN protocol — connect GPS, ESCs, sensors
```

---

### USB

| Version  | Speed         | Protocol    | Connector       |
|----------|---------------|-------------|-----------------|
| USB 1.1  | 12 Mbps       | Differential| Type-A/B        |
| USB 2.0  | 480 Mbps      | Differential| Type-A/B/Micro/Mini |
| USB 3.x  | 5–20 Gbps     | + SuperSpeed| Type-A/B/C      |
| USB4     | 40–80 Gbps    | Thunderbolt | Type-C          |

**For hobbyists:** USB mostly appears as:
- USB-UART bridge (CH340, CP2102) — for flashing firmware.
- USB CDC (Communications Device Class) — MCU appears as COM port.
- USB HID — MCU appears as keyboard/mouse/gamepad.
- USB Mass Storage — MCU appears as a USB flash drive.

---

## Chapter 7: Power and Batteries

### Battery Chemistry

| Chemistry | Nominal V | Full V | Empty V | C-rating | Notes                       |
|-----------|-----------|--------|---------|----------|-----------------------------|
| Alkaline AA | 1.5V   | 1.6V   | 0.9V    | 0.1C     | Non-rechargeable            |
| NiMH AA   | 1.2V      | 1.4V   | 1.0V    | 1C       | Rechargeable, low memory effect |
| Li-Ion 18650 | 3.7V  | 4.2V   | 2.7V    | 1–3C     | Phone batteries, laptop packs |
| LiPo      | 3.7V/cell | 4.2V/cell | 3.0V/cell | 25–150C | Drones, RC vehicles       |
| LiFePO4   | 3.2V/cell | 3.65V/cell | 2.5V/cell | 5–30C | Safer, longer cycle life  |
| Lead Acid | 2.0V/cell | 2.4V/cell | 1.75V/cell | 0.2C | Heavy, cheap, cars         |

### LiPo Terminology

**Cell count (S):** Number of cells in series.
```
1S = 3.7V nominal (3.0V–4.2V)
2S = 7.4V nominal (6.0V–8.4V)
3S = 11.1V nominal (9.0V–12.6V)
4S = 14.8V nominal (12.0V–16.8V)
6S = 22.2V nominal (18.0V–25.2V)
```

**Capacity (mAh):** Milliamp-hours. How much charge is stored.
```
2000 mAh battery at 1A drain → lasts 2 hours
2000 mAh battery at 10A drain → lasts 12 minutes
```

**C-rating:** Maximum safe continuous discharge current as a multiple of capacity.
```
2000 mAh × 25C = 50 A maximum continuous discharge current
```

**Never discharge LiPo below 3.0V per cell** — irreversible damage. Set low voltage cutoff in ESC or flight controller.

**LiPo fire hazard:** Puncture, over-charge, short circuit, or charging damaged cells can cause thermal runaway (fire). Store in a LiPo bag. Never charge unattended. Never charge damaged packs.

---

### Power Budget Calculation

Always calculate your power budget before building.

```
List all components and their current draw:
  ESP32 WiFi active:     250 mA
  OLED display:           20 mA
  BME280 sensor:           1 mA
  Buzzer:                 30 mA
  NeoPixel LEDs (×4):    80 mA
  Total:                 381 mA

Select battery:
  3.7V 1000 mAh LiPo
  Runtime = 1000 mAh / 381 mA = 2.6 hours (theoretical)
  Practical: ×0.7 efficiency = ~1.8 hours

Regulator selection:
  Total current 381 mA → use AMS1117 (1A rated) with thermal headroom.
```

---

### ESD — Electrostatic Discharge

The static shock that destroys components — can generate thousands of volts.

**ESD rules for hobbyists:**
- Touch a grounded metal object before handling ICs.
- Keep ICs in their original anti-static bags until used.
- Work on an anti-static mat when assembling.
- SMD components are most vulnerable.
- CMOS inputs (ESP32, STM32, MOSFETs gate) are most sensitive.

---

## Chapter 8: Test and Measurement Equipment

### Multimeter

Every hobbyist's first tool. Measures:

| Mode          | What it reads        | How to use                                |
|---------------|----------------------|-------------------------------------------|
| DC voltage    | Battery, rail voltage| Red to + point, black to GND             |
| AC voltage    | Mains, AC signals    | Touch probes to both terminals            |
| Current (DC)  | Circuit current      | Break circuit, meter in series. Set range before connecting. |
| Resistance    | Component value      | Power off circuit, measure across component |
| Continuity    | Beeps if connected   | Power off, probe two points — beep = connected |
| Diode test    | Forward voltage Vf   | Red to anode, black to cathode           |
| Capacitance   | Component value      | Power off and discharge cap first        |
| Frequency     | Signal frequency     | Probe oscillating signal                  |

**Critical safety rule:** Never measure current (mA/A mode) across a voltage source — this shorts the supply through the meter (destroys the fuse or the meter).

**Fuse in meter:** The current measuring path has a small fuse. Check it if current measurement always reads 0.

---

### Oscilloscope

Shows voltage waveforms over time — essential for debugging signals.

**Controls:**
| Control       | What it does                                        |
|---------------|-----------------------------------------------------|
| Volts/div     | Vertical scale — how many volts per grid square    |
| Time/div      | Horizontal scale — time per grid square            |
| Trigger level | Voltage threshold that starts the waveform capture |
| Trigger slope | Rising or falling edge                             |
| Coupling      | DC: shows all / AC: removes DC offset / GND: shows ground reference |
| Probe ×1/×10  | ×10 probe attenuates signal 10× for high voltage   |

**What to look for:**
- Correct voltage levels.
- Signal timing and frequency.
- Noise and glitches.
- Ringing after edges (impedance mismatch or poor decoupling).
- Correct I2C/SPI/UART framing.

**Cheap DSO options for hobbyists:**
- DS1054Z (Rigol, 4-channel, 50 MHz) — popular first scope.
- Hantek 6022BE (USB scope, 20 MHz) — very budget.
- DSO138 kit (DIY, 200 kHz) — learning tool only.

---

### Logic Analyser

Like an oscilloscope but samples many digital channels simultaneously and decodes protocols.

```
8–16 channel logic analyser → clip to I2C / SPI / UART lines
Software (Sigrok / PulseView) decodes the frames automatically
Shows you: exact timing, bit values, byte values, decoded messages
```

**Cheap options:**
- Saleae Logic clones (24 MHz, 8 channel, USB) — ~$10 on AliExpress.
- Bus Pirate — also a multi-protocol tool.
- Raspberry Pi Pico as logic analyser (PicoProbe firmware, Sigrok).

---

### Frequency Counter

Counts cycles per second — useful for measuring oscillator accuracy, PWM frequency, encoder pulses.

**MCU alternative:** Use `pulseIn()` in Arduino, or hardware timer input capture for higher accuracy.

---

## Chapter 9: Prototyping and PCB

### Breadboard

Solderless prototyping board. Holes connected in strips:
```
Power rails: long rows (+ and −) connected along full length.
Terminal strips: short rows (5 holes) connected across the middle gap.
The gap in the middle = DIP IC pin pitch (ICs straddle it).
```

**Breadboard limitations:**
- Stray capacitance and resistance — unreliable at > 1 MHz.
- Loose connections cause intermittent failures (most breadboard debugging is a loose wire).
- Not suitable for final products.

**Best practices:**
- Use short wires — long wires are antennas and noise sources.
- Power rail connections at both ends for long boards.
- Keep power and signal wires separate.

---

### Soldering

**Equipment:**
- Soldering iron: 25–60W adjustable. Temperature: 330–370°C for leaded, 350–400°C for lead-free.
- Solder: 60/40 Sn/Pb leaded (easiest for beginners), or lead-free SAC305 (harder but RoHS compliant).
- Flux: Helps solder flow and bond. Most solder has flux core. Additional flux for rework.
- Desoldering braid: copper wick for removing solder.
- Solder sucker (desoldering pump): removes bulk solder.

**Good solder joint:**
- Shiny, smooth, concave fillet.
- Solder wets both the pad and the lead.
- Takes < 3 seconds to form.

**Bad solder joint (cold joint):**
- Dull, grainy, lumpy appearance.
- Weak mechanical and electrical connection.
- Caused by too little heat, dirty surfaces, or moving the joint while cooling.

**SMD soldering:**
- Reflow: apply solder paste (solderpaste + flux), place components with tweezers, heat in oven or with hot air.
- Hand soldering SMD: tack one end, align, solder other end, go back to first.
- Drag soldering: for IC pins — flood with solder and drag iron across, then wick excess.

---

### PCB Design Basics

PCB (Printed Circuit Board) replaces breadboard wires with copper traces etched onto a board.

**Layers:** 2-layer boards (top + bottom copper) are standard for hobbyists. 4-layer adds internal power and ground planes.

**Key concepts:**
| Term         | Meaning                                                        |
|--------------|----------------------------------------------------------------|
| Trace        | Copper conductor connecting two pads                          |
| Pad          | Copper area where a component is soldered                     |
| Via          | A hole with copper plating connecting layers                  |
| Footprint    | The pad layout for a specific component package               |
| Silk screen  | Printed labels and outlines on the board surface              |
| Solder mask  | Green (or other colour) coating that protects copper, exposes pads |
| Ground plane | Solid copper fill connected to GND on a full layer — reduces noise, improves EMC |
| DRC          | Design Rule Check — software check for minimum trace widths, clearances |

**Trace width for current:**
```
Approximate rule of thumb:
  0.2 mm (0.008") → ~0.5 A
  0.5 mm (0.020") → ~1 A
  1.0 mm (0.040") → ~2 A
  2.5 mm (0.100") → ~4 A

Online calculators (Saturn PCB Toolkit) give exact values accounting for copper weight and temperature rise.
```

**PCB software:**
- KiCad (free, professional, most popular open source).
- EasyEDA (free, cloud-based, integrates with LCSC/JLCPCB ordering).
- Eagle (paid, used in many tutorials).
- Altium Designer (professional, expensive).

**PCB manufacturing (cheap options):**
- JLCPCB: 5 boards, 100×100 mm, 2-layer, 1.6 mm FR4 → ~$2 + shipping.
- PCBWay, OSHPark.

---

### Wire and Cable

| Type          | Current capacity | Use                                    |
|---------------|-----------------|----------------------------------------|
| 28 AWG        | 0.5 A           | Signal wires, dupont jumpers           |
| 26 AWG        | 1 A             | Light power                            |
| 22 AWG        | 3 A             | Moderate power, I2C/UART over distance |
| 20 AWG        | 5 A             | Motor leads on small drones            |
| 16 AWG        | 13 A            | Battery leads, high-current power      |
| 12 AWG        | 20 A            | Heavy power, ESC wires on large drones |

**AWG:** American Wire Gauge — lower number = thicker wire = more current.
**Twisted pair:** Twisting two wires together reduces magnetic field and makes them resistant to noise. Always twist CAN bus, differential signal pairs.

---

## Chapter 10: Programming Concepts for Embedded Systems

### State Machine

A way to organise code around distinct states and transitions between them:

```
States: IDLE, ARMED, FLYING, LANDING
Transitions: button_press → IDLE → ARMED
             takeoff_command → ARMED → FLYING
             land_command → FLYING → LANDING

Avoids complex nested if/else — much easier to debug and extend.
```

---

### Polling vs Interrupt-Driven

**Polling:** Main loop continuously checks if something happened.
```cpp
void loop() {
    if (digitalRead(button) == LOW) {  // check every loop iteration
        handleButton();
    }
}
// Problem: if loop is slow, you miss short events
```

**Interrupt-driven:** Hardware triggers your function instantly when the event happens.
```cpp
attachInterrupt(pin, handleButton, FALLING);  // fires immediately on button press
// Main loop is free to do other things
```

---

### Non-Blocking vs Blocking Code

**Blocking:** `delay(1000)` — stops everything for 1 second. Nothing else can run.

**Non-blocking:** `millis()` comparison — checks elapsed time without stopping anything.

**Rule:** Never use `delay()` in production firmware. Use millis() or FreeRTOS tasks.

---

### RTOS — Real-Time Operating System

Allows multiple tasks to run concurrently on the MCU (not true parallel — rapid time-sharing).

```
FreeRTOS (runs on ESP32, STM32, Arduino):
  Tasks run independently, each with its own stack.
  Scheduler switches between tasks based on priority and delays.
  Use: sensor reading task + communication task + display task running "simultaneously"

xTaskCreate(sensorTask, "Sensor", 2048, NULL, 1, NULL);
xTaskCreate(commsTask, "Comms", 4096, NULL, 2, NULL);
```

**Mutex (Mutual Exclusion):** Prevents two tasks from accessing the same resource simultaneously. Always use mutexes to protect shared variables between tasks.

---

### Fixed-Point vs Floating-Point

Some MCUs (Arduino Uno/ATmega328P) have no hardware floating-point unit (FPU) — `float` operations are done in software (slow, ~100× slower than integer).

**ESP32 and STM32F4+** have hardware FPU — `float` is fast.

**Fixed-point arithmetic:** Scale integers by a power of 2 to represent fractions:
```
Store temperature as integer in units of 0.01°C:
  2356 = 23.56°C
  Fast integer math, no float needed.
```

---

### Endianness

The order in which bytes are stored/transmitted for multi-byte values:
```
Value: 0x1234 (two bytes: 0x12 and 0x34)

Big-endian (network order, most protocols):  [0x12][0x34]  most significant first
Little-endian (x86, ARM, ESP32):             [0x34][0x12]  least significant first
```

**This matters when:** parsing binary sensor data, reading GPS binary (UBX), reading IMU registers, sending multi-byte values over UART/SPI/I2C.

```cpp
// Safe way to combine two bytes (endian-independent):
int16_t value = (int16_t)((highByte << 8) | lowByte);
```

---

## Chapter 11: Signals and Noise

### Analog vs Digital Signals

**Analog:** Continuous — can take any value between min and max. Real-world signals are analog (temperature, sound, light, voltage).

**Digital:** Discrete — only defined values. Binary: HIGH or LOW, 1 or 0.

**The interface between them:**
- World → ADC → MCU (analog to digital, for sensing).
- MCU → DAC → World (digital to analog, for output).
- MCU → PWM → Filter → World (PWM as pseudo-DAC).

---

### Noise

Unwanted random variation added to a signal.

| Type                | Source                              | Mitigation                          |
|---------------------|-------------------------------------|-------------------------------------|
| Thermal noise (Johnson) | Resistors, temperature          | Lower resistance, cool system       |
| Shot noise          | Current flowing across junctions    | Lower current, better components    |
| Flicker (1/f) noise | Transistor surfaces, low frequency | High-pass filter, JFET input stage  |
| Power supply noise  | Switching regulators, digital logic | Decoupling caps, LDO on analog rails|
| Electromagnetic (EMI)| Motors, RF, nearby digital signals | Shield, filter, separate grounds   |
| Quantisation noise  | ADC rounding                        | Higher bit depth, oversampling      |

**Oversampling and averaging:**
```
Read ADC 16 times, sum them, divide by 16.
This averages random noise → √16 = 4× noise reduction (6 dB).
Read N times → noise reduced by √N.
```

---

### Signal Integrity

How well a signal maintains its shape as it travels:

- **Ringing:** Oscillations after an edge — caused by inductance/capacitance mismatch. Add series resistor (~33Ω) on fast digital lines.
- **Crosstalk:** A signal on one trace induces a signal on an adjacent trace — keep traces separated, add ground traces between sensitive signals.
- **Ground bounce:** Multiple outputs switching simultaneously cause ground voltage to bounce — add decoupling caps, use spread output switching.
- **Transmission line effects:** At high frequencies (> ~30 MHz for typical PCB), traces behave as transmission lines — reflections occur if not terminated.

---

### Filtering in Software (Digital Filters)

**Moving average filter:**
```cpp
const int N = 10;
int readings[N];
int index = 0;
long total = 0;

int filteredRead() {
    total -= readings[index];
    readings[index] = analogRead(A0);
    total += readings[index];
    index = (index + 1) % N;
    return total / N;
}
// Smooth but adds N/2 samples of latency.
```

**Low-pass IIR (exponential moving average):**
```cpp
float filtered = 0;
float alpha = 0.1;  // 0 = no update, 1 = no filter

void loop() {
    float raw = analogRead(A0);
    filtered = alpha * raw + (1 - alpha) * filtered;
}
// Single line, very low memory. Alpha controls smoothing vs speed.
```

**Median filter:** Take N readings, sort, return middle value. Excellent for removing spikes/outliers. Higher CPU cost.

---

## Chapter 12: Wireless Communication Concepts

### Link Budget

How to predict if a wireless link will work:

```
Link budget = TX power + TX antenna gain − path loss − RX noise floor + RX antenna gain + margin

Example: nRF24L01 link
  TX power:         0 dBm (1 mW)
  TX antenna gain:  2 dBi (small PCB trace antenna)
  Path loss at 100m: −80 dB (free space at 2.4 GHz)
  RX noise floor:   −97 dBm (nRF24L01 spec)
  RX antenna gain:  2 dBi

  Link margin = 0 + 2 − 80 − (−97) + 2 = 21 dB

21 dB margin is good (> 10 dB = reliable link, > 20 dB = excellent).
```

### RSSI — Received Signal Strength Indicator

A value (usually in dBm or an arbitrary scale) indicating how strong the received signal is. Available from most wireless modules:

```
nRF24L01: reads register 0x09 after receive
ESP32 WiFi: WiFi.RSSI()
Bluetooth: available via HCI events

Typical values:
  −30 dBm: Excellent (very close to AP)
  −60 dBm: Good
  −75 dBm: Fair
  −85 dBm: Poor (approaching dropout)
  −95 dBm: Unusable
```

### Antenna Basics

- **Omnidirectional:** Radiates/receives equally in all horizontal directions — good for drones that can be in any position.
- **Directional (Yagi, patch):** Focuses gain in one direction — used for long-range fixed links.
- **dBi:** Antenna gain in dB relative to an isotropic (perfect sphere) radiator.
- **dBd:** Gain relative to a dipole (2.15 dBi better than dBd rating).

**Antenna placement rules:**
- Keep antenna away from metal objects (reflect/block signal).
- Keep antenna away from carbon fibre (absorbs RF).
- Keep antenna away from power wires and motors (EMI noise).
- For drones: mount antenna pointing down (radiation is strongest perpendicular to the antenna axis).

---

## Chapter 13: Sensors and Actuators

### Sensor Specifications

| Term              | Meaning                                                            |
|-------------------|--------------------------------------------------------------------|
| Range             | Min and max measurable value                                       |
| Resolution        | Smallest detectable change                                         |
| Accuracy          | How close to the true value (systematic error)                    |
| Precision         | Repeatability — how consistent repeated readings are              |
| Sensitivity       | Output change per unit of input change                            |
| Offset (bias)     | Fixed error — reading is always ±X from true value                |
| Drift             | Slow change in reading over time or temperature                   |
| Noise floor       | Minimum detectable signal above the sensor's own noise            |
| Bandwidth (-3dB)  | Fastest changing signal the sensor can accurately track           |
| Response time     | How long after a change until the reading settles                 |
| Hysteresis        | Difference in reading depending on whether value is increasing or decreasing |

**Accuracy vs precision (classic example):**
```
High accuracy, low precision: readings spread around the true value.
High precision, low accuracy: readings tightly clustered but consistently wrong (offset).
Both: tightly clustered around the true value. This is what you want.
```

### Calibration

Most sensors have an offset and scaling error. Calibration finds these and corrects them:

```
Measured = True × gain + offset

Two-point calibration:
  Measure at a known low value → record reading
  Measure at a known high value → record reading
  Fit a line through the two points → gain and offset

For magnetometer (compass):
  Hard iron calibration: figure-8 in the air, subtract the sphere offset
  Soft iron: scale the axes to a sphere (ellipsoid correction)
```

---

### Servo

A motor with a built-in feedback loop that holds a specific angular position:
```
Input: PWM signal (50 Hz, 1–2 ms pulse width)
Output: shaft holds a position (0°–180°)
Torque: typically 1–10 kg·cm
Power: 4.8–6V supply (not from GPIO pin — needs dedicated supply)
Current: 100–400 mA stall
```

**Continuous rotation servo:** Modified to rotate freely — pulse width controls speed and direction instead of position.

---

### Stepper Motor

Moves in precise discrete steps — no position feedback needed for moderate loads:
```
Step angle: typically 1.8° per step (200 steps per revolution)
Microstepping: driver divides each step (1/2, 1/4, 1/16, 1/32) for smoother motion
Torque at speed: drops significantly at high RPM (no back-EMF speed control)
Holding torque: maintains position when stopped (coils energised)
Use: 3D printers, CNC machines, camera sliders
```

---

### Brushless DC Motor (BLDC)

Three-phase motor — requires an ESC to commutate the phases electronically:
```
KV rating: RPM per volt. 1000KV motor at 12V → 12,000 RPM (no load).
T-number: stator winding count (e.g., 2306 = 23mm diameter, 6mm stator height).
Efficiency: 80–90% — much better than brushed motors.
Lifespan: very long — no brushes to wear out.
```

---

## Chapter 14: Common Problems and Debugging

### The Debugging Mindset

1. **Identify the symptom precisely.** "It doesn't work" is not a bug report. "The I2C sensor reads 0x00 on all registers" is.
2. **Form a hypothesis.** What could cause this specific symptom?
3. **Test the hypothesis.** Change one thing at a time.
4. **Binary search.** If 10 things could be wrong, test the midpoint — eliminate half.
5. **Trust nothing.** The "known good" cable might be broken. The voltage regulator might be wrong.

---

### Common Hardware Failures

| Symptom                          | Common Causes                                           |
|----------------------------------|---------------------------------------------------------|
| Nothing works                    | Incorrect power, reversed polarity, blown fuse, no GND |
| MCU resets randomly              | Insufficient decoupling, power dip under load, WDT     |
| I2C device not found             | Wrong address, missing pull-ups, bad wiring, 5V on 3.3V|
| SPI device not responding        | Wrong SPI mode, CS polarity, too fast clock, wrong MISO/MOSI |
| Analog reading wrong             | Floating input, loading effect, ADC reference issue    |
| Motor oscillates / doesn't spin  | ESC not calibrated, wrong protocol, power undervoltage |
| WiFi disconnects                 | Power supply noise, range, channel congestion          |
| Sensor reads constantly max/min  | Saturation, wrong reference voltage, reversed wires    |
| Intermittent failures            | Loose breadboard connection, cold solder joint         |
| GPS never gets a fix             | Indoors, antenna blocked, insufficient supply current  |
| Servo jitters                    | Power supply noise, insufficient current, PWM frequency|

---

### Checking the Power Supply First

80% of mysterious problems are power supply problems. Always check:
```
1. Correct voltage? (measure with multimeter)
2. Stable under load? (voltage sags when device loads it)
3. Sufficient current? (regulator rated for total load + margin)
4. Decoupling caps present? (especially near ICs)
5. Common ground? (all sub-circuits share same GND reference)
```

---

### Serial Debug Output

The most important debugging tool in embedded programming:

```cpp
void setup() {
    Serial.begin(115200);
    Serial.println("Boot");
}

void loop() {
    float temp = sensor.readTemperature();
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" C");
    delay(1000);
}
```

**Serial.print() is slow** — at 115200 baud, printing 100 characters takes ~8.7 ms. In time-critical code (fast control loops), remove or reduce serial output.

---

## Chapter 15: Safety

### Electrical Safety

- **Mains voltage (110V/230V AC) is lethal.** Never work on mains-connected circuits unless you fully understand what you are doing. Treat all mains circuits as live even when switched off (capacitors retain charge).
- **High-voltage capacitors** (in power supplies, camera flash units) can remain charged for minutes after power is removed — discharge before touching.
- **Lithium batteries** — fire hazard if punctured, shorted, or overcharged. Always use a proper LiPo charger. Never charge below 0°C.
- **Laser** — never point at eyes, even at low power. Diodes can be invisible (IR) but still cause permanent eye damage.
- **RF power** — at close range, high-power RF (VTX > 1W) can cause tissue heating. Keep away from body when transmitting.

### Fuses

Every powered circuit should have a fuse — a sacrificial link that opens (blows) before a fault can cause fire or damage.

```
Fuse selection: rated just above normal operating current.
Example: 1A circuit → use 1.5A or 2A fuse.
Self-resetting PTC fuses (polyfuses): recovers after cooling down — good for prototypes.
```

### Heat Management

```
Power dissipated as heat:
  P = I² × R (resistors, traces)
  P = Vce × Ic (transistors, linear regulators)
  P = (Vin − Vout) × Iout (linear regulators)

Above ~0.5W: consider a heatsink or spreading the heat over a larger area.
Above ~2W on a small IC: you need a heatsink or forced air cooling.
Thermal paste improves heat transfer between component and heatsink.
Maximum junction temperature: always check Tj(max) on datasheet (typically 125–150°C).
```

---

## Chapter 16: Acronyms and Abbreviations — Complete List

| Acronym  | Meaning                                      | Where used               |
|----------|----------------------------------------------|--------------------------|
| AC       | Alternating Current                          | Mains power, signals     |
| ADC      | Analog to Digital Converter                  | MCU, sensors             |
| AFH      | Adaptive Frequency Hopping                   | Bluetooth, RC            |
| AFSK     | Audio Frequency Shift Keying                 | Modem, APRS              |
| AGC      | Automatic Gain Control                       | Receivers, radios        |
| AGL      | Above Ground Level                           | Drone altitude           |
| AHI      | Artificial Horizon Indicator                 | FC OSD display           |
| AMC      | Adaptive Modulation and Coding               | WiFi, LTE                |
| APRS     | Automatic Packet Reporting System            | Ham radio, tracking      |
| ARM      | Advanced RISC Machine                        | MCU architecture         |
| ATC      | Air Traffic Control                          | Aviation                 |
| ATX      | Advanced Technology Extended                 | PC power supply standard |
| AUX      | Auxiliary                                    | FC input channels        |
| AVR      | Alf and Vegard's RISC processor              | Arduino/ATmega MCUs      |
| AWG      | American Wire Gauge                          | Wire sizing              |
| BEC      | Battery Eliminator Circuit                   | ESC 5V output for FC/RX  |
| BER      | Bit Error Rate                               | Communications           |
| BJT      | Bipolar Junction Transistor                  | Electronics              |
| BLE      | Bluetooth Low Energy                         | Wireless                 |
| BMS      | Battery Management System                    | LiPo/Li-Ion protection   |
| BOM      | Bill of Materials                            | PCB/design               |
| BPSK     | Binary Phase Shift Keying                    | GPS, satellite           |
| CAD      | Computer-Aided Design                        | PCB design               |
| CDMA     | Code Division Multiple Access                | GPS, 3G mobile           |
| CLI      | Command Line Interface                       | Software tools           |
| CMOS     | Complementary Metal-Oxide Semiconductor      | Digital ICs, image sensors |
| CRC      | Cyclic Redundancy Check                      | Data integrity           |
| CSI      | Camera Serial Interface                      | Raspberry Pi camera      |
| CW       | Continuous Wave                              | Radar, Morse code        |
| DAC      | Digital to Analog Converter                  | Audio, analog output     |
| DC       | Direct Current                               | Batteries, power         |
| DCM      | Direction Cosine Matrix                      | Flight controller math   |
| DIP      | Dual Inline Package                          | Through-hole IC package  |
| DMA      | Direct Memory Access                         | MCU peripheral           |
| DMM      | Digital Multimeter                           | Test equipment           |
| DOP      | Dilution of Precision                        | GPS quality              |
| DSP      | Digital Signal Processor                     | Audio, radar, comms      |
| DSSS     | Direct Sequence Spread Spectrum              | GPS, WiFi 802.11b        |
| DVR      | Digital Video Recorder                       | FPV recording            |
| DVM      | Digital Voltmeter                            | Test equipment           |
| ECC      | Error Correcting Code                        | Memory, comms            |
| ECEF     | Earth-Centred Earth-Fixed                    | GPS coordinate system    |
| ECM      | Electronic Countermeasures                   | Military EW              |
| ECCM     | Electronic Counter-Countermeasures           | Military EW              |
| EMC      | Electromagnetic Compatibility                | PCB design, compliance   |
| EMF      | Electromotive Force                          | Voltage source (motor BEMF) |
| EMI      | Electromagnetic Interference                 | PCB design, compliance   |
| ESC      | Electronic Speed Controller                  | Drone motor control      |
| ESD      | Electrostatic Discharge                      | Component handling       |
| FC       | Flight Controller                            | Drone                    |
| FEC      | Forward Error Correction                     | Communications           |
| FET      | Field Effect Transistor                      | Electronics              |
| FFT      | Fast Fourier Transform                       | DSP, radar               |
| FHSS     | Frequency Hopping Spread Spectrum            | RC, Bluetooth, military  |
| FPV      | First Person View                            | Drone video              |
| GCS      | Ground Control Station                       | UAV systems              |
| GNSS     | Global Navigation Satellite System          | GPS/GLONASS/Galileo/BeiDou|
| GPIO     | General Purpose Input/Output                 | MCU pins                 |
| GPS      | Global Positioning System                    | Navigation               |
| GND      | Ground                                       | Circuit reference        |
| HF       | High Frequency (3–30 MHz)                   | Radio                    |
| HID      | Human Interface Device                       | USB keyboard/mouse       |
| HV       | High Voltage                                 | LiPo high-voltage (4.35V)|
| I2C      | Inter-Integrated Circuit                     | MCU protocol             |
| IC       | Integrated Circuit                           | Electronics              |
| IMU      | Inertial Measurement Unit                    | Drone, robotics          |
| INS      | Inertial Navigation System                   | Aviation, military       |
| IoT      | Internet of Things                           | Wireless devices         |
| ISM      | Industrial Scientific Medical (RF band)     | WiFi, Bluetooth          |
| ISP      | Image Signal Processor / In-System Programmer | Camera / flashing MCU  |
| ISR      | Interrupt Service Routine                    | MCU programming          |
| KV       | Motor velocity constant (RPM/V)              | BLDC motors              |
| LCD      | Liquid Crystal Display                       | Displays                 |
| LDO      | Low Drop-Out (voltage regulator)            | Power supplies           |
| LED      | Light Emitting Diode                         | Indicators, displays     |
| LDPC     | Low Density Parity Check                     | FEC coding               |
| LiPo     | Lithium Polymer battery                      | Drone power              |
| LNA      | Low-Noise Amplifier                          | Radio receivers          |
| LO       | Local Oscillator                             | Radio receivers          |
| LoRa     | Long Range (CSS radio)                       | IoT, drone telemetry     |
| LSB      | Least Significant Bit                        | Digital data             |
| LUX      | Unit of illuminance                          | Light sensors            |
| MAH      | Milliamp-Hour                                | Battery capacity         |
| MCU      | Microcontroller Unit                         | Embedded systems         |
| MEMS     | Micro-Electromechanical Systems              | IMUs, pressure sensors   |
| MIMO     | Multiple Input Multiple Output               | WiFi, 5G antennas        |
| MOS      | Metal-Oxide Semiconductor                    | MOSFET transistors       |
| MOSFET   | Metal Oxide Semiconductor FET               | Power switching          |
| MSB      | Most Significant Bit                         | Digital data             |
| MSC      | Mass Storage Class                           | USB storage devices      |
| NMEA     | Nat. Marine Electronics Assn. (GPS format)  | GPS data protocol        |
| NVS      | Non-Volatile Storage                         | ESP32 settings           |
| OSD      | On-Screen Display                            | FPV video overlay        |
| OTA      | Over-The-Air (update)                        | Wireless firmware update |
| PA       | Power Amplifier                              | Radio transmitters       |
| PCB      | Printed Circuit Board                        | Electronics              |
| PID      | Proportional-Integral-Derivative             | Control loop (FC tuning) |
| PLL      | Phase-Locked Loop                            | Clock synthesis, radios  |
| PMW      | See PWM (common typo)                        | —                        |
| POI      | Point of Interest                            | Camera gimbal target     |
| PSRAM    | Pseudo-Static RAM                            | ESP32-WROVER extra RAM   |
| PWM      | Pulse Width Modulation                       | Motor, servo, LED control|
| QAM      | Quadrature Amplitude Modulation              | WiFi, LTE, cable         |
| QPSK     | Quadrature PSK                               | Satellite, LTE           |
| RAM      | Random Access Memory                         | MCU variable storage     |
| RC       | Radio Control / Resistor-Capacitor (context)| Drones / filters         |
| RF       | Radio Frequency                              | Wireless comms           |
| RMS      | Root Mean Square                             | AC voltage/current       |
| RSSI     | Received Signal Strength Indicator           | WiFi, BT, RF modules     |
| RTOS     | Real-Time Operating System                   | Embedded firmware        |
| RTK      | Real-Time Kinematic                          | Precise GPS              |
| Rx / RX  | Receiver                                     | Radio, UART              |
| SAR      | Synthetic Aperture Radar / Successive Approx| Radar / ADC type         |
| SDR      | Software Defined Radio                       | RTL-SDR, HackRF          |
| SMD      | Surface Mount Device                         | PCB components           |
| SMT      | Surface Mount Technology                     | PCB assembly             |
| SNR      | Signal-to-Noise Ratio                        | Signal quality           |
| SOC      | System on Chip                               | ESP32, Raspberry Pi      |
| SoH      | State of Health                              | Battery condition        |
| SPI      | Serial Peripheral Interface                  | MCU protocol             |
| SRAM     | Static RAM                                   | MCU internal memory      |
| UART     | Universal Async Receiver/Transmitter         | MCU serial protocol      |
| UHF      | Ultra High Frequency (300 MHz–3 GHz)        | Radio, drone RC          |
| UPS      | Uninterruptible Power Supply                 | Power backup             |
| USB      | Universal Serial Bus                         | PC connectivity          |
| VCC      | Voltage Common Collector (supply pin)        | Circuit power            |
| VDD      | Voltage Drain-Drain (supply pin)             | Circuit power            |
| VHF      | Very High Frequency (30–300 MHz)            | Radio, aviation          |
| VREF     | Voltage Reference                            | ADC reference            |
| VTX      | Video Transmitter                            | FPV drone                |
| WDT      | Watchdog Timer                               | MCU reliability          |
| WiFi     | Wireless Fidelity (IEEE 802.11)              | Wireless LAN             |
| Tx / TX  | Transmitter                                  | Radio, UART              |
| XOR      | Exclusive OR (logic gate)                    | DSSS spreading           |

---

## Chapter 17: Quick Reference Card

### Must-Know Formulas

```
Ohm's Law:              V = I × R
Power:                  P = V × I = I² × R = V² / R
Voltage divider:        Vout = Vin × (R2 / (R1 + R2))
LED resistor:           R = (Vsupply − Vf) / Iled
RC time constant:       τ = R × C  (fully charged at 5τ)
RC cutoff frequency:    fc = 1 / (2π × R × C)
Resonant frequency:     f = 1 / (2π × √(L × C))
Series resistors:       Rt = R1 + R2 + R3
Parallel resistors:     1/Rt = 1/R1 + 1/R2 + 1/R3
Capacitors in parallel: Ct = C1 + C2 + C3
Capacitors in series:   1/Ct = 1/C1 + 1/C2 + 1/C3
Charge:                 Q = C × V
Energy in capacitor:    E = ½ × C × V²
Energy in inductor:     E = ½ × L × I²
Frequency:              f = 1 / T
Period:                 T = 1 / f
PWM average voltage:    Vavg = Duty% × Vcc
ADC voltage:            V = (reading / (2ⁿ − 1)) × Vref
dB power ratio:         dB = 10 × log10(P2/P1)
dB voltage ratio:       dB = 20 × log10(V2/V1)
Shannon capacity:       C = B × log2(1 + SNR)
```

### Common Component Values

```
Pull-up/pull-down:      1kΩ – 100kΩ (I2C: 4.7kΩ, GPIO: 10kΩ)
LED current limiting:   150Ω – 470Ω (for 5V), 68Ω – 150Ω (for 3.3V)
Decoupling caps:        100 nF ceramic per IC supply pin, 10 μF bulk nearby
Base resistor (BJT):    1kΩ – 10kΩ (calculate: (Vin − 0.7) / (Ic/β/10))
Gate pull-down (MOSFET): 10kΩ from Gate to GND
Servo PWM:              50 Hz, 1 ms–2 ms pulse width
ESC calibration:        2 ms = full, 1 ms = zero (arm at < 1.1 ms)
UART standard:          8N1 @ 9600, 115200, or 230400 baud
I2C standard speed:     100 kHz with 4.7kΩ pull-ups
SPI max speed:          10–40 MHz (check device datasheet)
```

### Logic Level Reference

```
System  Supply  Logic HIGH  Logic LOW   Can connect to 5V?
ESP32   3.3V    > 2.0V      < 0.8V      No — use level shifter
Arduino 5.0V    > 3.0V      < 1.5V      Yes natively
RPi     3.3V    > 2.0V      < 0.8V      No — use level shifter
STM32   3.3V    > 2.0V      < 0.8V      Depends on pin (check FT flag in datasheet)
```
