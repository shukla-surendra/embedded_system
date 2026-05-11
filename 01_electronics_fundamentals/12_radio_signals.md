# Radio Signals — Generation, Transmission, and Reception

---

## What Is a Radio Signal?

A radio signal is an **electromagnetic wave** — a wave of combined electric and magnetic energy that travels through space at the speed of light (~300,000 km/s).

You cannot see it, but it behaves just like a water ripple:
- It has a **frequency** (how many waves per second — measured in Hz)
- It has a **wavelength** (physical distance between two peaks)
- It has **amplitude** (the strength/height of the wave)

### Frequency vs Wavelength
```
frequency × wavelength = speed of light (300,000,000 m/s)

Example:
  2.4 GHz signal → wavelength = 300,000,000 / 2,400,000,000 = 0.125 m = 12.5 cm
```
Higher frequency = shorter wavelength = more data capacity but shorter range.

---

## The Radio Frequency Spectrum

| Band        | Frequency Range  | Used For                                |
|-------------|------------------|-----------------------------------------|
| AM Radio    | 530 kHz – 1.7 MHz | Broadcast radio                        |
| FM Radio    | 87.5 – 108 MHz   | Broadcast radio                         |
| VHF         | 30 – 300 MHz     | TV, aviation, walkie-talkies            |
| UHF         | 300 MHz – 3 GHz  | WiFi, Bluetooth, drone control, LTE     |
| 2.4 GHz     | 2.4 – 2.485 GHz  | WiFi, Bluetooth, RC drones (your kit)   |
| 5.8 GHz     | 5.725 – 5.875 GHz| WiFi 5GHz, FPV drone video              |
| SHF / mmWave| 3 – 300 GHz      | Radar, 5G, satellite                    |

Your drone remote uses **2.4 GHz** — this is the UHF/SHF band, chosen because it gives a good balance of range, data rate, and antenna size.

---

## Part 1: How a Radio Signal Is Generated (Transmitter)

A transmitter has four main jobs:
1. Generate a stable high-frequency wave (the **carrier**)
2. Encode information onto it (called **modulation**)
3. Amplify the signal to a useful power level
4. Radiate it into space via an **antenna**

### Step 1 — Oscillator (Carrier Generation)

An oscillator is a circuit that produces a repeating AC signal at a precise frequency.

**How it works:**
- A feedback loop makes a transistor or op-amp continuously switch on and off.
- An **LC tank circuit** (inductor + capacitor) or a **crystal** sets the exact frequency.
- Crystal oscillators are far more stable than LC circuits.

```
LC Tank Circuit (resonant frequency):

        f = 1 / (2π × √(L × C))

  L = inductance (henries)
  C = capacitance (farads)
  f = resonant frequency (Hz)

A capacitor stores electric energy, an inductor stores magnetic energy.
They swap energy back and forth at the resonant frequency — like a pendulum.
```

**Crystal Oscillator:**
- A quartz crystal vibrates mechanically at an exact frequency when voltage is applied (piezoelectric effect).
- Very accurate — used in all modern radios, GPS, and microcontrollers.

---

### Step 2 — Modulation (Encoding Information)

The carrier wave by itself carries no information. Modulation means changing a property of the carrier wave in sync with your data/audio signal.

#### AM — Amplitude Modulation
- The **height (amplitude)** of the carrier wave is varied.
- Simple to implement, but noise easily corrupts amplitude — poor quality.

```
Carrier:   ~~~~~~~~~~~~~~~~~~~~
Signal:    _/‾\_
AM output: __/‾‾‾‾\__/‾‾‾‾\__   (envelope changes with signal)
```

#### FM — Frequency Modulation
- The **frequency** of the carrier wave is varied slightly.
- Noise doesn't easily change frequency — much better quality than AM.
- Used in: FM radio, WiFi, Bluetooth, RC drone remotes.

```
Carrier:   ~~~~~~~~~~~~~~~~~~~~
Signal:    _/‾\_
FM output: ~~~~||||||~~~~||||||   (denser waves = higher frequency = high signal)
```

#### Digital Modulation (used in your drone remote)
Modern systems send binary data (0s and 1s) using digital modulation:

| Type  | Full Name                          | How It Works                              |
|-------|------------------------------------|-------------------------------------------|
| OOK   | On-Off Keying                      | Signal ON = 1, Signal OFF = 0             |
| FSK   | Frequency Shift Keying             | Two slightly different frequencies = 0/1 |
| GFSK  | Gaussian FSK                       | Smoothed FSK — used in Bluetooth          |
| DSSS  | Direct Sequence Spread Spectrum    | Data spread across wide band — used in WiFi, GPS |
| FHSS  | Frequency Hopping Spread Spectrum  | Hops between frequencies 50–100x/sec — used in your 2.4GHz RC remote |

**FHSS is why your drone remote is resistant to interference** — it constantly hops frequencies, so brief interference on one frequency only corrupts a tiny slice of data.

---

### Step 3 — RF Amplifier (Power Amplification)

After modulation, the signal is still very weak (milliwatts). A **power amplifier (PA)** boosts it to the transmit power level (e.g., 20 mW for WiFi, 1 W for RC remote).

Components used:
- **RF MOSFETs** or **RF BJT transistors** — specially designed for high-frequency switching
- These are the same transistors from the amplifier doc, but optimized for GHz frequencies

Output power is measured in **dBm**:
```
0 dBm  = 1 mW
10 dBm = 10 mW
20 dBm = 100 mW
30 dBm = 1000 mW (1 W)
```

---

### Step 4 — Antenna (Radiation)

An antenna converts electrical signals into electromagnetic waves.

**How it works:**
- Alternating current flowing in a wire creates a changing electric field.
- A changing electric field creates a changing magnetic field.
- These two fields reinforce each other and propagate outward as an electromagnetic wave.

**Half-wave dipole antenna:**
- Most common type.
- Optimal length = half the wavelength of the signal.
```
  2.4 GHz → wavelength = 12.5 cm → dipole length = 6.25 cm
```
This is why antennas on WiFi routers and RC remotes are roughly 6 cm long.

**Gain:** Antennas don't amplify power — they shape (focus) the radiation pattern.
- Omnidirectional: radiates equally in all directions (like a light bulb)
- Directional (Yagi, patch): focuses power in one direction (like a flashlight)

---

## Part 2: How a Radio Signal Is Received (Receiver)

A receiver has four main jobs:
1. Capture the electromagnetic wave with an antenna
2. Select only the desired frequency (tuning/filtering)
3. Amplify the extremely weak received signal
4. Demodulate — extract the original information

### Step 1 — Antenna (Capture)

The receive antenna works in reverse:
- Incoming electromagnetic waves induce a tiny alternating current in the antenna wire.
- This current is measured in **microvolts** — extremely weak.

---

### Step 2 — LNA (Low-Noise Amplifier)

The first thing the receiver does is amplify the signal — but amplification also amplifies noise.

A **Low-Noise Amplifier (LNA)** is a special transistor circuit designed to:
- Boost the weak antenna signal (typically 10–20 dB gain)
- Add as little noise as possible (measured as **Noise Figure** in dB)

This is the most critical component in a receiver — a poor LNA ruins everything downstream.

Components: RF transistors (BJT or MOSFET) with very carefully controlled biasing.

---

### Step 3 — Mixer + Local Oscillator (Frequency Down-Conversion)

A 2.4 GHz signal is too fast to process directly. The receiver shifts it down to a lower frequency called the **Intermediate Frequency (IF)**.

**How mixing works:**
- A **local oscillator (LO)** generates a frequency close to the incoming signal.
- A **mixer** multiplies the two signals together.
- The output contains two new frequencies: `(signal + LO)` and `(signal − LO)`.
- A filter keeps only the difference — the IF.

```
Example:
  Received signal:   2,410 MHz
  Local oscillator:  2,400 MHz
  IF output:         10 MHz  ← much easier to process
```

This technique is called **superheterodyne** and is used in virtually every radio receiver built since the 1920s.

---

### Step 4 — IF Filter (Selectivity)

A **bandpass filter** centred on the IF frequency removes all other signals and noise.

This is what gives a radio its **selectivity** — its ability to pick one station/channel out of thousands without interference.

Components used:
- **Ceramic filters** (cheap, moderate performance)
- **SAW filters** (Surface Acoustic Wave) — very sharp, used in phones and RC modules
- **LC filters** — inductors and capacitors tuned to the IF frequency

---

### Step 5 — IF Amplifier

Amplifies the filtered IF signal to a level useful for demodulation.
Uses standard transistor amplifier stages (see `11_amplifiers.md`).

---

### Step 6 — Demodulator (Recover the Data)

The demodulator reverses whatever modulation was applied at the transmitter.

| Modulation | Demodulator Used             |
|------------|------------------------------|
| AM         | Envelope detector (diode + RC filter) |
| FM         | Discriminator or PLL circuit |
| FSK        | Frequency discriminator       |
| DSSS/FHSS  | Correlator / matched filter + despreader |

For digital modulation (your drone), a microcontroller ADC samples the IF signal and a **DSP algorithm** decodes the bits.

---

## Complete Signal Chain Diagram

```
TRANSMITTER
───────────────────────────────────────────────────────
  Joystick/button
       │
  Microcontroller (ESP32 / nRF24)
       │  digital data (bits)
  Modulator   ← Local Oscillator (crystal)
       │  modulated signal at IF
  Up-Converter (Mixer + LO)
       │  signal at 2.4 GHz
  RF Power Amplifier (MOSFET)
       │  amplified RF
  Antenna )))))) → electromagnetic wave → space

RECEIVER
───────────────────────────────────────────────────────
  Antenna )))))) ← electromagnetic wave from space
       │  microvolts of induced current
  LNA (Low-Noise Amplifier)
       │  amplified but still at 2.4 GHz
  Mixer + Local Oscillator
       │  down-converted to IF (e.g. 10 MHz)
  IF Filter (SAW / ceramic)
       │  clean, filtered IF signal
  IF Amplifier
       │
  Demodulator
       │  raw bits
  Microcontroller (ESP32 / flight controller)
       │
  Motor commands / control output
```

---

## Electronic Components Used in Radio Circuits

### Oscillators and Frequency Control
| Component       | Role                                             |
|-----------------|--------------------------------------------------|
| Crystal (XTAL)  | Sets precise transmit/receive frequency          |
| LC Tank circuit | Tunable oscillator (inductor + capacitor)        |
| PLL IC          | Phase-Locked Loop — synthesises any frequency from a reference crystal |
| VCO             | Voltage-Controlled Oscillator — used inside PLLs |

### Amplifiers
| Component       | Role                                             |
|-----------------|--------------------------------------------------|
| RF BJT          | Low-power amplifier stages at high frequency     |
| RF MOSFET       | Power amplifier stages (final output)            |
| LNA IC          | Dedicated low-noise amplifier chip (e.g. SPF5189)|

### Filters
| Component       | Role                                             |
|-----------------|--------------------------------------------------|
| SAW Filter      | Sharp bandpass filter — rejects interference     |
| Ceramic filter  | Cheaper IF filter                                |
| LC filter       | Inductor-capacitor bandpass/lowpass filter       |

### Mixing and Detection
| Component       | Role                                             |
|-----------------|--------------------------------------------------|
| Diode (mixer)   | Schottky diodes used as non-linear mixing element|
| Mixer IC        | Dedicated frequency-mixing chip                  |
| Envelope detector | Diode + RC: recovers AM signal               |

### Passive Supporting Components
| Component       | Role                                             |
|-----------------|--------------------------------------------------|
| Capacitor       | Coupling, bypass, tuning, filtering              |
| Inductor        | Tuning, filtering, impedance matching            |
| Resistor        | Biasing transistors, setting gain, termination   |
| Balun           | Balanced-to-unbalanced converter at antenna port |

### Integrated Radio Modules (what your drone kit likely uses)
| Module          | What's inside                                    |
|-----------------|--------------------------------------------------|
| nRF24L01+       | Complete 2.4 GHz transceiver IC (oscillator, modulator, PA, LNA, demodulator) |
| CC2500          | 2.4 GHz ISM transceiver — used in FlySky/Frsky remotes |
| ESP32 (built-in)| 2.4 GHz WiFi + Bluetooth radio fully integrated on chip |
| SX1278 (LoRa)   | 433/915 MHz long-range radio module              |

Modern radio modules pack the entire signal chain (oscillator → modulator → PA → LNA → demodulator) into a single chip. Your ESP32 has this built in for WiFi and Bluetooth.

---

## Key Terms Glossary

| Term              | Meaning                                                                |
|-------------------|------------------------------------------------------------------------|
| Carrier wave      | The high-frequency wave that "carries" the information                 |
| Modulation        | Changing the carrier to encode data                                    |
| Demodulation      | Recovering the original data from the modulated carrier                |
| Frequency (Hz)    | Number of wave cycles per second                                       |
| Bandwidth         | Range of frequencies used — determines data rate                       |
| Gain (dB)         | How much a signal is amplified (decibels)                              |
| Noise Figure (dB) | How much noise an amplifier adds — lower is better                     |
| SNR               | Signal-to-Noise Ratio — how much louder the signal is than the noise   |
| RSSI              | Received Signal Strength Indicator — how strong the received signal is |
| IF                | Intermediate Frequency — the down-converted working frequency          |
| LNA               | Low-Noise Amplifier — first amplifier after the antenna                |
| PLL               | Phase-Locked Loop — generates stable frequencies                       |
| FHSS              | Frequency Hopping Spread Spectrum — hops frequencies to avoid interference |
| Impedance (Ω)     | Resistance to AC signals — antenna and cable must be matched (usually 50Ω) |

---

## Relevance to Your Drone Project

- Your **2.4 GHz RC remote** uses FHSS to hop across 2.4 GHz channels — this is why it doesn't interfere with WiFi even though they share the same band.
- The **nRF24L01+** or equivalent module on your receiver board contains the full radio chain in one chip.
- **ESP32's built-in radio** can be used for drone telemetry (sending back battery voltage, altitude) using WiFi or ESP-NOW protocol.
- **Antenna orientation matters** — a dipole antenna radiates a donut-shaped pattern. If your drone's antenna points straight at the remote, signal drops — this is called **antenna null**.
- **RSSI** is a value you can read in code — when it drops below a threshold, trigger a failsafe (return to home or land).
