/*
 * Copyright (c) 2021 Joakim Nohlgård
 */

#include "deri/console.hpp"
#include "deri/span_literals.hpp"
#include "deri/testing/finish.hpp"

#include <cstdlib>
#include <span>

using namespace deri::literals;

namespace deri::testing {
namespace {
[[noreturn]] void finish_pass() {
  console.write(std::as_bytes("\n\n === TESTS PASS === \n\n"_span));
  std::exit(EXIT_SUCCESS);
}
[[noreturn]] void finish_fail(int code) {
  console.write(std::as_bytes("\n\n >>> TESTS FAILED <<<\n\n"_span));
  std::exit(code);
}
}  // namespace

void finish(int code) {
  if (code == 0) {
    finish_pass();
  } else {
    finish_fail(code);
  }
}

}  // namespace deri::testing
