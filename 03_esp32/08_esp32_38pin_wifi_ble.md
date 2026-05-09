# 08 — ESP32 38-Pin Board: WiFi, BLE, and Full Pinout

## What Makes the 38-Pin Board Different

You bought the **38-pin ESP32 NodeMCU** — the wider version compared to the common 30-pin DevKit V1.

The 38-pin board exposes more GPIO pins and adds:
- GPIO 35, 34, 39 (VP), 36 (VN) — input-only ADC pins on the right side
- A more stable 3.3V regulator layout
- Slightly different physical width — may not fit a standard 400-point breadboard without bridging two breadboards

The chip is identical: **ESP32-WROOM-32** with Xtensa dual-core 240 MHz, 520 KB SRAM, 4 MB flash.

---

## 38-Pin Full Pinout

```
                    [ USB ]
                 ┌─────────┐
           3.3V  │ 1    38 │  GND
            GND  │ 2    37 │  GPIO23 (MOSI)
            GND  │ 3    36 │  GPIO22 (SCL / I2C)
            VIN  │ 4    35 │  TX0 (GPIO1)
         GPIO36  │ 5    34 │  RX0 (GPIO3)
         GPIO39  │ 6    33 │  GPIO21 (SDA / I2C)
         GPIO34  │ 7    32 │  GPIO19 (MISO)
         GPIO35  │ 8    31 │  GPIO18 (SCK / SPI)
         GPIO32  │ 9    30 │  GPIO5  (SPI CSN)
         GPIO33  │ 10   29 │  GPIO17 (TX2)
         GPIO25  │ 11   28 │  GPIO16 (RX2)
         GPIO26  │ 12   27 │  GPIO4
         GPIO27  │ 13   26 │  GPIO0  (Boot button)
         GPIO14  │ 14   25 │  GPIO2  (onboard LED)
         GPIO12  │ 15   24 │  GPIO15
         GPIO13  │ 16   23 │  GPIO8  (internal flash — avoid)
           SHDN  │ 17   22 │  GPIO7  (internal flash — avoid)
            3.3V │ 18   21 │  GPIO6  (internal flash — avoid)
            GND  │ 19   20 │  GND
                 └─────────┘
```

### Pin Categories at a Glance

| Category | Pins | Notes |
|----------|------|-------|
| ADC (input only) | 34, 35, 36 (VP), 39 (VN) | Cannot output, no internal pullup |
| Safe ADC1 (also output) | 32, 33, 25, 26, 27 | Use for sensors; safe with WiFi |
| ADC2 (conflicts with WiFi) | 0, 2, 4, 12–15, 25–27 | Avoid for analog when WiFi active |
| I2C default | SDA=21, SCL=22 | Shared by all I2C sensors |
| SPI default | MOSI=23, MISO=19, SCK=18, CS=5 | Used by NRF24L01 |
| UART0 (USB serial) | TX=1, RX=3 | Reserved for Serial Monitor |
| UART2 | TX=17, RX=16 | Free for GPS or other serial |
| PWM capable | All output pins | 16 channels, ledc API |
| Touch input | 0, 2, 4, 12–15, 27, 32, 33 | Capacitive touch |
| Avoid completely | 6, 7, 8, 9, 10, 11 | Connected to internal flash |

---

## WiFi on the ESP32

The ESP32 has a full 802.11 b/g/n WiFi radio built in. No external module needed.

### WiFi Modes

| Mode | Description | Use Case |
|------|-------------|----------|
| Station (STA) | Connects to a home/office router | Send data to internet, OTA updates |
| Access Point (AP) | Creates its own WiFi network | Connect phone directly to drone |
| STA + AP | Both at once | Rare, used for mesh networking |

### Code Example — Station Mode: Connect to WiFi

```cpp
#include <WiFi.h>

const char* ssid     = "YourNetworkName";
const char* password = "YourPassword";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // WiFi stays connected automatically
}
```

### Code Example — Access Point Mode (Drone Ground Station)

The drone creates its own WiFi. You connect a phone or laptop to it directly.

```cpp
#include <WiFi.h>

const char* ap_ssid     = "Drone-01";
const char* ap_password = "drone1234";   // min 8 characters

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  Serial.print("AP started. IP: ");
  Serial.println(WiFi.softAPIP());   // usually 192.168.4.1
}

void loop() {
  // Connected devices can now reach the drone at 192.168.4.1
}
```

---

## Simple WiFi Telemetry Server (UDP)

Send drone sensor data to a laptop over WiFi — useful for logging and tuning.

```cpp
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "YourNetworkName";
const char* pass = "YourPassword";

// Laptop IP and port — set your laptop's IP here
IPAddress laptopIP(192, 168, 1, 100);
const int udpPort = 4210;

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected: " + WiFi.localIP().toString());
  udp.begin(udpPort);
}

void loop() {
  // Example: send pitch, roll, altitude as CSV string
  float pitch = 5.2, roll = -1.3, altitude = 12.4;

  char buf[64];
  snprintf(buf, sizeof(buf), "%.1f,%.1f,%.1f", pitch, roll, altitude);

  udp.beginPacket(laptopIP, udpPort);
  udp.write((uint8_t*)buf, strlen(buf));
  udp.endPacket();

  delay(100);   // 10 Hz telemetry
}
```

On your laptop, listen with Python:
```python
import socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", 4210))
while True:
    data, addr = sock.recvfrom(1024)
    print(data.decode())
```

---

## BLE (Bluetooth Low Energy) on the ESP32

BLE is lower power than Classic Bluetooth. You can connect a phone app (like **nRF Connect** or **BLE Terminal**) to the ESP32 without pairing.

### Code Example — BLE UART (send data to phone)

```cpp
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Standard Nordic UART Service UUIDs
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHAR_UUID_TX        "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHAR_UUID_RX        "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic* pTxChar;
bool deviceConnected = false;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* s)    { deviceConnected = true;  Serial.println("BLE connected"); }
  void onDisconnect(BLEServer* s) { deviceConnected = false; Serial.println("BLE disconnected"); }
};

class RxCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* c) {
    String value = c->getValue().c_str();
    Serial.print("Received from phone: "); Serial.println(value);
  }
};

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32-Drone");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  pTxChar = pService->createCharacteristic(CHAR_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY);
  pTxChar->addDescriptor(new BLE2902());

  BLECharacteristic* pRxChar = pService->createCharacteristic(CHAR_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE);
  pRxChar->setCallbacks(new RxCallbacks());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("BLE advertising as 'ESP32-Drone'");
}

void loop() {
  if (deviceConnected) {
    String msg = "Alt: " + String(12.4) + " Pitch: " + String(5.2);
    pTxChar->setValue(msg.c_str());
    pTxChar->notify();
  }
  delay(500);
}
```

Connect using **nRF Connect** app on Android/iOS → scan → "ESP32-Drone" → connect → enable notifications on the TX characteristic → see live data.

---

## OTA (Over-the-Air) Updates

Once your drone is assembled, you don't want to plug in a USB cable to update firmware. OTA lets you upload new code over WiFi.

```cpp
#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "YourNetwork";
const char* pass = "YourPassword";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  ArduinoOTA.setHostname("esp32-drone");
  ArduinoOTA.setPassword("ota-password");

  ArduinoOTA.onStart([]() { Serial.println("OTA starting..."); });
  ArduinoOTA.onEnd([]()   { Serial.println("OTA done. Rebooting."); });
  ArduinoOTA.onError([](ota_error_t e) {
    Serial.print("OTA error: "); Serial.println(e);
  });

  ArduinoOTA.begin();
  Serial.println("OTA ready at: " + WiFi.localIP().toString());
}

void loop() {
  ArduinoOTA.handle();   // check for incoming OTA upload
  // rest of your program here
}
```

In Arduino IDE: after first flash, go to **Tools → Port** → you'll see a network port appear: `esp32-drone at 192.168.x.x`. Upload to it wirelessly.

---

## ADC2 + WiFi Conflict (Important)

When WiFi is active, **ADC2 channels are unavailable** — they return garbage values or fail.

ADC2 pins: GPIO 0, 2, 4, 12, 13, 14, 15, 25, 26, 27

**Only use ADC1 pins for analog reading when WiFi is active:**
ADC1 pins: GPIO 32, 33, 34, 35, 36 (VP), 39 (VN)

This is why the joystick tutorial uses pins 32–35.

---

## Power Draw Reference

| State | Current Draw | Notes |
|-------|-------------|-------|
| Deep sleep | ~10 µA | Radio off, CPU off |
| Light sleep | ~800 µA | RAM preserved |
| Active (no radio) | ~80 mA | CPU running, WiFi off |
| WiFi transmitting | ~240 mA peak | Can spike to 500 mA |
| BLE advertising | ~130 mA | Lower than WiFi |
| WiFi + BLE | ~300 mA | Both active |

A 2000 mAh battery powering only the ESP32 at WiFi TX: ~8 hours if transmitting constantly. Much longer with sleep modes and intermittent TX.

---

## Boot and Strapping Pins

Some pins affect how the ESP32 boots — pulling them HIGH or LOW at power-on changes behavior:

| Pin | Effect if pulled LOW at boot |
|-----|------------------------------|
| GPIO 0 | Enter download/flash mode (Boot button) |
| GPIO 2 | Must be LOW for download mode to work |
| GPIO 12 | Sets flash voltage — leave floating |
| GPIO 15 | Silences boot log on UART0 if pulled LOW |

**Avoid using GPIO 0 and GPIO 12 for sensors** — they can prevent booting if held at the wrong voltage when power is applied.

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| "No port found" in Arduino IDE | Wrong USB driver | Install CP2102 or CH340 driver |
| Uploads work but device crashes | GPIO 0 tied LOW | Check nothing pulls GPIO 0 low |
| WiFi keeps disconnecting | Power dip during TX | Add 100µF cap across 3.3V/GND near ESP32 |
| ADC reads wrong values | Using ADC2 while WiFi active | Switch to GPIO 32–35 (ADC1) |
| BLE not appearing on phone | ArduinoOTA or WiFi started first | Only run one radio at a time or use both carefully |
| Board doesn't fit breadboard | 38-pin is wider than 30-pin | Use two breadboards side by side |

---

## What to Try

1. Run the WiFi Station example → confirm IP address printed on Serial Monitor
2. Ping the IP from your laptop: `ping 192.168.x.x`
3. Run the Access Point example → connect your phone to "Drone-01" network
4. Run the BLE example → install nRF Connect app → see "ESP32-Drone" advertising → connect → receive data
5. Enable OTA → reupload a modified sketch over WiFi (no USB cable)

---

## Next Steps

- **05_sensors_and_modules** — connect IMU, barometer, rangefinder to this board
- **07_drone_electronics** — WiFi telemetry from the drone back to a ground laptop
- **08_projects** — full drone flight controller project
