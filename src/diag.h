// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "diagdesc.h"

#if !defined(DEFINE_DIAGNOSTIC)
#define DEFINE_DIAGNOSTIC(id, alt, msg) \
    namespace Orthodoxy { namespace diag { \
        extern const OrthodoxyDiagDesc id; } }
#endif

DEFINE_DIAGNOSTIC(
    Include,
    include,
    "including this header is not allowed");

DEFINE_DIAGNOSTIC(
    Class,
    class,
    "class is not allowed");

DEFINE_DIAGNOSTIC(
    NonStandardLayout,
    non-standard-layout,
    "non-standard-layout types are not allowed");

DEFINE_DIAGNOSTIC(
    NonTrivial,
    non-trivial,
    "non-trivial types are not allowed");

DEFINE_DIAGNOSTIC(
    NonPOD,
    non-pod,
    "non-POD types are not allowed");

DEFINE_DIAGNOSTIC(
    EnumClass,
    enum-class,
    "enum class is not allowed");

DEFINE_DIAGNOSTIC(
    MemberFunction,
    member-function,
    "member functions are not allowed");

DEFINE_DIAGNOSTIC(
    VirtualMemberFunction,
    virtual-member-function,
    "virtual member functions are not allowed");

DEFINE_DIAGNOSTIC(
    Constructor,
    constructor,
    "constructors are not allowed");

DEFINE_DIAGNOSTIC(
    Destructor,
    destructor,
    "destructors are not allowed");

DEFINE_DIAGNOSTIC(
    Inheritance,
    inheritance,
    "inheritance is not allowed");

DEFINE_DIAGNOSTIC(
    VirtualInheritance,
    virtual-inheritance,
    "virtual inheritance is not allowed");

DEFINE_DIAGNOSTIC(
    Throw,
    throw,
    "throwing is not allowed");

DEFINE_DIAGNOSTIC(
    TryCatch,
    try-catch,
    "try/catch blocks are not allowed");

DEFINE_DIAGNOSTIC(
    Template,
    template,
    "templates are not allowed");

DEFINE_DIAGNOSTIC(
    Concept,
    concept,
    "concepts are not allowed");

DEFINE_DIAGNOSTIC(
    Access,
    access,
    "access specifiers are not allowed");

DEFINE_DIAGNOSTIC(
    MemberAccess,
    member-access,
    "member access specifiers are not allowed");

DEFINE_DIAGNOSTIC(
    InheritanceAccess,
    inheritance-access,
    "inheritance access specifiers are not allowed");

DEFINE_DIAGNOSTIC(
    ImplicitThis,
    implicit-this,
    "implicit this is not allowed");

DEFINE_DIAGNOSTIC(
    CStyleCast,
    c-style-cast,
    "C-style casts are not allowed");

DEFINE_DIAGNOSTIC(
    NamedCast,
    named-cast,
    "named casts are not allowed");

DEFINE_DIAGNOSTIC(
    DynamicCast,
    dynamic-cast,
    "dynamic casts are not allowed");

DEFINE_DIAGNOSTIC(
    StaticCast,
    static-cast,
    "static casts are not allowed");

DEFINE_DIAGNOSTIC(
    ConstCast,
    const-cast,
    "const casts are not allowed");

DEFINE_DIAGNOSTIC(
    ReinterpretCast,
    reinterpret-cast,
    "reinterpret casts are not allowed");

DEFINE_DIAGNOSTIC(
    FunctionOverload,
    function-overload,
    "function overloading is not allowed");

DEFINE_DIAGNOSTIC(
    OperatorOverload,
    operator-overload,
    "operator overloading is not allowed");

DEFINE_DIAGNOSTIC(
    ConversionOverload,
    conversion-overload,
    "conversion operator overloading is not allowed");

DEFINE_DIAGNOSTIC(
    Reference,
    reference,
    "references are not allowed");

DEFINE_DIAGNOSTIC(
    RValueReference,
    rvalue-reference,
    "r-value references are not allowed");

DEFINE_DIAGNOSTIC(
    LValueReference,
    lvalue-reference,
    "l-value references are not allowed");

DEFINE_DIAGNOSTIC(
    ModuleImport,
    module-import,
    "importing modules is not allowed");

DEFINE_DIAGNOSTIC(
    ModuleExport,
    module-export,
    "exporting modules is not allowed");

DEFINE_DIAGNOSTIC(
    RangeBasedLoop,
    range-based-loop,
    "range-based loops are not allowed");

DEFINE_DIAGNOSTIC(
    Lambda,
    lambda,
    "lambdas are not allowed");

DEFINE_DIAGNOSTIC(
    LambdaCapture,
    lambda-capture,
    "capturing lambdas are not allowed");

DEFINE_DIAGNOSTIC(
    New,
    new,
    "new/delete is not allowed");

DEFINE_DIAGNOSTIC(
    AutoReturn,
    auto-return,
    "functions with automatic return type are not allowed");

DEFINE_DIAGNOSTIC(
    AutoVariable,
    auto-variable,
    "variables with automatic type are not allowed");

DEFINE_DIAGNOSTIC(
    UserDefinedLiteral,
    user-defined-literal,
    "user-defined literals are not allowed");

DEFINE_DIAGNOSTIC(
    DefaultArgument,
    default-argument,
    "default arguments are not allowed");

DEFINE_DIAGNOSTIC(
    Namespace,
    namespace,
    "namespaces are not allowed");
