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
struct BaseStructDerivedPrivate : private BaseStruct // EXPECT(inheritance-access)
{
};
struct BaseStructDerivedProtected : protected BaseStruct // EXPECT(inheritance-access)
{
};
struct BaseStructDerivedPublic : public BaseStruct // EXPECT(inheritance-access)
{
};

class BaseClass
{
};
class BaseClassDerived : BaseClass
{
};
class BaseClassDerivedPrivate : private BaseClass // EXPECT(inheritance-access)
{
};
class BaseClassDerivedProtected : protected BaseClass // EXPECT(inheritance-access)
{
};
class BaseClassDerivedPublic : public BaseClass // EXPECT(inheritance-access)
{
};
