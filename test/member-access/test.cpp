// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

struct MyStruct
{
    int a;
    private: int b; // EXPECT(member-access)
    protected: int c; // EXPECT(member-access)
    public: int d; // EXPECT(member-access)
};

class MyClass
{
    int a;
    private: int b; // EXPECT(member-access)
    protected: int c; // EXPECT(member-access)
    public: int d; // EXPECT(member-access)
};

struct BaseStruct
{
};
struct BaseStructDerived : BaseStruct
{
};
struct BaseStructDerivedPrivate : private BaseStruct
{
};
struct BaseStructDerivedProtected : protected BaseStruct
{
};
struct BaseStructDerivedPublic : public BaseStruct
{
};

class BaseClass
{
};
class BaseClassDerived : BaseClass
{
};
class BaseClassDerivedPrivate : private BaseClass
{
};
class BaseClassDerivedProtected : protected BaseClass
{
};
class BaseClassDerivedPublic : public BaseClass
{
};
