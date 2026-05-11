# Electromagnetic Waves — Full Explanation and Types

---

## What Is an Electromagnetic Wave?

An electromagnetic (EM) wave is a **self-propagating disturbance of electric and magnetic fields** that travels through space — including through a vacuum where no air or matter exists.

Unlike sound waves (which need air to travel), EM waves need **nothing to travel through**. This is how sunlight reaches Earth across 150 million km of empty space, and how your drone remote communicates with your drone without any wire.

---

## How an EM Wave Is Born

### The Physics (step by step)

1. **A charged particle at rest** creates a static electric field around it — like a ball sitting on a stretched rubber sheet.

2. **A moving charge (electric current)** creates a magnetic field around it — this is how electromagnets work.

3. **An accelerating charge** (a charge that is speeding up, slowing down, or changing direction) causes the electric field to change over time.

4. **A changing electric field** creates a changing magnetic field (Maxwell's law of induction).

5. **A changing magnetic field** creates a changing electric field (Faraday's law).

6. Steps 4 and 5 feed each other in a loop — the two fields regenerate each other and **propagate outward as a wave**, forever, at the speed of light.

```
Accelerating charge
        │
        ▼
Changing Electric Field (E)
        │  creates
        ▼
Changing Magnetic Field (B)
        │  creates
        ▼
Changing Electric Field (E)
        │  creates
        ▼
   ... propagates outward at c = 3×10⁸ m/s
```

In a wire carrying alternating current (like a transmitting antenna), electrons are constantly accelerating back and forth — this is exactly the condition that radiates EM waves.

---

## Structure of an EM Wave

An EM wave has **two field components** oscillating perpendicular to each other and to the direction of travel:

```
Direction of travel →

        E field (electric)
        ↑   ↑   ↑
        │   │   │
────────┼───┼───┼──────────→ direction of propagation
        │   │   │
        ↓   ↓   ↓
        
        B field (magnetic) — oscillates into/out of the page
```

- **E field** and **B field** are always perpendicular to each other.
- Both are perpendicular to the direction of travel.
- This is called a **transverse wave** (unlike sound, which is longitudinal).

---

## Key Properties of EM Waves

### 1. Frequency (f)
- How many complete wave cycles pass a point per second.
- Unit: **Hz** (Hertz). 1 Hz = 1 cycle/second.
- Higher frequency = more energetic wave.

### 2. Wavelength (λ — lambda)
- Physical distance between two consecutive identical points on the wave (e.g., peak to peak).
- Unit: metres (m), centimetres (cm), nanometres (nm), etc.

### 3. Speed (c)
- All EM waves travel at the **speed of light in a vacuum**: `c = 299,792,458 m/s ≈ 3 × 10⁸ m/s`
- In other materials (glass, water) they slow down. The ratio is called the **refractive index**.

### 4. The Fundamental Relationship
```
c = f × λ

  c = speed of light (3 × 10⁸ m/s)
  f = frequency (Hz)
  λ = wavelength (metres)

So:   λ = c / f     and     f = c / λ
```

**Examples:**
```
FM radio (100 MHz):    λ = 3×10⁸ / 1×10⁸  = 3 m
WiFi / drone (2.4 GHz): λ = 3×10⁸ / 2.4×10⁹ = 0.125 m = 12.5 cm
Visible light (500 THz): λ = 3×10⁸ / 5×10¹⁴ = 600 nm (nanometres)
```

### 5. Amplitude
- The peak strength of the electric field.
- Related to the **intensity** (power per unit area) of the wave.
- Intensity ∝ Amplitude²

### 6. Polarisation
- The direction in which the E field oscillates.
- **Vertical polarisation**: E field oscillates up/down — used by most RC antennas.
- **Horizontal polarisation**: E field oscillates left/right.
- **Circular polarisation**: E field rotates as the wave travels — used in FPV video to reduce signal loss from antenna orientation changes.

---

## The Electromagnetic Spectrum

The EM spectrum is the complete range of all possible EM wave frequencies, from extremely low frequency radio waves to ultra-high-energy gamma rays. They are all the same kind of wave — only the frequency (and therefore wavelength and energy) differs.

```
Low Frequency ◄──────────────────────────────────────────────────► High Frequency
Long Wavelength                                                      Short Wavelength
Low Energy                                                           High Energy

Radio    Microwave   Infrared   Visible   Ultraviolet   X-ray   Gamma Ray
  │          │           │         │           │           │        │
10³Hz     10¹⁰Hz      10¹³Hz   10¹⁵Hz      10¹⁶Hz     10¹⁸Hz  10²⁰Hz
 km         cm          μm       nm          nm          pm       pm
```

---

## Detailed Types of Electromagnetic Waves

---

### Type 1: Radio Waves
**Frequency range:** 3 Hz – 300 GHz
**Wavelength range:** 100,000 km down to 1 mm

The longest-wavelength, lowest-energy EM waves. Produced by oscillating electrons in antennas (AC current at the transmission frequency).

#### Sub-bands:

| Sub-band | Frequency       | Wavelength   | Uses                                         |
|----------|-----------------|--------------|----------------------------------------------|
| ELF      | 3 – 30 Hz       | 100,000 km   | Submarine communication (penetrates seawater)|
| SLF      | 30 – 300 Hz     | 10,000 km    | Submarine communication                      |
| ULF      | 300 Hz – 3 kHz  | 1000 km      | Mine communication, earthquake detection     |
| VLF      | 3 – 30 kHz      | 100 km       | Navigation, time signals                     |
| LF       | 30 – 300 kHz    | 10 km – 1 km | AM longwave radio, RFID (125 kHz)            |
| MF       | 300 kHz – 3 MHz | 1 km – 100 m | AM broadcast radio (530–1700 kHz)            |
| HF       | 3 – 30 MHz      | 100 – 10 m   | Shortwave radio, amateur (ham) radio         |
| VHF      | 30 – 300 MHz    | 10 – 1 m     | FM radio, TV, aviation, marine               |
| UHF      | 300 MHz – 3 GHz | 1 m – 10 cm  | WiFi, Bluetooth, 4G LTE, drone remotes       |
| SHF      | 3 – 30 GHz      | 10 – 1 cm    | WiFi (5 GHz), radar, satellite, 5G           |
| EHF      | 30 – 300 GHz    | 1 – 0.1 cm   | mmWave 5G, automotive radar, body scanners   |

#### Key Behaviour:
- Long wavelengths (MF/HF) can **diffract** around the Earth's curvature — AM radio can be heard hundreds of miles away at night.
- Higher frequencies (UHF/SHF) travel in **line-of-sight** only — blocked by hills and buildings.
- Radio waves pass through walls (lower frequencies better than higher).
- Ionosphere reflects HF waves — enables worldwide shortwave communication.

#### How Produced Electronically:
- Oscillator circuit generates AC at the desired frequency.
- Amplified by RF transistors / MOSFETs.
- Fed into an antenna where accelerating electrons radiate the wave.

#### Drone relevance:
- 2.4 GHz (UHF/SHF boundary) — RC control link, WiFi telemetry, Bluetooth.
- 5.8 GHz (SHF) — FPV video downlink.
- 433 MHz (UHF) — long-range telemetry (LoRa modules, longer range but lower data rate).

---

### Type 2: Microwaves
**Frequency range:** 300 MHz – 300 GHz
**Wavelength range:** 1 m – 1 mm

Microwaves are technically a sub-set of radio waves but are often treated separately because of their distinct behaviour and applications.

#### Key Behaviour:
- Absorbed by water molecules — causes them to vibrate and heat up. This is literally how a microwave oven works (2.45 GHz).
- Reflected by metals — used in radar.
- Travel in straight lines — used for point-to-point links and satellite communication.
- **Skin effect**: at high microwave frequencies, current travels only on the surface of conductors — affects circuit board trace design (microstrip lines).

#### How Produced Electronically:
- **Magnetron**: a vacuum tube device that generates microwaves using electron resonance in a magnetic field — used in microwave ovens and older radar.
- **Gunn diode**: a special semiconductor that oscillates at microwave frequencies when voltage is applied.
- **IMPATT diode**: used in millimetre-wave radar.
- **MMIC (Monolithic Microwave Integrated Circuit)**: modern chip-scale solution — entire transmitter/receiver in one IC. Used in your WiFi and drone modules.

#### Applications:
| Application        | Frequency        | Detail                                          |
|--------------------|------------------|-------------------------------------------------|
| Microwave oven     | 2.45 GHz         | Heats water molecules in food                   |
| WiFi 2.4 GHz       | 2.4 – 2.485 GHz  | 802.11b/g/n                                     |
| WiFi 5 GHz         | 5.15 – 5.85 GHz  | 802.11a/n/ac                                    |
| Bluetooth          | 2.402 – 2.480 GHz| FHSS across 79 channels                         |
| 4G LTE             | 700 MHz – 2.7 GHz| Mobile internet                                 |
| 5G mmWave          | 24 – 100 GHz     | Very high speed, very short range               |
| Weather radar      | 2 – 10 GHz       | S-band and C-band radar                         |
| Speed radar (police)| 10.5 GHz / 24 GHz| K-band and X-band Doppler radar                 |
| Satellite TV       | 10.7 – 12.75 GHz | Ku-band downlink                                |

---

### Type 3: Infrared (IR)
**Frequency range:** 300 GHz – 430 THz
**Wavelength range:** 1 mm – 700 nm

Infrared means "below red" — just below the red end of visible light in frequency. Any object warmer than absolute zero emits infrared radiation.

#### Sub-bands:
| Sub-band    | Wavelength      | Key Uses                                          |
|-------------|-----------------|---------------------------------------------------|
| Far-IR      | 1 mm – 15 μm    | Thermal imaging, astronomy                        |
| Mid-IR      | 15 μm – 2.5 μm  | Gas sensing, spectroscopy, molecular absorption   |
| Near-IR     | 2.5 μm – 700 nm | Remote controls, short-range data (IrDA), night vision |

#### Key Behaviour:
- All warm objects radiate IR — humans radiate strongly at ~10 μm.
- Near-IR passes through silicon — IR cameras can "see through" silicon chips.
- Absorbed by CO₂ and water vapour — causes the greenhouse effect.
- Near-IR is reflected by plants (used in agricultural satellite imaging).

#### How Produced Electronically:
- **IR LED (infrared LED)**: emits near-IR (typically 850 nm or 940 nm) when current flows. Used in TV remotes, IR sensors, proximity sensors.
- **Laser diode**: emits coherent near-IR — used in fibre optic communications, DVD/Blu-ray players, LIDAR.
- **Thermal emitter**: any resistive heating element radiates broad-spectrum IR.

#### Detection:
- **Photodiode**: semiconductor junction that generates current when IR photons hit it.
- **Phototransistor**: like a photodiode but with built-in gain — used in IR receivers.
- **Thermopile**: array of thermocouples that convert temperature difference to voltage — used in non-contact thermometers.
- **Microbolometer**: resistive element whose resistance changes with temperature — used in thermal cameras.

#### Applications:
| Application             | Wavelength    | Detail                                         |
|-------------------------|---------------|------------------------------------------------|
| TV/AC remote control    | 940 nm        | NEC or RC5 protocol, modulated at 38 kHz       |
| IR obstacle sensor      | 850 nm        | Used in robot/drone proximity detection        |
| Night vision camera     | 850 nm        | IR LEDs illuminate, camera detects             |
| Thermal camera          | 8 – 14 μm     | Maps heat, used in industrial and military     |
| Fibre optic comms       | 1310 / 1550 nm| Low loss in glass fibre at these wavelengths   |
| Facial recognition (iPhone)| 940 nm   | Dot projector and IR camera for Face ID        |

#### Drone relevance:
- IR sensors used for altitude hold (down-facing IR rangefinder on some small drones).
- Thermal cameras mounted on drones for search and rescue.
- IR LEDs used in optical flow sensors for low-light ground tracking.

---

### Type 4: Visible Light
**Frequency range:** 430 THz – 770 THz
**Wavelength range:** 700 nm (red) – 380 nm (violet)

The narrow slice of the EM spectrum that human eyes are sensitive to. Only about one octave wide (frequency doubles from red to violet).

#### Colours and Wavelengths:
| Colour | Wavelength  | Frequency   |
|--------|-------------|-------------|
| Red    | 620 – 700 nm | 430 – 480 THz |
| Orange | 590 – 620 nm | 480 – 510 THz |
| Yellow | 570 – 590 nm | 510 – 530 THz |
| Green  | 495 – 570 nm | 530 – 605 THz |
| Blue   | 450 – 495 nm | 605 – 665 THz |
| Violet | 380 – 450 nm | 665 – 790 THz |

#### Key Behaviour:
- Refraction: slows in glass — different frequencies slow differently, causing **dispersion** (prisms split white light into colours).
- Reflection: determines the colour of objects (a red apple reflects 620–700 nm, absorbs the rest).
- Interference and diffraction: creates iridescent colours in soap bubbles, CDs, butterfly wings.
- Photoelectric effect: photons above a threshold energy knock electrons out of metal surfaces — this is how solar cells and photodetectors work.

#### How Produced Electronically:
- **LED (Light Emitting Diode)**: electrons recombine across a semiconductor bandgap and release energy as photons. The bandgap determines the colour.
  - Red LED: GaAsP (gallium arsenide phosphide) — ~1.8 eV bandgap
  - Green LED: InGaN — ~2.4 eV bandgap
  - Blue LED: GaN — ~3.1 eV bandgap (Nobel Prize 2014)
  - White LED: blue LED + yellow phosphor coating
- **Laser diode**: stimulated emission produces coherent, monochromatic, collimated light.
- **Incandescent bulb**: resistive heating to ~2700°C — broad thermal spectrum, mostly IR, some visible.

#### Detection:
- **Photodiode / phototransistor**: generates current from photons — used in light sensors.
- **LDR (Light Dependent Resistor)**: resistance decreases with light intensity — simple, slow, used in automatic night lights.
- **CCD / CMOS image sensor**: array of millions of photodiodes — used in cameras (drone FPV cameras use CMOS).

#### Drone relevance:
- **FPV (First Person View) camera** uses CMOS sensor — detects visible light.
- **RGB LEDs** on drones for orientation indication (red = left, green = right).
- **Optical flow sensor** uses visible/near-IR light to measure ground movement for position hold.
- **LiDAR** on advanced drones uses pulsed laser (near-IR or visible green) for 3D mapping.

---

### Type 5: Ultraviolet (UV)
**Frequency range:** 770 THz – 30 PHz
**Wavelength range:** 380 nm – 10 nm

"Above violet" — higher energy than visible light. Invisible to human eyes (some insects can see near-UV).

#### Sub-bands:
| Sub-band | Wavelength  | Key Properties                                         |
|----------|-------------|--------------------------------------------------------|
| UV-A     | 315 – 380 nm | Reaches Earth's surface, causes tanning, used in UV curing |
| UV-B     | 280 – 315 nm | Partially blocked by ozone, causes sunburn, produces vitamin D |
| UV-C     | 100 – 280 nm | Completely blocked by ozone at sea level — germicidal  |
| Vacuum UV| 10 – 100 nm  | Absorbed by air — only usable in vacuum                |

#### Key Behaviour:
- Ionises molecules — can break chemical bonds and damage DNA (UV-C is used for sterilisation).
- Causes fluorescence in certain materials — they absorb UV and emit visible light.
- Blocked by glass (UV-C and most UV-B cannot pass through regular glass).
- Ozone layer absorbs UV-C and most UV-B.

#### How Produced Electronically:
- **UV LED**: GaN or AlGaN semiconductors — produces 365 nm (UV-A) or 280 nm (UV-C).
- **Mercury vapour lamp**: electrical discharge in mercury vapour — emits strongly at 254 nm (UV-C).
- **Deuterium lamp**: used in UV spectrophotometers.

#### Detection:
- **UV photodiode**: responds to UV wavelengths — used in UV index sensors.
- **Photomultiplier tube (PMT)**: used in scientific instruments for very weak UV detection.

#### Applications:
- UV-C LED sterilisation (hospital equipment, water purification).
- UV curing of adhesives and resins (3D printing resins cure under UV).
- UV lamps for banknote verification (fluorescent security marks).
- Photolithography in semiconductor manufacturing (deep UV lasers etch nm-scale circuits).

---

### Type 6: X-Rays
**Frequency range:** 30 PHz – 30 EHz
**Wavelength range:** 10 nm – 0.01 nm (10 pm)

High-energy EM radiation capable of penetrating soft tissue. Wavelength is on the order of atomic spacing — used for crystal structure analysis.

#### Sub-bands:
| Type      | Wavelength   | Uses                                              |
|-----------|--------------|---------------------------------------------------|
| Soft X-ray| 10 – 0.1 nm  | Astronomy, surface analysis                       |
| Hard X-ray| 0.1 – 0.01 nm| Medical imaging, industrial inspection            |

#### Key Behaviour:
- Penetrates soft tissue but absorbed by dense materials (bone, metal).
- Ionising radiation — can damage DNA and cause cancer at high doses.
- Wavelength matches atomic spacing in crystals — enables X-ray crystallography (used to determine protein and DNA structures).
- Absorbed by lead — lead shielding used for protection.

#### How Produced Electronically:
- **X-ray tube**: electrons accelerated by high voltage (30–150 kV) slam into a tungsten target. The sudden deceleration produces X-rays (Bremsstrahlung radiation). Also, inner-shell electron excitation produces characteristic X-rays.
- **Synchrotron**: circular particle accelerator — produces extremely intense X-rays for research.

#### Detection:
- **Photographic film**: darkened by X-ray exposure (historical method).
- **Scintillator + photodetector**: X-rays excite a crystal (e.g., CsI) which emits visible light, detected by a CCD.
- **Direct flat-panel detector**: amorphous selenium converts X-rays directly to charge — modern digital X-ray.
- **Geiger-Müller tube**: detects ionising radiation including X-rays.

#### Applications:
- Medical imaging (bone fractures, chest X-rays, CT scans).
- Airport security scanners.
- Industrial non-destructive testing (checking welds, casting defects).
- X-ray astronomy (observing black holes, neutron stars).

---

### Type 7: Gamma Rays
**Frequency range:** Above 30 EHz
**Wavelength range:** Below 0.01 nm (below 10 pm)

The highest-energy EM radiation. Produced by nuclear reactions and radioactive decay — not by electronic circuits.

#### Key Behaviour:
- Extremely penetrating — requires thick lead or concrete shielding.
- Highly ionising — destroys biological tissue at high doses, kills cancer cells at therapeutic doses.
- Carries enough energy to convert into matter (pair production: γ → electron + positron).
- Distinguished from X-rays by origin, not wavelength: X-rays come from electron transitions, gamma rays come from nuclear transitions.

#### Sources:
- Radioactive decay of unstable nuclei (e.g., Cobalt-60, Iodine-131).
- Nuclear fission and fusion reactions.
- Cosmic sources: pulsars, black holes, gamma-ray bursts.
- Pair annihilation (positron + electron → 2 gamma photons at 511 keV).

#### Detection:
- **Scintillation counter**: gamma ray hits NaI crystal, produces light flash, detected by photomultiplier.
- **Geiger counter**: gamma ionises gas in a tube, creating a measurable current pulse.
- **Semiconductor detector** (germanium): highest energy resolution — used in nuclear spectroscopy.

#### Applications:
- Cancer radiotherapy (targeted gamma beams kill tumours).
- Gamma sterilisation of medical equipment.
- PET scan (positron emission tomography): body emits gamma pairs, detected to create 3D images.
- Nuclear power plant safety monitoring.
- Food irradiation to kill bacteria and extend shelf life.

---

## Complete Spectrum Comparison Table

| Type        | Frequency Range     | Wavelength Range | Energy/Photon   | Penetrates | Key Source (Electronic)        |
|-------------|---------------------|------------------|-----------------|------------|-------------------------------|
| Radio       | 3 Hz – 300 GHz      | 100,000 km – 1mm | < 1.2 μeV       | Most materials | Oscillator + antenna        |
| Microwave   | 300 MHz – 300 GHz   | 1 m – 1 mm       | 1.2 μeV – 1.2 meV | Walls, clothing | Magnetron, Gunn diode, MMIC |
| Infrared    | 300 GHz – 430 THz   | 1 mm – 700 nm    | 1.2 meV – 1.8 eV | Thin materials | IR LED, laser diode, thermal |
| Visible     | 430 THz – 770 THz   | 700 nm – 380 nm  | 1.8 eV – 3.2 eV  | Glass, water | LED, laser diode, incandescent |
| Ultraviolet | 770 THz – 30 PHz    | 380 nm – 10 nm   | 3.2 eV – 124 eV  | Thin materials (UV-A) | UV LED, mercury lamp |
| X-ray       | 30 PHz – 30 EHz     | 10 nm – 0.01 nm  | 124 eV – 124 keV | Tissue, thin metals | X-ray tube               |
| Gamma ray   | > 30 EHz            | < 0.01 nm        | > 124 keV        | Dense lead, thick concrete | Radioactive decay    |

---

## How EM Waves Interact With Matter

Understanding this is critical for electronics and RF design.

### Reflection
Wave bounces off a surface. Metals are excellent reflectors — used in antenna reflectors, satellite dishes, Faraday cages (which block EM by reflecting it back).

### Refraction
Wave changes direction when passing between materials of different density. Lenses (optical, microwave) use this to focus waves.

### Absorption
Material converts wave energy to heat. Microwave ovens exploit water absorption at 2.45 GHz. Carbon fibre absorbs radar (used in stealth aircraft). Different materials absorb different frequency ranges.

### Diffraction
Waves bend around obstacles and spread after passing through openings. The effect is strongest when the wavelength is similar in size to the obstacle. Long-wavelength radio bends around buildings; visible light barely bends (creates sharp shadows).

### Interference
Two waves combining:
- **Constructive interference**: peaks align → stronger wave.
- **Destructive interference**: peak meets trough → waves cancel.
This causes **multipath fading** in WiFi and RC links — the direct signal and a reflected signal cancel each other at certain positions, causing dead spots.

### Doppler Effect
A moving source shifts the frequency of received waves. Used in:
- Speed radar (police radar gun, weather Doppler radar).
- Doppler lidar on drones for velocity sensing.
- Medical ultrasound (technically sound waves, but same principle).

---

## Photon Energy — The Quantum View

Each EM wave also behaves as a stream of particles called **photons**. Photon energy is:
```
E = h × f

  E = energy (Joules or electron-volts, eV)
  h = Planck's constant = 6.626 × 10⁻³⁴ J·s
  f = frequency (Hz)

Higher frequency → higher energy per photon
```

This is why:
- Radio photons are harmless (too low energy to affect atoms).
- Visible light can trigger chemical reactions (photography, photosynthesis).
- UV can break chemical bonds (sunburn, sterilisation).
- X-rays and gamma rays ionise atoms and damage DNA.

The threshold energy for ionising biological molecules is about **10 eV** — this puts the boundary between "non-ionising" and "ionising" radiation at the UV-B/UV-C boundary. Radio, microwave, IR, visible, and UV-A are **non-ionising**. UV-C, X-ray, and gamma are **ionising**.

---

## EM Waves in Electronic Systems — Practical Summary

| Phenomenon          | How It Affects Your Electronics                                          |
|---------------------|-------------------------------------------------------------------------|
| Multipath fading    | WiFi dead spots, RC signal dropouts — solved by antenna diversity        |
| Skin effect         | At high frequency, current only flows on conductor surface — affects PCB trace width |
| EM interference (EMI) | Motors generate broad RF noise — shield cables, add decoupling caps   |
| Faraday cage        | Metal enclosure blocks EM — why signals die inside cars, elevators       |
| Impedance matching  | Antenna and cable must be matched (50Ω) or power reflects back — damages PA |
| Near field vs far field | Within ~1 wavelength of an antenna, fields behave differently — affects antenna placement |
| Ground plane        | PCB copper ground plane reflects and shapes antenna radiation pattern    |

---

## Relevance to Your Drone Project

| Topic              | Application                                                             |
|--------------------|-------------------------------------------------------------------------|
| 2.4 GHz radio      | RC control link — UHF microwave, 12.5 cm wavelength                    |
| 5.8 GHz FPV        | Video downlink — SHF microwave, 5.2 cm wavelength                      |
| IR sensors         | Altitude hold, obstacle detection, optical flow on small drones         |
| Visible light LEDs | Orientation indicators (red/green), strobe for visibility               |
| EMI from motors    | Brushless motors + ESCs generate RF noise — use capacitors across motor terminals |
| Antenna placement  | Keep antenna away from carbon frame (carbon absorbs RF), orient for best pattern |
| Multipath fading   | Fly away from metal buildings — reflections cause signal dropouts        |
| Photon energy      | Why 2.4 GHz is safe for humans (non-ionising, too low energy) but can still heat tissue at high power |
