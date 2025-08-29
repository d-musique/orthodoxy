// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

struct MyStruct
{
    operator bool() const; // EXPECT(conversion-overload)
    MyStruct &operator<<(int); // EXPECT(operator-overload)
    void method(int x); // EXPECT(function-overload)
    void method(float x); // EXPECT(function-overload)
    void method(int x, int y); // EXPECT(function-overload)
    void othermethod(long x);
};

MyStruct &operator>>(MyStruct &, int); // EXPECT(operator-overload)

void function(int x); // EXPECT(function-overload)
void function(float x); // EXPECT(function-overload)
void function(int x, int y); // EXPECT(function-overload)
void otherfunction(long x);
