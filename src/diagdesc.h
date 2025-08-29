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

class OrthodoxyDiagDesc
#if defined(USE_CLANG_DIAG_DESC)
    : public clang::DiagnosticIDs::CustomDiagDesc
#endif
{
public:
    OrthodoxyDiagDesc(
        llvm::StringRef orthodoxyID,
        llvm::StringRef orthodoxyALT,
        llvm::StringRef Description);

    llvm::StringRef GetOrthodoxyID() const
    {
        return M_ID;
    }

    llvm::StringRef GetOrthodoxyALT() const
    {
        return M_ALT;
    }

#if !defined(USE_CLANG_DIAG_DESC)
    llvm::StringRef GetDescription() const
    {
        return M_Description;
    }
#endif

private:
    llvm::StringRef M_ID;
    llvm::StringRef M_ALT;
#if !defined(USE_CLANG_DIAG_DESC)
    llvm::StringRef M_Description;
#endif
};
