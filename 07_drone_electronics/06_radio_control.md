# 06 — Radio Control

## The Radio Link

The RC link connects you to the drone through the air. It carries your stick commands from the transmitter in your hands to the receiver mounted on the drone.

```
YOU (Pilot)
  │
  ▼
┌──────────────────────┐
│   TX (Transmitter)   │  ← the controller in your hands
│   also called "radio"│
│   or "radio controller"│
└────────┬─────────────┘
         │  2.4 GHz or 900 MHz radio waves
         │  range: 1–30+ km depending on system
         ▼
┌──────────────────────┐
│   RX (Receiver)      │  ← small board mounted on drone
│   mounted on drone   │
└────────┬─────────────┘
         │  serial data (SBUS, IBUS, CRSF, etc.)
         ▼
  Flight Controller
```

---

## RC Channels

Each "channel" is one piece of information. A channel carries a value, typically 1000–2000 (microseconds in PWM terms, but kept as a number in digital protocols).

```
Standard 4-channel mapping (Mode 2 — most common):

  Left stick:                Right stick:
  ┌─────────┐               ┌─────────┐
  │    ↑    │               │    ↑    │
  │ Throttle│               │  Pitch  │
  │  CH3    │               │  CH2    │
  │←  Yaw  →│               │← Roll →│
  │  CH4    │               │  CH1    │
  └─────────┘               └─────────┘

Channel 1 (CH1): Roll      1000=full left,  1500=center, 2000=full right
Channel 2 (CH2): Pitch     1000=full back,  1500=center, 2000=full forward
Channel 3 (CH3): Throttle  1000=zero power, 2000=full power (no center)
Channel 4 (CH4): Yaw       1000=full left,  1500=center, 2000=full right

CH5+: Switches, arm/disarm, flight modes, beepers, etc.
```

**Mode 1 vs Mode 2:** In Mode 2 (most common globally), throttle is on the left stick. In Mode 1, throttle is on the right stick. You choose your preference when configuring your TX.

---

## Popular RC Systems

### FrSky (Taranis, Radiomaster TX16S / Zorro)

- Protocol: D16, ACCESS, or compatible with OpenTX/EdgeTX
- Receiver output: SBUS (most common), FPORT
- Range: 1–2 km typical
- Very popular in the FPV community
- Open-source firmware: **EdgeTX** (formerly OpenTX)

### ExpressLRS (ELRS)

- Protocol: ELRS (fully open source)
- Receiver output: CRSF serial
- Range: 10–30+ km (long range)
- Latency: as low as 1ms
- **Currently the most popular choice for new builds**
- Runs on Radiomaster, BetaFPV, and many other TXs

### FlySky

- Protocol: AFHDS, AFHDS-2A
- Receiver output: IBUS (digital), PPM (legacy)
- Range: 500m–1km typical
- Budget-friendly, good for beginners

### DSM2 / DSMX (Spektrum)

- Popular in the USA
- Receiver output: SBUS or serial
- Range: similar to FrSky

---

## RC Output Protocols

The receiver outputs data to the flight controller. Multiple formats exist.

### PPM (Pulse Position Modulation) — Legacy

```
One wire carries all channels as a sequence of pulses:
  ─┐ ┌─┐  ┌──┐   ┌───┐
   └─┘ └──┘  └───┘   └──  (each gap duration = channel value)
   CH1  CH2   CH3    CH4...

Update rate: ~50 Hz (very slow)
Channels: 8 max
Status: outdated, avoid
```

### SBUS (Serial Bus)

```
Protocol: inverted UART at 100,000 baud, 8E2 (8 bits, even parity, 2 stop bits)
Channels: 16 (+ 2 digital channels)
Update rate: 100 Hz (14ms frame period)
Voltage: 3.3V or 5V
One wire (plus GND)
Inverted: most UARTs need hardware inverter or software inversion
```

SBUS is the de-facto standard for FrSky and many other systems. Almost all FCs support it.

SBUS frame format:
```
[0x0F] [CH1-CH16 packed as 11-bit values] [flags] [0x00]
 start   22 bytes of channel data           1 byte   end
```

### IBUS (FlySky)

```
Protocol: normal UART at 115,200 baud
Channels: 14
Update rate: 7ms (142 Hz)
One wire
Not inverted (easier to connect)
```

### CRSF (Crossfire / ExpressLRS)

```
Protocol: full-duplex UART at 420,000 baud
Channels: 16
Update rate: 100–1000 Hz depending on mode
Bi-directional: FC can send data back to TX (telemetry on same wire)
```

CRSF is the best modern protocol. Used by TBS Crossfire and ExpressLRS receivers. Latency of ~1ms at 1000Hz.

---

## Connecting RX to FC

### SBUS Connection

```
Receiver         FC
  SBUS ─────────► UART RX pin (configured as SBUS input)
  5V   ─────────► 5V
  GND  ─────────► GND

Note: SBUS signal is inverted! On STM32 FCs, enable "UART inverter" in software.
On ESP32: use software serial inversion (set UART to inverted mode).
```

### IBUS Connection

```
Receiver         FC
  IBUS ─────────► UART RX pin (configured as IBUS input)
  5V   ─────────► 5V
  GND  ─────────► GND

Not inverted — direct connection works.
```

### CRSF Connection

```
Receiver         FC
  TX   ─────────► FC UART RX
  RX   ─────────► FC UART TX    ← bidirectional!
  5V   ─────────► 5V
  GND  ─────────► GND
```

---

## Binding TX and RX

Before the receiver will respond to your transmitter, they must be **bound** — they exchange secret codes to recognize each other and ignore other transmitters.

```
Generic binding procedure:
1. Put receiver into binding mode
   (usually: power on with bind button held, or specific procedure)
2. Put transmitter into bind mode
   (menu: Bind → select protocol → start bind)
3. They exchange codes and save them
4. Power cycle receiver
5. Receiver LED should be solid (not blinking) = bound and connected
```

---

## Reading SBUS on ESP32

SBUS uses inverted UART. ESP32 supports configuring UART with signal inversion.

```cpp
#include <HardwareSerial.h>

#define SBUS_BAUD 100000
#define SBUS_RX_PIN 16

HardwareSerial sbusSerial(2);  // UART2

void setup() {
  // SBUS: 100000 baud, 8E2 format, inverted
  sbusSerial.begin(SBUS_BAUD, SERIAL_8E2, SBUS_RX_PIN, -1, true);  // true = inverted
}

uint16_t channels[16];

void parseSBUS(uint8_t* buf) {
  if (buf[0] != 0x0F || buf[24] != 0x00) return;  // check frame markers

  channels[0]  = ((buf[1]     | buf[2]  << 8) & 0x07FF);
  channels[1]  = ((buf[2]>>3  | buf[3]  << 5) & 0x07FF);
  channels[2]  = ((buf[3]>>6  | buf[4]  << 2 | buf[5] << 10) & 0x07FF);
  // ... continue for all 16 channels (each is 11 bits, packed)
}

void loop() {
  if (sbusSerial.available() >= 25) {
    uint8_t buf[25];
    sbusSerial.readBytes(buf, 25);
    parseSBUS(buf);

    // channels[0] = roll (172–1811 range maps to 1000–2000)
    // channels[1] = pitch
    // channels[2] = throttle
    // channels[3] = yaw
  }
}
```

SBUS channel values range: **172 to 1811** (corresponds to 1000–2000 in PWM microseconds).

Conversion: `pwm_us = (sbus_value - 172) × (1000.0 / 1639.0) + 1000`

---

## Reading IBUS on ESP32

Simpler — no inversion needed.

```cpp
#include <HardwareSerial.h>

HardwareSerial ibusSerial(2);
uint16_t ibusChannels[14];

void setup() {
  ibusSerial.begin(115200, SERIAL_8N1, 16, -1);
}

bool parseIBUS(uint8_t* buf) {
  // IBUS frame: 0x20 0x40 [28 bytes channel data] [2 byte checksum]
  if (buf[0] != 0x20 || buf[1] != 0x40) return false;

  uint16_t checksum = 0xFFFF;
  for (int i = 0; i < 30; i++) checksum -= buf[i];
  uint16_t recv_checksum = buf[30] | (buf[31] << 8);
  if (checksum != recv_checksum) return false;

  for (int i = 0; i < 14; i++) {
    ibusChannels[i] = buf[2 + i*2] | (buf[3 + i*2] << 8);
  }
  return true;
}
```

IBUS channel range: **1000 to 2000** directly in microseconds — no conversion needed.

---

## Failsafe

What happens if you fly behind a building and lose radio signal?

A proper failsafe prevents a flyaway or crash.

```
Failsafe modes:
  1. No pulses → FC detects signal loss → apply pre-set position
  2. Preset: motors off (disarm) — safest for most situations
  3. Preset: hold last known position (requires GPS)
  4. Preset: return to home (requires GPS)

SBUS failsafe:
  The receiver sends a frame with the failsafe flag bit set
  (byte 23, bit 3 in the SBUS frame)
  FC detects this flag and applies failsafe action
```

**Always configure failsafe.** Default to "disarm" for beginners. "Return to Home" for GPS builds.

---

## Radio Range and Interference

### 2.4 GHz

- Short range (1–2 km line-of-sight)
- Susceptible to Wi-Fi interference (Wi-Fi also uses 2.4 GHz)
- Most RC systems: FrSky, FlySky, older ELRS

### 900 MHz

- Long range (10–30+ km)
- Better penetration through obstacles
- Larger antenna
- ExpressLRS 900MHz modules, TBS Crossfire
- Used for long-range and fixed-wing applications

### Antenna Placement

```
Do:
  Keep antennas perpendicular (one vertical, one horizontal)
  Keep away from carbon fiber (CF blocks RF)
  Keep away from video transmitter and its antenna

Don't:
  Coil the antenna wire (detunes it)
  Zip-tie the antenna flat against CF frame
  Put antennas directly next to each other (parallel)
```

---

## Channel Mapping Summary

| Channel | Control | Value 1000 | Value 1500 | Value 2000 |
|---------|---------|------------|------------|------------|
| CH1 | Roll | Full Left | Center | Full Right |
| CH2 | Pitch | Full Back | Center | Full Forward |
| CH3 | Throttle | Zero | Half | Full |
| CH4 | Yaw | Spin Left | Center | Spin Right |
| CH5 | Arm/Disarm | Disarmed | — | Armed |
| CH6 | Flight Mode | Angle | Acro | — |

---

## Practice

1. What is the difference between a TX and an RX?
2. A receiver is bound to a TX. What does "bound" mean?
3. SBUS operates at what baud rate? Is it inverted or not?
4. On a scale of 1000–2000, what RC value should throttle be at when the drone is sitting still on the ground?
5. What is a failsafe and why is it important?
6. Why is 900 MHz better than 2.4 GHz for long-range flying?
7. An SBUS channel reads 172. What is the equivalent PWM microsecond value? (Use the conversion formula.)
8. What is the update rate of SBUS vs CRSF? Which is faster?
