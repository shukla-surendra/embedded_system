# 02 — Ohm's Law

Ohm's Law is the single most important formula in electronics. If you understand one thing before picking up a multimeter, make it this.

---

## The Formula

```
V = I × R

V = Voltage (Volts)
I = Current (Amps)
R = Resistance (Ohms)
```

Three variables. Know any two and you can find the third:

```
V = I × R        (voltage)
I = V / R        (current)
R = V / I        (resistance)
```

A memory trick — the **VIR triangle**:

```
      [ V ]
      -----
      I | R
```

Cover the one you want to find; what's left is the formula:
- Cover V → I × R
- Cover I → V / R  
- Cover R → V / I

---

## Example 1 — LED Resistor Calculation

This is the most common real-world use of Ohm's Law for beginners.

**Problem:** You want to connect an LED to your ESP32's 3.3V output pin. The LED needs 2V across it and should only get 10mA of current. What resistor do you need?

**Step 1 — Find the voltage across the resistor**

The total voltage (3.3V) is split between the LED (2V) and the resistor. So:

```
V_resistor = 3.3V - 2V = 1.3V
```

**Step 2 — Apply Ohm's Law**

```
R = V / I
R = 1.3V / 0.010A      (10mA = 0.010A)
R = 130 Ω
```

**Step 3 — Pick a standard resistor value**

Resistors come in standard values. 130Ω isn't a standard value, so pick the next higher standard value: **150Ω** (or 220Ω for even more safety margin).

> Using a slightly higher resistor is fine — the LED will be a tiny bit dimmer but won't burn out. Using a lower resistor can destroy the LED.

---

## Example 2 — Finding Current

**Problem:** You connect a 330Ω resistor to a 5V supply. How much current flows?

```
I = V / R
I = 5V / 330Ω
I = 0.0151 A = 15.1 mA
```

---

## Example 3 — Checking if a Motor Will Work

**Problem:** A small DC motor is rated for 6V and has a coil resistance of 12Ω. How much current will it draw at startup?

```
I = V / R
I = 6V / 12Ω
I = 0.5A = 500mA
```

Your ESP32 GPIO pins can only supply ~12mA. You **cannot** drive this motor directly from a GPIO pin — you need a motor driver (covered later). This kind of check prevents you from burning out your microcontroller.

---

## Power: The Fourth Variable

There's one more formula that comes from Ohm's Law:

```
P = V × I

P = Power (Watts)
```

Power tells you how much heat a component generates. Exceed a component's power rating and it will overheat and fail.

**Example:** The 330Ω resistor from Example 2 dissipates:
```
P = 5V × 0.0151A = 0.075W = 75mW
```

A standard ¼W (250mW) resistor handles this easily. No problem.

**Example:** A 10Ω resistor connected to 12V:
```
I = 12/10 = 1.2A
P = 12V × 1.2A = 14.4W
```

A ¼W resistor would instantly burn out. You'd need a 25W resistor (and it would get very hot).

---

## Voltage, Current, and Resistance Interact

Change one variable and the others respond:

| Change | Effect |
|--------|--------|
| Increase voltage, same resistance | Current increases |
| Increase resistance, same voltage | Current decreases |
| Short circuit (R → 0) | Current → very large (dangerous) |

---

## Practice

Work these out by hand. Use a calculator if needed.

1. An LED needs 20mA and has a 2V forward voltage. You're powering it from a 5V supply. What resistor value do you need? (Round up to the nearest standard value: 10, 22, 33, 47, 68, 100, 150, 220, 330, 470, 1000 Ω)

2. A 470Ω resistor is connected to 3.3V. How much current flows through it?

3. A circuit draws 50mA from a 5V source. What is its effective resistance?

4. A 100Ω resistor connected to 3.3V — does a standard ¼W (0.25W) resistor have enough power rating to handle this safely?

**Answers (check your work):**
1. V_r = 5-2 = 3V, R = 3/0.020 = 150Ω — exactly 150Ω works
2. I = 3.3/470 = 7.02mA
3. R = 5/0.050 = 100Ω
4. P = (3.3²)/100 = 0.109W — yes, 0.109W < 0.25W, safe

---

**Next:** [03 — Basic Components](03_basic_components.md)
