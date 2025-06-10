<!--
SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
SPDX-License-Identifier: MIT
-->

# Contributing to COCOBOY

_Thank you for taking the time to contribute!_

> __NOTE__: Always remember that the following information is not considered to
> be steadfast rules, but simple guidelines to ensure quality. Thus, always use
> your best judgement!

## Expected Forms of Contribution

This project is open to the following forms of contribution:

- Reporting of bugs.
- Suggestions for features or improvements.
- Improvements or fixes to build system
- Improvements or fixes to source code.
- Improvements or fixes to tests.
- Improvements or fixes to documentation.
- Improvements or fixes to CI/CD workflow.

## Issue Tracker Submissions

the issue tracker of this project is only for bug reports or feature requests.
please do not use it for asking questions or help. please use the provided
templates when making a submission to the issue tracker.

## Toolset

The following is a list of software tools that you are going to need in your
development environment when contributing to this project.

- C++17 compliant compiler (mainly GCC, Clang, or MSVC).
- cmake [>= 3.25.0].
- conan [>= 2.0.7] (implies Python 3 as well).
- git [>= 2.20.0].
- Debugger (preferably GNU GDB).
- doxygen [>= 1.8.0].
- clang-tidy [>= 14.0.0].
- cppcheck [>= 2.10.0].
- include-what-you-use [>= 0.18].
- reuse [>= 2.0.0].

## Pull Request Style

Helpful templates are provided to you for properly formatting your pull
requests. Understand that this project uses trunk based development. Thus, all
changes you make must occur separately on an unprotected branch based off of
`main` from your fork. Try to keep a linear commit history by using rebase
merging. Avoid merge commits.

Here some tips for better pull requests:

- Keep pull requests short and simple.
- Keep pull requests focused on one goal/issue rather than multiple.
- Break up large scale changes into multiple pull requests.
- Make sure that your pull requests only last for a couple of days. Avoid long
  standing pull requests.
- Use `git fetch --all -p` to get changes from main and then use
  `get rebase origin/main` to put your changes over the changes on `main`
  keeping a nice linear history.
- Stash untracked local changes with `git stash -u` and later pop them with
  `git stash pop`.
- Label your pull request if possible.

Remember that your pull requests must pass through the various CI workflow tests
that the project uses to ensure code quality. If your pull requests fail any one
of these automated tests, then your changes __WILL NOT__ be merged into `main`
until you fix the issues.

Afterwards, manual code review will occur. Awkless is the main code owner and
may ask you to make some changes to your code before rebase merging the pull
request onto `main`. Remember, COCOBOY is an open-source project that
functions off of volunteerism, so there will be days that Awkless may not be
able to immediately respond to your pull request. Patience is key.

## Commit Style

All commits you submit must abide by the [Conventional Commits 1.0.0][cc1.0.0]
specification. Here are the following valid types of commits accepted by this
project:

- __chore__: General change that does not affect production code.
- __feat__: Change that implements a feature in production code.
- __fix__: Change that fixes a bug in production code.
- __doc__: Improvements or fixes to documentation.
- __style__: Improvements or fixes to the formatting of code.
- __refactor__: Changes involve refactoring the code base.
- __revert__: A set of commits were revereted.

Keep the subject, body, and trailer of your commits below 80 characters unless
you have good reason otherwise.

Here is a good commit sample:

```
fix(cocoboy::cpu): Fix `ld r16, imm16` opcode

Fix segmentation fault described in issue #12345. Apperently, some ROMs used
an `imm16` operand greater than the available address space allowed by
`cocoboy::mem::MemoryMap`. This edge case is now accounted for.

Signed-off-by: Jason Pena <jasonpena@awkless.com>
Fixes: #12345
```

## Coding Style

COCOBOY is mainly written in C++17 without any extensions. The project does
use a few scripting and markup languages to help manage its source code and
documentation, but C++ is the main bread and butter.

### C++ Style

All C++ code in the COCOBOY project must be written in the C++17 standard
without any extensions. You can automatically format C++ code using
clang-format, which has already been configured to abide by this project's
coding style through the `.clang-format` file at the top-level.

- Use a maximum of 4 spaces for indentation.
    - Do not indent code in namespaces, or access modifiers, or cases in a
      switch statement.
- Use K&R style when placing braces and spaces.
    - Opening braces for functions and methods on a separate line.
    - Opening braces for everything else on the same line as the statement
      separated by a single space.
    - Put spaces between ternary and binary operators, but do not add a space
      between unary operators.
    - Put a space between statements and expressions, e.g., `if (true)`, or
      `while (foo != 0)`, or `for (int i = 0; i < foo; ++i)`.
- Members of a class must have a `m_` prefix in its name. If the member was
  declared static, then it must have a `s_` prefix in its name.
- Template parameters for a function/method must be placed on its own line,
  followed by the return and the name of the function on this newline.
- Use smart pointers instead of using malloc, free, new, or delete.
    - To keep ownership rules consistent and simple, pass by value with
      `std::unique_ptr<>`/`std::shared_ptr<>` to indicate that a caller is
      transferring ownership to a callee with parameters and return types.
      Otherwise, pass by pointer to indicate that the caller is not transferring
      ownership with parameters and return types.
    - Do not use references with smart pointers unless you really have no other
      choice.
- All header files must have an include guard that is named by the current
  namespace the code belongs too, then the name of the header file itself, e.g.,
  `COCOBOY_CORE_MEMORY_HPP` signifies that the header file belongs to namespace
  `cocoboy::core` and is named `memory.hpp`.
    - The inclusion of header files must follow this order: project headers
      first, third-party dependency headers second, C++ standard headers third,
      and C standard headers last. Each grouping of headers must be separated by
      a blank line.
    - Do not use `pragma once` as an include guard.

## CMake Style

The COCOBOY project uses CMake 3.25 or later as its main build system.
Thus, do not try to code for CMake versions older than 3.25. You can
automatically format CMake code using cmake-format. A configuration file for
cmake-format is provided at the top-level.

- Maximum of 2 spaces for indentation.
    - Commands with arguments should place each argument on its own line if
      there is more than one argument.
    - If you use a command that have category arguments like `PRIVATE`,
      `PUBLIC`, `LANGUAGES`, etc, then the following arguments should be
      indented at the same level as the category arguments.

Example:

```
add_executable(foo)
target_sources(
  foo
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/foo.hpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/bar.hpp"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/foo.cpp"
          "${CMAKE_CURRENT_SOURCE_DIR}/bar.cpp"
```

- Closing commands like `endif()`, `endforeach()`, `endmacro()`, etc, should be
  left blank.
- Local variables should always be in lowercase.

## Conan Style

COCOBOY uses Python to configure the Conan package manager. Thus, just use
the PEP8 standard for coding and configuring Conan.

## YAML Style

YAML is used to configure GitHub Actions for CI/CD workflows. Thus, to have some
consistency with different CI/CD workflows, here is a basic style guide.

- Indentation is 2 spaces.
    - Comments must match the level of indentation.
- Split strings using literal style, or folded style to break up long strings.
  Do not use newlines.

Example:

```
literal_example: |
  This example is an example of literal block scalar style in YAML.
  It allows you to split a string into multiple lines.
folded_example: >
  This example is an example of a folded block scalar style in YAML.
  It allows you to split a string into multi lines, however, it magically
  removes all the new lines placed in your YAML.
```

- Use `true` and `false` as boolean values only.
- Put spaces between `{{ }}` for templates.
- Prefer to use double quotes, use single quotes only when double quotes are
  already being used in the statement.

## Build System Features

The COCOBOY project uses CMake as its main build system, and Conan as its
main package manager. The build system comes equipped with a couple of features
to help make development easier. Some of these features are expressly used in
the code quality check gauntlet CI pipeline. Thus, it may be quicker for your
pull request to pass by using some of these features beforehand.

### Enable Clang-Tidy

The `ENABLE_CLANG_TIDY` option enables the clang-tidy linter to perform source
code linting when building the project. By default, this feature is `OFF` and
must always be manually turned `ON`.

Here is an example of enabling this option:

```
# cmake --preset conan-debug -DENABLE_CLANG_TIDY=ON
# cmake --build --preset conan-debug
```

A `.clang-tidy` file is provided at the top-level of the project in case you
wish to run clang-tidy separately from the build process for whatever reason.
Warnings are not treated as errors by default when using this option.

### Enable CPPCHECK

The `ENABLE_CPPCHECK` option enables the cppcheck linter to perform source code
linting when building the project. By default, this feature is `OFF`, and must
be manually turned `ON`.

Here is an example of enabling this option:

```
# cmake --preset conan-debug -DENABLE_CPPCHECK=ON
# cmake --build --preset conan-debug
```

Warnings are not treated as errors by default when using this option.

### Enable Include-What-You-Use

The `ENABLE_IWYU` option enable the include-what-you-use header file checker to
ensure that any source code in the code base is including the header files it is
using. By default, this option is `OFF`, and must be manually turned `ON`.

Here is an example of enabling this option:

```
# cmake --preset conan-debug -DENABLE_IWYU=ON
# cmake --build --preset conan-debug
```

There is no way to treat warnings from include-what-you-use as errors.

### Treat all Warnings as Errors

The `WARNINGS_AS_ERRORS` option will make the CMake build system treat __ANY__
warning during compilation as an error. This includes compilation warnings,
clang-tidy warnings, and cppcheck warnings. By default this option is `OFF`, and
must be manually turned `ON`.

Here is an example of enabling this option:

```
# cmake --preset conan-debug -DWARNINGS_AS_ERRORS=ON
# cmake --build --preset conan-debug
```

### Generate Documentation Through Doxygen

The `ENABLE_DOXYGEN` option will make CMake generate document from production
code comments using Doxygen. By default this option is `OFF`, and must be
manually turned `ON`. The resulting documentation is stored in `docs/Doxygen`.

Here is an example of enabling this option:

```
# cmake --preset conan-debug -DENABLE_DOXYGEN=ON
# cmake --build --preset conan-debug
```

### Enable Testing Suite

The `ENABLE_TESTS` option will make CMake build unit test cases from the `test/`
subdirectory, which can be ran and verified by using `ctest --test-dir
build/(Debug|Release|MinSizeRel|RelWithDebInfo)`. By default this option is
`OFF`, and must be manually turned `ON`.

Here is an example of enabling this option:

```
# cmake --preset conan-debug -DENABLE_TESTS=ON
# cmake --build --preset conan-debug
```

If you enabled clang-tidy, cppcheck, and/or, include-what-you-use, then those
programs will also lint the source code in the `test/` subdirectory.

## 8. Licensing and Copyright

The COCOBOY project is provided to the public as freeware under the GNU GPL
v2+ __WITH__ GNU Cooperation Commitment version 1.0 (more information provided
in section 5.1). Additional licenses are utilitzed in the project's codebase
in order to provide legal coverage where the main project license does not,
e.g., documentation, or small configuration files too generic to claim true
copyright over.

## Rules of Licensing and Copyright

This project abides by the [REUSE 3.3 specification][reuse-3.3-spec]
specification to determine the licensing and copyright of files in the code
base. Thus, all files must have the proper SPDX copyright and licensing tags at
the top always. Contributors can Use the [reuse tool][reuse-tool] to determine
if their changes are REUSE 3.3 compliant.

OCD uses the MIT license as its main source code and documentation license. OCD
also uses the CC0-1.0 license to place files in the public domain that are
considered to be to small or generic to place copyright over. Thus, for almost
all contributions you will use the MIT license.

Do not forget to include the following SPDX copyright identifier at the top of
any file you create along with the SPDX license identifier:

```
SPDX-FileCopyrightText: <year> <name> <email>
SPDX-License-Identifier: MIT
```

[reuse-3.3-spec]: https://reuse.software/spec-3.3/
[reuse-tool]: https://reuse.software/tutorial/
