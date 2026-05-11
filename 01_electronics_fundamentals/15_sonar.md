# Sonar — How It Works, Types, and Electronics

SONAR = **SO**und **N**avigation **A**nd **R**anging

Sonar uses sound waves to detect objects, measure distances, and map environments — especially underwater where radio waves are useless. The same principle scaled down is used in ultrasonic sensors on drones, robots, and cars.

---

## Why Sound Instead of Radio Underwater?

Radio waves (EM waves) travel at 3 × 10⁸ m/s — but they are absorbed almost immediately in seawater (within centimetres to metres depending on frequency). Seawater is electrically conductive, and conductors absorb EM energy.

Sound waves (pressure waves) travel very well through water:
- Speed of sound in air: ~343 m/s
- Speed of sound in water: ~1480 m/s (about 4× faster)
- Speed of sound in steel: ~5000 m/s

Water is dense and incompressible — ideal for transmitting pressure waves over long distances with low loss. A naval sonar can detect a submarine hundreds of kilometres away.

---

## The Core Principle

Sonar works exactly like radar but with sound:

> Emit a pulse of sound. If it hits an object, some energy reflects back. Measure the round-trip time. Distance = (speed of sound × time) / 2.

```
Transducer → )))))))))) → Target (submarine, fish, sea floor)
                              │
Transducer ← ((((((((( ← reflected echo

Distance = (v × t) / 2
  v = speed of sound in medium (m/s)
  t = round-trip travel time (seconds)
```

**Example (underwater):**
```
Echo returns after 2 seconds
Speed of sound in water = 1480 m/s
Distance = (1480 × 2) / 2 = 1480 m = 1.48 km
```

**Example (ultrasonic sensor on drone):**
```
Echo returns after 600 μs (600 × 10⁻⁶ s)
Speed of sound in air = 343 m/s
Distance = (343 × 600×10⁻⁶) / 2 = 0.103 m = 10.3 cm
```

---

## Sound Wave Fundamentals

Sound is a **mechanical wave** — a pressure disturbance that travels through a medium by compressing and rarefying the material. Unlike EM waves, sound **cannot travel through a vacuum**.

### Longitudinal Wave Structure

Sound is a **longitudinal wave** — the particles oscillate in the same direction as the wave travels (unlike EM waves which are transverse).

```
Direction of travel →

Compression:    |||  |||  |||  (particles pushed together)
Rarefaction:       '    '    ' (particles pulled apart)

→ → → → direction of travel → → → →
```

### Key Properties

| Property     | Symbol | Unit         | Meaning                                          |
|--------------|--------|--------------|--------------------------------------------------|
| Frequency    | f      | Hz           | Cycles per second — determines pitch             |
| Wavelength   | λ      | metres       | Distance between compressions                    |
| Speed        | v      | m/s          | Depends on medium (density and elasticity)        |
| Amplitude    | A      | Pa (Pascals) | Pressure variation — determines loudness         |
| Period       | T      | seconds      | Time for one complete cycle (T = 1/f)            |

**The fundamental relationship:**
```
v = f × λ

Example (ultrasonic in air at 40 kHz):
  λ = 343 / 40,000 = 0.00858 m = 8.6 mm
```

### Frequency Ranges

| Range        | Frequency     | Human hearing | Used in                                |
|--------------|---------------|---------------|----------------------------------------|
| Infrasound   | < 20 Hz       | Not heard     | Earthquake, whale communication, elephants |
| Audible      | 20 Hz – 20 kHz| Heard         | Speech, music                          |
| Ultrasound   | 20 kHz – 1 GHz| Not heard     | Sonar, medical imaging, cleaning       |
| Hypersound   | > 1 GHz       | Not heard     | Research, acoustic microscopy          |

Sonar and ultrasonic sensors use frequencies above human hearing (ultrasound) so they do not disturb operators and face less interference from ambient noise.

---

## Speed of Sound in Different Media

Speed depends on the medium's **density** and **elasticity** (stiffness). Stiffer and denser-but-compressible = faster.

| Medium              | Speed of Sound  | Notes                                      |
|---------------------|-----------------|---------------------------------------------|
| Air (20°C)          | 343 m/s         | Decreases with altitude (colder, thinner)  |
| Air (0°C)           | 331 m/s         | Temperature affects speed significantly    |
| Fresh water (20°C)  | 1481 m/s        | Much faster — water is nearly incompressible|
| Sea water (20°C)    | ~1520 m/s       | Salinity and pressure increase speed       |
| Sea water (deep)    | ~1480–1520 m/s  | Varies with depth, temp, salinity          |
| Soft tissue (body)  | ~1540 m/s       | Used in medical ultrasound calculations    |
| Bone                | ~3500 m/s       | Denser than tissue                         |
| Aluminium           | 6320 m/s        | Used in industrial ultrasonic testing      |
| Steel               | ~5000–6000 m/s  | NDT testing of welds and structures        |

### Why Speed of Sound Varies in the Ocean

In the ocean, sound speed is affected by three factors:
- **Temperature**: warmer water → faster sound (dominant near surface)
- **Salinity**: more salt → faster sound
- **Pressure (depth)**: deeper → higher pressure → faster sound (dominant in deep water)

This creates a **SOFAR channel** (Sound Fixing And Ranging) — a depth layer (~700–1200 m in most oceans) where sound speed is minimum. Sound bends toward the minimum speed layer and gets trapped — a natural waveguide. Whales use it for long-range communication. The US Navy used it during the Cold War to track submarines.

```
Depth vs Sound Speed profile:

  Surface (warm)  → fast
       ↓
  Thermocline     → slowing (temperature drops fast)
       ↓
  SOFAR channel   → slowest (minimum)  ← sound trapped here
       ↓
  Deep water      → increasing (pressure dominates)
```

---

## Part 1: How Sonar Transmits — The Transducer

The core component of any sonar system is the **transducer** — a device that converts electrical energy into sound energy and vice versa.

### Piezoelectric Effect

Most sonar transducers use the **piezoelectric effect**:

- **Direct piezoelectric effect**: mechanical stress on a crystal → generates a voltage.
- **Converse piezoelectric effect**: apply voltage to a crystal → it physically deforms (expands or contracts).

By rapidly alternating the voltage at ultrasonic frequencies, the crystal vibrates and pumps pressure waves into the surrounding medium — generating sound.

The same crystal works in reverse as a receiver — incoming pressure waves deform the crystal and generate a voltage that is measured.

```
Transmit mode:
  Electrical signal (AC voltage at f) → Crystal vibrates → Sound wave emitted

Receive mode:
  Sound wave arrives → Crystal deforms → Voltage generated → Measured
```

**Common piezoelectric materials:**
| Material   | Full Name                          | Properties                              |
|------------|------------------------------------|-----------------------------------------|
| PZT        | Lead Zirconate Titanate            | Most common — high sensitivity, high coupling |
| BaTiO₃     | Barium Titanate                    | Historical, still used in some sensors |
| PVDF       | Polyvinylidene Fluoride            | Flexible polymer — used in wideband sensors |
| Quartz     | SiO₂                               | Very stable, low loss — used in precision timing |
| PMN-PT     | Lead Magnesium Niobate-Lead Titanate | Very high coupling — medical ultrasound |

### Resonant Frequency of a Transducer

A piezoelectric transducer has a natural resonant frequency determined by the thickness of the crystal:

```
f = v / (2 × d)

  f = resonant frequency (Hz)
  v = speed of sound in the crystal material
  d = thickness of the crystal (metres)

Example: PZT crystal, v = 4000 m/s, d = 1 mm
  f = 4000 / (2 × 0.001) = 2,000,000 Hz = 2 MHz
```

Thinner crystal → higher frequency → shorter wavelength → higher resolution but less penetration.

### MEMS Ultrasonic Transducers (cMUT / pMUT)

Modern compact ultrasonic sensors use microfabricated transducers:
- **cMUT** (Capacitive Micromachined Ultrasonic Transducer): a tiny membrane that vibrates electrostatically.
- **pMUT** (Piezoelectric MEMS Ultrasonic Transducer): tiny piezoelectric membrane made using semiconductor fabrication.

Used in: smartphone face unlock (ultrasonic fingerprint sensors), medical probe-on-chip, compact drone sensors.

---

## Part 2: Complete Sonar Signal Chain

```
┌─────────────────────────────────────────────────────┐
│                   TRANSMIT PATH                     │
│                                                     │
│  MCU/DSP generates pulse timing                     │
│       │                                             │
│  Waveform generator (frequency, shape)              │
│       │                                             │
│  Power amplifier (drives transducer)                │
│       │                                             │
│  T/R switch or duplexer                             │
│       │                                             │
│  Transducer (electrical → acoustic)                 │
│       │                                             │
│  )))))))))) sound wave into medium ))))))))          │
└─────────────────────────────────────────────────────┘
                  hits target → reflects
┌─────────────────────────────────────────────────────┐
│                   RECEIVE PATH                      │
│                                                     │
│  Transducer (acoustic → electrical)                 │
│       │ (microvolts of signal)                      │
│  T/R switch                                         │
│       │                                             │
│  TGC Amplifier (Time-Gain Compensation)             │
│       │ (boosts gain as time increases to           │
│       │  compensate for signal loss with distance)  │
│  Bandpass filter (centre at transmit frequency)     │
│       │                                             │
│  ADC (digitises the echo signal)                    │
│       │                                             │
│  DSP / FPGA (range, Doppler, imaging processing)    │
│       │                                             │
│  Output: distance, speed, image, object map         │
└─────────────────────────────────────────────────────┘
```

### Time-Gain Compensation (TGC)
Sound loses energy as it travels — echoes from far away are much weaker than near ones. TGC automatically increases the receiver gain over time after each pulse so that near and far echoes are displayed at equal amplitude. Without TGC, distant targets would look falsely dim.

---

## Part 3: Types of Sonar

---

### 1. Active Sonar
Emits a sound pulse and listens for echoes. Measures range, bearing, and speed.

**Monostatic**: transmitter and receiver at the same location (same transducer or co-located).
**Bistatic**: transmitter and receiver at different locations — more complex but harder for submarines to detect (they only hear the receiver, not the transmitter).

**Advantages:** Can detect silent/passive targets, gives range and bearing.
**Disadvantages:** Reveals your own position (submarines generally avoid using active sonar).

---

### 2. Passive Sonar
Only listens — does not transmit anything. Detects sounds emitted by targets (engine noise, propeller cavitation, machinery).

**How it works:**
- Array of hydrophones (underwater microphones) continuously records sound.
- DSP performs spectral analysis to identify target signatures.
- Triangulation across multiple hydrophones gives bearing and rough position.
- No transmitted signal → you cannot be detected.

**Used in:** Submarine warfare (submarines prefer passive), SOSUS (Sound Surveillance System — underwater hydrophone arrays on the sea floor the US used to track Soviet submarines during the Cold War).

**Limitations:** Cannot measure range directly, requires the target to be making noise.

---

### 3. Scanning Sonar (Mechanical)
A narrow-beam sonar that physically rotates to build a 360° map of the surroundings.

**How it works:**
- Directional transducer rotates (like a radar antenna).
- At each angle, a pulse is fired and echo time is measured.
- Result: a polar plot showing distance to objects in all directions.

**Used in:** Ship hull-mounted navigation sonar, ROV (remotely operated vehicle) navigation, underwater obstacle avoidance.

---

### 4. Phased Array Sonar
Like phased array radar — an array of transducers that steers beams electronically without mechanical rotation.

**How it works:**
- Grid of transducer elements, each driven with slightly different timing.
- Phase delays cause constructive interference in one direction → steered beam.
- Can form multiple simultaneous beams and scan electronically at high speed.

**Used in:** Modern naval sonar, medical 3D ultrasound probes, SONAR arrays on submarines.

---

### 5. Side-Scan Sonar
Mounted on a ship or underwater vehicle, it emits fan-shaped beams to both sides — building a strip image of the sea floor as the vehicle moves.

**How it works:**
- Wide beam in the cross-track direction, narrow beam in the along-track direction.
- Near returns come back first (short range), far returns come back later.
- Intensity of echo plotted against time → shadow image of bottom features.
- As the vehicle moves forward, successive strips are assembled into an image.

**Used in:** Sea floor mapping, wreck discovery (MH370 search used side-scan sonar), pipeline inspection, harbour security.

---

### 6. Multibeam Sonar (Swath Bathymetry)
Simultaneously measures water depth across a wide swath below the ship.

**How it works:**
- Transmits a wide fan beam perpendicular to ship track.
- Receives on a narrow fore-aft beam (Mills Cross arrangement).
- Hundreds of simultaneous depth measurements across the swath.
- Each beam intersection gives an exact depth measurement.

**Used in:** Precise sea floor mapping, ocean charts, offshore engineering, detecting undersea volcanoes and ridges.

---

### 7. Doppler Velocity Log (DVL)
A sonar that measures the speed of an underwater vehicle over the sea floor using the Doppler shift.

**How it works:**
- 4 transducers tilted at different angles, pointing toward the sea floor.
- Each measures Doppler shift of the echo from the bottom.
- The 4 Doppler measurements are combined to compute velocity in X, Y, Z.
- Works even with no GPS (GPS signals don't penetrate water).

**Used in:** AUV (Autonomous Underwater Vehicle) navigation, ROV positioning, submarine navigation, underwater drones.

**Drone relevance:** A **Doppler radar** works on the same principle on aerial drones — measures ground-relative velocity by detecting Doppler shift of microwave echo from the ground.

---

### 8. Fishfinder / Echo Sounder
A simple downward-looking sonar that measures water depth and detects fish schools.

**How it works:**
- Single-beam transducer (or CHIRP — swept frequency).
- Pulse sent downward, echoes from fish swim bladders and sea floor.
- Time → depth, echo strength → fish density.
- Modern fishfinders use 2D and 3D beam scanning.

**Used in:** Fishing boats, depth sounders on sailboats, underwater mapping.

**CHIRP Fishfinder:** Uses a swept frequency pulse (like FMCW radar) for much better depth resolution than single-frequency pulsed sonar. Can resolve fish at 1 cm separation.

---

### 9. Medical Ultrasound
Uses high-frequency ultrasound (1–20 MHz) to image the interior of the human body.

**How it works:**
- Probe pressed against skin — gel eliminates air gap (air strongly reflects ultrasound, gel matches acoustic impedance).
- Pulses sent into body, echoes from boundaries between tissues (different acoustic impedance) are received.
- Time of echo → depth. Echo strength → tissue boundary.
- Hundreds of A-scan lines assembled into a 2D or 3D image.

**Why different frequencies:**
- Higher frequency (15–20 MHz): high resolution but shallow penetration — used for superficial structures, skin, eyes.
- Lower frequency (1–5 MHz): deeper penetration but lower resolution — heart, abdomen, obstetrics.

**Modes:**
| Mode    | Description                                                        |
|---------|--------------------------------------------------------------------|
| A-mode  | Amplitude vs depth — single line scan (used in ophthalmology)      |
| B-mode  | 2D image built from many A-scan lines (standard ultrasound)        |
| M-mode  | Single beam repeatedly sampled over time — shows motion (heart valve)|
| Doppler | Measures blood flow velocity using frequency shift                 |
| 3D/4D   | Multiple 2D sweeps reconstructed into 3D volume                    |

---

### 10. SODAR (Sonic Detection and Ranging)
Like sonar but in the atmosphere — uses sound to measure wind profiles in the lower atmosphere.

**How it works:**
- Points acoustic pulses upward.
- Turbulence and temperature gradients scatter sound back.
- Doppler shift → wind speed at altitude.
- Used to measure wind shear at airports and around wind farms.

---

### 11. Acoustic Doppler Current Profiler (ADCP)
Measures water current velocity at multiple depths simultaneously.

**How it works:**
- Multiple beams pointing in different directions and downward.
- Doppler shift of backscatter from suspended particles (plankton, sediment).
- Each depth layer's velocity is measured independently.

**Used in:** Ocean research, river flow measurement, harbour engineering.

---

## Part 4: Ultrasonic Sensors (Embedded / Drone Use)

These are the miniaturised, low-cost descendants of sonar used in robotics and drones.

### How an HC-SR04 Works (Common Drone Ultrasonic Sensor)

```
  ESP32 / MCU
       │
   TRIG pin → sends 10 μs HIGH pulse to sensor
       │
   Sensor fires 8 pulses of 40 kHz ultrasound
       │
   Sound travels to target and reflects back
       │
   ECHO pin goes HIGH when pulse sent, LOW when echo received
       │
   Measure duration of HIGH on ECHO pin
       │
   Distance = (duration × 343) / 2  [in metres, duration in seconds]
```

**HC-SR04 specifications:**
- Frequency: 40 kHz
- Range: 2 cm – 400 cm
- Resolution: ~3 mm
- Beam angle: ~15° cone
- Supply: 5V (use logic level shifter with ESP32)

**Limitations:**
- Soft or angled surfaces absorb or deflect sound → missed returns.
- Temperature affects speed of sound (use `v = 331.5 + 0.606 × T°C`).
- Maximum ~400 cm because signal too weak beyond that.
- Blind zone: 2 cm minimum (transmit pulse still ringing when echo would arrive).
- Wind affects accuracy — ultrasonic altimeters on drones work poorly in strong wind.

### Other Ultrasonic Sensor Modules

| Module        | Frequency | Range         | Interface  | Best Use                       |
|---------------|-----------|---------------|------------|--------------------------------|
| HC-SR04       | 40 kHz    | 2 – 400 cm    | Digital    | General obstacle detection     |
| JSN-SR04T     | 40 kHz    | 25 – 450 cm   | Digital    | Waterproof, outdoor use        |
| US-100        | 40 kHz    | 2 – 450 cm    | UART/Digital| Temperature-compensated       |
| TF-Luna       | Lidar*    | 0.2 – 8 m     | UART/I2C   | Better for drones than HC-SR04 |
| MaxBotix MB1240| 42 kHz   | 20 – 765 cm   | Analog/UART| High reliability, narrow beam  |
| A02YYUW       | Ultrasonic| 3 – 450 cm    | UART       | Waterproof with blind-zone fix |

*TF-Luna is lidar not ultrasonic — included for comparison.

---

## Part 5: Electronic Components in Sonar Systems

### Transducers
| Component           | Role                                                              |
|---------------------|-------------------------------------------------------------------|
| PZT transducer      | Converts electrical ↔ acoustic — core of all sonar systems       |
| PVDF hydrophone     | Flexible receiver — wideband, used in passive arrays             |
| cMUT                | MEMS capacitive transducer — medical probes, compact devices     |
| Piezo buzzer        | Low-cost ultrasonic emitter (40 kHz) — used in HC-SR04           |

### Transmit Electronics
| Component           | Role                                                              |
|---------------------|-------------------------------------------------------------------|
| Waveform generator IC | Generates precise pulse or CHIRP waveform                      |
| High-voltage pulser | Drives transducer with 50–200V pulses — needed because PZT requires high voltage for good output |
| TX/RX switch        | PIN diode or MOSFET switch — disconnects sensitive receiver during high-power transmit pulse |
| Matching network    | LC circuit that matches transducer impedance to amplifier        |

### Receive Electronics
| Component           | Role                                                              |
|---------------------|-------------------------------------------------------------------|
| LNA / preamp        | First amplification — low noise, high input impedance            |
| TGC amplifier       | Variable gain amplifier — gain increases with time after pulse   |
| VGA (Variable Gain Amplifier) | Controlled by DAC — sets gain profile for TGC        |
| Bandpass filter     | Centred on transmit frequency — rejects out-of-band noise        |
| ADC                 | Converts analog echo to digital samples for DSP                  |
| DSP / FPGA          | Beamforming, range calculation, Doppler processing, imaging      |

### Specialized ICs
| IC                  | Manufacturer | Purpose                                              |
|---------------------|--------------|------------------------------------------------------|
| AFE5801             | TI           | 8-channel ultrasound analog front end (LNA + TGC + ADC) |
| MAX2082             | Maxim         | Ultrasound front end                                 |
| PGA460              | TI           | Ultrasonic signal processor — handles pulse, receive, distance calc |
| TDC1000             | TI           | Ultrasonic sensing AFE for flow metering, level sensing |
| HC-SR04 chip        | Various       | Complete 40 kHz sonar module — MCU, oscillator, preamp, comparator |

---

## Part 6: Acoustic Properties — Important Concepts

### Acoustic Impedance (Z)
Every material has an **acoustic impedance** — how much it resists the propagation of sound:

```
Z = ρ × v

  ρ = density of material (kg/m³)
  v = speed of sound in material (m/s)
  Z is measured in Rayl (kg/m²s)
```

| Material      | Z (MRayl)  |
|---------------|------------|
| Air           | 0.0004     |
| Water         | 1.48       |
| Soft tissue   | 1.63       |
| Bone          | 6–8        |
| PZT ceramic   | 30–35      |
| Steel         | 46         |

### Reflection at Boundaries

When sound hits a boundary between two materials with **different acoustic impedance**, some energy reflects back and some transmits through:

```
Reflection coefficient R = ((Z2 - Z1) / (Z2 + Z1))²

Example: Sound going from water into air:
  Z1 (water) = 1.48 MRayl
  Z2 (air)   = 0.0004 MRayl
  R = ((0.0004 - 1.48) / (0.0004 + 1.48))² ≈ 0.9989

99.89% of the sound is reflected — almost nothing gets through.
```

This is why:
- Submarines are very detectable by sonar (large impedance mismatch with water).
- Medical ultrasound uses gel (eliminates air gap between probe and skin — air would reflect 99.9% of the signal).
- Fish have swim bladders full of gas — high impedance mismatch with flesh → strong sonar echo.

### Absorption / Attenuation
Sound energy is absorbed by the medium as it travels:
```
Attenuation: A (dB) = α × f² × d

  α = absorption coefficient (depends on medium)
  f = frequency (Hz)
  d = distance (metres)
```

**Key point: higher frequency = greater absorption.** This is why:
- Long-range naval sonar uses very low frequencies (100 Hz – 10 kHz) to travel hundreds of km.
- Medical imaging uses MHz frequencies — only needs to penetrate cm to tens of cm.
- Ultrasonic sensors (40 kHz) work over metres, not km.

### Beam Forming and Directivity
A sonar beam can be focused by:
- **Physical aperture**: larger transducer → narrower beam (like a larger antenna → higher gain in radar).
- **Array phasing**: delay signals across an array to steer or focus the beam.

```
Beam width (approx): θ = arcsin(0.88 × λ / D)

  λ = wavelength
  D = transducer diameter

Example: 200 kHz, D = 5 cm, λ = 7.5 mm
  θ ≈ arcsin(0.88 × 0.0075 / 0.05) ≈ 7.6°
```

---

## Part 7: Sonar vs Radar vs Lidar Comparison

| Feature           | Sonar                     | Radar                      | Lidar                     |
|-------------------|---------------------------|----------------------------|---------------------------|
| Wave type         | Sound (mechanical)        | EM (radio/microwave)        | EM (laser / near-IR)      |
| Medium needed     | Yes (gas, liquid, solid)  | No (works in vacuum)       | No (works in vacuum)      |
| Works underwater  | Excellent                 | Very poor (absorbed)       | Very poor (absorbed)      |
| Works in air      | Yes (short range)         | Excellent                  | Excellent                 |
| Works in fog/rain | Yes                       | Yes                        | Poor                      |
| Frequency used    | 100 Hz – 20 MHz           | 300 MHz – 300 GHz          | 100–1000 THz (laser)      |
| Speed of signal   | 343–5000 m/s              | 3 × 10⁸ m/s               | 3 × 10⁸ m/s              |
| Typical range     | cm – 1000 km              | m – 1000 km                | cm – 200 m                |
| Resolution        | cm – dm                   | cm – m                     | mm – cm                   |
| Cost              | Low – Very high           | Medium – Very high         | Medium – High             |

---

## Part 8: Key Formulas — Quick Reference

```
Distance:           d = (v × t) / 2
Speed of sound (air): v = 331.5 + 0.606 × T  (T in °C)
Frequency:          f = v / λ
Wavelength:         λ = v / f
Resonant freq:      f = v_crystal / (2 × thickness)
Acoustic impedance: Z = ρ × v
Reflection coeff:   R = ((Z2 - Z1) / (Z2 + Z1))²
Beam width:         θ ≈ arcsin(0.88 × λ / D)
Doppler shift:      fd = 2 × v_target × f / v_sound
Attenuation:        A = α × f² × d
```

---

## Glossary

| Term          | Meaning                                                                    |
|---------------|----------------------------------------------------------------------------|
| Transducer    | Device that converts energy from one form to another (electrical ↔ acoustic)|
| Hydrophone    | Underwater microphone — passive acoustic receiver                          |
| Piezoelectric | Material that generates voltage under mechanical stress and vice versa     |
| PZT           | Lead Zirconate Titanate — most common piezo material in transducers        |
| Acoustic impedance | Resistance of a material to sound propagation (Z = ρv)               |
| TGC           | Time-Gain Compensation — increases gain over time to equalise near/far echoes |
| Ping          | A single transmitted sonar pulse                                           |
| Echo          | The reflected return of a sonar ping                                       |
| Cavitation    | Bubbles formed when propellers or sonar create local low pressure — creates noise, damages hardware |
| SOFAR channel | Sound Fixing And Ranging — natural underwater sound waveguide at ~1000 m depth |
| DVL           | Doppler Velocity Log — measures vehicle speed using Doppler sonar          |
| ADCP          | Acoustic Doppler Current Profiler — measures water current velocity        |
| A-scan        | Amplitude vs time (depth) — one line of ultrasound data                    |
| B-scan        | Brightness (2D image) — many A-scans assembled side by side                |
| CHIRP         | Swept-frequency sonar pulse — like FMCW radar but with sound               |
| NDT           | Non-Destructive Testing — using ultrasound to inspect metal for cracks     |
| Reverberation | Sound bouncing multiple times off surfaces — like echo in a room, causes false returns |

---

## Relevance to Your Drone Project

| Topic                    | Application                                                           |
|--------------------------|-----------------------------------------------------------------------|
| HC-SR04 ultrasonic sensor | Obstacle detection and landing altitude — direct project component  |
| Temperature correction   | Speed of sound changes with temperature — compensate in code: `v = 331.5 + 0.606 × T` |
| Blind zone               | HC-SR04 cannot detect objects closer than 2 cm — design landing logic to account for this |
| Beam angle               | HC-SR04 has ~15° cone — may detect the ground at an angle on a tilt, giving false readings |
| Wind interference        | Ultrasonic less reliable in wind above ~5 m/s — radar altimeter more robust for drones |
| Doppler principle        | Same Doppler math used in sonar DVL applies to radar ground-speed sensing on drones |
| Acoustic impedance       | Explains why ultrasonic sensors fail over foam, grass, and soft surfaces (low reflection) |
| CHIRP sonar              | Same swept-frequency principle as FMCW radar — understanding one helps understand the other |
| Industrial ultrasound NDT| Technique for inspecting drone motor shafts and frame welds — same physics as sonar |
