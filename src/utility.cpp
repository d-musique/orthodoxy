// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "utility.h"
BEFORE_LLVM
#include <clang/AST/Decl.h>
#include <clang/Basic/Version.h>
AFTER_LLVM

llvm::raw_ostream &Orthodoxy::error_message()
{
    return llvm::errs() << "orthodoxy ERROR: ";
}

llvm::raw_ostream &Orthodoxy::warning_message()
{
    return llvm::errs() << "orthodoxy WARNING: ";
}

bool Orthodoxy::TypeIsReference(clang::QualType qty)
{
    return TypeIsReference(qty.getTypePtrOrNull());
}

bool Orthodoxy::TypeIsReference(const clang::Type *ty)
{
    if (!ty) return false;
    clang::Type::TypeClass tc = ty->getTypeClass();
    return tc == clang::Type::TypeClass::LValueReference ||
        tc == clang::Type::TypeClass::RValueReference;
}

bool Orthodoxy::TypeIsRvalueReference(clang::QualType qty)
{
    return TypeIsRvalueReference(qty.getTypePtrOrNull());
}

bool Orthodoxy::TypeIsRvalueReference(const clang::Type *ty)
{
    if (!ty) return false;
    clang::Type::TypeClass tc = ty->getTypeClass();
    return tc == clang::Type::TypeClass::RValueReference;
}

bool Orthodoxy::TypeIsAuto(clang::QualType qty)
{
    return TypeIsAuto(qty.getTypePtrOrNull());
}

bool Orthodoxy::TypeIsAuto(const clang::Type *ty)
{
    if (!ty) return false;
    return ty->getContainedAutoType() != nullptr;
}

bool Orthodoxy::TagIsClass(const clang::TagDecl *TD)
{
#if CLANG_VERSION_MAJOR >= 18
    return TD->getTagKind() == clang::TagTypeKind::Class;
#else
    return TD->getTagKind() == clang::TTK_Class;
#endif
}

bool Orthodoxy::TagIsEnumClass(const clang::TagDecl *TD)
{
    return TD->isEnum() && llvm::cast<clang::EnumDecl>(TD)->isScoped();
}
