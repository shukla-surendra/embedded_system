# Module 01 — Electronics Fundamentals

Before touching any microcontroller, you need a mental model of how electricity works. This module builds that model from scratch.

## Topics

| File | What You'll Learn |
|------|-------------------|
| [01 — Electricity Basics](01_electricity_basics.md) | Voltage, current, resistance — what they actually mean |
| [02 — Ohm's Law](02_ohms_law.md) | The single most important formula in electronics |
| [03 — Basic Components](03_basic_components.md) | Resistors, capacitors, LEDs, transistors, diodes |
| [04 — Reading Circuits](04_reading_circuits.md) | Schematics, breadboards, and wiring diagrams |
| [05 — Capacitors In Depth](05_capacitors_in_depth.md) | All types, voltage rating, ESR, polarity, C0G vs X7R, your 3 cap types explained |
| [06 — Prototyping Hardware](06_prototyping_hardware.md) | Perfboard layout, soldering technique, pin headers, drone vibration rules |
| [07 — Resistors In Depth](07_resistors_in_depth.md) | All types, power ratings, colour codes, E-series, voltage dividers, pull-ups |
| [08 — Diodes In Depth](08_diodes_in_depth.md) | Rectifier, Schottky, Zener, TVS, LED, flyback diode, VI curve, selection guide |
| [09 — BJT Transistors](09_transistors_bjt.md) | NPN/PNP, hFE, saturation, base resistor design, Darlington, BJT vs MOSFET |
| [10 — MOSFETs](10_mosfets.md) | N/P-channel, logic-level, RDS(on), PWM motor control, H-bridge, gate pull-down |
| [11 — Amplifiers](11_amplifiers.md) | BJT, MOSFET, Op-Amp, JFET, vacuum tube — which components amplify and how |
| [12 — Radio Signals](12_radio_signals.md) | How radio waves are generated, transmitted, and received — oscillators, modulation, LNA, mixers, antennas |
| [13 — Electromagnetic Waves](13_electromagnetic_waves.md) | Full EM spectrum — radio, microwave, IR, visible, UV, X-ray, gamma — physics, properties, components, drone relevance |
| [14 — Radar](14_radar.md) | How radar works, all types (pulsed, CW, FMCW, SAR, phased array), components, signal processing, drone modules |
| [15 — Sonar](15_sonar.md) | How sonar works, all types (active, passive, FMCW, medical, DVL), transducers, piezo physics, ultrasonic sensors, drone use |
| [16 — Chips and Modules Reference](16_chips_and_modules_reference.md) | Complete catalogue of common ICs and modules — MCUs, RF, motor drivers, power, sensors, displays, ADC/DAC, audio, logic |
| [17 — GPS](17_gps.md) | How GPS works — trilateration, satellite signals, CDMA, error sources, RTK, GNSS constellations, NMEA, drone modules |
| [18 — WiFi](18_wifi.md) | How WiFi works — OFDM, MIMO, QAM, CSMA/CA, WPA2, ESP-NOW, power modes, all 802.11 standards |
| [19 — Bluetooth](19_bluetooth.md) | How Bluetooth works — FHSS, Classic vs BLE, GATT, BLE advertising, Bluetooth 5 long range, audio, drone use |
| [20 — Defence-Grade Technologies](20_defence_grade_technologies.md) | MIL-SPEC standards, anti-jam comms (SINCGARS, Link 16), military GPS (M-code, CRPA), electronic warfare, AESA radar, IFF, directed energy, rad-hard ICs, counter-drone |
| [21 — Video Signal Transfer](21_video_signal_transfer.md) | Complete video chain — analog (composite/FPV), digital interfaces (HDMI/SDI/CSI), codecs (H.264/H.265/AV1), streaming protocols (RTSP/HLS/WebRTC), wireless FPV, compression |
| [22 — Modulation and Data Over Waves](22_modulation_and_data_over_waves.md) | How data rides a wave — AM/FM/PM, ASK/FSK/PSK/QAM, OFDM, DSSS, FHSS, LoRa CSS, MIMO, Shannon limit, FEC, all modulation techniques |
| [23 — Hobbyist Complete Reference](23_hobbyist_complete_reference.md) | Master reference — every term, law, component, protocol, tool, skill, safety rule, debugging tip, formula, and acronym a hobbyist needs |
| [24 — Radio From First Principles](24_radio_from_first_principles.md) | Overview: Marconi's path, spark gaps, crystal sets, LC resonance, regeneration, superhet — no ICs, no modules |
| [25 — Silicon & Semiconductors](25_silicon_and_semiconductors.md) | Complete theory from atoms to chips — crystal structure, energy bands, doping, PN junction, BJT, MOSFET, chip fabrication, 10 home experiments |
| [26 — Inductors](26_inductors.md) | Complete inductor theory — magnetic fields, Faraday/Lenz laws, RL/LC circuits, transformers, buck/boost converters, flyback voltage, all types, 7 home experiments |
| **[Radio Stages (10 practical lab guides →)](radio_stages/README.md)** | **Complete stage-by-stage build series — each stage is a full practical guide with schematics, component values, calculations, and experiments** |
| &nbsp;&nbsp;↳ [Stage 1 — Spark and Hertz](radio_stages/stage_01_spark_and_hertz.md) | Leyden jar build, induction coil, spark = radio transmission experiment |
| &nbsp;&nbsp;↳ [Stage 2 — Crystal Set](radio_stages/stage_02_crystal_set.md) | Full crystal set: coil winding, LC tank, germanium diode, no-battery receiver |
| &nbsp;&nbsp;↳ [Stage 3 — LC Resonance Deep Dive](radio_stages/stage_03_lc_resonance_deep_dive.md) | Resonant frequency derived, Q factor, core materials, 3 design examples |
| &nbsp;&nbsp;↳ [Stage 4 — Diode Detector](radio_stages/stage_04_diode_detector.md) | Envelope detection waveforms, galena cat's whisker build, Ge vs Si vs Schottky |
| &nbsp;&nbsp;↳ [Stage 5 — First Amplifier](radio_stages/stage_05_first_amplifier_transistor.md) | Transistor bias design, gain calculation, crystal set + amp = loudspeaker output |
| &nbsp;&nbsp;↳ [Stage 6 — Regenerative Receiver](radio_stages/stage_06_regenerative_receiver.md) | Positive feedback, tickler coil, 3 operating regions, hear stations 500+ km |
| &nbsp;&nbsp;↳ [Stage 7 — Colpitts Oscillator](radio_stages/stage_07_colpitts_oscillator.md) | Barkhausen criterion, 1 MHz oscillator, AM voice transmitter |
| &nbsp;&nbsp;↳ [Stage 8 — Mixer and Heterodyne](radio_stages/stage_08_mixer_and_heterodyne.md) | cos(A)×cos(B) identity, diode ring mixer, transistor mixer, image frequency |
| &nbsp;&nbsp;↳ [Stage 9 — Complete Superheterodyne](radio_stages/stage_09_complete_superhet.md) | All 7 stages combined, alignment, AGC, full working AM radio from scratch |
| &nbsp;&nbsp;↳ [Stage 10 — FM and Beyond](radio_stages/stage_10_fm_and_beyond.md) | FM detection, Foster-Seeley, BFO for SSB/CW, path from discrete to ICs |

## Goal

By the end of this module you will be able to:
- Explain what voltage, current, and resistance are in plain words
- Calculate safe resistor values for an LED
- Read a basic schematic diagram
- Safely wire a simple circuit on a breadboard

## Time Estimate

3–5 hours of reading + hands-on practice
