// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "diagdesc.h"

namespace Orthodoxy::diag {

#define DEFINE_DIAGNOSTIC(id, alt, msg) \
        extern const OrthodoxyDiagDesc id;

#include "diag.inc"
#undef DEFINE_DIAGNOSTIC

} // namespace Orthodoxy::diag
