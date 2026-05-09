# 09 — PS2 Joystick Module (Breakout Sensor)

## What Is It?

The **PS2 Joystick Module** is a single thumbstick breakout — the same style of analog stick found in PlayStation 2 controllers. It has:

- **2-axis analog output** (X and Y) via potentiometers
- **1 push button** (press the stick down)
- 5 pins on a single-row header

Unlike the Joystick Shield (which has two sticks + multiple buttons on a PCB that mounts to an Arduino), this is just one stick as a standalone module — wire it wherever you need it.

**For this project:** use two PS2 modules to build a custom transmitter (left stick + right stick), or use one as a gimbal controller, throttle, or menu navigator for the OLED display.

---

## PS2 Module vs Joystick Shield

| Feature | PS2 Joystick Module | Joystick Shield |
|---------|---------------------|-----------------|
| Sticks | 1 | 2 |
| Extra buttons | 0 (just stick-press) | 4–6 |
| Form factor | Standalone, 5-pin header | Arduino-shaped PCB |
| Wiring | 5 wires | Mount + wire |
| Best for | Custom builds, single axis | Full RC transmitter layout |

---

## Hardware Overview

```
PS2 Joystick Module Pins:

  GND  ── Ground
  VCC  ── 3.3V or 5V
  VRx  ── X-axis analog output (left/right)
  VRy  ── Y-axis analog output (up/down)
  SW   ── Switch (stick press, active LOW)
```

The potentiometers output 0–VCC depending on stick position.
Center position = ~VCC/2.

---

## Wiring to ESP32

| PS2 Pin | ESP32 Pin | Notes |
|---------|-----------|-------|
| GND | GND | |
| VCC | 3.3V | Use 3.3V so ADC range matches |
| VRx | GPIO 34 | ADC1 — safe with WiFi |
| VRy | GPIO 35 | ADC1 — safe with WiFi |
| SW | GPIO 25 | Digital input, needs pullup |

> Use 3.3V for VCC. If you use 5V, the pot outputs 0–5V but ESP32 ADC only reads 0–3.3V — values above 3.3V are clipped and the top ~30% of stick travel is invisible.

For two sticks (full drone transmitter):

| Module | VRx | VRy | SW |
|--------|-----|-----|----|
| Left stick | GPIO 34 | GPIO 35 | GPIO 25 |
| Right stick | GPIO 32 | GPIO 33 | GPIO 26 |

---

## Code Example 1 — Single Stick Raw Reading

```cpp
#define PIN_X  34
#define PIN_Y  35
#define PIN_SW 25

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);         // 12-bit: 0–4095
  analogSetAttenuation(ADC_11db);   // full 0–3.3V range
  pinMode(PIN_SW, INPUT_PULLUP);    // switch is active LOW
}

void loop() {
  int x  = analogRead(PIN_X);
  int y  = analogRead(PIN_Y);
  bool pressed = !digitalRead(PIN_SW);

  Serial.print("X: "); Serial.print(x);
  Serial.print("  Y: "); Serial.print(y);
  Serial.print("  Button: "); Serial.println(pressed ? "PRESSED" : "---");

  delay(50);
}
```

Neutral (center): X ≈ 2048, Y ≈ 2048
Full left: X ≈ 0, Full right: X ≈ 4095
Full down: Y ≈ 0, Full up: Y ≈ 4095

---

## Code Example 2 — Calibrated Single Stick

```cpp
#define PIN_X  34
#define PIN_Y  35
#define PIN_SW 25

int centerX, centerY;
const int DEADZONE = 80;   // ignore small drift around center

int readCenter(int pin) {
  long s = 0;
  for (int i = 0; i < 50; i++) { s += analogRead(pin); delay(2); }
  return s / 50;
}

int applyDeadzone(int value, int deadzone) {
  if (abs(value) <= deadzone) return 0;
  // Scale so output reaches ±500 outside the deadzone
  if (value > 0) return map(value, deadzone, 2047, 1, 500);
  else           return map(value, -2047, -deadzone, -500, -1);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(PIN_SW, INPUT_PULLUP);

  Serial.println("Keep stick centered — calibrating...");
  delay(1000);
  centerX = readCenter(PIN_X);
  centerY = readCenter(PIN_Y);
  Serial.print("Center: X="); Serial.print(centerX);
  Serial.print(" Y="); Serial.println(centerY);
}

void loop() {
  int rawX = analogRead(PIN_X) - centerX;   // offset from center
  int rawY = analogRead(PIN_Y) - centerY;

  int mappedX = applyDeadzone(rawX, DEADZONE);
  int mappedY = applyDeadzone(rawY, DEADZONE);

  bool pressed = !digitalRead(PIN_SW);

  Serial.print("X: "); Serial.print(mappedX);   // -500 to +500
  Serial.print("  Y: "); Serial.print(mappedY); // -500 to +500
  if (pressed) Serial.print("  [CLICK]");
  Serial.println();

  delay(20);
}
```

---

## Code Example 3 — Two Sticks as Full Drone Transmitter

```cpp
#include <SPI.h>
#include <RF24.h>

// Left stick (throttle + yaw)
#define L_X  34
#define L_Y  35
#define L_SW 25

// Right stick (pitch + roll)
#define R_X  32
#define R_Y  33
#define R_SW 26

RF24 radio(4, 5);   // CE, CSN
const byte address[6] = "00001";

struct DroneCommand {
  int throttle;   // 0 to 1000
  int yaw;        // -500 to +500
  int pitch;      // -500 to +500
  int roll;       // -500 to +500
  bool arm;       // left stick click = arm/disarm toggle
};

int centerLX, centerLY, centerRX, centerRY;
bool armed = false;
bool lastArmBtn = false;

int readCenter(int pin) {
  long s = 0;
  for (int i = 0; i < 50; i++) { s += analogRead(pin); delay(2); }
  return s / 50;
}

int stickToSymmetric(int raw, int center) {
  int val = raw - center;
  if (abs(val) < 80) return 0;   // deadzone
  return constrain(map(abs(val), 80, 2047, 1, 500) * (val > 0 ? 1 : -1), -500, 500);
}

int stickToThrottle(int raw) {
  // Left Y: full down = 0, full up = 1000, no centering spring logic
  return constrain(map(raw, 100, 3900, 0, 1000), 0, 1000);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(L_SW, INPUT_PULLUP);
  pinMode(R_SW, INPUT_PULLUP);

  Serial.println("Calibrating — keep both sticks centered...");
  delay(1500);
  centerLX = readCenter(L_X);
  centerLY = readCenter(L_Y);
  centerRX = readCenter(R_X);
  centerRY = readCenter(R_Y);
  Serial.println("Calibration done.");

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();
  Serial.println("Radio ready. Transmitting at 50Hz.");
}

void loop() {
  DroneCommand cmd;

  cmd.throttle = stickToThrottle(analogRead(L_Y));
  cmd.yaw      = stickToSymmetric(analogRead(L_X), centerLX);
  cmd.pitch    = stickToSymmetric(analogRead(R_Y), centerRY);
  cmd.roll     = stickToSymmetric(analogRead(R_X), centerRX);

  // Arm toggle on left stick click
  bool armBtn = !digitalRead(L_SW);
  if (armBtn && !lastArmBtn) armed = !armed;
  lastArmBtn = armBtn;
  cmd.arm = armed;

  radio.write(&cmd, sizeof(cmd));

  Serial.print("THR:"); Serial.print(cmd.throttle);
  Serial.print(" YAW:"); Serial.print(cmd.yaw);
  Serial.print(" PIT:"); Serial.print(cmd.pitch);
  Serial.print(" ROL:"); Serial.print(cmd.roll);
  Serial.print(" ARM:"); Serial.println(cmd.arm ? "YES" : "NO");

  delay(20);   // 50 Hz
}
```

---

## Physical Differences from the Joystick Shield

The PS2 module's stick has springs on both X and Y axes — both axes self-center when released. This differs from a real RC transmitter where the left Y axis (throttle) has **no spring** and stays wherever you leave it.

Options:
1. **Use as-is:** throttle self-centers to mid (good for learning, drone won't full-throttle if you let go)
2. **Remove the Y-axis spring:** physically remove the spring from the left stick for authentic RC feel
3. **Software lock:** in your code, only allow throttle to change when actively moved; hold last value otherwise

For beginners, option 1 (self-centering throttle) is safest.

---

## Noise and Averaging

The PS2 module uses a bare potentiometer — no filtering. At rest, the ADC reading fluctuates ±5–20 counts.

```cpp
// Average 4 readings for stability
int stableRead(int pin) {
  int s = 0;
  for (int i = 0; i < 4; i++) s += analogRead(pin);
  return s / 4;
}
```

Combined with the deadzone, this makes stick output very clean.

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| One axis always reads 0 or 4095 | VCC too low or pin wrong | Confirm 3.3V VCC, check pin number |
| Button never registers | No `INPUT_PULLUP` | Add `pinMode(PIN_SW, INPUT_PULLUP)` |
| Drone drifts without touching stick | No deadzone | Add deadzone of 50–100 counts |
| Different range than expected | Used 5V for VCC | Switch to 3.3V to match ADC range |
| Center value is not ~2048 | Stick worn or pot off-center | Run `readCenter()` calibration at startup |

---

## What to Try

1. Run Example 1 → move stick through all directions, note the 0–4095 range
2. Run Example 2 → verify output is ±500 and returns to 0 at center
3. Wire two modules → run Example 3 → see all four RC channels in Serial Monitor
4. Connect an NRF24L01 adapter → run the full transmitter and watch the receiver side

---

## Next Steps

- **04_communication_protocols/05_nrf24l01_adapter_board.md** — clean power for the radio module
- **08_oled_display.md** — show the stick values live on the OLED for a proper transmitter HUD
- **07_drone_electronics** — wire the complete ground station: 2× PS2 + NRF24L01 + OLED
