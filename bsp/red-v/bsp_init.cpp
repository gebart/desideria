/*
 * Copyright (c) 2020 Joakim Nohlgård
 */

#include "deri/bsp_init.hpp"

#include "deri/bsp/config.hpp"
#include "deri/console.hpp"
#include "deri/soc/init.hpp"
#include "deri/span_literals.hpp"
#include "deri/time.hpp"

using namespace deri::literals;

namespace deri::bsp {
void init() {
  soc::init();
  sys::time::init();
  Console::write(std::as_bytes("Red 5 standing by\n"_span));
}
}  // namespace deri::bsp
