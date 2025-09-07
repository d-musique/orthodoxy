// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
BEFORE_LLVM
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/Version.h>
AFTER_LLVM

#if CLANG_VERSION_MAJOR >= 20
#define USE_CLANG_DIAG_DESC
#endif

class OrthodoxyAbstractDiagDesc
{
public:
    OrthodoxyAbstractDiagDesc(
        llvm::StringRef id, llvm::StringRef alt,
        const OrthodoxyAbstractDiagDesc *super)
        : M_ID(id), M_ALT(alt), M_super(super) {}
    virtual ~OrthodoxyAbstractDiagDesc()
        {}
    llvm::StringRef GetOrthodoxyID() const
        { return M_ID; }
    llvm::StringRef GetOrthodoxyALT() const
        { return M_ALT; }
    const OrthodoxyAbstractDiagDesc *GetSuper() const
        { return M_super; }

protected:
    llvm::StringRef M_ID;
    llvm::StringRef M_ALT;
    const OrthodoxyAbstractDiagDesc *M_super;
};

class OrthodoxyDiagDesc
#if defined(USE_CLANG_DIAG_DESC)
    : public clang::DiagnosticIDs::CustomDiagDesc,
      public OrthodoxyAbstractDiagDesc
#endif
{
public:
    OrthodoxyDiagDesc(
        llvm::StringRef orthodoxyID, llvm::StringRef orthodoxyALT,
        const OrthodoxyAbstractDiagDesc *super, llvm::StringRef Description);

#if !defined(USE_CLANG_DIAG_DESC)
    llvm::StringRef GetDescription() const { return M_Description; }
#endif

private:
#if !defined(USE_CLANG_DIAG_DESC)
    llvm::StringRef M_Description;
#endif
};
