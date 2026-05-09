# 03 — Hello World: Blink an LED

Blink is the "Hello World" of embedded systems. It verifies your toolchain, your board, and your wiring all at once.

---

## Circuit

Wire an external LED (more instructive than using the onboard one):

```
ESP32 GPIO4 ──── [220Ω resistor] ──── LED anode (+) ──── LED cathode (−) ──── GND
```

On a breadboard:
```
1. Jumper wire from GPIO4 to breadboard row 10, column a
2. 220Ω resistor: one leg row 10 column c, other leg row 10 column e  (bridge the gap)
   Wait — the center gap separates left/right halves!
   Better: one leg row 10a, other leg row 12a (same column, 2 rows down)
3. LED: anode (long leg) row 12c, cathode (short leg) row 14c
4. Jumper from row 14c to GND rail (black wire)
5. GND rail to ESP32 GND pin
```

Or more simply:
```
GPIO4 → resistor → LED → GND (all in series, one after another in the breadboard)
```

---

## The Code

```cpp
// Blink — turn an LED on and off with a 1-second interval

#define LED_PIN 4   // GPIO4

void setup() {
    pinMode(LED_PIN, OUTPUT);   // Tell the ESP32 this pin is an output
}

void loop() {
    digitalWrite(LED_PIN, HIGH);  // Set pin to 3.3V → LED on
    delay(1000);                  // Wait 1000 milliseconds (1 second)
    digitalWrite(LED_PIN, LOW);   // Set pin to 0V → LED off
    delay(1000);                  // Wait 1 second
}
```

Copy this into the Arduino IDE, upload, and the external LED should blink.

---

## Understanding the Code

### `#define LED_PIN 4`
A preprocessor constant — every time the compiler sees `LED_PIN` it replaces it with `4`. This makes your code easier to read and change — if you move the LED to GPIO13, you change one line.

### `pinMode(pin, mode)`
Configures a GPIO pin. Modes:
- `OUTPUT` — pin actively drives HIGH or LOW
- `INPUT` — pin reads external signal (high impedance, floating)
- `INPUT_PULLUP` — input with internal pull-up resistor enabled (default HIGH, LOW when grounded)
- `INPUT_PULLDOWN` — input with internal pull-down resistor (default LOW, HIGH when connected to 3.3V)

### `digitalWrite(pin, value)`
Sets an output pin to:
- `HIGH` → 3.3V
- `LOW` → 0V

### `delay(milliseconds)`
Pauses the program for the given number of milliseconds. During this time, **the CPU does nothing** — it just waits. This is fine for simple programs but becomes a problem in real-time applications (covered later).

---

## Experiment: Multiple Blink Rates

Wire up 3 LEDs on GPIO4, GPIO5, and GPIO18. Blink them at different rates:

```cpp
#define LED1 4
#define LED2 5
#define LED3 18

void setup() {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
}

void loop() {
    // LED1: fast blink (200ms)
    digitalWrite(LED1, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(200);

    // LED2: medium blink (500ms)
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED2, LOW);
    delay(500);

    // LED3: slow blink (1000ms)
    digitalWrite(LED3, HIGH);
    delay(1000);
    digitalWrite(LED3, LOW);
    delay(1000);
}
```

Upload and observe — but notice that only one LED changes at a time because `delay()` blocks everything. Real projects need non-blocking timing.

---

## Non-Blocking Blink (The Right Way)

`delay()` is a problem in real firmware — while it waits, you can't read sensors or respond to inputs. The solution is to check elapsed time instead:

```cpp
#define LED_PIN 4

unsigned long previousMillis = 0;  // stores the last time LED toggled
const long interval = 1000;        // interval in milliseconds
bool ledState = LOW;

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    unsigned long currentMillis = millis();  // milliseconds since boot

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;      // save the time
        ledState = !ledState;                // toggle state
        digitalWrite(LED_PIN, ledState);
    }

    // Other code here runs EVERY loop iteration, not blocked by LED timing
}
```

`millis()` returns the number of milliseconds since the ESP32 booted. By comparing elapsed time, the LED toggles at the right interval while the rest of `loop()` continues running.

This pattern — **"check if enough time has passed, then do the thing"** — is used everywhere in embedded firmware.

---

## Adding Serial Output

Always add Serial output to understand what your code is doing:

```cpp
#define LED_PIN 4

void setup() {
    Serial.begin(115200);          // Start serial at 115200 baud
    Serial.println("ESP32 booted, starting blink...");
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
    delay(1000);

    Serial.println("LED OFF");
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}
```

Open Serial Monitor (115200 baud) and watch the messages. This is how you debug every program.

---

## Common Problems

| Symptom | Likely cause |
|---------|-------------|
| LED doesn't light at all | Wrong pin, LED in backwards, no resistor connection, wrong breadboard row |
| LED stays on (doesn't blink) | Only `HIGH` line reached — check for compile errors, delay issue |
| LED very dim | Resistor too high (try lower value like 100Ω) |
| LED very bright, then dies | Resistor too low or missing — replace LED and add resistor |
| Upload succeeds but nothing happens | Check the pin number matches your physical wiring |

---

## Practice

1. Modify the blink code so the LED blinks 3 times fast (100ms), then pauses 2 seconds, then repeats.

2. Rewrite the same pattern using the non-blocking `millis()` approach instead of `delay()`. (Hint: use a state machine — track which "phase" of the pattern you're in.)

3. Use `Serial.println()` to print the current `millis()` value every time the LED changes state. What do you notice about the interval?

---

**Next:** [04 — Digital Input: Reading a Button](04_digital_input.md)
