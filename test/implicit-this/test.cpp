// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

class MyClass
{
    int x;

    void doThing();

    int implicitThis()
    {
        doThing(); // EXPECT(implicit-this)
        return x; // EXPECT(implicit-this)
    }

    int explicitThis()
    {
        this->doThing();
        return this->x;
    }
};
