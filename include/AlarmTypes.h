#pragma once

#include <cstdint>

struct AlarmSlot {
  std::uint8_t hour;
  std::uint8_t minute;
  std::uint8_t enabled;
};
