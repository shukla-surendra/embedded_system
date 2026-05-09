# 04 — HC-SR04 Ultrasonic Distance Sensor

## What Is It?

The **HC-SR04** measures distance by bouncing a 40 kHz ultrasonic pulse off a surface and timing how long the echo takes to return.

- Measurement range: **2 cm – 400 cm**
- Accuracy: **±3 mm**
- Beam angle: ~15°

**For a drone:** used as a downward-facing rangefinder for precision landing — the drone knows it's close to the ground when distance drops below a threshold, then cuts throttle gently.

---

## How It Works (Physics)

```
TRIG pin: ESP32 sends a 10µs HIGH pulse → starts ultrasonic burst
ECHO pin: goes HIGH when burst fires, goes LOW when echo returns
Time = duration of ECHO HIGH pulse
Distance = (Time × speed of sound) / 2
```

Speed of sound = 343 m/s = 0.0343 cm/µs

```
Distance (cm) = (echo duration in µs) × 0.0343 / 2
             = echo_us / 58.2
```

Divide by 2 because the sound travels to the target AND back.

---

## Hardware Overview

```
HC-SR04 Pins:

  VCC   ── 5V (sensor needs 5V to operate)
  GND   ── Ground
  TRIG  ── Trigger input (send pulse here)
  ECHO  ── Echo output (measures echo duration)
```

> **Important:** The HC-SR04 runs on 5V and its ECHO pin outputs 5V logic. The ESP32 GPIO is 3.3V and NOT 5V tolerant. You need a voltage divider on the ECHO line.

---

## Wiring to ESP32 (with Voltage Divider)

```
HC-SR04          ESP32
  VCC  ────────── VIN (5V from USB)
  GND  ────────── GND
  TRIG ────────── GPIO 5    (3.3V output is fine to drive TRIG)
  ECHO ──[1kΩ]── GPIO 18
            │
          [2kΩ]
            │
           GND
```

The 1kΩ + 2kΩ voltage divider divides 5V ECHO to 3.3V safe for ESP32:
`5V × (2000 / (1000 + 2000)) = 3.33V`

Alternatively, use a 4.7kΩ + 10kΩ pair — same ratio, just higher impedance.

| HC-SR04 Pin | Connection              | ESP32 Pin |
|-------------|-------------------------|-----------|
| VCC         | 5V (VIN on ESP32 board) | VIN       |
| GND         | Ground                  | GND       |
| TRIG        | Direct                  | GPIO 5    |
| ECHO        | Via 1kΩ+2kΩ divider     | GPIO 18   |

---

## Code Example 1 — Basic Distance Reading

```cpp
#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

long measureDistanceCm() {
  // Send 10µs trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo duration (timeout after 30ms = ~5m, no object)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) return -1;   // timeout — no object detected

  return duration / 58.2;   // convert µs to cm
}

void loop() {
  long dist = measureDistanceCm();

  if (dist == -1) {
    Serial.println("No object detected (> 4m)");
  } else {
    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.println(" cm");
  }

  delay(200);
}
```

---

## Code Example 2 — Averaged + Stable Reading

Raw readings jump around by ±2–5 cm. Averaging 5 readings smooths this significantly.

```cpp
#define TRIG_PIN 5
#define ECHO_PIN 18
#define NUM_SAMPLES 5

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

long singleMeasure() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return duration / 58.2;
}

float averagedDistance() {
  long readings[NUM_SAMPLES];
  int validCount = 0;
  long sum = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    long d = singleMeasure();
    if (d > 0 && d <= 400) {
      readings[validCount++] = d;
      sum += d;
    }
    delay(20);   // HC-SR04 needs ~20ms between measurements
  }

  if (validCount == 0) return -1;
  return (float)sum / validCount;
}

void loop() {
  float dist = averagedDistance();

  if (dist < 0) {
    Serial.println("No object in range");
  } else {
    Serial.print("Distance: ");
    Serial.print(dist, 1);
    Serial.println(" cm");
  }

  delay(100);
}
```

---

## Code Example 3 — Drone Landing Assist

Detects altitude in the final meters and signals when to cut throttle.

```cpp
#define TRIG_PIN 5
#define ECHO_PIN 18

// Landing thresholds
#define LAND_APPROACH_CM  150   // 1.5m: slow descent
#define LAND_FLARE_CM      30   // 30cm: slow further
#define LAND_CUTOFF_CM     10   // 10cm: cut motors

enum LandingState { FLYING, APPROACH, FLARE, TOUCHDOWN };

LandingState landingState = FLYING;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

long measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long d = pulseIn(ECHO_PIN, HIGH, 30000);
  return (d == 0) ? -1 : d / 58.2;
}

void loop() {
  long dist = measureDistanceCm();

  if (dist < 0) {
    Serial.println("Out of range");
    delay(100);
    return;
  }

  // State machine for landing
  if (dist > LAND_APPROACH_CM) {
    landingState = FLYING;
    Serial.print(dist); Serial.println(" cm — FLYING");
  } else if (dist > LAND_FLARE_CM) {
    landingState = APPROACH;
    Serial.print(dist); Serial.println(" cm — APPROACH: slow descent");
    // In real drone: reduce throttle target by 20%
  } else if (dist > LAND_CUTOFF_CM) {
    landingState = FLARE;
    Serial.print(dist); Serial.println(" cm — FLARE: very slow");
    // In real drone: reduce throttle target to near-hover minimum
  } else {
    landingState = TOUCHDOWN;
    Serial.print(dist); Serial.println(" cm — TOUCHDOWN: cut motors");
    // In real drone: set throttle to 0, disarm
  }

  delay(100);   // 10 Hz — sufficient for landing
}
```

---

## Measurement Rate Limit

The HC-SR04 needs at least **60 ms** between measurements to allow the sound to fully decay. Firing again sooner can pick up the previous echo.

```
Safe: measure every 60ms = ~16 Hz max
Good: measure every 100ms = 10 Hz (safer margin)
```

For a landing drone descending at 0.5 m/s, 10 Hz gives a position update every 5 cm — more than enough resolution.

---

## Beam Angle and Mounting

The HC-SR04 has a ~15° cone beam. Objects within that cone reflect back.

```
At 100cm distance, beam covers a circle of ~26cm diameter
At 50cm distance, beam covers a circle of ~13cm diameter
```

**Mount facing straight down** for landing. Angled mounting reads a diagonal distance, not true altitude.

Avoid mounting near:
- Motor propellers (vibration causes false echoes)
- Landing gear (constant false close reading)
- Foam or soft surfaces (absorb sound, weak or no echo)

---

## Temperature Compensation (Optional)

Speed of sound changes with temperature: `343 + 0.606 × (T - 20)` m/s

If you have a BMP280 (which gives temperature), you can compensate:

```cpp
float speedOfSound(float tempC) {
  return 331.3 + 0.606 * tempC;   // m/s
}

float distanceCm(long echoUs, float tempC) {
  float sos = speedOfSound(tempC);   // m/s
  return (echoUs * sos / 2.0) / 10000.0;  // µs → cm
}
```

At 20°C: 343 m/s → standard formula
At 40°C: 355 m/s → ~3.5% faster → uncorrected reading would be ~3.5% short

For most landing use cases this is negligible, but it's good to know.

---

## Common Problems

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| Always reads -1 (no object) | ECHO not wired or 5V kills ESP32 GPIO | Add voltage divider on ECHO |
| ESP32 resets or behaves erratically | ECHO connected directly at 5V | 5V damaged GPIO — add divider, may need to replace ESP32 |
| Reading jumps ±10–20 cm | No averaging, surface is uneven | Average 5 readings, use flat surface |
| Reads 2–3 cm when nothing is there | Sensor too close to a surface | Move sensor away from mounting surface |
| No echo from soft targets (foam, fabric) | Material absorbs ultrasound | HC-SR04 works best on hard, flat surfaces |
| Reading freezes at one value | `pulseIn` blocking too long | Use `pulseIn` with timeout, or non-blocking version below |

---

## Non-Blocking Version (for Real Flight Controllers)

`pulseIn()` blocks the CPU for up to 30ms waiting for the echo. A flight controller cannot stop for 30ms. Use interrupts instead:

```cpp
#define TRIG_PIN 5
#define ECHO_PIN 18

volatile long echoStart = 0;
volatile long echoEnd   = 0;
volatile bool newReading = false;

void IRAM_ATTR echoISR() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    echoStart = micros();
  } else {
    echoEnd = micros();
    newReading = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);
}

unsigned long lastTrig = 0;

void loop() {
  // Trigger every 100ms without blocking
  if (millis() - lastTrig > 100) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    lastTrig = millis();
  }

  // Read result whenever echo comes back
  if (newReading) {
    newReading = false;
    long duration = echoEnd - echoStart;
    float dist = duration / 58.2;
    Serial.print("Distance: "); Serial.print(dist); Serial.println(" cm");
  }

  // Flight controller PID loop can run here without blocking
}
```

---

## What to Try

1. Run Example 1 → move your hand toward/away from the sensor, watch values change
2. Try a soft target (pillow, foam) vs hard (wall, book) — compare consistency
3. Run the averaged version — note how much smoother the readings are
4. Mount facing the floor and lift the sensor — verify altitude reading in cm
5. Run the landing state machine and slowly lower the sensor — step through FLYING → APPROACH → FLARE → TOUCHDOWN

---

## Next Steps

- **05_sensors_and_modules/07_adxl345_accelerometer.md** — detect landing impact via acceleration spike
- **07_drone_electronics** — integrate ultrasonic landing assist into the full flight stack
