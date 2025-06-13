# SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
# SPDX-License-Identifier: MIT

add_library(cbgb)
target_sources(cbgb
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp"
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/cpu.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/memory.hpp")
target_include_directories(cbgb PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(cbgb PRIVATE cocoboy::deps)
add_library(cocoboy::cbgb ALIAS cbgb)
