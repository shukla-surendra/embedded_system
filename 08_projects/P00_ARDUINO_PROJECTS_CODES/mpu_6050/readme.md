# MPU6050 + BMP280 + LSM303DLHC
# Complete Beginner Guide

====================================================
WHAT IS THIS PROJECT?
====================================================

This project reads THREE sensors together:

1. MPU6050   - tells us how the board is tilting and rotating
2. BMP280    - tells us the altitude above sea level
3. LSM303DLHC - tells us which direction we are pointing (compass)

All three sensors are used in real drones.

MPU6050  = orientation sensor (most important for stability)
BMP280   = altitude sensor (used for altitude hold)
LSM303   = compass sensor  (used for heading/direction lock)

====================================================
SENSOR 1: MPU6050
====================================================

MPU6050 is a motion sensor module.

It contains TWO sensors inside one chip:

1. Accelerometer
2. Gyroscope

Also contains:

3. Temperature sensor

I2C Address: 0x68

----------------------------------------------------
ACCELEROMETER
----------------------------------------------------

Measures: "acceleration force"

Mainly it measures gravity.

This helps us understand:
- tilt angle
- which way is down
- movement

Axes:
- X  (forward / backward)
- Y  (left / right)
- Z  (up / down)

Units: m/s²

Earth gravity = 9.81 m/s²

When sensor is flat:
X = 0
Y = 0
Z = 9.8

When tilted FORWARD:
X becomes positive

When tilted BACKWARD:
X becomes negative

When tilted RIGHT:
Y becomes positive

When tilted LEFT:
Y becomes negative

----------------------------------------------------
GYROSCOPE
----------------------------------------------------

Measures: "rotation speed"

This helps us understand:
- how fast the board is spinning
- in which direction it is spinning

Axes:
- X rotation  (roll  - tilting left/right)
- Y rotation  (pitch - tilting forward/back)
- Z rotation  (yaw   - spinning flat like compass)

Units: degrees/second

When sensor is still:
GX = 0
GY = 0
GZ = 0

When rotating fast:
GX = 150  (spinning 150 degrees every second)

IMPORTANT:
Gyroscope does NOT directly give angle.
It gives how fast angle is changing.

To get angle from gyro:

angle += gyro * dt

where dt = time since last reading (in seconds)

Example:
gyro = 90 deg/sec
dt   = 1 second
angle = 90 degrees

----------------------------------------------------
TEMPERATURE SENSOR
----------------------------------------------------

MPU6050 has a built-in temperature sensor.

Measures the chip temperature.

Typical values: 25°C to 45°C

Formula for raw to Celsius:

Temp(C) = rawTemp / 340 + 36.53

This is from the MPU6050 datasheet.

----------------------------------------------------
MPU6050 COORDINATE SYSTEM
----------------------------------------------------

         ^ Z (up/down)
         |
         |  ^ X (forward = pitch)
         | /
         |/
         +-----------> Y (right = roll)

ROLL  = tilt around X axis -> board tips LEFT or RIGHT
PITCH = tilt around Y axis -> board tips FORWARD or BACK
YAW   = spin  around Z axis -> board rotates flat

----------------------------------------------------
RAW ADC VALUES  (MPU6050)
----------------------------------------------------

The chip converts physical motion into numbers.
These raw numbers come directly from the chip registers.

Reading raw:
Start at register 0x3B
Read 14 bytes

Register map:
0x3B-0x3C = Accel X  (high byte, low byte)
0x3D-0x3E = Accel Y
0x3F-0x40 = Accel Z
0x41-0x42 = Temperature
0x43-0x44 = Gyro X
0x45-0x46 = Gyro Y
0x47-0x48 = Gyro Z

Accel range set to +/-8G:
1G = 4096 LSB (raw counts)

Gyro range set to +/-500 deg/s:
1 deg/s = 65.5 LSB (raw counts)

Temperature formula:
Temp(C) = rawTemp / 340 + 36.53

----------------------------------------------------
WHY GYRO DRIFTS
----------------------------------------------------

Small errors accumulate over time.

0.1 + 0.1 + 0.1 + ...

After some time the sensor thinks it rotated
even when it did not.

This is called: GYRO DRIFT

----------------------------------------------------
WHY ACCELEROMETER ALONE FAILS
----------------------------------------------------

Accelerometer becomes noisy during movement.

Example:
Drone moves forward fast.
Accelerometer reads this as a tilt
even if the drone is perfectly level.

----------------------------------------------------
SOLUTION: COMPLEMENTARY FILTER
----------------------------------------------------

Combine gyro (fast, short-term accurate) with
accelerometer (slow, long-term accurate).

Formula:

angle =
  0.98 * gyro_prediction
  +
  0.02 * accelerometer_angle

Meaning:
- trust gyro 98% for fast movements
- slowly correct using accelerometer 2%
- gyro drift gets corrected over time
- accelerometer noise gets smoothed out

In this program:
Gyro is integrated at 100Hz (every 10ms)
Display updates every 1 second

This is why gyro integration is in a separate
fast loop - not inside the display timer.

====================================================
SENSOR 2: BMP280
====================================================

BMP280 is a barometric pressure sensor.

It measures:
1. Air pressure (hPa)
2. Temperature (C)
3. Altitude (m) - calculated from pressure

I2C Address:
0x76 = default (SDO pin LOW or floating)
0x77 = alternative (SDO pin HIGH)

----------------------------------------------------
HOW ALTITUDE WORKS
----------------------------------------------------

Air pressure decreases as you go higher.

At sea level:
Pressure = 1013.25 hPa (standard atmosphere)

At 500m altitude:
Pressure is lower

At 1000m altitude:
Pressure is even lower

The chip converts pressure to altitude using
the barometric formula.

This is the same formula real aircraft use.

----------------------------------------------------
BMP280 VALUES
----------------------------------------------------

readPressure()    - pressure in Pascals (Pa)
                    divide by 100 to get hPa

readTemperature() - temperature in Celsius

readAltitude(1013.25) - altitude in meters
                        pass sea level pressure as reference

----------------------------------------------------
HOME ALTITUDE
----------------------------------------------------

In this program:
Altitude at startup = home altitude (baseline)

Relative altitude = current - home

Example:
Home    = 330 m
Current = 345 m
Relative = +15 m

This is how drones track how high they have climbed.

Type B in Serial Monitor to reset home altitude
to wherever the sensor currently is.

----------------------------------------------------
ACCURACY
----------------------------------------------------

BMP280 is accurate to about 1 meter in stable air.

Accuracy is reduced by:
- Temperature changes
- Wind
- Indoor air conditioning
- Moving indoors vs outdoors

----------------------------------------------------
RAW ADC VALUES  (BMP280)
----------------------------------------------------

Reading raw:
Start at register 0xF7
Read 6 bytes

Register map:
0xF7-0xF9 = Pressure ADC (20-bit)
0xFA-0xFC = Temperature ADC (20-bit)

These are raw counts BEFORE compensation.
The chip has calibration trim data stored
in registers 0x88-0x9F.

The Adafruit library reads the trim data
and applies it automatically.

Raw values alone are not useful.
The library converts them to hPa and Celsius.

====================================================
SENSOR 3: LSM303DLHC
====================================================

LSM303DLHC is actually TWO sensors in one package:

1. Accelerometer  (I2C address: 0x19)
2. Magnetometer   (I2C address: 0x1E)

Accelerometer: similar to MPU6050 accel
Magnetometer:  measures magnetic field (like compass)

This sensor is used in drones for:
- Compass heading
- Direction lock
- GPS navigation assistance

----------------------------------------------------
MAGNETOMETER (COMPASS)
----------------------------------------------------

Measures the Earth's magnetic field.

Axes:
- X
- Y
- Z

Units: microtesla (uT)

From X and Y field values, we can calculate
which way North is:

heading = atan2(Y, Z) * 57.2958

This gives heading 0 to 360 degrees.

0   = North
90  = East
180 = South
270 = West

----------------------------------------------------
TILT-COMPENSATED HEADING
----------------------------------------------------

Simple heading only works when sensor is FLAT.

When the sensor is tilted, heading becomes wrong.

Example:
Sensor tilted 30 degrees forward.
Simple compass reads wrong direction.

Solution: tilt compensation.

Use MPU6050 roll and pitch to mathematically
correct the magnetic field values before
calculating heading.

Formula:

xh = magX * cos(pitch) + magZ * sin(pitch)

yh = magX * sin(roll) * sin(pitch)
   + magY * cos(roll)
   - magZ * sin(roll) * cos(pitch)

heading = atan2(yh, xh) * 57.2958

This is why MPU6050 and LSM303 work together.
MPU6050 gives roll/pitch.
LSM303 gives magnetic field.
Combined = accurate compass in any orientation.

----------------------------------------------------
LSM303 ACCELEROMETER VALUES
----------------------------------------------------

Output is the same as MPU6050 accelerometer:
m/s² on X, Y, Z axes.

In this program:
LSM303 accel is shown for comparison with
the MPU6050 accel.

Both should read very similar values.
Small differences are due to sensor placement
and different calibration.

----------------------------------------------------
RAW ADC VALUES  (LSM303 Accelerometer)
----------------------------------------------------

I2C address: 0x19

Reading raw:
Write 0xA8 (= 0x28 | 0x80)
The 0x80 bit enables auto-increment.
Read 6 bytes.

Register map (0x28-0x2D):
0x28 = OUT_X_L_A   (X low byte)
0x29 = OUT_X_H_A   (X high byte)
0x2A = OUT_Y_L_A   (Y low byte)
0x2B = OUT_Y_H_A   (Y high byte)
0x2C = OUT_Z_L_A   (Z low byte)
0x2D = OUT_Z_H_A   (Z high byte)

Data format:
16-bit, LEFT-ALIGNED.
Right-shift by 4 to get 12-bit actual value.

raw = (highByte << 8 | lowByte) >> 4

----------------------------------------------------
RAW ADC VALUES  (LSM303 Magnetometer)
----------------------------------------------------

I2C address: 0x1E

Reading raw:
Write 0x03 (OUT_X_H_M)
Read 6 bytes.

IMPORTANT - Register order is unusual:
0x03 = OUT_X_H_M  (X high)
0x04 = OUT_X_L_M  (X low)
0x05 = OUT_Z_H_M  (Z high)   <- Z comes BEFORE Y !
0x06 = OUT_Z_L_M  (Z low)
0x07 = OUT_Y_H_M  (Y high)
0x08 = OUT_Y_L_M  (Y low)

This is a quirk of this specific chip.
Most sensors are X, Y, Z order.
LSM303DLHC magnetometer is X, Z, Y order.

16-bit signed, big-endian:
raw = (highByte << 8) | lowByte

====================================================
HOW ALL THREE SENSORS WORK TOGETHER
====================================================

In a real drone flight controller:

MPU6050
- Measures roll, pitch, yaw rate
- Used for PID stabilization
- Updates at 100Hz or faster

BMP280
- Measures altitude
- Used for altitude hold mode
- Drone keeps itself at a set height

LSM303 Magnetometer
- Measures compass heading
- Used for position hold and navigation
- Drone knows which way it is facing
- GPS navigation needs this

LSM303 uses MPU6050 data to correct
for tilted compass readings.

Together these three sensors give the drone:
- Am I level?           (MPU6050 accel)
- Am I rotating?        (MPU6050 gyro)
- How high am I?        (BMP280)
- Which way am I facing?(LSM303 mag + MPU6050 tilt)

====================================================
SENSOR FUSION
====================================================

Sensor fusion = combining multiple sensors
to get better results than any one alone.

Examples in this project:

1. MPU6050 accel + gyro:
   Complementary filter gives accurate tilt angle.
   Gyro is fast, accel corrects drift.

2. LSM303 mag + MPU6050 accel:
   Tilt-compensated compass heading.
   Works even when sensor is tilted.

3. All three together:
   Full 3D orientation + altitude + heading.
   This is called: 9DOF + altitude
   (9 Degrees of Freedom = 3 accel + 3 gyro + 3 mag)

====================================================
WIRING
====================================================

All three sensors share the SAME I2C bus.

I2C uses only 2 wires:
SDA = data
SCL = clock

Connect all sensors to the same SDA and SCL pins.

----------------------------------------------------
ESP32 CONNECTIONS
----------------------------------------------------

SDA -> GPIO 18
SCL -> GPIO 19

All sensors:
VCC -> 3.3V
GND -> GND
SDA -> GPIO 18
SCL -> GPIO 19

----------------------------------------------------
I2C ADDRESSES
----------------------------------------------------

MPU6050              -> 0x68
BMP280  (SDO LOW)    -> 0x76
BMP280  (SDO HIGH)   -> 0x77
LSM303 Accelerometer -> 0x19
LSM303 Magnetometer  -> 0x1E

All five addresses are different.
No conflicts on the same bus.

----------------------------------------------------
WIRING DIAGRAM
----------------------------------------------------

            ESP32
              |
    +---------+----------+
    |         |          |
  GPIO18    GPIO19     3.3V + GND
  (SDA)     (SCL)
    |         |
    +---------+----------+----------+
    |                    |          |
  MPU6050             BMP280   LSM303DLHC
  0x68                0x76     0x19 + 0x1E

====================================================
ARDUINO LIBRARIES REQUIRED
====================================================

Install from Arduino IDE:
Sketch -> Include Library -> Manage Libraries

1. Adafruit MPU6050
2. Adafruit BMP280
3. Adafruit Unified Sensor  (dependency for above two)

LSM303DLHC does NOT need a library.
It is driven directly using Wire.h (built-in).
All LSM303 communication is done by writing and
reading I2C registers manually.

====================================================
PROGRAM DETAILS
====================================================

File: mpu_6050.ino

----------------------------------------------------
REFRESH RATE
----------------------------------------------------

Gyro integration:  every 10ms  (100Hz)
Screen display:    every 1000ms (1 second)

These are two separate timers.
The gyro integration MUST run fast.
The display can run slow.

----------------------------------------------------
WHAT IS DISPLAYED EVERY SECOND
----------------------------------------------------

1. Orientation bars (roll, pitch, yaw)
2. Front view ASCII diagram (roll tilt)
3. Side view ASCII diagram  (pitch tilt)
4. Top view compass rose    (yaw direction)
5. MPU6050 raw ADC values
6. Compass rose with heading (LSM303 mag)
7. LSM303 raw ADC values
8. Altitude bar and altimeter (BMP280)
9. BMP280 raw ADC values

----------------------------------------------------
SERIAL COMMANDS
----------------------------------------------------

Open Serial Monitor at 115200 baud.

B = Reset home altitude
    Sets current altitude as the new baseline.
    Use this when you move to a new location.

----------------------------------------------------
WHAT THE BARS MEAN
----------------------------------------------------

Symmetric bar (roll, pitch, yaw):

  LEFT [........#.....|...........] RIGHT

  # = current sensor position
  | = center (level/zero point)
  O = sensor is at dead center (level)

Heading bar (0 to 360):

  N(0) [......#......................] N(360)

  # = current compass heading

----------------------------------------------------
WHAT THE COMPASS ROSE MEANS
----------------------------------------------------

  NW    N   NE
   W  [+]    E
  SW    S   SE

  * marks the active direction:

  Example (pointing WEST):

  NW    N   NE
  *W*  [+]   E
  SW    S   SE

====================================================
AXIS REFERENCE
====================================================

----------------------------------------------------
MPU6050
----------------------------------------------------

         ^ Z (up)
         |
         |   ^ X (forward)
         |  /
         | /
         +-----------> Y (right)

ROLL  = rotate around X = tips LEFT / RIGHT
PITCH = rotate around Y = tips FORWARD / BACKWARD
YAW   = rotate around Z = spins FLAT like compass

----------------------------------------------------
LSM303 MAGNETOMETER
----------------------------------------------------

Magnetic field axes follow the same X, Y, Z system.

X = forward
Y = right
Z = down

heading = atan2(Y, X) when flat

With tilt correction: uses MPU6050 roll and pitch.

====================================================
COMMON PROBLEMS
====================================================

----------------------------------------------------
MPU6050 NOT DETECTED
----------------------------------------------------

Check:
- SDA wire to GPIO 18
- SCL wire to GPIO 19
- VCC to 3.3V (not 5V)
- GND connected
- I2C address is 0x68

----------------------------------------------------
BMP280 NOT DETECTED
----------------------------------------------------

Check:
- Same wiring as MPU6050
- Try changing #define BMP_ADDR 0x77
  if SDO pin is connected to 3.3V

----------------------------------------------------
LSM303 NOT DETECTED
----------------------------------------------------

Accel (0x19): Check SDO/SA0 pin on module
Mag   (0x1E): Check DRDY/INT pins not interfering

----------------------------------------------------
GYRO DRIFTS OVER TIME
----------------------------------------------------

This is normal.
The complementary filter slowly corrects it.
If drift is severe:
- improve filter coefficient
- add kalman filter

----------------------------------------------------
COMPASS HEADING JUMPS AROUND
----------------------------------------------------

Causes:
- magnetic interference (motors, wires, phones)
- need calibration (hard iron + soft iron)
- sensor not stable

----------------------------------------------------
ALTITUDE READS WRONG
----------------------------------------------------

Causes:
- wrong sea level pressure reference (1013.25 default)
- indoor air pressure changes
- sensor not yet settled

Fix:
Type B to reset baseline at current location.

----------------------------------------------------
VALUES NOISY
----------------------------------------------------

Causes:
- vibration from motors
- poor power supply
- long wires acting as antennas

Solution:
- add decoupling capacitors to power pins
- keep sensor away from motors
- add software filtering

====================================================
DRONE CONTEXT
====================================================

These three sensors together enable:

STABILIZATION  (MPU6050 gyro + accel)
- Drone detects tilt
- PID controller corrects motors
- Happens 100-500 times per second

ALTITUDE HOLD  (BMP280)
- Drone measures pressure
- Compares to target altitude
- Adjusts throttle automatically

HEADING LOCK   (LSM303 magnetometer)
- Drone knows which way it is facing
- Maintains direction against wind
- GPS can navigate using this

WITHOUT MPU6050:
Drone cannot balance - crashes immediately.

WITHOUT BMP280:
Drone drifts in altitude - goes up or down slowly.

WITHOUT LSM303:
Drone rotates randomly - cannot hold direction.

====================================================
IMPORTANT CONCEPTS SUMMARY
====================================================

MPU6050 Accelerometer:
-> Senses gravity -> tells us tilt angle

MPU6050 Gyroscope:
-> Senses rotation speed -> we integrate to get angle

Complementary filter:
-> Combines accel (long-term) + gyro (short-term)

BMP280:
-> Pressure drops with altitude -> we calculate height

LSM303 Magnetometer:
-> Senses Earth's magnetic field -> compass heading

Tilt compensation:
-> MPU6050 roll/pitch corrects LSM303 compass
-> Accurate heading even when sensor is tilted

Sensor fusion:
-> All sensors together are better than any one alone

====================================================
END
====================================================
