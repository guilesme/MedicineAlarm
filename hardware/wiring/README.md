# Wiring

Current breadboard wiring:

| Function | ESP32 pin | Connection |
|---|---|---|
| LED | GPIO25 | GPIO25 → 560 Ω → LED → GND |
| Button | GPIO26 | GPIO26 → button → GND |
| Active buzzer | GPIO27 | signal → GPIO27, + → 3V3, - → GND |

The button uses the ESP32 internal pull-up.

Future diagrams should be added here as the RTC and audio hardware are integrated.
