/*
 * Copyright (c) 2021 Joakim Nohlgård
 */

#include "deri/dev/uart_gd32.h"

#include "deri/bitwise_enums.h"
#include "deri/mmio/bits/USART_bits.hpp"

namespace deri::dev::uart {

void UsartGd32::init() {
  auto ctl0 = CTL0.load();
  ctl0 &= CTL0_bits::Reserved_mask;
  ctl0 |= CTL0_bits::UEN;
  CTL0.store(ctl0);
}

void UsartGd32::transmitterOn() {
  CTL0 |= CTL0_bits::TEN;
}

void UsartGd32::transmitterOff() {
  CTL0 &= ~CTL0_bits::TEN;
}

void UsartGd32::setBaud(unsigned pclk, unsigned baudrate) {
  auto baud = BAUD.load();
  baud &= BAUD_bits::Reserved_mask;
  uint32_t divider = (pclk + baudrate / 2) / baudrate;
  baud |= static_cast<BAUD_bits>(divider) &
      (BAUD_bits::INTDIV_mask | BAUD_bits::FRADIV_mask);
  BAUD.store(baud);
}

auto UsartGd32::write(std::span<const std::byte> buffer) -> decltype(buffer) {
  while (!buffer.empty() && STAT.any(STAT_bits::TBE)) {
    DATA.store(static_cast<DATA_bits>(buffer.front()));
    buffer = buffer.last(buffer.size() - 1);
  }
  return buffer;
}

static_assert(sizeof(UsartGd32) == 0x1C);

}  // namespace deri::hal::periph
