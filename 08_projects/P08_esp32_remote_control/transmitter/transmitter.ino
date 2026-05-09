/*
  ESP32 Drone Remote Control — Transmitter
  ─────────────────────────────────────────
  Hardware:
    • ESP32 (38-pin or LOLIN D1)
    • 2× PS2 Joystick modules  (or Joystick Shield)
    • NRF24L01+PA+LNA           (with 100µF cap on VCC/GND)
    • SSD1306 0.96" OLED        (I2C, address 0x3C)
    • 2× push buttons           (ARM, MODE)

  Wiring summary:
    NRF24L01  CE=4  CSN=5  SCK=18  MOSI=23  MISO=19  VCC=3.3V  GND
    Left stick  VRx(Yaw)=34  VRy(Throttle)=35  SW=25
    Right stick VRx(Roll)=32  VRy(Pitch)=33    SW=26
    OLED        SCL=22  SDA=21
    ARM button  GPIO 27  (INPUT_PULLUP, active LOW)
    MODE button GPIO 14  (INPUT_PULLUP, active LOW)

  Libraries needed (install via Arduino Library Manager):
    • RF24          by TMRh20
    • Adafruit SSD1306
    • Adafruit GFX Library

  Arming safety:
    Hold ARM button for 2 seconds.
    Throttle must be at bottom position to arm.
    All channels send zero while disarmed.
*/

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ── Pins ──────────────────────────────────────────────────────────
#define PIN_NRF_CE     4
#define PIN_NRF_CSN    5
#define PIN_LEFT_X    34    // Yaw axis
#define PIN_LEFT_Y    35    // Throttle axis
#define PIN_RIGHT_X   32    // Roll axis
#define PIN_RIGHT_Y   33    // Pitch axis
#define PIN_LEFT_SW   25
#define PIN_RIGHT_SW  26
#define PIN_ARM_BTN   27
#define PIN_MODE_BTN  14

// ── Config ────────────────────────────────────────────────────────
#define RADIO_CHANNEL  108    // 2.508 GHz — above most WiFi
#define TX_INTERVAL_MS  20    // 50 Hz
#define ARM_HOLD_MS   2000    // hold ARM button 2s to toggle arm state
#define STICK_DEADZONE  30    // dead band around center (in mapped units, out of 500)
#define OLED_ADDR     0x3C

// ── Packet ────────────────────────────────────────────────────────
// Both TX and RX must use the exact same struct.
struct DronePacket {
  uint16_t throttle;   // 0–1000  (0 = motors off)
  int16_t  roll;       // -500 to +500
  int16_t  pitch;      // -500 to +500
  int16_t  yaw;        // -500 to +500
  uint8_t  armed;      // 0 = disarmed, 1 = armed
  uint8_t  mode;       // 0=stabilize, 1=alt-hold, 2=manual
};

// ── Joystick calibration ──────────────────────────────────────────
// Run calibration mode first (see loop comment) then fill these in.
// Default values assume a typical ESP32 ADC range with 3.3V supply.
struct StickCal { int minVal, center, maxVal; };

StickCal calLeftX  = {150, 2048, 3900};   // Yaw
StickCal calLeftY  = {150, 2048, 3900};   // Throttle
StickCal calRightX = {150, 2048, 3900};   // Roll
StickCal calRightY = {150, 2048, 3900};   // Pitch

// Set to true, upload, open Serial Monitor, move sticks to all extremes,
// then set back to false and fill in the printed min/center/max values above.
const bool CALIBRATION_MODE = false;

// ── Globals ──────────────────────────────────────────────────────
RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const byte PIPE_ADDR[6] = "DRONE";

bool    armed        = false;
uint8_t flightMode   = 0;
bool    armBtnHeld   = false;
unsigned long armBtnPressedAt = 0;
unsigned long lastTxTime      = 0;
bool    radioOK      = false;
bool    lastSendOK   = false;
uint32_t packetsSent   = 0;
uint32_t packetsFailed = 0;

// Calibration tracking
int calMinLX, calMaxLX, calMinLY, calMaxLY;
int calMinRX, calMaxRX, calMinRY, calMaxRY;

// ── Stick helpers ─────────────────────────────────────────────────

// Maps a raw ADC value to -500..+500 with a center dead zone.
int16_t mapAxis(int raw, StickCal& cal, int deadzone) {
  raw = constrain(raw, cal.minVal, cal.maxVal);
  int16_t mapped;
  if (raw < cal.center) {
    mapped = map(raw, cal.minVal, cal.center, -500, 0);
  } else {
    mapped = map(raw, cal.center, cal.maxVal, 0, 500);
  }
  return (abs(mapped) < deadzone) ? 0 : mapped;
}

// Maps a raw ADC value to 0..1000 (no center, no deadzone — for throttle).
uint16_t mapThrottle(int raw, StickCal& cal) {
  raw = constrain(raw, cal.minVal, cal.maxVal);
  // Invert so stick UP = high throttle. Most PS2 modules read low when pushed up.
  return (uint16_t)map(raw, cal.maxVal, cal.minVal, 0, 1000);
}

// ── ARM button logic ──────────────────────────────────────────────
void handleArmButton(uint16_t currentThrottle) {
  bool pressed = (digitalRead(PIN_ARM_BTN) == LOW);

  if (pressed && !armBtnHeld) {
    armBtnHeld       = true;
    armBtnPressedAt  = millis();
  }
  if (!pressed) {
    armBtnHeld = false;
  }

  if (armBtnHeld && (millis() - armBtnPressedAt >= ARM_HOLD_MS)) {
    // Safety: can only arm when throttle is at bottom (< 10%)
    if (!armed && currentThrottle > 100) {
      return;
    }
    armed       = !armed;
    armBtnHeld  = false;  // prevent immediate re-toggle
    delay(300);
  }
}

// ── Display ───────────────────────────────────────────────────────
void updateDisplay(DronePacket& pkt) {
  char buf[22];

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Row 0 — link + arm status
  display.setCursor(0, 0);
  display.print(lastSendOK ? "LINKED  " : "NO LINK ");
  display.print(pkt.armed ? "[ARMED]" : "[SAFE] ");

  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  // Row 1-2 — channel values
  display.setCursor(0, 13);
  snprintf(buf, sizeof(buf), "THR:%4u  YAW:%4d", pkt.throttle, pkt.yaw);
  display.print(buf);

  display.setCursor(0, 23);
  snprintf(buf, sizeof(buf), "PIT:%4d  ROL:%4d", pkt.pitch, pkt.roll);
  display.print(buf);

  // Row 3 — throttle bar
  display.setCursor(0, 34);
  display.print("T ");
  int barW = map(pkt.throttle, 0, 1000, 0, 104);
  display.drawRect(14, 34, 106, 7, SSD1306_WHITE);
  if (barW > 0) display.fillRect(14, 34, barW, 7, SSD1306_WHITE);

  // Row 4 — mode + packet stats
  display.setCursor(0, 44);
  snprintf(buf, sizeof(buf), "MODE:%d  OK:%lu", pkt.mode, packetsSent % 10000);
  display.print(buf);

  // Row 5 — fail count + rate
  display.setCursor(0, 54);
  snprintf(buf, sizeof(buf), "FAIL:%lu  50Hz CH%d", packetsFailed % 10000, RADIO_CHANNEL);
  display.print(buf);

  display.display();
}

// ── Calibration mode ──────────────────────────────────────────────
void runCalibration() {
  int lx = analogRead(PIN_LEFT_X);
  int ly = analogRead(PIN_LEFT_Y);
  int rx = analogRead(PIN_RIGHT_X);
  int ry = analogRead(PIN_RIGHT_Y);

  calMinLX = min(calMinLX, lx); calMaxLX = max(calMaxLX, lx);
  calMinLY = min(calMinLY, ly); calMaxLY = max(calMaxLY, ly);
  calMinRX = min(calMinRX, rx); calMaxRX = max(calMaxRX, rx);
  calMinRY = min(calMinRY, ry); calMaxRY = max(calMaxRY, ry);

  Serial.printf("LX:%4d(%4d-%4d)  LY:%4d(%4d-%4d)  RX:%4d(%4d-%4d)  RY:%4d(%4d-%4d)\n",
    lx, calMinLX, calMaxLX,
    ly, calMinLY, calMaxLY,
    rx, calMinRX, calMaxRX,
    ry, calMinRY, calMaxRY);
}

// ── Setup ─────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP32 Drone Transmitter ===");

  pinMode(PIN_LEFT_SW,  INPUT_PULLUP);
  pinMode(PIN_RIGHT_SW, INPUT_PULLUP);
  pinMode(PIN_ARM_BTN,  INPUT_PULLUP);
  pinMode(PIN_MODE_BTN, INPUT_PULLUP);

  Wire.begin(21, 22);
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 20);
    display.println("Drone TX v1.0");
    display.setCursor(10, 34);
    display.println("Initializing...");
    display.display();
  } else {
    Serial.println("OLED not found — check I2C wiring");
  }

  radioOK = radio.begin();
  if (!radioOK) {
    Serial.println("NRF24L01 not responding — check wiring and power (100uF cap?)");
  } else {
    radio.openWritingPipe(PIPE_ADDR);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);    // lowest rate = longest range
    radio.setChannel(RADIO_CHANNEL);
    radio.stopListening();
    Serial.println("NRF24L01 OK");
  }

  if (CALIBRATION_MODE) {
    // Seed cal tracking with first reading
    calMinLX = calMaxLX = analogRead(PIN_LEFT_X);
    calMinLY = calMaxLY = analogRead(PIN_LEFT_Y);
    calMinRX = calMaxRX = analogRead(PIN_RIGHT_X);
    calMinRY = calMaxRY = analogRead(PIN_RIGHT_Y);
    Serial.println("CALIBRATION MODE — move all sticks to their extremes, then check Serial for min/max values");
  }

  delay(800);
}

// ── Loop ──────────────────────────────────────────────────────────
void loop() {
  if (CALIBRATION_MODE) {
    runCalibration();
    delay(100);
    return;
  }

  // MODE button cycles through 3 flight modes
  if (digitalRead(PIN_MODE_BTN) == LOW) {
    flightMode = (flightMode + 1) % 3;
    delay(250);
  }

  if (millis() - lastTxTime >= TX_INTERVAL_MS) {
    lastTxTime = millis();

    DronePacket pkt;
    pkt.throttle = mapThrottle(analogRead(PIN_LEFT_Y), calLeftY);
    pkt.yaw      = mapAxis(analogRead(PIN_LEFT_X),  calLeftX,  STICK_DEADZONE);
    pkt.roll     = mapAxis(analogRead(PIN_RIGHT_X), calRightX, STICK_DEADZONE);
    pkt.pitch    = mapAxis(analogRead(PIN_RIGHT_Y), calRightY, STICK_DEADZONE);
    // Invert pitch so stick forward = nose down (standard RC convention)
    pkt.pitch    = -pkt.pitch;
    pkt.mode     = flightMode;

    handleArmButton(pkt.throttle);
    pkt.armed = armed ? 1 : 0;

    // Zero everything while disarmed — drone side also enforces this but belt-and-suspenders
    if (!armed) {
      pkt.throttle = 0;
      pkt.roll = pkt.pitch = pkt.yaw = 0;
    }

    if (radioOK) {
      lastSendOK = radio.write(&pkt, sizeof(pkt));
      lastSendOK ? packetsSent++ : packetsFailed++;
    }

    updateDisplay(pkt);

    Serial.printf("T:%4u R:%4d P:%4d Y:%4d ARM:%d MODE:%d  %s\n",
      pkt.throttle, pkt.roll, pkt.pitch, pkt.yaw,
      pkt.armed, pkt.mode,
      lastSendOK ? "OK" : "FAIL");
  }
}
