// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

void foo()
{
    static_cast<void>(0); // EXPECT(static-cast)

    /* 1 level */

    #define Expand() static_cast<void>(0)
    Expand(); // EXPECT(static-cast)
    #undef Expand

    #define Expand() static_cast<void>(0) // HERESY(static-cast)
    Expand();
    #undef Expand

    /* 2 levels */

    #define Expand() Expand_()
    #define Expand_() static_cast<void>(0)
    Expand(); // EXPECT(static-cast)
    #undef Expand
    #undef Expand_

    #define Expand() Expand_() // HERESY(static-cast)
    #define Expand_() static_cast<void>(0)
    Expand();
    #undef Expand
    #undef Expand_

    #define Expand() Expand_()
    #define Expand_() static_cast<void>(0) // HERESY(static-cast)
    Expand();
    #undef Expand
    #undef Expand_

    /* 3 levels */

    #define Expand() Expand_()
    #define Expand_() Expand__()
    #define Expand__() static_cast<void>(0)
    Expand(); // EXPECT(static-cast)
    #undef Expand
    #undef Expand_
    #undef Expand__

    #define Expand() Expand_() // HERESY(static-cast)
    #define Expand_() Expand__()
    #define Expand__() static_cast<void>(0)
    Expand();
    #undef Expand
    #undef Expand_
    #undef Expand__

    #define Expand() Expand_()
    #define Expand_() Expand__() // HERESY(static-cast)
    #define Expand__() static_cast<void>(0)
    Expand();
    #undef Expand
    #undef Expand_
    #undef Expand__

    #define Expand() Expand_()
    #define Expand_() Expand__()
    #define Expand__() static_cast<void>(0) // HERESY(static-cast)
    Expand();
    #undef Expand
    #undef Expand_
    #undef Expand__
}
