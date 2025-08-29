// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

void doThing();

void foo()
{
    try // EXPECT(try-catch)
    {
        doThing();
    }
    catch (...)
    {
        throw;
    }

    throw 1;
}
