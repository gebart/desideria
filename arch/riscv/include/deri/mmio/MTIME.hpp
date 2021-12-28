/*
 * Copyright (c) 2021 Joakim Nohlgård
 */

#pragma once

#include "deri/registers.hpp"

#include <array>
#include <cstdint>

namespace deri::mmio {
enum class MTIME_bits : int64_t;

extern Register<MTIME_bits> mtime;
extern Register<MTIME_bits> mtimecmp;
}  // namespace deri::mmio
