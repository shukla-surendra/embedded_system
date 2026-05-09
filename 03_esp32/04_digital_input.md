# 04 — Digital Input: Reading a Button

## Circuit

```
ESP32 GPIO18 ──── Button ──── GND
(Internal pull-up enabled in code — GPIO18 reads HIGH normally, LOW when pressed)
```

No external resistor needed — the ESP32 has built-in pull-up resistors.

---

## Code — Read a Button

```cpp
#define BUTTON_PIN 18
#define LED_PIN    4

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Enable internal pull-up
    // Pin is HIGH by default, goes LOW when button pressed to GND
}

void loop() {
    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW) {      // LOW = pressed (pull-up logic)
        Serial.println("Button pressed!");
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}
```

---

## Understanding Pull-ups

With `INPUT_PULLUP`:
```
Internal:  3.3V ──[~47kΩ]──┬── GPIO18
                            │
                           [Button]
                            │
                           GND

Not pressed: GPIO18 = HIGH (3.3V, pulled up)
Pressed:     GPIO18 = LOW  (shorted to GND through button)
```

This is **active-low** logic — the action (press) produces LOW. Very common in embedded systems.

---

## The Bouncing Problem

Mechanical buttons don't cleanly switch from LOW to HIGH. They "bounce" — rapidly flickering between states for 5–50ms as the metal contacts settle.

```
Ideal:      _____|‾‾‾‾‾‾‾‾‾‾‾‾‾|_____
Real:       _____||‾|_|‾‾‾‾‾‾|_|_____
                  ↑ bouncing
```

Without debouncing, one physical press may register as many presses.

### Software debounce

```cpp
#define BUTTON_PIN 18
#define LED_PIN    4
#define DEBOUNCE_MS 50

bool ledOn = false;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    bool reading = digitalRead(BUTTON_PIN);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();   // reset the debounce timer
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
        // State has been stable for 50ms — it's a real change
        if (reading == LOW && lastButtonState == HIGH) {
            // Falling edge = button just pressed
            ledOn = !ledOn;
            digitalWrite(LED_PIN, ledOn);
            Serial.println(ledOn ? "LED on" : "LED off");
        }
    }

    lastButtonState = reading;
}
```

Each press of the button now toggles the LED — and bounces are ignored.

---

## Practice

1. Wire a button between GPIO18 and GND. Confirm the LED on GPIO4 turns on while you hold the button.

2. Modify the code so the LED stays on after one press and turns off on the next press (toggle behavior), with proper debouncing.

3. Add a second button on GPIO19 that turns the LED on, and use the first button to turn it off (two-button control).

---

**Next:** [05 — Analog Input: ADC](05_analog_input.md)
