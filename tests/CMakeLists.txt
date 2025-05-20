# SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
# SPDX-License-Identifier: MIT

add_executable(cocoboy_tests)
target_sources(
  cocoboy_tests
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_link_libraries(cocoboy_tests PRIVATE cocoboy::deps GTest::gtest)
gtest_discover_tests(cocoboy_tests)
