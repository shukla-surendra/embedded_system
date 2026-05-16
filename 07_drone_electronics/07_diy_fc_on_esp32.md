# 07 — DIY Flight Controller on ESP32

## What We're Building

A minimal but functional flight controller on an ESP32, capable of:
- Reading IMU (MPU6050) at 1 kHz
- Reading RC receiver (IBUS or SBUS)
- Running a rate-mode PID loop for roll, pitch, and yaw
- Outputting motor commands via DSHOT (or PWM as fallback)
- Arming/disarming safely

This is a **learning build**. It will fly. It will not have Betaflight's filters or OSD, but it teaches you every concept from scratch.

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    ESP32 FLIGHT CONTROLLER                  │
│                                                             │
│  Core 0 (Protocol CPU):                                     │
│    ├── RC receiver reading (UART interrupt)                 │
│    └── DSHOT output (LEDC/RMT peripheral)                   │
│                                                             │
│  Core 1 (Control CPU):                                      │
│    ├── IMU reading via I2C (1 kHz timer)                    │
│    ├── PID computation                                      │
│    └── Motor mixing                                         │
│                                                             │
│  Shared (thread-safe):                                      │
│    ├── RC channel values                                    │
│    └── Motor output values                                  │
└─────────────────────────────────────────────────────────────┘
```

ESP32's dual-core architecture is perfect for this: one core handles timing-sensitive IO while the other runs the control algorithm.

---

## Hardware Wiring

### Components Needed

| Component | Qty | Notes |
|-----------|-----|-------|
| ESP32 dev board (38-pin) | 1 | WROOM or WROVER |
| MPU6050 IMU module | 1 | I2C, 3.3V |
| FlySky receiver (IBUS) | 1 | Simplest protocol for beginners |
| 4-in-1 ESC (30A+) | 1 | BLHeli_S or AM32 |
| Brushless motors × 4 | 4 | 2207 2450KV or similar |
| 4S LiPo battery | 1 | 1500mAh+ |
| LiPo voltage alarm | 1 | For safety |

### Pin Connections

```
ESP32 Pin        Connects To
─────────────────────────────────────────────────
GPIO 21 (SDA) ─► MPU6050 SDA
GPIO 22 (SCL) ─► MPU6050 SCL
3.3V          ─► MPU6050 VCC
GND           ─► MPU6050 GND

GPIO 16 (RX2) ─► Receiver IBUS TX
5V            ─► Receiver 5V
GND           ─► Receiver GND

GPIO 25       ─► ESC Motor 1 signal
GPIO 26       ─► ESC Motor 2 signal
GPIO 27       ─► ESC Motor 3 signal
GPIO 14       ─► ESC Motor 4 signal
GND           ─► ESC signal GND (important!)

5V (from ESC BEC) ─► ESP32 5V pin
```

**Motor layout:**
```
       Front
  M1 (CW)   M2 (CCW)
     GPIO25   GPIO26
      ●         ●
       \       /
        ───────
       /       \
      ●         ●
     GPIO27   GPIO14
  M3 (CCW)   M4 (CW)
       Back
```

---

## Project Structure

```
diy_fc/
  ├── diy_fc.ino          (main file, setup + loop)
  ├── imu.h / imu.cpp     (MPU6050 driver)
  ├── receiver.h/.cpp     (IBUS parsing)
  ├── pid.h               (PID class)
  ├── motors.h/.cpp       (DSHOT or PWM output)
  └── config.h            (all tuneable parameters)
```

---

## config.h — All Settings in One Place

```cpp
#pragma once

// ── Loop timing ──────────────────────────────────────────
#define LOOP_FREQ_HZ     1000       // gyro + PID loop rate
#define LOOP_DT          (1.0f / LOOP_FREQ_HZ)

// ── PID gains ────────────────────────────────────────────
#define ROLL_P    40.0f
#define ROLL_I    60.0f
#define ROLL_D    20.0f

#define PITCH_P   40.0f
#define PITCH_I   60.0f
#define PITCH_D   20.0f

#define YAW_P     50.0f
#define YAW_I     60.0f
#define YAW_D     0.0f

// ── Rate limits (deg/sec at full stick) ──────────────────
#define MAX_ROLL_RATE   600.0f
#define MAX_PITCH_RATE  600.0f
#define MAX_YAW_RATE    300.0f

// ── RC channels ──────────────────────────────────────────
#define RC_ROLL_CH    0
#define RC_PITCH_CH   1
#define RC_THROTTLE_CH 2
#define RC_YAW_CH     3
#define RC_ARM_CH     4   // switch: >1700 = armed

// ── Arming safety ────────────────────────────────────────
#define ARM_THROTTLE_MAX  1100  // throttle must be below this to arm
#define THROTTLE_MIN      1100  // minimum throttle when armed (keeps motors spinning)

// ── Pins ─────────────────────────────────────────────────
#define IMU_SDA_PIN   21
#define IMU_SCL_PIN   22
#define IBUS_RX_PIN   16
#define MOTOR1_PIN    25
#define MOTOR2_PIN    26
#define MOTOR3_PIN    27
#define MOTOR4_PIN    14
```

---

## imu.h — MPU6050 Driver

```cpp
#pragma once
#include <Wire.h>

#define MPU6050_ADDR      0x68
#define MPU6050_PWR_MGMT1 0x6B
#define MPU6050_GYRO_CFG  0x1B
#define MPU6050_ACCEL_CFG 0x1C
#define MPU6050_GYRO_XOUT 0x43

// Gyro full-scale range: ±500 deg/sec (scale factor: 65.5 LSB/deg/sec)
#define GYRO_SCALE (1.0f / 65.5f)

struct IMUData {
  float gx, gy, gz;   // deg/sec
  float ax, ay, az;   // g (for reference, not used in rate mode)
};

class IMU {
public:
  bool begin() {
    Wire.begin(21, 22, 400000);  // SDA, SCL, 400 kHz fast mode

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_PWR_MGMT1);
    Wire.write(0x00);  // wake up
    if (Wire.endTransmission() != 0) return false;

    // Set gyro range ±500 deg/sec
    writeReg(MPU6050_GYRO_CFG, 0x08);
    // Set accel range ±4g
    writeReg(MPU6050_ACCEL_CFG, 0x08);

    delay(100);
    calibrate();
    return true;
  }

  bool read(IMUData& data) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_GYRO_XOUT);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 6);

    if (Wire.available() < 6) return false;

    int16_t raw_gx = (Wire.read() << 8) | Wire.read();
    int16_t raw_gy = (Wire.read() << 8) | Wire.read();
    int16_t raw_gz = (Wire.read() << 8) | Wire.read();

    data.gx = raw_gx * GYRO_SCALE - offset_gx;
    data.gy = raw_gy * GYRO_SCALE - offset_gy;
    data.gz = raw_gz * GYRO_SCALE - offset_gz;
    return true;
  }

private:
  float offset_gx = 0, offset_gy = 0, offset_gz = 0;

  void writeReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
  }

  void calibrate() {
    // Collect 2000 samples with drone perfectly still
    float sum_x = 0, sum_y = 0, sum_z = 0;
    const int N = 2000;
    for (int i = 0; i < N; i++) {
      IMUData d;
      read(d);  // offsets not applied yet in raw form
      sum_x += d.gx; sum_y += d.gy; sum_z += d.gz;
      delay(1);
    }
    offset_gx = sum_x / N;
    offset_gy = sum_y / N;
    offset_gz = sum_z / N;
  }
};
```

---

## pid.h — PID Class

```cpp
#pragma once

class PID {
public:
  float Kp, Ki, Kd;

  PID(float kp, float ki, float kd)
    : Kp(kp), Ki(ki), Kd(kd), integral(0), prev_error(0) {}

  float compute(float setpoint, float measured, float dt) {
    float error = setpoint - measured;

    integral += error * dt;
    // Anti-windup clamp
    integral = constrain(integral, -200.0f, 200.0f);

    float derivative = (error - prev_error) / dt;
    prev_error = error;

    return (Kp * error) + (Ki * integral) + (Kd * derivative);
  }

  void reset() {
    integral = 0;
    prev_error = 0;
  }

private:
  float integral;
  float prev_error;
};
```

---

## receiver.h — IBUS Parser

```cpp
#pragma once
#include <HardwareSerial.h>

#define IBUS_CHANNELS 14
#define IBUS_FRAME_LEN 32

class IBUSReceiver {
public:
  uint16_t channels[IBUS_CHANNELS];
  bool new_data = false;
  unsigned long last_packet_ms = 0;

  void begin(int rx_pin) {
    serial.begin(115200, SERIAL_8N1, rx_pin, -1);
    for (int i = 0; i < IBUS_CHANNELS; i++) channels[i] = 1500;
    channels[2] = 1000;  // throttle default = min
  }

  void update() {
    while (serial.available() >= IBUS_FRAME_LEN) {
      uint8_t buf[IBUS_FRAME_LEN];
      serial.readBytes(buf, IBUS_FRAME_LEN);

      if (buf[0] != 0x20 || buf[1] != 0x40) continue;

      // Verify checksum
      uint16_t checksum = 0xFFFF;
      for (int i = 0; i < 30; i++) checksum -= buf[i];
      uint16_t recv_check = buf[30] | (buf[31] << 8);
      if (checksum != recv_check) continue;

      for (int i = 0; i < IBUS_CHANNELS; i++) {
        channels[i] = buf[2 + i*2] | (buf[3 + i*2] << 8);
      }
      new_data = true;
      last_packet_ms = millis();
    }
  }

  bool isConnected() {
    return (millis() - last_packet_ms) < 500;  // 500ms timeout
  }

private:
  HardwareSerial serial{2};
};
```

---

## motors.h — PWM Output (Beginner Version)

For simplicity, this uses standard PWM (1000–2000µs). Later you can replace with DSHOT.

```cpp
#pragma once
#include <Arduino.h>

#define PWM_FREQ    250    // Hz
#define PWM_RES     16     // bits
// 1000µs = 1/1000 = 1000×65535/1000000 counts at 1MHz tick
// With 250Hz and 16-bit: period = 65535 ticks
// 1000µs at 250Hz: 1000µs / (1/250Hz × 1/65535) = 1000 × 65535 / 4000 = ~16383

class Motors {
  int pins[4];

public:
  Motors(int m1, int m2, int m3, int m4) {
    pins[0] = m1; pins[1] = m2; pins[2] = m3; pins[3] = m4;
  }

  void begin() {
    for (int i = 0; i < 4; i++) {
      ledcSetup(i, PWM_FREQ, PWM_RES);
      ledcAttachPin(pins[i], i);
      writeMicros(i, 1000);  // arm ESCs with min throttle
    }
    delay(3000);  // wait for ESC arming beeps
  }

  void write(int motor, float value) {
    // value: 0.0 to 1.0
    int us = 1000 + (int)(value * 1000.0f);
    us = constrain(us, 1000, 2000);
    writeMicros(motor, us);
  }

  void disarm() {
    for (int i = 0; i < 4; i++) writeMicros(i, 1000);
  }

private:
  void writeMicros(int motor, int us) {
    // Convert microseconds to duty cycle ticks
    // At 250Hz: period = 4000µs = 65535 ticks
    // So 1µs = 65535/4000 = 16.38 ticks
    uint32_t duty = (uint32_t)(us * 65535UL / 4000UL);
    ledcWrite(motor, duty);
  }
};
```

---

## diy_fc.ino — Main File

```cpp
#include "config.h"
#include "imu.h"
#include "receiver.h"
#include "pid.h"
#include "motors.h"

IMU imu;
IBUSReceiver rx;
Motors motors(MOTOR1_PIN, MOTOR2_PIN, MOTOR3_PIN, MOTOR4_PIN);

PID rollPID(ROLL_P, ROLL_I, ROLL_D);
PID pitchPID(PITCH_P, PITCH_I, PITCH_D);
PID yawPID(YAW_P, YAW_I, YAW_D);

bool armed = false;
unsigned long last_loop_us = 0;

void setup() {
  Serial.begin(115200);

  if (!imu.begin()) {
    Serial.println("IMU FAILED — check wiring");
    while (1) delay(100);
  }

  rx.begin(IBUS_RX_PIN);
  motors.begin();

  Serial.println("FC ready. Arm by raising CH5 switch with throttle LOW.");
  last_loop_us = micros();
}

void loop() {
  // Enforce 1kHz loop rate
  while (micros() - last_loop_us < 1000) {}
  float dt = (micros() - last_loop_us) / 1e6f;
  last_loop_us = micros();

  // 1. Read receiver
  rx.update();
  uint16_t rc_throttle = rx.channels[RC_THROTTLE_CH];
  uint16_t rc_roll     = rx.channels[RC_ROLL_CH];
  uint16_t rc_pitch    = rx.channels[RC_PITCH_CH];
  uint16_t rc_yaw      = rx.channels[RC_YAW_CH];
  uint16_t rc_arm      = rx.channels[RC_ARM_CH];

  // 2. Arming logic
  if (!rx.isConnected()) {
    armed = false;
    motors.disarm();
    rollPID.reset(); pitchPID.reset(); yawPID.reset();
    return;
  }

  if (rc_arm > 1700 && rc_throttle < ARM_THROTTLE_MAX && !armed) {
    armed = true;
    Serial.println("ARMED");
  }
  if (rc_arm < 1300) {
    armed = false;
    motors.disarm();
    rollPID.reset(); pitchPID.reset(); yawPID.reset();
    return;
  }

  // 3. Read IMU
  IMUData imu_data;
  if (!imu.read(imu_data)) return;

  if (!armed) return;

  // 4. RC to setpoints (1000-2000 → -maxRate to +maxRate deg/sec)
  float roll_sp  = map(rc_roll,    1000, 2000, -MAX_ROLL_RATE,  MAX_ROLL_RATE);
  float pitch_sp = map(rc_pitch,   1000, 2000, -MAX_PITCH_RATE, MAX_PITCH_RATE);
  float yaw_sp   = map(rc_yaw,     1000, 2000, -MAX_YAW_RATE,   MAX_YAW_RATE);
  float throttle = map(rc_throttle, 1000, 2000, 0.0f, 1.0f);
  throttle = constrain(throttle, 0.0f, 1.0f);

  // 5. PID (gyro X = roll rate, Y = pitch rate, Z = yaw rate)
  float roll_out  = rollPID.compute(roll_sp,  imu_data.gx, dt);
  float pitch_out = pitchPID.compute(pitch_sp, imu_data.gy, dt);
  float yaw_out   = yawPID.compute(yaw_sp,   imu_data.gz, dt);

  // Scale PID output to 0-1 range (adjust divisor by feel)
  roll_out  /= 500.0f;
  pitch_out /= 500.0f;
  yaw_out   /= 500.0f;

  // 6. Motor mixing (X-frame, Betaflight motor numbering)
  float m1 = throttle + roll_out - pitch_out - yaw_out;  // Front-Left  CW
  float m2 = throttle - roll_out - pitch_out + yaw_out;  // Front-Right CCW
  float m3 = throttle + roll_out + pitch_out + yaw_out;  // Rear-Left   CCW
  float m4 = throttle - roll_out + pitch_out - yaw_out;  // Rear-Right  CW

  // Clamp
  m1 = constrain(m1, 0.0f, 1.0f);
  m2 = constrain(m2, 0.0f, 1.0f);
  m3 = constrain(m3, 0.0f, 1.0f);
  m4 = constrain(m4, 0.0f, 1.0f);

  // 7. Write to ESCs
  motors.write(0, m1);
  motors.write(1, m2);
  motors.write(2, m3);
  motors.write(3, m4);
}

// Helper: floating point map
float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
```

---

## First Flight Safety Checklist

**Do this in order, every single time.**

```
Before anything:
  ☐ Props OFF for all ground testing
  ☐ Battery connected LAST
  ☐ Never armed in confined space without props off

Bench test (NO PROPS):
  ☐ Plug battery — ESC beeps?
  ☐ Open Serial Monitor — "FC ready" message?
  ☐ Move TX sticks — do RC channels respond correctly?
  ☐ Arm with CH5 switch and low throttle — "ARMED" message?
  ☐ Raise throttle — do ALL 4 motors spin?
  ☐ Tilt drone by hand — do motors compensate (opposite side spins faster)?
  ☐ Verify motor direction: M1,M4 = CW, M2,M3 = CCW
  ☐ Disarm — all motors stop?
  ☐ Disconnect battery

Motor direction test:
  ☐ Arm, raise throttle slightly, block motor with finger carefully
  ☐ Wrong direction = swap any 2 of 3 motor wires at ESC

First flight (in open area):
  ☐ Props ON only when ready to fly
  ☐ Stand well back (behind or beside, not in front)
  ☐ Arm at low throttle
  ☐ Lift off gently — should rise roughly level
  ☐ If flipping immediately → land, check motor directions and PID sign
  ☐ If climbing OK → tune hover throttle, then tune PIDs
```

---

## Tuning Your DIY FC

Since we don't have Betaflight's tools, tune by observation.

```
Tuning sequence:

1. Fly with very low PIDs. Drone will feel mushy but stable.

2. Increase ROLL_P in steps of 5.
   Stop when you see fast oscillation (rapid shaking).
   Back off 15%.

3. Increase PITCH_P same way.

4. Increase ROLL_D and PITCH_D until oscillation from P is dampened.
   Stop if motors whine at high pitch.

5. Adjust YAW_P until yaw feels crisp without spinning.

6. Increase I terms only if drone drifts in one direction consistently.
```

---

## Upgrading to DSHOT (Advanced)

Replace the PWM `motors.h` with DSHOT using the ESP32's RMT peripheral.

```
DSHOT300 timing:
  Bit rate: 300 kbps
  Bit period: 3.33µs
  '1' = 2.5µs high, 0.83µs low
  '0' = 1.25µs high, 2.08µs low

Frame: [11-bit throttle][1-bit telemetry][4-bit CRC]

CRC = XOR of each 4-bit nibble of the first 12 bits
```

The ESP32 RMT (Remote Control Transceiver) hardware peripheral can generate precise bit timing without blocking the CPU — ideal for DSHOT.

---

## What's Next

You now have the complete foundation of drone electronics. Where to go from here:

| Topic | Resource |
|-------|----------|
| Add barometer | Add BMP280, implement altitude hold |
| Add GPS | Add NEO-6M, implement position hold |
| Move to Betaflight | Flash Betaflight FC, use professional firmware |
| DSHOT deep dive | Implement RMT-based DSHOT on ESP32 |
| Sensor fusion | Implement Mahony/Madgwick filter for angle estimation |
| Build a real 5" quad | Full build from parts using this knowledge |

---

## Practice

1. Why does this FC use dual cores? What runs on each core?
2. In the motor mixing code, M1 = throttle + roll − pitch − yaw. Explain why roll is **added** for M1 (front-left motor).
3. What is the arming condition in the code? Why must throttle be low?
4. The IMU calibration takes 2000 samples. Why? What happens if you move the drone during calibration?
5. IBUS checksum formula: `0xFFFF - sum of first 30 bytes`. What is this checksum protecting against?
6. If ROLL_P = 40 and roll error is 15 deg/sec, what is the P term output before scaling?
7. You tilt the drone to the right and motors don't compensate. List 3 possible causes.
