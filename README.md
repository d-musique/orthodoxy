# Orthodoxy

Orthodoxy is a tool for programmers who believe that C++ needs *less*,
not *more*.

> Within C++, there is a much smaller and cleaner language struggling
> to get out.
> 
> — Bjarne Stroustrup

**This project is open to feature requests.
Feedback is appreciated.**

## Introduction

Orthodoxy is a plugin for the Clang compiler, which selectively
disables specific features of the C++ language. This way, a programmer
can configure their own custom subset of the C++ language which is
tailored to a particular project.

## Basic usage

Create a configuration file in the project directory, and compile the
code with `orthodox-clang++`.

Under CMake, `orthodoxy` is easy to use. The following code will
enable it for all subsequent targets.

> CMakeLists.txt
>
> ```
> find_package(orthodoxy CONFIG OPTIONAL_COMPONENTS plugin)
> if(orthodoxy_plugin_FOUND)
>   link_libraries(orthodoxy::plugin)
> endif()
> ```

See [Reference](#reference) for available rules, and the `test` folder
for a set of minimal examples.

> .orthodoxy.yml
>
> ```yaml
> ---
> Class: false
> NonPOD: false
> Constructor: false
> Destructor: false
> Inheritance: false
> Access: false
> ImplicitThis: false
> NamedCast: false
> OperatorOverload: false
> ConversionOverload: false
> Reference: false
> Exception: false
> RangeBasedLoop: false
> Lambda: false
> New: false
> Auto: false
> DefaultArgument: false
> ```

If present in source code, any use of a disabled language features
will raise a compilation error. One can suppress such errors on an
individual basis by writing a comment on the same line as the element
which raises the error.

> example.cpp
>
> ```cpp
> static_cast<int>(x); /* HERESY(static-cast) */
> ```

## Installation

This compiler plugin tightly integrates with a particular version of
the Clang compiler, so you have to compile a version that matches your
installed compiler.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
sudo cmake --build build --target install
```

**TODO: Help needed to write Windows-specific instructions and
helper scripts.**

## Reference

- [x] `InheritParent (bool)` inherit from parent directory rules (default: `false`)
- [x] `IncludeForbid (list[string])` patterns of forbidden system headers
- [x] `IncludeAllow (list[string])` patterns of allowed system headers
- [x] `Class (bool)` structures declared with the `class` keyword
- [x] `NonPOD (bool)` non-POD structures
- [x] `NonTrivial (bool)` non-trivial structures
- [x] `NonStandardLayout (bool)` non-standard-layout structures
- [x] `Constructor (bool)` constructors
- [x] `Destructor (bool)` destructors
- [x] `Inheritance (bool)` inheritance, both virtual and non-virtual
- [x] `VirtualInheritance (bool)` virtual inheritance
- [x] `MemberFunction (bool)` member functions
- [x] `VirtualMemberFunction (bool)` virtual member functions
- [x] `EnumClass (bool)` enum classes
- [x] `Access (bool)` member or inheritance access specifier
- [x] `MemberAccess (bool)` member access specifier
- [x] `InheritanceAccess (bool)` inheritance access specifier
- [x] `ImplicitThis (bool)` member access without an explicit `this`
- [x] `CStyleCast (bool)` C-style casts
- [x] `NamedCast (bool)` all C++-style named casts
- [x] `DynamicCast (bool)` dynamic casts 
- [x] `StaticCast (bool)` static casts
- [x] `ConstCast (bool)` const casts
- [x] `Overload (bool)` overloading, both function and operator
- [x] `FunctionOverload (bool)` function overloading
- [x] `OperatorOverload (bool)` operator overloading
- [x] `ConversionOverload (bool)` conversion operator overloading
- [x] `Reference (bool)` references of all kinds
- [x] `RvalueReference (bool)` r-value references
- [x] `Exception (bool)` all exception handling
- [x] `Throw (bool)` exception throwing
- [x] `TryCatch (bool)` exception catching
- [x] `Template (bool)` templates
- [x] `Concept (bool)` concepts
- [x] `Module (bool)` modules
- [x] `ModuleImport (bool)` importing modules
- [x] `ModuleExport (bool)` exporting modules
- [x] `RangeBasedLoop (bool)` range-based loop
- [x] `Lambda (bool)` lambda functions
- [x] `LambdaCapture (bool)` capturing lambda functions
- [x] `New (bool)` `new` and `delete`
- [x] `Auto (bool)` declarations with automatic type
- [x] `AutoReturn (bool)` function declarations with automatic return type
- [x] `AutoVariable (bool)` variable declarations with automatic type
- [x] `UserDefinedLiteral (bool)` user-defined literals
- [x] `DefaultArgument (bool)` default arguments
- [x] `Namespace (bool)` namespaces

## Motivation

C++ is often considered a very complicated language, while C is very
simplistic, and there exists no language in between to fill the gap.
As C++ adds features and grows its standard library, complication
worsens and compilation speed degrades.

Many developers limit themselves to more of less strict, vaguely
defined subsets of C++. One such subset is known as Orthodox C++.

Orthodoxy is named after Orthodox C++ and its goal is to easily
remodel C++ into a simpler, *better C* language according to the needs of
a specific project.

## Copyright

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
