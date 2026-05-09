# 04 — NRF24L01+PA+LNA Wireless Transceiver

## What Is It?

The **NRF24L01+PA+LNA** is a 2.4 GHz radio transceiver module made by Nordic Semiconductor.

- **NRF24L01+** — the chip itself, handles 2.4 GHz radio
- **PA** — Power Amplifier — boosts transmit power (longer range)
- **LNA** — Low Noise Amplifier — improves receive sensitivity (hear weak signals better)
- **SMA antenna** — external screw-on antenna for maximum range

The plain NRF24L01 (no PA/LNA) has a range of ~10–30m. The PA+LNA version with external antenna reaches **100–1000m** line-of-sight.

**For a drone:** this is your radio control link — the ESP32 on the drone receives commands from an ESP32 + joystick transmitter on the ground.

---

## How It Works

- Communicates over **SPI** (4 wires + 2 control pins)
- Supports 1–126 radio channels in the 2.4 GHz band
- Automatically handles acknowledgement, retransmission, and error checking
- Data rate: 250 kbps, 1 Mbps, or 2 Mbps
- Both modules must use the same channel and pipe address to talk to each other
- One acts as **transmitter (TX)**, one as **receiver (RX)** — roles can switch in software

---

## Hardware Overview

```
NRF24L01+PA+LNA Pin Layout (8 pins):

  GND  ── Ground
  VCC  ── 3.3V (DO NOT use 5V — will destroy the chip)
  CE   ── Chip Enable (RX/TX mode select)
  CSN  ── Chip Select Not (SPI chip select, active LOW)
  SCK  ── SPI Clock
  MOSI ── SPI Master Out Slave In
  MISO ── SPI Master In Slave Out
  IRQ  ── Interrupt (optional, active LOW)
```

> **Critical:** The NRF24L01 is 3.3V only. Even the data pins can be damaged by 5V logic in some cases. ESP32 runs at 3.3V so it's safe — never use with a 5V Arduino without a level shifter.

---

## Power Supply Warning (Very Important)

The PA+LNA version draws up to **250 mA peak** when transmitting. The ESP32's onboard 3.3V regulator cannot supply this reliably — you'll get random disconnects, garbled data, or no connection at all.

**Always add a 10µF–100µF electrolytic capacitor across the NRF24L01 VCC and GND pins.** This filters the current spikes and is the #1 fix for flaky connections.

If problems persist, power the NRF24L01 from a separate AMS1117 3.3V module (see `06_power_systems/02_ams1117_power_module.md`).

---

## Wiring to ESP32

| NRF24L01 Pin | ESP32 Pin  | Notes                              |
|--------------|------------|------------------------------------|
| GND          | GND        |                                    |
| VCC          | 3.3V       | Add 100µF cap here                 |
| CE           | GPIO 4     | Any digital pin                    |
| CSN          | GPIO 5     | Any digital pin                    |
| SCK          | GPIO 18    | Hardware SPI SCK on ESP32          |
| MOSI         | GPIO 23    | Hardware SPI MOSI on ESP32         |
| MISO         | GPIO 19    | Hardware SPI MISO on ESP32         |
| IRQ          | Not used   | Leave disconnected                 |

---

## Installing the Library

In Arduino IDE:
1. Open **Sketch → Include Library → Manage Libraries**
2. Search: `RF24`
3. Install: **"RF24" by TMRh20** — the most widely used and maintained library

---

## Code Example 1 — Transmitter (Ground Station / Joystick Side)

```cpp
#include <SPI.h>
#include <RF24.h>

// CE=GPIO4, CSN=GPIO5
RF24 radio(4, 5);

// Both ends must use the same address (5 bytes, arbitrary but matching)
const byte address[6] = "00001";

struct DroneCommand {
  int throttle;   // 0–1000
  int roll;       // -500 to +500
  int pitch;      // -500 to +500
  int yaw;        // -500 to +500
};

void setup() {
  Serial.begin(115200);

  if (!radio.begin()) {
    Serial.println("NRF24L01 not responding. Check wiring and power.");
    while (true);
  }

  radio.openWritingPipe(address);     // TX writes to this address
  radio.setPALevel(RF24_PA_HIGH);     // RF24_PA_MIN / LOW / HIGH / MAX
  radio.setDataRate(RF24_250KBPS);    // longer range at lower data rate
  radio.setChannel(108);              // 2.508 GHz — above most WiFi channels
  radio.stopListening();              // set as transmitter

  Serial.println("Transmitter ready");
}

void loop() {
  DroneCommand cmd;
  cmd.throttle = 500;   // mid-throttle for test
  cmd.roll     = 0;
  cmd.pitch    = 0;
  cmd.yaw      = 0;

  bool sent = radio.write(&cmd, sizeof(cmd));

  if (sent) {
    Serial.println("Command sent OK");
  } else {
    Serial.println("Send failed — no ACK received");
  }

  delay(20);  // 50 Hz command rate
}
```

---

## Code Example 2 — Receiver (Drone Side)

```cpp
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5);

const byte address[6] = "00001";   // must match transmitter exactly

struct DroneCommand {
  int throttle;
  int roll;
  int pitch;
  int yaw;
};

DroneCommand lastCmd = {0, 0, 0, 0};   // safe default

void setup() {
  Serial.begin(115200);

  if (!radio.begin()) {
    Serial.println("NRF24L01 not found!");
    while (true);
  }

  radio.openReadingPipe(0, address);   // RX listens on this address
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();              // set as receiver

  Serial.println("Receiver ready, listening...");
}

void loop() {
  if (radio.available()) {
    DroneCommand cmd;
    radio.read(&cmd, sizeof(cmd));
    lastCmd = cmd;

    Serial.print("THR: "); Serial.print(cmd.throttle);
    Serial.print("  ROL: "); Serial.print(cmd.roll);
    Serial.print("  PIT: "); Serial.print(cmd.pitch);
    Serial.print("  YAW: "); Serial.println(cmd.yaw);
  }

  // If no packet received in 500ms — failsafe
  // In a real drone: cut throttle or hold position
}
```

---

## Code Example 3 — Two-Way Communication (Telemetry Back)

A real drone link sends commands down AND sends battery/status back up.

```cpp
// === TRANSMITTER SIDE (ground) ===
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5);

const byte txAddress[6] = "GNDST";   // ground → drone
const byte rxAddress[6] = "DRONE";   // drone → ground

struct Command  { int thr, rol, pit, yaw; };
struct Telemetry { float voltage; float altitude; int rssi; };

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(txAddress);
  radio.openReadingPipe(1, rxAddress);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();
}

void loop() {
  Command cmd = {500, 0, 0, 0};

  // Send command, switch to RX briefly to get telemetry
  radio.stopListening();
  radio.write(&cmd, sizeof(cmd));
  radio.startListening();
  delay(5);   // wait for drone to respond

  if (radio.available()) {
    Telemetry telem;
    radio.read(&telem, sizeof(telem));
    Serial.print("Battery: "); Serial.print(telem.voltage); Serial.print("V  ");
    Serial.print("Alt: ");     Serial.print(telem.altitude); Serial.println("m");
  }

  delay(20);
}
```

---

## PA Level Guide

| PA Level      | Current Draw | Indoor Range | Outdoor Range |
|---------------|-------------|-------------|---------------|
| RF24_PA_MIN   | ~7 mA       | ~5m         | ~20m          |
| RF24_PA_LOW   | ~7 mA       | ~10m        | ~50m          |
| RF24_PA_HIGH  | ~45 mA      | ~30m        | ~200m         |
| RF24_PA_MAX   | ~115 mA     | ~50m        | ~1000m        |

At MAX power the module needs clean 3.3V with decoupling capacitors — power supply quality matters most at this setting.

---

## Channel Selection

The 2.4 GHz band is shared with WiFi (channels 1–13 occupy 2.412–2.484 GHz).

```
WiFi coverage:  2.400 ──────────────────────── 2.500 GHz
NRF24 ch 0:     2.400 GHz
NRF24 ch 76:    2.476 GHz   ← common choice, above most WiFi
NRF24 ch 108:   2.508 GHz   ← above WiFi entirely
NRF24 ch 125:   2.525 GHz   ← maximum
```

Use channel 100+ to avoid WiFi interference. Both ends must use the same channel.

---

## Failsafe — Critical for Drones

If the radio link drops (drone flies out of range, interference), the drone must react safely.

```cpp
// On the receiver (drone):
unsigned long lastPacketTime = 0;
const unsigned long FAILSAFE_MS = 500;   // 0.5 seconds with no signal

void loop() {
  if (radio.available()) {
    radio.read(&cmd, sizeof(cmd));
    lastCmd = cmd;
    lastPacketTime = millis();
  }

  // Failsafe: no packet for 500ms → cut motors
  if (millis() - lastPacketTime > FAILSAFE_MS) {
    Serial.println("FAILSAFE: radio link lost, cutting throttle");
    lastCmd.throttle = 0;
    // set all motor PWM to zero here
  }
}
```

**Without failsafe:** a drone that loses signal keeps flying its last command — usually straight into the ground or away at full throttle.

---

## Common Problems

| Problem                            | Likely Cause                          | Fix                                                       |
|------------------------------------|---------------------------------------|-----------------------------------------------------------|
| "NRF24L01 not responding"          | Bad power supply                      | Add 100µF cap, use separate 3.3V regulator                |
| Sends fail ("no ACK")              | Wrong channel or address mismatch     | Check both sides use same channel and address             |
| Works on desk, fails at distance   | PA_MIN setting or no antenna          | Set RF24_PA_MAX, screw antenna on fully                   |
| Connects then drops randomly       | Power spike when transmitting         | Larger cap (100–470µF) or dedicated power supply          |
| Transmitter "sent OK" but RX shows nothing | TX and RX roles swapped      | Confirm TX calls `stopListening()`, RX calls `startListening()` |
| Interference from nearby WiFi      | Same frequency channel as WiFi        | Change channel to 100+                                    |

---

## How This Applies to the Drone Project

```
GROUND SIDE                              DRONE SIDE
[Joystick Shield]                        [ESP32 Flight Controller]
    ↓                                         ↑
[ESP32 Transmitter]  ──2.4GHz──→  [NRF24L01 Receiver]
    ↑                                         ↓
[NRF24L01 TX]        ←─2.4GHz──  [Telemetry: voltage, altitude]
```

The 50 Hz command loop gives 20ms latency — fast enough for stable flight. A real RC link (like FrSky) is similar in concept.

---

## What to Try

1. Wire two ESP32s, run Transmitter on one, Receiver on the other → see packets on Serial Monitor
2. Walk away — find your range limit
3. Change PA level and compare range
4. Unplug transmitter → verify failsafe triggers in 500ms on receiver
5. Add joystick values (next tutorial) → send real control inputs

---

## Next Steps

- **05_sensors_and_modules/03_joystick_shield.md** — add a joystick to the transmitter side
- **07_drone_electronics** — wire the full drone radio control stack
