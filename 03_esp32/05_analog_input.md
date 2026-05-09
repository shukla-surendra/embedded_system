# 05 — Analog Input: ADC

Digital signals are HIGH or LOW. But the real world is analog — temperature, battery voltage, joystick position, light level are all continuous values.

The **ADC (Analog-to-Digital Converter)** measures voltage and converts it to a number your code can work with.

---

## How ADC Works

The ESP32's ADC is 12-bit. It maps voltage to a number:

```
0V    →    0
1.65V →    2047  (midpoint)
3.3V  →    4095

Formula: ADC_value = (voltage / 3.3) × 4095
Reverse: voltage = (ADC_value / 4095) × 3.3
```

---

## Circuit — Potentiometer

A potentiometer (pot) is a variable resistor with a wiper. Rotating the knob moves the wiper between GND and VCC, producing a variable voltage output. Perfect for testing ADC.

```
3.3V ──[Pot end 1]──[wiper (middle pin)]──[Pot end 2]── GND
                          │
                        GPIO34 (ADC input)
```

Wiring:
- Pot left pin → ESP32 3.3V
- Pot right pin → ESP32 GND
- Pot middle (wiper) pin → GPIO34

---

## Code — Read Potentiometer

```cpp
#define POT_PIN 34   // ADC1 channel, safe with WiFi

void setup() {
    Serial.begin(115200);
    // No pinMode needed for analog input — GPIO34 is input-only
}

void loop() {
    int rawValue = analogRead(POT_PIN);           // 0–4095
    float voltage = (rawValue / 4095.0) * 3.3;   // convert to volts

    Serial.print("Raw: ");
    Serial.print(rawValue);
    Serial.print("  |  Voltage: ");
    Serial.print(voltage, 2);    // 2 decimal places
    Serial.println(" V");

    delay(100);   // Read 10 times per second
}
```

Open Serial Monitor at 115200 baud and turn the potentiometer. You should see values change from ~0 to ~4095.

---

## ADC Non-Linearity Warning

The ESP32's ADC is not perfectly linear — it's slightly inaccurate near 0V and 3.3V. For precise measurements, stay in the 0.1V–3.1V range, or use calibration.

For rough measurements (battery monitoring, joystick position), it's fine.

---

## Practical Example — Battery Voltage Monitor

A 2S LiPo battery ranges from 6.0V (discharged) to 8.4V (full). You can't connect 8.4V directly to a 3.3V ADC input — it would damage the ESP32.

Use a **voltage divider** to scale it down:

```
Battery+ ──[R1: 100kΩ]──┬── GPIO35
                        │
                    [R2: 47kΩ]
                        │
                       GND

Output voltage = Battery_V × R2 / (R1 + R2)
At full charge: 8.4V × 47000/147000 = 2.69V  ← safe for ESP32
At discharged:  6.0V × 47000/147000 = 1.92V
```

```cpp
#define BATT_PIN 35
#define R1 100000.0   // 100kΩ
#define R2 47000.0    // 47kΩ

void setup() {
    Serial.begin(115200);
}

void loop() {
    int raw = analogRead(BATT_PIN);
    float adcVoltage = (raw / 4095.0) * 3.3;
    float batteryVoltage = adcVoltage * (R1 + R2) / R2;  // reverse the divider

    Serial.print("Battery: ");
    Serial.print(batteryVoltage, 2);
    Serial.println(" V");

    if (batteryVoltage < 6.8) {
        Serial.println("WARNING: Battery low!");
    }

    delay(1000);
}
```

This pattern — voltage divider + ADC — is exactly how drone flight controllers monitor battery voltage.

---

## Practice

1. Wire a potentiometer to GPIO34. Read values and confirm they span 0–4095 through the full rotation.

2. Map the ADC reading to an LED brightness using `analogWrite()` (covered in the next module) — so the LED dims as you turn the pot.

3. Calculate the resistor values for a voltage divider that safely measures a 3S LiPo (7.4V–12.6V) with the ESP32 ADC.

**Answer to #3:**
- Need max output ≤ 3.1V when input is 12.6V
- Ratio needed: 3.1/12.6 = 0.246
- Use R2/(R1+R2) = 0.246 → try R1=220kΩ, R2=68kΩ → 68/(220+68) = 0.236 → at 12.6V = 2.97V ✓

---

**Next:** [06 — PWM: Dimming LEDs & Servo Control](06_pwm.md)
