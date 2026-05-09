# 06 — PWM: Controlling Speed and Brightness

**PWM (Pulse Width Modulation)** is one of the most important concepts for drone building. It's how you control motor speed, LED brightness, servo position — essentially any "analog-like" control from a digital pin.

---

## What is PWM?

A GPIO pin can only be HIGH (3.3V) or LOW (0V). It can't output 1.65V. But what if you switch it on and off very fast?

```
100% duty cycle (always on):
‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾

50% duty cycle:
‾‾‾‾‾__‾‾‾‾‾__‾‾‾‾‾__‾‾‾‾‾__

25% duty cycle:
‾‾__‾‾__‾‾__‾‾__

0% duty cycle (always off):
____________________________
```

- **Duty cycle** = percentage of time the signal is HIGH
- **Period** = time for one full on+off cycle
- **Frequency** = cycles per second (Hz)

If you switch fast enough (>1kHz), an LED perceives it as a steady dimmer brightness. A motor averages it as a proportional voltage. An ESC (drone motor controller) interprets specific pulse widths as throttle commands.

---

## ESP32 PWM — LEDC

The ESP32 has a dedicated PWM peripheral called **LEDC** (originally for LED control, but used for anything).

Key parameters:
- **16 channels** (0–15) — each can control a different pin
- **Frequency:** 1Hz to 40MHz
- **Resolution:** 1–16 bits (8-bit = 0–255, 16-bit = 0–65535)

### Arduino-style (ESP32 Arduino library ≥2.0)

```cpp
// New simple API — analogWrite works like Arduino
analogWrite(pin, value);   // value 0–255
```

### Full LEDC API (more control)

```cpp
// Setup
ledcSetup(channel, frequency, resolution_bits);
ledcAttachPin(pin, channel);

// Control
ledcWrite(channel, duty);  // duty: 0 to (2^resolution - 1)
```

---

## Example 1 — LED Fade

```cpp
#define LED_PIN   4
#define PWM_CH    0
#define PWM_FREQ  5000   // 5 kHz
#define PWM_RES   8      // 8-bit → duty 0–255

void setup() {
    ledcSetup(PWM_CH, PWM_FREQ, PWM_RES);
    ledcAttachPin(LED_PIN, PWM_CH);
}

void loop() {
    // Fade in
    for (int duty = 0; duty <= 255; duty++) {
        ledcWrite(PWM_CH, duty);
        delay(10);
    }
    // Fade out
    for (int duty = 255; duty >= 0; duty--) {
        ledcWrite(PWM_CH, duty);
        delay(10);
    }
}
```

The LED smoothly fades in and out. At 50% duty (127), it appears at roughly half brightness.

---

## Example 2 — Potentiometer Controls LED Brightness

```cpp
#define LED_PIN  4
#define POT_PIN 34
#define PWM_CH   0

void setup() {
    ledcSetup(PWM_CH, 5000, 8);    // 5kHz, 8-bit
    ledcAttachPin(LED_PIN, PWM_CH);
}

void loop() {
    int potVal = analogRead(POT_PIN);      // 0–4095
    int brightness = map(potVal, 0, 4095, 0, 255);  // scale to 0–255
    ledcWrite(PWM_CH, brightness);
    delay(10);
}
```

`map(value, fromLow, fromHigh, toLow, toHigh)` — scales a number from one range to another.

---

## PWM for Servo Motors

A servo motor is controlled by a specific PWM signal:
- **Frequency:** 50Hz (one pulse every 20ms)
- **Pulse width:** 1ms = full left (0°), 1.5ms = center (90°), 2ms = full right (180°)

```
50Hz period = 20ms

Position 0°  (1ms pulse):   ‾|_________________| 
Position 90° (1.5ms pulse): ‾‾|________________|
Position 180° (2ms pulse):  ‾‾‾|_______________|
```

### Using the Servo library (easiest):

```cpp
#include <ESP32Servo.h>   // Install via Library Manager: "ESP32Servo"

Servo myServo;

void setup() {
    myServo.attach(13);   // Servo signal wire to GPIO13
}

void loop() {
    myServo.write(0);     // 0 degrees
    delay(1000);
    myServo.write(90);    // 90 degrees (center)
    delay(1000);
    myServo.write(180);   // 180 degrees
    delay(1000);
}
```

### Servo wiring:
| Servo wire color | Connect to |
|-----------------|-----------|
| Brown / Black | GND |
| Red | 5V (or 3.3V for micro servos) |
| Orange / Yellow | GPIO13 (signal) |

> Servo motors draw more current than a GPIO pin can supply. Power them from the board's VIN/5V pin or an external 5V supply — not from 3V3.

---

## PWM for ESCs (Drone Motors) — Preview

**ESC (Electronic Speed Controller)** converts a PWM signal into power for a brushless motor. The ESC accepts the same type of signal as a servo:

- **50Hz frequency**
- **1000µs pulse (1ms)** = motor stopped (minimum throttle)
- **2000µs pulse (2ms)** = full speed (maximum throttle)

You'll use this in Module 07. For now, understand that motor control = servo-like PWM.

```
Arming sequence (mandatory for most ESCs):
1. Send 1000µs pulse (minimum) for 2+ seconds → ESC arms, beeps
2. Then increase pulse width to increase throttle
```

---

## Practice

1. Wire an LED to GPIO4 and make it fade in and out smoothly using LEDC PWM.

2. Connect a potentiometer (ADC on GPIO34) and use it to control LED brightness in real-time.

3. If you have a servo, connect it to GPIO13, power it from 5V, and sweep it from 0° to 180° and back.

4. What duty cycle value (0–255, 8-bit) corresponds to 75% brightness? What about 33%?

**Answers to #4:** 75% = 0.75 × 255 ≈ 191. 33% = 0.33 × 255 ≈ 84.

---

**Next:** [07 — Serial Communication (UART)](07_uart.md)
