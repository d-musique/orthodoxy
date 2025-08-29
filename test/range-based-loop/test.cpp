// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>
#include <cstdio>

void foo(const std::string &s)
{
    for (std::size_t i = 0; i < s.size(); ++i)
        std::putchar(s[i]);

    for (char c : s) // EXPECT(range-based-loop)
        std::putchar(c);
}
