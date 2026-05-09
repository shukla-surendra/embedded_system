# 03 — Joystick Shield Module (Robotics Control)

## What Is It?

A **Joystick Shield** combines two analog joystick thumbsticks and push-buttons onto a single PCB, designed to mount directly onto an Arduino Uno or be wired to an ESP32.

Each joystick axis is a **potentiometer** — a variable resistor that outputs a voltage proportional to the stick position. The ESP32 reads this voltage via its ADC (Analog-to-Digital Converter).

**For a drone transmitter:** Left stick = throttle + yaw, Right stick = pitch + roll. This is the standard "Mode 2" RC layout used by most drones.

---

## Hardware Overview

A typical Joystick Shield has:

| Component    | Count | Output type    |
|--------------|-------|----------------|
| Analog stick | 2     | 2× analog (X,Y per stick) |
| Stick press  | 2     | Digital (push down on stick) |
| Extra buttons| 4–6   | Digital                       |

Each stick gives you 2 analog channels (X and Y axis) = **4 analog channels total** for a standard RC transmitter (throttle, yaw, pitch, roll).

```
LEFT STICK:             RIGHT STICK:
  Y-axis = Throttle       Y-axis = Pitch (forward/back)
  X-axis = Yaw (rotate)   X-axis = Roll (left/right)
```

---

## Pin Connections to ESP32

The shield has labeled pins — wire them to ESP32 as follows:

| Shield Pin | Signal              | ESP32 Pin | Notes                         |
|------------|---------------------|-----------|-------------------------------|
| VCC        | Power               | 3.3V      | Pots work fine at 3.3V        |
| GND        | Ground              | GND       |                               |
| LX         | Left stick X (Yaw)  | GPIO 34   | ADC input, no internal pullup |
| LY         | Left stick Y (Thr)  | GPIO 35   | ADC input                     |
| RX         | Right stick X (Roll)| GPIO 32   | ADC input                     |
| RY         | Right stick Y (Pitch)| GPIO 33  | ADC input                     |
| SW_L       | Left stick press    | GPIO 25   | Digital, needs pullup         |
| SW_R       | Right stick press   | GPIO 26   | Digital, needs pullup         |
| BTN_A      | Button A            | GPIO 27   | Digital, needs pullup         |
| BTN_B      | Button B            | GPIO 14   | Digital, needs pullup         |

> **ESP32 ADC pins to use:** 32–39 are ADC1 (reliable). Avoid GPIO 36/39 if WiFi is active — they share ADC2 which conflicts with WiFi on ESP32.

---

## Understanding Analog Joystick Output

A joystick potentiometer outputs 0–3.3V depending on position:
- **Center:** ~1.65V → ADC reads ~2048 (12-bit) or ~512 (10-bit)
- **Full up/right:** ~3.3V → ADC reads ~4095
- **Full down/left:** ~0V → ADC reads ~0

The exact center value varies per unit — **always read and calibrate the center at startup**.

```
Physical position:   Full Down ←──── Center ────→ Full Up
Voltage:                0V           1.65V           3.3V
ADC value (12-bit):     0            2048            4095
```

---

## Code Example 1 — Raw Joystick Reading

```cpp
#define PIN_LX  34   // Left X  (Yaw)
#define PIN_LY  35   // Left Y  (Throttle)
#define PIN_RX  32   // Right X (Roll)
#define PIN_RY  33   // Right Y (Pitch)

#define PIN_SW_L 25
#define PIN_SW_R 26

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);         // ESP32 default: 12-bit (0–4095)
  analogSetAttenuation(ADC_11db);   // full range: 0–3.3V

  pinMode(PIN_SW_L, INPUT_PULLUP);
  pinMode(PIN_SW_R, INPUT_PULLUP);
}

void loop() {
  int lx = analogRead(PIN_LX);
  int ly = analogRead(PIN_LY);
  int rx = analogRead(PIN_RX);
  int ry = analogRead(PIN_RY);

  bool sw_l = !digitalRead(PIN_SW_L);   // active LOW (pullup)
  bool sw_r = !digitalRead(PIN_SW_R);

  Serial.print("LX: "); Serial.print(lx);
  Serial.print("  LY: "); Serial.print(ly);
  Serial.print("  RX: "); Serial.print(rx);
  Serial.print("  RY: "); Serial.print(ry);
  Serial.print("  SW_L: "); Serial.print(sw_l);
  Serial.print("  SW_R: "); Serial.println(sw_r);

  delay(50);  // 20 Hz read rate
}
```

Move each stick — watch the corresponding value change in Serial Monitor.

---

## Code Example 2 — Calibrated + Mapped Output

Maps raw ADC values to useful ranges for drone control:
- Throttle: 0–1000
- Roll / Pitch / Yaw: -500 to +500 (centered at 0)

```cpp
#define PIN_LX  34
#define PIN_LY  35
#define PIN_RX  32
#define PIN_RY  33

// Calibration values — read from Serial Monitor with sticks at center and extremes
// Run the raw example first to get these numbers for your specific module
struct JoyCalib {
  int minVal, center, maxVal;
};

// Defaults — adjust after running Example 1
JoyCalib calibLX = {100, 2020, 3900};
JoyCalib calibLY = {100, 2020, 3900};
JoyCalib calibRX = {100, 2020, 3900};
JoyCalib calibRY = {100, 2020, 3900};

int mapAxis(int raw, JoyCalib c, int outMin, int outMax) {
  raw = constrain(raw, c.minVal, c.maxVal);
  if (raw < c.center) {
    return map(raw, c.minVal, c.center, outMin, (outMin + outMax) / 2);
  } else {
    return map(raw, c.center, c.maxVal, (outMin + outMax) / 2, outMax);
  }
}

struct DroneCommand {
  int throttle;  // 0–1000
  int roll;      // -500 to +500
  int pitch;     // -500 to +500
  int yaw;       // -500 to +500
};

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  DroneCommand cmd;

  // Left stick: LY up = more throttle, LX right = yaw right
  cmd.throttle = mapAxis(analogRead(PIN_LY), calibLY, 0, 1000);
  cmd.yaw      = mapAxis(analogRead(PIN_LX), calibLX, -500, 500);

  // Right stick: RY up = pitch forward, RX right = roll right
  cmd.pitch    = mapAxis(analogRead(PIN_RY), calibRY, -500, 500);
  cmd.roll     = mapAxis(analogRead(PIN_RX), calibRX, -500, 500);

  // Dead zone — ignore tiny stick drift around center
  if (abs(cmd.yaw)   < 20) cmd.yaw   = 0;
  if (abs(cmd.pitch) < 20) cmd.pitch = 0;
  if (abs(cmd.roll)  < 20) cmd.roll  = 0;

  Serial.print("THR: "); Serial.print(cmd.throttle);
  Serial.print("  ROL: "); Serial.print(cmd.roll);
  Serial.print("  PIT: "); Serial.print(cmd.pitch);
  Serial.print("  YAW: "); Serial.println(cmd.yaw);

  delay(20);  // 50 Hz
}
```

---

## Code Example 3 — Full Transmitter (Joystick + NRF24L01)

This combines the joystick with the NRF24L01 to make a working drone transmitter.

```cpp
#include <SPI.h>
#include <RF24.h>

#define PIN_LX  34
#define PIN_LY  35
#define PIN_RX  32
#define PIN_RY  33
#define PIN_SW_L 25

RF24 radio(4, 5);   // CE=GPIO4, CSN=GPIO5

const byte address[6] = "00001";

struct DroneCommand {
  int throttle;
  int roll;
  int pitch;
  int yaw;
  bool armButton;    // left stick press = arm/disarm
};

int readCenter(int pin) {
  // Average 50 readings at startup for center calibration
  long sum = 0;
  for (int i = 0; i < 50; i++) { sum += analogRead(pin); delay(2); }
  return sum / 50;
}

int centerLX, centerLY, centerRX, centerRY;

int applyDeadzone(int val, int deadzone) {
  if (abs(val) < deadzone) return 0;
  return val;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(PIN_SW_L, INPUT_PULLUP);

  // Auto-calibrate centers at power-on (sticks must be centered)
  Serial.println("Calibrating joystick centers — keep sticks centered...");
  delay(1000);
  centerLX = readCenter(PIN_LX);
  centerLY = readCenter(PIN_LY);
  centerRX = readCenter(PIN_RX);
  centerRY = readCenter(PIN_RY);
  Serial.print("Centers: LX="); Serial.print(centerLX);
  Serial.print(" LY="); Serial.print(centerLY);
  Serial.print(" RX="); Serial.print(centerRX);
  Serial.print(" RY="); Serial.println(centerRY);

  if (!radio.begin()) {
    Serial.println("NRF24L01 not found!");
    while (true);
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();

  Serial.println("Transmitter ready!");
}

void loop() {
  DroneCommand cmd;

  // Throttle: 0–1000, no centering (left stick Y, up = more throttle)
  int rawLY = analogRead(PIN_LY);
  cmd.throttle = map(constrain(rawLY, 100, 3900), 100, 3900, 0, 1000);

  // Yaw: center = 0, ±500
  int rawLX = analogRead(PIN_LX);
  cmd.yaw = map(constrain(rawLX, 100, 3900), 100, 3900, -500, 500);
  cmd.yaw = applyDeadzone(cmd.yaw, 30);

  // Pitch: center = 0, ±500 (right stick Y)
  int rawRY = analogRead(PIN_RY);
  cmd.pitch = map(constrain(rawRY, 100, 3900), 100, 3900, -500, 500);
  cmd.pitch = applyDeadzone(cmd.pitch, 30);

  // Roll: center = 0, ±500 (right stick X)
  int rawRX = analogRead(PIN_RX);
  cmd.roll = map(constrain(rawRX, 100, 3900), 100, 3900, -500, 500);
  cmd.roll = applyDeadzone(cmd.roll, 30);

  cmd.armButton = !digitalRead(PIN_SW_L);

  bool ok = radio.write(&cmd, sizeof(cmd));
  if (!ok) Serial.println("TX failed");

  Serial.print("THR:"); Serial.print(cmd.throttle);
  Serial.print(" ROL:"); Serial.print(cmd.roll);
  Serial.print(" PIT:"); Serial.print(cmd.pitch);
  Serial.print(" YAW:"); Serial.print(cmd.yaw);
  Serial.print(" ARM:"); Serial.println(cmd.armButton);

  delay(20);  // 50 Hz
}
```

---

## ESP32 ADC Quirks

The ESP32 ADC is not perfectly linear. Known issues:

- **Non-linearity near 0V and 3.3V:** the curve flattens at the extremes. Values near 0 and 4095 are not as accurate.
- **ADC2 conflicts with WiFi:** if you use WiFi + BLE, only use ADC1 pins (32–39).
- **Noise:** averaging multiple readings reduces noise significantly.

```cpp
// Better analog read — average 4 samples
int stableRead(int pin) {
  int sum = 0;
  for (int i = 0; i < 4; i++) sum += analogRead(pin);
  return sum / 4;
}
```

---

## Dead Zone Explained

Potentiometers drift — when a stick is physically centered, the ADC might read 2018–2055 instead of a steady 2048. Without a dead zone, the drone sees a constant tiny yaw/pitch/roll command even when you're not touching the stick.

```
Without deadzone:   center reads as  roll=+12 → drone slowly drifts
With deadzone=30:   any value in -30 to +30 → treated as 0 → drone holds still
```

Typical dead zone: **20–50 ADC counts** (0.5%–1.2% of full range).

---

## Mode 2 vs Mode 1 Layout

| Channel | Mode 2 (standard)       | Mode 1 (alternative)     |
|---------|------------------------|--------------------------|
| Left Y  | Throttle (no spring)   | Pitch                    |
| Left X  | Yaw                    | Yaw                      |
| Right Y | Pitch                  | Throttle (no spring)     |
| Right X | Roll                   | Roll                     |

Most drones and simulators default to Mode 2. The joystick shield has springs on both axes of both sticks — a real RC transmitter's throttle stick has no spring (it stays where you leave it). You can remove the spring from the left Y potentiometer if you want authentic feel.

---

## Common Problems

| Problem                           | Cause                            | Fix                                              |
|-----------------------------------|----------------------------------|--------------------------------------------------|
| All axes read same value          | VCC not connected                | Check 3.3V to shield VCC                         |
| Axis reads 0 or 4095 always       | Pin assignment wrong             | Confirm analog pin numbers in code               |
| Stick center drifts over time     | Temperature / no calibration     | Run center calibration at each startup           |
| Drone rolls without stick input   | No dead zone                     | Add dead zone of 20–50 counts                    |
| Buttons never register            | Missing INPUT_PULLUP             | Buttons are active-LOW — add `INPUT_PULLUP`      |
| Values noisy / jittery            | Floating ADC or no filtering     | Average 4–8 samples, use shorter wires           |

---

## What to Try

1. Run Example 1 → move each stick and confirm the correct axis changes
2. Note the min/center/max values of each axis → plug them into Example 2 calibration struct
3. Add dead zone and confirm stick returns cleanly to zero when released
4. Run Example 3 with the NRF24L01 wired → see live joystick values on the receiver

---

## Next Steps

- **04_communication_protocols/04_nrf24l01_wireless.md** — pair this transmitter with a receiver on the drone
- **07_drone_electronics** — combine joystick → NRF24L01 → flight controller → ESCs
