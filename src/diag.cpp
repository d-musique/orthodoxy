// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#define DEFINE_DIAGNOSTIC(id, alt, msg) \
    namespace Orthodoxy { namespace diag { \
        extern const OrthodoxyDiagDesc id; } } \
    \
    const OrthodoxyDiagDesc Orthodoxy::diag::id \
        { #id, #alt, msg " [orthodoxy::" #alt "]" }

#include "diag.h"
