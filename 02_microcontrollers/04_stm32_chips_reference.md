# STM32 Chips — Individual Deep-Dive Reference
## F405 · F411 · F722 · H743 · H750 · H723

---

## How to Read an STM32 Part Number

Before diving into each chip, learn to decode the name. Every STM32 part number tells a story.

```
  Example: STM32 F 405 R G T 6

  STM32  → Made by STMicroelectronics, 32-bit ARM family
  F      → Product line (F = Foundation/mainstream, H = High-performance, L = Low-power, G = General)
  405    → Specific chip number within that line
  R      → Pin count (C=48, R=64, V=100, Z=144, I=176, A=168)
  G      → Flash size (B=128K, C=256K, D=384K, E=512K, G=1MB, H=1.5MB, I=2MB)
  T      → Package type (T=LQFP, H=UFBGA, U=UFQFPN, Y=WLCSP)
  6      → Temperature range (6 = Industrial: -40°C to +85°C)
```

```
  Quick decode table:

  STM32 [Line][Number][Pins][Flash][Package][Temp]
        ────── ─────── ──── ───── ─────── ────
        F405   R       G    T     6
               │       │
               │       └── Flash = G → 1 MB
               └── Pins = R → 64 pins (LQFP64)

  STM32H743VIT6:
  H = High-performance series
  743 = chip number
  V = 100 pins
  I = 2 MB flash
  T = LQFP package
  6 = industrial temp
```

---

## The Three Families in This Guide

```
  ┌────────────────────────────────────────────────────────────────┐
  │                  STM32 Drone-Relevant Families                  │
  ├──────────────────┬─────────────────┬───────────────────────────┤
  │   F4 Series      │   F7 Series     │   H7 Series               │
  │  (Cortex-M4F)   │  (Cortex-M7)    │  (Cortex-M7, advanced)   │
  ├──────────────────┼─────────────────┼───────────────────────────┤
  │ F405, F411       │ F722, F745, F7xx│ H743, H750, H723          │
  │ 168–180 MHz      │ 216–216 MHz     │ 480–550 MHz               │
  │ Hardware FPU ✓   │ FPU + DSP ✓    │ FPU + DSP + Cache ✓       │
  │ Workhorse of     │ Bridge between  │ Most powerful             │
  │ hobby FCs        │ F4 and H7       │ modern FCs                │
  └──────────────────┴─────────────────┴───────────────────────────┘
```

> **Important correction:** STM32**F722** is part of the **F7 series** (Cortex-M7), NOT F4. The user grouped it with F405/F411 but it belongs one generation above them. This guide places it correctly.

---

## Part 1 — F4 Series

### The F4 in Context

The F4 family launched around 2011. Its Cortex-M4F core added a hardware **Floating Point Unit (FPU)** — the feature that made it viable for flight controllers. Before F4, developers used integer math and lookup tables to fake floating-point. The F4 let them write `sin(angle)` and have hardware do it in a single clock cycle.

```
  F4 Internal Architecture (simplified):

  ┌─────────────────────────────────────────────────────────────┐
  │                   STM32F4xx Die                             │
  │                                                             │
  │  ┌──────────────────────────────────────────────────────┐  │
  │  │           ARM Cortex-M4F Core @ 168 MHz              │  │
  │  │  ┌──────────┐  ┌──────────┐  ┌─────────────────────┐│  │
  │  │  │   ALU    │  │ FPU      │  │  MPU (Memory Protect)││  │
  │  │  │(integers)│  │(floats)  │  │                     ││  │
  │  │  └──────────┘  └──────────┘  └─────────────────────┘│  │
  │  └──────────────────────────────────────────────────────┘  │
  │           │ AHB Bus (fast, 168 MHz)                         │
  │  ┌────────┴────────────────────────────────────────────┐   │
  │  │  Flash  │  SRAM  │  CCM RAM  │  DMA1  │  DMA2      │   │
  │  │  (code) │(128KB) │  (64KB)   │        │             │   │
  │  └─────────┴────────────────────────────────────────────┘   │
  │           │ APB Bus (slower peripherals)                     │
  │  ┌────────┴────────────────────────────────────────────┐   │
  │  │  UART × 6  │  SPI × 3  │  I2C × 3  │  Timers × 14 │   │
  │  │  ADC × 3   │  DAC × 2  │  USB OTG   │  CAN × 2     │   │
  │  └─────────────────────────────────────────────────────┘   │
  └─────────────────────────────────────────────────────────────┘
```

---

### Chip 1 — STM32F405

**The Workhorse of Drone Flight Controllers**

```
  ┌─────────────────────────────────────────────────────────────┐
  │                     STM32F405 Profile                       │
  ├─────────────────────────────────────────────────────────────┤
  │  CPU Core:       ARM Cortex-M4F                             │
  │  Max Clock:      168 MHz                                    │
  │  Architecture:   32-bit, Thumb-2 instruction set            │
  ├─────────────────────────────────────────────────────────────┤
  │  MEMORY                                                     │
  │  Flash (code):   1 MB (on-chip, zero wait-state access)     │
  │  SRAM:           128 KB (general purpose)                   │
  │  CCM-SRAM:        64 KB (Core Coupled Memory — fastest RAM) │
  │  Total RAM:      192 KB                                     │
  ├─────────────────────────────────────────────────────────────┤
  │  FLOATING POINT                                             │
  │  FPU:            Single-precision (32-bit float)            │
  │  Speed:          1 clock cycle for add/multiply             │
  │                  (vs ~20 cycles in software on Cortex-M3)   │
  ├─────────────────────────────────────────────────────────────┤
  │  DMA                                                        │
  │  Controllers:    2 (DMA1 + DMA2)                            │
  │  Streams:        7 (DMA1) + 8 (DMA2) = 15 total            │
  │  Channels/stream:4 (you choose which peripheral)            │
  ├─────────────────────────────────────────────────────────────┤
  │  TIMERS                                                     │
  │  Advanced:       2 (TIM1, TIM8) — for motor PWM            │
  │  General Purpose:10 (16-bit and 32-bit)                    │
  │  Basic:          2 (TIM6, TIM7)                             │
  │  Total:          14                                         │
  ├─────────────────────────────────────────────────────────────┤
  │  COMMUNICATION                                              │
  │  UART/USART:     6                                          │
  │  SPI:            3 (+ 2 I2S audio)                          │
  │  I2C:            3                                          │
  │  CAN:            2                                          │
  │  USB:            OTG Full Speed (12 Mbps) + High Speed port │
  ├─────────────────────────────────────────────────────────────┤
  │  ADC:            3 × 12-bit, 24 channels total              │
  │  DAC:            2 × 12-bit                                 │
  │  GPIO:           Up to 140 pins (depends on package)        │
  ├─────────────────────────────────────────────────────────────┤
  │  PACKAGES                                                   │
  │  LQFP64  (8×8mm)  ← most common on flight controllers      │
  │  LQFP100 (14×14mm)                                         │
  │  LQFP144 (20×20mm)                                         │
  ├─────────────────────────────────────────────────────────────┤
  │  PRICE (chip alone):  ~$5–8 USD                             │
  │  POWER:               Active: ~100mA @ 3.3V @ 168MHz        │
  │                        ≈ 330mW                              │
  └─────────────────────────────────────────────────────────────┘
```

#### What Makes the F405 Special for Drones

**CCM-SRAM (Core Coupled Memory):**

```
  Normal SRAM access path:
  CPU → AHB Bus → SRAM → back via AHB → CPU
        (shared bus, may have traffic)
  
  CCM access path:
  CPU ←──────→ CCM (64KB)
       (direct, private bus, zero contention, fastest possible)
  
  Betaflight places its most critical variables — PID state,
  IMU buffers, motor values — in CCM to maximize speed.
```

**The 6 UARTs — Why It Matters:**

```
  A typical drone FC needs at least:
  
  UART1 → GPS module (NMEA or UBX at 115200 baud)
  UART2 → RC receiver (SBUS, IBUS, CRSF)
  UART3 → VTX (video transmitter control — SmartAudio)
  UART4 → ESC telemetry (temperature, RPM, current)
  UART5 → Spare (blackbox, second GPS, serial debug)
  UART6 → USB-UART bridge (programming, Betaflight Configurator)
  
  F405 has all 6. F411 has only 3. This alone is why pros pick F405.
```

#### Real Flight Controllers Using STM32F405

| Product | Notes |
|---------|-------|
| Matek F405-WING | Popular fixed-wing FC |
| Matek F405-STD | Quad racing/freestyle |
| DIATONE Mamba F405 | Budget racing stack |
| iFlight SucceX F4 | Common on iFlight builds |
| Seriously Pro SP Racing F4 | Open-source reference design |

#### Code — What F405's 6 UARTs Look Like in Betaflight Config

```c
// Betaflight target definition (simplified) for F405:
// Each UART is assigned to a drone function

#define USE_UART1           // GPS
#define USE_UART2           // Receiver (SBUS)
#define USE_UART3           // Smart Audio (VTX)
#define USE_UART4           // ESC Telemetry
#define USE_UART6           // USB/MSP Configurator
// UART5 spare or blackbox serial
```

---

### Chip 2 — STM32F411

**The Budget F4 — Know Its Limits**

```
  ┌─────────────────────────────────────────────────────────────┐
  │                     STM32F411 Profile                       │
  ├─────────────────────────────────────────────────────────────┤
  │  CPU Core:       ARM Cortex-M4F                             │
  │  Max Clock:      100 MHz  (← 40% slower than F405!)        │
  │  Architecture:   32-bit, Thumb-2                            │
  ├─────────────────────────────────────────────────────────────┤
  │  MEMORY                                                     │
  │  Flash:          512 KB  (← half of F405's 1MB)            │
  │  SRAM:           128 KB  (no separate CCM!)                 │
  │  Total RAM:      128 KB  (vs 192KB on F405)                 │
  ├─────────────────────────────────────────────────────────────┤
  │  FLOATING POINT                                             │
  │  FPU:            Single-precision ✓ (same as F405)          │
  ├─────────────────────────────────────────────────────────────┤
  │  DMA                                                        │
  │  Controllers:    2 (DMA1 + DMA2) ← same count as F405      │
  │  Streams:        7 + 8 = 15 ← same                         │
  ├─────────────────────────────────────────────────────────────┤
  │  TIMERS                                                     │
  │  Advanced:       1 (TIM1 only)                              │
  │  General Purpose:6                                          │
  │  Total:          11  (vs 14 on F405)                        │
  ├─────────────────────────────────────────────────────────────┤
  │  COMMUNICATION                                              │
  │  UART/USART:     3  (← only 3! vs 6 on F405)               │
  │  SPI:            5                                          │
  │  I2C:            3                                          │
  │  CAN:            0  (no CAN bus!)                           │
  │  USB:            OTG Full Speed only                        │
  ├─────────────────────────────────────────────────────────────┤
  │  ADC:            1 × 12-bit, 16 channels                    │
  │  DAC:            None (0 DAC channels)                      │
  │  GPIO:           Up to 50 usable pins                       │
  ├─────────────────────────────────────────────────────────────┤
  │  PACKAGES                                                   │
  │  UFQFPN48 (7×7mm) ← used on "Black Pill" dev board         │
  │  LQFP64 (10×10mm)                                          │
  ├─────────────────────────────────────────────────────────────┤
  │  PRICE:          ~$2–4 USD (notably cheaper than F405)      │
  │  POWER:          Active: ~50mA @ 3.3V @ 100MHz ≈ 165mW     │
  └─────────────────────────────────────────────────────────────┘
```

#### F411 vs F405 — The Cuts That Matter

```
  ┌────────────────────────────────────────────────────────────┐
  │  What STM32 removed to make the F411 cheaper:             │
  ├────────────────┬─────────────────────────────────────────┤
  │ Removed        │ Impact                                   │
  ├────────────────┼─────────────────────────────────────────┤
  │ 3 UARTs (6→3)  │ Can't connect GPS + RX + VTX + ESC telem│
  │ 64KB CCM RAM   │ No ultra-fast RAM for PID variables      │
  │ 512KB Flash    │ Less room for complex firmware features  │
  │ 68 MHz clock   │ PID loop and sensor fusion runs slower   │
  │ 2nd ADC, DAC   │ Less analog flexibility                  │
  │ CAN bus        │ No industrial protocol support           │
  │ TIM8 (Advanced)│ Fewer high-resolution PWM channels       │
  └────────────────┴─────────────────────────────────────────┘
```

#### Where F411 Actually Shines

```
  F411 is the RIGHT choice for:
  
  ✓ Learning STM32 (cheaper to experiment with, same Cortex-M4F)
  ✓ Development boards (Nucleo-F411RE is excellent for learning)
  ✓ Simple fixed-wing FCs with minimal peripheral needs
  ✓ Non-FC embedded projects needing FPU (robotics, data logging)
  ✓ "Black Pill" development board (~₹400) — great learning tool
  
  F411 is the WRONG choice for:
  ✗ Full-featured quad FC (not enough UARTs)
  ✗ Projects needing CAN bus
  ✗ High-frequency PID loops where every MHz matters
```

#### The Black Pill Development Board

```
  STM32F411CEU6 "Black Pill" (Weact Studio):

  ┌─────────────────────────────────────────────┐
  │  ┌─USB-C─┐                                  │
  │  │       │  STM32F411CE chip                 │
  │  │ ┌───┐ │  48-pin UFQFPN package           │
  │  │ │   │ │  512KB Flash / 128KB SRAM         │
  │  │ └───┘ │  Crystal: 25MHz → PLL → 100MHz   │
  │  │       │  SWD debug port (4 pins)          │
  │  │[BOOT] │  USER button                      │
  │  │[NRST] │  User LED (PC13)                  │
  │  └───────┘  USB bootloader (no ST-Link needed│
  │  GPIO along both sides                       │
  └─────────────────────────────────────────────┘
  
  Cost: ~₹350–500
  Recommended starter board for STM32 learning
  Works with STM32CubeIDE, PlatformIO
```

---

## Part 2 — F7 Series (One Step Up)

### Why F7 Exists

The F7 series launched in 2015. ST moved to a **Cortex-M7** core — the next generation above M4. Key upgrades:

```
  Cortex-M4F → Cortex-M7 improvements:

  ┌───────────────────────────────────────────────────────────┐
  │  Feature          Cortex-M4F        Cortex-M7             │
  ├───────────────────────────────────────────────────────────┤
  │  Pipeline depth   3-stage           6-stage (faster IPC)  │
  │  FPU              Single (32-bit)   Double (64-bit) too ✓  │
  │  DSP              Basic             Full SIMD DSP ✓        │
  │  Branch predict   Basic             Advanced ✓             │
  │  Instruction cache No               Yes (optional) ✓      │
  │  Data cache       No               Yes (optional) ✓       │
  │  ITCM/DTCM        No               Yes (zero-wait RAM) ✓  │
  └───────────────────────────────────────────────────────────┘
  
  Double-precision FPU means: sensor fusion math using 64-bit
  doubles is hardware-accelerated. More accurate than F4.
```

---

### Chip 3 — STM32F722

**The "Affordable H7 Alternative"**

```
  ┌─────────────────────────────────────────────────────────────┐
  │                     STM32F722 Profile                       │
  ├─────────────────────────────────────────────────────────────┤
  │  CPU Core:       ARM Cortex-M7  (NOT M4 — one gen ahead)   │
  │  Max Clock:      216 MHz                                    │
  │  Architecture:   32-bit, Thumb-2 + DSP instructions         │
  ├─────────────────────────────────────────────────────────────┤
  │  MEMORY                                                     │
  │  Flash:          512 KB (on-chip)                           │
  │  DTCM RAM:       256 KB (Data Tightly Coupled Memory)       │
  │  ITCM RAM:        16 KB (Instruction TCM — for ISR code)    │
  │  Total usable:   ~272 KB                                    │
  ├─────────────────────────────────────────────────────────────┤
  │  TCM EXPLAINED:                                             │
  │  TCM = Tightly Coupled Memory                               │
  │  Runs at CPU speed (216 MHz) with ZERO wait states          │
  │  Even faster than CCM on F405                               │
  │                                                             │
  │  DTCM → your critical variables, DMA buffers               │
  │  ITCM → your most performance-critical ISR/function code    │
  ├─────────────────────────────────────────────────────────────┤
  │  FLOATING POINT                                             │
  │  FPU:            Double-precision (64-bit) ✓                │
  │  DSP:            SIMD instructions for signal processing ✓  │
  ├─────────────────────────────────────────────────────────────┤
  │  ART ACCELERATOR                                            │
  │  Flash is slower than CPU (216MHz CPU, flash < 216MHz)      │
  │  ART = Adaptive Real-Time Accelerator                       │
  │  Prefetches and caches instructions from flash              │
  │  Result: Flash code executes as if CPU-speed ✓              │
  ├─────────────────────────────────────────────────────────────┤
  │  DMA:            2 controllers, 15 streams total            │
  │  TIMERS:         14 timers                                  │
  │  UART/USART:     8 (more than F405!)                        │
  │  SPI:            6                                          │
  │  I2C:            4                                          │
  │  CAN:            1                                          │
  │  USB:            OTG FS + HS                                │
  │  ADC:            3 × 12-bit                                 │
  ├─────────────────────────────────────────────────────────────┤
  │  PACKAGE:        LQFP64 (same footprint as some F4 boards!) │
  │  PRICE:          ~$6–10 USD                                 │
  │  POWER:          Active: ~120mA @ 3.3V @ 216MHz ≈ 400mW    │
  └─────────────────────────────────────────────────────────────┘
```

#### Why the F722 Was Popular (2017–2020)

```
  At the time of release, F722 offered:
  
  ✓ Same pin count/footprint as F405 boards (easy PCB upgrade)
  ✓ 216MHz vs 168MHz — ~28% faster
  ✓ Double-precision FPU (more accurate sensor math)
  ✓ 8 UARTs vs 6 (even more connectivity)
  ✓ ITCM for placing ISRs in fastest-access RAM
  ✓ ART Accelerator eliminates flash bottleneck
  ✓ More reasonable price than H7
  
  Flight controllers that used F722:
  • Matek F722-SE / F722-MINI
  • JHEMCU F7
  • SucceX-E F7
  • Several GEPRC boards
```

#### F722 Gotcha — Only 512KB Flash

```
  Betaflight firmware size (compiled):
  
  F405 target: ~450 KB (fits in 1MB with 550KB spare)
  F722 target: ~490 KB (fits in 512KB — barely!)
  
  The tight flash means:
  ✗ Some features must be disabled to fit
  ✗ Cannot include every optional Betaflight module
  ✗ Less room for debugging symbols
  
  This is why H7 chips with more flash (H743 = 2MB) can include
  every feature and still have headroom.
```

---

## Part 3 — H7 Series

### The H7 Generation

The H7 family is STM32's current high-performance line. Same Cortex-M7 as F7, but with everything turned up:

```
  F7 → H7 upgrades:

  ┌───────────────────────────────────────────────────────────┐
  │  Feature          STM32F7           STM32H7               │
  ├───────────────────────────────────────────────────────────┤
  │  Max clock        216 MHz           480–550 MHz (+120%)   │
  │  L1 cache         Optional 4KB      16KB I + 16KB D ✓     │
  │  RAM              256KB             1MB+ (multiple banks) │
  │  Flash            512KB–2MB         1–2MB (dual bank!)    │
  │  Dual-bank flash  No                Yes (live firmware OTA│
  │  MDMA             No                Yes (master DMA) ✓    │
  │  Dual-core (H745) No                Optional (Cortex-M4!) │
  │  CAN-FD           No                Yes (CAN-FD on H7) ✓  │
  │  Max ADC speed    2.4 MSPS          3.6 MSPS              │
  └───────────────────────────────────────────────────────────┘
```

#### H7 Memory Architecture — More Complex, More Powerful

```
  STM32H7 Multi-Bank RAM Architecture:

  ┌──────────────────────────────────────────────────────────┐
  │                  H7 Memory Map                           │
  │                                                          │
  │  ITCM  (64KB) ← Instruction TCM, fastest, for ISR code  │
  │  DTCM  (128KB)← Data TCM, fastest, for PID variables    │
  │                                                          │
  │  D1 AXI SRAM  (512KB) ← DMA accessible, fast            │
  │  D2 SRAM1     (128KB) ← Peripheral bus domain           │
  │  D2 SRAM2     (128KB) ← Peripheral bus domain           │
  │  D2 SRAM3      (32KB) ← For low-power domain            │
  │  D3 Backup SRAM (4KB) ← Survives low-power sleep        │
  │                                                          │
  │  Total on H743: ~1 MB RAM across 6 different regions     │
  │                                                          │
  │  CAUTION: DMA cannot access ITCM/DTCM directly!         │
  │  (You must use AXI SRAM for DMA buffers)                 │
  └──────────────────────────────────────────────────────────┘
```

---

### Chip 4 — STM32H743

**The Top-of-Range FC Chip**

```
  ┌─────────────────────────────────────────────────────────────┐
  │                     STM32H743 Profile                       │
  ├─────────────────────────────────────────────────────────────┤
  │  CPU Core:       ARM Cortex-M7                              │
  │  Max Clock:      480 MHz  (fastest in H7 mainstream line)   │
  │  Architecture:   32-bit, Thumb-2, DSP, FPU double-precision │
  ├─────────────────────────────────────────────────────────────┤
  │  MEMORY                                                     │
  │  Flash:          2 MB (dual-bank — bank A + bank B, 1MB ea) │
  │  ITCM:           64 KB (fastest, for critical code)         │
  │  DTCM:          128 KB (fastest, for critical data)         │
  │  D1 AXI SRAM:   512 KB (for DMA-accessible buffers)         │
  │  D2 SRAM1/2/3:  288 KB (peripheral domain)                  │
  │  D3 Backup:       4 KB (survives sleep, config storage)      │
  │  Total RAM:      ~1 MB (split across banks)                  │
  ├─────────────────────────────────────────────────────────────┤
  │  CACHE (L1)                                                 │
  │  Instruction cache: 16 KB (fetch-ahead from flash)          │
  │  Data cache:        16 KB (frequently used variables)       │
  │  Effect: flash latency largely eliminated                   │
  ├─────────────────────────────────────────────────────────────┤
  │  DMA                                                        │
  │  MDMA (Master DMA): For large cross-domain transfers        │
  │  DMA1 + DMA2:  Standard DMA, 8 streams each                │
  │  BDMA:         Basic DMA for low-power domain               │
  ├─────────────────────────────────────────────────────────────┤
  │  TIMERS:         22 total (more than any other on this list) │
  │  UART/USART:     10                                          │
  │  SPI:            6                                           │
  │  I2C:            4                                           │
  │  CAN-FD:         2 (CAN with Flexible Data-rate, up to 8Mbps│
  │  USB:            OTG FS + HS (with external PHY support)     │
  │  Ethernet:       MAC 10/100 (need external PHY chip)        │
  │  SDMMC:          2 (SD card interface, fast)                 │
  │  JPEG:           Hardware JPEG encoder/decoder               │
  ├─────────────────────────────────────────────────────────────┤
  │  ADC:            3 × 16-bit (!) at 3.6 MSPS                 │
  │                  (F4 had 12-bit; H743 has 16-bit = 16× more │
  │                  resolution for voltage/current sensing)     │
  │  DAC:            2 × 12-bit                                 │
  ├─────────────────────────────────────────────────────────────┤
  │  DUAL BANK FLASH — Game Changer                             │
  │  Bank A can be READ while Bank B is being WRITTEN           │
  │  Result: OTA firmware updates while drone is FLYING         │
  │          (code executes from Bank A, new code writes to B)  │
  ├─────────────────────────────────────────────────────────────┤
  │  PACKAGE:  LQFP100, LQFP144, TFBGA176                      │
  │  PRICE:    ~$10–15 USD                                      │
  │  POWER:    ~200mA @ 3.3V @ 480MHz ≈ 660mW                  │
  └─────────────────────────────────────────────────────────────┘
```

#### H743 Real-World FC Performance

```
  H743 vs F405 in a Betaflight FC:

  Gyro loop:
  F405 @ 168MHz:  8kHz loop (125μs per cycle)
  H743 @ 480MHz: 32kHz–40kHz loop (25–32μs per cycle)  ← 4–5× faster
  
  PID Math:
  F405: ~0.5μs for full PID calculation (FPU @ 168MHz)
  H743: ~0.15μs for full PID calculation (FPU @ 480MHz + cache)
  
  ADC resolution:
  F405: 12-bit → 4096 steps (measures 0.8mV steps at 3.3V)
  H743: 16-bit → 65536 steps (measures 0.05mV steps) ← much more precise
  
  This precision matters for battery voltage monitoring and
  current sensing on the FC.
```

#### Real Flight Controllers Using STM32H743

| Product | Notes |
|---------|-------|
| Kakute H7 V1/V2 | Holybro, very popular |
| Matek H743-SLIM | Excellent for fixed-wing |
| Matek H743-WING | Full-feature wing FC |
| iFlight Succex H7 | Racing/freestyle |
| GEPRC SPAN F7 HD | with HD VTX |

---

### Chip 5 — STM32H750

**The Trick Chip — Same Power, Almost No Flash**

```
  ┌─────────────────────────────────────────────────────────────┐
  │                     STM32H750 Profile                       │
  ├─────────────────────────────────────────────────────────────┤
  │  CPU Core:       ARM Cortex-M7 (IDENTICAL to H743)          │
  │  Max Clock:      480 MHz (IDENTICAL to H743)                │
  ├─────────────────────────────────────────────────────────────┤
  │  MEMORY                                                     │
  │  Flash:          128 KB  (← !! Only 128KB vs 2MB on H743!!)│
  │  ITCM:            64 KB                                     │
  │  DTCM:           128 KB                                     │
  │  D1 AXI SRAM:    512 KB                                     │
  │  Total RAM:      ~1 MB (same as H743)                       │
  ├─────────────────────────────────────────────────────────────┤
  │  EVERYTHING ELSE: Identical to H743                         │
  │  Same timers, UARTs, SPI, I2C, ADC, DMA, CAN-FD...        │
  ├─────────────────────────────────────────────────────────────┤
  │  PRICE:          ~$5–7 USD  (vs ~$12 for H743 — 50% less!) │
  └─────────────────────────────────────────────────────────────┘
```

#### The H750 Puzzle — Why Remove Flash?

This seems like a terrible trade-off. 128KB of flash won't even hold Betaflight (which is ~500KB+). How does it work?

```
  H750 Flash Strategy: eXecute In Place (XIP) from external flash
  
  ┌──────────────────────────────────────────────────────────────┐
  │  Normal chip (F405, H743):                                   │
  │                                                              │
  │  H743 Internal Flash (2MB) ─── directly runs firmware       │
  │  (Fast, zero setup, just works)                              │
  │                                                              │
  ├──────────────────────────────────────────────────────────────┤
  │  H750 approach:                                              │
  │                                                              │
  │  H750 Internal Flash (128KB)                                 │
  │  ┌──────────────────────────────────────────────────┐       │
  │  │ Bootloader only (~64KB):                         │       │
  │  │ 1. Initialise QSPI external flash chip           │       │
  │  │ 2. Configure QSPI in memory-mapped mode          │       │
  │  │ 3. Jump to firmware IN the external flash        │       │
  │  └──────────────────────────────────────────────────┘       │
  │             │ QSPI (Quad SPI — 4 data lines)                 │
  │             ▼                                                │
  │  ┌─────────────────────────────┐                            │
  │  │ External NOR Flash (e.g.    │                            │
  │  │ W25Q128 = 128Mbit = 16MB)  │                            │
  │  │ Firmware lives here         │                            │
  │  └─────────────────────────────┘                            │
  │                                                              │
  │  H750's AHB bus reads external flash via QSPI as if         │
  │  it were internal memory (XIP = eXecute In Place)           │
  │  Performance: ~80% of internal flash speed (acceptable)     │
  └──────────────────────────────────────────────────────────────┘
```

#### H750 vs H743 — When to Choose Which

```
  Choose H750 when:
  ✓ Cost is a priority (PCB manufacturer ordering 1000 chips)
  ✓ You need large firmware storage (16MB ext flash > 2MB H743)
  ✓ You understand QSPI bootloader design
  ✓ Production product where complexity is manageable
  
  Choose H743 when:
  ✓ Building a personal/prototype FC
  ✓ You want simplicity — flash and run, no bootloader complexity
  ✓ You want dual-bank live OTA
  ✓ $5 price difference doesn't matter to you
  
  For LEARNING: H743. The H750 bootloader adds significant
  complexity that will frustrate a beginner.
```

#### H750 in the Wild

```
  FC boards using H750:
  • Some budget "H7" boards where manufacturer chose H750 to cut cost
  • Matek H743 boards that switched to H750 in later revisions
    (same PCB, cheaper chip, QSPI flash added)
  
  How to tell which chip your FC has:
  → In Betaflight CLI, type: version
  → It prints the exact chip model
  → Or look up your FC's schematic — the chip is labeled
```

---

### Chip 6 — STM32H723

**The Fastest in the Group — But With Trade-offs**

```
  ┌─────────────────────────────────────────────────────────────┐
  │                     STM32H723 Profile                       │
  ├─────────────────────────────────────────────────────────────┤
  │  CPU Core:       ARM Cortex-M7 (newer revision than H743)   │
  │  Max Clock:      550 MHz  (← FASTEST of all six chips!)     │
  │  Architecture:   32-bit, Thumb-2, DSP, double FPU           │
  ├─────────────────────────────────────────────────────────────┤
  │  MEMORY                                                     │
  │  Flash:          1 MB (dual-bank, 512KB each)               │
  │  ITCM:           64 KB                                      │
  │  DTCM:          128 KB                                      │
  │  D1 AXI SRAM:   320 KB (less than H743's 512KB!)            │
  │  D2 SRAM1/2:    128 KB total                                │
  │  D3 SRAM:         4 KB                                      │
  │  Total RAM:      ~564 KB (vs ~1MB on H743 — significant cut)│
  ├─────────────────────────────────────────────────────────────┤
  │  USB: OTG HS with built-in HIGH-SPEED PHY                   │
  │       (H743 needs external PHY chip for USB HS)             │
  │       This means USB HS works with no extra parts! ✓        │
  ├─────────────────────────────────────────────────────────────┤
  │  TIMERS:         22                                          │
  │  UART/USART:     8                                          │
  │  SPI:            6                                           │
  │  I2C:            4                                           │
  │  CAN-FD:         2                                           │
  │  Ethernet:       MAC 10/100 (with ext PHY)                  │
  │  ADC:            3 × 16-bit (same as H743)                  │
  ├─────────────────────────────────────────────────────────────┤
  │  L1 CACHE:       16KB I + 16KB D (same as H743)             │
  ├─────────────────────────────────────────────────────────────┤
  │  PACKAGE:        LQFP100, LQFP144                           │
  │  PRICE:          ~$8–12 USD                                 │
  └─────────────────────────────────────────────────────────────┘
```

#### H723 — The Trade-offs Explained

```
  H723 gains vs H743:
  ┌──────────────────────────────────────────────────────────┐
  │ + 550MHz vs 480MHz → 14.6% faster                       │
  │ + USB HS PHY built-in (saves external PHY chip + space)  │
  │ + Slightly newer silicon process                         │
  └──────────────────────────────────────────────────────────┘
  
  H723 costs vs H743:
  ┌──────────────────────────────────────────────────────────┐
  │ - 564KB vs 1MB RAM (significant, especially for logging) │
  │ - 1MB vs 2MB flash (Betaflight on H743 uses ~700KB)      │
  │   H723 target must be trimmed more aggressively          │
  │ - No Ethernet in some packages                           │
  └──────────────────────────────────────────────────────────┘
```

#### H723 vs H743 for Betaflight

```
  Betaflight loop rates on H7 chips:
  
  H743 @ 480MHz:   40kHz gyro loop possible (25μs/loop)
  H723 @ 550MHz:   40kHz+ gyro loop possible (22μs/loop)
  
  In practice, most pilots use 8kHz loop rate.
  The extra 70MHz of H723 is not noticeable in typical flying.
  
  Where H723's speed matters:
  → Research FCs running custom sensor fusion algorithms
  → Boards doing onboard blackbox analysis in real-time
  → Applications with heavy computation between loop ticks
  
  Real FC boards using H723:
  • Matek H723-SLIM
  • JHEMCU H723
  • T-Motor branded FC
```

---

## Part 4 — All Six Chips, Side by Side

```
  ┌─────────────────────────────────────────────────────────────────────────────┐
  │                    Complete Comparison: All 6 Chips                         │
  ├──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤
  │          │  F405    │  F411    │  F722    │  H743    │  H750    │  H723    │
  ├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
  │ CPU Core │ M4F      │ M4F      │ M7       │ M7       │ M7       │ M7       │
  │ Max MHz  │ 168      │ 100      │ 216      │ 480      │ 480      │ 550      │
  │ FPU      │ 32-bit   │ 32-bit   │ 64-bit   │ 64-bit   │ 64-bit   │ 64-bit   │
  │ DSP      │ Basic    │ Basic    │ Full ✓   │ Full ✓   │ Full ✓   │ Full ✓   │
  │ L1 Cache │ No       │ No       │ No       │ 32KB ✓   │ 32KB ✓   │ 32KB ✓   │
  ├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
  │ Flash    │ 1MB      │ 512KB    │ 512KB    │ 2MB      │ 128KB!   │ 1MB      │
  │ Total RAM│ 192KB    │ 128KB    │ 272KB    │ ~1MB     │ ~1MB     │ ~564KB   │
  │ CCM/TCM  │ 64KB CCM │ None     │ 272KB TCM│ 192KB TCM│ 192KB TCM│ 192KB TCM│
  ├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
  │ UARTs    │ 6        │ 3        │ 8        │ 10       │ 10       │ 8        │
  │ SPI      │ 3        │ 5        │ 6        │ 6        │ 6        │ 6        │
  │ I2C      │ 3        │ 3        │ 4        │ 4        │ 4        │ 4        │
  │ CAN      │ 2 (CAN)  │ None     │ 1 (CAN)  │ 2 FD ✓   │ 2 FD ✓   │ 2 FD ✓   │
  │ USB HS   │ w/ext PHY│ FS only  │ w/ext PHY│ w/ext PHY│ w/ext PHY│ Built-in!│
  │ ADC bits │ 12-bit   │ 12-bit   │ 12-bit   │ 16-bit ✓ │ 16-bit ✓ │ 16-bit ✓ │
  │ Ethernet │ No       │ No       │ No       │ Yes      │ Yes      │ Yes      │
  ├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
  │ ~Price $ │ $5–8     │ $2–4     │ $6–10    │ $10–15   │ $5–7     │ $8–12    │
  │ FC use   │ ★★★★★   │ ★★☆☆☆   │ ★★★★☆   │ ★★★★★   │ ★★★★☆   │ ★★★★☆   │
  │ Learning │ ★★★★☆   │ ★★★★★   │ ★★★☆☆   │ ★★★☆☆   │ ★★☆☆☆   │ ★★★☆☆   │
  └──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
```

---

## Part 5 — Which Chip Should YOU Use?

```
  Decision tree for your situation:

  START → "What am I building?"
      │
      ├── "Learning STM32 for the first time"
      │        → STM32F411 (Black Pill, cheap, full F4 experience)
      │           OR
      │           STM32 Nucleo-F411RE (has ST-Link built in)
      │
      ├── "Building a DIY flight controller from scratch"
      │        → STM32F405 (proven, tons of Betaflight target examples,
      │                    6 UARTs, enough for everything you need)
      │
      ├── "Building an FC with more features (HD video, datalink)"
      │        → STM32F722 (if budget-conscious)
      │           OR
      │           STM32H743 (if you want headroom)
      │
      ├── "Building the most capable FC possible"
      │        → STM32H743 (2MB flash, 1MB RAM, 480MHz, proven in Betaflight)
      │
      ├── "Building a product (manufacturing 100+ units)"
      │        → STM32H750 (same as H743, cheaper if QSPI complexity acceptable)
      │
      └── "Need fastest clock, integrated USB-HS, 1MB flash enough"
               → STM32H723
```

---

## Part 6 — Where Each Chip Sits on the Learning Path

```
  ────────────────────────────────────────────────────────────►
  Beginner                                              Expert
  
  [F411 Black Pill]  [F405 DIY FC]  [F722 FC]  [H743/H723 FC]
       │                  │               │            │
  Learn the            Build first    Add more     Top-end
  Cortex-M4F           real FC        features     capability
  basics               (Stage 3-4)    (Stage 5-6)  (Stage 6+)
```

---

## Quick Reference — Finding Chip Documents

| Document | Where to find |
|----------|--------------|
| F405 Reference Manual | RM0090 at st.com (free download) |
| F411 Reference Manual | RM0383 at st.com |
| F722 Reference Manual | RM0431 at st.com |
| H743 Reference Manual | RM0433 at st.com (1700+ pages!) |
| H750 Reference Manual | RM0433 (same as H743) |
| H723 Reference Manual | RM0468 at st.com |
| Betaflight F405 target | github.com/betaflight/betaflight → src/main/target/ |
| Betaflight H743 target | github.com/betaflight/betaflight → same folder |

> **Next step:** Pick the chip that matches your stage. If you're just starting with STM32, buy a Black Pill (F411) or Nucleo board. Read RM0383. Configure one LED, one UART, one timer. The H7 will still be there when you're ready for it.
