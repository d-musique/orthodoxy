// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diag.h"

#define DEFINE_DIAGNOSTIC(id, alt, super, msg) \
    const OrthodoxyDiagDesc &Orthodoxy::diag::id() \
    { \
        static const OrthodoxyDiagDesc desc \
            { #id, alt, super, msg " [orthodoxy::" alt "]" }; \
        return desc; \
    }

#define DEFINE_ABSTRACT_DIAGNOSTIC(id, alt, super) \
    const OrthodoxyAbstractDiagDesc &Orthodoxy::diag::id() \
    { \
        static const OrthodoxyAbstractDiagDesc desc \
            { #id, alt, super }; \
        return desc; \
    }

#include "diag.inc"
#undef DEFINE_DIAGNOSTIC
