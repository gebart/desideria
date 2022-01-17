/*
 * Copyright (C) 2022 Joakim Nohlgård <joakim@nohlgard.se>
 */

#include "deri/log.hpp"
#include "deri/unit_test.hpp"

using namespace deri::test;

int sut(int num) { return 2 * num; }

namespace log {
using namespace deri::log;
struct Test;
struct Test : LogConfig<Level::INFO> {};
}  // namespace log

using Logger = deri::log::Logger<log::Test>;

using namespace deri::literals;

void test_true() { check % true; }

void test_false() { check % false != true; }

int main() {
  int count = 0;
  "Subcases"_test = [&count] {
    ++count;
    auto actual = sut(12);
    auto expected = 24;
    check % actual == expected;
    Logger::info("Before subcases\n");
    "subcase1"_test = [] {
      Logger::info(" First subcase\n");
      "subsub1"_test = [] { Logger::info("  in subsub1\n"); };
      "subsub2"_test = [] { Logger::info("  in subsub2\n"); };
    };
    "subcase2"_test = [&count] {
      check % count == 3;
      Logger::info(" Second subcase\n");
    };
    "subcase3"_test = [] {
      Logger::info(" Third subcase\n");
      "subsub1"_test = [] {
        Logger::info("  in 3-1\n");
        "subsubsub"_test = [] { Logger::info("   in 3-1-1\n"); };
        "subsubsub"_test = [] { Logger::info("   in 3-1-2\n"); };
      };
    };
    "sub4"_test = [] { Logger::info(" in 4\n"); };
    Logger::info("After subcases\n");
  };
  Logger::info("Top level was called ") << count << " times\n";
  check % count == 4 + 2;  // number of leaf nodes in the tree above
  check % count < 10;
  check % count > 1;
  check % count - 6 == 0 || false;
  test_false();
  test_true();
  logTestSummary(Logger::error);
  return 0;
}
