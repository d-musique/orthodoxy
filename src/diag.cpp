// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diag.h"

#define DEFINE_DIAGNOSTIC(id, alt, msg) \
    const OrthodoxyDiagDesc Orthodoxy::diag::id \
        { #id, #alt, msg " [orthodoxy::" #alt "]" }

#include "diag.inc"
#undef DEFINE_DIAGNOSTIC
