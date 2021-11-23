/*
 * Copyright (c) 2021 Joakim Nohlgård
 */

#include "deri/arch_init.hpp"
#include "deri/dev/afio_gd32.hpp"
#include "deri/soc/gpio_dev.hpp"
#include "deri/soc/irq_dev.hpp"
#include "deri/soc/vectors.hpp"

namespace deri::soc {
void init() {
  arch::init();
  clic.init();
  dev::irq::IrqClic::setVectorTable(vector_table);
  soc::afio.init();
  arch::irq_enable();
}
}  // namespace deri::soc
