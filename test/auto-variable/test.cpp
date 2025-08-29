// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

int foo(int x)
{
    auto y = x; // EXPECT(auto-variable)
    return y;
}

auto bar(int x)
{
    return x;
}

auto baz(int x) -> int
{
    return x;
}
