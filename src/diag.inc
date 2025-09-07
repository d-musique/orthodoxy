// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

DEFINE_DIAGNOSTIC(
    Include, "include", nullptr,
    "including this header is not allowed");

DEFINE_DIAGNOSTIC(
    Class, "class", nullptr,
    "class is not allowed");

DEFINE_DIAGNOSTIC(
    NonStandardLayout, "non-standard-layout", &NonPOD(),
    "non-standard-layout types are not allowed");

DEFINE_DIAGNOSTIC(
    NonTrivial, "non-trivial", &NonPOD(),
    "non-trivial types are not allowed");

DEFINE_DIAGNOSTIC(
    NonPOD, "non-pod", nullptr,
    "non-POD types are not allowed");

DEFINE_DIAGNOSTIC(
    EnumClass, "enum-class", nullptr,
    "enum class is not allowed");

DEFINE_DIAGNOSTIC(
    MemberFunction, "member-function", nullptr,
    "member functions are not allowed");

DEFINE_DIAGNOSTIC(
    VirtualMemberFunction, "virtual-member-function", &MemberFunction(),
    "virtual member functions are not allowed");

DEFINE_DIAGNOSTIC(
    Constructor, "constructor", nullptr,
    "constructors are not allowed");

DEFINE_DIAGNOSTIC(
    Destructor, "destructor", nullptr,
    "destructors are not allowed");

DEFINE_DIAGNOSTIC(
    Inheritance, "inheritance", nullptr,
    "inheritance is not allowed");

DEFINE_DIAGNOSTIC(
    VirtualInheritance, "virtual-inheritance", &Inheritance(),
    "virtual inheritance is not allowed");

DEFINE_ABSTRACT_DIAGNOSTIC(
    Exception, "exception", nullptr);

DEFINE_DIAGNOSTIC(
    Throw, "throw", &Exception(),
    "throwing is not allowed");

DEFINE_DIAGNOSTIC(
    TryCatch, "try-catch", &Exception(),
    "try/catch blocks are not allowed");

DEFINE_DIAGNOSTIC(
    Template, "template", nullptr,
    "templates are not allowed");

DEFINE_DIAGNOSTIC(
    Concept, "concept", &Template(),
    "concepts are not allowed");

DEFINE_DIAGNOSTIC(
    Access, "access", nullptr,
    "access specifiers are not allowed");

DEFINE_DIAGNOSTIC(
    MemberAccess, "member-access", &Access(),
    "member access specifiers are not allowed");

DEFINE_DIAGNOSTIC(
    InheritanceAccess, "inheritance-access", &Access(),
    "inheritance access specifiers are not allowed");

DEFINE_DIAGNOSTIC(
    ImplicitThis, "implicit-this", nullptr,
    "implicit this is not allowed");

DEFINE_DIAGNOSTIC(
    CStyleCast, "c-style-cast", nullptr,
    "C-style casts are not allowed");

DEFINE_DIAGNOSTIC(
    NamedCast, "named-cast", nullptr,
    "named casts are not allowed");

DEFINE_DIAGNOSTIC(
    DynamicCast, "dynamic-cast", &NamedCast(),
    "dynamic casts are not allowed");

DEFINE_DIAGNOSTIC(
    StaticCast, "static-cast", &NamedCast(),
    "static casts are not allowed");

DEFINE_DIAGNOSTIC(
    ConstCast, "const-cast", &NamedCast(),
    "const casts are not allowed");

DEFINE_DIAGNOSTIC(
    ReinterpretCast, "reinterpret-cast", &NamedCast(),
    "reinterpret casts are not allowed");

DEFINE_ABSTRACT_DIAGNOSTIC(
    Overload, "overload", nullptr);

DEFINE_DIAGNOSTIC(
    FunctionOverload, "function-overload", &Overload(),
    "function overloading is not allowed");

DEFINE_DIAGNOSTIC(
    OperatorOverload, "operator-overload", &Overload(),
    "operator overloading is not allowed");

DEFINE_DIAGNOSTIC(
    ConversionOverload, "conversion-overload", &Overload(),
    "conversion operator overloading is not allowed");

DEFINE_DIAGNOSTIC(
    Reference, "reference", nullptr,
    "references are not allowed");

DEFINE_DIAGNOSTIC(
    RValueReference, "rvalue-reference", &Reference(),
    "r-value references are not allowed");

DEFINE_DIAGNOSTIC(
    LValueReference, "lvalue-reference", &Reference(),
    "l-value references are not allowed");

DEFINE_ABSTRACT_DIAGNOSTIC(
    Module, "module", nullptr);

DEFINE_DIAGNOSTIC(
    ModuleImport, "module-import", &Module(),
    "importing modules is not allowed");

DEFINE_DIAGNOSTIC(
    ModuleExport, "module-export", &Module(),
    "exporting modules is not allowed");

DEFINE_DIAGNOSTIC(
    RangeBasedLoop, "range-based-loop", nullptr,
    "range-based loops are not allowed");

DEFINE_DIAGNOSTIC(
    Lambda, "lambda", nullptr,
    "lambdas are not allowed");

DEFINE_DIAGNOSTIC(
    LambdaCapture, "lambda-capture", &Lambda(),
    "capturing lambdas are not allowed");

DEFINE_DIAGNOSTIC(
    New, "new", nullptr,
    "new/delete is not allowed");

DEFINE_ABSTRACT_DIAGNOSTIC(
    Auto, "auto", nullptr);

DEFINE_DIAGNOSTIC(
    AutoReturn, "auto-return", &Auto(),
    "functions with automatic return type are not allowed");

DEFINE_DIAGNOSTIC(
    AutoVariable, "auto-variable", &Auto(),
    "variables with automatic type are not allowed");

DEFINE_DIAGNOSTIC(
    UserDefinedLiteral, "user-defined-literal", nullptr,
    "user-defined literals are not allowed");

DEFINE_DIAGNOSTIC(
    DefaultArgument, "default-argument", nullptr,
    "default arguments are not allowed");

DEFINE_DIAGNOSTIC(
    Namespace, "namespace", nullptr,
    "namespaces are not allowed");
