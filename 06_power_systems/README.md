# Module 06 — Power Systems

A drone's power system is its most critical (and most dangerous) subsystem. Wiring mistakes here can destroy your drone or start a fire.

## Topics

| File | What You'll Learn | Status |
|------|-------------------|--------|
| 01 — LiPo Batteries | Cell count (1S–6S), C-rating, safe handling, charging | Coming soon |
| [02_ams1117_power_module.md](02_ams1117_power_module.md) | LDO regulator, 3.3V supply for NRF24L01, decoupling caps, heat | Done |
| [03_ams1117_5v_module.md](03_ams1117_5v_module.md) | 5V output from 9V/12V, powers ESP32 VIN + HC-SR04, heat vs load table | Done |
| 04 — Power Distribution | PDB (Power Distribution Board), current paths, fusing | Coming soon |
| 04 — ESC Basics | How ESCs work, BEC, UBEC | Coming soon |

## Critical Safety Rules (Read First)

1. **Never overcharge** a LiPo — use a balance charger only
2. **Never short** a LiPo — carry in fireproof bags
3. **Never discharge below 3.5V/cell** — ruins the battery
4. **Never puncture** a LiPo — store in ammo boxes or LiPo bags
5. **Always charge supervised** — LiPo fires spread fast
