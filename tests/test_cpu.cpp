// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT


#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cstdint>

#include "cbgb/memory.hpp"
#include "cbgb/cpu.hpp"


TEST_CASE("void Sm83::step() - ld r, n", "[sm83]")
{
    auto logger = spdlog::stdout_color_mt("test");
    cbgb::MemoryBus memory(logger);
    cbgb::Sm83 cpu(logger, memory);

    uint8_t ld_r_n[14] = {
        0x06, 0x01, // LD B, 1
        0x0E, 0x02, // LD C, 2
        0x16, 0x03, // LD D, 3
        0x1E, 0x04, // LD E, 4
        0x26, 0x05, // LD H, 5
        0x2E, 0x06, // LD L, 6
        0x3E, 0x07  // LD A, 7
    };

    uint16_t address = 0x0100;
    for (size_t i = 0; i < 14; ++i) {
        memory.write(address++, ld_r_n[i]);
    }

    for (size_t i = 0; i < 7; ++i) {
        cpu.step();
    }

    auto state = cpu.get_state();
    CHECK(state.b == 0x01);
    CHECK(state.c == 0x02);
    CHECK(state.d == 0x03);
    CHECK(state.e == 0x04);
    CHECK(state.h == 0x05);
    CHECK(state.l == 0x06);
    CHECK(state.a == 0x07);
    CHECK(cpu.get_mcycle_count() == 14);
}
