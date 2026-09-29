# Hardware

## Selected controller

### WEMOS D1 R32 / ESP32

The controller was selected after directly testing both the D1 R32 and a NodeMCU ESP8266.

Measured results:

| Metric | NodeMCU ESP8266 | WEMOS D1 R32 / ESP32 |
|---|---:|---:|
| Flash | 4 MB | 4 MB |
| Basic free heap | ~51.9 KB | ~350 KB |
| CPU | 80 MHz | 240 MHz |
| Cores | 1 | 2 |
| I²S suitability | limited | native / preferred |

The ESP32 provides enough headroom for the local web UI, persistent configuration, RTC integration, and voice playback.

## Flash measurement

On the ESP32 prototype:

| Metric | Value |
|---|---:|
| Physical flash | 4 MB |
| Test sketch size | ~312 KB |
| LittleFS total | ~1.38 MB |
| LittleFS free | ~1.37 MB |
| Free heap before AP | ~307 KB |
| Free heap with AP | ~254 KB |

This is sufficient to continue evaluating compressed voice clips in internal flash before introducing microSD.

## Current breadboard wiring

| Function | ESP32 pin | Wiring |
|---|---|---|
| Status LED | GPIO25 | GPIO25 → 560 Ω → LED → GND |
| Acknowledge/config button | GPIO26 | GPIO26 → button → GND, `INPUT_PULLUP` |
| Active buzzer module | GPIO27 | S → GPIO27, + → 3V3, - → GND |

The buzzer is a temporary stand-in for the real voice output.

## Planned MVP BOM

| Component | Quantity | Status |
|---|---:|---|
| WEMOS D1 R32 / ESP32 | 1 | Have / validated |
| DS3231 RTC | 1 | To acquire / integrate |
| MAX98357A I²S amplifier | 1 | To acquire / integrate |
| 4 Ω / 3 W speaker | 1 | To acquire / integrate |
| Push button | 1 | Have / validated |
| LED | 1 | Have / validated |
| 560 Ω resistor | 1 | Have / validated |
| USB 5 V supply | 1 | Have |
| Breadboard/jumpers | — | Have |

## 3D-printed enclosure

The enclosure will be designed after the electronics and user interaction are stable.

Planned mechanical concept:

- large 3D-printed button/cap visible to the user;
- the printed part mechanically actuates a small inexpensive internal push button;
- internal button load should be transferred to the case structure, not glue alone;
- enough travel for reliable actuation;
- return movement must not bind;
- case should prioritize speaker intelligibility and easy USB access.

Future CAD/STL files belong under `hardware/case/`.
