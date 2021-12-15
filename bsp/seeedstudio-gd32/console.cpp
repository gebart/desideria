/*
 * Copyright (C) 2020 Joakim Nohlgård <joakim@nohlgard.se>
 */

#include "deri/bsp/console.hpp"

#include "deri/bsp/devices.hpp"
#include "deri/soc/clock_dev.hpp"
#include "deri/soc/gpio_dev.hpp"

namespace deri {
using namespace dev;
namespace bsp {
void console_init() {
  gpio::Gpio uart_pin{dev::gpio::Gpio::Port::A, 9};
  soc::gpio.initOutAfio(uart_pin);

  soc::rcu.enableModules(clock::RcuGd32::APB2EN_bits::USART0EN);
  uart0.init();
  uart0.setBaud(soc::rcu.apb2Freq(), 115200);
}
bsp::Console &console() {
  static bsp::Console &instance = []() -> auto & {
    static bsp::Console instance{bsp::uart0};
    bsp::console_init();
    return instance;
  }
  ();
  return instance;
}
}  // namespace bsp
}  // namespace deri
