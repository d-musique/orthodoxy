// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diag.h"

#define DEFINE_DIAGNOSTIC(id, alt, msg) \
    const OrthodoxyDiagDesc &Orthodoxy::diag::id() \
    { \
        static const OrthodoxyDiagDesc desc \
            { #id, alt, msg " [orthodoxy::" alt "]" }; \
        return desc; \
    }

#include "diag.inc"
#undef DEFINE_DIAGNOSTIC
