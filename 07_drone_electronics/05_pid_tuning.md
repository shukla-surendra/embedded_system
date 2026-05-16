# 05 — PID Tuning

## What Is a PID Controller?

PID stands for **Proportional, Integral, Derivative**. It's a control algorithm used everywhere in engineering — thermostats, cruise control, industrial robots, and flight controllers.

In a drone, the PID controller answers: **"The drone is tilting — how hard and fast should I correct it?"**

---

## The Problem PID Solves

Imagine you're trying to hold a drone at exactly 0° (level). The world keeps pushing it off:
- Wind gusts
- Uneven motor performance
- Battery sag during flight

You need a feedback system that constantly measures the error and applies corrective force.

```
Target (setpoint): 0° tilt
Current state:     5° tilt
Error:             5° (too tilted)

PID computes: "Apply correction force X to fix this"
```

---

## The PID Equation

```
Output = (Kp × Error) + (Ki × ∫Error dt) + (Kd × dError/dt)

Where:
  Error     = Setpoint − Current Value
  ∫Error dt = sum of all past errors (integral)
  dError/dt = rate of change of error (derivative)

  Kp = Proportional gain
  Ki = Integral gain
  Kd = Derivative gain
```

These three terms work together. Let's understand each.

---

## P — Proportional Term

**"React to the current error."**

```
P_output = Kp × Error

Error = 10°  → Output = Kp × 10
Error = 5°   → Output = Kp × 5
Error = 0°   → Output = 0
```

Think of it like a spring. The bigger the tilt, the harder it pushes back.

### What Kp Affects

| Kp Too Low | Kp Correct | Kp Too High |
|------------|-----------|-------------|
| Drone is sluggish, slow to correct | Drone snaps back quickly | Drone oscillates rapidly ("buzzing") |
| Won't hold position well | Holds well | May feel stiff or "propwash" |

```
Visualizing Kp effect on step response:

Target ────────────────────────────
                          ___
Kp too high:      /\/\/\/    \___   (overshoots and oscillates)
                 /
Kp correct:   /‾              ‾‾   (reaches target and settles)
             /
Kp too low:  /                      (reaches target slowly)
```

---

## I — Integral Term

**"React to accumulated past error."**

```
I_output = Ki × (sum of all previous errors)
```

The integral grows over time if there's a persistent error the P term can't fix.

### Why We Need I

Imagine the drone hovers slightly to the left even with P at zero stick input. P sees the error and pushes right. But maybe one motor is slightly weaker — no matter how hard P pushes, there's always a small leftover tilt.

I accumulates this error over time and adds a permanent correction bias. It "learns" about mechanical imbalances.

### What Ki Affects

| Ki Too Low | Ki Correct | Ki Too High |
|------------|-----------|-------------|
| Drone drifts slowly | Holds position | Slow wobble/oscillation |
| Doesn't correct steady-state error | No permanent offset | Drone rocks like a boat |

### I Wind-Up

If the drone is held by hand or crashed and can't respond, the integral keeps accumulating ("winding up"). When released, it applies a massive sudden correction. Good firmware includes anti-windup protection (clamps the I term).

---

## D — Derivative Term

**"React to how fast the error is changing."**

```
D_output = Kd × (change in error per time step)

If error is shrinking fast → D adds braking (slows the correction)
If error is growing fast → D adds urgency (speed up correction)
```

Think of D as a damper on a car suspension. P is the spring (pushes toward target), D is the shock absorber (prevents bouncing past the target).

### What Kd Affects

| Kd Too Low | Kd Correct | Kd Too High |
|------------|-----------|-------------|
| Drone overshoots, bouncy | Crisp, quick settle | High-frequency "D-term buzz" |
| Oscillates at medium frequency | No overshoot | Very hot motors |

### D and Noise

The derivative amplifies noise. If the gyro reading has high-frequency noise (from motor vibrations), the D term amplifies that noise into motor commands → motors vibrate → more noise → runaway. This is why D-term filtering is critical in Betaflight.

---

## PID in a Drone Context

For a quadcopter, we run **separate PID loops for each axis**:

```
Roll PID:
  setpoint = pilot's roll stick input (deg/sec in acro mode)
  actual   = gyro X reading (deg/sec)
  error    = setpoint − actual
  output   → motor corrections for roll

Pitch PID:
  setpoint = pilot's pitch stick input (deg/sec)
  actual   = gyro Y reading (deg/sec)
  output   → motor corrections for pitch

Yaw PID:
  setpoint = pilot's yaw stick input (deg/sec)
  actual   = gyro Z reading (deg/sec)
  output   → motor corrections for yaw
```

In Acro mode the setpoint is a desired **rotation rate** (deg/sec).  
In Angle mode there's an outer loop: stick → desired angle → error → desired rate → PID rate loop.

---

## Motor Mixing

After the PID loop outputs corrections for roll, pitch, and yaw, plus throttle from the stick, these are mixed into individual motor commands:

```
Standard quad (X configuration):

  M1 = Throttle + Roll_correction − Pitch_correction − Yaw_correction
  M2 = Throttle − Roll_correction − Pitch_correction + Yaw_correction
  M3 = Throttle + Roll_correction + Pitch_correction + Yaw_correction
  M4 = Throttle − Roll_correction + Pitch_correction − Yaw_correction

         Front
   M1 (CW)   M2 (CCW)
      +Roll    -Roll
      -Pitch   -Pitch
      -Yaw     +Yaw
```

Each motor gets a unique combination of the corrections depending on its position and spin direction.

---

## How to Tune PID

Tuning order: **P first, then D, then I.**

### Step 1 — Set Everything Low

Start with very conservative (low) values. The drone will feel soft but won't oscillate.

```
Betaflight starting values for 5" quad:
  Roll/Pitch P:  40
  Roll/Pitch I:  60
  Roll/Pitch D:  20
  Yaw P:         50
  Yaw I:         60
  Yaw D:         0
```

### Step 2 — Tune P (Roll/Pitch)

Fly in acro mode. Punch throttle and release. Observe how the drone handles.

- **Too low P:** Drone feels mushy, won't snap back from disturbances
- **Too high P:** Drone oscillates (medium frequency shaking on rolls/flips)

Increase P until you start to see oscillations, then back off 10–20%.

### Step 3 — Tune D (Roll/Pitch)

D helps dampen the P-term bounce.

- Increase D until overshoots disappear and the drone feels crisp
- **Too high D:** High-pitched motor whine / buzz, motors get hot

### Step 4 — Tune I

I is usually fine with default values. Adjust only if:
- Drone drifts consistently → increase I
- Drone wobbles slowly like a pendulum → decrease I

### Step 5 — Tune Yaw

Yaw only has P and I (D is usually 0 for yaw — less noise on that axis).
- Increase Yaw P until yaw feels crisp
- Increase Yaw I if yaw drifts when you stop spinning

---

## Reading Oscillations

Learning to identify oscillation type helps you know what to adjust.

```
TYPE 1 — Fast oscillation (buzzing)
  Symptom: Motors make high-pitched buzz, drone feels rigid/stiff
  Cause:   P too high, or D too high
  Fix:     Lower P, or lower D

TYPE 2 — Medium oscillation (bobbing)
  Symptom: Drone rocks back and forth after a roll or pitch
  Cause:   P too high relative to D
  Fix:     Lower P a little, raise D

TYPE 3 — Slow oscillation (floating)
  Symptom: Drone slowly sways side to side
  Cause:   I too high
  Fix:     Lower I

TYPE 4 — Propwash
  Symptom: Drone wobbles after fast descents (flying into its own wash)
  Cause:   P/D balance or filter settings
  Fix:     More complex, usually involves D filter tuning
```

---

## Blackbox Analysis

Best practice: don't tune by feel alone. Use blackbox.

```
Workflow:
  1. Enable Blackbox logging in Betaflight
  2. Fly a short test flight (sharp rolls, throttle punches)
  3. Download log with Betaflight Blackbox Explorer
  4. Plot: Gyro (actual) vs Setpoint (target)

Good tune:           Bad tune (P too high):
  Setpoint ─────         Setpoint ─────
  Gyro ─────         Gyro ────/\/\/\──
  (lines match)          (oscillates around setpoint)
```

---

## Feedforward

Modern Betaflight adds a **feedforward** term (not classic PID). Instead of only reacting to error (PID), feedforward predicts the needed correction by looking at the **rate of change of the setpoint** (how fast you're moving the stick).

```
Classic PID: reacts to error → always has some lag
Feedforward: predicts from stick movement → reduces lag

FF_output = Kff × (change in setpoint per time step)
```

This makes the drone feel more immediately responsive to stick inputs. Common in racing setups.

---

## Code Example — Simple PID Class (Arduino/ESP32)

```cpp
class PID {
  float Kp, Ki, Kd;
  float prev_error;
  float integral;
  float dt;

public:
  PID(float kp, float ki, float kd, float dt_seconds)
    : Kp(kp), Ki(ki), Kd(kd), prev_error(0), integral(0), dt(dt_seconds) {}

  float compute(float setpoint, float measured) {
    float error = setpoint - measured;

    integral += error * dt;

    float derivative = (error - prev_error) / dt;
    prev_error = error;

    return (Kp * error) + (Ki * integral) + (Kd * derivative);
  }

  void reset() {
    integral = 0;
    prev_error = 0;
  }
};

// Usage:
PID roll_pid(40.0f, 60.0f, 20.0f, 0.001f);  // 1ms loop time

// In your main loop (runs every 1ms):
float output = roll_pid.compute(roll_setpoint_dps, gyro_x_dps);
// output is added to/subtracted from motor commands
```

---

## Practice

1. What does the P term respond to? What happens when P is too high?
2. You fly your drone in a hover and it slowly drifts to the left even with no stick input. Which PID term should you increase?
3. Motors are making a high-pitched buzz during flight. What is the most likely cause?
4. Why does the D term cause problems if there's high noise on the gyro signal?
5. In acro mode, the setpoint in the PID loop is a desired __________. In angle mode, the outer loop converts desired angle into desired __________.
6. Given Kp=50, Ki=0, Kd=0, and a roll error of 8°/sec, what is the P_output?
7. What is feedforward and how does it differ from the standard D term?
