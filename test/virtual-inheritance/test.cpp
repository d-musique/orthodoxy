// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

class A
{
};

class B : A
{
};

class C : virtual A // EXPECT(virtual-inheritance)
{
};
