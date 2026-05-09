# 02 — Common Microcontrollers Compared

You'll encounter several MCU families in the embedded/drone world. Here's how they compare so you can understand why we use the ESP32 in this course.

---

## The Main Players

### Arduino Uno (ATmega328P)
- **CPU:** 8-bit AVR, 16 MHz
- **RAM:** 2 KB SRAM
- **Flash:** 32 KB
- **Voltage:** 5V logic
- **Connectivity:** None built-in (needs shields)
- **Best for:** Learning basics, simple projects
- **Limitation:** Too slow and limited for drones; no WiFi/BT

### ESP8266 (NodeMCU)
- **CPU:** 32-bit Xtensa LX106, 80 MHz
- **RAM:** 80 KB
- **Flash:** 4 MB (external)
- **Voltage:** 3.3V logic
- **Connectivity:** WiFi built-in
- **Best for:** Simple IoT projects
- **Limitation:** Single core, no Bluetooth, fewer GPIO

### ESP32 (our choice)
- **CPU:** 32-bit dual-core Xtensa LX6, up to 240 MHz
- **RAM:** 520 KB SRAM
- **Flash:** 4 MB (external SPI flash)
- **Voltage:** 3.3V logic
- **Connectivity:** WiFi + Bluetooth 4.2 + BLE built-in
- **GPIO:** 34 programmable pins
- **Best for:** IoT, drones, robotics, data logging

### STM32 (F4 series)
- **CPU:** 32-bit ARM Cortex-M4, up to 180 MHz + FPU
- **RAM:** 192 KB
- **Flash:** 1 MB on-chip
- **Voltage:** 3.3V logic
- **Connectivity:** None built-in
- **Best for:** High-performance flight controllers (Betaflight runs on STM32)
- **Limitation:** Steeper learning curve, more complex toolchain

### Raspberry Pi RP2040
- **CPU:** 32-bit dual ARM Cortex-M0+, 133 MHz
- **RAM:** 264 KB SRAM
- **Flash:** External
- **Voltage:** 3.3V logic
- **Connectivity:** None built-in (Pi Pico W adds WiFi)
- **Best for:** Projects needing precise timing, PIO state machines

### Raspberry Pi (4B / Zero 2W)
- **CPU:** Full ARM Cortex-A (1.5–1.8 GHz, quad-core)
- **RAM:** 1–8 GB
- **OS:** Linux
- **Connectivity:** WiFi, Bluetooth, USB, HDMI, Ethernet
- **Best for:** Computer vision on drones, companion computers
- **Note:** This is a single-board computer, not a microcontroller

---

## Comparison Table

| Board | Speed | RAM | WiFi | BT | Cost (approx ₹) | Drone use |
|-------|-------|-----|------|-----|-----------------|-----------|
| Arduino Uno | 16 MHz | 2 KB | ✗ | ✗ | ₹500 | Learning only |
| ESP8266 | 80 MHz | 80 KB | ✓ | ✗ | ₹200 | Simple IoT |
| **ESP32** | **240 MHz** | **520 KB** | **✓** | **✓** | **₹300–500** | **Great for DIY flight controller** |
| STM32F4 | 180 MHz | 192 KB | ✗ | ✗ | ₹800–2000 | Pro flight controllers |
| RP2040 | 133 MHz | 264 KB | ✗ | ✗ | ₹300–600 | Timing-critical tasks |
| RPi 4B | 1800 MHz | 1–8 GB | ✓ | ✓ | ₹4000–8000 | Companion computer |

---

## Why ESP32 for This Course

1. **Dual core** — run flight control on one core, communication on the other
2. **WiFi + BLE built-in** — wireless telemetry, configuration without extra hardware
3. **Rich peripherals** — I2C, SPI, UART, ADC, DAC, PWM, touch — everything you need
4. **3.3V logic** — compatible with almost all modern sensors
5. **Large community** — massive documentation, libraries, and examples
6. **Cheap** — ₹300–500 for a full dev board
7. **Arduino IDE compatible** — easiest development experience for beginners

### The ESP32 Family

There are several ESP32 variants:

| Chip | Cores | Special feature |
|------|-------|----------------|
| ESP32 (original) | 2 × LX6 | Most common, our choice |
| ESP32-S2 | 1 × LX7 | USB native, no BT |
| ESP32-S3 | 2 × LX7 | AI acceleration, USB |
| ESP32-C3 | 1 × RISC-V | Ultra cheap, single core |
| ESP32-H2 | 1 × RISC-V | Zigbee/Thread support |

For this course, the **original ESP32** (ESP-WROOM-32 module on a dev board) is what we'll use.

---

## Popular ESP32 Dev Board Variants

A "dev board" takes the raw ESP32 chip and adds USB programming, voltage regulators, and breakout pins for easy use.

| Board | Pins | Notes |
|-------|------|-------|
| ESP32 DevKit V1 (30-pin) | 30 | Smaller, fits standard breadboard |
| ESP32 DevKit V1 (38-pin) | 38 | More GPIO exposed |
| NodeMCU-32S | 38 | Similar to 38-pin DevKit |
| LOLIN D32 | 32 | Has LiPo charger built in |
| FireBeetle ESP32 | 24 | Ultra-low power design |

**Recommendation:** Start with a **38-pin ESP32 DevKit V1**. It's cheap, widely available on Amazon India / Robu.in / Electronicscomp, and every tutorial in this course targets it.

---

## Practice

1. A commercial flight controller uses STM32F405. Why would a manufacturer choose STM32 over ESP32 for this application? (Think: FPU, timing precision, certifications)

2. You want to build a drone that streams live video over WiFi to your phone. Would you use the ESP32 alone, or pair it with a Raspberry Pi? Why?

3. The ESP32 has 520KB of RAM. An Arduino Uno has 2KB. How many times more RAM does the ESP32 have?

**Answers:**
1. STM32F4 has a hardware Floating Point Unit (FPU) critical for fast PID calculations, deterministic timing with no WiFi interference, and is used in certified aerospace-adjacent products. Also has more on-chip Flash.
2. Both — ESP32 for flight control, Raspberry Pi as companion computer for camera processing and streaming.
3. 520,000 / 2,000 = 260× more RAM

---

**Module 02 complete.**

**Next module:** [03 — ESP32 Basics](../03_esp32/README.md)
