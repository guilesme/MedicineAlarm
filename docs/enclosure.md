# Enclosure design brief

## Goal

Create a simple, accessible, printable enclosure for the MedicineAlarm MVP.

The enclosure should make the daily interaction obvious:

- hear the reminder;
- press one large button;
- no display;
- no menu;
- no exposed electronics.

## Functional layout

Preferred v0.1 arrangement:

- one large top/front acknowledgement actuator;
- speaker grille facing the user;
- visible status LED;
- USB power/programming access from the rear;
- recessed configuration access so it is not pressed accidentally;
- internal mounts for ESP32, RTC and audio amplifier;
- removable lid/base for maintenance.

## Parametric-first approach

Do not freeze external dimensions until the real RTC, amplifier and speaker have arrived and been measured.

The CAD should therefore keep these dimensions adjustable:

- controller footprint;
- speaker diameter/depth;
- button actuator diameter and travel;
- wall thickness;
- PCB standoff positions;
- USB opening;
- enclosure width/depth/height.

For a public/open project, a text-based parametric source such as OpenSCAD is attractive because it versions cleanly in Git. STL/3MF exports can be published beside the editable source.

## Button concept

The user-facing button can be much larger than the internal electrical switch.

Concept:

```text
large printed cap
      ↓
guided travel
      ↓
small internal momentary switch
      ↓
structural support in enclosure
```

Important mechanical checks:

- cap cannot rotate/bind;
- sufficient return clearance;
- switch load goes into the case structure, not glue;
- travel is obvious but short;
- button can be found by touch;
- force is comfortable for an older adult.

## Speaker

The enclosure must prioritize speech intelligibility over compactness.

Design considerations:

- grille directly in front of the speaker;
- enough open area to avoid muffling speech;
- avoid placing the speaker against the table;
- provide solid mounting so vibration does not create rattling;
- keep the amplifier close enough for short speaker wiring.

## MakerWorld references

Useful references were found, but the final MedicineAlarm enclosure should be designed independently so its source can be published with a compatible open-hardware license.

### Australian Pedestrian Crossing Button

https://makerworld.com/en/models/1725266-australian-pedestrian-crossing-button-ams

Very relevant mechanical/electronic reference because it combines:

- ESP32;
- large 30 mm arcade button;
- LED;
- small speaker;
- MAX98357 I2S amplifier.

Its MakerWorld listing uses the Standard Digital File License, which restricts redistribution/remixing. Treat it as design inspiration only unless explicit permission is obtained.

### A.I. Speakerbox

https://makerworld.com/it/models/1475284-a-i-speaker-box

Relevant audio packaging reference because it combines:

- ESP32-S3;
- 2-inch speaker;
- MAX98357;
- 24 mm arcade button.

It includes display/microphone features MedicineAlarm does not need. The listing is Creative Commons Attribution-NonCommercial, so it is better treated as reference material rather than the canonical enclosure.

### ESP32 Arcade Button Box

https://makerworld.com/en/models/194798-esp32-arcade-button-box

Useful visual reference for a simple box dominated by large tactile controls. Exact licensing/fit should be checked before reusing any geometry.

## Proposed public-project structure

```text
hardware/case/
├── README.md
├── source/
│   └── medicine-alarm-case.scad
├── stl/
├── 3mf/
└── photos/
```

The editable source is the canonical artifact; STL/3MF are generated releases.

## License direction

Software remains Apache-2.0.

For original enclosure CAD, choose a dedicated hardware/design license when the first source model is published. CERN-OHL-P is a strong candidate for open hardware; CC BY 4.0 is another simple option for design files.

## Gate before final CAD

Before locking dimensions:

1. measure the actual D1 R32 board;
2. measure the exact DS3231 module;
3. measure MAX98357A;
4. measure the chosen speaker;
5. confirm USB connector location and cable clearance;
6. validate the large-button mechanism with a quick test print.

A rough parametric shell can be developed before those parts arrive, but the final mounting geometry waits for physical measurements.
