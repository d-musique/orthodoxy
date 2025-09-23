// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

void TestR(int &r); // EXPECT(reference-to-non-const)
void TestRC(const int &r);
void TestRR(int &&r); // EXPECT(reference-to-non-const)
void TestRRC(const int &&r);

struct StructR
{
    int &r; // EXPECT(reference-to-non-const)
    const int &rc;
};
struct StructRR
{
    int &&r; // EXPECT(reference-to-non-const)
    const int &&rc;
};
