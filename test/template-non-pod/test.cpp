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

/* template specializations */

template <typename T> struct Specialized;
template <> struct Specialized<int> { int x; };
template <> struct Specialized<std::string> { std::string x; }; // EXPECT(non-pod)
Specialized<int> xS;
Specialized<std::string> yS;

/* template nested struct */

template <typename T> struct Outer1 { struct Inner { T x; }; };
Outer1<int>::Inner x1;
Outer1<std::string>::Inner y1; // EXPECT(non-pod)

template <typename T> struct Outer2 { struct Inner; };
template <typename T> struct Outer2<T>::Inner { T x; };
Outer2<int>::Inner x2;
Outer2<std::string>::Inner y2; // EXPECT(non-pod)
