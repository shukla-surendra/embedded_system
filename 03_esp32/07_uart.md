# 07 — UART: Serial Communication

UART is the most fundamental way two devices exchange data. Your GPS module, Bluetooth module, and debug output all use UART.

---

## What is UART?

**UART (Universal Asynchronous Receiver/Transmitter)** sends data as a sequence of bits over a single wire (one wire per direction).

```
Device A                    Device B
         TX ──────────────► RX
         RX ◄────────────── TX
         GND ────────────── GND   (always connect GND!)
```

- **TX (Transmit):** sends data out
- **RX (Receive):** receives incoming data
- **Cross-connect:** A's TX goes to B's RX (and vice versa)
- **Baud rate:** speed in bits per second — must match on both devices

### Common baud rates
| Rate | Use |
|------|-----|
| 9600 | Old GPS modules, slow sensors |
| 57600 | Older modules |
| 115200 | Standard for Arduino/ESP32 debug |
| 921600 | ESP32 flashing speed |

---

## The ESP32 Has 3 UARTs

| Port | Default Pins | Use |
|------|-------------|-----|
| UART0 | GPIO1 (TX), GPIO3 (RX) | USB-Serial (debug) — avoid for devices |
| UART1 | configurable | Available |
| UART2 | GPIO17 (TX), GPIO16 (RX) | Available — use for GPS, sensors |

---

## Using Serial (UART0 — Debug)

```cpp
void setup() {
    Serial.begin(115200);    // UART0 at 115200 baud
    Serial.println("System started");
}

void loop() {
    Serial.print("Counter: ");
    Serial.println(millis() / 1000);   // seconds since boot
    delay(1000);
}
```

| Function | What it does |
|----------|-------------|
| `Serial.print(x)` | Print without newline |
| `Serial.println(x)` | Print with newline |
| `Serial.printf("V=%.2f\n", v)` | Formatted print (like C printf) |
| `Serial.available()` | Returns number of bytes waiting to be read |
| `Serial.read()` | Read one byte |
| `Serial.readString()` | Read until timeout |

---

## Reading Data From Serial

You can send commands to the ESP32 from the Serial Monitor:

```cpp
void setup() {
    Serial.begin(115200);
}

void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');  // read until Enter key
        command.trim();  // remove whitespace/newline

        if (command == "hello") {
            Serial.println("Hi there!");
        } else if (command == "led on") {
            digitalWrite(4, HIGH);
            Serial.println("LED turned on");
        } else if (command == "led off") {
            digitalWrite(4, LOW);
            Serial.println("LED turned off");
        } else {
            Serial.println("Unknown command: " + command);
        }
    }
}
```

Open Serial Monitor, set baud to 115200, set line ending to "Newline", type "hello" and press Enter.

---

## Using UART2 — Connecting a GPS Module

GPS modules typically output **NMEA sentences** over UART at 9600 baud.

### Wiring (GPS module like Neo-6M/Neo-8M):
```
GPS VCC → ESP32 3.3V (some need 5V — check datasheet)
GPS GND → ESP32 GND
GPS TX  → ESP32 GPIO16 (UART2 RX)
GPS RX  → ESP32 GPIO17 (UART2 TX)
```

### Code:

```cpp
HardwareSerial GPS(2);   // UART2

void setup() {
    Serial.begin(115200);   // Debug output
    GPS.begin(9600, SERIAL_8N1, 16, 17);   // UART2: 9600 baud, RX=16, TX=17
    Serial.println("Waiting for GPS data...");
}

void loop() {
    while (GPS.available()) {
        char c = GPS.read();
        Serial.print(c);    // Forward raw GPS data to debug monitor
    }
}
```

With a GPS module connected and a clear sky view, you'll see NMEA sentences like:
```
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
```

Use the **TinyGPS++** library to parse these into readable latitude/longitude.

---

## Debugging Tips with Serial

The Serial Monitor is your most important debugging tool. Use it to:

```cpp
// Print variable values
Serial.printf("Sensor raw=%d  voltage=%.3fV\n", rawADC, voltage);

// Print at the start of key functions
Serial.println("[setup] Initializing IMU...");

// Time measurements
unsigned long t1 = micros();
doSomething();
Serial.printf("doSomething() took %lu µs\n", micros() - t1);

// Watch for unexpected states
if (sensorValue > 4000) {
    Serial.println("WARNING: Sensor near max range!");
}
```

---

## Practice

1. Use `Serial.printf()` to print a table of ADC readings (0–4095 range) along with their voltage equivalents, one reading per second.

2. Create a simple serial command interpreter: type "blink fast", "blink slow", or "blink stop" into Serial Monitor to change blink behavior.

3. If you have a GPS module, wire it to UART2 and confirm you receive NMEA data.

---

**Module 03 complete.**

You can now:
- Write and upload programs to the ESP32
- Control digital outputs (LEDs, relays)
- Read digital inputs (buttons) with debouncing
- Read analog signals (sensors, battery voltage)
- Generate PWM signals (LED brightness, servos, ESC signals)
- Communicate via UART for debugging and sensors

**Next module:** [04 — Communication Protocols](../04_communication_protocols/README.md)
