#pragma once

#include <cstdint>

struct ScheduleSlot {
  std::uint8_t hour;
  std::uint8_t minute;
  std::uint8_t enabled;
};
