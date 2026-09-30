#pragma once

#include <cstddef>
#include <cstdint>

#include "AlarmTypes.h"

struct ClockSnapshot {
  std::uint16_t year;
  std::uint8_t month;
  std::uint8_t day;
  std::uint8_t hour;
  std::uint8_t minute;
  bool valid;
};

std::uint64_t makeMinuteKey(const ClockSnapshot& now);

std::uint16_t dueAlarmMask(
  const ScheduleSlot* alarms,
  std::size_t count,
  const ClockSnapshot& now,
  std::uint64_t lastTriggeredMinuteKey
);
