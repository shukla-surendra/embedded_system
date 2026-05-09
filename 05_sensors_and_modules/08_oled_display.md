# 08 — 0.96" OLED Display Module (I2C / SPI, Blue)

## What Is It?

A **0.96-inch OLED** (Organic Light Emitting Diode) display with **128×64 pixels**, driven by the **SSD1306** controller chip.

- Each pixel emits its own light — no backlight needed
- Pure black = pixel OFF (zero power), which saves battery
- Very readable in sunlight compared to LCD
- Blue pixel color (fixed — blue OLEDs cannot change colour)
- Communicates over **I2C (4-pin)** or **SPI (7-pin)** depending on version

**For this project:** the 4-pin version uses I2C. It shows live sensor data (altitude, heading, battery voltage) during flight tuning without needing a laptop connected.

---

## I2C vs SPI — Which Do You Have?

Count the pins on your module:

| Pins | Interface | Speed | Wires needed |
|------|-----------|-------|-------------|
| 4 (GND, VCC, SCL, SDA) | I2C | Slower, ~400 kHz | 2 signal |
| 7 (GND, VCC, D0, D1, RES, DC, CS) | SPI | Faster, ~8 MHz | 5 signal |

You have the **4-pin I2C version**. This tutorial covers I2C. The SPI wiring is different but the display library is the same.

---

## Hardware Overview

```
0.96" OLED I2C (4-pin):

  GND  ── Ground
  VCC  ── 3.3V or 5V (most modules accept both)
  SCL  ── I2C Clock
  SDA  ── I2C Data
```

I2C address: **0x3C** (most common) or **0x3D** (some modules, set by a solder bridge)

The SSD1306 shares the I2C bus — address 0x3C does not conflict with:
- MPU-6050 (0x68), BMP280 (0x76), ADXL345 (0x53), HMC5883L (0x1E)

---

## Wiring to ESP32 (I2C)

| OLED Pin | ESP32 Pin | Notes |
|----------|-----------|-------|
| GND | GND | |
| VCC | 3.3V or 5V | Either works |
| SCL | GPIO 22 | Shared I2C bus |
| SDA | GPIO 21 | Shared I2C bus |

Plugs directly into the same I2C bus as all other sensors.

---

## Installing the Library

In Arduino IDE → Manage Libraries, install **two** libraries:

1. Search `SSD1306` → Install **"Adafruit SSD1306"** by Adafruit
2. Also install: **"Adafruit GFX Library"** (dependency — prompted automatically)
3. Also install: **"Adafruit BusIO"** (dependency)

---

## Code Example 1 — Hello World

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1        // no reset pin on most modules
#define OLED_ADDRESS 0x3C    // try 0x3D if this fails

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found. Check wiring or try address 0x3D.");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);            // 1 = 6×8 pixels per character
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello, Drone!");
  display.display();                 // MUST call display() to push buffer to screen
}

void loop() {}
```

> **Important:** Every `display.print()` call writes to a RAM buffer. Nothing appears on screen until you call `display.display()`.

---

## Code Example 2 — Live Sensor Dashboard

Shows multiple values laid out like an instrument panel:

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu(Wire);
Adafruit_BMP280 bmp;

float groundAlt = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  mpu.begin();
  delay(500);
  mpu.calcOffsets();

  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
  delay(500);
  groundAlt = bmp.readAltitude(1013.25);
}

void loop() {
  mpu.update();
  float pitch   = mpu.getAngleX();
  float roll    = mpu.getAngleY();
  float altitude = bmp.readAltitude(1013.25) - groundAlt;
  float temp    = bmp.readTemperature();

  display.clearDisplay();

  // --- Title bar ---
  display.setTextSize(1);
  display.setCursor(30, 0);
  display.println("DRONE STATUS");
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);  // horizontal rule

  // --- Pitch & Roll ---
  display.setTextSize(1);
  display.setCursor(0, 13);
  display.print("Pitch:");
  display.setTextSize(2);
  display.setCursor(0, 22);
  display.print(pitch, 1); display.print((char)247);  // degree symbol

  display.setTextSize(1);
  display.setCursor(70, 13);
  display.print("Roll:");
  display.setTextSize(2);
  display.setCursor(70, 22);
  display.print(roll, 1); display.print((char)247);

  // --- Altitude ---
  display.setTextSize(1);
  display.setCursor(0, 44);
  display.print("Alt:");
  display.setTextSize(2);
  display.setCursor(0, 52);
  display.print(altitude, 1); display.print("m");

  // --- Temperature ---
  display.setTextSize(1);
  display.setCursor(75, 44);
  display.print("Temp:");
  display.setTextSize(1);
  display.setCursor(75, 53);
  display.print(temp, 1); display.print("C");

  display.display();   // push to screen
  delay(100);  // 10 Hz refresh
}
```

---

## Code Example 3 — Text Sizes and Graphics

Understanding what the display can draw:

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  // --- Page 1: Text sizes ---
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);  display.setTextSize(1); display.println("Size 1: 6x8px abc");
  display.setCursor(0, 12); display.setTextSize(2); display.println("Sz2: 12x");
  display.setCursor(0, 36); display.setTextSize(3); display.println("Sz3");
  display.display();
  delay(3000);

  // --- Page 2: Shapes ---
  display.clearDisplay();
  display.drawRect(0, 0, 40, 20, SSD1306_WHITE);        // outline rectangle
  display.fillRect(45, 0, 40, 20, SSD1306_WHITE);       // filled rectangle
  display.drawCircle(105, 10, 10, SSD1306_WHITE);       // circle outline
  display.fillCircle(105, 45, 10, SSD1306_WHITE);       // filled circle
  display.drawLine(0, 30, 127, 63, SSD1306_WHITE);      // diagonal line
  display.drawTriangle(0,63, 20,40, 40,63, SSD1306_WHITE); // triangle
  display.display();
  delay(3000);

  // --- Page 3: Progress bar (useful for battery level) ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Battery:");
  int percent = 73;
  display.drawRect(0, 15, 100, 16, SSD1306_WHITE);
  display.fillRect(2, 17, percent, 12, SSD1306_WHITE);  // fill proportional to %
  display.setCursor(105, 18);
  display.print(percent); display.print("%");
  display.display();
  delay(3000);
}
```

---

## Screen Coordinate System

```
(0,0) ─────────────────────────── (127,0)
  │                                   │
  │   128 pixels wide                 │
  │   64 pixels tall                  │
  │                                   │
(0,63) ─────────────────────────(127,63)
```

- `setCursor(x, y)` — top-left corner of the next text character
- Text size 1: each character is 6 wide × 8 tall pixels
- Text size 2: 12 × 16 pixels, etc.
- 128 ÷ 6 = **21 characters per row** at size 1
- 64 ÷ 8 = **8 rows** at size 1

---

## Display Update Rate

The SSD1306 over I2C at 400 kHz sends 128×64 = 8192 bytes per full refresh.
At 400 kHz that takes ~1.5 ms → theoretical max ~650 Hz.

In practice with the Adafruit library and other I2C devices sharing the bus: **10–20 Hz** is comfortable. Don't call `display.display()` inside a fast PID loop — update the display from a slower task or use a timer.

```cpp
// Update display every 100ms, run control loop every 10ms
unsigned long lastDisplay = 0;

void loop() {
  // Control loop runs every time (10ms)
  mpu.update();
  runPID();

  // Display updates at 10 Hz
  if (millis() - lastDisplay >= 100) {
    lastDisplay = millis();
    updateOLED();
  }
}
```

---

## Reducing Power Consumption

Each lit OLED pixel draws ~0.3 mW. A fully white screen draws ~30 mW.
A mostly dark screen with a few text labels draws ~5–10 mW.

```cpp
// Dim the display after 30 seconds of inactivity
display.ssd1306_command(SSD1306_SETCONTRAST);
display.ssd1306_command(0x10);   // 0x00 = dimmest, 0xFF = brightest

// Turn off completely (preserves RAM buffer — display() restores it)
display.ssd1306_command(SSD1306_DISPLAYOFF);

// Turn back on
display.ssd1306_command(SSD1306_DISPLAYON);
```

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| Nothing on screen | Missing `display.display()` call | Always call `display.display()` after drawing |
| "OLED not found" | Wrong I2C address | Try `0x3D` instead of `0x3C` |
| Screen is all white noise | Library / wiring issue | Check SDA/SCL not swapped |
| Text appears mirrored | display.setRotation() set wrong | Call `display.setRotation(0)` to reset |
| Screen flickers | Too many `clearDisplay()` calls | Only clear once per frame, then redraw |
| Old text stays on screen | Missing `clearDisplay()` | Call `display.clearDisplay()` at start of each frame |
| Works alone, fails with other I2C devices | I2C bus contention | Reduce I2C clock to 100 kHz: `Wire.begin(); Wire.setClock(100000);` |

---

## What to Try

1. Run Example 1 → confirm "Hello, Drone!" appears
2. Change text to `display.setTextSize(2)` → see larger text
3. Run Example 2 with MPU-6050 + BMP280 connected → live pitch/roll/altitude dashboard
4. Run Example 3 → page through text sizes and shape primitives
5. Draw a progress bar for a simulated battery percentage

---

## Next Steps

- **08_projects** — drone HUD: combine IMU + barometer + OLED into a real-time status screen
- **04_communication_protocols/04_nrf24l01_wireless.md** — show received telemetry on the ground station OLED
