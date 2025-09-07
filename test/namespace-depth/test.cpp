// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

namespace N1
{
    namespace N2
    {
        namespace N3 // EXPECT(namespace-depth)
        {
        }
    }
}

namespace
{
    namespace M1
    {
        namespace
        {
            namespace M2
            {
                namespace
                {
                    namespace M3 // EXPECT(namespace-depth)
                    {
                    }
                }
            }
        }
    }
}
