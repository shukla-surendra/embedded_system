# 06 — HMC5883L Triple Axis Compass

## What Is It?

The **HMC5883L** is a 3-axis magnetometer (compass) made by Honeywell.
It measures the Earth's magnetic field on X, Y, and Z axes and from that you calculate a **heading** — the direction the sensor is pointing relative to magnetic north.

- Resolution: 2 milligauss (mG)
- Field range: ±8 gauss
- Interface: I2C (address 0x1E)
- Voltage: 3.3V (some modules have a 5V regulator)

**For a drone:** the magnetometer is the only sensor that gives absolute yaw (compass heading). The gyroscope drifts over time — the magnetometer corrects it. Together they give stable, drift-free heading.

---

## HMC5883L vs LSM303AGR Magnetometer

Both do the same job. Key differences:

| Feature | HMC5883L | LSM303AGR (mag part) |
|---------|----------|----------------------|
| Type | Standalone compass | Combo chip (accel + mag) |
| I2C address | 0x1E | 0x1E |
| Resolution | 2 mG | 1.5 mG |
| Interface | I2C only | I2C or SPI |
| Price | Very low | Higher |
| Library | QMC5883L / HMC5883L | Adafruit LIS2MDL |

> **Note:** Many modules sold as "HMC5883L" actually contain a clone chip called **QMC5883L** (made by QST Corp). The pinout is identical but the register map is different — the QMC5883L library is needed if the HMC5883L library gives "sensor not found." The module looks identical. Check the chip marking under magnification if unsure.

---

## Hardware Overview

```
HMC5883L Module Pins:

  VCC  ── 3.3V (or 5V on modules with onboard regulator)
  GND  ── Ground
  SCL  ── I2C Clock
  SDA  ── I2C Data
  DRDY ── Data Ready interrupt (optional)
```

I2C address: **0x1E** (fixed, not configurable)

---

## Wiring to ESP32

| HMC5883L Pin | ESP32 Pin | Notes |
|-------------|-----------|-------|
| VCC | 3.3V | |
| GND | GND | |
| SCL | GPIO 22 | Shared I2C bus |
| SDA | GPIO 21 | Shared I2C bus |
| DRDY | Not connected | Optional interrupt |

Shares the I2C bus safely with MPU-6050 (0x68), BMP280 (0x76), LSM303AGR accel (0x19) — address 0x1E doesn't conflict with any of those.

---

## Installing the Library

First, scan the I2C bus to confirm which chip your module actually has (see scan sketch below). Then install the correct library:

**If it's HMC5883L (genuine Honeywell):**
- Search: `HMC5883L`
- Install: **"DFRobot_QMC5883"** or **"Adafruit HMC5883 Unified"**

**If it's QMC5883L (clone — very common):**
- Search: `QMC5883`
- Install: **"QMC5883LCompass" by MPrograms** — works with both chips

The QMC5883LCompass library is the safest choice — it handles both variants.

---

## I2C Scanner — Confirm Your Chip

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Scanning I2C bus...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("Scan complete.");
}

void loop() {}
```

Expected output with HMC5883L/QMC5883L: `Found device at 0x1E` or `Found device at 0x0D` (QMC5883L uses 0x0D on some modules — in that case install the QMC5883L library specifically).

---

## Code Example 1 — Basic Compass Reading

```cpp
#include <Wire.h>
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  compass.init();
  Serial.println("Compass ready");
}

void loop() {
  compass.read();

  int x = compass.getX();   // raw magnetic field values
  int y = compass.getY();
  int z = compass.getZ();

  int heading = compass.getAzimuth();   // 0–359 degrees

  char direction[3];
  compass.getDirection(direction, heading);

  Serial.print("X: "); Serial.print(x);
  Serial.print("  Y: "); Serial.print(y);
  Serial.print("  Z: "); Serial.print(z);
  Serial.print("  Heading: "); Serial.print(heading);
  Serial.print("°  Direction: "); Serial.println(direction);

  delay(250);
}
```

---

## Code Example 2 — Manual Heading Calculation

Understanding the math helps when integrating into a flight controller:

```cpp
#include <Wire.h>
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

// Hard-iron calibration offsets — run calibration first
float offsetX = 0, offsetY = 0;

// Magnetic declination for your city (degrees)
// Find at: magnetic-declination.com
const float DECLINATION = -0.5;   // example for central India

void setup() {
  Serial.begin(115200);
  Wire.begin();
  compass.init();
}

void loop() {
  compass.read();

  float x = compass.getX() - offsetX;
  float y = compass.getY() - offsetY;

  // Calculate heading from magnetic field components
  float heading = atan2(y, x) * 180.0 / PI;

  // Add declination correction
  heading += DECLINATION;

  // Normalize to 0–360
  if (heading < 0)   heading += 360;
  if (heading > 360) heading -= 360;

  Serial.print("Heading: ");
  Serial.print(heading, 1);
  Serial.print("°  ");

  // Cardinal direction
  if      (heading < 22.5  || heading >= 337.5) Serial.println("North");
  else if (heading < 67.5)  Serial.println("NE");
  else if (heading < 112.5) Serial.println("East");
  else if (heading < 157.5) Serial.println("SE");
  else if (heading < 202.5) Serial.println("South");
  else if (heading < 247.5) Serial.println("SW");
  else if (heading < 292.5) Serial.println("West");
  else                       Serial.println("NW");

  delay(200);
}
```

---

## Code Example 3 — Magnetometer Calibration

Rotate the sensor slowly through all orientations for 30–60 seconds.
The sketch finds the min and max of X and Y to compute hard-iron offsets.

```cpp
#include <Wire.h>
#include <QMC5883LCompass.h>

QMC5883LCompass compass;

float minX = 99999, maxX = -99999;
float minY = 99999, maxY = -99999;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  compass.init();
  Serial.println("Calibrating — rotate sensor in all directions for 60 seconds...");
}

void loop() {
  compass.read();

  float x = compass.getX();
  float y = compass.getY();

  if (x < minX) minX = x;
  if (x > maxX) maxX = x;
  if (y < minY) minY = y;
  if (y > maxY) maxY = y;

  float offX = (minX + maxX) / 2.0;
  float offY = (minY + maxY) / 2.0;

  Serial.print("offsetX: "); Serial.print(offX);
  Serial.print("  offsetY: "); Serial.println(offY);

  delay(50);
}
```

After 60 seconds, copy the final offsetX and offsetY values into your main sketch.

---

## Code Example 4 — Compass + MPU-6050 (Yaw Fusion)

Corrects gyroscope yaw drift using the magnetometer heading.

```cpp
#include <Wire.h>
#include <MPU6050_light.h>
#include <QMC5883LCompass.h>

MPU6050 mpu(Wire);
QMC5883LCompass compass;

float magOffsetX = -120.0, magOffsetY = 45.0;   // from your calibration
const float DECLINATION = -0.5;

// Complementary filter weight for yaw correction
const float YAW_ALPHA = 0.98;  // 98% gyro, 2% magnetometer

float yaw = 0;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  delay(500);
  mpu.calcOffsets();
  compass.init();
  lastTime = millis();
}

void loop() {
  mpu.update();
  compass.read();

  float dt = (millis() - lastTime) / 1000.0;
  lastTime = millis();

  // Magnetometer heading
  float mx = compass.getX() - magOffsetX;
  float my = compass.getY() - magOffsetY;
  float magYaw = atan2(my, mx) * 180.0 / PI + DECLINATION;
  if (magYaw < 0)   magYaw += 360;
  if (magYaw > 360) magYaw -= 360;

  // Gyro yaw integration (fast, drifts over time)
  float gyroYaw = mpu.getAngleZ();

  // Complementary filter: mostly gyro + small magnetometer correction
  yaw = YAW_ALPHA * gyroYaw + (1 - YAW_ALPHA) * magYaw;

  Serial.print("Pitch: "); Serial.print(mpu.getAngleX(), 1);
  Serial.print("°  Roll: "); Serial.print(mpu.getAngleY(), 1);
  Serial.print("°  Yaw: "); Serial.print(yaw, 1);
  Serial.print("°  MagHeading: "); Serial.print(magYaw, 1); Serial.println("°");

  delay(50);  // 20 Hz
}
```

---

## Understanding Magnetic Interference

The HMC5883L is extremely sensitive — it detects the Earth's field (~25–65 µT), but also:

| Source | Approximate Field at 5 cm |
|--------|--------------------------|
| Earth's field | 25–65 µT |
| Small neodymium magnet | 10,000+ µT |
| Brushless motor at 10A | 500–2000 µT |
| USB cable carrying 1A | 50–200 µT |
| ESP32 PCB traces | 10–50 µT |

**Mount the compass as far as possible from any current-carrying wire, motor, or battery.**
On a drone frame: top of the GPS mast, center of the frame, or on an arm extension.

---

## Hard-Iron vs Soft-Iron Distortion

**Hard-iron distortion:** a fixed offset added to all readings — caused by permanent magnets (speakers, motor magnets) near the sensor. Fixed by the calibration offsets (offsetX, offsetY).

**Soft-iron distortion:** the magnetic field is distorted in a direction-dependent way — caused by ferromagnetic materials (steel screws, aluminum frame). Requires a 3×3 matrix correction. For beginner drones, hard-iron calibration alone is usually sufficient.

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| "Sensor not found" at 0x1E | Clone chip at 0x0D | Install QMC5883LCompass library |
| Heading spins while stationary | No hard-iron calibration | Run calibration sketch |
| Heading correct but off by 10–20° | Magnetic declination | Add local declination value |
| Heading jumps when motors spin | Motor magnetic interference | Move sensor further from motors |
| Tilt changes the heading | No tilt compensation | Use accel data to compensate (see Example 3 in LSM303AGR tutorial) |
| Reading freezes at one value | Library mismatch (HMC vs QMC) | Try the other library |

---

## Heading Accuracy Expectations

With calibration + declination correction, the HMC5883L gives:
- **±1–2° accuracy** on a still bench, away from interference
- **±5–10° accuracy** mounted on a drone with calibration
- **±20–30° accuracy** without calibration or near magnetic interference

For GPS-assisted flight modes, ±5° is acceptable. For precise autonomous navigation, use a high-quality external compass module mounted away from interference.

---

## What to Try

1. Run the I2C scanner → confirm address (0x1E or 0x0D)
2. Run Example 1 → rotate sensor slowly, confirm heading changes
3. Point at a window facing North → verify reading is near 0° (after declination correction)
4. Run calibration → enter offsets → compare accuracy before and after
5. Run Example 4 with MPU-6050 → shake the sensor → observe yaw stays stable because magnetometer corrects gyro drift

---

## Next Steps

- **05_lsm303agr_accel_mag.md** — alternative combo chip with built-in accelerometer
- **01_mpu6050_imu.md** — pair with gyroscope for complete orientation solution
- **07_drone_electronics** — mount compass on GPS mast, integrate into flight stack
