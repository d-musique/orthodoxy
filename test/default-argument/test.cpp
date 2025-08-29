// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

int foo(int x)
{
    return x;
}

int bar(int x = 1) // EXPECT(default-argument)
{
    return x;
}
