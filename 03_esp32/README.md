# Module 03 — ESP32 Basics

Time to write real code. This module gets your ESP32 set up and working, starting with the "Hello World" of hardware: blinking an LED.

## Topics

| File | What You'll Learn |
|------|-------------------|
| [01 — ESP32 Pinout & Board Overview](01_esp32_pinout.md) | Physical pins, their functions, what's safe to use |
| [02 — Toolchain Setup](02_toolchain_setup.md) | Arduino IDE, drivers, first upload |
| [03 — Hello World: Blink](03_blink.md) | Your first program — digital output |
| [04 — Digital Input: Reading a Button](04_digital_input.md) | GPIO as input, pull-ups, debouncing |
| [05 — Analog Input: ADC](05_analog_input.md) | Reading sensors that output varying voltage |
| [06 — PWM: Dimming LEDs & Servo Control](06_pwm.md) | Pulse Width Modulation — essential for motors |
| [07 — Serial Communication (UART)](07_uart.md) | Debugging output, talking to other devices |
| [08 — 38-Pin Board: WiFi, BLE & Full Pinout](08_esp32_38pin_wifi_ble.md) | WiFi STA/AP, UDP telemetry, BLE UART, OTA updates, ADC2 + WiFi conflict |
| [09 — LOLIN32 D1 (CP2102)](09_lolin32_d1_board.md) | Compact board, LiPo charging, CP2102 driver, battery monitor, transmitter build |

## Hardware Needed
- ESP32 DevKit V1 (38-pin recommended)
- USB Micro-B cable
- Breadboard
- LEDs + 220Ω resistors
- Push button
- 10kΩ resistor (for pull-down, optional — ESP32 has internal pull-ups)
- Potentiometer (10kΩ) — for analog input lesson

## Goal

By the end of this module you will be able to write programs that read inputs and control outputs on real hardware.

## Time Estimate

4–6 hours (includes hands-on wiring and coding)
