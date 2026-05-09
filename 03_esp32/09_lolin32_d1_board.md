# 09 — ESP32 D1 LOLIN32 (CP2102 Development Board)

## What Is It?

The **LOLIN32 D1** (also called Wemos D1 Mini32 or D1 ESP32) is a compact ESP32 development board made by LOLIN/Wemos. Key differences from the 38-pin NodeMCU board:

- **Smaller footprint** — fits a standard breadboard without bridging
- **CP2102 USB-UART chip** — often more reliable than CH340G on some computers
- **LiPo battery connector** — JST-PH 2-pin, can run directly from a LiPo with onboard charging
- **32-pin layout** — fewer pins exposed than the 38-pin board, but all essential ones are there
- **LED on GPIO 5** (vs GPIO 2 on standard boards)

Same ESP32 chip inside: dual-core 240 MHz, 520 KB SRAM, 4 MB flash, WiFi + BLE.

---

## Why You Have Two ESP32 Boards

You now have:
- **38-pin NodeMCU** — full pin access, ideal for the drone (fits all sensors)
- **LOLIN32 D1** — compact, has LiPo charging, ideal for the hand-held transmitter

Recommended split:
```
DRONE SIDE:    38-pin NodeMCU  + MPU-6050 + BMP280 + NRF24L01 adapter
TRANSMITTER:   LOLIN32 D1      + 2× PS2 joystick + NRF24L01 adapter + OLED
```

The LOLIN32 on the transmitter can run from a small 500 mAh LiPo via the battery connector.

---

## LOLIN32 D1 Pinout

```
                 [ USB-C / Micro-USB ]
              ┌──────────────────────┐
         RST  │ RST             GND  │ GND
      GPIO36  │ SVP             GPIO23│ MOSI
      GPIO26  │ 26              GPIO22│ SCL
      GPIO18  │ SCK             GPIO1 │ TX
      GPIO19  │ MISO            GPIO3 │ RX
      GPIO5   │ 5               GPIO21│ SDA
      GPIO17  │ 17              GPIO0 │ 0
      GPIO16  │ 16              GND   │ GND
       GPIO4  │ 4               GPIO2 │ 2 (LED)
       GPIO0  │ 0               GPIO15│ 15
        GND   │ GND             GPIO8 │ (flash — avoid)
        VCC   │ 3.3V            GPIO7 │ (flash — avoid)
              └──────────────────────┘
              [ BAT ] — JST-PH LiPo connector
```

---

## CP2102 vs CH340G Driver

The LOLIN32 uses a **Silicon Labs CP2102** USB-to-serial chip.

| Chip | Common on | Driver needed |
|------|-----------|--------------|
| CP2102 | LOLIN32, some NodeMCU | Usually auto-installs on Win10/11/Mac |
| CH340G | Cheap NodeMCU clones | May need manual install |

If the LOLIN32 doesn't appear as a COM port:
1. Windows: install **CP210x Universal Windows Driver** from Silicon Labs website
2. Mac: install **CP210x VCP macOS driver**
3. Linux: usually works out of the box (`/dev/ttyUSB0`)

---

## LiPo Battery Connector (Key Feature)

The LOLIN32 has a JST-PH 2-pin connector for a single-cell (1S) LiPo battery with **onboard TP4054 charger**.

- **Charging:** plug USB in while battery is connected — it charges at 500 mA
- **Runtime:** a 500 mAh LiPo at ~120 mA (ESP32 + NRF24L01 + OLED) = ~4 hours
- **Battery polarity:** double-check before connecting — reversed polarity destroys the board instantly

```
JST-PH connector polarity (standard):
  Pin 1 (nearest board edge): POSITIVE (+)
  Pin 2: NEGATIVE (-)
```

> Measure your battery cable with a multimeter before first connection. JST-PH cables are not always consistent.

---

## Arduino IDE Board Selection

When uploading to the LOLIN32 in Arduino IDE:

1. **Tools → Board → ESP32 Arduino → WEMOS LOLIN32**
2. **Tools → Port → COMx** (Windows) or `/dev/ttyUSB0` (Linux)
3. **Upload Speed → 921600**

If "WEMOS LOLIN32" is not listed, use **"ESP32 Dev Module"** — it works identically.

---

## Code Example 1 — Blink (confirms board works)

```cpp
// LOLIN32: onboard LED is on GPIO 5 (NOT GPIO 2 like standard boards)
#define LED_PIN 5

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
```

---

## Code Example 2 — Battery Voltage Monitor

The LOLIN32 has a voltage divider on **GPIO 35** (or GPIO 34 on some versions) connected to the battery pin. Reading it gives approximate battery voltage.

```cpp
#define BAT_PIN 35           // battery ADC pin
#define R1 100.0             // voltage divider top resistor (kΩ) — check your board
#define R2 100.0             // voltage divider bottom resistor (kΩ)

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}

float readBatteryVoltage() {
  int raw = analogRead(BAT_PIN);
  float vADC = raw * (3.3 / 4095.0);           // ADC voltage
  float vBat = vADC * ((R1 + R2) / R2);        // reverse voltage divider
  return vBat;
}

void loop() {
  float v = readBatteryVoltage();
  Serial.print("Battery: ");
  Serial.print(v, 2);
  Serial.println(" V");

  // 1S LiPo: 4.2V = full, 3.7V = nominal, 3.5V = low, 3.0V = cutoff
  if (v < 3.5) Serial.println("WARNING: Low battery!");

  delay(2000);
}
```

> The exact voltage divider resistor values vary by LOLIN32 revision. Check your board schematic or measure both resistors. Common is 100kΩ/100kΩ (divides by 2) or 100kΩ/47kΩ.

---

## Code Example 3 — LOLIN32 as Drone Transmitter

Combines PS2 joysticks + NRF24L01 adapter + OLED + battery monitor in one compact transmitter.

```cpp
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Joystick pins
#define L_X  34
#define L_Y  36   // SVP on LOLIN32
#define R_X  32
#define R_Y  33
#define L_SW 25
#define R_SW 26
#define BAT_PIN 35

RF24 radio(4, 5);
const byte address[6] = "00001";

Adafruit_SSD1306 display(128, 64, &Wire, -1);

struct DroneCommand {
  int throttle, yaw, pitch, roll;
  bool arm;
};

bool armed = false;
bool lastArmBtn = false;

int centerLX, centerLY, centerRX, centerRY;

int readCenter(int pin) {
  long s = 0;
  for (int i = 0; i < 50; i++) { s += analogRead(pin); delay(2); }
  return s / 50;
}

int stick(int raw, int center) {
  int v = raw - center;
  if (abs(v) < 80) return 0;
  return constrain(map(abs(v), 80, 2000, 1, 500) * (v > 0 ? 1 : -1), -500, 500);
}

float readBattery() {
  return analogRead(BAT_PIN) * (3.3 / 4095.0) * 2.0;
}

unsigned long lastDisplay = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(L_SW, INPUT_PULLUP);
  pinMode(R_SW, INPUT_PULLUP);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 20);
  display.println("Calibrating...");
  display.display();

  delay(1500);
  centerLX = readCenter(L_X); centerLY = readCenter(L_Y);
  centerRX = readCenter(R_X); centerRY = readCenter(R_Y);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();
}

void loop() {
  DroneCommand cmd;
  cmd.throttle = constrain(map(analogRead(L_Y), 100, 3900, 0, 1000), 0, 1000);
  cmd.yaw      = stick(analogRead(L_X), centerLX);
  cmd.pitch    = stick(analogRead(R_Y), centerRY);
  cmd.roll     = stick(analogRead(R_X), centerRX);

  bool armBtn = !digitalRead(L_SW);
  if (armBtn && !lastArmBtn) armed = !armed;
  lastArmBtn = armBtn;
  cmd.arm = armed;

  radio.write(&cmd, sizeof(cmd));

  // Update OLED at 10 Hz
  if (millis() - lastDisplay >= 100) {
    lastDisplay = millis();
    float vbat = readBattery();

    display.clearDisplay();
    display.setTextSize(1);

    // Title
    display.setCursor(25, 0);
    display.print(armed ? "** ARMED **" : "  DISARMED ");

    // Channels
    display.setCursor(0, 14);
    display.print("THR:"); display.print(cmd.throttle);
    display.setCursor(70, 14);
    display.print("YAW:"); display.print(cmd.yaw);

    display.setCursor(0, 28);
    display.print("PIT:"); display.print(cmd.pitch);
    display.setCursor(70, 28);
    display.print("ROL:"); display.print(cmd.roll);

    // Battery
    display.setCursor(0, 50);
    display.print("BAT:");
    display.print(vbat, 1);
    display.print("V");
    if (vbat < 3.6) {
      display.setCursor(60, 50);
      display.print("LOW BATT!");
    }

    display.display();
  }

  delay(20);   // 50 Hz radio loop
}
```

---

## Pin Differences vs 38-pin NodeMCU

| Function | 38-pin NodeMCU | LOLIN32 D1 |
|----------|---------------|------------|
| Onboard LED | GPIO 2 | GPIO 5 |
| SDA (I2C) | GPIO 21 | GPIO 21 (same) |
| SCL (I2C) | GPIO 22 | GPIO 22 (same) |
| SPI SCK | GPIO 18 | GPIO 18 (same) |
| SPI MOSI | GPIO 23 | GPIO 23 (same) |
| SPI MISO | GPIO 19 | GPIO 19 (same) |
| Input-only ADC | 34, 35, 36, 39 | 34, 35, 36 (SVP), 39 |
| LiPo battery | Not built-in | JST-PH on board |
| Battery ADC | Not present | GPIO 35 |

SPI and I2C are identical — sensor libraries work unchanged on both boards.

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| Board not detected by PC | CP2102 driver missing | Download from Silicon Labs |
| Upload fails | Wrong board selected | Set to "WEMOS LOLIN32" or "ESP32 Dev Module" |
| Battery reading is 0V | Wrong GPIO or R divider values | Check board schematic, measure resistors |
| LiPo gets warm during charge | Normal for TP4054 at 500mA | Acceptable — stop if hot |
| Blink sketch doesn't work on LED | Using GPIO 2 instead of GPIO 5 | Change LED_PIN to 5 |
| Serial Monitor shows garbage | Wrong baud rate | Set Serial Monitor to 115200 |

---

## What to Try

1. Upload the Blink sketch → confirm GPIO 5 LED blinks (not GPIO 2)
2. Run battery voltage example → connect USB only → should read ~5V (USB powers board directly)
3. Connect a 3.7V LiPo → read ~3.7V on battery monitor
4. Run the full transmitter example (Example 3) → confirm OLED shows channels + battery
5. Walk away from the drone receiver → confirm radio range at PA_HIGH

---

## Next Steps

- **09_ps2_joystick_module.md** — joystick wiring for this board
- **08_oled_display.md** — OLED display works identically on LOLIN32
- **07_drone_electronics** — assign LOLIN32 as transmitter, 38-pin as drone flight controller
