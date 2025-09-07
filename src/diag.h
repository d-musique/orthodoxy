// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "diagdesc.h"

namespace Orthodoxy::diag {

#define DEFINE_DIAGNOSTIC(id, alt, super, msg) \
        const OrthodoxyDiagDesc &id();

#define DEFINE_ABSTRACT_DIAGNOSTIC(id, alt, super) \
        const OrthodoxyAbstractDiagDesc &id();

#include "diag.inc"
#undef DEFINE_DIAGNOSTIC
#undef DEFINE_ABSTRACT_DIAGNOSTIC

} // namespace Orthodoxy::diag
