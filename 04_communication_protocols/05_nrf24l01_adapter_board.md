# 05 — NRF24L01 Adapter Board

## What Is It?

The **NRF24L01 Adapter Board** (also called breakout board or base board) is a small PCB that the NRF24L01 module plugs into. It solves two problems at once:

1. **Power filtering** — has a built-in 3.3V LDO regulator + decoupling capacitors, so the NRF24L01 gets clean power from a 5V supply without needing an external AMS1117 module
2. **Pin breakout** — the 2×4 female socket accepts the NRF24L01's 8 pins and routes them to a single-row male header, easier to breadboard

Without this adapter, the NRF24L01+PA+LNA is the #1 source of mysterious failures for beginners. With it, the power problem is solved by design.

---

## What the Adapter Adds

```
NRF24L01 module (8 pins, 2 rows, 2.54mm)
           ↓ plugs into ↓
    [Adapter Board]
    - AMS1117-3.3 regulator
    - 10µF + 100nF decoupling caps
    - LED power indicator
           ↓ output ↓
    Single-row 8-pin header (breadboard friendly)
```

The adapter board **accepts 5V on its VCC pin** and internally steps it down to 3.3V for the NRF24L01. Your microcontroller side only needs to connect to the 8-pin header.

---

## Adapter Board Pin Header

The single-row output header pinout (most common variant):

| Pin | Label | Connect to |
|-----|-------|-----------|
| 1 | GND | ESP32 GND |
| 2 | VCC | **5V** (not 3.3V — adapter has its own regulator) |
| 3 | CE | Any ESP32 GPIO (e.g. GPIO 4) |
| 4 | CSN | Any ESP32 GPIO (e.g. GPIO 5) |
| 5 | SCK | GPIO 18 (SPI clock) |
| 6 | MOSI | GPIO 23 (SPI MOSI) |
| 7 | MISO | GPIO 19 (SPI MISO) |
| 8 | IRQ | Not connected (optional) |

> **Critical:** Feed **5V into the adapter VCC**, not 3.3V. The adapter's regulator needs headroom to regulate down to 3.3V. If you feed 3.3V in, the regulator output will be even lower (≈2.8V) and the NRF24L01 will not work.

---

## Wiring to ESP32

| Adapter Header Pin | ESP32 Pin | Notes |
|-------------------|-----------|-------|
| GND | GND | |
| VCC | **5V (VIN)** | Must be 5V — adapter regulates to 3.3V |
| CE | GPIO 4 | |
| CSN | GPIO 5 | |
| SCK | GPIO 18 | Hardware SPI |
| MOSI | GPIO 23 | Hardware SPI |
| MISO | GPIO 19 | Hardware SPI |
| IRQ | — | Leave disconnected |

This is the same pin assignment as the bare NRF24L01 tutorial — the adapter is a drop-in replacement, the software does not change.

---

## Comparison: With vs Without Adapter

| | Bare NRF24L01+PA+LNA | With Adapter Board |
|-|---------------------|-------------------|
| Power source | Must be clean 3.3V | Can use 5V directly |
| Decoupling caps | You add them manually | Built into the board |
| Breadboard fit | 2×4 socket (awkward) | Single-row header (easy) |
| Failure rate | High (power issues) | Low |
| Cost | ₹180 (module only) | ₹180 + ₹35 = ₹215 |

For ₹35 the adapter is almost always worth it.

---

## Code

**No code changes needed.** The adapter is transparent to the software.
Use the same code from `04_nrf24l01_wireless.md`:

```cpp
#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5);   // CE=GPIO4, CSN=GPIO5 — exactly the same as without adapter

const byte address[6] = "00001";

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();
}

void loop() {
  const char msg[] = "ping";
  radio.write(&msg, sizeof(msg));
  delay(20);
}
```

---

## Checking if the Adapter Is Working

1. Power LED on the adapter should be lit when VCC is connected
2. Run the NRF24L01 basic transmitter sketch
3. If `radio.begin()` returns true — adapter is working correctly

If `radio.begin()` returns false:
- Confirm VCC is 5V (not 3.3V)
- Check SPI pin connections (SCK/MOSI/MISO)
- Confirm NRF24L01 module is fully seated in the adapter socket (all 8 pins engaged)

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| `radio.begin()` returns false | NRF module not seated properly | Press the module firmly into the adapter socket |
| `radio.begin()` false even when seated | VCC = 3.3V instead of 5V | Move VCC wire to ESP32 VIN (5V) pin |
| Sends OK but range is very short | PA level on MIN | Change to `RF24_PA_HIGH` or `RF24_PA_MAX` |
| Power LED off | No VCC or GND | Check both power connections |
| Overheating adapter chip | VCC > 6V | AMS1117 max input is ~12V but 5V is ideal |

---

## Using Two Adapters (TX + RX)

Each ESP32 needs its own adapter board — one for the transmitter, one for the receiver (drone). Both plug in identically.

```
GROUND STATION:
  ESP32 + NRF24L01 Adapter + NRF24L01+PA+LNA module

DRONE:
  ESP32 + NRF24L01 Adapter + NRF24L01+PA+LNA module
```

The adapters make the build much cleaner — no loose caps, no bodge wires, no power noise.

---

## Next Steps

- **04_nrf24l01_wireless.md** — full transmitter/receiver software (unchanged with adapter)
- **03_joystick_shield.md** or **09_ps2_joystick_module.md** — add joystick to the transmitter side
