#include "Scheduler.h"

std::uint64_t makeMinuteKey(const ClockSnapshot& now) {
  if (
    !now.valid ||
    now.month < 1 ||
    now.month > 12 ||
    now.day < 1 ||
    now.day > 31 ||
    now.hour > 23 ||
    now.minute > 59
  ) {
    return 0;
  }

  return
    static_cast<std::uint64_t>(now.year) * 100000000ULL +
    static_cast<std::uint64_t>(now.month) * 1000000ULL +
    static_cast<std::uint64_t>(now.day) * 10000ULL +
    static_cast<std::uint64_t>(now.hour) * 100ULL +
    static_cast<std::uint64_t>(now.minute);
}

std::uint16_t dueAlarmMask(
  const ScheduleSlot* alarms,
  std::size_t count,
  const ClockSnapshot& now,
  std::uint64_t lastTriggeredMinuteKey
) {
  if (alarms == nullptr || count == 0 || !now.valid) {
    return 0;
  }

  const std::uint64_t minuteKey = makeMinuteKey(now);

  if (minuteKey == 0 || minuteKey == lastTriggeredMinuteKey) {
    return 0;
  }

  std::uint16_t mask = 0;
  const std::size_t limit = count > 16 ? 16 : count;

  for (std::size_t i = 0; i < limit; ++i) {
    if (
      alarms[i].enabled &&
      alarms[i].hour == now.hour &&
      alarms[i].minute == now.minute
    ) {
      mask |= static_cast<std::uint16_t>(1U << i);
    }
  }

  return mask;
}
