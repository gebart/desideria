/*
 * Copyright (c) 2020 Joakim Nohlgård
 */
#include "global_ctor_test.h"

namespace test {
Sut sut(&canary);

bool Sut::verify(const void *actual) { return expected == actual; }
Sut::Sut(const void *expected) : expected(expected) {}
} // namespace test
