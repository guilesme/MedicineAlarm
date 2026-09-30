# Engineering Decisions

This file records decisions that materially shape the project.

## ADR-001 — ESP32 over NodeMCU ESP8266

**Decision:** use the WEMOS D1 R32 / ESP32 for the MVP.

**Why:** both boards had 4 MB flash, but the ESP32 exposed ~350 KB free heap in the basic test versus ~52 KB on the ESP8266, plus higher CPU capacity, two cores, and a cleaner path to I²S audio.

**Trade-off:** the NodeMCU board is physically smaller and already available in multiple units.

## ADR-002 — No display in the MVP

**Decision:** omit a display.

**Why:** the target user has low vision, and a small OLED would add complexity without solving the core accessibility problem. Configuration feedback belongs on the caregiver's phone through the local web UI.

Visual feedback on the device is provided by an LED.

## ADR-003 — Temporary local Wi-Fi instead of an app

**Decision:** configuration uses an ESP32-hosted access point and local web page.

**Why:** avoids mobile app development, cloud services, account management, home Wi-Fi provisioning, and internet dependency.

## ADR-004 — One large interaction target

**Decision:** daily interaction should center on one large acknowledge button.

**Why:** reduces cognitive and visual load.

The final enclosure can use a large 3D-printed button/cap that actuates a small internal switch.

## ADR-005 — USB first, battery later

**Decision:** validate the product on USB 5 V before designing backup power.

**Why:** battery charging, protection, regulation, and power-path behavior are a separate engineering problem and do not validate the reminder interaction.

## ADR-006 — Internal flash audio first

**Decision:** first attempt local audio from ESP32 flash through MAX98357A.

**Why:** avoids removable microSD and reduces parts.

**Fallback:** DFPlayer Mini + microSD if internal decoding/storage is not worth the implementation cost.

## ADR-007 — Acknowledged is not taken

**Decision:** the button records that the reminder was acknowledged, not that the medication was ingested.

**Why:** the device has no sensor capable of verifying ingestion, so the software and documentation must not imply otherwise.

## ADR-008 — No automatic dispenser in v0.1

**Decision:** do not mechanically dispense medication.

**Why:** motors, jams, pill geometry, dosing errors, and safety implications would dramatically increase complexity and risk before the reminder concept is validated.


## ADR-009 — Daily schedules for MVP

**Decision:** alarm slots repeat every day in v0.1.

**Why:** the first real use case does not require weekday-specific scheduling. A daily model keeps configuration and scheduler behavior simple.

**Deferred:** per-weekday and date-specific rules may be added only if real usage demonstrates a need.

## ADR-010 — Two recipient profiles

**Decision:** the MVP must support two independent recipient groups: grandfather and grandmother.

**Initial audio behavior:** each configured reminder belongs to one recipient profile and plays a generic personalized phrase, for example “<name>, está na hora de tomar o remédio.”

**Names:** the actual names will be added later when supplied by the family.

**Medication names:** associating individual medication names with reminders is desirable, but explicitly deferred until the medication list and final audio workflow are known.

## ADR-011 — Reminder persists until acknowledgement

**Decision:** an active reminder is not considered complete until the physical acknowledgement button is pressed.

**Behavior:** voice reminders should repeat indefinitely, at a controlled interval, until acknowledgement. The exact repeat interval is still to be validated with the users.

**Why:** a single playback can be missed, while automatic timeout would silently turn a missed reminder into an apparently completed event.


## ADR-009 — Daily schedules for MVP

**Decision:** alarm slots repeat every day in v0.1.

**Why:** the first use case does not require weekday-specific scheduling. Daily scheduling keeps configuration and scheduler behavior simple.

## ADR-010 — Two recipient profiles

**Decision:** the MVP supports two independent recipient profiles.

Each reminder belongs to one profile. Initial spoken prompts are generic and personalized by the profile name. Names will be added when available. Item-specific spoken labels can be added later after the real schedule is known.

## ADR-011 — Reminder persists until acknowledgement

**Decision:** a reminder remains active until the physical acknowledgement button is pressed.

The spoken prompt should repeat indefinitely at a controlled interval. The interval remains TBD until it can be validated with the real users.
