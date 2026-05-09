# 01 — ESP32 Pinout & Board Overview

## The ESP32 DevKit V1 (38-pin)

```
                    ┌──────────────────┐
                    │   ESP32 DevKit   │
             3V3 ───┤ 3V3          GND ├─── GND
              EN ───┤ EN         GPIO23├─── MOSI (SPI)
          GPIO36 ───┤ VP         GPIO22├─── I2C SCL
          GPIO39 ───┤ VN         GPIO01├─── UART TX (USB)
          GPIO34 ───┤ GPIO34     GPIO03├─── UART RX (USB)
          GPIO35 ───┤ GPIO35     GPIO21├─── I2C SDA
          GPIO32 ───┤ GPIO32     GPIO19├─── MISO (SPI)
          GPIO33 ───┤ GPIO33     GPIO18├─── SCK (SPI)
          GPIO25 ───┤ GPIO25      GPIO5├─── SPI CS
          GPIO26 ───┤ GPIO26     GPIO17├─── UART2 TX
          GPIO27 ───┤ GPIO27     GPIO16├─── UART2 RX
          GPIO14 ───┤ GPIO14      GPIO4├─── 
          GPIO12 ───┤ GPIO12      GPIO0├─── BOOT button
             GND ───┤ GND        GPIO2├─── Onboard LED
          GPIO13 ───┤ GPIO13     GPIO15├─── 
             VIN ───┤ VIN        GPIO8├─── (SPI Flash — avoid)
                    └──────────────────┘
                           │ USB │
```

---

## Pin Categories

### Power Pins
| Pin | Voltage | Max Current | Use |
|-----|---------|-------------|-----|
| 3V3 | 3.3V | 500mA | Power sensors from the on-board regulator |
| VIN | 5V (from USB) | 1A | Alternative power input |
| GND | 0V | — | Ground reference |

> Never apply 5V to a GPIO pin — the ESP32 is a **3.3V device**. 5V on a GPIO pin will damage it.

### GPIO Pins — Safe to Use Freely
These are general-purpose and have no special restrictions:

```
GPIO4, GPIO5, GPIO12*, GPIO13, GPIO14*, GPIO15*,
GPIO16, GPIO17, GPIO18, GPIO19, GPIO21, GPIO22,
GPIO23, GPIO25, GPIO26, GPIO27, GPIO32, GPIO33
```

*Boot-sensitive: these have internal pull-ups/pull-downs that affect boot. Avoid driving them HIGH on startup.

### Input-Only Pins (no output)
These can only be used as inputs. They do NOT have internal pull-up/pull-down resistors.

```
GPIO34, GPIO35, GPIO36 (VP), GPIO39 (VN)
```

Use these for sensors that output analog or digital signals.

### Special Pins — Handle with Care

| Pin | Special role | Note |
|-----|-------------|------|
| GPIO0 | BOOT mode select | Must be HIGH during normal operation; LOW = enter flash mode |
| GPIO1 | UART0 TX | USB serial — avoid using as GPIO |
| GPIO3 | UART0 RX | USB serial — avoid using as GPIO |
| GPIO2 | Onboard LED | Also boot-sensitive; LOW at boot is fine |
| GPIO6–11 | SPI Flash | Connected to internal flash chip — **do not use** |
| EN | Enable/Reset | Active-HIGH enable. Pulling LOW resets the ESP32 |

---

## Onboard Features

### Onboard LED
Most DevKit boards have a blue LED connected to **GPIO2**. Write HIGH to turn it on.

### Boot & Reset Buttons
- **BOOT (GPIO0):** Hold during power-up to enter download (flash) mode
- **EN:** Reset button. Press to restart the program without reprogramming

### USB-Serial Chip
The DevKit has a CP2102 or CH340C chip that converts USB to UART for programming. This is why you can program the ESP32 over a regular USB cable.

---

## GPIO Electrical Specifications

| Property | Value |
|----------|-------|
| Logic HIGH voltage | 3.3V |
| Logic LOW voltage | 0V |
| Max output current per pin | 12 mA (40 mA absolute max — don't push it) |
| Max total current (all GPIO) | 1200 mA (but power supply limits this more) |
| Input voltage range | 0–3.3V (never exceed 3.3V) |

**Consequence:** You can power a single LED (10mA) directly from a GPIO pin. You cannot power a motor, relay, or buzzer directly — you need a driver circuit.

---

## GPIO Functions (Multiplexing)

Every GPIO pin can serve multiple functions. The software configures which function is active:

```
GPIO22 can be:
  ├─ Digital output (HIGH/LOW)
  ├─ Digital input
  ├─ I2C SCL (clock line for I2C bus)
  └─ PWM output
```

This is called **pin multiplexing**. You configure it in code with the appropriate library function.

---

## ADC Pins (Analog to Digital Converter)

These pins can read analog voltage (0V–3.3V) and convert it to a number (0–4095 for 12-bit ADC):

```
ADC1: GPIO32, GPIO33, GPIO34, GPIO35, GPIO36, GPIO39
ADC2: GPIO0, GPIO2, GPIO4, GPIO12–GPIO15, GPIO25–GPIO27
```

**Important gotcha:** ADC2 pins are shared with WiFi. When WiFi is on, **ADC2 pins don't work reliably**. Always use ADC1 pins when WiFi is active.

---

## DAC Pins (Digital to Analog)
Two pins can output a true analog voltage:
- **GPIO25** — DAC1
- **GPIO26** — DAC2

Outputs 0–3.3V as a continuous analog signal. Useful for audio or analog control signals.

---

## Touch Pins (Capacitive Touch)
The ESP32 can detect finger touch on these pins (no button needed):
```
GPIO4, GPIO0, GPIO2, GPIO15, GPIO13, GPIO12, GPIO14, GPIO27, GPIO33, GPIO32
```

---

## Quick Pin Selection Guide

| Need | Use |
|------|-----|
| Digital output (LED, relay) | GPIO4, 13, 16, 17, 18, 19, 21, 25, 26, 27, 32, 33 |
| Digital input (button) | Same as above + GPIO34, 35, 36, 39 |
| Analog input (potentiometer, sensor) | GPIO32–36, 39 (ADC1) |
| I2C bus | GPIO21 (SDA), GPIO22 (SCL) — convention |
| SPI bus | GPIO18 (SCK), GPIO19 (MISO), GPIO23 (MOSI), GPIO5 (CS) |
| UART (second serial) | GPIO16 (RX), GPIO17 (TX) |
| PWM (motors, servos) | Any digital output pin |
| Avoid completely | GPIO1, GPIO3, GPIO6–GPIO11 |

---

## Practice

1. You want to connect 3 LEDs and 1 button to your ESP32. Pick appropriate pins for each from the safe list above.

2. Why can't you connect a 5V sensor's signal output directly to an ESP32 GPIO pin?

3. You're building a drone that will use WiFi for telemetry. You want to read an analog battery voltage sensor. Which ADC channel group should you use — ADC1 or ADC2? Why?

**Answers:**
1. Example: LEDs on GPIO4, GPIO16, GPIO17; button on GPIO18 (any safe pins work)
2. The ESP32 GPIO is rated for 0–3.3V max. 5V would exceed this and could permanently damage the pin or chip. You'd need a voltage divider or level shifter.
3. ADC1 (GPIO32–36, 39) — ADC2 is shared with WiFi and gives unreliable readings when WiFi is active.

---

**Next:** [02 — Toolchain Setup](02_toolchain_setup.md)
