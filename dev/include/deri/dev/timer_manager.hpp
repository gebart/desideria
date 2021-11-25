/*
 * Copyright (c) 2021 Joakim Nohlgård
 */

#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>

namespace deri::dev {

template <class TimerDriverType, typename CountType = std::uint64_t>
class TimerManager {
 public:
  using TimerDriver = TimerDriverType;
  using Count = CountType;
  enum class Frequency : unsigned long;
  static constexpr auto num_channels = TimerDriver::num_channels;

  explicit TimerManager(TimerDriver &timer, Frequency tick_rate_hz)
      : timer(&timer), tick_rate_hz{tick_rate_hz}, count(0) {}
  TimerManager() = default;
  TimerManager(const TimerManager &) = default;
  TimerManager &operator=(const TimerManager &rhs) noexcept {
    if (this == &rhs) {
      return *this;
    }
    timer = rhs.timer;
    tick_rate_hz = rhs.tick_rate_hz;
    count.store(rhs.count.load());
    return *this;
  }

  void init() {
    timer->setPeriod(
        static_cast<typename TimerDriver::Count>(TimerDriver::max_value));
    timer->setPeriodHandler({[](uintptr_t ctx) {
                               auto *self =
                                   reinterpret_cast<decltype(this)>(ctx);
                               self->onRollover();
                             },
                             reinterpret_cast<uintptr_t>(this)});
    timer->start();
  }

  Count read() const {
    return count + static_cast<Count>(timer->read());
  }

 private:
  void onRollover() { count += static_cast<Count>(TimerDriver::max_value) + 1; }

  TimerDriver *timer{nullptr};
  Frequency tick_rate_hz{};
  std::atomic_uint64_t count{};
};
}  // namespace deri::dev
