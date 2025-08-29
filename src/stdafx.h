// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <utility>
#include <type_traits>
#include <cassert>
#include <cstddef>
#include <cstdint>

#define BEFORE_LLVM \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")

#define AFTER_LLVM \
    _Pragma("GCC diagnostic pop")
