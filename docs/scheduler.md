# Scheduler core

The scheduling rule can be implemented and validated independently from the physical RTC.

## Input

The scheduler receives:

- the persisted daily alarm slots;
- a `ClockSnapshot` containing calendar date, hour and minute;
- the minute key of the last alarm event already fired.

The future DS3231 integration will be responsible only for producing a valid `ClockSnapshot`.

## Output

`dueAlarmMask()` returns a bit mask of all enabled alarm slots matching the current hour/minute.

This preserves all matching slots if more than one reminder is configured for the same minute.

## Duplicate prevention

A minute is encoded as:

```text
YYYYMMDDHHMM
```

Example:

```text
2026-09-29 08:15 -> 202609290815
```

When an alarm fires, the firmware should persist that minute key to NVS. If the device reboots during the same minute, the scheduler receives the persisted key and does not fire the same scheduled event again.

## Current scope

The MVP scheduler is daily:

- each enabled slot is evaluated every day;
- weekday-specific rules are deferred unless real use demonstrates the need.

## Hardware boundary

```text
DS3231
  |
  v
ClockSnapshot
  |
  v
dueAlarmMask()
  |
  +---- no match ----> IDLE
  |
  +---- match --------> ALARM
                         |
                         +--> voice / LED
                         +--> acknowledge button
```

The scheduler core has no dependency on Arduino, I2C or a DS3231 library.
