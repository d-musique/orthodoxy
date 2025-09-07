// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

struct MyStruct
{
    MyStruct &operator=(const MyStruct &); // EXPECT(copy-assignment-operator)
    MyStruct &operator=(MyStruct &&); // EXPECT(move-assignment-operator)
};
