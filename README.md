# DIY Embedded Systems & Drone Electronics

A self-paced learning guide from zero to building your own drone — starting with
the fundamentals of electricity and working all the way up through transistors,
logic gates, CPU architecture, assembly, ESP32 programming, sensors, and drone electronics.

---

## Quick Start — Browse the Docs

The entire repo converts to a local website with one command:

```bash
make docs
```

Then open **http://localhost:8080** in your browser.
Every markdown file becomes a searchable, navigable HTML page with a sidebar.

```bash
make docs-build    # generate site/ only (no server)
make docs-serve    # build + start server (same as make docs)
make docs-clean    # delete generated site/
make help          # list all targets
```

> **No dependencies needed** — uses only Python 3 stdlib.
> Change the port: `make docs PORT=9000`

---

## Requirements & Dependencies

### To read the docs (zero install)

Just a browser. Clone the repo, run `make docs`.

### Software dependencies

| Tool | Version | Purpose | Install |
|------|---------|---------|---------|
| Python 3 | 3.8+ | `make docs` HTML builder | Usually pre-installed on Linux/macOS |
| Git | any | clone / version control | `sudo apt install git` |
| Make | any | run `make` commands | `sudo apt install make` |

### Hardware (from Module 03 onward)

| Item | Purpose |
|------|---------|
| ESP32 dev board (38-pin or 30-pin) | Main microcontroller |
| USB-A to Micro-USB cable | Programming & power |
| Breadboard (830 tie-points) | Prototyping circuits |
| Jumper wires (M-M, M-F, F-F assortment) | Connections |
| LEDs (assorted) + 220Ω resistors | Basic GPIO output |
| Multimeter (any cheap one) | Measuring voltage & current |

### Optional (for embedded C / assembly work)

| Tool | Install |
|------|---------|
| ESP-IDF (ESP32 toolchain) | `sudo apt install esp-idf` or Espressif installer |
| ARM GCC cross-compiler | `sudo apt install gcc-arm-none-eabi` |
| RISC-V GCC cross-compiler | `sudo apt install gcc-riscv64-linux-gnu` |
| OpenOCD (STM32 debugging) | `sudo apt install openocd` |
| NASM (x86 assembly) | `sudo apt install nasm` |

---

## Learning Roadmap

```
Foundation
├── Module 01 — Electronics Fundamentals
│     Voltage, current, resistance, components, circuits, radio & signals
│
├── Module 02 — Microcontrollers & Computer Architecture
│     ├── What is a microcontroller vs CPU
│     ├── How a computing chip is built (transistor → logic gate → CPU)  ← NEW
│     └── Assembly programming (x86, ARM, RISC-V) from scratch           ← NEW
│
├── Module 03 — ESP32 Basics
│     Board overview, toolchain, GPIO, UART, PWM, ADC
│
├── Module 04 — Communication Protocols
│     UART, I2C, SPI, nRF24L01 wireless
│
├── Module 05 — Sensors & Modules
│     MPU6050 IMU, BMP280 barometer, ultrasonic, compass, OLED
│
├── Module 06 — Power Systems
│     LiPo batteries, voltage regulators, power design
│
├── Module 07 — Drone Electronics
│     Brushless motors, ESC, flight controller, PID tuning, radio
│
└── Module 08 — Projects
      Real builds tying all modules together
```

---

## Modules

- [01 — Electronics Fundamentals](01_electronics_fundamentals/README.md)
- [02 — Microcontrollers & Architecture](02_microcontrollers/README.md)
  - [How a chip works: Transistor → Logic Gate → CPU](02_microcontrollers/06_from_transistor_to_cpu.md)
  - [Assembly Programming Guide (x86 / ARM / RISC-V)](02_microcontrollers/05_assembly_programming_guide.md)
- [03 — ESP32 Basics](03_esp32/README.md)
- [04 — Communication Protocols](04_communication_protocols/README.md)
- [05 — Sensors & Modules](05_sensors_and_modules/README.md)
- [06 — Power Systems](06_power_systems/README.md)
- [07 — Drone Electronics](07_drone_electronics/README.md)
- [08 — Projects](08_projects/README.md)

---

## Repo Layout

```
embedded_system/
├── Makefile                         ← make docs / make docs-clean
├── scripts/
│   └── build_docs.py                ← pure-Python MD→HTML converter
├── site/                            ← generated (git-ignored)
├── 01_electronics_fundamentals/
├── 02_microcontrollers/
├── 03_esp32/
├── 04_communication_protocols/
├── 05_sensors_and_modules/
├── 06_power_systems/
├── 07_drone_electronics/
└── 08_projects/
```
