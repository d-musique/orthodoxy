// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

void foo()
{
    int *x;

    x = new int; // EXPECT(new)
    delete x; // EXPECT(new)

    x = new int[100]; // EXPECT(new)
    delete[] x; // EXPECT(new)
}
