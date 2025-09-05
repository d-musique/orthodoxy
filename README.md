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

This is a plugin for clang which enforces C++ programming rules to
disable a set of undesired features.
These rules are defined by the user on a per-directory basis.

While the main goal is to allow enforcing Orthodox C++ practices with
any desired level of strictness, anyone may find it useful in order to
enforce any specific programming guidelines.

## Rationale

With the evolution of the C++ language, the set of features provided
by the standard library and the core language grows rapidly.

This evolution leads to an unfortunate trend: compiles are
substantially slower than with former revisions of the language.
This inconvenience is not necessarily outweighed by the benefits of
getting the newer functionality, which sometimes motivates a rejection
of Modern C++.

There is a desire to limit what subset of the language gets used in a
given project, to avoid complication and slowness.

## How it works

The functionality is implemented in a clang plugin, which is enabled
on the command-line of the compiler. One can alternatively invoke the
wrapper program `orthodox-clang++`.

`clang++ -fplugin=/usr/lib64/clang/plugins/orthodoxy.so`

The plugin registers a pass in the compilation process, which
navigates the AST to find uses of forbidden language features.

If something forbidden is detected, the plugin emits an error
diagnostic with the location of the problem, and halts the
compilation.

The verifications can be bypassed on an individual basis by writing a
comment on the same line as the offending code.

`static_cast<int>(x); /* HERESY(static-cast) */`

The set of forbidden features is controlled by a file `.orthodoxy.yml`
(alternatively `orthodoxy.yml`) which contains a set of rules. The
file is located in a parent directory of the source file.

Note that the rules in the `.orthodoxy.yml` file only affect the
source files in its directory. Included files from elsewhere are
affected by the `.orthodoxy.yml` of the directory where they come from.

The rules will not be inherited from parent directories, unless the
rule `InheritParent: true` is given.

## Installation

This compiler plugin is tightly integrated with a particular version
of the Clang compiler, so you have to compile a version that matches
your installed compiler.

Get the source code and run these commands.

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
sudo cmake --build build --target install
```

**TODO: Help needed to write Windows-specific instructions and
helper scripts.**

## Usage

See the `test` folder for a set of minimal examples.

This is a complete example which can actually be useful in practice:

> .orthodoxy.yml
>
> ```
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

## Reference

- [x] `InheritParent (bool)` whether to inherit from parent directory rules
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

## Copyright

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
