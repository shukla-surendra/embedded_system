# Module 04 — Communication Protocols

Microcontrollers talk to sensors and modules over standardized buses. Understanding I2C, SPI, and UART lets you connect almost any sensor to the ESP32.

## Topics

| File | What You'll Learn | Status |
|------|-------------------|--------|
| 01 — I2C | Two-wire bus, addresses, scanning, connecting IMU | Coming soon |
| 02 — SPI | Four-wire fast bus, SD cards, displays | Coming soon |
| 03 — UART (advanced) | Multi-device serial, parsing protocols | Coming soon |
| [04_nrf24l01_wireless.md](04_nrf24l01_wireless.md) | 2.4 GHz radio link, TX/RX roles, failsafe, telemetry | Done |
| [05_nrf24l01_adapter_board.md](05_nrf24l01_adapter_board.md) | Adapter board wiring, 5V in → 3.3V out, why it fixes connection failures | Done |

## Prerequisites
- Module 03 complete — you can upload code and use Serial Monitor

## Goal

Connect real sensor modules (IMU, barometer, display) over I2C and SPI.
