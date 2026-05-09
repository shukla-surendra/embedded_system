# Module 05 — Sensors & Modules

The hardware that makes a drone "aware" of its environment and position.

## Topics

| File | What You'll Learn | Status |
|------|-------------------|--------|
| [01_mpu6050_imu.md](01_mpu6050_imu.md) | Accelerometer + gyroscope over I2C, angles, complementary filter | Done |
| [02_bmp280_barometer.md](02_bmp280_barometer.md) | Pressure, altitude, AGL reference, sensor fusion with MPU-6050 | Done |
| [03_joystick_shield.md](03_joystick_shield.md) | Analog sticks, ADC mapping, dead zone, Mode 2 RC layout, NRF24L01 transmitter | Done |
| [04_hcsr04_ultrasonic.md](04_hcsr04_ultrasonic.md) | Distance sensing, voltage divider, landing state machine, non-blocking ISR | Done |
| [05_lsm303agr_accel_mag.md](05_lsm303agr_accel_mag.md) | Combo accel + magnetometer, compass heading, tilt compensation, calibration | Done |
| [06_hmc5883l_compass.md](06_hmc5883l_compass.md) | Standalone compass, HMC vs QMC clone, yaw fusion with MPU-6050, declination | Done |
| [07_adxl345_accelerometer.md](07_adxl345_accelerometer.md) | High-res accel, tap/freefall hardware interrupts, landing detect, low power | Done |
| [08_oled_display.md](08_oled_display.md) | SSD1306 128×64, text/shapes/progress bar, live sensor dashboard, rate limiting | Done |
| [09_ps2_joystick_module.md](09_ps2_joystick_module.md) | Single thumbstick, calibration, deadzone, 2-stick NRF24L01 transmitter | Done |
| 10 — GPS (Neo-6M/Neo-8M) | Position, speed, heading via UART + TinyGPS++ | Coming soon |

## Key Sensor: MPU6050 IMU

The most important sensor for a drone — measures:
- **Accelerometer:** linear acceleration on X, Y, Z axes
- **Gyroscope:** angular velocity (rotation rate) on X, Y, Z axes

From these two, you can estimate the drone's pitch, roll, and yaw — the three angles that define its orientation. The flight controller's PID algorithm uses these to keep the drone stable.
