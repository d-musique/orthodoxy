// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

class MyClass
{
    MyClass();
    MyClass(const MyClass &);
    MyClass(MyClass &&);
    MyClass(int); // EXPECT(conversion-constructor)
    explicit MyClass(float);
    ~MyClass();
};
