// Copyright: D.M., 2025-present
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>

struct ArrayInt
{
	int arr[256];
};

struct ArrayString // EXPECT(non-pod)
{
	std::string arr[256];
};

template <size_t N>
struct ArrayIntN
{
	int arr[N];
};

template <size_t N>
struct ArrayStringN
{
	std::string arr[N];
};

template <typename T, size_t N>
struct ArrayGenericN
{
	T arr[N];
};

void foo()
{
    { ArrayInt x; }
    { ArrayString x; }
    { ArrayIntN<256> x; }
    { ArrayStringN<256> x; } // EXPECT(non-pod)
    { ArrayGenericN<int, 256> x; }
    { ArrayGenericN<std::string, 256> x; } // EXPECT(non-pod)
}
