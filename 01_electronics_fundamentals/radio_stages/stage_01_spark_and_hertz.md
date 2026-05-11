# Stage 1 — The Spark Gap: How Hertz Proved Radio Waves Exist

---

## What You Will Learn

- Why moving electric charges produce electromagnetic waves (the physical reason, not just the formula)
- How Heinrich Hertz built the first radio transmitter and receiver in 1887 using nothing but wire, metal, and a spark
- How Guglielmo Marconi turned Hertz's laboratory demonstration into long-distance communication
- The Leyden jar as the first practical capacitor — how to build one and what it proves
- The induction coil as a transformer — how to wind one and measure voltage step-up
- Why sparks produce radio waves you can hear on a modern AM radio right now, today, in your workshop
- The physics of why a spark circuit produces a wide band of frequencies (damped oscillation)
- How to calculate capacitance of a Leyden jar and the resonant frequency of an LC circuit

**Prerequisites:** You should know what voltage, current, and resistance are. You should have read the basic components document. You do NOT need calculus — all physics here will be explained using analogies and simple algebra.

**Time required:** 3–4 hours for reading + 2–3 hours for experiments

**Cost:** Under ₹200 / $3 USD for all experiments

---

## Part 1 — The Physical Reason Radio Waves Exist

### Why Does a Moving Charge Radiate?

To understand radio, you must first understand one fundamental fact of physics:

**A stationary charge creates an electric field. A steadily moving charge (a DC current) creates a magnetic field. An ACCELERATING charge — one that is speeding up, slowing down, or changing direction — creates both an electric and a magnetic field that detach from the charge and travel away through space at the speed of light. That detached, self-propagating field is an electromagnetic wave. That is what radio is.**

Let us build the intuition carefully.

Imagine a charged particle sitting still. Around it is an electric field — invisible lines of force pointing outward in all directions, like a porcupine's spines. This field is attached to the charge. Where the charge goes, the field goes with it, instantly — no, wait. Not instantly. Nothing travels faster than light.

Here is the key insight: when you suddenly move the charge, the part of the field far away does not know yet that the charge has moved. The information that the charge has moved travels outward at the speed of light. Between the old field configuration and the new one there is a kink — a disturbance in the field. That kink propagates outward on its own, as a wave.

This is the electromagnetic wave.

If you wiggle the charge back and forth repeatedly — that is, accelerate it back and forth — you produce a continuous train of kinks radiating outward in all directions. The faster you wiggle, the closer together the kinks are, and the higher the frequency of the wave. Wiggle a charge at 1 MHz (one million times per second) and you produce a radio wave with a wavelength of 300 metres.

### The Relationship Between Frequency and Wavelength

The fundamental relationship is:

```
c = f × λ

where:
  c = speed of light = 300,000,000 m/s (3 × 10^8 m/s)
  f = frequency in Hz
  λ = wavelength in metres
```

Examples:

| Frequency | Band | Wavelength |
|-----------|------|------------|
| 1 MHz | AM broadcast (medium wave) | 300 m |
| 7 MHz | Amateur HF (40m band) | 42.8 m |
| 100 MHz | FM broadcast | 3 m |
| 2.4 GHz | WiFi / 2.4GHz drone RC | 12.5 cm |
| 5.8 GHz | FPV video link | 5.2 cm |

Notice that your drone's 2.4 GHz RC link and 5.8 GHz FPV video link are electromagnetic waves — radio waves — produced by rapidly accelerating charges inside the transmitter chip. The same physics Hertz demonstrated in 1887 with a spark gap.

### What Creates the Acceleration?

In Hertz's spark transmitter, and in every radio transmitter, what causes the charge to accelerate repeatedly is an oscillating LC circuit. The charge sloshes back and forth in the LC circuit — first building up on one plate of the capacitor, then rushing through the inductor to the other plate, then back again — at the circuit's resonant frequency. Each reversal of direction is an acceleration event. Each acceleration event radiates a little energy as an electromagnetic wave.

---

## Part 2 — Hertz's Exact Experimental Setup (1887)

Heinrich Hertz did his experiments at the Karlsruhe Institute of Technology in Germany. He was trying to prove Maxwell's mathematical prediction (from 1865) that electromagnetic waves must exist and travel at the speed of light. Maxwell had worked out the maths but could not demonstrate it experimentally. Hertz made it real.

### The Transmitter (Primary Circuit)

Hertz's transmitter consisted of:

```
HIGH VOLTAGE SOURCE
        |
        |
   [INDUCTION COIL]
   (Ruhmkorff coil)
        |
        |
    +---+---+
    |       |
  [Leyden [Leyden
   jar 1]  jar 2]
    |       |
    +---+---+
        |
    SPARK GAP
    (two brass spheres
     separated by ~7mm)
        |
    DIPOLE RODS
    (two 1-metre copper
     rods, collinear)
```

The induction coil (a type of transformer) took 4-volt battery power and stepped it up to around 20,000 to 40,000 volts. This high voltage charged the Leyden jars. When the voltage across the spark gap reached the breakdown voltage of air (about 30,000 V/cm), the air ionised, a spark jumped, and the Leyden jars discharged explosively through the gap.

The discharge was not a simple pulse. It was an oscillation. The energy sloshed back and forth between the Leyden jars (capacitance) and the connecting rods (inductance) at a frequency determined by LC. Each cycle, a little energy was lost to resistance and radiation. The oscillation decayed — a damped oscillation — and the spark gap extinguished after a few dozen cycles. Then the induction coil recharged the jars and the whole process repeated.

The frequency of Hertz's transmitter was approximately 50–100 MHz (the rods were about 1 metre each, giving a half-wave resonance around 150 MHz depending on exact geometry).

### The Receiver (Secondary Circuit)

Hertz's receiver was beautifully simple:

```
    WIRE LOOP
    (about 1 metre diameter,
     made of thick copper wire)
         |
         |
     SMALL GAP
     (two small balls
      almost touching,
      adjustable with a screw)
```

That is all. A wire loop with a tiny gap in it.

When the electromagnetic wave from the transmitter passed through this loop, it induced an oscillating voltage in the loop. If the loop was tuned to the same resonant frequency as the transmitter, the induced voltage was large enough to produce a tiny visible spark at the gap.

Hertz could see sparks at the receiver up to 1.5 metres away. More importantly, he could do controlled experiments:

- If he rotated the receiving loop 90 degrees to the transmitting dipole, the spark disappeared (the fields were orthogonal)
- If he placed a metal sheet between transmitter and receiver, the spark disappeared (absorption/reflection)
- If he placed the receiver around the corner via metal reflectors, the spark reappeared (he bent the beam)

He measured the wavelength by finding nodes and antinodes in a standing wave pattern in his laboratory. From the wavelength and the known oscillation frequency (calculable from the circuit geometry), he could calculate the speed of propagation — and it matched the speed of light.

Maxwell was vindicated. Radio waves exist.

### What Hertz Proved — Summarised

1. Electromagnetic waves propagate through empty space (no medium needed)
2. They travel at the speed of light
3. They reflect, refract, and show interference (just like light, but at longer wavelengths)
4. They are produced by oscillating electric charges
5. They can induce voltages in a remote conductor

---

## Part 3 — Marconi's Improvements (1895–1901)

Hertz had proved radio waves exist but showed no interest in practical communication. Guglielmo Marconi, a young Italian amateur working in his parents' attic in Bologna in 1895, read about Hertz's work and immediately understood its commercial potential.

Marconi made four crucial improvements over Hertz's laboratory setup:

### Improvement 1: The Vertical Antenna + Earth Ground

Hertz used a horizontal dipole — good for controlled lab experiments, bad for omnidirectional transmission. Marconi replaced this with a long vertical wire raised as high as possible, with the bottom connected to an earth ground (a metal plate buried in the ground or dipped in the sea).

**Why this worked:** The earth acts as a mirror. A vertical wire plus its image in the conductive earth forms an effective half-wave dipole. The wave radiates horizontally along the earth's surface — exactly the direction you want for long-distance communication.

**Also:** A taller antenna means more wire, which means more capacitance and inductance, which means lower resonant frequency, which means longer wavelength, which means the wave diffracts better over the curvature of the earth.

Marconi found empirically that every time he made his antenna taller, his transmission range approximately doubled. By 1895 he was communicating across 1.5 km. By 1897 he was communicating 14 km across the Bristol Channel. By 1901 he was transmitting across the Atlantic — 3,500 km.

### Improvement 2: The Tuned LC Circuit

Hertz's transmitter and receiver resonated only very roughly. Marconi added a proper LC resonant circuit — a coil in series with the antenna-earth capacitance — to both transmitter and receiver. This allowed tuning to a specific frequency, which meant:

- The transmitter put most of its energy into a narrow band of frequencies
- The receiver was selective — it responded only to signals near its resonant frequency
- Multiple stations could operate at different frequencies without interfering (in principle — early practice was chaotic)

### Improvement 3: The Coherer Detector

Hertz's spark gap receiver required the operator to stare at a tiny spark in a dark room with a magnifying glass. Marconi used a "coherer" — a glass tube filled with metal filings between two electrodes. In the presence of RF, the filings cohere (stick together, lowering resistance). A battery and relay could then drive a sounder or printer. After each signal, a tapper knocked the tube to de-cohere the filings.

This allowed the reception of Morse code automatically. Hertz's receiver was a laboratory instrument; Marconi's was a communication system.

### Improvement 4: Decoherer + Self-Restoring Circuit

Marconi made the coherer self-resetting by attaching a small hammer that tapped the tube automatically, driven by the relay that the coherer activated. Each signal pulse would cohere the filings, activate the relay, the relay would sound the buzzer AND tap the coherer to reset it, ready for the next pulse.

### Marconi's Timeline

| Year | Achievement |
|------|-------------|
| 1895 | First tests in Bologna attic — 30 metres |
| 1895 | Cross the hill, out of line of sight — proved radio goes over hills |
| 1896 | Arrives in London, patents first system |
| 1897 | 14.4 km across Bristol Channel |
| 1898 | 30 km — English Channel |
| 1899 | 50 km — reported ship positions to newspapers (first commercial use) |
| 1901 | Transatlantic — Poldhu, Cornwall to St. John's, Newfoundland — 3,500 km |

The transatlantic transmission was a single letter S (three dots in Morse) transmitted at 500 kHz (wavelength 600 m). Marconi received it with a kite-lifted antenna 150 metres high. Many scientists of the time did not believe him because they thought radio waves would not follow the curve of the earth. They were wrong. At these low frequencies, the waves follow the ground and also reflect from the ionosphere.

---

## Part 4 — The Leyden Jar: The First Practical Capacitor

### History

The Leyden jar was invented in 1745 in Leiden, Netherlands (independently by Ewald von Kleist and Pieter van Musschenbroek). It was the first device that could store a useful quantity of electric charge. Before its invention, experimenters could generate static electricity but had nowhere to accumulate it.

### Physical Construction

A Leyden jar is simply a capacitor made from a glass jar:

```
         METAL CHAIN
         (inner connection)
              |
              |
         METAL FOIL
         (inner coating,
          inside the jar)
              |
         GLASS WALL
         (the dielectric)
              |
         METAL FOIL
         (outer coating,
          outside the jar)
              |
             GND
         (held by the experimenter
          or connected to earth)
```

The inner foil and outer foil are the two plates of the capacitor. The glass is the dielectric — the insulating material between the plates.

### The Physics of How It Stores Charge

When you connect a high voltage source to the inner electrode (the chain):

1. Positive charges accumulate on the inner foil
2. These positive charges repel positive charges on the outer foil — they are pushed away through the experimenter's hand to ground
3. This leaves negative charges on the outer foil
4. The electric field across the glass holds the charges in place even after you disconnect the charging source

The energy is stored in the ELECTRIC FIELD in the glass, not in the charges themselves. This is a crucial insight. The charges are just the mechanism by which the field is established. The field stores the energy.

When you discharge the jar — by connecting a conductor from inner to outer electrode — the field collapses, releasing all that energy in an instant. With a small jar, this is a nasty shock. With a bank of large jars, it was historically used to kill animals for demonstrations and, in early medical experiments, to attempt to restart stopped hearts.

### The Capacitance Formula

The capacitance of a parallel-plate capacitor (which the Leyden jar approximates) is:

```
         ε₀ × εr × A
    C = ─────────────
               d

Where:
  C  = capacitance in Farads (F)
  ε₀ = permittivity of free space = 8.854 × 10⁻¹² F/m
  εr = relative permittivity of the dielectric (dimensionless)
       Air: 1.0
       Glass: 4–10 (depends on glass type, use 7 as an estimate)
       Mica: 5–8
       Ceramic: 10–10,000 (various types)
  A  = area of the plates in m²
  d  = separation between plates (= thickness of glass wall) in metres
```

**Worked Example — Estimating Your Leyden Jar Capacitance:**

Suppose you use a glass jar with:
- Inner foil area: 10 cm × 10 cm = 0.01 m²
- Glass wall thickness: 3 mm = 0.003 m
- εr of glass: 7 (estimate)

```
         8.854 × 10⁻¹² × 7 × 0.01
    C = ────────────────────────────
                 0.003

         6.198 × 10⁻¹³
    C = ────────────────
             0.003

    C = 2.07 × 10⁻¹⁰ F = 207 pF
```

This is a small capacitance by modern standards — a modern 100 nF capacitor (100,000 pF) holds 500 times as much charge. But for Hertz's experiments, it was more than sufficient.

### Energy Stored in a Capacitor

```
         1
    E = ─── × C × V²
         2

Where:
  E = energy in Joules
  C = capacitance in Farads
  V = voltage in Volts
```

At 30,000 V with C = 207 pF:

```
    E = 0.5 × 207 × 10⁻¹² × (30,000)²
    E = 0.5 × 207 × 10⁻¹² × 9 × 10⁸
    E = 0.5 × 1.863 × 10⁻¹
    E = 0.093 J = 93 mJ
```

93 millijoules. That is roughly the energy of a small firecracker — delivered in a microsecond. A brief, violent event. That is why a spark gap discharge sounds like a crack.

---

## Part 5 — The Induction Coil (Ruhmkorff Coil)

### What It Is

An induction coil is a type of transformer. It uses mutual inductance to step up a low DC voltage to a very high AC voltage. It differs from a mains transformer in that it operates on interrupted DC rather than AC — the interruption creates the changing current that is necessary for transformer action.

### Mutual Inductance

Faraday's law says: a changing magnetic field through a loop of wire induces a voltage in that loop. The induced voltage is proportional to the rate of change of magnetic flux.

```
    V = -N × dΦ/dt

Where:
  V  = induced voltage
  N  = number of turns in the coil
  dΦ/dt = rate of change of magnetic flux through one turn
```

In a transformer with a primary coil (N₁ turns) and secondary coil (N₂ turns) wound on the same magnetic core:

```
    Vsecondary     N₂
    ─────────── = ────
    Vprimary       N₁
```

If N₂/N₁ = 100, then a 12V input becomes 1200V output (approximately — real transformers have losses).

For an induction coil used to generate sparks:
- Primary: 5–10 turns, thick wire (carries large current)
- Secondary: 1000–10000 turns, fine wire (produces high voltage, tiny current)
- The ratio of turns might be 1:1000 — stepping 12V to 12,000V

### The Interrupter

A transformer only works with changing current. An induction coil uses a mechanical interrupter (like a buzzer mechanism) or, in Hertz's era, a vibrating reed to repeatedly make and break the primary circuit. Each make and break creates a pulse of changing current, which induces a pulse of high voltage in the secondary.

The break is actually more useful than the make. When the circuit is broken, the primary current falls to zero very rapidly — a large dΦ/dt — inducing the largest voltage pulse. This is the pulse that charges the Leyden jar.

---

## Part 6 — Spark Gap Physics: Why the Discharge Oscillates

### Air Breakdown and Ionisation

At normal pressure, air is an excellent insulator. But if you apply a sufficiently high electric field (about 30,000 volts per centimetre for dry air at sea level), something changes: free electrons in the air accelerate in the field until they have enough energy to knock electrons off air molecules — ionisation. The knocked-off electrons also accelerate and ionise more molecules. This avalanche process creates a plasma — a conducting channel of ionised gas.

That conducting channel is the spark.

### The LC Ringing After the Spark

Once the spark fires and the gap becomes conducting, the Leyden jar (capacitance C) and the connecting rods/coil (inductance L) form a complete circuit. They oscillate at a frequency determined by:

```
              1
    f₀ = ──────────────
          2π × √(L × C)

Where:
  f₀ = resonant frequency in Hz
  L  = circuit inductance in Henries
  C  = circuit capacitance in Farads
  π  = 3.14159...
```

**What happens physically:**

1. Jar fully charged (all energy in electric field in jar)
2. Spark fires. Jar begins to discharge through the circuit.
3. Current flows through the inductor. As current builds, energy transfers from the electric field in the capacitor to the magnetic field in the inductor.
4. Capacitor fully discharged. All energy now in the magnetic field of the inductor.
5. The inductor's magnetic field collapses, but inductors resist change in current — the current keeps flowing, NOW charging the capacitor in the OPPOSITE polarity.
6. Capacitor is now charged to (nearly) its original voltage but reversed.
7. The discharge repeats in the opposite direction.
8. This sloshing back and forth is oscillation.
9. Each cycle, some energy is radiated as radio waves, some is lost as heat in the resistance. The amplitude decays. This is a DAMPED oscillation.
10. The spark extinguishes when the oscillating voltage is no longer sufficient to maintain the plasma.

### Why the Spark Transmitter Produces Wide Bandwidth

A pure continuous sine wave at one frequency has theoretically zero bandwidth. But a damped oscillation — one that starts suddenly, rings briefly, and decays — has a wide frequency spectrum.

The mathematical reason is Fourier analysis: a signal that is finite in time must have a spread of frequencies in its spectrum. The faster the damping, the wider the spread. The longer the oscillation rings, the narrower the spectrum.

This is why spark transmitters were eventually banned. They splattered energy across a huge swath of spectrum, interfering with many channels at once. Marconi's early transmitters occupied hundreds of kilohertz of bandwidth. A modern FM station uses only 200 kHz. A modern digital radio station can operate in 10 kHz.

The "Q factor" of the circuit determines how long it rings. High Q (low resistance) = narrow bandwidth, long ringing. Low Q (high resistance) = wide bandwidth, fast decay. Spark transmitters typically had Q factors of 5–20. Modern crystal oscillators have Q factors of 10,000 to 100,000.

---

## Part 7 — Build Experiment 1: The Leyden Jar

**Goal:** Build a working Leyden jar capacitor, charge it safely with static electricity, discharge it through a spark gap.

**What this proves:** That electric charge can be stored in a capacitor and released explosively — the fundamental mechanism behind Hertz's transmitter.

### Materials

- 1 glass jar (a preserve jar, jam jar, or pickle jar — thick glass, any size from 250 ml upward)
- Aluminium kitchen foil (about 30 cm × 30 cm total)
- Scissors
- Tape (masking tape or tape is fine)
- Thin wire or paper clip (for the inner electrode)
- A plastic rod (a plastic pen, plastic ruler, or piece of PVC pipe)
- A piece of wool fabric (an old wool jumper, wool socks — must be actual wool not synthetic)
- A dark room or dim lighting (to see the spark better)

### Construction

**Step 1: Line the lower half of the INSIDE of the jar with aluminium foil.**
Cut a strip of foil about 3–4 cm wide and wrap it around the inside of the jar, pressing it flat to the glass. It should cover the lower half of the jar interior, but NOT reach the top — leave a 2–3 cm gap below the rim. This is your inner plate.

**Step 2: Line the lower half of the OUTSIDE of the jar with aluminium foil.**
Do the same on the outside. The outer foil should be aligned with the inner foil — they are the two plates of your capacitor, separated by the glass wall (the dielectric). Do not let the inner and outer foils touch at the top.

**Step 3: Make the inner electrode.**
Bend a paper clip or short length of wire into an L-shape. Rest one end on the inner foil and let the other end stick up above the rim of the jar. This gives you a connection point to charge the inner plate.

**Step 4: Test the structure.**
Hold the outside foil in your hand (this grounds it through your body). The jar is assembled. Do not touch the inner electrode when charging.

### Charging Safely

This experiment uses static electricity. The voltages can be several thousand volts but the current is tiny (microamps). You will feel a mild shock if you touch the inner electrode. This is not dangerous. Do not use mains electricity to charge this jar.

**How to charge with a triboelectric source:**

1. Hold the jar with your hand on the outer foil (you are the ground).
2. Vigorously rub the plastic rod with the wool for 15–20 seconds. The rod becomes negatively charged (electrons transferred from wool to plastic).
3. Touch the inner electrode (the wire sticking up from the inner foil) with the charged plastic rod.
4. Some charge transfers to the inner foil.
5. Repeat several times, re-rubbing the rod between each touch.
6. On a dry day with low humidity, you can build up a charge of several thousand volts on a 200 ml jar with 10–15 rubs.

**Note:** This works much better on dry days. High humidity (rain, damp weather) allows the charge to leak away quickly. Do this experiment on a dry day or indoors with a dehumidifier running.

### Discharging Through a Spark Gap

1. Tear off a 20 cm strip of wire.
2. Hold one end and touch the other end to the outer foil (this end is "grounded").
3. While still holding the grounded end, slowly bring the free end toward the inner electrode.
4. At about 0.5 to 2 mm separation, you should see (and hear) a small spark.
5. In a dark room this is much more visible.

**What you are observing:** The stored electric field in the glass is collapsing. The energy accumulated over many rubs with the rod is released in a fraction of a millisecond. The crack you hear is the rapid expansion of the ionised air column.

### What to Record

In your notebook, record:

| Observation | Your Result |
|-------------|-------------|
| Number of rubs needed before a spark appears | |
| Approximate spark gap at which discharge occurred (mm) | |
| Sound: snap, crack, or nothing | |
| Visible spark: yes/no, colour (blue, white?) | |
| Did high humidity affect results? | |
| Estimated capacitance from the formula (show your measurements and calculation) | |

---

## Part 8 — Build Experiment 2: Hear a Spark on an AM Radio

**Goal:** Generate RF interference with a simple spark source and detect it on a commercial AM radio. This DIRECTLY demonstrates radio transmission.

**What this proves:** Any spark radiates a broadband electromagnetic signal. Your AM radio acts as the receiver. This is exactly what Marconi's system did in 1895 — just much cruder.

### Materials

- Any AM radio (a small pocket radio, clock radio, or your phone's AM radio app if it has one — "nextRadio" app on Android, for example; or a cheap AM pocket radio from a street market for ₹100–200)
- A lighter (piezoelectric sparking type is best — the kind where you press a button and a spark ignites the gas)
- Alternatively: two AA batteries and a piece of wire (touch and separate the wire on the battery terminal rapidly)
- Alternative alternative: comb your hair with a plastic comb on a dry day and touch the comb near the radio

### Procedure

**Step 1:** Tune the AM radio to a quiet part of the dial — a frequency where there is no station. Frequencies around 1200–1400 kHz often have quiet gaps in most regions. Turn the volume up to maximum.

**Step 2:** You should hear background noise (hiss or static). This is the thermal noise floor and any weak interfering signals.

**Step 3:** Hold the lighter (do NOT ignite the gas — just use the piezoelectric sparker) about 30 cm from the radio. Press the sparker button. You should hear a distinct CLICK or CRACKLE on the radio.

**Step 4:** Repeat at different distances: 10 cm, 30 cm, 50 cm, 1 m, 2 m, 3 m. Note at what distance the click becomes inaudible.

**Step 5:** Try different positions relative to the radio — above, below, same height, at the antenna end, at the back.

**Step 6:** Try it with the radio tuned to different frequencies. Try the same spark at 540 kHz and at 1600 kHz. Does the click sound the same? (It should, because the spark radiates across a wide band — damped oscillation, remember.)

### What You Are Observing

The piezoelectric lighter creates a spark of several thousand volts across a very small gap. The discharge oscillates at a frequency determined by the stray capacitance and inductance of the lighter's internal circuit — probably in the range of 1–100 MHz.

Because it is a damped oscillation (not a continuous sine wave), the energy is spread across a very wide band of frequencies. Your AM radio at 1 MHz "hears" its slice of that energy as a click.

This is exactly what Hertz saw from his transmitter. The difference is Hertz used a tuned LC circuit so the energy was concentrated in a narrower band, giving more power at the specific frequency his receiver was tuned to.

### Further Exploration

Try these variations and record results:

1. **Electric motor sparking:** If you have any small DC motor (from a toy), run it near the radio. The brushes create sparks at the commutator. You will hear a buzzing noise on the AM radio — this is called "motor interference" and was a major problem in early 20th century radio.

2. **Light switch:** Quickly flip a light switch near the radio with the switch near the radio's antenna. You should hear a click as the arc forms when the circuit opens (inductive kick from the light fixture wiring).

3. **Car ignition (from a safe distance):** If you are outside and a car starts nearby, you may hear the ignition interference on the AM radio. Modern car electronics suppress this with resistive spark plug leads and coil suppressors.

### What to Record

| Distance (cm) | Click audible? | Approximate volume (relative) |
|--------------|---------------|-------------------------------|
| 10 | | |
| 30 | | |
| 50 | | |
| 100 | | |
| 200 | | |
| 300 | | |

Also record:
- What AM frequency were you tuned to?
- Was the click present across multiple AM frequencies you tried?
- What was the rough weather / humidity today?
- Any other spark sources you tried and their results?

---

## Part 9 — Build Experiment 3: Wind a Simple Induction Coil and Measure Voltage Step-Up

**Goal:** Wind a two-winding transformer on a toilet roll core. Measure the input and output voltage. Confirm the turns ratio = voltage ratio.

**What this proves:** Mutual inductance — the mechanism by which Marconi's and Hertz's induction coils stepped low voltage to high voltage, and the same mechanism inside every transformer in your power supply, motor controller, and switching regulator.

### Materials

- 1 toilet roll tube (about 40 mm diameter, 110 mm long) — or a similar cardboard tube
- Enamelled copper wire, 0.3 mm diameter — about 10 metres
  (eBay search term: "enamelled copper wire 0.3mm" — a 100g spool is sufficient and costs about ₹200/$3)
- Alternatively: wire from a discarded transformer (unspool it carefully)
- A multimeter (set to AC voltage mode)
- A 6V or 9V AC source — OR a 9V battery with a 555 timer generating square wave (see below)
- Tape (masking tape)
- Fine sandpaper (400 grit) to strip the enamel from the wire ends
- A marker pen

### Preparing the Core

1. Cut two pieces of masking tape about 5 cm long and make marks on the tube at 10 mm and 30 mm from one end, and 50 mm and 80 mm from the same end. These mark the two winding zones.

2. Mark the first zone (10–30 mm from end) "PRIMARY".
3. Mark the second zone (50–80 mm from end) "SECONDARY".

### Winding the Primary (10 Turns)

1. Leave 10 cm of wire free at the start (this is the start lead, label it P1).
2. Wind 10 tight, even turns of enamelled wire in the PRIMARY zone. Keep turns close together, all in the same direction (all clockwise as you look at the end of the tube, for example).
3. Leave 10 cm of wire free at the end (label it P2).
4. Tape the winding to prevent it from unwinding.

### Winding the Secondary (100 Turns)

1. Leave 10 cm of wire free at the start (label it S1).
2. Wind 100 turns in the SECONDARY zone. This will take a while — count carefully. Keep the same winding direction as the primary.
3. Leave 10 cm of wire free at the end (label it S2).
4. Tape the winding.

**Total turns ratio: N₂/N₁ = 100/10 = 10. You expect the secondary voltage to be 10× the primary voltage.**

### Stripping the Enamel

Enamelled wire is coated with a thin varnish insulator. You MUST remove this from the wire ends before connecting anything — if you don't, you will have no electrical connection even though it looks fine.

Method 1 — Sandpaper: Hold the wire end against a piece of fine (400-grit) sandpaper and rub 10–15 times. The coating will come off and the bare copper will be visible (shiny).

Method 2 — Flame: Pass the wire end through a lighter flame for 2–3 seconds. The enamel burns off. Let it cool. The wire end will be bare but slightly oxidised — wipe with fine sandpaper to clean.

Test with your multimeter on continuity mode: touch the probes to the wire ends. You should get a beep (low resistance). If you get open circuit, the enamel is not stripped.

### Measuring the Voltage Step-Up

**Using a 9V AC source (if available):**
Connect P1 and P2 to the 9V AC source. Measure voltage across S1–S2 with the multimeter on AC mode.

Expected: 9V × (100/10) = 90V AC

**Important safety note:** 90V AC can give a painful and potentially dangerous shock. Do not touch the secondary winding while it is connected to the AC source. Measure with probes only.

**Using a 9V battery + 555 timer square wave:**
If you do not have an AC source, build a simple astable 555 oscillator:

```
   9V ─────────────────────────────────┐
                                       │
        ┌─────────┐                    │
        │  555    │                    R1 (1kΩ)
        │         │                    │
   Vcc ─┤8      4 ├─ Reset (to Vcc)   │
   Out ─┤3      2 ├─ Trigger ──┐      R2 (10kΩ)
   Dis ─┤7      6 ├─ Threshold─┤       │
        │       5 ├─ (100nF to GND)   C1 (10nF)
   GND ─┤1        │              │     │
        └─────────┘              └─────┘
                                      GND
```

A 555 in astable mode with R1=1kΩ, R2=10kΩ, C1=10nF gives approximately:

```
    f ≈ 1.44 / ((R1 + 2×R2) × C1)
    f ≈ 1.44 / ((1000 + 20000) × 10×10⁻⁹)
    f ≈ 1.44 / (21000 × 10⁻⁸)
    f ≈ 1.44 / 2.1×10⁻⁴
    f ≈ 6,857 Hz ≈ 7 kHz
```

Connect the 555 output to the primary coil. The square wave acts as a rapidly changing current, inducing voltage in the secondary. Measure the secondary output with a multimeter on AC mode (set to 20V range first).

A 555 square wave is not a clean sine wave, so the multimeter reading will be approximate, but the ratio of secondary to primary voltage should be close to 10:1.

### What to Record

| Measurement | Value |
|-------------|-------|
| Primary voltage (measured) | |
| Secondary voltage (measured) | |
| Calculated ratio V_secondary / V_primary | |
| Expected ratio (N2/N1 = 100/10) | 10 |
| Percentage error | |

If your ratio differs significantly from 10, likely causes:
- Enamel not properly stripped (high contact resistance)
- Miscounted turns
- The core is air (not ferrite) — the coupling is not perfect, energy leaks around the outside of the toroid — air-core transformers at low frequencies are inefficient. At RF frequencies they work fine.

---

## Part 10 — Key Calculations Reference

### Leyden Jar Capacitance

```
         ε₀ × εr × A
    C = ─────────────
               d

  ε₀ = 8.854 × 10⁻¹² F/m
  εr (glass) ≈ 4 to 10, use 7 for standard jar glass
  A = overlap area of foil in m²
  d = glass wall thickness in m
```

### Resonant Frequency of LC Circuit

```
              1
    f₀ = ──────────────
          2π × √(L × C)

  f₀ in Hz
  L in Henries
  C in Farads

  Rearranged to find C for a given f₀:
              1
    C = ──────────────
         (2πf₀)² × L

  Rearranged to find L for a given f₀:
              1
    L = ──────────────
         (2πf₀)² × C
```

### Transformer Voltage Ratio

```
    Vsecondary     N₂      (turns in secondary)
    ─────────── = ──── = ─────────────────────────────
    Vprimary       N₁      (turns in primary)
```

### Energy Stored in Capacitor

```
         1
    E = ─── × C × V²     (in Joules)
         2
```

### Spark Gap Breakdown Voltage in Air

```
    V_breakdown ≈ 30,000 V/cm × gap in cm
    (at sea level, standard pressure, dry air)
    So 1 mm gap requires about 3,000 V to spark
```

### Worked Example: Hertz's Circuit

Assume Hertz's dipole rods had:
- Approximate inductance: L ≈ 2 µH (2 × 10⁻⁶ H) for two 1-metre rods
- Approximate capacitance: C ≈ 10 pF (10 × 10⁻¹² F) for the Leyden jars

```
              1
    f₀ = ──────────────────────────────
          2π × √(2×10⁻⁶ × 10×10⁻¹²)

              1
    f₀ = ──────────────────────────────
          2π × √(2×10⁻¹⁷)

              1
    f₀ = ──────────────────────────────
          2π × 1.414×10⁻⁸·⁵

              1
    f₀ = ──────────────────────────────
          2π × 4.47×10⁻⁹

    f₀ ≈ 35.6 MHz
```

This is in the VHF range — wavelength about 8.4 metres. Consistent with the ~1-metre rod dimensions Hertz used (a half-wave dipole resonates at wavelength = 2 × rod length × adjustment factor).

---

## Part 11 — Safety Notes for High Voltage Experiments

### Leyden Jar (Static Electricity)
- The voltages are high (thousands of volts) but the energy is tiny (millijoules) and the current is microamps.
- You will feel a mild, sharp shock if you touch the inner electrode when charged. It is unpleasant but not dangerous for a healthy adult.
- People with heart conditions or pacemakers should avoid this experiment.
- Do not build a very large bank of Leyden jars — the energy scales with capacitance, and a large bank could deliver a dangerous shock.
- Do not attempt to charge the jar from a mains outlet. Static electricity charging only.

### Induction Coil Experiment
- If you use an AC source on the primary, the secondary output CAN be high enough to be dangerous.
- A 9V primary with 10:100 turns ratio gives ~90V on the secondary. This can cause muscle contraction and inability to let go.
- Never touch the secondary terminals when power is connected. Use insulated probes only.
- Do not use mains voltage (230V) as the primary input. The secondary would then be 2300V — this is lethal.
- Keep the experiment on an insulated surface (wooden table, not metal workbench).

### General RF Safety
- The RF energy produced by the spark experiments in this document is negligibly small — less than a milliwatt. There is no health risk.
- Be aware that spark sources can interfere with sensitive electronics — turn off computers, smartphones, or any in-progress data operations before experimenting.

---

## Part 12 — What to Record in Your Notebook

For each experiment, create a dedicated page in your lab notebook. Use this template:

```
Date:
Temperature:
Humidity (low/medium/high):
Experiment name:

AIM:
(What are you trying to demonstrate or measure?)

CIRCUIT/SETUP DIAGRAM:
(Hand-drawn schematic or physical layout)

PREDICTION BEFORE THE EXPERIMENT:
(What do you expect to happen based on theory?)

OBSERVATIONS:
(What actually happened — record EVERYTHING including failures and unexpected results)

MEASUREMENTS TABLE:
(Tabulate all measured values with units)

CALCULATIONS:
(Show all working with units on every number)

COMPARISON OF MEASURED vs PREDICTED:
(Are they the same? If different, why?)

CONCLUSIONS:
(What does this experiment prove? How does it connect to the theory?)

WHAT I WOULD DO DIFFERENTLY:
(What would you change to get better results or learn more?)
```

---

## Part 13 — Troubleshooting Table

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| Leyden jar won't hold charge | Humidity too high | Do experiment on drier day; warm the jar in a low oven for 10 min to dry the glass |
| Leyden jar holds charge but no spark on discharge | Inner/outer foil don't fully overlap | Recheck construction — gap between plates must be covered by glass only |
| Induction coil shows no secondary voltage | Enamel not stripped from wire ends | Re-strip all four ends with sandpaper; test continuity with multimeter |
| Secondary voltage much less than 10× primary | Loose connections, or primary and secondary too far apart on the core | Slide the windings closer together; ensure tight joints |
| No click on AM radio from lighter spark | Radio not on AM mode, or on FM/DAB | Set radio to AM (MW) explicitly; try different frequencies |
| Click inaudible beyond 20 cm | Normal — static sparks are weak | Try at maximum radio volume; use a better spark source (ignition coil if available) |
| Spark experiment works at 540 kHz but not 1400 kHz | Your lighter produces a lower-frequency spark | Try a different spark source |
| Multimeter reads 0 on secondary | AC mode not selected | Switch to AC mode (V~) not DC mode (V—) |

---

## Part 14 — Historical Context: What Marconi Did, Step by Step

To fully appreciate what Marconi achieved, consider the state of communication in 1895:

- The fastest communication was telegraph — electrical pulses over copper wires strung between poles
- A message from London to New York went via undersea telegraph cable (laid 1866) — worked, but expensive, and the cable was physically vulnerable
- No cable reached many places — ships at sea had NO communication once out of port. This killed thousands of sailors who could not call for help.
- Weather forecasting was almost impossible — ships could not report sea conditions back to land

By 1901, Marconi had demonstrated that:
- Radio could cross the Atlantic without any physical cable
- Ships could communicate with the shore (and were required by law to carry wireless sets after the Titanic disaster in 1912)
- Multiple stations could operate at different frequencies without (in principle) interfering
- A new industry — wireless telegraphy, eventually just "wireless" or "radio" — was born

The direct line from Hertz's spark gap to your ESP32's 2.4 GHz radio:

```
1887 Hertz spark gap transmitter
         ↓
1895 Marconi with tuned LC + antenna
         ↓
1901 Continuous wave (CW) transmitter — replaced spark with vacuum tube oscillator
         ↓
1906 First voice transmission (Reginald Fessenden, Christmas Eve, ships at sea)
         ↓
1920s Broadcast radio — AM stations
         ↓
1930s FM radio (invented by Edwin Armstrong)
         ↓
1940s Radar — pulsed microwave radio
         ↓
1950s Transistors replace vacuum tubes
         ↓
1970s–80s Integrated circuit RF chips
         ↓
1990s Cellular networks, digital modulation
         ↓
2000s WiFi, Bluetooth, ZigBee
         ↓
TODAY: Your ESP32 with integrated 2.4 GHz transceiver — all on one chip
```

Every step in that chain is the same physics. An accelerating charge produces an electromagnetic wave. The improvements are in how we control the frequency, how we modulate information onto it, and how efficiently we detect it at the other end.

---

## Part 15 — Key Insight: Connecting to Modern Radio Chips

You are building a drone with an ESP32. Inside that ESP32 is a 2.4 GHz radio transceiver. Here is what you now understand about what is happening inside that chip when it transmits:

1. A digital oscillator circuit creates a current that oscillates at 2.4 GHz. This is the LC resonance you learned about — but at 2.4 billion cycles per second, implemented in silicon rather than wire and capacitor.

2. The oscillating current is fed to a tiny antenna — a trace on the PCB or a wire monopole — which is carefully dimensioned for 2.4 GHz (12.5 cm wavelength → antenna ≈ 3 cm for a quarter-wave monopole).

3. The antenna is the accelerating charge. At 2.4 GHz, the charges in that antenna are accelerating back and forth 2.4 billion times per second. Each acceleration radiates a tiny electromagnetic wave.

4. On the receive side, the incoming wave induces a tiny voltage in the receiving antenna. An LC tank tuned to 2.4 GHz resonates with that signal, building it up to detectable levels.

5. A detector (modern version: a mixer and ADC, not a crystal detector) recovers the modulated information.

Hertz's 1887 experiment and your ESP32 are doing exactly the same thing. The difference is 130 years of engineering refinement, miniaturisation, and digital signal processing. The physics has not changed at all.

---

*End of Stage 1. Proceed to Stage 2: The Crystal Set — Radio With No Battery.*
