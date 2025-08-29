// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

struct MyStruct
{
    operator bool() const; // EXPECT(conversion-overload)
    MyStruct &operator<<(int);
    void method(int x);
    void method(float x);
    void method(int x, int y);
    void othermethod(long x);
};

MyStruct &operator>>(MyStruct &, int);

void function(int x);
void function(float x);
void function(int x, int y);
void otherfunction(long x);
