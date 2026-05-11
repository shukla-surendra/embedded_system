# Modulation — How Data Is Carried Over a Wave

Modulation is the process of embedding information onto a carrier wave so it can be transmitted through air, cable, or fibre. At the receiver, demodulation extracts the original information back out.

This is the single most important concept in all of wireless communication. Every system — AM radio, WiFi, GPS, 4G, Bluetooth, FPV drones, satellite links — uses some form of modulation.

---

## The Core Idea

A carrier wave on its own carries zero information. It is just a pure sine wave repeating forever:

```
Carrier wave (no data):
  1│  /\    /\    /\    /\    /\
   │ /  \  /  \  /  \  /  \  /  \
  0│/    \/    \/    \/    \/    \/
   └────────────────────────────────→ time
   Frequency = fixed, Amplitude = fixed, Phase = fixed
```

A sine wave has exactly three properties that can be varied:

| Property    | Symbol | What it means                                   |
|-------------|--------|-------------------------------------------------|
| Amplitude   | A      | Height of the wave (signal strength)            |
| Frequency   | f      | How many cycles per second (Hz)                 |
| Phase       | φ      | Where in its cycle the wave starts (degrees)    |

**Modulation = deliberately changing one or more of these properties in sync with the data being sent.**

```
General carrier wave formula:
  s(t) = A(t) × cos(2π × f(t) × t + φ(t))

  Vary A(t)  → Amplitude Modulation (AM)
  Vary f(t)  → Frequency Modulation (FM)
  Vary φ(t)  → Phase Modulation (PM)
  Vary all three → advanced digital modulation
```

---

## Part 1: Analog Modulation

Analog modulation varies the carrier continuously in proportion to an analog input signal (voice, music, video).

---

### AM — Amplitude Modulation

**How it works:**
The amplitude (height) of the carrier wave is made to follow the shape of the message signal.

```
Message signal (audio):
  1│  /\        /\
   │ /  \      /  \
  0│/    \    /    \____
   └──────────────────────→ time

Carrier wave:
  1│ /\/\/\/\/\/\/\/\/\/\
  0│────────────────────
 -1│ \/\/\/\/\/\/\/\/\/\/
   └──────────────────────→ time

AM output (carrier amplitude follows message):
  1│  /\/\    /\/\
   │ /    \  /    \
  0│/      \/      \____
 -1│
   └──────────────────────→ time
   ↑ envelope matches message signal shape
```

**Mathematical expression:**
```
s(t) = [1 + m × msg(t)] × A × cos(2πfct)

  fc  = carrier frequency
  msg(t) = normalised message signal (−1 to +1)
  m   = modulation index (0 = no modulation, 1 = full modulation)
  A   = carrier amplitude
```

**Modulation index (m):**
- m < 1: under-modulated — safe, some power wasted in carrier.
- m = 1: 100% modulation — maximum efficiency without distortion.
- m > 1: over-modulated — envelope inverts → severe distortion at receiver.

**Bandwidth:**
```
AM bandwidth = 2 × fmax (highest frequency in message signal)
Example: Audio up to 5 kHz → AM channel needs 10 kHz bandwidth
```

**Receiver (envelope detector):**
A diode + RC filter extracts the envelope (message) from the AM signal — one of the simplest receivers ever built.

```
AM signal → [Diode] → [RC filter] → message signal recovered
```

**Variants:**
| Variant | Name                      | Description                                       |
|---------|---------------------------|---------------------------------------------------|
| AM (DSB-FC) | Double Sideband Full Carrier | Standard broadcast AM — carrier always present |
| DSB-SC | Double Sideband Suppressed Carrier | Carrier removed — more efficient          |
| SSB    | Single Sideband           | One sideband removed too — half the bandwidth    |
| VSB    | Vestigial Sideband        | One sideband partially filtered — used in TV     |

**SSB (Single Sideband):** Used in HF amateur radio and long-distance communication — halves the bandwidth and eliminates carrier power waste. Voice sounds nasal (Darth Vader effect) without carrier reinsertion.

**Used in:** AM broadcast radio (530–1700 kHz), aircraft HF, some old TV standards.

---

### FM — Frequency Modulation

**How it works:**
The frequency of the carrier deviates from its rest frequency in proportion to the message signal amplitude.

```
Message signal:    high ──── low ──── high
Carrier frequency: fast ──── slow ──── fast

FM output:
  │/\/\/\/\/\/\  /\  /\  /\/\/\/\/\/\
  │             \/  \/
  └──────────────────────────────────→ time
   ↑ waves closer = higher freq    ↑ waves spread = lower freq
```

**Mathematical expression:**
```
s(t) = A × cos(2πfct + 2π × kf × ∫msg(t)dt)

  kf = frequency sensitivity (Hz per unit of message amplitude)
  Δf = peak frequency deviation = kf × max|msg(t)|
```

**Modulation index (β):**
```
β = Δf / fmax

  Δf   = peak frequency deviation
  fmax = highest frequency in the message signal

FM Radio (broadcast): Δf = 75 kHz, fmax = 15 kHz → β = 5
```

**Carson's rule for bandwidth:**
```
FM bandwidth ≈ 2 × (Δf + fmax) = 2 × fmax × (β + 1)

FM Radio: 2 × (75 kHz + 15 kHz) = 180 kHz per channel
(Stations are spaced 200 kHz apart to leave a guard band)
```

**Why FM sounds better than AM:**
- Noise and interference typically affect amplitude — not frequency.
- FM receiver ignores amplitude variations (limiters remove them before detection).
- Result: static-free audio in strong signal conditions.
- **Capture effect:** If two stations are on the same frequency, the stronger one completely captures the FM receiver (weaker one is suppressed). AM would produce a mix of both.

**FM demodulator (PLL-based):**
```
FM signal → PLL (Phase-Locked Loop) → VCO tracks instantaneous frequency
         → VCO control voltage = message signal
```

**Used in:** FM broadcast radio (87.5–108 MHz), analog FPV drone video, police/fire radio, some satellite audio.

---

### PM — Phase Modulation

The phase of the carrier shifts in proportion to the message signal.

```
Message: 0 → 0 → 1 → 0 → 1 → 1
Phase:   0°  0°  180° 0°  180° 180°
```

PM and FM are closely related — a PM signal looks like an FM signal. The difference is what is varied:
- FM: frequency proportional to message amplitude.
- PM: phase proportional to message amplitude (→ frequency proportional to message derivative).

PM is the basis for all modern digital phase-based modulation schemes.

---

## Part 2: Digital Modulation — Representing Bits as Waves

Digital modulation maps discrete bit patterns onto changes in the carrier's properties. Instead of continuously varying amplitude/frequency/phase, there are a fixed number of distinct states — each state represents a specific bit pattern.

**Symbol vs bit:**
- A **symbol** is one transmission event — one change of state.
- Each symbol carries one or more bits.
- **Baud rate** (symbols/sec) ≠ **bit rate** (bits/sec).

```
Bit rate = Baud rate × bits per symbol

Example:
  QPSK: 4 symbols, each symbol = 2 bits
  Baud rate = 1 million symbols/sec → bit rate = 2 Mbps
```

---

### OOK — On-Off Keying

The simplest digital modulation — carrier is either ON (1) or OFF (0).

```
Data:    1    0    1    1    0    1    0    0
Output:  ████ ░░░░ ████ ████ ░░░░ ████ ░░░░ ░░░░
         ↑ carrier on  ↑ carrier off
```

**Used in:** IR TV remotes (38 kHz carrier, OOK), cheap 433 MHz RF modules, simple RFID, early telegraph (Morse code is OOK).

---

### ASK — Amplitude Shift Keying

Multiple amplitude levels instead of just ON/OFF.

```
2-ASK (same as OOK): two amplitudes (0 and A)
4-ASK: four amplitudes (0, A/3, 2A/3, A) — each symbol = 2 bits
```

**Used in:** ADSL (combined with PSK as QAM), optical fibre (OOK is a form of ASK), some RFID.

**Weakness:** Amplitude is easily corrupted by noise and fading — ASK alone is rarely used in wireless systems. QAM (combined amplitude + phase) is used instead.

---

### FSK — Frequency Shift Keying

Different frequencies represent different bit states.

```
Binary FSK (2-FSK):
  Bit 1 → carrier at f1 (e.g., 1200 Hz)
  Bit 0 → carrier at f0 (e.g., 2200 Hz)

Output:
  ─/\/\/\/\/\─────────/\/\/\─────/\/\/\/\/\─
   f1 = "1"    f0 = "0"    f1 = "1"
```

**Variants:**
| Variant | Name                      | Description                                   |
|---------|---------------------------|-----------------------------------------------|
| 2-FSK   | Binary FSK                | 1 bit per symbol                              |
| 4-FSK   | 4-level FSK               | 2 bits per symbol (4 distinct frequencies)   |
| GFSK    | Gaussian FSK              | Smooth frequency transitions (Bluetooth)     |
| MSK     | Minimum Shift Keying      | FSK where frequency separation = 1/(2T) — minimum to maintain orthogonality |
| GMSK    | Gaussian MSK              | MSK with Gaussian filter — used in GSM       |

**Bandwidth of FSK:**
```
Approximate bandwidth ≈ 2 × Δf + 2 × bitrate
where Δf = frequency separation between tones
```

**GFSK — used in Bluetooth Classic and BLE:**
A Gaussian low-pass filter smooths the transition between frequencies before modulation:
- Without Gaussian filter: abrupt frequency changes → wide sidelobes → interferes with adjacent channels.
- With Gaussian filter: smooth transitions → narrower spectrum → fits in 1 or 2 MHz channel.

**Used in:** Bluetooth (GFSK), old dial-up modems (Bell 202: FSK at 1200/2200 Hz), pager networks, some UHF radios.

---

### PSK — Phase Shift Keying

The phase of the carrier changes to represent data. Amplitude stays constant.

#### BPSK — Binary Phase Shift Keying
Two phases: 0° = bit 1, 180° = bit 0. One bit per symbol.

```
Data:    1      0      1      1      0
Phase:   0°    180°    0°     0°    180°

Output:  /\/\/\ ─\/\/\ /\/\/\ /\/\/\ ─\/\/\
          ↑ same    ↑ inverted    ↑ inverted
```

**Key property:** Most noise-resistant PSK — requires the worst SNR to corrupt. Used where signal is weak (GPS, deep space, satellite). 1 bit per symbol so data rate is low.

#### QPSK — Quadrature Phase Shift Keying
Four phases: 45°, 135°, 225°, 315°. Two bits per symbol.

```
Symbol  →  Phase
  00    →   45°
  01    →  135°
  11    →  225°
  10    →  315°
```

**Constellation diagram (I/Q plane):**
```
           Q
           │
    01 ×   │   × 00
           │
───────────┼──────────── I
           │
    11 ×   │   × 10
           │
```

Each dot (constellation point) is one symbol. The receiver measures the phase of the incoming signal and decides which point it is closest to.

**Why I and Q?**
- **I (In-phase)**: the component aligned with the carrier cosine.
- **Q (Quadrature)**: the component aligned with the carrier sine (90° shifted).
- Any combination of amplitude and phase can be expressed as a point in the I/Q plane.

```
s(t) = I × cos(2πfct) − Q × sin(2πfct)
     = A × cos(2πfct + φ)

I = A × cos(φ)
Q = A × sin(φ)
```

This is why software-defined radios (SDR) always deal in I/Q samples — they capture the full information about the signal.

#### 8PSK — 8-Phase Shift Keying
Eight phases (every 45°). Three bits per symbol.

Used in DVB-S2 (satellite TV), EDGE (2.5G mobile data upgrade to GSM).

#### DPSK — Differential PSK
Phase change relative to previous symbol (not absolute).
- 180° change = 1, no change = 0 (DBPSK).
- No need for absolute phase reference at receiver — easier to implement.
- Used in: some WiFi (802.11b), DECT cordless phones, Zigbee.

---

### QAM — Quadrature Amplitude Modulation

Combines amplitude AND phase modulation together. The constellation has multiple amplitude levels at multiple phases.

**Concept:**
- Vary both I and Q independently.
- Each symbol is a unique (I, Q) point on the constellation.
- More points → more bits per symbol → higher data rate.
- More points → points closer together → easier to confuse by noise → needs better SNR.

#### 16-QAM
16 points arranged in a 4×4 grid. 4 bits per symbol.

```
Q
│
× × × ×    (4 rows)
× × × ×
× × × ×
× × × ×
└──────────── I
```

#### 64-QAM
64 points (8×8 grid). 6 bits per symbol. Used in WiFi 802.11n/ac, LTE.

#### 256-QAM
256 points (16×16 grid). 8 bits per symbol. Used in WiFi 802.11ac/ax, DOCSIS (cable internet).

#### 1024-QAM
1024 points. 10 bits per symbol. WiFi 6 (802.11ax). Requires SNR > 35 dB.

#### 4096-QAM
4096 points. 12 bits per symbol. WiFi 7 (802.11be). Requires near-perfect channel conditions.

**QAM comparison:**
```
Modulation   Bits/symbol  Required SNR  Spectral efficiency
BPSK         1            6 dB          1 bit/s/Hz
QPSK         2            12 dB         2 bit/s/Hz
16-QAM       4            16 dB         4 bit/s/Hz
64-QAM       6            22 dB         6 bit/s/Hz
256-QAM      8            28 dB         8 bit/s/Hz
1024-QAM     10           35 dB         10 bit/s/Hz
4096-QAM     12           42 dB         12 bit/s/Hz
```

**The fundamental trade-off:** More bits per symbol = faster data rate but needs stronger signal (higher SNR). This is why WiFi automatically drops to lower QAM when you move further from the router.

**Adaptive modulation:** Modern systems (WiFi, LTE, 5G) continuously measure SNR and switch modulation order to always use the highest order that the current channel can support — this is called **Link Adaptation** or **AMC (Adaptive Modulation and Coding)**.

---

## Part 3: Multi-Carrier Modulation

All the modulations above use a single carrier frequency. Multi-carrier modulation splits the channel into many sub-carriers and modulates each independently.

### OFDM — Orthogonal Frequency Division Multiplexing

Used in: WiFi (802.11a/g/n/ac/ax), 4G LTE, 5G NR, DVB-T, ADSL, DAB radio, digital FPV links.

**The problem OFDM solves — multipath fading:**
```
Transmitter → direct path → Receiver
           → reflected path (bounces off wall) → Receiver (delayed)

The direct and reflected waves arrive at different times and can cancel each other
at certain frequencies — a "frequency-selective fade" destroys data on those frequencies.
```

**How OFDM works:**
Instead of one wide channel, divide it into many narrow sub-carriers. Each sub-carrier is so narrow that it experiences flat fading (either all good or all bad — no selective cancellation within a sub-carrier):

```
Wideband channel (20 MHz):
  ─────────────────────────────── one wide signal (selective fading possible)

OFDM (52 sub-carriers in 20 MHz):
  │││││││││││││││││││││││││││││││││││││││││││││││││││││
  ↑ each sub-carrier is ~300 kHz wide — flat fading only
  If a few sub-carriers fade, the rest survive
```

**Orthogonality:**
The sub-carriers are spaced so that each one has exactly zero contribution at the peak of all others — they do not interfere despite being close together:

```
Sub-carrier spacing = 1 / symbol duration

For WiFi 802.11n:
  Symbol duration = 3.2 μs → sub-carrier spacing = 312.5 kHz
  20 MHz / 312.5 kHz = 64 sub-carriers (52 data, 4 pilot, 8 guard)
```

**IFFT/FFT — the mathematical heart of OFDM:**
- **Transmitter**: uses an **IFFT (Inverse Fast Fourier Transform)** to convert frequency-domain symbols (one per sub-carrier) into one time-domain OFDM symbol.
- **Receiver**: uses an **FFT** to convert the received time-domain signal back to frequency-domain symbols.

```
Transmitter:
  Data bits → QAM mapper → {X0, X1, X2, ... X63}  (one symbol per sub-carrier)
                          → IFFT → x[n] (time domain signal) → transmit

Receiver:
  Received signal → FFT → {Y0, Y1, Y2, ... Y63} → QAM demapper → data bits
```

**Cyclic prefix:**
A short copy of the end of each OFDM symbol is prepended to the start. This creates a guard interval that absorbs the delay spread of multipath echoes — the FFT window is always clear of intersymbol interference.

```
Symbol: [guard prefix │────────── data ──────────]
                      ↑
             Copy of end of data
             Absorbs up to max_delay multipath echoes
```

For WiFi 802.11n:
- Symbol duration: 3.2 μs
- Guard interval (cyclic prefix): 0.8 μs (short) or 1.6 μs (long)
- Total symbol time: 4.0 μs

**OFDM in 4G LTE:**
- Sub-carrier spacing: 15 kHz (wider than WiFi's 312.5 kHz? No — WiFi is 312.5 kHz, LTE 15 kHz)
- Actually LTE: 15 kHz sub-carrier spacing.
- Resource block = 12 sub-carriers × 0.5 ms slot = the smallest LTE scheduling unit.

### OFDMA — Orthogonal Frequency Division Multiple Access

Extension of OFDM for multiple simultaneous users:

```
OFDM: all sub-carriers go to ONE user at a time.

OFDMA: sub-carriers are divided among MULTIPLE users simultaneously.

Frequency →
[User1][User1][User2][User3][User3][User3][User2][User1]
  sub-carriers allocated per user, per time slot
```

Used in: **WiFi 6 (802.11ax)**, 4G LTE downlink, 5G NR. Allows a WiFi 6 router to serve multiple devices simultaneously instead of one at a time — very important when many IoT devices are active.

### SC-FDMA — Single Carrier FDMA

Used in 4G LTE **uplink** (phone to tower):
- Same sub-carrier allocation as OFDMA.
- But each user's sub-carriers carry a single-carrier signal — not OFDM.
- Lower **PAPR (Peak to Average Power Ratio)** than OFDM — phone battery lasts longer.
- OFDM has high PAPR because many sub-carriers can add up in phase → large peaks → power amplifier must be backed off.

---

## Part 4: Spread Spectrum Techniques

Spread spectrum deliberately spreads the signal across a much wider bandwidth than the data requires. This looks like noise to anyone not knowing the code, and is resistant to jamming.

### DSSS — Direct Sequence Spread Spectrum

**How it works:**
- Take the data bit stream (slow).
- XOR it with a **pseudo-random chipping code** running at a much higher rate (chip rate).
- Transmit the result — spreading the signal across a wide bandwidth.

```
Data bit:     1  1  1  1  1  1  1  1  (1 bit — lasting 8 chip periods)
Chipping code: 1  0  1  1  0  1  0  0  (8 chips per bit — pseudo-random)
XOR result:   0  1  0  0  1  0  1  1  (transmitted — looks random)

Bandwidth expands:
  Data rate:  1 Mbps × bandwidth = 1 MHz
  Chip rate: 11 Mbps × bandwidth = 11 MHz (802.11b uses 11 Mcps)
```

**At the receiver:**
- Correlate received signal with same chipping code.
- Only the signal using that code produces a strong correlation output.
- Random noise and other signals do not correlate → appear as low-level noise.
- The despreading **processing gain** = chip rate / data rate:

```
Processing gain (dB) = 10 × log10(chip rate / data rate)

802.11b DSSS: 10 × log10(11 Mcps / 1 Mbps) = 10.4 dB
GPS L1 C/A:   10 × log10(1.023 Mcps / 50 bps) = 43 dB
Military GPS P(Y): 10 × log10(10.23 Mcps / 50 bps) = 53 dB
```

**The 43 dB GPS processing gain** means the GPS signal can be buried 43 dB below the noise floor and still be recovered — this is why GPS signals are received at 1/20,000,000,000th of a watt.

**CDMA (Code Division Multiple Access):**
Multiple users can share the same frequency band simultaneously if each uses a different, orthogonal chipping code:

```
User 1 data × Code1 → transmitted
User 2 data × Code2 → transmitted  (same frequency, same time)
User 3 data × Code3 → transmitted

Receiver for User 1:
  Received = User1×Code1 + User2×Code2 + User3×Code3
  × Code1 → User1×(Code1×Code1) + User2×(Code2×Code1) + User3×(Code3×Code1)
           = User1×1 + User2×0 + User3×0  (orthogonal codes)
           = User1 data  ← perfectly separated
```

Used in: GPS (each satellite has its own PRN code), 3G mobile (WCDMA/cdma2000), 802.11b WiFi.

---

### FHSS — Frequency Hopping Spread Spectrum

**How it works:**
Instead of spreading on one wide channel, the transmitter rapidly hops between many narrow channels in a pseudo-random sequence known only to transmitter and receiver.

```
Time →
Ch23: ██          ██
Ch51:     ██              ██
Ch 7:         ██    ██
Ch68:                 ██      ██

The signal visits each channel briefly, then moves on.
To an observer, each channel shows only a brief burst.
```

**Slow FHSS:** Hop rate < symbol rate — multiple symbols per hop.
**Fast FHSS:** Hop rate > symbol rate — multiple hops per symbol.

```
Bluetooth Classic: 1600 hops/sec — 79 channels — 1 MHz each
Military SINCGARS: 111 hops/sec — 2320 channels
FHSS RC remote (2.4 GHz): ~50 hops/sec — 79 channels
```

**Anti-jam advantage:**
A jammer trying to follow FHSS must:
- Know the hopping sequence (impossible without the crypto key), OR
- Jam the entire band simultaneously (barrage jamming — needs enormous power).

**Capture window:**
Even if a jammer hits one hop, only that hop's data is lost. Error correction codes (FEC) recover the missing data.

**Used in:** Bluetooth, RC drone remotes, DECT cordless phones, SINCGARS, HAVE QUICK, 802.11b (also FHSS option before 802.11g standardised on DSSS/OFDM).

---

### CSS — Chirp Spread Spectrum

A swept-frequency spread spectrum technique — the signal sweeps from low to high frequency (or high to low) continuously.

```
Chirp (up-chirp):
  Frequency ↑
            │      /
            │    /
            │  /
            │/
            └────────────────→ time
```

**LoRa (Long Range)** uses CSS:
- The frequency sweep represents one symbol.
- Different starting points of the chirp = different symbol values.
- **Spreading factor (SF)**: SF7 to SF12 — higher SF = more processing gain = longer range but lower data rate.

```
SF7:  128 chips per symbol — shortest, fastest, least range
SF12: 4096 chips per symbol — longest, slowest, most range

LoRa processing gain at SF12:
  Processing gain = 10 × log10(4096) = 36 dB
```

**LoRa data rate vs range:**
| SF | Chips/symbol | Data rate (125 kHz BW) | Typical range |
|----|-------------|------------------------|---------------|
| 7  | 128         | 5469 bps              | ~1 km         |
| 8  | 256         | 3125 bps              | ~2 km         |
| 9  | 512         | 1758 bps              | ~4 km         |
| 10 | 1024        | 977 bps               | ~6 km         |
| 11 | 2048        | 537 bps               | ~10 km        |
| 12 | 4096        | 293 bps               | ~15 km        |

**Why CSS is hard to jam:** The energy is spread across the entire bandwidth over time — a narrowband jammer only interferes with a fraction of the chirp.

**Used in:** LoRa, IEEE 802.15.4a (UWB), some military ranging systems.

---

## Part 5: Advanced and Modern Modulation

### MIMO — Multiple Input Multiple Output

Not a modulation scheme per se, but a way to use multiple antennas to transmit independent data streams simultaneously on the same frequency:

```
Transmitter                   Receiver
  Ant1 ──stream1────────────── Ant1
  Ant2 ──stream2────────────── Ant2

Each antenna transmits a different stream (spatial multiplexing).
The receiver uses signal processing to separate the streams.

Capacity multiplied by number of streams (spatial layers).
2×2 MIMO → 2× capacity
4×4 MIMO → 4× capacity
```

**Beamforming (MIMO variant):**
Instead of spatial multiplexing, phase-shift the signals from all antennas to focus energy toward a specific user:

```
Transmitter antennas:
  Ant1 → phase 0°  ─────\
  Ant2 → phase 30° ──────→ constructive interference at user
  Ant3 → phase 60° ─────/

Other directions: destructive interference → less energy, less interference to other users.
```

Used in WiFi 6 (MU-MIMO beamforming), 5G massive MIMO (64–256 antennas).

### Massive MIMO (5G)

5G base stations use 64–256 antenna elements:
- Each element individually controlled in amplitude and phase.
- Simultaneous narrow beams formed to many users at once.
- **3D beamforming**: horizontal AND vertical steering (key for dense urban environments with tall buildings).
- Capacity increases proportionally to number of antennas.

### NOMA — Non-Orthogonal Multiple Access (5G)

Traditional multiple access (OFDMA) gives different sub-carriers to different users — they never share:

```
OFDMA: User1 gets sub-carriers 1-10, User2 gets 11-20. Never overlap.

NOMA: User1 and User2 share the SAME sub-carriers but at different power levels.
  Near user (strong channel): low power
  Far user (weak channel): high power

Receiver:
  Far user just decodes their high-power signal (near user's signal below noise).
  Near user decodes far user first (SIC — Successive Interference Cancellation),
  then subtracts it and decodes their own low-power signal.
```

**Advantage:** More users per unit of spectrum — important for massive IoT connectivity.

### PAPR — Peak to Average Power Ratio

A key challenge with OFDM:

```
OFDM = sum of many sub-carriers.
If all sub-carriers align in phase at one moment:
  Peak power = N² × average power  (N = number of sub-carriers)

For 64 sub-carriers: peak can be 64× the average.
```

High PAPR means the power amplifier must handle large peaks → PA must be linear over a wide range → lower efficiency → more heat, more battery drain.

**PAPR reduction techniques:**
- **Clipping**: hard-limit peaks (introduces distortion — out-of-band emissions).
- **PAPR reduction codes**: choose data bits that produce low-PAPR constellations.
- **SLM (Selected Mapping)**: try multiple phase rotations, pick the one with lowest PAPR.
- **DFT-spread OFDM (SC-FDMA)**: LTE uplink — converts to single carrier before IFFT → inherently low PAPR.

---

## Part 6: Error Correction — Protecting Data in Transit

Modulation gets the bits across. Error correction makes sure they arrive correctly even when some bits are corrupted by noise.

### Why Errors Happen
```
Transmitted: 1 0 1 1 0 0 1 0 1 1
Noise added: . . . ↑ . . . . . ↑
Received:    1 0 1 0 0 0 1 0 1 0  ← 2 bit errors
```

Any communication channel has a **BER (Bit Error Rate)** — the probability that any given bit is received incorrectly. WiFi at −90 dBm might have BER = 10⁻³ (1 in 1000 bits wrong). With 50 Mbps data rate that would be 50,000 errors per second — unacceptable.

Forward Error Correction (FEC) adds redundancy so the receiver can detect and fix errors without retransmission.

### Parity Bits (Simple FEC)
Add one extra bit per word:
- Even parity: extra bit set so total 1s in word is even.
- Detects 1-bit errors, cannot correct.
- Used in UART serial (7-bit data + 1 parity bit).

### Hamming Code
Adds multiple parity bits at specific positions — allows the receiver to both detect and correct a single-bit error:

```
4 data bits + 3 parity bits = 7-bit Hamming(7,4) code
Can correct 1-bit error, detect 2-bit error.
```

### Convolutional Codes
The encoder feeds data bits through a shift register with XOR taps — each output bit depends on current and several previous input bits:

```
Input bit → [delay] → [delay] → [delay]
               ↓          ↓          ↓
Output1 = XOR of input + delay1 + delay3
Output2 = XOR of input + delay2 + delay3
(rate 1/2 code — 2 output bits per 1 input bit)
```

The decoder (Viterbi algorithm) finds the most likely input sequence given the received bits — extremely powerful.

Used in: original 802.11 WiFi, LTE, satellite, GPS signal.

### Turbo Codes
Two convolutional encoders working in parallel with an interleaver between them. Iterative "turbo decoding" approaches the Shannon limit (theoretical maximum data rate for a channel).

Used in: 3G/4G LTE, deep space communication (Mars rovers).

### LDPC — Low Density Parity Check Codes
Sparse parity check matrix — extremely efficient decoding using belief propagation. Approaches Shannon limit.

Used in: WiFi 802.11n/ac/ax, DVB-S2 (satellite TV), 5G NR, 10GbE Ethernet.

### Polar Codes
Provably achieve channel capacity (Shannon limit) for certain channel types. Computationally efficient encoding and decoding.

Used in: **5G NR control channels** (the first 5G standardised use).

### Reed-Solomon Codes
Block code operating on symbols (groups of bits) rather than individual bits — corrects bursts of errors:

```
RS(255,223): 255 byte block, 223 data bytes, 32 parity bytes
Can correct up to 16 byte errors anywhere in the block.
```

Used in: CDs, DVDs, QR codes, RAID-6 storage, deep space probes, digital broadcast (DVB).

### FEC in Practice — Coding Rate and Coding Gain

```
Coding rate = data bits / total bits

Rate 1/2: half the bits are parity — maximum protection
Rate 2/3: 1/3 parity
Rate 3/4: 1/4 parity
Rate 5/6: 1/6 parity — minimum protection, maximum throughput

WiFi 802.11n MCS table (1 spatial stream, 20 MHz):
  MCS0:  BPSK,   rate 1/2  →  6.5 Mbps
  MCS1:  QPSK,   rate 1/2  →  13 Mbps
  MCS2:  QPSK,   rate 3/4  →  19.5 Mbps
  MCS3:  16-QAM, rate 1/2  →  26 Mbps
  MCS4:  16-QAM, rate 3/4  →  39 Mbps
  MCS5:  64-QAM, rate 2/3  →  52 Mbps
  MCS6:  64-QAM, rate 3/4  →  58.5 Mbps
  MCS7:  64-QAM, rate 5/6  →  65 Mbps  ← maximum, needs best SNR
```

---

## Part 7: Channel Capacity — The Shannon Limit

**Claude Shannon's fundamental theorem (1948):**

```
C = B × log2(1 + S/N)

  C = channel capacity (maximum error-free data rate, bits/sec)
  B = bandwidth (Hz)
  S/N = signal-to-noise ratio (linear, not dB)

Example: WiFi 20 MHz channel, SNR = 30 dB (= 1000 linear):
  C = 20×10⁶ × log2(1 + 1000)
  C = 20×10⁶ × 9.97
  C = 199 Mbps maximum theoretical rate
```

**Implications:**
- Doubling bandwidth doubles capacity (linear).
- Doubling SNR only adds log2(2) = 1 bit/s/Hz (logarithmic — diminishing returns).
- No coding scheme can exceed Shannon capacity — physics imposes a hard limit.
- Modern systems like LDPC and turbo codes operate within 0.1 dB of this limit.

**Shannon limit explains:**
- Why 5G uses massive MIMO and wide bandwidths — bandwidth scales linearly.
- Why WiFi gets slower far from the router — lower SNR → lower capacity.
- Why QAM order matters — each step doubles points, adding log2(2) = 1 bit/symbol.

---

## Part 8: Pulse Shaping — Preventing ISI

When symbols are transmitted back to back, each symbol's waveform extends in time and overlaps with the next — **Inter-Symbol Interference (ISI)**.

**Nyquist pulse shaping** designs the pulse so it has zero value at all other symbol sampling times:

```
Symbol 1:  ──────────────/\/\──────────────
                              ↑ = 1 at t=0, = 0 at t=T, 2T, 3T...
Symbol 2:      ──────────/\/\──────────────
                              ↑ = 0 at Symbol 1's sample time

No ISI at the sampling instants.
```

**Raised Cosine filter:** The standard Nyquist pulse shape. A **Roll-off factor (α)** controls the bandwidth:
- α = 0: minimum bandwidth (sinc pulse, infinite in time — impractical).
- α = 1: double bandwidth (very short in time — easy to implement).
- Typical: α = 0.2 to 0.5.

Applied as a **Root Raised Cosine (RRC)** split equally between transmitter and receiver.

**Used in:** QPSK/QAM systems, LTE, 5G, DOCSIS cable, VSAT satellite.

---

## Part 9: Modulation in Wired Systems

Modulation is not only for wireless — it appears in wired data systems too.

### NRZ — Non-Return to Zero
Simplest line code — voltage high = 1, voltage low = 0. No modulation, just voltage levels.
- Problem: long runs of 0s or 1s → no clock transitions → clock recovery fails.
- Used in: UART, SPI, I2C at short distances.

### Manchester Encoding
Each bit encoded as a transition: rising edge = 1, falling edge = 0 (or reverse).
- Always a transition in the middle of each bit → easy clock recovery.
- Used in: original 10BASE-T Ethernet, RFID.

### 8B/10B Encoding
Every 8 data bits encoded as 10 transmission bits:
- Guarantees enough transitions for clock recovery.
- Balances 1s and 0s (DC balance) — required for AC-coupled links.
- Used in: USB 3.0, SATA, PCIe Gen 1/2, HDMI, Fibre Channel.

### PAM4 — 4-Level Pulse Amplitude Modulation
Uses 4 voltage levels instead of 2 — each symbol carries 2 bits:

```
Levels:  +3, +1, −1, −3
Symbols: 11,  10,  01,  00

PAM4 doubles the bit rate for the same symbol rate.
```

Used in: **400GbE / 800GbE Ethernet**, PCIe Gen 5/6, NVMe over Fabrics, modern data centre interconnects. Replacing older NRZ at speeds where NRZ bandwidth becomes impractical.

### DMT — Discrete Multi-Tone (ADSL/VDSL)
OFDM applied to telephone copper wire:
- ADSL2+: 512 sub-carriers (tones), each up to 15 bits/symbol (QAM32768).
- Higher tones attenuate more on long copper runs → lower QAM at higher tones.
- Automatic: modem probes each tone, assigns the maximum QAM order it can support.
- **Bit loading**: different QAM order per sub-carrier → maximum total throughput.

---

## Part 10: Modulation in Optical Fibre

### Intensity Modulation / Direct Detection (IM/DD)
Simplest optical: LED or laser intensity switches on/off = OOK.
- Used in short-reach links (SFP modules, 1GbE, older 10GbE).

### Coherent Optical Modulation
Long-haul fibre (transoceanic cables, backbone networks) uses advanced coherent modulation:
- **DP-QPSK (Dual Polarisation QPSK):** Two orthogonal polarisations of light carry independent QPSK signals simultaneously — 4× BPSK capacity.
- **DP-16QAM, DP-64QAM:** Even higher orders for maximum spectral efficiency.
- Coherent receiver: local oscillator laser + 90° hybrid coupler → measures both I and Q of the optical signal.
- Current commercial systems: 800 Gbps per wavelength, 96 wavelengths per fibre (DWDM) = 76.8 Tbps per fibre pair.

---

## Part 11: Summary — Modulation Techniques Quick Reference

### By Application

| System             | Modulation           | Why                                         |
|--------------------|----------------------|---------------------------------------------|
| AM broadcast radio | AM (DSB-FC)          | Simple transmitter and receiver             |
| FM broadcast radio | FM (WBFM)            | Better noise immunity than AM               |
| Analog FPV drone   | FM                   | < 1 ms latency, simple                     |
| WiFi 802.11b       | DSSS + BPSK/QPSK     | First WiFi — spread spectrum for noise      |
| WiFi 802.11g/n/ac  | OFDM + 64/256-QAM    | Multipath immunity + high spectral efficiency |
| WiFi 802.11ax (6)  | OFDMA + 1024-QAM     | Multi-user, highest efficiency              |
| Bluetooth Classic  | GFSK                 | Simple, low power                           |
| BLE                | GFSK (1M/2M PHY)     | Low power                                   |
| GPS L1 C/A         | BPSK-DSSS            | 43 dB processing gain → works below noise   |
| GPS military P(Y)  | BPSK-DSSS (10.23 Mcps)| 53 dB processing gain → jam resistant      |
| LoRa               | CSS (chirp)          | Very long range, very low power             |
| 4G LTE downlink    | OFDMA + 64/256-QAM   | Efficient multi-user                        |
| 4G LTE uplink      | SC-FDMA + 64-QAM     | Low PAPR → battery life on phones           |
| 5G NR              | OFDMA + 256/1024-QAM | Massive MIMO + beamforming                 |
| SINCGARS           | FM + FHSS            | Anti-jam for military                       |
| Digital TV (DVB-T) | COFDM + 64-QAM       | NLOS robustness + high capacity             |
| Satellite (DVB-S2) | QPSK / 8PSK + LDPC   | Maximum power efficiency in space           |
| ADSL               | DMT (OFDM) + high-QAM| Adapts to each phone line's quality        |
| 400GbE Ethernet    | PAM4                 | Double data rate on same bandwidth          |
| Optical backbone   | DP-16QAM coherent    | Maximum spectral efficiency on fibre        |

---

## Part 12: Key Formulas — Quick Reference

```
Carrier wave:         s(t) = A×cos(2πft + φ)
AM signal:            s(t) = [1 + m×msg(t)] × A × cos(2πfct)
AM bandwidth:         BW = 2 × fmax
FM deviation ratio:   β = Δf / fmax
FM bandwidth (Carson): BW = 2(Δf + fmax)
Bit rate vs baud:     bit rate = baud rate × log2(M)  [M = number of symbols]
Shannon capacity:     C = B × log2(1 + SNR)
OFDM sub-carrier spacing: Δf = 1 / Tuseful
OFDM symbol time:     T = Tuseful + Tguard
Processing gain (DSSS): Gp (dB) = 10×log10(chip rate / bit rate)
LoRa chips/symbol:    2^SF  (SF = spreading factor)
QAM bits per symbol:  n = log2(M)  [M = constellation size]
Eb/N0 to SNR:         SNR = (Eb/N0) × (bit rate / bandwidth)
```

---

## Glossary

| Term     | Meaning                                                                         |
|----------|---------------------------------------------------------------------------------|
| Carrier  | High-frequency sine wave onto which data is modulated                           |
| Modulation | Varying a carrier property (amplitude, frequency, phase) to carry data       |
| Demodulation | Recovering the original data from the modulated carrier                    |
| Symbol   | One discrete transmission event — can carry multiple bits                       |
| Baud     | Symbols per second (not same as bits per second)                                |
| Bit rate | Bits per second = baud × bits per symbol                                        |
| SNR      | Signal-to-Noise Ratio — determines maximum usable modulation order              |
| BER      | Bit Error Rate — probability of a received bit being wrong                      |
| FEC      | Forward Error Correction — adds redundancy to fix errors without retransmission |
| I/Q      | In-phase and Quadrature — two orthogonal components that fully describe a signal|
| Constellation | Diagram of all possible symbol positions on the I/Q plane               |
| PAPR     | Peak to Average Power Ratio — how spiky the waveform is                        |
| ISI      | Inter-Symbol Interference — one symbol corrupting adjacent symbols              |
| Multipath | Signal arrives via multiple reflected paths — causes fading                   |
| Fading   | Signal strength variation due to multipath or movement                          |
| Diversity | Using multiple paths/antennas to combat fading                                |
| Spreading factor | LoRa parameter — higher SF = more processing gain, lower data rate    |
| Chip     | One bit of the spreading code — much faster than data bits                     |
| Processing gain | dB benefit from spread spectrum — signals survive below noise floor    |
| MCS      | Modulation and Coding Scheme — index defining QAM order + FEC rate             |
| AMC      | Adaptive Modulation and Coding — automatically select best MCS for channel     |
| OFDM     | Multiple sub-carriers in parallel — multipath immune                           |
| OFDMA    | OFDM with sub-carriers divided among multiple users                            |

---

## Relevance to Your Drone Project

| Topic               | Application                                                           |
|---------------------|-----------------------------------------------------------------------|
| GFSK (Bluetooth)    | HC-05 and ESP32 BT use GFSK — explains why signal is narrow-band     |
| FHSS (RC remote)    | Your 2.4 GHz RC link hops channels to avoid WiFi — FHSS in action    |
| OFDM (WiFi)         | ESP32 WiFi uses OFDM — multipath in rooms and obstacles doesn't destroy stream |
| DSSS processing gain| GPS works at 1/20 billionth of a watt because of 43 dB spread gain   |
| QAM + AMC (WiFi)    | ESP32 WiFi drops from 64-QAM to QPSK when flying far from AP — this is AMC |
| OFDMA (WiFi 6)      | Router can serve drone telemetry + laptop + phone simultaneously      |
| CSS / LoRa          | LoRa telemetry for very long range drone links — SF12 = 15 km        |
| Shannon limit       | Explains why doubling drone VTX power does not double range           |
| FM (analog FPV)     | FPV video transmitted as FM-modulated composite on 5.8 GHz           |
| LDPC (WiFi)         | 802.11n/ac LDPC FEC recovers data even when flying through marginal signal |
| I/Q samples         | SDR receivers (like RTL-SDR) output raw I/Q — understand this to decode any drone signal |
| BPSK + DSSS         | GPS chip inside your NEO-M8N module recovers position using exactly this |
