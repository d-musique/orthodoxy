// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

auto foo()
{
    return []() -> int { return 4; }; // EXPECT(lambda)
}

auto bar(int x)
{
    return [x]() -> int { return x; }; // EXPECT(lambda)
}
