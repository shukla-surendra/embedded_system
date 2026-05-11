# WiFi — How It Works, Fundamentals, and Electronics

WiFi = **Wi**reless **Fi**delity (marketing name for IEEE 802.11 standards)

WiFi uses radio waves to create a wireless local area network (WLAN) — allowing devices to communicate without cables over distances of tens to hundreds of metres.

---

## The Core Idea

WiFi is a two-way radio communication system operating in unlicensed frequency bands (2.4 GHz and 5 GHz). An **Access Point (AP)** — usually a router — coordinates communication. **Stations (STA)** — phones, laptops, ESP32s — connect to the AP.

Unlike radar or GPS (one-way), WiFi is **full bidirectional** — devices both transmit and receive, sharing the same frequency channel using time-division rules.

---

## WiFi Frequency Bands

### 2.4 GHz Band (2.400 – 2.4835 GHz)
- 83.5 MHz of total spectrum.
- Divided into **14 channels** (1 MHz spacing), but channels are 20 MHz wide → they overlap.
- Only **3 non-overlapping channels** in most regions: **1, 6, and 11**.
- **Longer range** than 5 GHz — 2.4 GHz penetrates walls better.
- **More congested** — shared with Bluetooth, microwave ovens, RC drone remotes, baby monitors, Zigbee.

```
2.4 GHz channel layout (20 MHz channels):
Ch: 1    2    3    4    5    6    7    8    9    10   11   12   13
    ─────────────────────────────────────────────────────────────
    [  Ch1  ]                 [  Ch6  ]                 [ Ch11 ]
         ← These 3 don't overlap each other →
```

### 5 GHz Band (5.150 – 5.850 GHz)
- Much more spectrum — ~500 MHz available.
- 25+ non-overlapping 20 MHz channels (varies by country).
- Support for 40 MHz, 80 MHz, 160 MHz channel bonding (wider = faster).
- **Shorter range** — higher frequency absorbed more by walls.
- **Less congested** — fewer legacy devices use it.

### 6 GHz Band (WiFi 6E / WiFi 7)
- 5.925 – 7.125 GHz — 1200 MHz of new spectrum.
- 59 non-overlapping 20 MHz channels.
- Very low congestion (new band, new devices only).
- Short range (similar to 5 GHz).

---

## WiFi Standards — IEEE 802.11 History

| Standard  | Common Name | Year | Max Speed    | Bands      | Key Technology               |
|-----------|-------------|------|--------------|------------|------------------------------|
| 802.11b   | WiFi 1      | 1999 | 11 Mbps      | 2.4 GHz    | DSSS modulation              |
| 802.11a   | WiFi 2      | 1999 | 54 Mbps      | 5 GHz      | OFDM — first OFDM WiFi       |
| 802.11g   | WiFi 3      | 2003 | 54 Mbps      | 2.4 GHz    | OFDM on 2.4 GHz              |
| 802.11n   | WiFi 4      | 2009 | 600 Mbps     | 2.4 + 5 GHz| MIMO (up to 4 streams)       |
| 802.11ac  | WiFi 5      | 2013 | 3.5 Gbps     | 5 GHz only | MU-MIMO, 80/160 MHz channels |
| 802.11ax  | WiFi 6      | 2019 | 9.6 Gbps     | 2.4 + 5 GHz| OFDMA, MU-MIMO, TWT, BSS colouring |
| 802.11ax  | WiFi 6E     | 2021 | 9.6 Gbps     | 6 GHz added| Same as WiFi 6 + 6 GHz band  |
| 802.11be  | WiFi 7      | 2024 | 46 Gbps      | 2.4+5+6 GHz| MLO (multi-link), 320 MHz channels |

**ESP32 supports: 802.11 b/g/n (WiFi 4) on 2.4 GHz only.**

---

## How WiFi Transmits Data

### Step 1 — OFDM Modulation (Orthogonal Frequency Division Multiplexing)

Modern WiFi (802.11g/n/ac/ax) uses **OFDM** — a technique that splits the channel into many narrow sub-carriers transmitted in parallel.

```
Single-carrier (old):  ────────────────────── one wide stream
                                               (vulnerable to narrowband interference)

OFDM (modern):         │││││││││││││││││││││  52 sub-carriers in 20 MHz channel
                        ↑
                       Each sub-carrier carries a small piece of data
                       They are mathematically orthogonal — don't interfere
```

**Why OFDM is better:**
- Narrowband interference only destroys a few sub-carriers — most data gets through.
- Each sub-carrier is a slow, long symbol — resistant to multipath echoes.
- Sub-carriers right next to each other are perfectly spaced so they don't interfere (orthogonal).

A **20 MHz 802.11n channel** has:
- 52 data sub-carriers
- 4 pilot sub-carriers (for synchronisation)
- Each sub-carrier modulated at up to 64-QAM (6 bits/symbol) or 256-QAM (8 bits/symbol)

### Step 2 — QAM Modulation (Quadrature Amplitude Modulation)

Each OFDM sub-carrier carries data using QAM:

| Modulation | Bits per symbol | Requires (SNR) | Max throughput contribution |
|------------|-----------------|-----------------|----------------------------|
| BPSK       | 1               | Low SNR needed  | Lowest — used for management frames |
| QPSK       | 2               | Moderate        | |
| 16-QAM     | 4               | Good signal     | |
| 64-QAM     | 6               | Strong signal   | 802.11n/ac max             |
| 256-QAM    | 8               | Very strong     | 802.11ac/ax                |
| 1024-QAM   | 10              | Excellent signal| WiFi 6                     |
| 4096-QAM   | 12              | Near-perfect    | WiFi 7 only                |

**Higher order QAM = more data per transmission, but requires much better signal quality.** When you move away from the router, the device drops to lower QAM automatically — this is why WiFi speed decreases with distance.

### Step 3 — MIMO (Multiple Input Multiple Output)

WiFi uses multiple antennas to send and receive multiple data streams simultaneously:

```
Router (2 antennas)           Device (2 antennas)
   ─Ant1──────────────stream 1──────────Ant1─
   ─Ant2──────────────stream 2──────────Ant2─

Each stream is spatially independent → 2× throughput
```

- **802.11n**: up to 4×4 MIMO (4 antennas each side) = 4 spatial streams.
- **802.11ac**: up to 8×8 MIMO.
- **MU-MIMO** (Multi-User): router sends different streams to different devices simultaneously.

ESP32 has a single antenna — **1×1 MIMO (SISO)** — no spatial multiplexing.

### Step 4 — CSMA/CA (Channel Access)

WiFi is a **shared medium** — only one device can transmit at a time on a channel. Multiple devices coordinate using **CSMA/CA (Carrier Sense Multiple Access with Collision Avoidance)**:

```
Device wants to transmit:
  1. Listen — is the channel free?
  2. Yes → wait a random backoff time (avoids simultaneous transmission)
  3. Send RTS (Request to Send) to AP (optional for large frames)
  4. Receive CTS (Clear to Send) from AP
  5. Transmit data frame
  6. Wait for ACK (acknowledgment)
  7. If no ACK → assume collision → wait random time → retry

Channel busy → wait, sense again
```

This is fundamentally different from Ethernet (which uses CSMA/CD — Collision Detection) because you cannot detect collisions while transmitting over radio.

---

## WiFi Security

| Protocol  | Year | Encryption        | Status          | Notes                                  |
|-----------|------|-------------------|-----------------|----------------------------------------|
| WEP       | 1999 | RC4 (40-bit)      | Broken — never use | Cracked in minutes with freely available tools |
| WPA       | 2003 | TKIP (RC4)        | Deprecated      | Transitional — also weak              |
| WPA2-CCMP | 2004 | AES-128           | Current standard| PBKDF2 key derivation — strong        |
| WPA3      | 2018 | AES-192/256 + SAE | Recommended     | SAE replaces PSK handshake — forward secrecy |
| WPA2-Enterprise | — | 802.1X + RADIUS | Corporate use  | Each user has own credentials          |

**ESP32 supports WPA, WPA2-PSK, WPA2-Enterprise.**

### WPA2 4-Way Handshake (how WiFi login actually works)

```
1. AP → Client: ANonce (random number from AP)
2. Client → AP: SNonce + MIC (client proves it knows the password)
3. AP → Client: GTK (group temporal key) + MIC (AP confirms)
4. Client → AP: ACK

From both nonces + the password (via PBKDF2) → PTK (Pairwise Transient Key) derived
All subsequent data encrypted with PTK
```

The password is never sent — both sides independently derive the same key from the password. This is why WPA2 is hard to crack without the right password — an attacker must try all possible passwords (dictionary/brute-force attack).

---

## WiFi Operating Modes

| Mode             | Description                                                       | ESP32 Use                      |
|------------------|-------------------------------------------------------------------|--------------------------------|
| Station (STA)    | Device connects to existing AP                                    | ESP32 joins home WiFi          |
| Access Point (AP)| Device creates its own network                                    | ESP32 becomes a hotspot        |
| AP + STA         | Both simultaneously                                               | ESP32 bridges networks         |
| Monitor mode     | Receive all frames without being associated (packet sniffing)     | WiFi analysis tools            |
| ESP-NOW          | Direct peer-to-peer, no AP needed, 250 bytes/frame, very fast    | **Drone RC link, telemetry**   |

### ESP-NOW — Key for Drones
ESP-NOW is Espressif's proprietary protocol built on top of the 802.11 PHY layer:
- No router needed — direct ESP32-to-ESP32.
- Latency: < 1 ms (compared to 10–50 ms for normal WiFi).
- Range: ~200 m open air (same as normal WiFi).
- Up to 20 paired peers.
- One-way or two-way.
- **Used for drone remote control:** remote ESP32 sends joystick data, drone ESP32 receives and acts.

---

## WiFi Receiver Architecture (What's Inside a WiFi Chip)

```
Antenna
   │
RF Switch (TX/RX switching — not full duplex like cellular)
   │
LNA (Low-Noise Amplifier) — sets receiver sensitivity
   │
Mixer + Local Oscillator (PLL-based, synthesised)
   │  down-converts 2.4 GHz to baseband (0 Hz) — direct conversion architecture
ADC — samples in-phase (I) and quadrature (Q) components
   │
Digital baseband processor:
  - OFDM FFT — converts time-domain samples to sub-carrier signals
  - Channel estimation — corrects amplitude/phase per sub-carrier
  - QAM demodulation — extracts bits from each sub-carrier
  - FEC decoder — corrects bit errors (Viterbi or LDPC decoder)
  - MAC layer — handles CSMA/CA, ACK, fragmentation, security
   │
Host interface (SPI / SDIO / USB — how it talks to CPU)
   │
Application processor (ESP32 Xtensa core, or ARM in separate MCU)
```

### Direct Conversion vs Superheterodyne

Most modern WiFi chips use **direct conversion (zero-IF)**:
- Mix the 2.4 GHz signal directly down to baseband (0 Hz).
- Simpler, fewer components, easier to integrate on one chip.
- Challenge: DC offset and I/Q imbalance must be corrected in digital.

Older radios used superheterodyne (down to IF first, then to baseband) — now mainly only in high-performance radios.

---

## Key WiFi Chips and Modules

| Chip / Module  | Manufacturer   | Standard      | CPU          | Key Features                      |
|----------------|----------------|---------------|--------------|-----------------------------------|
| ESP8266        | Espressif      | 802.11 b/g/n  | Tensilica 80 MHz | WiFi only, basic IoT            |
| ESP32          | Espressif      | 802.11 b/g/n  | Xtensa 240 MHz | WiFi + BT 4.2, dual core        |
| ESP32-S3       | Espressif      | 802.11 b/g/n  | Xtensa 240 MHz | USB-OTG, AI acceleration        |
| ESP32-C3       | Espressif      | 802.11 b/g/n  | RISC-V 160 MHz | Cheap, small                    |
| ATWINC1500     | Microchip      | 802.11 b/g/n  | None (peripheral) | WiFi add-on chip for Arduino  |
| CC3200         | TI             | 802.11 b/g/n  | ARM M4 80 MHz | TI IoT SoC                      |
| BCM4329        | Broadcom       | 802.11 a/b/g/n| None         | Used in Raspberry Pi             |
| RTL8720DN      | Realtek        | 802.11 a/b/g/n| Cortex-M33   | Dual band — BW16 module         |
| QCA4020        | Qualcomm       | 802.11 a/b/g/n| ARM M4       | Thread + BLE too                 |

---

## WiFi Range and Power

### Factors Affecting Range
- **Transmit power**: ESP32 max ~+20 dBm (100 mW) — regulated by country.
- **Receiver sensitivity**: ESP32 ≈ −97 dBm at 1 Mbps (lower = better).
- **Path loss**: Signal weakens with distance — `FSPL = 20log(d) + 20log(f) + 20log(4π/c)`.
- **Obstacles**: Each wall reduces signal by 3–15 dB depending on material.
- **Interference**: Other 2.4 GHz devices raise the noise floor.

```
Free space path loss at 2.4 GHz:
  10 m:   60 dB loss
  100 m:  80 dB loss
  1000 m: 100 dB loss

ESP32 link budget:
  TX power:          +20 dBm
  RX sensitivity:    −97 dBm
  Total budget:      117 dB
  → Theoretical range (no obstacles): ~1000 m+
  → Practical range (indoors, obstacles): 20–100 m
```

### Power Modes (important for battery-powered drones/IoT)
| Mode           | Current Draw | Wake-up Time | Use                                    |
|----------------|-------------|--------------|----------------------------------------|
| Active TX      | 160–260 mA  | Immediate    | Transmitting data                      |
| Active RX      | 80–100 mA   | Immediate    | Receiving / associated to AP          |
| Modem sleep    | 20–30 mA    | < 1 ms       | CPU runs, WiFi sleeps between DTIM beacons |
| Light sleep    | 800 μA      | < 5 ms       | CPU suspended, WiFi wakes periodically|
| Deep sleep     | 10 μA       | ~300 ms      | Everything off except RTC — WiFi must full reconnect |

For a drone, WiFi is mostly used for **telemetry and configuration** (not real-time control) because the latency and power draw are too high for the control loop.

---

## Glossary

| Term        | Meaning                                                                    |
|-------------|----------------------------------------------------------------------------|
| SSID        | Network name (Service Set Identifier)                                      |
| BSSID       | AP's MAC address — uniquely identifies an AP                              |
| AP          | Access Point — the router/hub of a WiFi network                           |
| STA         | Station — any device connected to an AP                                   |
| OFDM        | Orthogonal Frequency Division Multiplexing — splits channel into sub-carriers |
| MIMO        | Multiple antennas sending/receiving simultaneously for higher throughput   |
| QAM         | Quadrature Amplitude Modulation — encodes bits in amplitude+phase          |
| MCS         | Modulation and Coding Scheme — index combining QAM + MIMO + coding rate   |
| RSSI        | Received Signal Strength Indicator — approximate received power in dBm    |
| SNR         | Signal-to-Noise Ratio — determines maximum usable MCS                     |
| DTIM        | Delivery Traffic Indication Message — AP announces buffered data for sleeping devices |
| Beacon      | Frame sent by AP every 100 ms announcing its presence                     |
| Probe       | Frame sent by STA scanning for networks                                   |
| CSMA/CA     | Medium access protocol — listen before transmit, random backoff            |
| WPA2        | WiFi Protected Access 2 — current encryption standard                     |
| AES-CCMP    | Encryption cipher used by WPA2                                            |
| PTK         | Pairwise Transient Key — per-session encryption key derived during handshake |
| BSS colouring| WiFi 6 feature — different networks mark their frames so devices ignore other networks' traffic |
| OFDMA       | WiFi 6 — multiple users share one OFDM symbol simultaneously (like LTE)  |
| TWT         | Target Wake Time — WiFi 6 feature, AP schedules wake times for IoT devices |

---

## Relevance to Your Drone Project

| Topic              | Application                                                           |
|--------------------|-----------------------------------------------------------------------|
| ESP-NOW protocol   | Best choice for ESP32 drone RC link — ~1 ms latency, 200 m range, no router needed |
| Station mode       | ESP32 on drone connects to phone hotspot for configuration/OTA update  |
| AP mode            | Drone ESP32 becomes its own network — phone connects directly         |
| RSSI monitoring    | Read WiFi RSSI in code — if drops below threshold, activate failsafe  |
| Channel selection  | Use channel 1, 6, or 11 — avoid interference with drone RC (also 2.4 GHz) |
| Power management   | Use modem sleep between telemetry packets to reduce current draw      |
| OTA update         | Update drone firmware over WiFi without USB cable                     |
| WebSocket / HTTP   | Stream telemetry (battery, altitude, GPS) to phone browser in real-time |
| WPA2 security      | Set a password on drone AP mode — prevent hijacking                   |
| Dual-band modules  | RTL8720DN (BW16) module gives 5 GHz WiFi on small board if 2.4 GHz too crowded |
