# GPS — How It Works, Fundamentals, and Electronics

GPS = **G**lobal **P**ositioning **S**ystem

GPS tells you exactly where you are on Earth — latitude, longitude, altitude — anywhere, anytime, in any weather, for free. It works by receiving radio signals from satellites and using precise timing to calculate position.

---

## The Core Idea

GPS does not transmit anything. Your GPS receiver is **passive** — it only listens. The satellites transmit, you receive.

Each GPS satellite continuously broadcasts:
1. **Its own precise location** (orbital position)
2. **The exact time** the signal was sent (from an atomic clock)

Your receiver compares the time the signal was sent vs the time it arrived. The difference × speed of light = distance to that satellite.

With distances to enough satellites, your position can be calculated by **trilateration**.

---

## Trilateration — How Position Is Computed

Trilateration is not the same as triangulation (which uses angles). Trilateration uses distances.

### 2D Example (simplified)
- You know you are 5 km from tower A → you are somewhere on a circle of radius 5 km around A.
- You know you are 4 km from tower B → you are on a circle of radius 4 km around B.
- Two circles intersect at exactly 2 points.
- Add a third distance → only 1 intersection point → your location is determined.

### 3D GPS Reality
In 3D space, distances define spheres instead of circles.
- Distance from satellite 1 → you are on a sphere.
- Distance from satellite 2 → intersection of 2 spheres = a circle.
- Distance from satellite 3 → intersection = 2 points.
- Distance from satellite 4 → resolves to 1 point (and also corrects clock error).

```
Satellite 1 ─────── distance d1 ─────────┐
Satellite 2 ─────── distance d2 ─────────┼──→ Your position (x, y, z)
Satellite 3 ─────── distance d3 ─────────┤
Satellite 4 ─────── distance d4 ─────────┘ (also corrects receiver clock)
```

**Why 4 satellites?** Your receiver's clock is cheap (not atomic) — it has a small error. The 4th satellite measurement allows the receiver to solve for the clock error simultaneously with the 3D position. This is why GPS needs a minimum of 4 satellites for a 3D fix.

---

## The Distance Calculation

```
Distance = (time of flight) × (speed of light)
d = Δt × c

  c = 299,792,458 m/s
  Δt = (time signal received) − (time signal sent)
```

**The precision required is extraordinary:**
- GPS signals take about 67 ms to travel from satellite to Earth (20,200 km altitude).
- A timing error of just 1 microsecond = 300 m position error.
- GPS satellites carry caesium and rubidium atomic clocks accurate to nanoseconds.

---

## GPS Signal Structure

GPS signals are transmitted on radio frequencies in the L-band:

| Signal  | Frequency     | Description                                      |
|---------|---------------|--------------------------------------------------|
| L1      | 1575.42 MHz   | Primary civilian signal — most GPS modules use this |
| L2      | 1227.60 MHz   | Military + dual-frequency civilian (L2C)         |
| L5      | 1176.45 MHz   | Modern high-accuracy civilian signal             |
| L1 + L5 | Both          | Dual-frequency — eliminates ionospheric error    |

### Signal Encoding — CDMA Spread Spectrum

All GPS satellites broadcast on the **same frequency** (L1 = 1575.42 MHz). They don't interfere because each satellite uses a **unique pseudo-random code (PRN code)** — a different pattern of 1s and 0s.

Your receiver generates a copy of each satellite's PRN code internally and slides it in time until it matches the incoming signal. The time shift = signal travel time. This is **CDMA (Code Division Multiple Access)** — same technique used in 3G mobile phones.

```
Satellite PRN code:   1011001101010011...
Receiver copy:              1011001101010011...  (shifted by travel time Δt)
                            ← Δt →
```

The signal is spread across a ~2 MHz bandwidth using **DSSS (Direct Sequence Spread Spectrum)** — very resistant to interference and jamming.

### Navigation Message
Modulated onto the PRN code is the **navigation message** — transmitted at 50 bps (very slow, takes 12.5 minutes to receive the full almanac):
- Satellite ephemeris (precise orbital parameters)
- Satellite health status
- Clock correction data
- Ionospheric correction model
- Almanac (coarse data for all satellites — used to predict which satellites are visible)

---

## Error Sources in GPS

GPS is never perfectly accurate — multiple factors introduce position errors.

| Error Source        | Typical Error  | Explanation                                                   |
|---------------------|----------------|---------------------------------------------------------------|
| Ionospheric delay   | 5 – 15 m       | Ionosphere slows signal — delay varies with solar activity    |
| Tropospheric delay  | 0.5 – 2 m      | Atmosphere (weather) affects signal speed                     |
| Satellite clock     | < 1 m          | Atomic clocks drift — corrected by ground stations            |
| Ephemeris error     | < 1 m          | Small orbital prediction errors                               |
| Multipath           | 1 – 10 m       | Signal reflects off buildings → arrives via two paths        |
| Receiver noise      | 0.2 – 1 m      | Electronic noise in the receiver circuit                      |
| Geometric dilution (GDOP)| Variable | Poorly spread satellites → amplified errors                  |
| **Total typical**   | **2 – 5 m CEP**| Civilian GPS accuracy on a clear day                          |

### DOP — Dilution of Precision
When all visible satellites are clustered in one part of the sky, geometry is poor — errors are amplified. DOP quantifies this:
- **PDOP** (Position DOP) < 2 is ideal.
- **HDOP** (Horizontal DOP) — most important for drone navigation.
- **VDOP** (Vertical DOP) — altitude is always less accurate than horizontal position.

---

## GPS Accuracy Levels

| Method              | Accuracy      | How                                                           |
|---------------------|---------------|---------------------------------------------------------------|
| Standard GPS (L1)   | 2 – 5 m       | Single-frequency, consumer receiver                          |
| Multi-constellation | 1 – 3 m       | Uses GPS + GLONASS + Galileo + BeiDou simultaneously         |
| SBAS (WAAS/EGNOS)   | < 1 m         | Differential correction from ground stations via satellite   |
| DGPS                | 0.3 – 1 m     | FM/UHF broadcast corrections from a known reference station  |
| RTK                 | 1 – 2 cm      | Real-Time Kinematic — carrier-phase measurement + base station|
| PPP                 | 5 – 10 cm     | Precise Point Positioning — satellite-based correction data  |

### RTK — How Centimetre Accuracy Works
Standard GPS measures the **code phase** (when the PRN code arrives). RTK also measures the **carrier phase** — the phase of the 1575.42 MHz carrier wave itself (wavelength = 19 cm). This is far more precise.

A **base station** at a known location computes the error in real-time and sends corrections to the **rover** (your drone's RTK receiver). The rover combines its measurements with the corrections to achieve 1–2 cm accuracy.

```
Base station (known position)
  → measures GPS error
  → sends correction data via radio link (or internet — NTRIP)
  → rover applies correction
  → result: 1–2 cm position accuracy
```

Used in: survey drones, precision agriculture, autonomous vehicles.

---

## GNSS Constellations

GPS is one of several **Global Navigation Satellite Systems (GNSS)**:

| System    | Country  | Satellites | Frequency(s) | Notes                          |
|-----------|----------|------------|--------------|--------------------------------|
| GPS       | USA      | 31 active  | L1, L2, L5  | Original system, global coverage|
| GLONASS   | Russia   | 24 active  | L1, L2      | Uses FDMA (each sat different freq) |
| Galileo   | EU       | 30 active  | E1, E5, E6  | Civilian-first design, very accurate |
| BeiDou    | China    | 35+ active | B1, B2, B3  | Regional + global coverage     |
| QZSS      | Japan    | 7 active   | L1, L2, L5  | Augments GPS over Asia-Pacific |
| NavIC     | India    | 8 active   | L5, S-band  | Regional (India + 1500 km)    |

Modern drone GPS modules (NEO-M8N and newer) receive **all four main constellations simultaneously** — more satellites visible = better accuracy and faster fix.

---

## GPS Cold Start vs Warm Start vs Hot Start

| Type       | Condition                                 | Time to Fix  |
|------------|-------------------------------------------|--------------|
| Cold start | No almanac, no last position known        | 30 – 90 sec  |
| Warm start | Has almanac but not recent position/time  | 20 – 45 sec  |
| Hot start  | Has almanac, recent position, recent time | 1 – 5 sec    |

The **almanac** (coarse data for all satellites) is valid for weeks. **Ephemeris** (precise orbital data per satellite) is valid for 2–4 hours. GPS modules store these in battery-backed RAM to enable hot start.

---

## GPS Receiver Architecture

### Block Diagram

```
GPS Antenna (patch or helical)
       │
  LNA (Low-Noise Amplifier) — first amplification, sets receiver sensitivity
       │
  Band-pass filter — passes 1575.42 MHz, rejects interference
       │
  Down-converter (Mixer + LO)
       │  IF signal (usually a few MHz)
  ADC — digitises the IF
       │
  Correlator bank (hardware) — one correlator per tracked satellite
       │  each correlator slides PRN code in time, finds the match
  Navigation processor (ARM core)
       │  solves the trilateration equations
  Position / velocity / time (PVT) output
       │
  UART/I2C/SPI → your microcontroller (NMEA sentences or binary protocol)
```

### The Correlator — Heart of the Receiver

The correlator is where the GPS magic happens. For each satellite being tracked:
- It generates a local copy of the satellite's PRN code.
- It multiplies the incoming signal by its local code and integrates (sums) the result.
- If the codes are aligned: high correlation (signal found).
- If misaligned: near-zero correlation (no signal).
- By searching different time offsets, it finds the exact delay = travel time.

Modern GPS receivers track 32–72+ channels simultaneously (one per satellite signal).

---

## GPS Output — NMEA Sentences

Most GPS modules output **NMEA 0183** sentences over UART at 9600 baud (default):

```
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

Fields:
  GPGGA = GPS Fix Data
  123519 = Time (12:35:19 UTC)
  4807.038,N = Latitude 48°07.038' North
  01131.000,E = Longitude 11°31.000' East
  1 = Fix quality (1=GPS, 2=DGPS, 4=RTK fixed, 5=RTK float)
  08 = Number of satellites tracked
  0.9 = HDOP
  545.4,M = Altitude above sea level (metres)
```

**Common NMEA sentences:**

| Sentence | Contains                                                  |
|----------|-----------------------------------------------------------|
| $GPGGA   | Fix data: lat, lon, altitude, satellites, HDOP            |
| $GPRMC   | Recommended minimum: lat, lon, speed, course, date/time  |
| $GPGSV   | Satellites in view: elevation, azimuth, SNR per satellite |
| $GPGSA   | DOP and active satellites                                 |
| $GPVTG   | Course and speed over ground                              |

**Binary protocols (faster, more data):**
- **UBX** (u-blox proprietary) — configure module, get raw measurements, set update rate above 1 Hz.
- **RTCM3** — differential correction data format for RTK.

---

## GPS Antenna Types

| Type          | Gain   | Size        | Use                                    |
|---------------|--------|-------------|----------------------------------------|
| Patch (ceramic) | 3–5 dBi | 15×15 mm to 25×25 mm | Most drone GPS modules — flat, light |
| Helical       | 3–5 dBi | Cylindrical | Better sky coverage than patch        |
| Active patch  | 20–28 dBi | Patch + built-in LNA | Compensates for cable loss     |
| Chip antenna  | 1–2 dBi | mm-scale   | Smartphones, fitness trackers — tiny but poor performance |

**Active vs passive:** Most standalone GPS modules (NEO-6M, NEO-M8N) include a built-in LNA just after the antenna input — equivalent to an active patch. Some drone GPS modules include the patch antenna and LNA together with a ground plane for best performance.

---

## GPS Module Comparison

| Module        | Chip         | Constellations       | Update Rate | Accuracy | RTK  | Use                     |
|---------------|--------------|----------------------|-------------|----------|------|-------------------------|
| NEO-6M        | u-blox 6     | GPS only             | 5 Hz        | ~2.5 m   | No   | Basic, budget builds    |
| NEO-M8N       | u-blox M8    | GPS+GLONASS+Galileo  | 10 Hz       | ~2 m     | No   | Standard drone GPS      |
| NEO-M9N       | u-blox M9    | All 4 + QZSS         | 25 Hz       | ~1.5 m   | No   | Fast update, new builds |
| ZED-F9P       | u-blox F9    | All 4 (L1+L2)        | 20 Hz       | 1–2 cm   | Yes  | Survey, precision agri  |
| ATGM336H      | Zhongkewei   | GPS + BeiDou         | 10 Hz       | ~2.5 m   | No   | Budget alternative      |
| BN-880        | u-blox M8    | GPS+GLONASS + compass| 10 Hz       | ~2 m     | No   | Drone GPS+compass combo |

---

## Key Formulas

```
Distance to satellite:   d = Δt × c  (c = 3×10⁸ m/s)
Clock error solve:       4 unknowns (x, y, z, t) → minimum 4 satellites
Carrier wavelength:      λ = c / f = 3×10⁸ / 1.57542×10⁹ = 0.1903 m = 19 cm
Ionospheric delay (approx): δ ≈ 40.3 × TEC / f²  (TEC = Total Electron Content)
```

---

## Relevance to Your Drone Project

| Topic              | Application                                                           |
|--------------------|-----------------------------------------------------------------------|
| NEO-M8N module     | Standard GPS for autonomous waypoint flight, return-to-home          |
| HDOP monitoring    | Check HDOP < 2.0 before arming drone — reject poor geometry           |
| Satellite count    | Require ≥ 8 satellites before allowing GPS-assisted flight modes     |
| Hot start          | GPS module keeps battery — fast fix after power cycle                |
| UART + NMEA        | Read $GPRMC or $GPGGA sentences on ESP32 UART1 to get lat/lon        |
| UBX protocol       | Set update rate to 10 Hz (default 1 Hz) via UBX-CFG-RATE command    |
| Multipath          | Avoid flying near tall metal buildings — GPS jumps in position        |
| RTK drone          | ZED-F9P + ground station for centimetre precision mapping missions   |
| Compass module     | Many drone GPS modules (BN-880, M8N with compass) include QMC5883L  |
| Antenna placement  | Mount GPS high on a mast, away from ESCs and power wires — interference degrades fix |
