// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

struct A
{
    virtual ~A() = default;
};

struct B : A
{
};

B *testC(A *x)
{
    return (B *)x;
}

B *testStatic(A *x)
{
    return static_cast<B *>(x);
}

B *testDynamic(A *x)
{
    return dynamic_cast<B *>(x);
}

A *testConst(const A *x)
{
    return const_cast<A *>(x); // EXPECT(const-cast)
}
