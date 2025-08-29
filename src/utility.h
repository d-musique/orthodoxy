// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
BEFORE_LLVM
#include <clang/AST/Type.h>
#include <llvm/Support/raw_ostream.h>
AFTER_LLVM

namespace clang { class TagDecl; }

namespace Orthodoxy {

llvm::raw_ostream &error_message();
llvm::raw_ostream &warning_message();

bool TypeIsReference(clang::QualType qty);
bool TypeIsReference(const clang::Type *ty);
bool TypeIsRvalueReference(clang::QualType qty);
bool TypeIsRvalueReference(const clang::Type *ty);
bool TypeIsAuto(clang::QualType qty);
bool TypeIsAuto(const clang::Type *ty);

bool TagIsClass(const clang::TagDecl *TD);
bool TagIsEnumClass(const clang::TagDecl *TD);

} // namespace Orthodoxy
