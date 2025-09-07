// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diagdesc.h"

OrthodoxyDiagDesc::OrthodoxyDiagDesc(
    llvm::StringRef orthodoxyID, llvm::StringRef orthodoxyALT,
    const OrthodoxyAbstractDiagDesc *super, llvm::StringRef Description)
    :
#if defined(USE_CLANG_DIAG_DESC)
    clang::DiagnosticIDs::CustomDiagDesc{
        clang::diag::Severity::Error, std::string{Description}},
#endif
    OrthodoxyAbstractDiagDesc{orthodoxyID, orthodoxyALT, super}
#if !defined(USE_CLANG_DIAG_DESC)
    , M_Description{Description}
#endif
{
}
