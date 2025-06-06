# SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
  "cocoboy"
  VERSION 0.1.0
  DESCRIPTION "A sweet little gameboy emulator"
  LANGUAGES CXX C)

# Prevent in-source builds...
get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(srcdir "${CMAKE_BINARY_DIR}" REALPATH)

if("${srcdir}" STREQUAL "${bindir}")
  message(FATAL_ERROR "In-source builds not allowed! Use build directory!")
endif()

# Build options...
option(ENABLE_TESTS "Enable unit testing framework" OFF)
option(WARNINGS_AS_ERRORS "Treat any compiler warning as an error" OFF)
option(ENABLE_CLANG_TIDY "Use clang-tidy for static analysis" OFF)
option(ENABLE_CPPCHECK "Use cppcheck for static analysis" OFF)
option(ENABLE_IWYU "Use include-what-you-use to check headers" OFF)
option(ENABLE_DOXYGEN "Generate documentation with Doxygen" OFF)

# Set compiler options...
add_library(options INTERFACE)
target_compile_features(options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
add_library(cocoboy::options ALIAS options)

# Set compiler warnings...
add_library(warnings INTERFACE)

# References for these warnings I found here:
# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170
set(windows_warning_flags
    /W4
    /w14242
    /w14254
    /w14263
    /w14265
    /w14287
    /we4289
    /w14296
    /w14311
    /w14545
    /w14546
    /w14547
    /w14549
    /w14555
    /w14619
    /w14640
    /w14826
    /w14905
    /w14906
    /w14928
    /permissive-
)

# Clang matches the general settings of GCC, which is very nice. See:
# https://clang.llvm.org/docs/UsersManual.html#command-line-options
set(clang_warning_flags
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2)

if(WARNINGS_AS_ERRORS)
  set(clang_warning_flags ${clang_warning_flags} -Werror)
  set(windows_warning_flags ${windows_warning_flags} /WX)
endif()

# Some extra stuff that GCC offers to make life easier...
set(gcc_warning_flags
    ${clang_warning_flags}
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wuseless-cast)

if(MSVC)
  set(project_warnings ${windows_warning_flags})
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(project_warnings ${clang_warning_flags})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(project_warnings ${gcc_warning_flags})
else()
  set(project_warnings)
  message(
    AUTHOR_WARNING "No warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()

target_compile_options(warnings INTERFACE ${project_warnings})
add_library(cocoboy::warnings ALIAS warnings)

# Obtain dependencies...
find_package(SDL3 REQUIRED)
find_package(cxxopts REQUIRED)
find_package(fmt REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)

# Setup Dear Imgui backends being used. For whatever reason, Conan does not
# automatically include the backend header files of the Dear Imgui library.
# Thus, we force Conan to import the backend source code we want, and must
# manually build it. To ensure that people do not confuse these manually built
# dependencies with actual project production code, a special library target is
# created to be linked against separately.
add_library(backends STATIC)
target_sources(
  backends
  PUBLIC "${CMAKE_BINARY_DIR}/src/imgui/backends/imgui_impl_sdl3.h"
         "${CMAKE_BINARY_DIR}/src/imgui/backends/imgui_impl_sdlrenderer3.h"
  PRIVATE "${CMAKE_BINARY_DIR}/src/imgui/backends/imgui_impl_sdl3.cpp"
          "${CMAKE_BINARY_DIR}/src/imgui/backends/imgui_impl_sdlrenderer3.cpp")

target_link_libraries(
  backends
  PRIVATE cocoboy::options
          SDL3::SDL3
          imgui::imgui)
add_library(imgui::backends ALIAS backends)
target_include_directories(backends PUBLIC "${CMAKE_BINARY_DIR}/src")

# Place all dependencies under one banner to save my fingers from typing.
add_library(deps INTERFACE)
target_link_libraries(
  deps
  INTERFACE cocoboy::options
            cocoboy::warnings
            spdlog::spdlog
            SDL3::SDL3
            cxxopts::cxxopts
            fmt::fmt
            imgui::imgui
            imgui::backends)
add_library(cocoboy::deps ALIAS deps)

# Setup Doxygen documentation generation option...
if(ENABLE_DOXYGEN)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    # Load docs/Doxyfile.in configuration file...
    set(doxygen_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxyfile.in)
    set(doxygen_out ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
    configure_file(${doxygen_in} ${doxygen_out} @ONLY)

    # Generate Doxygen documentation as a custom target...
    message("Doxygen build started")
    add_custom_target(
      doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${doxygen_out}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)

    # Remove annoying *_source.html files, they mess up the reuse tool...
    file(GLOB_RECURSE SOURCE_HTML
         ${CMAKE_SOURCE_DIR}/docs/doxygen/**/*_source.html)
  else()
    message(FATAL_ERROR "Doxygen not found")
  endif()
endif()

# Implement clang-tidy static analysis option...
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (ENABLE_CLANG_TIDY)
  find_program(clang_tidy_exe NAMES clang-tidy)
  if(clang_tidy_exe)
    set(CMAKE_CXX_CLANG_TIDY
        ${clang_tidy_exe}
        -extra-arg=-Wno-unknown-warning-option
        -checks=-*,clang-diagnostic-*,clang-analyzer-*,bugprone-*,modernize-*
        -checks=-modernize-use-trailing-return-type,portability-*,performance-*
        -checks=-modernize-avoid-c-arrays
        -checks=readability-*,-readability-braces-around-statements
    )

    if(WARNINGS_AS_ERRORS)
      list(APPEND CMAKE_CXX_CLANG_TIDY -warnings-as-errors=*)
    endif()
  else()
    message(WARNING "clang-tidy not found")
  endif()
endif()

# Implement cppcheck static analysis option...
if(ENABLE_CPPCHECK)
    find_program(cppcheck_exe NAMES cppcheck)
    if(cppcheck_exe)
    set(CMAKE_CXX_CPPCHECK
        ${cppcheck_exe} --enable=style,performance,warning,portability
        --suppress=internalAstError --inline-suppr
        --suppress=unmatchedSuppression --inconclusive)

    if(WARNINGS_AS_ERRORS)
      list(APPEND CMAKE_CXX_CPPCHECK --error-exitcode=2)
    endif()
  else()
    message(WARNING "cppcheck not found")
  endif()
endif()

# Implement include-what-you-use static analysis option...
if (ENABLE_IWYU)
  find_program(iwyu_exe NAMES include-what-you-use)
  if(iwyu_exe)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_exe} -Xiwyu --no_comments)
  else()
    message(WARNING "include-what-you-use not found")
  endif()
endif()

# Build project...
add_subdirectory(src)

# Build testing suite...
if(ENABLE_TESTS)
  find_package(Catch2 REQUIRED)
  include(CTest)
  include(Catch)

  enable_testing()
  add_subdirectory(tests)
endif()
