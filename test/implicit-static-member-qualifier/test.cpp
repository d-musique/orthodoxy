// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

struct MyClass
{
    using MyAlias = MyClass;

    static void MyStaticFunc() {}
    static int MyStaticVar;
    enum MyEnum { E_1 };

    void quux()
    {
        MyStaticFunc(); // EXPECT(implicit-static-member-qualifier)
        MyClass::MyStaticFunc();
        MyAlias::MyStaticFunc();

        (void)MyStaticVar; // EXPECT(implicit-static-member-qualifier)
        (void)MyClass::MyStaticVar;
        (void)MyAlias::MyStaticVar;

        (void)MyEnum{};
        (void)MyClass::MyEnum{};
        (void)MyAlias::MyEnum{};

        (void)E_1;
        (void)MyClass::E_1;
        (void)MyAlias::E_1;
    }
};
