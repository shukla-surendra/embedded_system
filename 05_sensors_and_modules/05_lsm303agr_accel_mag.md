# 05 — LSM303AGR Accelerometer + Magnetometer

## What Is It?

The **LSM303AGR** is a combo sensor from ST Microelectronics combining:
- **3-axis Accelerometer** — ±2g / ±4g / ±8g / ±16g, 12-bit resolution
- **3-axis Magnetometer** — ±50 gauss, 16-bit resolution, factory-calibrated

Both sensors are in one IC, communicate over I2C or SPI (separately addressed), and are powered by **3.3V only**.

**For a drone:** the magnetometer gives an absolute heading (like a compass) to correct yaw drift from the gyroscope. The accelerometer complements the MPU-6050 or can replace it in lighter builds.

---

## LSM303AGR vs MPU-6050 vs ADXL345

| Feature | LSM303AGR | MPU-6050 | ADXL345 |
|---------|-----------|---------|---------|
| Accelerometer | Yes (12-bit) | Yes (16-bit) | Yes (13-bit) |
| Gyroscope | **No** | Yes | No |
| Magnetometer | **Yes** | No | No |
| I2C addresses | Accel: 0x19, Mag: 0x1E | 0x68 | 0x53 |
| Voltage | 3.3V | 3.3V / 5V-tolerant | 3.3V |
| Key use | Compass + tilt | Full IMU | Precision tilt/tap |

The LSM303AGR does **not** have a gyroscope — for full drone stabilization, pair it with the MPU-6050 (use LSM303AGR's magnetometer for yaw reference).

---

## Hardware Overview

```
LSM303AGR Module Pins:

  VIN / VCC  ── 3.3V
  GND        ── Ground
  SCL        ── I2C Clock
  SDA        ── I2C Data
  INT1       ── Accelerometer interrupt (optional)
  INT2       ── Accelerometer interrupt (optional)
  DRDY       ── Magnetometer data-ready interrupt (optional)
```

The chip uses **two separate I2C addresses**:
- Accelerometer: **0x19**
- Magnetometer: **0x1E**

Both appear on the same SDA/SCL bus — the library handles each independently.

---

## Wiring to ESP32

| LSM303AGR Pin | ESP32 Pin | Notes |
|---------------|-----------|-------|
| VCC | 3.3V | 3.3V only |
| GND | GND | |
| SCL | GPIO 22 | Shared I2C bus |
| SDA | GPIO 21 | Shared I2C bus |
| INT1 / INT2 / DRDY | Not connected | Optional |

All I2C sensors (MPU-6050, BMP280, LSM303AGR) share the same GPIO 21/22 wires.

---

## Installing the Library

In Arduino IDE → Manage Libraries:
1. Search: `LSM303AGR`
2. Install: **"Adafruit LSM303AGR Mag"** and **"Adafruit LSM303AGR Accel"**
3. Also install: **"Adafruit Unified Sensor"** (dependency)
4. Also install: **"Adafruit BusIO"** (dependency)

---

## Code Example 1 — Accelerometer Readings

```cpp
#include <Wire.h>
#include <Adafruit_LSM303AGR_Accel.h>
#include <Adafruit_Sensor.h>

Adafruit_LSM303AGR_Accel accel;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!accel.begin()) {
    Serial.println("LSM303AGR accelerometer not found. Check wiring.");
    while (true);
  }

  Serial.println("LSM303AGR Accelerometer ready");
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  // Values are in m/s² (1g = 9.81 m/s²)
  Serial.print("X: "); Serial.print(event.acceleration.x);
  Serial.print("  Y: "); Serial.print(event.acceleration.y);
  Serial.print("  Z: "); Serial.print(event.acceleration.z);
  Serial.println(" m/s²");

  delay(200);
}
```

At rest flat: Z ≈ 9.81 m/s², X ≈ 0, Y ≈ 0

---

## Code Example 2 — Magnetometer (Raw Compass)

```cpp
#include <Wire.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS2MDL mag(12345);   // ID is arbitrary

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!mag.begin()) {
    Serial.println("LSM303AGR magnetometer not found.");
    while (true);
  }

  mag.setDataRate(LIS2MDL_RATE_10_HZ);
  Serial.println("Magnetometer ready");
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);

  // Raw magnetic field in microtesla (µT)
  Serial.print("MagX: "); Serial.print(event.magnetic.x);
  Serial.print("  MagY: "); Serial.print(event.magnetic.y);
  Serial.print("  MagZ: "); Serial.print(event.magnetic.z);
  Serial.println(" µT");

  delay(200);
}
```

---

## Code Example 3 — Compass Heading

Heading is calculated from the X and Y magnetic field components when the sensor is held level.

```cpp
#include <Wire.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_LSM303AGR_Accel.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS2MDL           mag(12345);
Adafruit_LSM303AGR_Accel   accel;

// Magnetometer hard-iron calibration offsets
// Run the calibration sketch first (see below) to find these
float magOffsetX = 0, magOffsetY = 0, magOffsetZ = 0;

float computeHeading(float mx, float my, float ax, float ay, float az) {
  // Tilt-compensated heading
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  float roll  = atan2(ay, az);

  float Xh = mx * cos(pitch) + my * sin(roll) * sin(pitch) - my * cos(roll) * sin(pitch);
  float Yh = my * cos(roll) + my * sin(roll);

  float heading = atan2(Yh, Xh) * 180.0 / PI;
  if (heading < 0) heading += 360.0;
  return heading;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  accel.begin();
  mag.begin();
  mag.setDataRate(LIS2MDL_RATE_10_HZ);
}

void loop() {
  sensors_event_t magEvent, accelEvent;
  mag.getEvent(&magEvent);
  accel.getEvent(&accelEvent);

  // Apply hard-iron offsets (calibrate these for your location)
  float mx = magEvent.magnetic.x - magOffsetX;
  float my = magEvent.magnetic.y - magOffsetY;

  float ax = accelEvent.acceleration.x;
  float ay = accelEvent.acceleration.y;
  float az = accelEvent.acceleration.z;

  float heading = computeHeading(mx, my, ax, ay, az);

  Serial.print("Heading: "); Serial.print(heading, 1); Serial.println("°");
  Serial.print("  (N=0/360, E=90, S=180, W=270)");

  delay(200);
}
```

---

## Magnetometer Calibration (Hard-Iron Offsets)

The magnetometer picks up permanent magnetic interference from PCB traces, nearby magnets, and battery wires. This causes a fixed offset error — the compass reads wrong even when your board is level and still.

**Calibration procedure:**

```cpp
// Run this sketch once. Rotate the sensor slowly through all orientations
// for 30 seconds. Note the min/max of each axis.

#include <Wire.h>
#include <Adafruit_LIS2MDL.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS2MDL mag(12345);

float minX = 9999, maxX = -9999;
float minY = 9999, maxY = -9999;
float minZ = 9999, maxZ = -9999;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mag.begin();
  Serial.println("Rotate sensor through all orientations for 30 seconds...");
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);

  float x = event.magnetic.x;
  float y = event.magnetic.y;
  float z = event.magnetic.z;

  if (x < minX) minX = x; if (x > maxX) maxX = x;
  if (y < minY) minY = y; if (y > maxY) maxY = y;
  if (z < minZ) minZ = z; if (z > maxZ) maxZ = z;

  // Offsets are the midpoint of min/max on each axis
  Serial.print("OffsetX: "); Serial.print((minX + maxX) / 2);
  Serial.print("  OffsetY: "); Serial.print((minY + maxY) / 2);
  Serial.print("  OffsetZ: "); Serial.println((minZ + maxZ) / 2);

  delay(50);
}
```

After 30 seconds of slow rotation, note the final offset values and plug them into `magOffsetX`, `magOffsetY`, `magOffsetZ` in your main sketch.

---

## Magnetic Declination

True North and Magnetic North differ by a few degrees depending on your location. This angle is called **magnetic declination**.

For India (approximate):
- Mumbai: ~1° W (declination = -1°)
- Delhi: ~0.5° E (declination = +0.5°)
- Bengaluru: ~0.5° W

Find your exact declination: search "magnetic declination [your city]"

```cpp
const float DECLINATION = -1.0;   // degrees, negative = West

float trueHeading = heading + DECLINATION;
if (trueHeading < 0)   trueHeading += 360;
if (trueHeading > 360) trueHeading -= 360;
```

---

## Using LSM303AGR + MPU-6050 Together

Both on the same I2C bus — addresses don't conflict:
- MPU-6050: 0x68
- LSM303AGR accel: 0x19
- LSM303AGR mag: 0x1E

Use MPU-6050 for pitch + roll + gyro, LSM303AGR magnetometer for yaw reference.

```cpp
// No address conflict — just Wire.begin() once and use all three
Wire.begin();   // GPIO 21 SDA, GPIO 22 SCL
mpu.begin();    // 0x68
accel.begin();  // 0x19
mag.begin();    // 0x1E
```

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| "Accelerometer not found" | Wrong address or wiring | Check SDA/SCL, confirm VCC=3.3V |
| "Magnetometer not found" | Same I2C bus issue | Scan I2C — should see 0x19 and 0x1E |
| Heading spins even when still | No hard-iron calibration | Run calibration sketch, enter offsets |
| Heading off by fixed amount | Magnetic declination ignored | Add local declination correction |
| Erratic readings near drone motors | Magnetic interference from motor currents | Mount as far from ESCs/motors as possible |
| Heading accurate on bench, wrong in flight | Frame metal or battery | Calibrate with all electronics powered and mounted |

---

## Magnetometer Placement on a Drone

Motor currents up to 20A generate strong magnetic fields. The magnetometer must be:
- Mounted **far from motors and ESCs** — ideally on an arm extension or top of frame
- **Above the battery** not next to it
- Recalibrated after any frame modification

On many drones, the compass is mounted on the GPS mast — 10–15 cm above the frame, away from interference.

---

## What to Try

1. Run Example 1 → tilt sensor, watch X/Y/Z change relative to gravity vector
2. Run Example 2 → hold sensor level, note X and Y values — rotating 90° should flip them
3. Run calibration sketch → rotate slowly for 30 sec → note offsets
4. Run Example 3 with offsets → point at a known compass direction → verify
5. Add MPU-6050 alongside → merge gyro angles + magnetometer yaw

---

## Next Steps

- **06_hmc5883l_compass.md** — alternative magnetometer, similar concept
- **01_mpu6050_imu.md** — pair with this for full IMU stack (accel + gyro + mag)
- **07_drone_electronics** — mounting, interference shielding, full sensor integration
