# Bluetooth — How It Works, Fundamentals, and Electronics

Bluetooth is a short-range wireless technology for exchanging data between devices over distances of a few centimetres to about 100 metres. It operates in the 2.4 GHz ISM band and uses frequency hopping to coexist with WiFi and other devices in the same band.

---

## The Core Idea

Bluetooth creates small, temporary networks called **piconets** — one **master** device coordinates up to 7 active **slave** devices. All devices in a piconet share the same hopping sequence, set by the master.

Unlike WiFi (which needs an Access Point), Bluetooth is **peer-to-peer** — any two Bluetooth devices can connect directly without infrastructure.

---

## Bluetooth Frequency Hopping

Bluetooth operates in the **2.400 – 2.4835 GHz ISM band**, divided into **79 channels** (1 MHz each, starting at 2402 MHz).

Rather than staying on one channel, Bluetooth **hops between all 79 channels** up to **1600 times per second** (Classic Bluetooth). This is **FHSS (Frequency Hopping Spread Spectrum)**:

```
Time →
Ch:  23 → 51 → 7 → 68 → 14 → 42 → 5 → 37 → ...  (pseudo-random sequence)
     ↑
Each slot = 625 μs (Classic) or 1.25 ms (BLE)
```

**Why frequency hopping?**
- Interference on one channel only destroys that one time slot.
- Nearby WiFi uses only 3 of the 79 channels — Bluetooth skips over them quickly.
- Difficult to intercept or jam (must know the hopping sequence).
- Multiple piconets in the same area use different hopping sequences → coexist.

Bluetooth and WiFi use **Adaptive Frequency Hopping (AFH)** — Bluetooth learns which channels are occupied by WiFi and avoids them.

---

## Bluetooth Versions and Families

There are two completely different Bluetooth technologies that share the name:

| Feature       | Classic Bluetooth (BR/EDR)     | Bluetooth Low Energy (BLE)         |
|---------------|-------------------------------|-------------------------------------|
| Also called   | Bluetooth Basic Rate / EDR    | Bluetooth Smart, BLE, BTLE          |
| Standard      | Bluetooth 1.0 – 5.x           | Introduced in Bluetooth 4.0 (2010)  |
| Channels      | 79 (1 MHz each)               | 40 (2 MHz each)                     |
| Hopping       | 1600 hops/sec                 | 37 advertising + 3 data channels   |
| Data rate     | 1 Mbps (BR), 2/3 Mbps (EDR)  | 125 kbps – 2 Mbps                  |
| Power         | ~30 mA active                 | < 15 mA active, μA sleep           |
| Latency       | ~100 ms connection setup      | ~3 ms connection setup             |
| Range         | ~10–100 m                     | ~10–100 m (up to 400 m BT5 long range) |
| Best for      | Audio streaming, file transfer | Sensors, wearables, beacons, IoT  |
| Phone support | All Android/iOS               | All modern Android/iOS             |

**You cannot directly connect Classic Bluetooth to BLE** — they are different protocols even though they share the same hardware radio.

---

## Classic Bluetooth — How It Works

### Profiles
Classic Bluetooth uses **profiles** — standardised sets of features for specific use cases:

| Profile | Name                              | Use                                     |
|---------|-----------------------------------|-----------------------------------------|
| A2DP    | Advanced Audio Distribution Profile | Stream stereo audio (headphones, speakers) |
| HFP     | Hands-Free Profile                | Phone calls via car/headset             |
| HSP     | Headset Profile                   | Simple headset audio                    |
| SPP     | Serial Port Profile               | Virtual serial cable (used by HC-05)    |
| HID     | Human Interface Device            | Keyboards, mice, game controllers       |
| AVRCP   | Audio/Video Remote Control Profile| Play/pause/skip on BT speakers          |
| OPP     | Object Push Profile               | Send files between devices              |
| PAN     | Personal Area Network             | Internet sharing over Bluetooth         |

**HC-05 and HC-06 modules use SPP** — they appear to your MCU as a UART connection.

### Pairing Process (Classic)
```
Device A (master)        Device B (slave)
       │                        │
  Inquiry (discover)            │
       │──────────────────────►│
       │◄─────────────────────│ FHS packet (address, clock)
       │                        │
  Paging (connect)              │
       │──────────────────────►│
       │◄─────────────────────│ Page response
       │                        │
  Authentication (PIN/SSP):     │
  - Legacy: enter same PIN on both devices
  - SSP: numeric comparison or just works
       │                        │
  Encrypted connection established
```

**SSP (Secure Simple Pairing)** uses Elliptic Curve Diffie-Hellman (ECDH) key exchange — devices don't need a PIN, but user confirms a 6-digit number matches on both screens.

---

## Bluetooth Low Energy (BLE) — How It Works

BLE is a completely different architecture from Classic Bluetooth, designed for devices that run for months or years on a coin cell battery.

### BLE Channel Structure

BLE uses **40 channels** of 2 MHz each:
- **3 advertising channels** (37, 38, 39) — at 2402, 2426, and 2480 MHz (placed in WiFi channel gaps).
- **37 data channels** — used for actual data transfer once connected.

```
2.4 GHz band with BLE advertising channels marked:

WiFi Ch 1           WiFi Ch 6           WiFi Ch 11
  ↓                    ↓                     ↓
[─────────────────────────────────────────────────]
       ↑                    ↑                      ↑
    BLE ch 37           BLE ch 38             BLE ch 39
   (2402 MHz)          (2426 MHz)            (2480 MHz)
```

The 3 advertising channels are deliberately placed to avoid WiFi channel overlap — BLE devices can always advertise even when WiFi is active.

### BLE Roles

| Role       | Description                                                         |
|------------|---------------------------------------------------------------------|
| Advertiser | Broadcasts packets on advertising channels — can be discovered      |
| Scanner    | Listens on advertising channels — discovers advertisers            |
| Central    | Initiates a connection (like master) — typically phone/tablet      |
| Peripheral | Accepts a connection (like slave) — typically sensor/device        |
| Broadcaster| Advertises data with no intention to connect (beacon)              |
| Observer   | Scans for broadcasts, never connects                               |

### BLE Connection Process

```
Peripheral (sensor/device)          Central (phone/computer)
       │                                    │
  Advertise on ch 37, 38, 39               │
  every 100 ms (advertising interval)      │
       │──── ADV_IND packet ──────────────►│
       │                              Scan response
       │◄──── SCAN_REQ ───────────────────│ (optional — get more info)
       │──── SCAN_RSP ─────────────────►  │
       │                                   │
       │◄──── CONNECT_IND ────────────────│ (connection request)
       │                                   │
  Switch to data channels                  │
  Hop using connection parameters          │
       │◄══════ data channel ════════════► │
```

Once connected, BLE uses the 37 data channels with its own hopping pattern — now only 37 channels are used, not 79 like Classic.

### GATT — How BLE Data Is Organised

BLE data is structured using **GATT (Generic Attribute Profile)**:

```
Device (GATT Server)
  └── Service: Heart Rate (UUID: 0x180D)
        ├── Characteristic: Heart Rate Measurement (UUID: 0x2A37)
        │     ├── Value: [flags][heart rate value]
        │     └── Descriptor: Client Characteristic Config (enable notify)
        └── Characteristic: Body Sensor Location (UUID: 0x2A38)
              └── Value: "Wrist"

  └── Service: Battery (UUID: 0x180F)
        └── Characteristic: Battery Level (UUID: 0x2A19)
              └── Value: 85 (percent)
```

- **Service**: a group of related data (like a feature).
- **Characteristic**: a single data item with a UUID, value, and permissions.
- **UUID**: 16-bit (assigned by Bluetooth SIG) or 128-bit (custom).
- **Descriptor**: metadata about a characteristic (e.g., enable/disable notifications).

**BLE data operations:**
| Operation   | Direction         | Description                                          |
|-------------|-------------------|------------------------------------------------------|
| Read        | Central → Peripheral → Central | Central requests value, peripheral responds |
| Write       | Central → Peripheral | Central sends value to peripheral              |
| Notify      | Peripheral → Central | Peripheral pushes data when it changes (central enabled it) |
| Indicate    | Peripheral → Central | Same as notify but requires ACK                |
| Broadcast   | Peripheral → Everyone | Advertise data without connecting             |

### BLE Advertising Data Format

An advertiser can include data in its advertising packets without anyone connecting:

```
ADV_IND packet payload (max 31 bytes):
  └── AD Structure 1: Flags (type 0x01) → BLE general discoverable, no classic BT
  └── AD Structure 2: Complete Local Name (type 0x09) → "ESP32 Drone"
  └── AD Structure 3: Manufacturer Specific (type 0xFF) → your custom data
  └── AD Structure 4: Service UUID (type 0x07) → advertises available services

Extended advertising (BT5): up to 255 bytes
```

**Beacons** use advertising-only (no connection): iBeacon (Apple), Eddystone (Google), AltBeacon — broadcast a fixed payload with UUID/major/minor identifiers. Used in: asset tracking, indoor navigation, proximity marketing.

---

## Bluetooth 5 — Key Improvements

| Feature          | BT 4.x (BLE)    | BT 5.x             | Benefit                              |
|------------------|-----------------|--------------------|--------------------------------------|
| Max data rate    | 1 Mbps          | 2 Mbps (2M PHY)    | Faster transfer                      |
| Long range       | ~40 m           | ~400 m (Coded PHY) | Drone telemetry at distance          |
| Advertising      | 31 bytes        | 255 bytes          | More beacon data                     |
| Advertising sets | 1               | Multiple           | Different ads to different devices   |
| Audio            | No native       | LE Audio (LC3)     | BT5.2+ — lower latency, better quality audio than A2DP |

**Coded PHY (long range):** BT5 introduces forward error correction (FEC) coding. The LE Coded PHY sends each bit 2× (S=2) or 8× (S=8) redundantly:
- S=2: 500 kbps effective, ~4× longer range than 1M PHY.
- S=8: 125 kbps effective, ~8× longer range — up to 400 m open air.

ESP32 supports BT5 advertising extensions but not full BT5 connections — check specific variant (ESP32-C3/S3 support more BT5 features).

---

## Bluetooth Audio

### Classic: SBC / AAC / aptX / LDAC
Audio over Classic Bluetooth (A2DP profile) uses codecs to compress audio:

| Codec | Bitrate   | Latency   | Quality    | Support              |
|-------|-----------|-----------|------------|----------------------|
| SBC   | ~320 kbps | ~150 ms   | Good       | Mandatory — all BT devices |
| AAC   | ~250 kbps | ~120 ms   | Good       | Apple devices        |
| aptX  | ~352 kbps | ~40 ms    | Better     | Qualcomm chips       |
| aptX HD| ~576 kbps| ~40 ms   | Hi-Res     | Qualcomm chips       |
| LDAC  | ~990 kbps | ~30 ms    | Hi-Res     | Sony, Android 8+     |
| aptX LL| ~352 kbps| < 40 ms  | Low latency| Gaming headsets      |

### BLE Audio (BT 5.2+): LC3 Codec
- New **LC3 (Low Complexity Communication Codec)** — better quality than SBC at lower bitrate.
- Enables **True Wireless Stereo (TWS)** — left and right earbuds each receive independent BLE streams.
- **Auracast broadcast audio** — one source streams to unlimited BLE audio receivers simultaneously (e.g., silent disco, public address systems).

---

## Bluetooth Mesh

BLE 5 introduced **Bluetooth Mesh** — a many-to-many network topology:

```
Normal BLE: Star (one central, many peripherals)

BLE Mesh:
  Node ── Node ── Node ── Node
   │        │               │
  Node     Node            Node
                            │
                           Node ── [Internet gateway]
```

- Messages flood through the network — each node relays messages it hasn't seen before.
- Up to thousands of nodes.
- Used in: smart building lighting (Philips Hue), industrial sensor networks, smart home.

---

## Bluetooth Receiver Architecture

```
Antenna
   │
RF switch (share antenna between TX and RX)
   │
LNA (Low-Noise Amplifier)
   │
Mixer + PLL (down-converts 2.4 GHz to baseband — direct conversion)
   │
I/Q ADC (in-phase and quadrature — two samples per symbol)
   │
Digital baseband:
  - GFSK demodulator (Classic) or GFSK/QPSK/8DPSK demodulator
  - FEC decoder (BLE Coded PHY)
  - Whitening / de-whitening (scrambles data to avoid long runs of 0s/1s)
  - CRC check (24-bit CRC — catches transmission errors)
  - Link Layer controller (handles hopping, timing, ACK, retries)
   │
HCI (Host-Controller Interface) — separates radio from protocol stack
   │
Host stack (L2CAP, ATT, GATT, SMP, GAP)
   │
Application (your code)
```

### GFSK Modulation (Classic Bluetooth and BLE advertising)

Bluetooth uses **GFSK (Gaussian Frequency Shift Keying)**:
- Binary 1 → carrier frequency + Δf (frequency deviation ~160 kHz for BLE)
- Binary 0 → carrier frequency − Δf
- **Gaussian filter** smooths the frequency transitions — reduces bandwidth, reduces adjacent channel interference.

```
Data:     1    0    1    1    0    0    1
Freq:  ─╥─╨─╥─╨─╥─╥─╨─╨─╥─  (smooth transitions due to Gaussian filter)
```

---

## Key Bluetooth Chips and Modules

| Chip / Module    | Manufacturer  | Type            | Interface | Key Features                        |
|------------------|---------------|-----------------|-----------|-------------------------------------|
| HC-05            | Unknown/CSR   | Classic BT 2.0  | UART      | SPP serial bridge, AT commands, master+slave |
| HC-06            | Unknown/CSR   | Classic BT 2.0  | UART      | SPP serial bridge, slave only       |
| HM-10            | JNHuaMao/TI CC2541 | BLE 4.0    | UART      | BLE serial bridge, iOS compatible   |
| HM-11            | JNHuaMao      | BLE 4.0         | UART      | Smaller HM-10                       |
| nRF52832         | Nordic Semi   | BT 5.0 BLE      | SPI/I2C/UART | Very popular, Adafruit Feather, keyboards |
| nRF52840         | Nordic Semi   | BT 5.0 + 802.15.4 | USB    | USB native, Zigbee/Thread capable  |
| nRF5340          | Nordic Semi   | BT 5.3 dual-core | —        | Separate net + app cores            |
| CC2640           | TI            | BLE 4.2         | SPI/I2C   | SimpleLink — very low power         |
| CC2652           | TI            | BT 5.2 + 15.4  | —         | Multi-protocol SoC                  |
| ESP32 built-in   | Espressif     | BT 4.2 + BLE   | N/A       | Dual-mode, fully integrated with CPU|
| ESP32-C3         | Espressif     | BLE 5.0         | N/A       | BLE 5 only (no Classic BT)          |
| CYBLE-416045     | Infineon/Cypress | BLE 5.0      | —         | PSoC 6 BLE module                   |
| DA14531          | Dialog        | BLE 5.1         | SPI/UART  | Smallest/lowest power BLE chip      |

---

## Bluetooth vs WiFi vs Other Protocols

| Feature         | Classic BT  | BLE          | WiFi (2.4G)  | Zigbee       | Z-Wave       |
|-----------------|-------------|--------------|--------------|--------------|--------------|
| Frequency       | 2.4 GHz     | 2.4 GHz      | 2.4 / 5 GHz  | 2.4 GHz      | 868/915 MHz  |
| Range           | 10–100 m    | 10–100 m     | 20–150 m     | 10–100 m     | 30–100 m     |
| Max data rate   | 3 Mbps      | 2 Mbps       | 600+ Mbps    | 250 kbps     | 100 kbps     |
| Power           | Medium      | Very low     | High         | Low          | Very low     |
| Topology        | Piconet/star| Star / mesh  | Star (AP)    | Mesh         | Mesh         |
| Latency         | ~100 ms     | 3–6 ms       | 10–50 ms     | 15–30 ms     | 10 ms        |
| Best for        | Audio, HID  | Sensors, IoT | High data    | Smart home   | Smart home   |

---

## Key Formulas and Numbers

```
Classic BT channels:    79 (1 MHz each, 2402–2480 MHz)
BLE channels:           40 (2 MHz each) = 37 data + 3 advertising
Hopping rate:           1600 hops/sec (Classic) / ~50 hops/sec (BLE connection interval)
Classic slot duration:  625 μs
BLE connection interval: 7.5 ms – 4 sec (configurable)
BLE advertising interval: 20 ms – 10.24 sec
Max TX power:           +20 dBm (class 1), +4 dBm (class 2), 0 dBm (class 3)
BLE RX sensitivity:     ~−97 dBm (1M PHY), ~−103 dBm (Coded S=8)
BLE max payload:        251 bytes per PDU (DLE — Data Length Extension)
GATT max MTU:           23 bytes default, up to 512 bytes negotiated
```

---

## Glossary

| Term        | Meaning                                                                   |
|-------------|---------------------------------------------------------------------------|
| Piconet     | Bluetooth network of 1 master + up to 7 active slaves                    |
| Scatternet  | Multiple overlapping piconets                                             |
| AFH         | Adaptive Frequency Hopping — avoids channels used by WiFi                |
| BR          | Basic Rate — 1 Mbps Classic Bluetooth                                    |
| EDR         | Enhanced Data Rate — 2 or 3 Mbps Classic Bluetooth                      |
| GFSK        | Gaussian Frequency Shift Keying — modulation used by BLE and Classic BT  |
| GATT        | Generic Attribute Profile — defines how BLE data is structured            |
| ATT         | Attribute Protocol — the low-level protocol under GATT                   |
| GAP         | Generic Access Profile — controls advertising and connections            |
| SMP         | Security Manager Protocol — handles pairing and key generation           |
| UUID        | Universally Unique Identifier — names services and characteristics        |
| MTU         | Maximum Transmission Unit — max bytes in one ATT packet                  |
| DLE         | Data Length Extension (BT 4.2) — increased MTU from 23 to 251 bytes     |
| PHY         | Physical Layer — 1M, 2M, Coded S=2, Coded S=8 (BLE 5)                  |
| Coded PHY   | Forward-error-corrected PHY for long range at the cost of data rate      |
| Isochronous | BT 5.2 — time-synchronized streams for audio (LE Audio)                 |
| CIS         | Connected Isochronous Stream — BLE audio between connected devices       |
| BIS         | Broadcast Isochronous Stream — BLE audio broadcast (Auracast)            |
| LC3         | Low Complexity Communication Codec — BLE Audio codec                     |
| HCI         | Host-Controller Interface — separates radio hardware from protocol stack  |
| L2CAP       | Logical Link Control — segmentation and reassembly layer                 |
| SPP         | Serial Port Profile — virtual UART over Classic Bluetooth                |

---

## Relevance to Your Drone Project

| Topic                  | Application                                                          |
|------------------------|----------------------------------------------------------------------|
| HC-05 / ESP32 BT       | Configure drone over Bluetooth from Android app (SPP)               |
| BLE + phone app        | Read telemetry (battery %, altitude, GPS lock) on phone without WiFi association |
| ESP32 BLE advertising  | Drone broadcasts its status as BLE beacon — ground station scans without connecting |
| BLE GATT service       | Custom service: Characteristic = throttle/yaw/pitch/roll written by phone |
| Classic BT audio       | Not useful for drone directly — but same ESP32 can stream audio for ground station alerts |
| AFH coexistence        | ESP32 BT and your 2.4 GHz RC remote can coexist — AFH avoids occupied channels |
| BLE power              | BLE on ESP32 draws very little when advertising — good for always-on telemetry |
| nRF52840               | Alternative to ESP32 for BLE-only drone telemetry — more BT5 features, lower power |
| Coded PHY (BT5)        | Long-range BLE on ESP32-C3/S3 for extended telemetry range (~400 m) |
| BLE OTA                | Update drone firmware over BLE — no WiFi AP needed                  |
