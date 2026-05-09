# 10 — MOSFETs: Complete Reference

## What a MOSFET Actually Does

A **MOSFET** (Metal-Oxide-Semiconductor Field-Effect Transistor) is a voltage-controlled switch. Unlike a BJT (which needs base current), a MOSFET turns on and off based purely on the **voltage applied to its Gate**.

```
Three terminals:

  G — Gate     (control input — voltage, no sustained current)
  D — Drain    (high-side current path)
  S — Source   (low-side current path)

N-channel (most common):
        D
        │
   G ───┤   Vgs > threshold → D-S channel opens → current flows D→S
        │
        S

P-channel:
        S
        │
   G ───┤   Vgs < −threshold → channel opens → current flows S→D
        │
        D
```

The gate is insulated from the channel by a thin oxide layer — **essentially infinite input impedance**. No current flows into the gate in steady state. This makes MOSFETs ideal for microcontroller-driven switching: the ESP32 GPIO pin does not need to supply any sustained current.

---

## N-Channel vs P-Channel

| Property | N-Channel | P-Channel |
|----------|-----------|-----------|
| Channel opens when | Vgs **above** threshold | Vgs **below** threshold |
| Switch position | **Low-side** (source to GND) | High-side (source to VCC) |
| On-resistance (RDS_on) | Lower = better performance | Higher than N-ch equivalent |
| Most common | **Yes** — use N-ch by default | Less common |
| Drive from ESP32 | Yes (if logic-level) | More complex — needs level shift |

For 99% of ESP32 projects: use **N-channel, low-side switch**.

---

## Enhancement vs Depletion Mode

**Enhancement mode** (standard): OFF by default. Gate voltage turns it ON. This is what you buy when you buy "a MOSFET."

**Depletion mode**: ON by default. Gate voltage turns it OFF. Used in specific circuit topologies — not common in hobby electronics.

---

## Key MOSFET Parameters

### VGS(th) — Gate Threshold Voltage

The minimum gate-to-source voltage needed to start conducting. Two important ranges:

```
Standard MOSFETs: VGS(th) = 4V–10V
  → Need 10V gate drive to fully turn on
  → ESP32's 3.3V GPIO cannot drive these directly

Logic-level MOSFETs: VGS(th) = 1V–2.5V
  → Specified to be fully on at VGS = 4.5V or lower
  → ESP32's 3.3V GPIO drives these directly ← use these
```

**Look for** in datasheet: RDS(on) specified at VGS = 4.5V or VGS = 3.3V → that is a logic-level MOSFET.

### RDS(on) — On-State Drain-Source Resistance

When fully on, the MOSFET is not a perfect short — there is a small resistance. This causes power dissipation:

```
P_on = IC² × RDS(on)

IRLZ44N: RDS(on) = 22mΩ (0.022Ω) at VGS = 5V
At 5A:  P = 5² × 0.022 = 0.55W — barely warm

Cheap standard MOSFET: RDS(on) = 500mΩ
At 5A:  P = 5² × 0.5 = 12.5W — needs large heatsink
```

Lower RDS(on) = better efficiency, less heat. This is the most important figure of merit.

### ID(max) — Maximum Drain Current

Continuous current the MOSFET can handle. Derated by temperature — at 100°C, the rating drops significantly. Always use a device rated 2–3× your actual current.

### VDSS — Drain-Source Breakdown Voltage

Maximum voltage from drain to source when off. The MOSFET must withstand the full supply voltage plus any inductive spikes. Use at least 2× your supply voltage.

### Gate Charge (Qg)

The charge needed to switch the gate from off to on. This determines how fast you can switch and how much current the driver needs during switching.

```
At 100kHz PWM, Qg = 50nC:
Average gate current = Qg × frequency = 50nC × 100,000 = 5mA

ESP32 GPIO can supply ~12mA — adequate for most logic-level MOSFETs at 100kHz.
At higher frequencies or larger Qg: use a gate driver IC.
```

---

## Common MOSFETs for This Project

### IRLZ44N (N-channel, through-hole)

| Spec | Value |
|------|-------|
| VGS(th) | 1.0–2.0V |
| RDS(on) | 22mΩ at VGS = 10V, ~35mΩ at VGS = 5V |
| ID max | 47A (30A practical with heatsink) |
| VDSS | 55V |
| Package | TO-220 |
| Logic-level | **Yes** — works with 3.3V and 5V GPIO |

**The go-to choice for this project.** Drives DC motors, LED strips, solenoids, large loads. Fully on with ESP32's 3.3V GPIO.

### IRF520N (N-channel, through-hole)

| Spec | Value |
|------|-------|
| VGS(th) | 2.0–4.0V |
| RDS(on) | 270mΩ at VGS = 5V |
| ID max | 9.7A |
| VDSS | 100V |
| Package | TO-220 |
| Logic-level | Marginal at 3.3V — better with 5V gate |

### 2N7000 (N-channel, small signal, TO-92)

| Spec | Value |
|------|-------|
| VGS(th) | 0.8–3.0V |
| RDS(on) | 1.8Ω at VGS = 5V |
| ID max | 200mA |
| VDSS | 60V |
| Package | TO-92 (same shape as BJT) |
| Logic-level | Yes |

Small-signal switching — LED control, relay coils. Same size as a small BJT, simpler to drive.

### AO3400 (N-channel SMD, SOT-23)

| Spec | Value |
|------|-------|
| VGS(th) | 0.45–1.0V |
| RDS(on) | 50mΩ at VGS = 4.5V |
| ID max | 5.7A |
| VDSS | 30V |
| Package | SOT-23 (SMD) |
| Logic-level | Yes |

Excellent SMD choice for PCB designs.

### IRF4905 (P-channel, TO-220)

| Spec | Value |
|------|-------|
| VGS(th) | −2.0V to −4.0V |
| RDS(on) | 20mΩ at VGS = −10V |
| ID max | 74A |
| VDSS | −55V |
| Package | TO-220 |
| Use | High-side switch when paired with N-channel |

---

## TO-220 Package Pin Identification

```
Looking at front (the side with the part number printed):

  G    D    S
  │    │    │
  └────┼────┘
       │
  (metal tab = Drain on most N-channel MOSFETs)
```

> **Verify pinout from your specific datasheet.** The standard for N-channel TO-220 is Gate–Drain–Source left to right (flat face toward you), but some devices differ.

---

## Basic N-Channel Low-Side Switch Circuit

```
+VCC (battery / supply)
     │
  [Load]         ← motor, LED strip, relay
     │
   Drain
G ──┤   IRLZ44N
   Source
     │
    GND

ESP32 GPIO ──[optional 100Ω]──► Gate

When GPIO = 3.3V (HIGH): Gate turns on → Drain-Source conducts → Load gets current
When GPIO = 0V (LOW):    Gate off → no current through load
```

The 100Ω gate resistor limits inrush current during switching and prevents oscillation. Optional at low frequencies, recommended for PWM.

---

## Gate Pull-Down Resistor — Critical

When the ESP32 boots, GPIO pins float briefly before your code sets their state. A floating gate can turn the MOSFET on accidentally.

```
+VCC
  │
[Load]
  │
Drain
ESP32 GPIO ──[100Ω]──── Gate ──[10kΩ]── GND
                                         ↑
                        Pull-down: holds Gate LOW
                        when ESP32 hasn't driven it yet
Source
  │
GND
```

The 10kΩ from Gate to GND ensures the MOSFET stays OFF until the GPIO explicitly drives it HIGH. This is especially important for motor drivers — you don't want motors to spin during ESP32 startup.

---

## PWM Motor Control with MOSFET

The MOSFET switches fast enough to use PWM to vary motor speed:

```cpp
// ESP32 PWM example — control motor speed via MOSFET

const int MOTOR_PIN = 16;
const int PWM_CHANNEL = 0;
const int PWM_FREQ = 20000;   // 20kHz — above human hearing
const int PWM_BITS = 10;      // 0–1023

void setup() {
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_BITS);
  ledcAttachPin(MOTOR_PIN, PWM_CHANNEL);
}

void loop() {
  // Ramp up
  for (int duty = 0; duty <= 1023; duty += 10) {
    ledcWrite(PWM_CHANNEL, duty);
    delay(10);
  }
  // Ramp down
  for (int duty = 1023; duty >= 0; duty -= 10) {
    ledcWrite(PWM_CHANNEL, duty);
    delay(10);
  }
}
```

At 20kHz the MOSFET switches 20,000 times per second. At 50% duty cycle the motor sees 6V average from a 12V supply.

**Freewheeling diode is mandatory with motors:**

```
+VCC
  │
[Motor]──┬── Drain
         │
      [1N5819]   ← cathode to +VCC, anode to Drain
         │         Schottky preferred for fast PWM
Gate ────┤
      Source
         │
        GND
```

---

## H-Bridge: Bidirectional Motor Control

Two N-channel MOSFETs alone only switch in one direction. An H-bridge (4 MOSFETs) allows reversing motor direction.

```
+VCC
  │
  ├──[Q1 P-ch]──┬──[Q2 P-ch]──┤
                │              │
               M1─────────── M2    ← motor connections
                │              │
  ├──[Q3 N-ch]──┴──[Q4 N-ch]──┤
  │
 GND

Q1+Q4 ON: current flows M1→M2 (forward)
Q2+Q3 ON: current flows M2→M1 (reverse)
Never Q1+Q3 or Q2+Q4 together → shoot-through → instant destruction
```

In practice, use an integrated H-bridge IC (L298N, TB6612, DRV8833) rather than discrete MOSFETs. These handle the shoot-through protection and dead-time internally.

---

## Logic-Level Drive from ESP32 — Detailed

The ESP32 outputs 3.3V HIGH. For a MOSFET to be fully on:

```
Fully on condition: VGS >= VGS(th) + some margin

IRLZ44N: VGS(th) max = 2.0V, RDS(on) spec at VGS = 5V
At VGS = 3.3V: the IRLZ44N is on, but RDS(on) is higher than at 5V
              datasheet shows RDS(on) ~60mΩ at VGS = 3.3V (vs 22mΩ at 10V)

For 5A load at 3.3V drive:
P = 5² × 0.060 = 1.5W — still manageable with heatsink

For maximum efficiency: drive gate to 5V using a BSS138 level shifter or dedicated gate driver
```

For loads under 3A, the IRLZ44N driven directly by 3.3V ESP32 is fine.

---

## MOSFET vs BJT Comparison for Switching

| Scenario | Best Choice | Reason |
|----------|-------------|--------|
| LED, buzzer, small relay < 100mA | Either | BJT simpler to source locally |
| DC motor 500mA–5A | **MOSFET** | Lower saturation loss, faster PWM |
| LED strip (constant current, low freq) | Either | BJT adequate |
| Brushless ESC | **MOSFET** | Fast switching, low loss mandatory |
| Very small load, low cost priority | BJT | 2N2222 costs less than IRLZ44N |
| Battery disconnect switch | **MOSFET** | Near-zero quiescent current |

---

## Body Diode — Free Feature

Every MOSFET has an inherent **body diode** between source and drain. In N-channel: anode at source, cathode at drain.

```
N-channel with body diode:

    D
    │
   ─┤>│── S   ← body diode (anode at S, cathode at D)
    │
    G
```

This diode conducts when the drain goes below the source — important in motor control (back-EMF can momentarily reverse the voltage). The body diode acts as a built-in freewheeling diode.

For fast PWM applications: the body diode's reverse recovery is slow. Use an external Schottky in parallel for better performance.

---

## Heat and Thermal Design

```
Power dissipated when fully on:
P = ID² × RDS(on)

IRLZ44N at 10A, 3.3V drive (RDS_on ≈ 60mΩ):
P = 100 × 0.060 = 6W

TO-220 thermal resistance (junction to case): 1.4°C/W
TO-220 thermal resistance (case to ambient, no heatsink): ~60°C/W

Total: 6W × (1.4 + 60) = 368°C rise above ambient → junction fails at ~175°C
→ heatsink mandatory above ~2W

With heatsink (5°C/W):
6W × (1.4 + 5) = 38°C rise → 38 + 25 = 63°C junction — fine
```

**Rule:** if the MOSFET is too hot to touch (60°C = ouch) — it needs a heatsink or a lower RDS(on) device.

---

## ESD — Gate Is Fragile

The thin gate oxide can be destroyed by static electricity — voltages as low as 100V (from touching it with your finger) can punch through the gate irreversibly.

**Handling rules:**
- Store in anti-static bags (the dark foam)
- Touch a grounded metal object before handling
- Never leave the gate floating in a circuit — always connect to GND via pull-down
- When soldering, use an ESD-safe iron or let the board discharge through the solder pads

---

## Common Problems

| Problem | Cause | Fix |
|---------|-------|-----|
| MOSFET always on | Gate floating or driven by non-logic-level signal | Add 10kΩ gate pull-down |
| MOSFET doesn't turn on | Standard MOSFET driven by 3.3V (VGS too low) | Use logic-level MOSFET (IRLZ44N) |
| MOSFET gets very hot | RDS(on) too high or overcurrent | Use lower RDS(on) device or add heatsink |
| Motor runs at startup before code runs | No gate pull-down | Add 10kΩ resistor from gate to GND |
| MOSFET fails with inductive load | No freewheeling diode | Add 1N5819 across load |
| MOSFET blows immediately | Shoot-through in H-bridge | Never turn on both high-side and low-side at same time |
| Only partially on at low load, gets hot at high load | Driving standard MOSFET at 3.3V | Replace with logic-level type |

---

## Full Circuit: ESP32 → IRLZ44N → DC Motor

```
+12V ──────────────────────── Motor (+)
                               │
                           Motor (−)
                               │
                             Drain ───────── [1N5819 cathode → +12V]
                             │               [1N5819 anode  → Drain]
ESP32 GPIO16 ──[100Ω]──── Gate
                         [10kΩ] ─── GND      ← pull-down
                             │
                           Source
                             │
                            GND ─────────── Battery (−)
```

Code:
```cpp
const int MOTOR_PIN = 16;

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);  // motor off at start
}

void loop() {
  digitalWrite(MOTOR_PIN, HIGH);  // motor full on
  delay(2000);
  digitalWrite(MOTOR_PIN, LOW);   // motor off
  delay(2000);
}
```

---

## Practice Questions

1. You want to switch a 12V, 3A DC motor from an ESP32 GPIO. Name a suitable MOSFET and explain why it works with 3.3V gate drive.

2. What is the power dissipated in an IRLZ44N (RDS_on = 35mΩ at 3.3V drive) switching 4A?

3. Why is a 10kΩ pull-down resistor on the gate important in a motor controller?

4. A standard IRF540N has VGS(th) = 4V. Can it be driven directly by an ESP32 GPIO outputting 3.3V? What happens if you try?

**Answers:**

1. **IRLZ44N** — it is a logic-level MOSFET with VGS(th) max = 2V and RDS(on) specified at VGS = 5V. At 3.3V gate drive, it turns on fully (VGS = 3.3V >> 2V threshold). With RDS(on) ≈ 35mΩ at 3.3V, it dissipates 3² × 0.035 = 0.315W at 3A — no heatsink needed.

2. P = ID² × RDS(on) = 4² × 0.035 = 16 × 0.035 = **0.56W** — MOSFET will be warm but no heatsink needed.

3. During ESP32 boot, GPIO pins float briefly. Without the pull-down, the gate floats at an unknown voltage and the motor may spin unexpectedly — dangerous for a drone. The 10kΩ resistor holds the gate at 0V (OFF) until the firmware explicitly drives it HIGH.

4. No — the IRF540N's threshold is 4V, so at 3.3V the transistor is only partially on (in the linear region). It does not saturate, meaning RDS(on) is very high (several ohms instead of milliohms). The MOSFET dissipates far more power than expected, heats up rapidly, and may fail. Always verify VGS(th) and RDS(on) specs at the actual drive voltage.
