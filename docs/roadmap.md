# MVP Roadmap — 7 Days

Target: produce a demonstrable, supervised-use prototype in one week.

## Status

| Day | Goal | Status |
|---|---|---|
| Day 1 | Select controller and validate development environment | ✅ Done |
| Day 2 | Firmware core, persistence, state machine | ✅ Done |
| Day 3 | Local Wi-Fi configuration UI | ✅ Done |
| Day 4 | DS3231 RTC + scheduler | ⏳ Next |
| Day 5 | Local voice playback | ⏳ Planned |
| Day 6 | Robustness and UX | ⏳ Planned |
| Day 7 | 3D assembly + real-world supervised test | ⏳ Planned |

## Day 1 — Controller qualification

Completed:

- PlatformIO installed on Linux;
- CH340 serial path validated;
- NodeMCU ESP8266 tested;
- WEMOS D1 R32 tested;
- upload and serial monitoring validated;
- real flash/heap measured;
- D1 R32 selected.

## Day 2 — Core firmware

Completed:

- IDLE / ALARM state behavior;
- physical button with debounce;
- LED;
- buzzer placeholder;
- watchdog;
- NVS persistence;
- 10 alarm slots;
- reboot preserves configuration.

## Day 3 — Local configuration

Completed:

- temporary ESP32 access point;
- mobile web interface;
- edit/enable 10 alarm slots;
- save to NVS;
- test alarm from web UI;
- long press toggles configuration mode;
- Wi-Fi disabled outside configuration.

Known non-blocking log noise:

- unmatched browser requests can trigger `WebServer.cpp: request handler not found`;
- Wi-Fi shutdown produced a netstack callback error once during bench testing.

These are tracked as cleanup, not blockers.

## Day 4 — RTC and scheduler

- integrate DS3231;
- configure current date/time;
- compare RTC time against enabled alarm slots;
- avoid duplicate firing within the same minute;
- recover correctly after reboot;
- validate day rollover and power interruption.

**Done when:** configured alarms fire at the correct wall-clock time without internet.

## Day 5 — Voice

Preferred path:

- MAX98357A;
- short compressed audio stored locally;
- test intelligibility and volume;
- associate each alarm slot with one of two recipient profiles;
- play a generic personalized message per recipient (“<name>, está na hora de tomar o remédio”);
- keep medication-specific naming optional until the real medication list is available.

Fallback gate:

If internal-flash audio becomes unstable, memory-constrained, or consumes too much implementation time, use DFPlayer Mini + microSD.

**Done when:** scheduler → voice → acknowledge button works end-to-end.

## Day 6 — Robustness and UX

- repeat the voice reminder indefinitely until physical acknowledgement;
- validate a comfortable repeat interval with the real users;
- LED patterns for normal/alarm/config;
- repeated reboot testing;
- Wi-Fi timeout;
- watchdog behavior;
- volume at realistic distance;
- remove unnecessary interactions.

## Day 7 — Physical prototype

- temporary 3D-printed enclosure;
- large printed button actuator;
- speaker placement;
- realistic daily schedule for two recipient profiles;
- supervised test;
- capture friction points;
- define v0.2 backlog.

## Explicitly out of scope for v0.1

- custom PCB;
- commercial certification;
- cloud backend;
- mobile app;
- OTA updates;
- remote monitoring;
- battery/UPS;
- automatic pill dispensing.
