// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstddef>

std::size_t operator ""_n(const char *, std::size_t n) // EXPECT(user-defined-literal)
{
    return n;
}
