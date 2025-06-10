# SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena @awkless.com>
# SPDX-License-Identifier: MIT

add_executable(cocoboy_unit_tests)
target_sources(cocoboy_unit_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/unit_register_pair.cpp")
target_link_libraries(cocoboy_unit_tests PRIVATE cocoboy::soc cocoboy::deps Catch2::Catch2WithMain)
catch_discover_tests(cocoboy_unit_tests)
