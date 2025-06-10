# SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
# SPDX-License-Identifier: MIT

set(config_hpp_in "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in")
set(config_hpp_out "${CMAKE_BINARY_DIR}/src/config.hpp")
configure_file(${config_hpp_in} ${config_hpp_out} @ONLY)

add_library(soc memory.cpp cpu.cpp)
target_include_directories(soc PUBLIC "${CMAKE_SOURCE_DIR}/src")
add_library(cocoboy::soc ALIAS soc)

add_executable(cocoboy)
target_sources(cocoboy PRIVATE main.cpp)
target_link_libraries(cocoboy PRIVATE cocoboy::soc cocoboy::deps)
install(TARGETS cocoboy)
