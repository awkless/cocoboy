# SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena @awkless.com>
# SPDX-License-Identifier: MIT

add_executable(cbgb_tests)
target_sources(cbgb_tests
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/test_memory.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_cpu.cpp")
target_link_libraries(cbgb_tests PRIVATE cocoboy::cbgb cocoboy::deps Catch2::Catch2WithMain)
catch_discover_tests(cbgb_tests)
