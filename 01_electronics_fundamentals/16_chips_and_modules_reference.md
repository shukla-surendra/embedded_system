# Electronic Chips and Modules — Complete Reference

A practical catalogue of the most common ICs and modules used in embedded systems, robotics, and drone projects. Organised by function category. Each entry covers what the chip does, key specs, typical wiring, and where you'd use it.

---

## How to Read This Document

- **IC** = Integrated Circuit — a chip you solder onto a PCB.
- **Module** = a PCB with the IC already mounted, plus supporting components (decoupling caps, crystal, voltage regulator, connectors). Easier to prototype with.
- **Package** = the physical form factor of the chip (DIP = through-hole, SOIC/QFP/QFN = surface-mount).
- **Interface** = how the chip talks to your microcontroller (GPIO, UART, SPI, I2C, PWM, Analog).

---

## Category Index

1. [Microcontrollers](#1-microcontrollers)
2. [Wireless / RF Modules](#2-wireless--rf-modules)
3. [Motor Driver ICs](#3-motor-driver-ics)
4. [Power Management ICs](#4-power-management-ics)
5. [Sensor ICs — IMU / Motion](#5-sensor-ics--imu--motion)
6. [Sensor ICs — Environmental](#6-sensor-ics--environmental)
7. [Sensor ICs — Distance / Proximity](#7-sensor-ics--distance--proximity)
8. [GPS Modules](#8-gps-modules)
9. [Display Driver ICs](#9-display-driver-ics)
10. [ADC and DAC ICs](#10-adc-and-dac-ics)
11. [Communication Interface ICs](#11-communication-interface-ics)
12. [Logic and Utility ICs](#12-logic-and-utility-ics)
13. [Amplifier ICs](#13-amplifier-ics)
14. [Voltage Reference ICs](#14-voltage-reference-ics)
15. [Memory ICs](#15-memory-ics)
16. [Clock and Timer ICs](#16-clock-and-timer-ics)
17. [Audio ICs](#17-audio-ics)
18. [Optocouplers and Isolation ICs](#18-optocouplers-and-isolation-ics)

---

## 1. Microcontrollers

Microcontrollers are the brain — they run your code, control peripherals, and communicate with other chips.

---

### ESP32 (Espressif)
| Item       | Detail                                                       |
|------------|--------------------------------------------------------------|
| Core       | Dual-core Xtensa LX6, 240 MHz                                |
| Flash      | 4 MB (module-dependent)                                      |
| RAM        | 520 KB SRAM                                                  |
| Wireless   | WiFi 802.11 b/g/n (2.4 GHz) + Bluetooth 4.2 / BLE           |
| GPIO       | 34 programmable pins                                         |
| Peripherals| 2× UART, 2× SPI, 2× I2C, 16× PWM, 18× ADC (12-bit), 2× DAC |
| Supply     | 3.3V (module has onboard 3.3V regulator, input 5V via USB)  |
| Package    | Module (ESP32-WROOM, ESP32-WROVER) or bare chip              |
| Use        | WiFi/BLE IoT devices, drone flight controller, remote control, telemetry |

**Common variants:**
- **ESP32-WROOM-32**: standard module, 4 MB flash, no PSRAM.
- **ESP32-WROVER**: adds 8 MB PSRAM — needed for camera projects.
- **ESP32-S3**: newer, faster, USB-OTG, better AI acceleration.
- **ESP32-C3**: single-core RISC-V, cheaper, WiFi + BLE only.

---

### Arduino Uno (ATmega328P)
| Item       | Detail                                                      |
|------------|-------------------------------------------------------------|
| Core       | AVR ATmega328P, 16 MHz                                      |
| Flash      | 32 KB                                                       |
| RAM        | 2 KB SRAM                                                   |
| GPIO       | 14 digital, 6 analog (10-bit ADC)                           |
| Peripherals| 1× UART, 1× SPI, 1× I2C, 6× PWM                           |
| Supply     | 5V logic (3.3V also available)                              |
| Use        | Learning, simple control tasks, motor control, sensor reading |

---

### Arduino Nano (ATmega328P)
- Same chip as Uno in a smaller package.
- Breadboard-friendly — fits directly on a breadboard.
- Popular for compact builds where Uno is too large.

---

### STM32F103C8T6 ("Blue Pill")
| Item       | Detail                                                      |
|------------|-------------------------------------------------------------|
| Core       | ARM Cortex-M3, 72 MHz                                       |
| Flash      | 64 KB (128 KB on some variants)                             |
| RAM        | 20 KB SRAM                                                  |
| GPIO       | 37 pins                                                     |
| Peripherals| 3× UART, 2× SPI, 2× I2C, 7× PWM timers, 10× ADC (12-bit) |
| Supply     | 3.3V                                                        |
| Use        | Flight controllers (Betaflight runs on STM32), motor control, real-time tasks |

**Used in:** Most commercial drone flight controllers (F1–F7 series boards).

---

### STM32F4 / STM32F7 / STM32H7 Series
- More powerful STM32 variants used in advanced flight controllers.
- F4: 168 MHz, FPU — used in Pixhawk, older Betaflight FC boards.
- F7: 216 MHz — used in mid-range Betaflight boards.
- H7: 480 MHz, dual-core — used in high-end FC boards (Matek H743, etc.).

---

### RP2040 (Raspberry Pi Pico)
| Item       | Detail                                                      |
|------------|-------------------------------------------------------------|
| Core       | Dual-core ARM Cortex-M0+, 133 MHz                           |
| Flash      | External (2 MB on Pico)                                     |
| RAM        | 264 KB SRAM                                                 |
| GPIO       | 26 usable pins, 3× ADC (12-bit)                             |
| Peripherals| 2× UART, 2× SPI, 2× I2C, 16× PWM, 8× PIO (programmable I/O)|
| Supply     | 3.3V                                                        |
| Use        | Real-time I/O, custom protocols via PIO, robotics, audio    |

---

### ATtiny85
- 8-pin AVR microcontroller — tiny and cheap.
- 8 KB flash, 512 B RAM, 6 usable GPIO.
- Used when you need a tiny dedicated controller for a single task (e.g., LED animation, simple sensor interface).

---

## 2. Wireless / RF Modules

---

### nRF24L01+ (Nordic Semiconductor)
| Item       | Detail                                                      |
|------------|-------------------------------------------------------------|
| Frequency  | 2.400 – 2.525 GHz (125 channels, 1 MHz spacing)            |
| Modulation | GFSK                                                        |
| Data rate  | 250 kbps / 1 Mbps / 2 Mbps                                 |
| Range      | ~100 m (standard), ~1000 m (PA+LNA version)                |
| Interface  | SPI (up to 10 MHz)                                          |
| Supply     | 1.9 – 3.6V (use 3.3V; do NOT connect to 5V directly)       |
| Current    | 11.3 mA TX, 12.3 mA RX, 900 nA sleep                       |
| Use        | RC drone control links, wireless sensor networks, 2-way data |

**PA+LNA version (nRF24L01+PA+LNA):** adds external power amplifier and LNA — boosts range to ~1 km line-of-sight.

---

### HC-12 (UART Radio Module)
| Item       | Detail                                                      |
|------------|-------------------------------------------------------------|
| Frequency  | 433 MHz                                                     |
| Data rate  | 1200 – 115200 bps (configurable)                            |
| Range      | ~1000 m open air (100 mW version)                           |
| Interface  | UART (transparent — send bytes in, they appear at the other end) |
| Supply     | 3.2 – 5.5V                                                  |
| Use        | Long-range telemetry, simple wireless UART link             |

---

### LoRa (SX1276 / SX1278) — e.g. Ra-02 module
| Item       | Detail                                                      |
|------------|-------------------------------------------------------------|
| Frequency  | 433 MHz (SX1278) / 868–915 MHz (SX1276)                    |
| Modulation | LoRa (Chirp Spread Spectrum — extremely noise resistant)    |
| Data rate  | 0.3 – 37.5 kbps                                             |
| Range      | 2 – 15 km line-of-sight (low data rate)                    |
| Interface  | SPI                                                         |
| Supply     | 3.3V                                                        |
| Use        | Long-range drone telemetry (ExpressLRS uses SX127x), IoT networks |

**ExpressLRS (ELRS):** Open-source drone RC link protocol built on SX127x / SX128x LoRa chips — extremely low latency, very long range.

---

### ESP8266 (Espressif)
| Item       | Detail                                                      |
|------------|-------------------------------------------------------------|
| Core       | Tensilica L106, 80/160 MHz                                  |
| Wireless   | WiFi 802.11 b/g/n                                           |
| GPIO       | 11 usable (limited compared to ESP32)                       |
| Supply     | 3.3V                                                        |
| Use        | Simple WiFi IoT, used as a WiFi add-on for Arduino via AT commands |

**Note:** ESP32 is better for new projects — more GPIO, faster, BLE too. ESP8266 is older but extremely common in existing projects.

---

### Bluetooth Modules

| Module     | Chip         | Type         | Interface | Range   | Use                             |
|------------|--------------|--------------|-----------|---------|----------------------------------|
| HC-05      | CSR BC417    | Classic BT 2.0 | UART    | ~10 m   | Wireless serial, Android apps    |
| HC-06      | CSR BC417    | Classic BT 2.0 (slave only) | UART | ~10 m | Same as HC-05, slave only   |
| HM-10      | TI CC2541    | BLE 4.0      | UART      | ~10 m   | iPhone/Android BLE apps          |
| ESP32 built-in | —        | BT 4.2 + BLE | N/A      | ~10 m   | Preferred — no separate module needed |

---

### NRF52840 (Nordic)
- Powerful BLE 5.0 + 802.15.4 (Zigbee/Thread) SoC.
- Used in: Adafruit Feather nRF52840, nice!nano keyboard controller, OpenMV camera.
- Far more capable than HC-05/HM-10 — handles complex BLE profiles.

---

### CC1101 (Texas Instruments)
- Sub-GHz RF transceiver (315 / 433 / 868 / 915 MHz).
- SPI interface, very low power.
- Used in: home automation, smart meters, some drone RC links, keyfob cloning (security research).

---

## 3. Motor Driver ICs

---

### L298N
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Dual H-bridge                                             |
| Max voltage  | 46V                                                       |
| Max current  | 2A per channel (4A peak)                                 |
| Interface    | IN1/IN2 direction pins + PWM enable pin                  |
| Supply       | Separate logic (5V) and motor power pins                  |
| Package      | Multiwatt-15 (module widely available)                   |
| Use          | DC motor direction and speed control, small stepper motors |
| Limitation   | Large voltage drop (~2V) across the chip — wastes power as heat, not suitable for battery-powered projects |

---

### L293D
- Similar to L298N but smaller, lower current (600 mA/channel).
- Has built-in flyback diodes (L298N does not).
- Used in: small toy motors, low-power robotics.

---

### DRV8833 (Texas Instruments)
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Dual H-bridge                                             |
| Max voltage  | 10.8V                                                     |
| Max current  | 1.5A per channel                                          |
| Interface    | IN1/IN2 per channel (PWM directly on input pins)         |
| Supply       | 2.7 – 10.8V (logic auto-detects 3.3V or 5V)             |
| Advantage    | Very low voltage drop — efficient, no heatsink needed     |
| Use          | Small DC motors, LEGO-scale robots, micro drones         |

---

### DRV8825 / A4988 (Stepper Motor Drivers)
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Stepper motor driver with microstepping                   |
| Max current  | DRV8825: 2.5A / A4988: 2A                                 |
| Microstepping| DRV8825: up to 1/32 step / A4988: up to 1/16 step        |
| Interface    | STEP + DIR pins (pulse STEP to move, DIR sets direction)  |
| Use          | 3D printers (standard in RAMPS/GRBL), CNC machines, camera sliders |

---

### TB6612FNG (Toshiba)
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Dual H-bridge                                             |
| Max voltage  | 15V motor, 5V logic                                       |
| Max current  | 1.2A continuous, 3.2A peak                               |
| Advantage    | Very small, efficient (MOSFET-based — no big voltage drop) |
| Use          | Arduino motor shields, small robots, camera gimbals      |

---

### ESC (Electronic Speed Controller) — for Brushless Motors
Not a single chip — an ESC is a full circuit board containing:
- 6 MOSFETs (3-phase H-bridge)
- MCU (often STM32 or AVR)
- Current sensing
- BEC (Battery Eliminator Circuit — 5V regulator for receiver)

**Interface:** PWM (standard 1000–2000 μs pulse), DSHOT (digital protocol, no calibration needed), UART (for telemetry).

**Common ESC ICs:**
- **FD6288**: 3-phase MOSFET gate driver used in many cheap ESCs.
- **DRV8323**: TI 3-phase gate driver with current sensing — used in quality ESCs.

---

## 4. Power Management ICs

---

### AMS1117 (LDO Voltage Regulator)
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Linear LDO (Low Drop-Out) regulator                       |
| Variants     | AMS1117-3.3 (3.3V), AMS1117-5.0 (5V), adjustable         |
| Max input    | 15V                                                       |
| Max current  | 1A                                                        |
| Dropout      | ~1.2V (input must be ≥ 4.5V to output 3.3V)             |
| Use          | Powering ESP32/Arduino from 5V USB or batteries          |
| Limitation   | Linear — inefficient, wastes excess voltage as heat       |

---

### LM7805 / LM7833 (Linear Regulators)
- Classic 7805: 5V, 1.5A output.
- LM7833: 3.3V, 0.5A.
- Very robust, extremely common, but inefficient (same limitation as AMS1117).

---

### LM2596 (Buck Converter IC)
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Switching step-down (buck) converter                      |
| Input        | 4.5 – 40V                                                 |
| Output       | 1.2 – 37V adjustable (or fixed 3.3V / 5V / 12V versions)|
| Max current  | 3A                                                        |
| Efficiency   | ~75–85%                                                   |
| Use          | Converting drone battery (12V) to 5V for ESP32, powering logic from LiPo |

**Advantage over LM7805:** Switching — much more efficient, less heat. Disadvantage: needs inductor and capacitors, more EMI noise.

---

### MP1584 / LMR14050 (Mini Buck Converters)
- Smaller, higher-efficiency alternatives to LM2596.
- MP1584: up to 3A, very compact — used in mini buck modules.
- LMR14050 (TI SIMPLE SWITCHER): up to 5A, excellent efficiency.

---

### MT3608 / XL6009 (Boost Converter ICs)
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Switching step-up (boost) converter                       |
| MT3608 input | 2 – 24V                                                   |
| MT3608 output| Up to 28V                                                 |
| Max current  | MT3608: 2A / XL6009: 4A                                  |
| Use          | Boosting 3.7V LiPo cell to 5V or 12V                     |

---

### TP4056 (LiPo Charger IC)
| Item         | Detail                                                    |
|--------------|-----------------------------------------------------------|
| Type         | Single-cell Li-Ion / LiPo charger                         |
| Charge current| 1A max (set by resistor)                                 |
| Input        | 4 – 8V (normally 5V USB)                                  |
| Protection   | Needs external DW01A + FS8205A for over/under-voltage protection |
| Interface    | No microcontroller needed — automatic CC/CV charging      |
| Use          | Charging single-cell LiPo/Li-Ion in DIY portable devices |

**Module version:** TP4056 + protection circuit on one small PCB — extremely common in DIY power banks.

---

### IP5306 (Power Bank IC)
- Complete power bank management IC.
- Handles charging, boost conversion, LED indicators, auto-on/off.
- Found in commercial power banks and some ESP32 IoT builds.

---

### BMS ICs (Battery Management System)
| Chip   | Cells | Functions                                         |
|--------|-------|---------------------------------------------------|
| DW01A  | 1S    | Overcurrent, over-charge, over-discharge protection |
| S8254  | 2S    | 2-cell protection                                 |
| BQ24195| 1S    | Advanced charger with fuel gauge interface        |
| BQ34Z100| Multi | Fuel gauge — measures actual remaining capacity  |

---

## 5. Sensor ICs — IMU / Motion

IMU = Inertial Measurement Unit. Measures acceleration and rotation — essential for drone stabilisation.

---

### MPU-6050 (InvenSense / TDK)
| Item        | Detail                                                     |
|-------------|------------------------------------------------------------|
| Type        | 6-DOF IMU (3-axis gyro + 3-axis accelerometer)            |
| Gyro range  | ±250 / ±500 / ±1000 / ±2000 °/s                          |
| Accel range | ±2 / ±4 / ±8 / ±16 g                                     |
| Interface   | I2C (up to 400 kHz) or SPI                                |
| Supply      | 3.3V                                                       |
| DMP         | Built-in Digital Motion Processor — does orientation math |
| Use         | Drone stabilisation (older builds), self-balancing robots, step counters |

**Limitation:** Older chip — known gyro noise issues at high vibration (common on drones). Better alternatives exist.

---

### MPU-9250 / ICM-20948
- MPU-9250: 9-DOF — adds 3-axis magnetometer (compass). Used in drone flight controllers.
- ICM-20948: newer replacement for MPU-9250, lower noise.

---

### BMI270 (Bosch)
| Item        | Detail                                                     |
|-------------|------------------------------------------------------------|
| Type        | 6-DOF IMU (gyro + accelerometer)                          |
| Anti-vibration | Hardware vibration rejection — designed for drones     |
| Interface   | SPI (up to 10 MHz) or I2C                                 |
| Supply      | 1.71 – 3.6V                                               |
| Use         | Modern drone flight controllers (Betaflight, iNav)        |

**This is the IMU used in many current-generation Betaflight flight controllers** — specifically chosen for its low noise under motor vibration.

---

### ICM-42688-P (TDK InvenSense)
- Very low noise 6-DOF IMU — used in high-end racing drone FCs.
- Anti-aliasing filter — reduces vibration noise without losing signal.
- SPI only (up to 24 MHz).

---

### LSM6DSO / LSM6DSR (STMicroelectronics)
- 6-DOF IMU used in many STM32-based flight controllers.
- LSM6DSR: improved version with OIS (Optical Image Stabilisation) output — used in gimbals.

---

### ADXL345 (Analog Devices)
- 3-axis accelerometer only (no gyro).
- ±16g range, I2C or SPI.
- Used for tilt sensing, vibration analysis, shock detection.

---

### QMC5883L / HMC5883L (Magnetometer / Compass)
- 3-axis digital compass — measures Earth's magnetic field.
- I2C interface.
- Used to determine absolute heading (yaw) on drones.
- **QMC5883L:** current common module (clone of HMC5883L).
- Affected by nearby magnetic fields — must be mounted away from motors and power wires.

---

## 6. Sensor ICs — Environmental

---

### DHT11 / DHT22 (Temperature + Humidity)
| Item       | DHT11             | DHT22              |
|------------|-------------------|--------------------|
| Temp range | 0 – 50°C          | -40 – 80°C         |
| Temp accuracy | ±2°C           | ±0.5°C             |
| Humidity   | 20 – 80% RH       | 0 – 100% RH        |
| Interface  | Single-wire (proprietary) | Single-wire |
| Sample rate| 1 sample/sec      | 0.5 sample/sec     |
| Supply     | 3 – 5.5V          | 3 – 5.5V           |
| Use        | Weather station, HVAC monitoring |           |

**Limitation:** Very slow, single-wire protocol is not standard (library required). For faster/better accuracy use BME280.

---

### BME280 (Bosch)
| Item       | Detail                                                     |
|------------|------------------------------------------------------------|
| Measures   | Temperature, humidity, barometric pressure                 |
| Temp range | -40 – 85°C, ±1°C                                          |
| Pressure   | 300 – 1100 hPa, ±1 hPa                                    |
| Altitude   | Derived from pressure — ±1 m resolution in good conditions|
| Interface  | I2C or SPI                                                 |
| Supply     | 1.71 – 3.6V                                               |
| Use        | **Drone barometric altimeter** (altitude hold), weather stations |

**This is a critical drone sensor.** Pressure changes with altitude — the flight controller reads barometric pressure to hold altitude. Rate: ~26 Hz at oversampling x1.

---

### BMP280 (Bosch)
- Like BME280 but no humidity sensor — slightly cheaper.
- Commonly used in drone flight controllers for baro altitude.

---

### MS5611 (TE Connectivity)
- High-resolution barometric pressure sensor.
- Resolution: ~10 cm altitude.
- 24-bit ADC — more accurate than BMP280 for altitude hold.
- I2C or SPI.
- Used in: Pixhawk, high-accuracy flight controllers.

---

### MQ Gas Sensor Series
| Sensor | Target Gas           | Use                          |
|--------|---------------------|------------------------------|
| MQ-2   | LPG, smoke, H₂      | Gas leak detector            |
| MQ-3   | Alcohol             | Breathalyser                 |
| MQ-4   | Methane (natural gas)| Gas pipeline monitoring     |
| MQ-5   | LPG, natural gas    | Home gas detector            |
| MQ-7   | Carbon monoxide (CO)| CO alarm                     |
| MQ-9   | CO + flammable gas  | Multi-gas detector           |
| MQ-135 | Air quality (NH₃, NOx, benzene) | AQI monitor        |

All MQ sensors: analog voltage output proportional to gas concentration, need warm-up time (~20 sec), supply 5V.

---

### CCS811 / ENS160 (Air Quality)
- Digital I2C VOC (Volatile Organic Compound) and CO₂ equivalent sensor.
- ENS160 is the modern replacement for CCS811.
- Used in indoor air quality monitors, smart HVAC.

---

### DS18B20 (Temperature — Waterproof)
- 1-Wire digital temperature sensor.
- Range: -55°C to +125°C, accuracy ±0.5°C.
- Waterproof probe version available.
- Multiple DS18B20 sensors can share one wire (each has unique 64-bit ID).
- Used in: aquariums, pool controllers, pipe temperature monitoring.

---

## 7. Sensor ICs — Distance / Proximity

---

### HC-SR04 (Ultrasonic)
- 40 kHz ultrasonic — detailed in sonar doc (15_sonar.md).
- Range: 2 cm – 4 m, GPIO interface.
- Use: obstacle detection, height above ground.

---

### VL53L0X (ST Microelectronics — ToF Laser)
| Item       | Detail                                                     |
|------------|------------------------------------------------------------|
| Type       | Time-of-Flight laser ranging (940 nm IR)                   |
| Range      | 30 – 2000 mm (2 m)                                        |
| Accuracy   | ±3%                                                        |
| Interface  | I2C (up to 400 kHz)                                        |
| Supply     | 2.6 – 3.5V                                                 |
| Use        | Obstacle avoidance on small drones, phone proximity sensor |

**Advantage over HC-SR04:** Not affected by wind, faster (up to 50 Hz), much more accurate. Works on transparent surfaces (the laser spot is tiny).

---

### VL53L1X / VL53L5CX (ST Microelectronics)
- VL53L1X: longer range (up to 4 m), same I2C interface.
- VL53L5CX: 8×8 multi-zone ToF — gives a depth map, not just a single distance.

---

### TF-Luna / TF-Mini (Benewake LiDAR)
| Item       | TF-Luna           | TF-Mini            |
|------------|-------------------|--------------------|
| Range      | 0.2 – 8 m         | 0.3 – 12 m         |
| Interface  | UART or I2C       | UART               |
| Update rate| Up to 250 Hz      | 100 Hz             |
| Use        | Drone altitude hold, obstacle avoidance |       |

---

### GP2Y0A21 / GP2Y0A02 (Sharp IR Distance)
- Infrared triangulation distance sensor.
- Analog voltage output proportional to distance.
- GP2Y0A21: 10–80 cm / GP2Y0A02: 20–150 cm.
- Not suitable for transparent surfaces.
- Simple interface — just read the analog pin.

---

### RCWL-0516 (Microwave Radar Motion Sensor)
- 3.18 GHz Doppler microwave motion sensor.
- Digital output (HIGH when motion detected).
- Range: ~7 m, 360° detection through non-metal walls.
- Very cheap, works through plastic enclosures.
- Use: motion-activated lighting, intruder detection.

---

## 8. GPS Modules

---

### NEO-6M (u-blox)
| Item        | Detail                                                    |
|-------------|-----------------------------------------------------------|
| Satellites  | GPS only                                                  |
| Accuracy    | ~2.5 m CEP                                               |
| Update rate | 5 Hz max                                                  |
| Interface   | UART (9600 baud default), outputs NMEA sentences          |
| Supply      | 3.3V                                                      |
| Cold start  | ~27 sec (time to first fix with no almanac)              |
| Use         | Basic drone GPS, navigation, tracking                     |

---

### NEO-M8N (u-blox)
- Multi-constellation: GPS + GLONASS + Galileo + BeiDou.
- Accuracy: ~2 m CEP (better than NEO-6M with multi-constellation).
- Update rate: 10 Hz.
- **Standard GPS for mid-range drone builds.**

---

### NEO-M9N / ZED-F9P (u-blox)
- M9N: newest standard GPS — all 4 constellations, 25 Hz.
- **ZED-F9P**: RTK (Real-Time Kinematic) GPS — cm-level accuracy using correction data from a base station. Used in survey drones, precision agriculture.

---

### ATGM336H (Zhongkewei)
- Cheaper alternative to NEO-6M, similar performance.
- GPS + BeiDou.
- Common in budget drone kits and Chinese electronics.

---

## 9. Display Driver ICs

---

### SSD1306 (OLED Driver)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Display    | 0.96" or 1.3" OLED — 128×64 or 128×32 pixels            |
| Interface  | I2C (address 0x3C or 0x3D) or SPI                        |
| Supply     | 3.3 – 5V                                                  |
| Use        | Status display on drone OSD boards, IoT devices, meters  |

---

### SH1106 (OLED Driver)
- Similar to SSD1306, used in 1.3" OLED panels.
- Slightly different display buffer addressing — needs different library.

---

### ST7735 / ST7789 (TFT LCD Driver)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Display    | 1.8" TFT (ST7735) or 1.3" / 2.0" TFT (ST7789)          |
| Resolution | 128×160 (ST7735) / 240×240 (ST7789)                     |
| Colour     | 65K colours (16-bit)                                      |
| Interface  | SPI                                                       |
| Use        | Colour status displays, small HMI panels                 |

---

### ILI9341 (TFT LCD Driver)
- 2.4" – 3.2" TFT LCD, 320×240 pixels, 65K colours.
- SPI interface.
- Used in Arduino TFT shields, Raspberry Pi displays, touch screens.

---

### MAX7219 (LED Matrix / 7-Segment Driver)
- Drives 8× 7-segment digits or 8×8 LED matrix.
- SPI daisy-chain — connect multiple MAX7219s on the same SPI bus.
- Handles PWM brightness control internally.
- Use: scoreboard displays, numeric meters, dot-matrix message boards.

---

### TM1637 (7-Segment Display Driver)
- Simpler, cheaper alternative to MAX7219 for 4-digit 7-segment displays.
- 2-wire interface (CLK + DIO — not standard I2C).
- Very common in cheap clock modules.

---

## 10. ADC and DAC ICs

---

### ADS1115 (Texas Instruments — 16-bit ADC)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Resolution | 16-bit                                                    |
| Channels   | 4 single-ended or 2 differential                         |
| Sample rate| 8 – 860 SPS                                              |
| Input range| ±6.144V max (programmable PGA: ±0.256V to ±6.144V)      |
| Interface  | I2C (4 addresses via ADDR pin)                           |
| Supply     | 2 – 5.5V                                                 |
| Use        | Precision voltage/current measurement, adding accurate analog inputs to ESP32 (ESP32's built-in ADC is poor) |

---

### ADS1015
- 12-bit version of ADS1115 — faster (3300 SPS) but lower resolution.
- Use when speed matters more than precision.

---

### MCP3208 (Microchip — 12-bit ADC)
- 8-channel, 12-bit, SPI ADC.
- 100 ksps — fast multi-channel acquisition.
- Used when you need more analog inputs than your MCU has.

---

### MCP4725 (Microchip — 12-bit DAC)
- 12-bit single-channel DAC with I2C interface.
- Onboard EEPROM stores power-on default voltage.
- Use: setting reference voltages, analog control signals, waveform generation.

---

### PCF8591 (NXP — 8-bit ADC+DAC)
- 4-channel 8-bit ADC + 1-channel 8-bit DAC on one chip.
- I2C interface.
- Low resolution but cheap and easy — used in learning kits.

---

## 11. Communication Interface ICs

---

### CH340G / CH340C (USB to UART)
- Converts USB to serial UART — used on Arduino clones and ESP32 dev boards.
- Requires driver installation on Windows.
- Use: flashing firmware, serial debugging.

---

### CP2102 / CP2104 (Silicon Labs USB to UART)
- Higher quality USB-UART bridge — better Windows support.
- CP2104 supports higher baud rates.
- Used on quality dev boards.

---

### FT232RL (FTDI USB to UART)
- Most reliable USB-UART bridge — driver support is excellent.
- Slightly more expensive — genuine FTDI chips are premium.
- Use: professional development boards, reliable flashing.

---

### MAX485 / MAX3485 (RS-485 Transceiver)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Type       | RS-485 / RS-422 differential transceiver                 |
| Data rate  | Up to 2.5 Mbps (MAX485), 10 Mbps (MAX3485)              |
| Distance   | Up to 1200 m at 100 kbps                                 |
| Supply     | 5V (MAX485) / 3.3V (MAX3485)                             |
| Use        | Drone ESC telemetry (S.Bus, UART over long cables), industrial sensors, Modbus |

---

### TJA1050 / MCP2551 (CAN Bus Transceiver)
- Converts between MCU logic levels and the differential CAN bus signal.
- CAN bus used in: automotive electronics, industrial robotics, some advanced drone ecosystems (UAVCAN / DroneCAN protocol for ESCs, GPS, sensors).

---

### PCA9685 (I2C PWM Driver)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Channels   | 16 PWM outputs                                            |
| Resolution | 12-bit (4096 steps)                                       |
| Frequency  | 24 – 1526 Hz (configurable)                              |
| Interface  | I2C (6 address pins — up to 62 on one bus)               |
| Supply     | 2.3 – 5.5V logic, separate V+ for PWM outputs            |
| Use        | Driving 16 servos from ESP32/Arduino, LED dimming, drone gimbal control |

---

### PCF8574 (I2C GPIO Expander)
- Adds 8 digital GPIO pins via I2C.
- Used to overcome GPIO shortage on small MCUs.
- Also used to drive 16×2 LCD displays via I2C adapters.

---

### MCP23017 (I2C GPIO Expander — 16-bit)
- 16 GPIO pins via I2C — two banks of 8.
- Interrupt support — can wake up MCU when input changes.
- Up to 8 chips on one I2C bus = 128 extra GPIO.

---

## 12. Logic and Utility ICs

---

### 74HC Series (Logic Gates)
| IC       | Function                   | Use                                    |
|----------|----------------------------|----------------------------------------|
| 74HC00   | Quad NAND gate             | General logic                          |
| 74HC04   | Hex inverter (NOT gate)    | Signal inversion, logic level shifting |
| 74HC14   | Hex Schmitt-trigger inverter| Debouncing noisy signals               |
| 74HC74   | Dual D flip-flop           | Edge detection, frequency division     |
| 74HC595  | 8-bit shift register (SPI out) | Driving 8 outputs from 3 MCU pins  |
| 74HC165  | 8-bit shift register (SPI in) | Reading 8 inputs from 3 MCU pins   |
| 74HC245  | Octal bus transceiver      | 5V ↔ 3.3V level shifting              |

---

### CD4051 / CD4052 / CD4053 (Analog Multiplexer)
- Switches one of 8 (4051) or 2-of-4 (4052) analog signals through to a single output.
- Used when you need more analog inputs than your ADC has channels.
- Controlled by digital select pins.
- Both signal directions work (mux or demux).

---

### SN74AHCT125 (Logic Level Shifter)
- 4-channel level shifter: converts 3.3V logic to 5V.
- One-directional — only 3.3V → 5V.
- Faster than the I2C-specific level shifters.

---

### BSS138-based Level Shifter (SparkFun / Adafruit)
- MOSFET-based bidirectional logic level shifter.
- Works with I2C, UART, SPI — bidirectional.
- Most common module for shifting between 5V (Arduino) and 3.3V (ESP32) on I2C lines.

---

### NE555 / LM555 (Timer IC)
| Mode         | Output                                            | Use                               |
|--------------|---------------------------------------------------|-----------------------------------|
| Astable      | Continuous square wave (frequency set by RC)      | LED blinker, tone generator, PWM  |
| Monostable   | One-shot pulse of fixed width                     | Debouncing, pulse generation      |
| Bistable     | SR flip-flop (set/reset)                          | Latch, toggle switch              |

One of the most-produced ICs in history. Still useful for simple timing without a microcontroller.

---

### CD4017 (Decade Counter)
- Counts 0–9, each count drives one of 10 output pins HIGH in sequence.
- Driven by a clock signal (NE555 output).
- Use: LED chaser, sequencer, frequency divider.

---

## 13. Amplifier ICs

---

### LM358 (Dual Op-Amp)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Type       | Dual general-purpose op-amp                               |
| Supply     | 3 – 32V single, ±1.5V – ±16V dual                       |
| Bandwidth  | 1 MHz                                                     |
| Use        | Comparator, sensor signal conditioning, summing amplifier |
| Limitation | Output cannot swing to supply rail (not rail-to-rail)    |

---

### LM393 (Dual Comparator)
- Similar pin-out to LM358 but optimised as a **comparator** — not an amplifier.
- Open-collector output — needs pull-up resistor.
- Use: threshold detection (e.g., soil moisture sensor modules, line following robots).

---

### TL071 / TL072 / TL074 (JFET Input Op-Amp)
- Very high input impedance (JFET input).
- Low noise — better audio quality than LM358.
- TL071: single, TL072: dual, TL074: quad.
- Use: audio pre-amplifiers, active filters, instrumentation.

---

### NE5532 (Low-Noise Audio Op-Amp)
- Designed specifically for audio — very low noise.
- Used in: microphone pre-amps, audio mixers, headphone amplifiers.

---

### INA219 (Current/Power Monitor)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Measures   | Bus voltage (up to 26V), shunt voltage, calculates current and power |
| Resolution | 12-bit                                                    |
| Interface  | I2C                                                       |
| Use        | **Drone battery current monitoring**, solar panel monitoring, smart power supplies |

---

### INA3221 (3-Channel Power Monitor)
- 3 independent INA219-like channels in one chip.
- Monitors 3 separate power rails simultaneously.
- Used in: Raspberry Pi power management boards, multi-rail drone power systems.

---

### MAX4466 / MAX9814 (Microphone Amplifier)
- MAX4466: fixed-gain mic amp (adjustable 25×–125×) — simple, cheap.
- MAX9814: automatic gain control mic amp — adjusts gain for loud and quiet sounds.
- Both output analog audio for MCU ADC.
- Use: voice detection, sound level monitoring on ESP32.

---

## 14. Voltage Reference ICs

---

### LM4040 / LM336 (Shunt Voltage Reference)
- Provides a precise fixed reference voltage (2.048V, 2.5V, 4.096V, 5V).
- Used when ADC needs a stable reference — ESP32's internal reference drifts with temperature.

---

### REF3325 / REF3330 (TI Precision Reference)
- 2.5V / 3.0V precision voltage reference.
- Very low temperature drift (±10 ppm/°C).
- Used in precision measurement circuits.

---

## 15. Memory ICs

---

### 24Cxx EEPROM (I2C)
| Chip   | Capacity  | Use                                           |
|--------|-----------|-----------------------------------------------|
| 24C02  | 256 bytes | Store a few config values                     |
| 24C32  | 4 KB      | Store calibration data, settings              |
| 24C256 | 32 KB     | Small data logging                            |

I2C interface, very simple to use. Non-volatile — data survives power-off.

---

### W25Q32 / W25Q128 (SPI Flash)
- Serial NOR Flash memory.
- W25Q32: 32 Mbit (4 MB) / W25Q128: 128 Mbit (16 MB).
- SPI interface, very fast (up to 104 MHz).
- Used in: **ESP32 program storage** (the flash chip on ESP32 modules is a W25Q32 or similar), data logging, firmware update storage.

---

### AT24C32 (RTC EEPROM — on DS3231 module)
- 32 Kbit I2C EEPROM that ships on the DS3231 RTC module.
- Used to store timestamps, config alongside the RTC.

---

## 16. Clock and Timer ICs

---

### DS3231 (Real-Time Clock)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Accuracy   | ±2 ppm (very accurate — temperature-compensated crystal) |
| Interface  | I2C                                                       |
| Battery    | CR2032 coin cell keeps time when main power off           |
| Outputs    | 1 Hz / 4 kHz / 8 kHz / 32 kHz square wave, alarm outputs |
| Use        | Timestamping data logs, scheduled wake-up of MCU from sleep, clock displays |

---

### DS1307 (Basic RTC)
- Cheaper but less accurate than DS3231 (no temperature compensation).
- Needs external 32.768 kHz crystal.
- I2C interface.

---

### PCF8523 / PCF8563 (RTC — 3.3V)
- 3.3V RTCs — better suited to ESP32 than DS1307 (which is 5V).
- PCF8523: alarm + timer, I2C.

---

## 17. Audio ICs

---

### MAX98357A (I2S DAC + Class D Amplifier)
| Item       | Detail                                                    |
|------------|-----------------------------------------------------------|
| Type       | I2S input, Class D audio amplifier                        |
| Output     | 3.2W into 4Ω                                              |
| Interface  | I2S (3 pins: BCLK, LRCLK, DIN)                           |
| Supply     | 2.7 – 5.5V                                               |
| Use        | **ESP32 audio output** — play WAV/MP3 directly to speaker |

**Used with ESP32:** ESP32 has built-in I2S peripheral — connect MAX98357A and play audio from SD card without an external DAC chip.

---

### PCM5102A (I2S DAC — High Quality)
- 32-bit I2S stereo DAC.
- High dynamic range — audiophile quality output.
- Requires external amplifier (line-level output only).
- Use: ESP32 music player, audio streaming devices.

---

### INMP441 / SPH0645 (I2S MEMS Microphone)
- Digital MEMS microphone with I2S output — directly into ESP32.
- No analog conversion needed.
- INMP441: very popular for ESP32 voice projects.
- Use: voice command detection, audio recording on drone.

---

### VS1053 (MP3/OGG Decoder IC)
- Decodes MP3, OGG, WAV, AAC, WMA — hardware decoder.
- SPI interface.
- Outputs to 3.5 mm headphone jack.
- Use: standalone MP3 player, audio announcement system.

---

### DFPlayer Mini (MP3 Module)
- Complete module: flash reader + VS chip + amplifier.
- UART interface — send commands to play track numbers.
- Very easy to use for audio playback.
- Use: sound effects in robots, announcement systems.

---

## 18. Optocouplers and Isolation ICs

---

### PC817 (Optocoupler)
- LED + phototransistor in one package.
- Electrically isolates two circuits — signal passes via light, no electrical connection.
- Forward current on LED side → transistor conducts on output side.
- Use: isolating MCU from high-voltage AC circuits, isolating noisy motor signals, relay driver protection.

---

### 6N137 (High-Speed Optocoupler)
- Same function as PC817 but much faster (1 Mbps).
- Used in: MIDI interfaces, high-speed isolated digital signals.

---

### AMC1200 / ISO7641 (Digital Isolators — TI)
- Modern replacement for optocouplers — uses capacitive or inductive coupling.
- Much faster (up to 150 Mbps), lower power, longer life than optocouplers.
- Used in: isolated SPI/I2C/UART in medical devices, industrial inverters.

---

### MOC3021 / MOC3041 (Triac Driver Optocoupler)
- Controls AC mains loads via a TRIAC.
- MCU pin → optocoupler → TRIAC triggers → AC load switches.
- MOC3041: zero-crossing detection built in — switches cleanly to reduce noise.
- Use: AC dimmer, AC motor speed control, solid-state relay.

---

## Quick Chip Lookup Table

| Need                                | Chip / Module to Use                    |
|-------------------------------------|-----------------------------------------|
| WiFi + BLE microcontroller          | ESP32                                   |
| Simple microcontroller              | ATmega328P (Arduino Uno/Nano)           |
| ARM for flight controller           | STM32F4 / STM32H7                       |
| 2.4 GHz RC link                     | nRF24L01+ / CC2500 / SX128x (ELRS)     |
| Long-range 433 MHz link             | HC-12 / SX1278 (LoRa)                  |
| DC motor control                    | L298N / DRV8833 / TB6612FNG             |
| Stepper motor                       | DRV8825 / A4988                         |
| LiPo charger                        | TP4056                                  |
| 5V from battery (efficient)         | LM2596 / MP1584                         |
| 3.3V regulator                      | AMS1117-3.3                             |
| Drone IMU                           | BMI270 / ICM-42688-P                    |
| Baro altimeter                      | BMP280 / MS5611                         |
| GPS                                 | NEO-M8N                                 |
| Temperature + humidity              | BME280 / DHT22                          |
| Current monitoring                  | INA219                                  |
| I2C distance (short)                | VL53L0X                                 |
| Ultrasonic distance                 | HC-SR04                                 |
| OLED display                        | SSD1306 (I2C, 0.96")                   |
| Colour TFT display                  | ST7789 / ILI9341                        |
| Accurate ADC (ESP32 fix)            | ADS1115                                 |
| 16× PWM outputs (servos)            | PCA9685                                 |
| Real-time clock                     | DS3231                                  |
| SPI Flash                           | W25Q32 / W25Q64                         |
| Audio output (ESP32)                | MAX98357A (I2S)                         |
| Microphone (digital)                | INMP441 (I2S)                           |
| 5V ↔ 3.3V level shifting            | BSS138 module / 74AHCT125              |
| USB to serial (flashing)            | CH340G / CP2102 / FT232RL              |
| AC mains switching from MCU         | PC817 + MOC3041 + TRIAC                 |
