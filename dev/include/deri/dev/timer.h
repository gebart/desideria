/*
 * Copyright (c) 2021 Joakim Nohlgård
 */

#pragma once

#include "deri/delegate.h"

#include <array>
#include <cstdint>

namespace deri::dev::timer {

template <class TimerDeviceType>
class TimerDriver {
 public:
  using TimerDevice = TimerDeviceType;
  using Count = typename TimerDevice::Count;
  using Channel = typename TimerDevice::Channel;
  static constexpr auto max_value = TimerDevice::max_value;
  static constexpr auto num_channels = TimerDevice::num_channels;

  using Callback = delegate<void(Channel)>;
  using PeriodCallback = delegate<void()>;

  explicit TimerDriver(TimerDevice &timer) : timer(timer) {}

  void init() { timer.init(); }

  [[nodiscard]] Count read() const { return timer.read(); }
  void start() { timer.start(); }
  void stop() { timer.stop(); }

  void setPeriod(Count period) { timer.setPeriod(period); }
  void setCompare(Channel channel, Count target) {
    timer.setCompare(channel, target);
  }

  void setInterruptHandler(Channel channel, Callback callback);
  void clearInterruptHandler(Channel channel);
  void setPeriodHandler(PeriodCallback callback);
  void clearPeriodHandler();

  void interruptCallback(Channel channel) {
    timer.clearInterruptFlag(channel);
    auto &&callback = callbacks[static_cast<unsigned>(channel)];
    callback(channel);
  }

  void periodCallback() {
    timer.clearPeriodFlag();
    period_callback();
  }

  const TimerDevice &underlyingTimer() const { return timer; }
  TimerDevice &underlyingTimer() { return timer; }

 private:
  TimerDevice &timer;
  std::array<Callback, TimerDevice::num_channels> callbacks{};
  PeriodCallback period_callback{};
};
template <class TimerDeviceType>
void TimerDriver<TimerDeviceType>::setInterruptHandler(Channel channel,
                                                       Callback callback) {
  timer.disableInterrupt(channel);
  timer.clearInterruptFlag(channel);
  callbacks[static_cast<unsigned>(channel)] = callback;
  timer.enableInterrupt(channel);
}

template <class TimerDeviceType>
void TimerDriver<TimerDeviceType>::clearInterruptHandler(Channel channel) {
  timer.disableInterrupt(channel);
  timer.clearInterruptFlag(channel);
  callbacks[static_cast<unsigned>(channel)].clear();
}

template <class TimerDeviceType>
void TimerDriver<TimerDeviceType>::setPeriodHandler(PeriodCallback callback) {
  timer.disablePeriodInterrupt();
  timer.clearPeriodFlag();
  period_callback = callback;
  timer.enablePeriodInterrupt();
}

template <class TimerDeviceType>
void TimerDriver<TimerDeviceType>::clearPeriodHandler() {
  timer.disablePeriodInterrupt();
  timer.clearPeriodFlag();
  period_callback = {};
}
}  // namespace deri::dev::timer
