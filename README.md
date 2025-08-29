# Orthodoxy

Orthodoxy is a plugin for clang which enforces C++ programming rules
to disable a set of undesired features.
These rules are defined by the user on a per-directory basis.

While the main goal is to allow enforcing Orthodox C++ practices with
any desired level of strictness, anyone may find it useful in order to
enforce any specific programming guidelines.

**Note that this project is currently in the idea stage. Get in touch on
Github if you want to work on this.**

## Rationale

With the evolution of the C++ language, the set of features of provided
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
on the command-line of the compiler.

`clang -fplugin=orthodoxy.so`

The plugin registers a pass in the compilation process, which
navigates the AST to find uses of forbidden language features.

If something forbidden is detected, the plugin emits an error
diagnostic with the location of the problem, and halts the
compilation.

The verifications can be bypassed on an individual basis by writing a
comment on the same line as the offending code.

`static_cast<int>(x); /* HERESY(StaticCast) */`

The set of forbidden features is controlled by a file `.orthodoxy.yml`
which contains an array of settings. The file is located in a parent
directory of the source file.

Note that the settings in the `.orthodoxy.yml` file only affect the
source files in its directory. Included files from elsewhere are
affected by the `.orthodoxy.yml` of the directory where they come from.

The settings will not be inherited from parent directories, unless the
setting `InheritParent true` is given.

## Settings

- `STL (list[string])` patterns of allowed and forbidden STL headers
- `Class (bool)` structures declared with the `class` keyword
- `NonStandardLayout (bool)` non-standard-layout structures
- `NonPOD (bool)` non-POD structures
- `Constructor (bool)` constructors
- `Destructor (bool)` destructors
- `Inheritance (bool)` inheritance, both virtual and non-virtual
- `VirtualInheritance (bool)` virtual inheritance
- `MemberFunction (bool)` member functions
- `EnumClass (bool)` enum classes
- `Visibility (bool)` member of inheritance visibility
- `MemberVisibility (bool)` member visibility
- `InheritanceVisibility (bool)` inheritance visibility
- `ImplicitThis (bool)` member access without an explicit `this`
- `CStyleCast (bool)` C-style casts
- `C++StyleCast (bool)` all C++-style casts
- `DynamicCast (bool)` dynamic casts 
- `StaticCast (bool)` static casts
- `ConstCast (bool)` const casts
- `Overloading (bool)` overloading, both function and operator
- `FunctionOverloading (bool)` function overloading
- `OperatorOverloading (bool)` operator overloading
- `Reference (bool)` references of all kinds
- `RvalueReference (bool)` r-value references
- `Exception (bool)` all exception handling
- `Throw (bool)` exception throwing
- `Catch (bool)` exception catching
- `Template (bool)` templates
- `Concept (bool)` concepts
- `Module (bool)` modules
- `RangeBasedForLoop (bool)` range-based for loop
- `Lambda (bool)` lambda functions
- `New (bool)` `new` and `delete`
- `Auto (bool)` declarations with automatic type
- `UserDefinedLiteral (bool)` user-defined literals
