// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

class MyClass
{
    MyClass(); // EXPECT(constructor)
    MyClass(const MyClass &); // EXPECT(copy-constructor)
    MyClass(MyClass &&); // EXPECT(move-constructor)
    MyClass(int); // EXPECT(conversion-constructor)
    explicit MyClass(float); // EXPECT(constructor)
    ~MyClass();
};
