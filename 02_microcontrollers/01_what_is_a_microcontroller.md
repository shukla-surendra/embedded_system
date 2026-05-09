# 01 — What Is a Microcontroller?

## The Short Answer

A **microcontroller (MCU)** is a tiny, self-contained computer built into a single chip. Unlike the processor in your laptop, a microcontroller includes:
- The processor (CPU core)
- Memory (both program storage and working memory)
- Input/Output pins (to talk to the outside world)

...all on one chip, designed to run one program, forever.

---

## Microcontroller vs. Computer

| Feature | Your Laptop | Microcontroller (ESP32) |
|---------|------------|------------------------|
| CPU speed | 2–4 GHz | 80–240 MHz |
| RAM | 8–32 GB | 520 KB |
| Storage | 256 GB SSD | 4 MB Flash |
| OS | Windows / Linux | None (bare metal) or tiny RTOS |
| Boots in | ~10 seconds | ~100 milliseconds |
| Power use | 15–45W | 0.01–0.5W |
| Cost | ₹40,000+ | ₹200–₹500 |
| Purpose | General purpose | One specific task |

A microcontroller trades raw power for **simplicity, low power, small size, and low cost**. Your drone's flight controller, your car's ABS system, your microwave's timer — all microcontrollers.

---

## What's Inside a Microcontroller

### 1. CPU Core
Executes instructions. Common architectures:
- **AVR** — used in classic Arduino Uno (8-bit, slow, simple)
- **ARM Cortex-M** — used in ESP32, STM32, Arduino Due (32-bit, modern, fast)
- **Xtensa LX6/LX7** — ESP32's specific CPU (32-bit dual-core)

### 2. Flash Memory (Program Storage)
This is where your code lives permanently. It keeps the program even when power is removed — like a hard drive for your program.

The ESP32 has 4MB of flash. Your compiled program typically fits in 100–500 KB.

### 3. SRAM (Working Memory / RAM)
Where variables and runtime data live. Lost when power is off. 

The ESP32 has 520 KB of SRAM. A variable like `int sensorValue = 0;` lives here.

### 4. GPIO Pins (General Purpose Input/Output)
Physical metal pins on the chip that you can:
- **Read as input** — detect if a button is pressed, read a sensor
- **Write as output** — turn on an LED, control a motor driver

Each pin can typically be set to HIGH (3.3V) or LOW (0V).

### 5. Peripherals (Built-in Hardware Modules)
Modern MCUs include hardware modules for common tasks. The ESP32 has:

| Peripheral | What it does |
|-----------|-------------|
| UART | Serial communication (USB, GPS, Bluetooth serial) |
| I2C | Two-wire bus for sensors (IMU, barometer) |
| SPI | Fast four-wire bus for displays, SD cards |
| ADC | Analog-to-Digital Converter — reads analog voltage (sensors) |
| DAC | Digital-to-Analog Converter — outputs analog voltage |
| PWM | Pulse Width Modulation — controls motor speed, LED brightness |
| WiFi | 2.4GHz wireless networking |
| Bluetooth | BLE and classic Bluetooth |
| Timer | Precise time measurement, interrupts |

Each of these is covered in its own module.

---

## How a Microcontroller Program Works

Unlike your computer that runs many programs simultaneously, a microcontroller runs **one program** that loops forever:

```c
// This is the structure of every Arduino/ESP32 program

void setup() {
    // Runs ONCE when power is applied or reset button is pressed
    // Initialize pins, serial port, sensors here
}

void loop() {
    // Runs FOREVER, over and over, as fast as possible
    // Read sensors, make decisions, control outputs
}
```

The "loop" runs thousands or millions of times per second. Each pass through the loop:
1. Read inputs (sensors, buttons, communication)
2. Process data (run algorithms, make decisions)
3. Write outputs (move motors, light LEDs, send data)

This is called the **sense → think → act** cycle.

---

## Memory Types Summary

```
┌─────────────────────────────────────────────────┐
│                 MICROCONTROLLER                 │
│                                                 │
│  ┌──────────┐  ┌──────────┐  ┌──────────────┐  │
│  │  Flash   │  │  SRAM    │  │  Peripherals │  │
│  │          │  │          │  │              │  │
│  │ Program  │  │Variables │  │ UART, SPI,   │  │
│  │ storage  │  │ Runtime  │  │ I2C, PWM,    │  │
│  │          │  │ data     │  │ ADC, WiFi... │  │
│  │Persists  │  │ Lost on  │  │              │  │
│  │ on power │  │ power off│  │              │  │
│  │   off    │  │          │  │              │  │
│  └──────────┘  └──────────┘  └──────────────┘  │
│                                                 │
│  ┌─────────────────────────────────────────┐    │
│  │              CPU Core                   │    │
│  │   Reads Flash → Executes instructions   │    │
│  │   Reads/writes SRAM for variables       │    │
│  │   Controls peripherals                  │    │
│  └─────────────────────────────────────────┘    │
│                                                 │
└────────────────────┬────────────────────────────┘
                     │
              GPIO Pins (connect to real world)
```

---

## Practice

1. Your ESP32 has 520KB of SRAM. You declare an array: `uint8_t buffer[1000];` (1000 bytes). How much SRAM is left?

2. If the power is cut to an ESP32 mid-operation, what happens to: (a) your program code, (b) the current sensor readings stored in variables?

3. Name three built-in peripherals of the ESP32 that will be useful in a drone (think: sensors communicate using I2C/SPI, motors need PWM, etc.)

**Answers:**
1. 520,000 – 1,000 = 519,000 bytes ≈ 507 KB remaining
2. (a) Code in Flash is preserved — it's non-volatile. (b) Variable values in SRAM are lost.
3. I2C (for IMU/barometer), PWM (for ESC/motor speed control), UART (for GPS), WiFi/BLE (for remote control)

---

**Next:** [02 — Common Microcontrollers Compared](02_common_microcontrollers.md)
