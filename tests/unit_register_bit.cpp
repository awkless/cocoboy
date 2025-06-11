// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cocoboy/soc/memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

TEST_CASE("RegisterBit& RegisterBit::operator=(T val)", "[register_bit]")
{
    cocoboy::soc::Register<uint8_t> reg(0x00);
    cocoboy::soc::RegisterBit<0, 1, uint8_t> bit1(reg);
    cocoboy::soc::RegisterBit<4, 4, uint8_t> bit2(reg);

    bit1 = 0x1; // NOLINT
    bit2 = 0xF; // NOLINT
    REQUIRE(reg == 0xF1); // NOLINT
}
