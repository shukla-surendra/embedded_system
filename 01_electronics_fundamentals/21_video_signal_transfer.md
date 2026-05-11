# Video Signal Transfer — Complete Guide to All Technologies

Video is a sequence of still images (frames) displayed fast enough that the human eye perceives motion. Getting those frames from a camera to a screen — whether across a cable, a room, or the world — involves a chain of technologies: capture, encoding, transmission, and display. This document covers all of them.

---

## Part 1: Fundamentals — What a Video Signal Actually Is

### Frames, Resolution, and Refresh Rate

| Term            | Meaning                                                             |
|-----------------|---------------------------------------------------------------------|
| Frame           | One complete still image                                            |
| Resolution      | Number of pixels per frame (width × height)                        |
| Frame rate (fps)| How many frames are displayed per second                           |
| Interlaced (i)  | Each frame split into two fields (odd lines, then even lines)      |
| Progressive (p) | Each frame drawn complete, top to bottom                           |
| Aspect ratio    | Width:Height ratio (16:9, 4:3, 21:9)                              |
| Bit depth       | Bits per colour channel (8-bit = 256 levels, 10-bit = 1024 levels) |
| Colour space    | Mathematical model defining how colour is encoded (RGB, YCbCr)    |

**Common resolutions:**

| Name    | Pixels         | Pixels Total | Notes                              |
|---------|----------------|--------------|------------------------------------|
| SD      | 720 × 480/576  | ~345K        | Standard definition, DVD           |
| 720p    | 1280 × 720     | 921K         | HD, most webcams                   |
| 1080p   | 1920 × 1080    | 2.07M        | Full HD — most FPV cameras         |
| 1440p   | 2560 × 1440    | 3.69M        | 2K / QHD                           |
| 4K UHD  | 3840 × 2160    | 8.29M        | Consumer 4K                        |
| 4K DCI  | 4096 × 2160    | 8.85M        | Cinema 4K                          |
| 8K      | 7680 × 4320    | 33.2M        | 8K broadcast / cinema              |

### Raw Bandwidth — Why Compression Is Necessary

```
Uncompressed 1080p60:
  1920 × 1080 pixels × 3 bytes (RGB) × 60 fps
  = 373 MB/s = 2.98 Gbps

Uncompressed 4K60:
  3840 × 2160 × 3 × 60
  = 1.49 GB/s = 11.9 Gbps

A USB 2.0 port can only do 60 MB/s — impossible without compression.
A standard WiFi (802.11n) link can do ~20 MB/s real-world — 1080p60 raw is 20× too much.
```

This is why every video transmission system either uses very wide bandwidth cables (HDMI, SDI) or compresses the video first.

### Colour Spaces

**RGB (Red, Green, Blue):**
- Used in computer graphics, displays, camera sensors.
- Each pixel stores R, G, B values.
- 3 × 8 bits = 24-bit colour = 16.7 million colours.

**YCbCr / YPbPr:**
- Y = luma (brightness), Cb/Pb = blue difference, Cr/Pr = red difference.
- Human eyes are more sensitive to brightness (Y) than colour (C).
- **Chroma subsampling** exploits this — store Y at full resolution, C at reduced resolution.

```
Chroma subsampling notation: J:a:b

4:4:4 — Full colour, no subsampling    (used in high-end production)
4:2:2 — Halve horizontal colour res   (professional video, HDMI 2.0)
4:2:0 — Halve both H and V colour res (broadcast TV, streaming, most cameras)
4:1:1 — Quarter horizontal colour res (used in some DV formats)

4:2:0 gives ~33% bandwidth saving with minimal visible quality loss.
```

---

## Part 2: Analog Video Standards

Before digital, all video was transmitted as an analog voltage waveform — the voltage at each moment represented the brightness of the corresponding pixel on that scan line.

### Composite Video (CVBS)

The oldest and simplest standard — **all video information on a single wire**:
- **Luma** (brightness): amplitude of the signal at each point in the scan line.
- **Chroma** (colour): a colour subcarrier at 3.58 MHz (NTSC) or 4.43 MHz (PAL) multiplexed onto the same signal.
- **Sync pulses**: short voltage drops that tell the display when each line and each frame begins.

```
Composite signal waveform (one scan line):
         ←─── Active video ────────────────────→
  0.7V ┌─────────────────────────────────────────┐
       │   Luma level varies with brightness     │
  0V ──┤                                         │──
−0.3V  └──────────────┘  ←── Horizontal sync pulse (−0.3V, 4.7 μs)
       │◄──── Blanking interval ────────────────►│
```

**NTSC** (North America, Japan): 525 lines, 29.97 fps, 3.58 MHz subcarrier.
**PAL** (Europe, Australia): 625 lines, 25 fps, 4.43 MHz subcarrier, better colour stability.
**SECAM** (France, Russia): 625 lines, 25 fps, FM colour encoding.

**Connector:** RCA (yellow). BNC connector in professional use.

**Limitation:** Luma and chroma are multiplexed on one wire — they interfere with each other → dot crawl, colour bleeding artefacts.

**Still used in:** FPV drone goggles analog video, old CCTV, retro gaming. NTSC/PAL FPV cameras on racing drones output composite video at 700+ TVL (TV Lines) resolution.

---

### S-Video (Separate Video / Y/C)

Improves on composite by separating luma (Y) and chroma (C) onto two separate wires — eliminates interference between them.

- **Y wire**: brightness + sync.
- **C wire**: colour subcarrier only.
- Still uses the same NTSC/PAL standards, just better separation.
- Connector: 4-pin mini-DIN.

---

### Component Video (YPbPr)

Splits video into **three separate analog wires**:
- **Y**: luma + sync.
- **Pb**: blue minus luma (B − Y).
- **Pr**: red minus luma (R − Y).

Green is mathematically derived — `G = Y − 0.299R − 0.114B`.

**Advantages:**
- No chroma subcarrier — no interference, better colour accuracy.
- Supports HD resolutions: 480i, 480p, 720p, 1080i, 1080p.
- Used in DVD players, PS2/PS3, HD broadcast equipment.

**Connectors:** 3× RCA (red, green, blue) or 3× BNC professional.

---

### CVBS in FPV Drones (Analog FPV)

Analog FPV systems use composite video (NTSC or PAL) for one key reason: **latency**.

```
Analog FPV latency:  < 1 ms (essentially zero — direct electrical transmission)
Digital FPV latency: 15–40 ms (encoding + transmission + decoding)
```

Racing drone pilots prefer analog FPV for this reason — milliseconds matter for obstacle avoidance at 150 km/h.

**Analog FPV chain:**
```
FPV Camera (CMOS/CCD) → NTSC/PAL composite signal
       │
Video Transmitter (VTX) — modulates composite onto 5.8 GHz RF carrier
       │  RF transmission through air
Video Receiver (VRX) — demodulates RF, outputs composite
       │
FPV Goggles or monitor — displays composite video
```

**5.8 GHz FPV channels:** 40 channels across 5.645 – 5.945 GHz, grouped into bands (A, B, E, F, R, L). Each channel is ~25 MHz wide.

**Video transmitter power levels:** 25 mW (indoor), 200 mW (typical outdoor), 600 mW (long range), up to 1000 mW+ (modified/illegal in many countries).

**Resolution comparison (analog FPV):**
- Standard FPV camera: 600–1000 TVL (TV Lines) in NTSC/PAL — looks roughly like 480p digital.
- TVL measures horizontal resolution — more TVL = sharper image.

---

## Part 3: Digital Video Interfaces (Wired)

### HDMI — High-Definition Multimedia Interface

HDMI carries digital video + audio + control signals on a single cable.

**Physical layer:**
- Uses **TMDS (Transition Minimised Differential Signalling)** — same as DVI.
- 3 data channels + 1 clock channel, each differential pair.
- Differential signalling: two wires carry inverted signals — noise affects both equally, cancelled on receive.

```
TMDS pair: ────+─────────── signal
           ────-─────────── inverted signal
                             ↑
Receiver sees the difference — immune to common-mode noise
```

**HDMI versions and bandwidth:**

| Version | Max Bandwidth | Max Resolution    | New Features                        |
|---------|---------------|-------------------|-------------------------------------|
| 1.0     | 4.95 Gbps     | 1080p60           | First version (2002)                |
| 1.3     | 10.2 Gbps     | 1440p120 / 4K30   | Deep colour (10/12-bit)             |
| 1.4     | 10.2 Gbps     | 4K30 / 4K@24      | ARC (Audio Return Channel), 3D     |
| 2.0     | 18 Gbps       | 4K60 / 1080p240   | HDR, 4:2:0 chroma, 32 audio channels |
| 2.0b    | 18 Gbps       | 4K60 HDR          | HDR10, Dolby Vision                 |
| 2.1     | 48 Gbps       | 8K60 / 4K120      | VRR, ALLM, eARC, DSC compression  |

**HDMI Connector types:**
- **Type A**: Standard — TVs, monitors, most devices.
- **Type C**: Mini HDMI — cameras, tablets.
- **Type D**: Micro HDMI — small cameras, Raspberry Pi, drones.

**CEC (Consumer Electronics Control):** Allows one remote to control multiple HDMI-connected devices via the same cable — single-wire serial bus.

**ARC / eARC:** Audio Return Channel — sends audio from TV back to soundbar on the same HDMI cable (instead of needing a separate audio cable).

---

### DisplayPort

Primarily for computer displays — designed by VESA.

| Version | Max Bandwidth | Max Resolution    | New Features                     |
|---------|---------------|-------------------|----------------------------------|
| 1.2     | 21.6 Gbps     | 4K60              | Multi-stream (daisy chain)       |
| 1.3     | 32.4 Gbps     | 5K60 / 4K120      |                                  |
| 1.4     | 32.4 Gbps     | 8K30 / 4K120 HDR  | DSC 1.2, HDR, FEC               |
| 2.0     | 77.37 Gbps    | 16K60 / 8K120     | USB4 tunnelling                  |

**USB-C / Thunderbolt DisplayPort Alt Mode:** DisplayPort signal sent over USB-C cable — why a USB-C cable can drive a 4K monitor.

**Mini DisplayPort:** Used on older Apple laptops, some graphics cards.

---

### DVI — Digital Visual Interface

Older digital video standard (1999) — still found on PC monitors and graphics cards.

| Type     | Channels | Bandwidth  | Max Resolution |
|----------|----------|------------|----------------|
| DVI-D Single Link | 1 | 4.95 Gbps | 1920×1200@60  |
| DVI-D Dual Link  | 2 | 9.9 Gbps  | 2560×1600@60  |
| DVI-I    | 1 or 2   | Same       | Also carries analog VGA signal |
| DVI-A    | —        | Analog only| VGA via DVI connector           |

DVI uses TMDS — same physical layer as HDMI 1.x. A passive DVI-to-HDMI adapter works (same signal, different connector).

---

### VGA — Video Graphics Array

Pure analog video — RGB + HSync + VSync on 15-pin D-sub connector. Maximum practical resolution ~2048×1536 before signal quality degrades. Still found on projectors and older monitors. No audio.

---

### SDI — Serial Digital Interface (Professional Broadcast)

The standard for professional video production — found in TV studios, broadcast trucks, cameras.

**Key advantage over HDMI/DisplayPort:** Uses standard **75Ω BNC coaxial cable** — the same cable used for RF. Runs up to 100+ metres on a single cable without active electronics.

| Standard  | Data Rate   | Max Resolution      | Max Cable Length |
|-----------|-------------|---------------------|------------------|
| SD-SDI    | 270 Mbps    | 576i / 480i         | ~300 m           |
| HD-SDI    | 1.485 Gbps  | 1080i / 720p        | ~100 m           |
| 3G-SDI    | 2.97 Gbps   | 1080p60             | ~100 m           |
| 6G-SDI    | 5.94 Gbps   | 2160p30 / 1080p120  | ~70 m            |
| 12G-SDI   | 11.88 Gbps  | 2160p60             | ~70 m            |
| 24G-SDI   | 23.76 Gbps  | 4K120 / 8K30        | ~40 m            |

**Why SDI over HDMI in broadcast:**
- Locking connectors (BNC) — cannot accidentally pull out.
- Much longer cable runs — HDMI maxes out at ~8 m without active cable.
- Includes embedded audio + timecode + metadata.
- Ultra-reliable — designed for 24/7 broadcast operation.

**SDI over fibre:** For very long runs (kilometres) — SDI signal converted to optical via SFP modules.

---

### CSI — Camera Serial Interface (Embedded Systems)

The standard interface between image sensors and application processors — used in smartphones, Raspberry Pi, drones.

**MIPI CSI-2 (most common):**
- Uses D-PHY (differential pairs) or C-PHY (3-wire trio signalling).
- 1–8 data lanes, each running at 80 Mbps to 4.5 Gbps.
- 4-lane CSI-2 at 2.5 Gbps per lane = 10 Gbps total — enough for 4K60 RAW.
- Very short range — within a device (ribbon cable, < 30 cm).

**Raspberry Pi camera uses MIPI CSI-2** — the flat ribbon cable from camera to Pi is a 2-lane CSI-2 bus.

**DVP (Digital Video Port) / Parallel DVP:**
- Older, simpler interface — 8 or 10 parallel data pins + sync pins.
- Lower bandwidth than CSI-2 — limited to ~10 MP.
- Used on OV2640, OV5640 cameras with ESP32-CAM, Arduino.

**ESP32-CAM OV2640 interface: DVP (parallel)** — 8 data pins, HREF, VSYNC, PCLK, XCLK.

---

### USB Video Class (UVC)

A USB standard that defines how webcams send video to a computer — any USB camera that implements UVC works without a driver on Windows, macOS, and Linux.

- **USB 2.0 UVC**: up to ~30 MB/s — limits to 1080p30 MJPEG or 720p30 uncompressed.
- **USB 3.0 UVC**: up to ~400 MB/s — 4K30 uncompressed possible.
- Camera compresses internally (MJPEG or H.264) to fit within USB bandwidth.

---

## Part 4: Video Compression — How to Make Video Small

Uncompressed 1080p60 is 2.98 Gbps. A YouTube stream at 1080p60 uses ~8 Mbps — a compression ratio of ~370:1. How?

### Intra-Frame Compression (Spatial)

**DCT (Discrete Cosine Transform):**
Used in JPEG and as a stage in all video codecs.

```
Image block (8×8 pixels):
  Raw: 64 brightness values (e.g., slowly changing background)
  DCT: Converts to 64 frequency coefficients
       Most energy is in low-frequency coefficients (gradual changes)
       High-frequency coefficients (fine detail) are small or zero
  Quantise: Divide coefficients by a quantisation matrix — small values become zero
  Entropy code: Run-length encode the zeros → very compact
```

Many zeros appear because natural images have large smooth regions — the DCT concentrates the energy into a few coefficients.

**Quantisation is where quality is lost** — larger quantisation step = more coefficients forced to zero = more compression but more artefacts (blockiness).

### Inter-Frame Compression (Temporal)

Video frames are highly similar to adjacent frames — most of the scene does not change. Inter-frame compression only encodes the **difference** between frames.

**Frame types:**
| Type  | Full Name         | What It Contains                              |
|-------|-------------------|-----------------------------------------------|
| I-frame | Intra-coded    | Complete frame — no reference to other frames |
| P-frame | Predictive     | Difference from the previous reference frame  |
| B-frame | Bidirectional  | Difference using both past and future frames  |

```
GOP (Group of Pictures):
I──P──B──P──B──P──B──B──P──B──B──I──...

I-frame: ~200 KB (full JPEG-like image)
P-frame: ~50 KB (only changed blocks + motion vectors)
B-frame: ~25 KB (uses past AND future frames → best compression)

Only I-frames allow random seek — that is why scrubbing video sometimes jumps to a keyframe.
```

**Motion Compensation:**
Instead of encoding a complete changed region, the encoder records a **motion vector** — "this block moved X pixels right, Y pixels down from the previous frame". Only the prediction error (residual) is encoded. Most blocks in a video frame can be described with a motion vector and a small residual.

```
Block in frame N:       Block in frame N+1:
  ┌───────────┐           ┌───────────┐
  │ car shape │  →motion→ │ car shape │ (moved 5px right)
  └───────────┘           └───────────┘

Encode: motion vector (5,0) + tiny residual (car changed slightly)
Instead of: 16×16 = 256 pixels of new data
```

---

## Part 5: Video Codecs — All Major Standards

A **codec** (Coder-Decoder) is the algorithm that compresses and decompresses video.

### MJPEG (Motion JPEG)
- Each frame is independently compressed as a JPEG.
- No inter-frame prediction — every frame is an I-frame.
- **Advantages:** Simple, low latency, easy random access, low CPU to decode.
- **Disadvantages:** Poor compression — 5–10× less efficient than H.264.
- **Used in:** Webcams (over USB), ESP32-CAM streaming, security cameras with simple processors, Raspberry Pi camera low-latency preview.

### H.264 / AVC (Advanced Video Coding) — MPEG-4 Part 10
The most widely used video codec in the world.

| Feature        | Detail                                                     |
|----------------|------------------------------------------------------------|
| Introduced     | 2003                                                       |
| Profiles       | Baseline, Main, High (quality/complexity trade-off)       |
| Compression    | ~1000:1 achievable for low-motion video                   |
| Typical bitrate| 1080p30: 4–8 Mbps / 4K30: 15–25 Mbps                    |
| Hardware decode| Almost every device built after 2008                      |
| Latency        | Baseline profile: low / High profile: several seconds     |
| Used in        | YouTube, Netflix, Blu-ray, CCTV, FPV digital systems     |

**How H.264 works:**
1. Divide frame into macroblocks (16×16 pixels).
2. For each macroblock: choose intra (spatial) or inter (motion) coding.
3. Apply motion compensation (inter) or intra prediction.
4. Apply 4×4 DCT to the residual.
5. Quantise DCT coefficients.
6. Entropy code with CABAC or CAVLC.
7. Encapsulate in NAL units (Network Abstraction Layer).

### H.265 / HEVC (High Efficiency Video Coding)
Successor to H.264 — **2× better compression at the same quality**.

| Feature        | Detail                                                     |
|----------------|------------------------------------------------------------|
| Introduced     | 2013                                                       |
| Coding unit    | CTU up to 64×64 (vs 16×16 macroblock in H.264)           |
| Compression    | 40–50% better than H.264 at same quality                  |
| Typical bitrate| 1080p30: 2–4 Mbps / 4K30: 8–15 Mbps                     |
| Hardware decode| Most devices built after 2015                             |
| Used in        | 4K streaming (Netflix, Disney+), UHD Blu-ray, DJI drones |
| Drawback       | Higher encoder complexity — more CPU/power                 |

**Improvements over H.264:**
- Larger coding units (64×64 CTU) — better for high-resolution content.
- More intra prediction angles (33 vs 9).
- Improved deblocking filter.
- Parallel processing friendly — tiles and slices.

### H.266 / VVC (Versatile Video Coding)
Successor to H.265 — 2× better than HEVC (4× better than H.264).
Introduced 2020. Very high encoder complexity — not yet widely deployed. Designed for 8K and 360° video.

### VP8 / VP9 (Google)
Google's royalty-free alternatives to H.264 / H.265:
- **VP8**: comparable to H.264 — used in WebRTC (browser video calls).
- **VP9**: comparable to H.265 — used in YouTube 4K, Google Meet.
- Open source — no licensing fees (unlike H.264/H.265).

### AV1 (Alliance for Open Media)
Open-source, royalty-free, next-generation codec:
- ~30% better than H.265 / VP9.
- Supported by Google, Mozilla, Microsoft, Netflix, Amazon, Apple.
- Used in: YouTube (gradually), Netflix 4K, Discord screen share.
- Hardware decode available on newer chips (2020+: Apple M1, AMD RDNA2, Intel 11th gen).

### ProRes / DNxHD / DNxHR (Professional Production)
Used during editing — intra-only (every frame is an I-frame) for easy random access:

| Codec    | Maker  | Bitrate (1080p30) | Use                               |
|----------|--------|--------------------|-----------------------------------|
| ProRes 422 | Apple | ~147 Mbps        | Standard post-production on Mac   |
| ProRes RAW | Apple | ~1 Gbps          | Raw sensor data + ProRes wrapper  |
| DNxHD    | Avid   | 36–220 Mbps       | Avid Media Composer               |
| DNxHR    | Avid   | Up to 4K          | High-res editing                  |

### RAW Video
No compression — straight sensor data before demosaicing (for Bayer sensors) or after debayer.
- **CinemaDNG**: Adobe's raw format — used in Blackmagic, some DJI drones.
- **ARRIRAW**: Arri's raw — cinema standard.
- **R3D**: RED Digital Cinema raw format.
- **Requires very fast storage** — PCIe NVMe SSD or RAID.

### Codec Comparison Summary

```
Compression efficiency (same quality, lower bitrate = more efficient):
AV1 > H.266/VVC > H.265/HEVC ≈ VP9 > H.264/AVC ≈ VP8 >> MJPEG

Decoding hardware support (wider = easier to play back):
H.264 > H.265 > VP9 > AV1 > H.266

Encoder speed (faster = less delay):
MJPEG > H.264 Baseline > H.264 High > H.265 > AV1
```

---

## Part 6: Video Containers vs Codecs

A **container** (file format) wraps compressed video + audio + subtitles + metadata. The codec is what's inside.

| Container | Extension | Common Video Codecs       | Common Audio    | Notes                         |
|-----------|-----------|---------------------------|-----------------|-------------------------------|
| MP4       | .mp4/.m4v | H.264, H.265, AV1         | AAC, MP3        | Most universal format         |
| MKV       | .mkv      | Any codec                 | Any             | Open, flexible, no DRM        |
| MOV       | .mov      | H.264, ProRes, HEVC       | AAC, PCM        | Apple QuickTime               |
| AVI       | .avi      | H.264, DivX, XviD         | MP3, PCM        | Old Windows format            |
| WebM      | .webm     | VP8, VP9, AV1             | Vorbis, Opus    | Browser streaming (open)      |
| TS        | .ts       | H.264, H.265              | AAC, AC3        | MPEG transport stream — broadcast, DVR |
| MXF       | .mxf      | XDCAM, ProRes, DNxHD      | PCM             | Professional broadcast        |
| FLV       | .flv      | H.263, H.264              | MP3, AAC        | Old Flash — largely obsolete  |

---

## Part 7: Video Streaming Protocols

### RTSP — Real-Time Streaming Protocol
The most common protocol for IP camera streams.
- **Port:** 554.
- **How it works:** RTSP sets up the session (like HTTP for streams). The actual video data flows via **RTP (Real-time Transport Protocol)** over UDP.
- **Latency:** 1–3 seconds typically.
- **Used in:** IP security cameras, NVRs, VLC player network streams.

```
Client → RTSP DESCRIBE → Server (what streams are available?)
Client → RTSP SETUP → Server (set up RTP transport)
Client → RTSP PLAY → Server (start sending video)
Server → RTP packets → Client (video data over UDP)
```

### RTMP — Real-Time Messaging Protocol
Originally Adobe's Flash streaming protocol — now the standard for sending video to streaming platforms.
- **Port:** 1935 (RTMP) or 443 (RTMPS — secure).
- **Direction:** One-way, low latency ingest (OBS → YouTube, Twitch, Facebook Live).
- **Latency:** 1–3 seconds to ingest server, then platform adds buffer.
- **Used by:** OBS Studio, streaming encoders, IP cameras with cloud upload.

### HLS — HTTP Live Streaming (Apple)
Standard for delivering video to web browsers and mobile apps.
- Video is split into small **segments** (2–6 second .ts or .mp4 files).
- A **manifest file (.m3u8)** lists the segment URLs and available quality levels.
- Player downloads segments sequentially via standard HTTP.
- Supports **Adaptive Bitrate (ABR)**: player switches quality based on bandwidth.
- **Latency:** Traditional HLS = 20–30 seconds. Low-Latency HLS (LLHLS) = 2–5 seconds.
- **Used in:** Apple devices, YouTube, Netflix (partially), TV broadcasts, CDNs.

```
Server creates:
  playlist.m3u8 (manifest)
  segment0001.ts (2 seconds)
  segment0002.ts (2 seconds)
  ...

Player requests:
  GET playlist.m3u8 → sees segment list
  GET segment0001.ts → decode → display
  GET segment0002.ts → decode → display
  ...
```

### DASH — Dynamic Adaptive Streaming over HTTP (MPEG-DASH)
Open alternative to HLS — same concept, different format.
- Manifest is an **MPD (Media Presentation Description)** XML file.
- Segments in .m4s format (fragmented MP4).
- Used by: YouTube, Netflix, Amazon Prime Video.

### WebRTC — Web Real-Time Communications
Designed for browser-to-browser real-time communication — the technology behind video calls.
- **Latency:** 50–200 ms (true real-time).
- **Codec:** VP8 or H.264 mandatory, AV1 emerging.
- **Transport:** RTP over UDP with DTLS encryption.
- **STUN/TURN/ICE:** Protocols for traversing NAT and firewalls.
- **Used in:** Google Meet, Zoom (partially), Discord, FaceTime, drone ground station streaming.

### SRT — Secure Reliable Transport
Open-source protocol for low-latency streaming over unreliable networks (internet links):
- Adds retransmission, forward error correction (FEC), and AES encryption on top of UDP.
- Designed for contribution (studio → internet → broadcast server) links.
- **Latency:** Configurable — 120 ms minimum, typically 200–500 ms over internet.
- **Used in:** Professional live production, IP broadcast, IP camera contribution.

### NDI — Network Device Interface (NewTek)
High-quality, low-latency video over Gigabit Ethernet:
- Designed for local network studio use — camera to switcher over standard LAN.
- **Latency:** < 1 frame (16 ms at 60 fps).
- **Bandwidth:** ~100–150 Mbps per 1080p60 stream (compressed but visually lossless).
- **Used in:** Virtual production, broadcast studios, video conferencing (OBS NDI plugin).

### ONVIF — Open Network Video Interface Forum
A standard that defines how IP cameras expose their control interface:
- Not a streaming protocol — a device discovery and management standard.
- Any ONVIF-compliant camera can be added to any ONVIF-compliant NVR.
- Uses RTSP underneath for the actual video stream.

---

## Part 8: Wireless Video Transmission

### Analog FPV (5.8 GHz)
Already described in Part 2. Key points:
- FM modulation of composite video onto 5.8 GHz carrier.
- < 1 ms latency.
- No encryption — anyone with the right receiver can watch.
- Range: 0.5–3 km depending on transmitter power and antenna.
- Degrades gracefully — signal weakens before completely failing (snow, static).

### Digital FPV Systems

| System       | Maker         | Frequency | Latency    | Resolution     | Range    | Encryption |
|--------------|---------------|-----------|------------|----------------|----------|------------|
| DJI FPV      | DJI           | 2.4 + 5.8 GHz | 28 ms | 1080p60        | ~10 km   | Yes        |
| DJI O3/O4    | DJI           | 2.4 + 5.8 GHz | 20–40 ms | 1080p / 4K  | ~20 km   | Yes        |
| HDZero       | HDZERO        | 5.8 GHz   | 8–13 ms    | 1080p30 / 720p60 | ~3 km  | No         |
| Walksnail    | Caddx         | 5.8 GHz   | 20–30 ms   | 1080p60 / 4K   | ~5 km    | Yes        |
| Shark Byte   | —             | 5.8 GHz   | 8 ms       | 720p60         | ~2 km    | No         |
| Orqa Link    | Orqa          | 5.8 GHz   | < 15 ms   | 720p60         | ~2 km    | No         |
| Connex       | Amimon        | 5 GHz     | < 1 ms    | 1080p60        | ~0.5 km  | No         |

**How digital FPV works (DJI O3 example):**
```
Camera → H.265 encoder (hardware, very fast) → transmitter
  → OFDM modulated signal on 2.4 GHz and/or 5.8 GHz
  → received by goggles
  → H.265 decoder (hardware) → display
```

The latency is the encode + transmit + decode time. Hardware H.265 encoders can encode in ~5 ms, plus transmission and decode adds more.

**Why analog FPV still exists:** 8–28 ms is noticeable when flying at speed. Some pilots still prefer analog's < 1 ms latency for racing.

### WiFi Video Streaming (ESP32-CAM, drones)
ESP32-CAM streaming over WiFi:
- Captures JPEG frames via DVP interface from OV2640.
- JPEG compressed in hardware.
- Sent via HTTP MJPEG stream or WebSocket.
- Typical resolution: 640×480 to 1600×1200.
- Frame rate: 10–25 fps (limited by WiFi throughput and JPEG encoding time).
- Latency: 100–500 ms (WiFi + encoding + buffering).

```
ESP32-CAM firmware (typical):
  Camera captures frame (OV2640 hardware JPEG)
       │
  JPEG frame in ESP32 RAM (~15 KB at 640×480 quality 10)
       │
  HTTP server sends multipart/x-mixed-replace response
       │  (browser-compatible MJPEG stream)
  Browser or VLC displays stream
```

### Dedicated Video Radio Links (Long Range Drones)

**Herelink (CubePilot):**
- 2.4 GHz custom OFDM link.
- Carries 1080p video + RC control + telemetry on one link.
- Range: ~20 km.

**Connex ProSight:**
- 5.8 GHz OFDM.
- Sub-1 ms latency (unique — uses HARQ ARQ retransmission).
- 1080p60 — used in cinema drones.

**Peplink / Rajant:**
- Multi-operator cellular bonding — bonds 4G/5G SIM cards for very long range.
- Used in drones that fly beyond radio LOS range.

### COFDM — Coded OFDM (Professional Broadcast / Military)
Used in professional wireless video links for broadcast and military:
- **COFDM (Coded Orthogonal Frequency Division Multiplexing)**: OFDM + FEC coding.
- Works in NLOS (Non-Line of Sight) environments — signal bounces off buildings and still works.
- High robustness — used in DVB-T (digital terrestrial TV), DAB radio.
- Military COFDM links: encrypted, frequency agile, anti-jam.
- Typical bitrate: 4–20 Mbps at ranges of 1–50 km.

---

## Part 9: HDCP and Video Copy Protection

**HDCP (High-bandwidth Digital Content Protection):**
Encrypts video on HDMI, DisplayPort, and DVI to prevent copying.

```
Source (Blu-ray player) ←──── authenticate ────→ Sink (TV)
   - Both devices have HDCP keys
   - Exchange and verify keys (ECDH-style handshake)
   - Derive shared encryption key
   - All video data encrypted in transit
   - TV can only decrypt because it has valid HDCP hardware key
```

**HDCP versions:**
- HDCP 1.4: 40-bit key — cracked (master key leaked 2010).
- HDCP 2.2: 128-bit AES — used for 4K content (required by UHD Blu-ray, Netflix 4K).
- HDCP 2.3: Minor update to 2.2.

This is why screen capture cards need HDCP stripping for protected content — and why HDCP-compliant capture cards (like Elgato 4K) must authenticate as a valid HDCP sink.

---

## Part 10: Video over IP — Broadcast and Data Centre

### SDI to IP Transition (SMPTE 2110)

Traditional broadcast used SDI cables everywhere in a TV studio. Modern studios switch to IP:

**SMPTE ST 2110:** Standard for professional media over IP:
- **ST 2110-20**: Uncompressed video over IP (RTP/UDP).
- **ST 2110-30**: Audio over IP (AES67).
- **ST 2110-40**: Metadata over IP.
- Runs on **10 Gbps or 25 Gbps Ethernet** — uncompressed 1080p59.94 needs ~3 Gbps.

**Advantage:** One network infrastructure for everything — cameras, servers, graphics, monitoring all on IP switches. More flexible routing than physical SDI patching.

### Video over Fibre

| Type          | Use                                             | Distance      |
|---------------|-------------------------------------------------|---------------|
| Single-mode fibre | Long-haul HDMI/SDI extension — km scale  | Up to 40 km   |
| Multi-mode fibre | Short runs in facilities — cheaper optics  | Up to 550 m   |
| CWDM/DWDM    | Multiple video channels on one fibre (wavelength multiplexing) | Hundreds of km |
| Tactical fibre | Military — battlefield CCTV, UAV ground station | 1–10 km    |

### KVM over IP
Extends keyboard/video/mouse over a network — used in data centres and remote control rooms:
- **H.264 KVM extenders:** Compress the screen, send over IP, decompress at remote station.
- Latency: 30–100 ms — acceptable for management, not for gaming.

---

## Part 11: Display Technologies and How They Receive Video

Understanding the display end completes the chain.

### LVDS — Low Voltage Differential Signalling
Internal video bus between graphics chip and LCD panel in laptops and embedded systems:
- 4 pairs (LVDS) or 8 pairs (Dual LVDS).
- Bandwidth: up to ~2 Gbps.
- Short range (within device).
- Being replaced by eDP.

### eDP — Embedded DisplayPort
Modern internal display bus in laptops, tablets, Raspberry Pi DSI displays:
- Up to 32.4 Gbps (eDP 1.4b) — enough for 4K 120 Hz internal displays.
- Replaces LVDS.

### DSI — Display Serial Interface (MIPI)
Used in smartphones and embedded systems to drive small LCD/OLED displays:
- 1–4 data lanes, up to 2.5 Gbps per lane.
- Used by Raspberry Pi Official touchscreen, many phone displays.
- SPI/I2C for commands, DSI for pixel data.

### SPI / I2C Displays (Embedded / Drone HUD)
For small displays on microcontrollers:
- **SPI TFT (ST7789/ILI9341):** 40 MHz SPI → enough for 240×240 at 30 fps.
- **I2C OLED (SSD1306):** 400 kHz I2C → 128×64 monochrome, adequate for status display.
- **Parallel 8-bit:** Faster than SPI — used in some colour LCDs on Arduino.

---

## Part 12: Key Chips in the Video Chain

### Image Sensors
| Chip      | Maker     | Resolution | Interface | Use                                 |
|-----------|-----------|------------|-----------|-------------------------------------|
| OV2640    | OmniVision| 2 MP       | DVP/SCCB  | ESP32-CAM — cheapest FPV/streaming  |
| OV5640    | OmniVision| 5 MP       | DVP/MIPI CSI | Mid-range, autofocus available  |
| IMX219    | Sony      | 8 MP       | MIPI CSI-2| Raspberry Pi Camera v2              |
| IMX477    | Sony      | 12 MP      | MIPI CSI-2| Raspberry Pi HQ Camera              |
| IMX708    | Sony      | 12 MP      | MIPI CSI-2| Raspberry Pi Camera v3              |
| IMX385    | Sony      | 2 MP       | MIPI CSI-2| Low light — night-vision FPV cameras |
| IMX586    | Sony      | 48 MP      | MIPI CSI-2| Smartphone flagship sensors         |

### ISP (Image Signal Processor)
Converts raw sensor Bayer pattern into processed colour video:
- Demosaicing, white balance, noise reduction, sharpening, HDR.
- **Built into:** Raspberry Pi (dedicated ISP hardware), smartphone SoCs, DJI camera systems.
- **Standalone ISP chips:** TW9900 (video decoder), ADV7280 (analog to digital for NTSC/PAL).

### Video Encoders
| Chip      | Maker    | Codecs        | Bitrate  | Use                               |
|-----------|----------|---------------|----------|-----------------------------------|
| H3A       | Ambarella| H.265/H.264   | 100+ Mbps| DJI drone cameras, action cameras |
| S5L       | Ambarella| H.265/H.264/AV1 | 200+ Mbps | GoPro, high-end cameras         |
| Hi3516    | HiSilicon| H.265/H.264   | 50 Mbps  | IP security cameras               |
| ESP32-S3  | Espressif| JPEG (HW), H.264 (SW) | 2 Mbps | ESP32 based streaming           |
| NVIDIA Jetson | NVIDIA| H.265/H.264 | 4K60     | Edge AI + video encode/decode    |

### Video Transmitter / FPV VTX Chips
| Chip      | Used In      | Frequency | Power    | Notes                             |
|-----------|--------------|-----------|----------|-----------------------------------|
| RTC6705   | Budget VTX   | 5.8 GHz   | 25–200 mW| SPI-programmable, old standard   |
| SA818     | VHF/UHF VTX  | 134–174 MHz / 400–480 MHz | 1W | Long-range video (analog) |
| DJI custom| DJI O3/O4    | 2.4+5.8 GHz| 28–1000 mW | OFDM H.265 digital FPV        |

---

## Part 13: Complete Video Chain — End to End

```
CAPTURE
─────────────────────────────────────────────────────────────────
  Scene (photons)
       │
  Camera lens → focuses light onto sensor
       │
  Image sensor (CMOS / CCD)
    - Photodiodes convert photons to electrons
    - Bayer colour filter array (RGGB pattern)
    - ADC: converts voltage to digital pixel values (RAW)
       │
  ISP (Image Signal Processor)
    - Demosaicing: interpolates full-colour pixels from Bayer
    - White balance, exposure, noise reduction, sharpening
    - Output: YCbCr or RGB video frames
       │
ENCODE / COMPRESS
─────────────────────────────────────────────────────────────────
  Video encoder (hardware H.264/H.265 block)
    - Intra/inter prediction
    - DCT + quantisation
    - Entropy coding (CABAC)
    - Packetise into NAL units
       │
TRANSMIT
─────────────────────────────────────────────────────────────────
  Wired: HDMI / SDI / DisplayPort / USB
     OR
  Network: RTP/RTSP, RTMP, HLS, WebRTC, MPEG-DASH
     OR
  Wireless RF: 5.8 GHz analog FM / OFDM digital FPV / WiFi MJPEG
       │
RECEIVE / DECODE
─────────────────────────────────────────────────────────────────
  Receive + buffer (jitter buffer for network streams)
       │
  Decode (hardware H.264/H.265 decoder)
    - Entropy decode
    - Inverse DCT
    - Motion compensation
    - Output: YCbCr frame
       │
  Colour convert: YCbCr → RGB
       │
DISPLAY
─────────────────────────────────────────────────────────────────
  GPU compositing (overlay UI, subtitles)
       │
  Display controller → eDP / LVDS / DSI (internal)
                     → HDMI / DP (external)
       │
  LCD / OLED / Projector
    - Backlight (LCD) or OLED self-emitting
    - Pixels updated at display refresh rate (60/90/120/144/240 Hz)
       │
  Human eye
```

---

## Glossary

| Term     | Meaning                                                                    |
|----------|----------------------------------------------------------------------------|
| Codec    | Coder-Decoder — algorithm that compresses and decompresses video           |
| Container| File format wrapping compressed video + audio (MP4, MKV, MOV)             |
| Bitrate  | Data per second (Mbps) — higher = better quality or less compression       |
| Keyframe | I-frame — complete independently decodable frame                          |
| GOP      | Group of Pictures — sequence from one I-frame to the next                 |
| Latency  | Delay from capture to display — critical for FPV and video calls          |
| TMDS     | Transition Minimised Differential Signalling — HDMI/DVI physical layer    |
| MIPI CSI | Camera interface standard in smartphones and embedded systems             |
| OFDM     | Multi-carrier modulation — used in WiFi, digital FPV, DVB-T              |
| COFDM    | OFDM + FEC — used in broadcast and military for NLOS robustness          |
| YCbCr    | Luma + chroma colour space — used in video compression                    |
| 4:2:0    | Chroma subsampling — halved colour resolution (most compressed video)     |
| DCT      | Discrete Cosine Transform — converts pixel blocks to frequency coefficients|
| ABR      | Adaptive Bitrate — streaming switches quality automatically with bandwidth |
| VTX      | Video Transmitter — FPV drone component that modulates video onto RF      |
| VRX      | Video Receiver — receives and demodulates FPV RF signal                   |
| HDCP     | Copy protection encryption on HDMI/DP                                     |
| ISP      | Image Signal Processor — converts RAW sensor data to usable video         |
| TVL      | TV Lines — horizontal resolution measure for analog cameras               |
| NVR      | Network Video Recorder — IP camera recording system                       |
| DVR      | Digital Video Recorder — records analog CCTV                              |

---

## Relevance to Your Drone Project

| Topic                | Application                                                          |
|----------------------|----------------------------------------------------------------------|
| Analog FPV (NTSC/PAL)| Direct build option — cheap, < 1 ms latency, perfect for racing FPV |
| Digital FPV (DJI O3) | Better image quality, encrypted, longer range — used in cinema/inspection drones |
| ESP32-CAM OV2640     | DVP interface — read camera frames, compress JPEG, stream over WiFi  |
| MIPI CSI-2           | Raspberry Pi camera interface — higher quality than ESP32-CAM       |
| H.264 / H.265        | DJI drones record in H.264/H.265 — understanding codec helps with settings |
| MJPEG streaming      | Best option for ESP32-CAM — simple, low CPU, direct to browser      |
| WebRTC               | Browser-based real-time drone video stream with ~100 ms latency     |
| 5.8 GHz channel plan | Choose VTX channel away from WiFi 5 GHz to avoid interference       |
| RTSP                 | IP cameras and advanced drone FPV links expose RTSP streams          |
| OSD (On-Screen Display) | Flight controllers overlay telemetry text on FPV video before VTX |
| Chroma subsampling   | 4:2:0 explains why drone footage colour-grading differs from raw photos |
| Frame rate           | 60 fps FPV feels smoother for flying; 24 fps cinematic for recording |
| Bitrate setting      | Higher bitrate = less compression artefacts = larger file — set per card write speed |
