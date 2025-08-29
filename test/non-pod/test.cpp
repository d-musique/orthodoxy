// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

struct POD
{
    int i;
    int j;
};

struct StandardLayout // EXPECT(non-pod)
{
    int i;
    StandardLayout(int i) : i(i) {}
};

struct Trivial // EXPECT(non-pod)
{
public: int i;
private: int j;
};
