# SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
# SPDX-License-Identifier: MIT

set(config_hpp_in "${CMAKE_CURRENT_SOURCE_DIR}/cocoboy/config.hpp.in")
set(config_hpp_out "${CMAKE_BINARY_DIR}/src/cocoboy/config.hpp")
configure_file(${config_hpp_in} ${config_hpp_out} @ONLY)

add_library(emu
  "${CMAKE_CURRENT_SOURCE_DIR}/cocoboy/soc/memory.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/cocoboy/soc/cpu.cpp")
target_include_directories(emu PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(emu PRIVATE cocoboy::deps)
add_library(cocoboy::emu ALIAS emu)

add_executable(cocoboy_bin)
target_sources(cocoboy_bin  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/cocoboy/main.cpp")
target_link_libraries(cocoboy_bin PRIVATE cocoboy::emu cocoboy::deps)
set_target_properties(cocoboy_bin
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  OUTPUT_NAME cocoboy)
