// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
BEFORE_LLVM
#include <clang/AST/Type.h>
#include <llvm/Support/raw_ostream.h>
AFTER_LLVM

namespace clang { class TagDecl; }
namespace clang { class ClassTemplateSpecializationDecl; }

namespace Orthodoxy {

llvm::raw_ostream &error_message();
llvm::raw_ostream &warning_message();

bool TypeIsLvalueReference(clang::QualType qty);
bool TypeIsLvalueReference(const clang::Type *ty);
bool TypeIsRvalueReference(clang::QualType qty);
bool TypeIsRvalueReference(const clang::Type *ty);
bool TypeIsAuto(clang::QualType qty);
bool TypeIsAuto(const clang::Type *ty);

bool TagIsClass(const clang::TagDecl *TD);
bool TagIsEnumClass(const clang::TagDecl *TD);

bool FunctionIsCopyConstructor(const clang::FunctionDecl *FD);
bool FunctionIsMoveConstructor(const clang::FunctionDecl *FD);
bool FunctionIsImplicitConversionConstructor(const clang::FunctionDecl *FD);
bool FunctionIsAssignmentOperator(const clang::FunctionDecl *FD);
bool FunctionIsCopyAssignmentOperator(const clang::FunctionDecl *FD);
bool FunctionIsMoveAssignmentOperator(const clang::FunctionDecl *FD);

unsigned int NamespaceDepth(const clang::NamespaceDecl *ND, bool countAnonymous);

const clang::ClassTemplateSpecializationDecl *OuterTemplateSpecialization(const clang::DeclContext *DC);
const clang::ClassTemplateSpecializationDecl *OutermostTemplateSpecialization(const clang::DeclContext *DC);

} // namespace Orthodoxy
