// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "utility.h"
BEFORE_LLVM
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
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

bool Orthodoxy::TypeIsLvalueReference(clang::QualType qty)
{
    return TypeIsLvalueReference(qty.getTypePtrOrNull());
}

bool Orthodoxy::TypeIsLvalueReference(const clang::Type *ty)
{
    if (!ty) return false;
    clang::Type::TypeClass tc = ty->getTypeClass();
    return tc == clang::Type::TypeClass::LValueReference;
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

bool Orthodoxy::FunctionIsCopyConstructor(const clang::FunctionDecl *FD)
{
    const clang::CXXConstructorDecl *ctor = llvm::dyn_cast<clang::CXXConstructorDecl>(FD);
    return ctor && ctor->isCopyConstructor();
}

bool Orthodoxy::FunctionIsMoveConstructor(const clang::FunctionDecl *FD)
{
    const clang::CXXConstructorDecl *ctor = llvm::dyn_cast<clang::CXXConstructorDecl>(FD);
    return ctor && ctor->isMoveConstructor();
}

bool Orthodoxy::FunctionIsImplicitConversionConstructor(const clang::FunctionDecl *FD)
{
    const clang::CXXConstructorDecl *ctor = llvm::dyn_cast<clang::CXXConstructorDecl>(FD);
    return ctor && !ctor->isCopyOrMoveConstructor() && ctor->isConvertingConstructor(false);
}

unsigned int Orthodoxy::NamespaceDepth(const clang::NamespaceDecl *ND, bool countAnonymous)
{
    unsigned int depth =
        (countAnonymous ? 1 : !ND->isAnonymousNamespace());

    const clang::DeclContext *context = ND;
    while ((context = context->getParent()))
    {
        const clang::NamespaceDecl *ns =
            llvm::dyn_cast<clang::NamespaceDecl>(context);
        if (ns)
            depth += (countAnonymous ? 1 : !ns->isAnonymousNamespace());
    }

    return depth;
}
