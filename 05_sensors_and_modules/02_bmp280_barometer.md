# 02 — BMP280 Barometric Pressure and Altitude Sensor

## What Is It?

The **BMP280** is a precision digital sensor made by Bosch that measures:

- **Barometric pressure** (hPa / mbar) — the weight of the air column above you
- **Temperature** (°C) — used internally to compensate pressure readings
- **Altitude** (meters) — calculated from pressure using a physics formula

For a drone, the BMP280 is the **altimeter** — it tells the flight controller how high above the ground the drone is hovering. This enables altitude-hold mode.

---

## The Physics in Plain Language

### Why does pressure tell you altitude?

At sea level, all the atmosphere is above you → high pressure (~1013 hPa).
At the top of a mountain, less air is above → lower pressure.

The relationship follows the **barometric formula**:

```
altitude = 44330 × (1 - (pressure / seaLevelPressure)^(1/5.255))
```

This gives altitude in meters relative to wherever "sea level pressure" is set.

**Accuracy:** The BMP280 resolves ~0.1 hPa in pressure, which equals roughly **~0.8 meters** in altitude. Good enough for indoor hover, not precise enough for GPS-level navigation.

**Important:** Barometric altitude is *relative*, not absolute. A rising or falling weather front can shift it by 10–50 meters over hours. For drone work, it's used for *hold this altitude*, not *fly to exactly 50m above sea level*.

---

## Hardware Overview

The BMP280 module supports both **I2C** and **SPI** communication.

```
BMP280 Module Pins:

  VCC  ── 3.3V (do NOT use 5V directly — sensor is 3.3V)
  GND  ── Ground
  SCL  ── I2C clock (or SPI clock)
  SDA  ── I2C data (or SPI MOSI)
  CSB  ── SPI chip select (tie to 3.3V for I2C mode)
  SDO  ── I2C address select OR SPI MISO
          SDO LOW  → I2C address 0x76
          SDO HIGH → I2C address 0x77
```

Most breakout modules have pull-up resistors built in. Use I2C for simplicity.

---

## Wiring to ESP32 (I2C Mode)

| BMP280 Pin | ESP32 Pin | Notes                          |
|------------|-----------|--------------------------------|
| VCC        | 3.3V      | 3.3V only — not 5V tolerant    |
| GND        | GND       |                                |
| SCL        | GPIO 22   | Default I2C SCL on ESP32       |
| SDA        | GPIO 21   | Default I2C SDA on ESP32       |
| CSB        | 3.3V      | Forces I2C mode                |
| SDO        | GND       | Sets I2C address to 0x76       |

> If SDO is HIGH (connected to 3.3V), address is 0x77.
> Check which your module defaults to if sensor is not found.

---

## Running Both MPU-6050 and BMP280 Together

Both use I2C, and their addresses don't conflict:
- MPU-6050: **0x68**
- BMP280: **0x76** or **0x77**

Wire both to the same SCL (GPIO 22) and SDA (GPIO 21) lines. I2C is a bus — multiple devices share the same two wires.

```
ESP32 GPIO21 (SDA) ──┬── MPU-6050 SDA
                     └── BMP280 SDA

ESP32 GPIO22 (SCL) ──┬── MPU-6050 SCL
                     └── BMP280 SCL
```

---

## Installing the Library

In Arduino IDE:
1. Open **Sketch → Include Library → Manage Libraries**
2. Search: `BMP280`
3. Install: **"Adafruit BMP280 Library"** by Adafruit
4. Also install its dependency: **"Adafruit Unified Sensor"** (prompted automatically)

---

## Code Example 1 — Basic Pressure, Temperature, Altitude

```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;   // I2C interface

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!bmp.begin(0x76)) {       // try 0x77 if this fails
    Serial.println("BMP280 not found. Check wiring and I2C address.");
    while (true);
  }

  // Recommended settings from Bosch datasheet for weather/indoor use
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,   // temperature oversampling
                  Adafruit_BMP280::SAMPLING_X16,  // pressure oversampling
                  Adafruit_BMP280::FILTER_X16,    // IIR filter — smooths noise
                  Adafruit_BMP280::STANDBY_MS_500);
}

void loop() {
  float temperature = bmp.readTemperature();        // Celsius
  float pressure    = bmp.readPressure() / 100.0;  // Pa → hPa
  float altitude    = bmp.readAltitude(1013.25);    // 1013.25 = standard sea level hPa

  Serial.print("Temp: ");     Serial.print(temperature); Serial.print(" °C  ");
  Serial.print("Pressure: "); Serial.print(pressure);    Serial.print(" hPa  ");
  Serial.print("Altitude: "); Serial.print(altitude);    Serial.println(" m");

  delay(1000);
}
```

**Expected output (at typical indoor elevation):**
```
Temp: 28.43 °C  Pressure: 1009.21 hPa  Altitude: 33.84 m
```

---

## Code Example 2 — Relative Altitude (for Drone Use)

For a drone, you don't care about altitude above sea level.
You care about altitude above the **takeoff point**. This is called **AGL — Above Ground Level**.

```cpp
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

float groundPressure = 0;    // pressure at the moment of takeoff
const float SEA_LEVEL_HPA = 1013.25;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 not found!");
    while (true);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  delay(500);

  // Record ground-level pressure as baseline
  groundPressure = bmp.readPressure() / 100.0;
  Serial.print("Ground pressure set to: ");
  Serial.print(groundPressure);
  Serial.println(" hPa");
}

void loop() {
  float currentPressure = bmp.readPressure() / 100.0;

  // Convert ground pressure to an "effective sea level" so altitude reads 0 at home
  float effectiveSeaLevel = groundPressure / pow(1.0 - (0.0 / 44330.0), 5.255);

  float agl = bmp.readAltitude(groundPressure);   // altitude above takeoff point

  Serial.print("AGL: ");
  Serial.print(agl, 2);
  Serial.print(" m  |  Pressure: ");
  Serial.print(currentPressure);
  Serial.println(" hPa");

  delay(200);  // 5 Hz update rate
}
```

**What to test:**
- Run this on the ground → AGL shows ~0
- Lift the sensor 1 meter above the table → AGL shows ~1.0
- Put it on an upper floor → AGL shows ~3–4 m

---

## Code Example 3 — MPU-6050 + BMP280 Together

This is the foundation of a drone's sensor stack.

```cpp
#include <Wire.h>
#include <MPU6050_light.h>
#include <Adafruit_BMP280.h>

MPU6050 mpu(Wire);
Adafruit_BMP280 bmp;

float groundAltitude = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Init MPU-6050
  if (mpu.begin() != 0) {
    Serial.println("MPU-6050 not found!");
    while (true);
  }
  Serial.println("Calibrating IMU...");
  delay(1000);
  mpu.calcOffsets();

  // Init BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 not found!");
    while (true);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  // Record ground reference
  delay(500);
  groundAltitude = bmp.readAltitude(1013.25);
  Serial.print("Ground altitude reference: ");
  Serial.print(groundAltitude);
  Serial.println(" m");
  Serial.println("All sensors ready!");
}

void loop() {
  // --- IMU ---
  mpu.update();
  float pitch = mpu.getAngleX();
  float roll  = mpu.getAngleY();

  // --- Barometer ---
  float altitude = bmp.readAltitude(1013.25) - groundAltitude;
  float pressure = bmp.readPressure() / 100.0;
  float temp     = bmp.readTemperature();

  // --- Print ---
  Serial.print("Pitch: ");    Serial.print(pitch, 1);    Serial.print("°  ");
  Serial.print("Roll: ");     Serial.print(roll, 1);     Serial.print("°  | ");
  Serial.print("Alt AGL: ");  Serial.print(altitude, 2); Serial.print(" m  ");
  Serial.print("Temp: ");     Serial.print(temp, 1);     Serial.println(" °C");

  delay(100);  // 10 Hz
}
```

---

## Understanding the Filter Settings

The BMP280 has a built-in **IIR (Infinite Impulse Response) filter** that smooths out spikes from:
- Doors slamming
- Wind gusts
- Fast movements

| Filter Coefficient | Response Speed | Noise Rejection |
|--------------------|---------------|-----------------|
| FILTER_OFF         | Fastest       | None            |
| FILTER_X2          | Fast          | Low             |
| FILTER_X4          | Medium        | Medium          |
| FILTER_X8          | Slower        | High            |
| FILTER_X16         | Slowest       | Highest         |

For a drone **in flight**: use `FILTER_X4` or `FILTER_X8` — fast enough to track altitude changes, stable enough to ignore motor vibration.

For **weather station**: use `FILTER_X16`.

---

## Pressure Oversampling

More oversampling = more readings averaged = lower noise, but slower.

| Oversampling | RMS Noise | Time per reading |
|--------------|-----------|-----------------|
| X1           | 2.62 Pa   | ~5 ms            |
| X2           | 1.31 Pa   | ~8 ms            |
| X4           | 0.66 Pa   | ~15 ms           |
| X16          | 0.33 Pa   | ~53 ms           |

For drones: `SAMPLING_X4` is a good balance.

---

## Common Problems

| Problem                           | Likely Cause                       | Fix                                              |
|-----------------------------------|------------------------------------|--------------------------------------------------|
| "BMP280 not found"                | Wrong I2C address                  | Try `0x77` instead of `0x76`                     |
| "BMP280 not found" with 0x77 too  | Wrong wiring                       | Check VCC=3.3V, SCL/SDA not swapped              |
| Altitude jumps ±5m randomly       | No filter, motor vibration         | Use `FILTER_X16`, `SAMPLING_X16`                 |
| Altitude drifts 20m over an hour  | Weather front / temperature change | Normal — barometric altitude drifts with weather |
| Temperature reads too high        | Self-heating from nearby ESP32     | Keep sensor away from heat sources               |
| Reading shows 0 Pa                | VCC connected to 5V                | Must be 3.3V — sensor destroyed, replace it      |

---

## How This Applies to Drone Flight

```
Desired altitude: 5m
Current altitude: 4.2m  (from BMP280)
Error: -0.8m

→ Throttle PID increases motor speed to climb
→ BMP280 reads 4.9m, error is now -0.1m
→ PID reduces correction → drone settles at 5m

This runs 10–50 times per second.
```

This is **altitude hold mode**. It's impossible without the barometer.

---

## What to Try

1. Run Example 1 → note the pressure and altitude in your city
2. Take the sensor to a different floor → watch altitude change ~3m per floor
3. Run Example 2 → set ground reference → lift sensor 1m → confirm ~1m reading
4. Open a window suddenly → watch pressure flicker from wind
5. Run Example 3 with both sensors → you now have pitch, roll, and altitude all at once

---

## Sensor Noise — What to Expect

If you log raw BMP280 altitude readings at 5 Hz indoors:
```
33.84, 33.91, 33.78, 33.85, 33.93, 33.82, ...
```
This ±0.1m noise is normal. In a flight controller it gets filtered further. The drone doesn't react to sub-10cm noise — that's just sensor physics.

---

## Next Steps

- **05_magnetometer.md** — add compass for yaw
- **03_gps.md** — add GPS for position hold
- **Module 07** — wire IMU + barometer + GPS into a flight controller stack
