/*
 * Copyright (c) 2020 Joakim Nohlgård
 */

#pragma once

#include "deri/dev/gpio_gd32.h"
#include "deri/dev/uart_gd32.h"
#include "deri/soc/gpio_dev.h"

#include <array>

namespace deri::bsp {
extern deri::dev::uart::UsartGd32 uart0;

inline constexpr std::array leds{
    dev::gpio::Gpio{dev::gpio::Gpio::Port::B, 5},
    dev::gpio::Gpio{dev::gpio::Gpio::Port::B, 0},
    dev::gpio::Gpio{dev::gpio::Gpio::Port::B, 1},
};

inline constexpr std::array buttons{
    dev::gpio::Gpio{dev::gpio::Gpio::Port::A, 0},
    dev::gpio::Gpio{dev::gpio::Gpio::Port::C, 13},
};
}  // namespace deri::bsp
