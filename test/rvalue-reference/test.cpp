// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

void TestR(int &r);
void TestRR(int &&r); // EXPECT(rvalue-reference)

struct StructR
{
    int &r;
};
struct StructRR
{
    int &&r; // EXPECT(rvalue-reference)
};
