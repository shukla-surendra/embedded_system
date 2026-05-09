# 07 — ADXL345 Accelerometer Module

## What Is It?

The **ADXL345** is a 3-axis digital accelerometer from Analog Devices.

- Range: ±2g / ±4g / ±8g / ±16g (software selectable)
- Resolution: **13-bit at ±16g** — the highest resolution accelerometer in this project kit
- Interface: **I2C or SPI**
- Voltage: 3.3V (some modules have 5V regulator, tolerates 5V logic)
- Special features: **tap detection, double-tap detection, freefall detection** (built into hardware)

**For a drone project:** the ADXL345 is excellent for detecting landing (impact/tap detection fires when the drone touches down) and for precise tilt sensing. It can also detect the freefall state if the drone loses power.

---

## ADXL345 vs MPU-6050 Accelerometer

| Feature | ADXL345 | MPU-6050 (accel part) |
|---------|---------|----------------------|
| Resolution | 13-bit (±16g) | 16-bit |
| Built-in tap detection | **Yes** | No |
| Built-in freefall detect | **Yes** | No |
| Gyroscope | No | Yes |
| Power consumption | 23–145 µA | 3.9 mA |
| I2C address | 0x53 (SDO=GND) or 0x1D (SDO=3.3V) | 0x68 |

The ADXL345 uses far less power and has hardware interrupt features the MPU-6050 lacks. For battery-powered applications where you need tap detection or freefall, it's the better choice.

---

## Hardware Overview

```
ADXL345 Module Pins:

  VCC  ── 3.3V (or 5V if module has regulator)
  GND  ── Ground
  SCL  ── I2C Clock (or SPI Clock)
  SDA  ── I2C Data (or SPI MOSI)
  SDO  ── I2C address select (LOW=0x53, HIGH=0x1D)
         OR SPI MISO
  CS   ── SPI Chip Select (tie HIGH for I2C mode)
  INT1 ── Interrupt 1 (tap, freefall, data ready)
  INT2 ── Interrupt 2 (secondary interrupt)
```

For I2C mode: tie CS to 3.3V and SDO to GND (address 0x53).

---

## Wiring to ESP32 (I2C)

| ADXL345 Pin | ESP32 Pin | Notes |
|-------------|-----------|-------|
| VCC | 3.3V | |
| GND | GND | |
| SCL | GPIO 22 | Shared I2C |
| SDA | GPIO 21 | Shared I2C |
| CS | 3.3V | Forces I2C mode |
| SDO | GND | I2C address = 0x53 |
| INT1 | GPIO 4 | Optional — for tap/freefall interrupts |
| INT2 | Not connected | |

I2C address 0x53 does not conflict with MPU-6050 (0x68), BMP280 (0x76), or HMC5883L (0x1E) — all four can run on the same bus.

---

## Installing the Library

In Arduino IDE → Manage Libraries:
1. Search: `ADXL345`
2. Install: **"Adafruit ADXL345"** by Adafruit
3. Also install: **"Adafruit Unified Sensor"** and **"Adafruit BusIO"** (dependencies)

---

## Code Example 1 — Basic Acceleration Reading

```cpp
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>

Adafruit_ADXL345_Unified adxl(12345);   // unique ID, arbitrary

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!adxl.begin()) {
    Serial.println("ADXL345 not found. Check wiring.");
    while (true);
  }

  adxl.setRange(ADXL345_RANGE_16_G);   // ±16g for vibration/impact
  // Options: ADXL345_RANGE_2_G (most precise), _4_G, _8_G, _16_G

  Serial.println("ADXL345 ready");
  adxl.printSensorDetails();
}

void loop() {
  sensors_event_t event;
  adxl.getEvent(&event);

  // Values in m/s² (9.81 m/s² = 1g)
  Serial.print("X: "); Serial.print(event.acceleration.x, 2);
  Serial.print("  Y: "); Serial.print(event.acceleration.y, 2);
  Serial.print("  Z: "); Serial.print(event.acceleration.z, 2);
  Serial.println(" m/s²");

  delay(200);
}
```

Flat and still: X ≈ 0, Y ≈ 0, Z ≈ 9.81 m/s²

---

## Code Example 2 — Tilt Angle (Pitch and Roll)

```cpp
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>

Adafruit_ADXL345_Unified adxl(12345);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  adxl.begin();
  adxl.setRange(ADXL345_RANGE_2_G);   // ±2g gives best resolution for tilt
}

void loop() {
  sensors_event_t event;
  adxl.getEvent(&event);

  float ax = event.acceleration.x;
  float ay = event.acceleration.y;
  float az = event.acceleration.z;

  // Calculate pitch and roll from accelerometer
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
  float roll  = atan2(ay, az) * 180.0 / PI;

  Serial.print("Pitch: "); Serial.print(pitch, 1);
  Serial.print("°  Roll: "); Serial.print(roll, 1);
  Serial.println("°");

  delay(100);
}
```

---

## Code Example 3 — Tap Detection (Landing Assist)

The ADXL345 can detect a sudden impact in hardware — the INT1 pin fires a pulse when a tap is detected. No polling needed.

```cpp
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>

#define INT1_PIN 4

Adafruit_ADXL345_Unified adxl(12345);

volatile bool tapDetected = false;

void IRAM_ATTR tapISR() {
  tapDetected = true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  adxl.begin();
  adxl.setRange(ADXL345_RANGE_16_G);   // wide range for impact

  // Configure tap detection
  adxl.writeRegister(ADXL345_REG_THRESH_TAP, 50);    // tap threshold: 50 × 62.5mg = 3.125g
  adxl.writeRegister(ADXL345_REG_DUR,        15);    // tap must last < 15 × 625µs = 9.375ms
  adxl.writeRegister(ADXL345_REG_LATENT,     80);    // 80 × 1.25ms = 100ms between taps (for double-tap)
  adxl.writeRegister(ADXL345_REG_WINDOW,    200);    // double-tap window: 200 × 1.25ms = 250ms
  adxl.writeRegister(ADXL345_REG_TAP_AXES,    7);    // enable tap on X, Y, Z (7 = 0b00000111)

  // Map tap interrupt to INT1 pin, enable single tap
  adxl.writeRegister(ADXL345_REG_INT_ENABLE, 0x40);  // bit 6 = single tap
  adxl.writeRegister(ADXL345_REG_INT_MAP,    0x00);  // all interrupts → INT1

  // Attach interrupt
  pinMode(INT1_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT1_PIN), tapISR, RISING);

  // Clear any pending interrupt
  adxl.readRegister(ADXL345_REG_INT_SOURCE);

  Serial.println("Tap detection ready. Tap the sensor!");
}

void loop() {
  if (tapDetected) {
    tapDetected = false;
    adxl.readRegister(ADXL345_REG_INT_SOURCE);   // must read to clear interrupt

    Serial.println("TAP DETECTED — drone has landed!");
    // In real drone: trigger landing confirmation, disarm motors
  }

  delay(10);
}
```

---

## Code Example 4 — Freefall Detection

Freefall = all axes simultaneously near 0g. In a drone this means power-loss crash.

```cpp
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>

#define INT1_PIN 4

Adafruit_ADXL345_Unified adxl(12345);
volatile bool freefallDetected = false;

void IRAM_ATTR freefallISR() {
  freefallDetected = true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  adxl.begin();
  adxl.setRange(ADXL345_RANGE_2_G);

  // Freefall: all axes below threshold for minimum time
  adxl.writeRegister(ADXL345_REG_THRESH_FF, 7);    // 7 × 62.5mg = 437.5mg threshold
  adxl.writeRegister(ADXL345_REG_TIME_FF,   30);   // 30 × 5ms = 150ms minimum freefall duration

  adxl.writeRegister(ADXL345_REG_INT_ENABLE, 0x04);  // bit 2 = freefall
  adxl.writeRegister(ADXL345_REG_INT_MAP,    0x00);  // → INT1

  pinMode(INT1_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT1_PIN), freefallISR, RISING);
  adxl.readRegister(ADXL345_REG_INT_SOURCE);

  Serial.println("Freefall detection active. Drop the sensor!");
}

void loop() {
  if (freefallDetected) {
    freefallDetected = false;
    adxl.readRegister(ADXL345_REG_INT_SOURCE);
    Serial.println("FREEFALL DETECTED!");
    // In real application: deploy recovery, log black-box data
  }

  delay(10);
}
```

Test by dropping the sensor (over something soft!) — the interrupt fires during the fall.

---

## Code Example 5 — ADXL345 + MPU-6050 Together

Use MPU-6050 for gyroscope + general IMU. Use ADXL345 for its tap/freefall hardware detection.

```cpp
#include <Wire.h>
#include <MPU6050_light.h>
#include <Adafruit_ADXL345_U.h>

MPU6050 mpu(Wire);
Adafruit_ADXL345_Unified adxl(12345);

void setup() {
  Serial.begin(115200);
  Wire.begin();   // one Wire.begin() for all I2C devices

  mpu.begin();    // 0x68
  delay(500);
  mpu.calcOffsets();

  adxl.begin();   // 0x53
  adxl.setRange(ADXL345_RANGE_16_G);

  Serial.println("Both sensors ready");
}

void loop() {
  mpu.update();

  sensors_event_t event;
  adxl.getEvent(&event);

  Serial.print("IMU Pitch: "); Serial.print(mpu.getAngleX(), 1);
  Serial.print("°  Roll: "); Serial.print(mpu.getAngleY(), 1);
  Serial.print("°  |  ADXL Z: "); Serial.print(event.acceleration.z, 2);
  Serial.println(" m/s²");

  delay(100);
}
```

I2C addresses in use: 0x68 (MPU), 0x53 (ADXL) — no conflict.

---

## Choosing the Right Range

| Range | Resolution | Best For |
|-------|-----------|----------|
| ±2g | 3.9 mg/LSB | Precision tilt, slow motion |
| ±4g | 7.8 mg/LSB | General purpose |
| ±8g | 15.6 mg/LSB | Moderate impact, drone flight |
| ±16g | 31.25 mg/LSB | Landing impacts, crash detection |

For tap detection and landing: use **±16g**.
For precision tilt sensing: use **±2g**.

---

## Tap Threshold Tuning

The `THRESH_TAP` register sets the minimum acceleration to count as a tap.
Units: 62.5 mg per bit.

| Register value | Threshold | Typical trigger |
|----------------|-----------|-----------------|
| 16 | 1g | Very sensitive, triggers from hand movement |
| 32 | 2g | Light tap |
| 50 | 3.1g | Firm tap (recommended for landing) |
| 80 | 5g | Hard impact only |

Start at 50 and increase if you get false triggers during normal flight.

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| "ADXL345 not found" | CS not tied HIGH (SPI mode) | Connect CS to 3.3V for I2C |
| All readings are 0 | Wrong I2C address | Try 0x1D (SDO=3.3V), run I2C scanner |
| Tap interrupt never fires | Threshold too high | Lower THRESH_TAP value |
| Tap interrupt fires constantly | Threshold too low or no DUR set | Increase THRESH_TAP, set DUR register |
| Values noisy in flight | Vibration from motors | Use ±16g range, average readings, use foam damping |
| INT1 pin not triggering | Forgot to read INT_SOURCE to clear | Always read ADXL345_REG_INT_SOURCE in the ISR handler |

---

## Vibration Damping for Drones

Motor vibration ruins accelerometer readings. Mount the ADXL345 on:
- **Foam tape** between sensor and frame
- **Rubber grommets** (flight controller mounting)
- Never hard-mount directly to the motor arm

This applies to all accelerometers — MPU-6050, ADXL345, LSM303AGR.

---

## Low Power Mode

The ADXL345 can operate at **23 µA in sleep mode** with wakeup on activity — useful for a drone that powers up when picked up:

```cpp
// Enable activity detection, go to sleep when still
adxl.writeRegister(ADXL345_REG_THRESH_ACT, 4);      // 4 × 62.5mg = 250mg activity threshold
adxl.writeRegister(ADXL345_REG_ACT_INACT_CTL, 0xF0); // enable AC activity detection on X,Y,Z
adxl.writeRegister(ADXL345_REG_INT_ENABLE, 0x10);    // activity interrupt
adxl.writeRegister(ADXL345_REG_POWER_CTL, 0x08);     // measurement mode
```

---

## What to Try

1. Run Example 1 → tilt sensor, watch axis values shift
2. Run Example 2 → calculate pitch and roll, compare to MPU-6050 output
3. Wire INT1 to GPIO 4, run Example 3 → tap the module with a finger → see "TAP DETECTED" in Serial
4. Adjust `THRESH_TAP` from 20 to 80 → notice sensitivity change
5. Drop the sensor 10 cm onto a book → run Example 4 → verify freefall triggers

---

## Next Steps

- **01_mpu6050_imu.md** — add gyroscope for full orientation (ADXL345 has no gyro)
- **06_hmc5883l_compass.md** — add compass for yaw reference
- **07_drone_electronics** — use tap interrupt to auto-disarm on touchdown
