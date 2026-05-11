# Radar — How It Works, Types, and Electronics

RADAR = **R**adio **D**etection **A**nd **R**anging

Radar uses radio waves to detect the position, distance, speed, and shape of objects — without any physical contact, in complete darkness, through fog, rain, and smoke.

---

## The Core Principle

Radar works on one simple idea:

> Send out a pulse of radio energy. If it hits something, some energy bounces back. Measure how long the round trip takes. Distance = (speed of light × time) / 2.

```
Transmitter → )))))))))))))) → Target
                                 │
Receiver    ← ((((((((((((( ← reflected echo

Distance = (c × t) / 2
  c = 3 × 10⁸ m/s (speed of light)
  t = round-trip time (seconds)
```

The division by 2 is because the wave travels to the target AND back — two trips for one distance.

**Example:**
```
Echo returns after 20 microseconds (20 × 10⁻⁶ s)
Distance = (3 × 10⁸ × 20 × 10⁻⁶) / 2 = 3000 m = 3 km
```

---

## What Radar Can Measure

| Measurement  | How                                                                      |
|--------------|--------------------------------------------------------------------------|
| Distance     | Round-trip travel time of the pulse                                       |
| Direction    | Angle the antenna is pointing when the echo is received                  |
| Speed        | Doppler shift — change in frequency of the returned echo                 |
| Size / shape | Strength and pattern of the returned echo (radar cross-section)          |
| Altitude     | Elevation angle of the antenna (in 3D radar systems)                     |

---

## Part 1: The Radar Equation

The radar equation tells you how strong the received echo will be:

```
Pr = (Pt × G² × λ² × σ) / ((4π)³ × R⁴)

  Pr  = received power (watts)
  Pt  = transmitted power (watts)
  G   = antenna gain (dimensionless)
  λ   = wavelength (metres)
  σ   = radar cross-section of target (m²)
  R   = range / distance to target (metres)
```

### Key Insight — the R⁴ problem
The received power drops with the **fourth power** of distance. Double the range → received power drops by 16×. This is why radar transmitters need huge power and sensitive receivers — detecting a target 100 km away is 10,000× harder than detecting the same target at 10 km.

### Radar Cross-Section (RCS)
RCS (σ) is a measure of how "visible" a target is to radar. It depends on:
- Physical size of the target
- Shape (flat surfaces perpendicular to radar = high RCS; curved shapes scatter energy away = low RCS)
- Material (metal = very high RCS; carbon fibre, wood = lower RCS)
- Stealth aircraft are designed to minimise RCS (angled surfaces, radar-absorbing materials).

```
Typical RCS values:
  Insect:            0.001 m²
  Bird:              0.01 m²
  Small drone:       0.01 – 0.1 m²
  Person:            1 m²
  Car:               10 – 100 m²
  Commercial airliner: 10 – 1000 m²
  Stealth fighter:   0.001 – 0.01 m²
```

---

## Part 2: How Radar Detects Speed — The Doppler Effect

When a target is moving, the reflected echo has a slightly different frequency than the transmitted signal.

- **Target moving toward radar**: echo is compressed → higher frequency (blue shift)
- **Target moving away from radar**: echo is stretched → lower frequency (red shift)

```
Doppler frequency shift:
  fd = 2 × v × f / c

  fd = Doppler shift (Hz)
  v  = target velocity toward/away from radar (m/s)
  f  = radar frequency (Hz)
  c  = speed of light (3 × 10⁸ m/s)

Example: X-band radar at 10 GHz, car moving at 30 m/s (108 km/h):
  fd = 2 × 30 × 10×10⁹ / 3×10⁸ = 2000 Hz = 2 kHz
```

The Doppler shift is very small (kHz) compared to the carrier (GHz) — but it is easy to detect by mixing the echo with the transmitted signal and looking at the difference frequency.

**CW Doppler radar** (Continuous Wave) transmits continuously and only measures the Doppler shift — it can measure speed but not distance.

**Pulsed Doppler radar** measures both distance (pulse timing) and speed (Doppler shift) — used in weather radar, air traffic control, fighter aircraft.

---

## Part 3: The Complete Radar Signal Chain

```
┌─────────────────────────────────────────────────────────┐
│                     TRANSMITTER                         │
│                                                         │
│  Waveform Generator → Modulator → Up-converter → PA    │
│        │                                    │           │
│   (pulse timing,                     (high power RF)   │
│    frequency)                                           │
└───────────────────────────────────┬─────────────────────┘
                                    │
                              Duplexer / T/R Switch
                            (switches between TX and RX)
                                    │
                               Antenna
                           (transmit & receive)
                                    │
                              EM wave travels outward
                                    │
                              Hits target → reflects
                                    │
                               Antenna
                           (receives faint echo)
                                    │
                              Duplexer / T/R Switch
                                    │
┌───────────────────────────────────▼─────────────────────┐
│                      RECEIVER                           │
│                                                         │
│  LNA → Down-converter → IF Amplifier → ADC → DSP       │
│         (mixer + LO)                    (digital        │
│                                          processing)    │
│                                              │          │
│                                    Range, speed,        │
│                                    target detection     │
└─────────────────────────────────────────────────────────┘
```

---

## Part 4: Key Radar Parameters

### Pulse Width (τ)
Duration of each transmitted pulse. Affects:
- **Range resolution**: minimum distance between two targets that can be distinguished.
  ```
  Range resolution = (c × τ) / 2
  Shorter pulse → better range resolution
  ```
- Shorter pulse = less energy per pulse → need higher peak power.

### Pulse Repetition Frequency (PRF)
How many pulses are sent per second.
- Higher PRF → more updates per second (better for fast targets).
- Higher PRF → shorter unambiguous range (echo from a distant target may return after the next pulse is sent, causing confusion).
```
Maximum unambiguous range = c / (2 × PRF)

Example: PRF = 1000 Hz
  Max range = 3×10⁸ / (2 × 1000) = 150,000 m = 150 km
```

### Duty Cycle
```
Duty cycle = Pulse Width × PRF

Example: τ = 1 μs, PRF = 1000 Hz
  Duty cycle = 1×10⁻⁶ × 1000 = 0.001 = 0.1%

Radar is transmitting only 0.1% of the time → peak power can be 1000× higher than average power.
```

### Bandwidth
The frequency range occupied by the radar pulse.
```
Bandwidth ≈ 1 / τ

Wider bandwidth → shorter pulses possible → better range resolution
```

---

## Part 5: Types of Radar

---

### 1. Pulsed Radar (Primary Radar)
The classic radar type. Sends a short high-power pulse, then listens for echoes during the quiet period.

**How it works:**
- Transmitter fires a burst (typically 0.1 – 10 μs)
- T/R switch disconnects transmitter, connects receiver
- Receiver listens for echoes
- Time of arrival determines distance
- Antenna direction determines bearing

**Used in:** Air traffic control, weather, ship navigation, military surveillance.

---

### 2. Continuous Wave (CW) Radar
Transmits continuously (no pulses). Can only measure **speed** via Doppler — cannot measure distance.

**How it works:**
- Transmitter and receiver operate simultaneously (separate antennas to isolate them).
- Received signal is mixed with the transmitted signal.
- The difference frequency = Doppler shift = proportional to target speed.

**Used in:** Police speed guns, automatic door sensors, simple motion detectors.

```
Police radar gun — how it works:
  Transmits: 10.525 GHz
  Car at 30 m/s reflects back: 10.525 GHz + 2 kHz (Doppler shifted)
  Mixer output: 2 kHz → measured → speed calculated
```

---

### 3. FMCW Radar (Frequency-Modulated Continuous Wave)
Transmits continuously but sweeps (chirps) the frequency up and down over time. Can measure **both distance AND speed**.

**How it works:**
- Transmit frequency ramps linearly (e.g., 77 GHz to 81 GHz over 1 ms).
- Echo returns after a delay corresponding to target distance.
- Mix the current transmitted frequency with the delayed echo.
- The difference frequency (beat frequency) is proportional to distance.

```
Transmitted: ramp up from f1 to f2 over time T
Echo:        same ramp, delayed by time t (= 2R/c)
Beat freq:   fb = (f2 - f1) × t / T = (BW × 2R) / (c × T)

So distance: R = fb × c × T / (2 × BW)
```

**Advantages over pulsed radar:**
- Low peak power (continuous transmission).
- High range resolution (determined by sweep bandwidth, not pulse width).
- Much cheaper to build.
- Works at very short ranges (cm to meters) — pulsed radar has a "blind zone" near the antenna.

**Used in:** Automotive collision avoidance (77 GHz), drone altimeters, level sensors, through-wall detection, gesture recognition (Google Soli chip).

#### FMCW on Drones:
- **Radar altimeter**: measures height above ground — more reliable than ultrasonic in wind.
- **Collision avoidance**: detects obstacles in flight path.
- **Landing radar**: precise height for auto-land.

---

### 4. Phased Array Radar
Instead of mechanically rotating an antenna, a phased array steers the beam electronically by controlling the phase of signals at each antenna element.

**How it works:**
- Array of many small antennas, each driven by a phase shifter.
- When all elements transmit in phase → beam points straight ahead.
- When phases are shifted progressively across the array → beam tilts.
- Beam can be steered instantly (microseconds) in any direction — no moving parts.

```
Array of 4 elements, beam steered right:

Element: 1      2      3      4
Phase:   0°    45°    90°   135°

Combined wave front tilts → beam points right
```

**Advantages:** Instant beam steering, can track multiple targets simultaneously, no mechanical failure, very agile.

**Used in:** AESA fighter radar, Patriot missile defence, 5G base stations, modern weather radar.

---

### 5. SAR — Synthetic Aperture Radar
A moving radar that synthesises a large virtual antenna by combining echoes received at different positions along its flight path.

**How it works:**
- Aircraft or satellite carries a small radar.
- As it moves, it collects echoes from the same ground patch at many positions.
- DSP combines all these echoes coherently — simulating a very long (synthetic) antenna.
- Longer antenna → narrower beam → higher resolution image.

**Result:** Can produce photographic-quality images of the ground regardless of darkness, cloud, or weather — because it uses microwaves not visible light.

**Used in:** Military reconnaissance satellites, Earth observation (ESA Sentinel-1), geological mapping, flood monitoring, detecting buried structures.

---

### 6. Weather Radar (Doppler Weather Radar)
Measures precipitation (rain, hail, snow) and wind speed in the atmosphere.

**How it works:**
- Sends pulses and detects echoes from rain droplets and ice crystals.
- Echo strength → precipitation intensity.
- Doppler shift → wind speed (rain droplets moving with the wind).
- Dual-polarisation: sends both horizontal and vertical pulses → distinguishes rain from hail, measures drop size.

**Frequency:** S-band (2–4 GHz), C-band (4–8 GHz), X-band (8–12 GHz). X-band is compact enough to mount on aircraft.

---

### 7. Ground-Penetrating Radar (GPR)
Low-frequency radar that penetrates the ground to detect buried objects.

**Frequency:** 10 MHz – 3 GHz (lower frequency = deeper penetration, lower resolution).

**How it works:**
- Pulse is transmitted into the ground.
- Echoes return when the wave hits boundaries between different materials (soil/rock, soil/metal, soil/plastic pipe).
- 2D scan produces a cross-section image of what's underground.

**Used in:** Finding buried pipes and cables, archaeology, detecting landmines, forensic investigation, inspecting concrete structures.

---

### 8. Bistatic Radar
Transmitter and receiver are at different locations — separated by a large distance.

**How it works:**
- Transmitter illuminates the target from one direction.
- Receiver is positioned elsewhere and detects the scatter.
- Target does not need to be between TX and RX — more flexible coverage.

**Passive bistatic radar:** Uses signals from FM radio, TV broadcast, or WiFi as the "transmitter" — no dedicated transmitter needed. Can detect stealth aircraft (which are optimised to deflect monostatic radar but scatter in other directions).

---

### 9. Lidar (Light Detection and Ranging)
Technically not radar (uses laser light, not radio waves) but works on exactly the same time-of-flight principle.

**Frequency:** Near-infrared (905 nm, 1550 nm) or green laser (532 nm).

**How it works:**
- Laser fires short pulses.
- Time of flight → distance.
- Scanning mirror or rotating head → 3D point cloud.

**Advantages over radar:**
- Much higher resolution (mm to cm vs dm to m for radar).
- Better angular resolution — can build detailed 3D maps.

**Disadvantages:**
- Blocked by rain, dust, fog (radar goes through these).
- Eye safety limits (power limits on near-IR lasers).

**Used in:** Autonomous vehicles, survey drones, robot navigation (ROS + LiDAR), altitude hold on precision drones.

---

## Part 6: Electronic Components in Radar Systems

### Transmit Side

| Component           | Role                                                                  |
|---------------------|-----------------------------------------------------------------------|
| Waveform generator  | Creates the pulse timing or FMCW frequency ramp                       |
| VCO (Voltage Controlled Oscillator) | Generates the RF carrier — frequency set by control voltage |
| PLL (Phase-Locked Loop) | Locks VCO to a precise reference frequency                      |
| Frequency multiplier| Multiplies lower frequency to reach mmWave (e.g., 24 GHz or 77 GHz) |
| RF Power Amplifier (PA) | MOSFET or GaN HEMT boosts signal to transmit power             |
| PIN diode switch    | Fast RF switch for T/R switching (switching time: nanoseconds)       |
| Circulator / duplexer | Ferrite device that routes TX to antenna and echo to RX without them interfering |

### Receive Side

| Component           | Role                                                                  |
|---------------------|-----------------------------------------------------------------------|
| LNA (Low-Noise Amplifier) | First amplifier after antenna — sets sensitivity of entire receiver |
| Mixer               | Down-converts RF echo to IF or baseband — Schottky diodes or active mixers |
| Local Oscillator (LO) | Provides reference frequency for mixing                           |
| IF Amplifier        | Amplifies the down-converted signal                                   |
| ADC (Analog-to-Digital Converter) | Samples the IF signal for DSP processing               |
| DSP / FPGA / MCU    | Computes range, speed, target detection via FFT and signal processing |

### Antenna

| Component           | Role                                                                  |
|---------------------|-----------------------------------------------------------------------|
| Parabolic dish      | High-gain directional antenna — focuses beam, used in long-range radar|
| Phased array        | Grid of patch antennas with phase shifters — electronic beam steering |
| Patch antenna       | Flat PCB antenna — used in FMCW radar modules, automotive radar       |
| Waveguide           | Hollow metal tube that guides microwaves — low-loss at high frequency |
| Horn antenna        | Flared waveguide opening — moderate gain, wide bandwidth              |

### Semiconductors Used in Radar

| Device              | Material     | Role                                              |
|---------------------|--------------|---------------------------------------------------|
| GaAs MESFET         | Gallium Arsenide | RF amplifier / oscillator — high frequency    |
| GaN HEMT            | Gallium Nitride | High-power PA — handles high voltage and heat |
| SiGe HBT            | Silicon Germanium | Low-noise LNA, mixer, VCO in FMCW radar ICs |
| Schottky diode      | Silicon/GaAs | Mixer, detector — very fast, low forward voltage |
| PIN diode           | Silicon      | RF switch, phase shifter in phased arrays         |
| InP HEMT            | Indium Phosphide | Extremely low-noise LNA for mmWave and space |

**Why not normal silicon BJT/MOSFET?** Regular silicon transistors can't switch fast enough at GHz frequencies. GaAs and GaN have higher electron mobility — electrons move faster, allowing operation at 10–100 GHz.

---

## Part 7: Signal Processing in Radar

Modern radar is mostly a signal processing problem. The hardware front-end captures the echo, then algorithms extract the information.

### Range FFT
The raw ADC samples from an FMCW radar beat signal are a mix of frequencies — each frequency corresponds to one range. An **FFT (Fast Fourier Transform)** separates them into a range profile showing how much echo power exists at each distance.

### Doppler FFT (2D FFT)
Collecting multiple pulses and doing a second FFT across pulses gives the Doppler dimension — a 2D map of range vs speed.

### CFAR Detection (Constant False Alarm Rate)
A threshold algorithm that adapts to local noise level. Detects targets that stand out above the local background noise — prevents both missing targets (threshold too high) and false alarms (threshold too low).

### Beamforming (Phased Array)
Combining signals from multiple receive antennas with different phase weights to focus reception on one direction — digital phased array processing.

### Clutter Filtering
Ground, buildings, and sea surface return strong echoes (clutter). Moving Target Indication (MTI) and Doppler filtering suppress stationary clutter to reveal moving targets.

---

## Part 8: Radar Frequency Bands

| Band | Frequency       | Wavelength  | Typical Use                                      |
|------|-----------------|-------------|--------------------------------------------------|
| HF   | 3 – 30 MHz      | 100 – 10 m  | Over-the-horizon radar (reflects off ionosphere) |
| VHF  | 30 – 300 MHz    | 10 – 1 m    | Long-range air surveillance, foliage penetration |
| UHF  | 300 – 1000 MHz  | 1 m – 30 cm | Ballistic missile detection                      |
| L    | 1 – 2 GHz       | 30 – 15 cm  | Long-range air traffic control (ATC)             |
| S    | 2 – 4 GHz       | 15 – 7.5 cm | Weather radar, ATC, ship radar                   |
| C    | 4 – 8 GHz       | 7.5 – 3.75 cm | Weather radar, satellite altimeters            |
| X    | 8 – 12 GHz      | 3.75 – 2.5 cm | Airborne weather, maritime, police radar       |
| Ku   | 12 – 18 GHz     | 2.5 – 1.67 cm | Satellite altimetry, small drone radar        |
| K    | 18 – 27 GHz     | 1.67 – 1.1 cm | Police speed radar (24 GHz)                  |
| Ka   | 27 – 40 GHz     | 1.1 – 7.5 mm | High-resolution SAR, police radar             |
| W    | 75 – 110 GHz    | 4 – 2.7 mm  | Automotive radar (77 GHz), imaging             |
| mm   | 110 – 300 GHz   | 2.7 – 1 mm  | Airport security scanners, 5G mmWave           |

**Rule of thumb:**
- Lower frequency → longer range, worse resolution, larger antenna
- Higher frequency → shorter range, better resolution, smaller antenna

---

## Part 9: Radar vs Other Sensing Technologies

| Technology  | Principle      | Works in darkness | Fog/rain | Range       | Resolution  | Cost    |
|-------------|----------------|-------------------|----------|-------------|-------------|---------|
| Radar       | RF reflection  | Yes               | Yes      | mm – 1000 km| dm – m      | Medium  |
| Lidar       | Laser TOF      | Yes               | Poor     | 0.1 – 200 m | mm – cm     | High    |
| Ultrasonic  | Sound TOF      | Yes               | Yes      | 0.02 – 5 m  | cm          | Very low|
| IR sensor   | IR reflection  | Yes               | Poor     | 0.02 – 1.5 m| cm          | Low     |
| Stereo camera | Visual depth | No                | Poor     | 0.5 – 20 m  | cm          | Low     |
| GPS         | RF TOA         | Yes               | Yes      | Global      | 1 – 10 m    | Low     |

---

## Part 10: Radar Modules Used in Embedded / Drone Systems

### HLK-LD2410 (24 GHz FMCW)
- 24 GHz millimetre-wave radar module
- Detects presence, movement, and distance of humans
- Used for occupancy detection, security
- Output: UART serial data
- Range: 0 – 6 m

### TI AWR1642 / AWR1843 (77 GHz FMCW)
- Automotive-grade radar SoC from Texas Instruments
- 77 GHz, complete radar on a chip
- Built-in DSP and ARM Cortex-R processor
- Used in: collision avoidance, gesture recognition, drone obstacle avoidance
- Provides 3D point cloud output

### Ainstein US-D1 Radar Altimeter
- 24 GHz FMCW
- Specifically designed for drone altitude measurement
- Range: 0.5 – 50 m
- Works reliably over water, tall grass, uneven terrain where ultrasonic fails

### Google Soli (60 GHz FMCW)
- Built into Pixel 4 phone
- Detects sub-millimetre hand gestures
- Range: a few centimetres
- Shows how FMCW radar can be miniaturised to chip scale

### Module Comparison for Drone Use:

| Module         | Frequency | Range    | Best Use                         |
|----------------|-----------|----------|----------------------------------|
| Ainstein US-D1 | 24 GHz    | 0.5–50 m | Radar altimeter                  |
| TI AWR1642     | 77 GHz    | 0.1–30 m | Obstacle avoidance, point cloud  |
| HLK-LD2410     | 24 GHz    | 0–6 m    | Presence detection (ground station) |
| Benewake TF03  | Lidar     | 0.1–100 m| Precision altimeter (not radar but similar use) |

---

## Key Formulas — Quick Reference

```
Distance:          R = (c × t) / 2
Doppler shift:     fd = (2 × v × f) / c
Target speed:      v = (fd × c) / (2 × f)
Range resolution:  ΔR = c / (2 × B)       [B = bandwidth]
FMCW beat freq:    fb = (2 × R × BW) / (c × T)
Max unambiguous R: Rmax = c / (2 × PRF)
Radar equation:    Pr = (Pt × G² × λ² × σ) / ((4π)³ × R⁴)
Antenna gain:      G = 4π × Ae / λ²       [Ae = effective area]
```

---

## Glossary

| Term    | Meaning                                                                       |
|---------|-------------------------------------------------------------------------------|
| RCS     | Radar Cross-Section — how reflective a target is (m²)                        |
| PRF     | Pulse Repetition Frequency — pulses per second                                |
| PRI     | Pulse Repetition Interval — time between pulses (1/PRF)                       |
| CW      | Continuous Wave — transmits without interruption                              |
| FMCW   | Frequency-Modulated Continuous Wave — swept-frequency radar                   |
| LFM     | Linear Frequency Modulation — same as FMCW, also called a chirp              |
| Chirp   | A pulse whose frequency sweeps up or down over time                           |
| Duplexer| Component that allows one antenna to be shared by TX and RX                   |
| Clutter | Unwanted echoes from ground, sea, rain, buildings                             |
| MTI     | Moving Target Indication — filters out stationary clutter                     |
| CFAR    | Constant False Alarm Rate — adaptive threshold for target detection           |
| SAR     | Synthetic Aperture Radar — uses motion to synthesise a large antenna          |
| FFT     | Fast Fourier Transform — splits beat signal into range/speed components       |
| SNR     | Signal-to-Noise Ratio — target echo power vs receiver noise                   |
| HEMT    | High Electron Mobility Transistor — GaN/GaAs device used in radar RF circuits |
| GaN     | Gallium Nitride — semiconductor for high-power RF amplifiers                  |

---

## Relevance to Your Drone Project

| Topic                  | Application                                                          |
|------------------------|----------------------------------------------------------------------|
| FMCW radar altimeter   | Measures height above ground — more reliable than ultrasonic in wind |
| 77 GHz obstacle radar  | Detects trees, poles, other aircraft ahead                           |
| Doppler radar          | Measures drone ground speed independent of GPS                       |
| Radar cross-section    | Small drones have low RCS (~0.01 m²) — hard to detect with ATC radar, raising airspace safety questions |
| FMCW beat frequency    | Understanding this helps you read radar altimeter datasheets         |
| T/R switch             | Your radio modules use PIN diode switches in the same way radar does |
| FFT in signal processing | The same FFT used in radar range processing appears in audio, vibration, and motor current analysis on drones |
| LNA in receiver        | Your RC receiver and radar use the same LNA principle — weak signal, first-stage amplification defines system sensitivity |
