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
