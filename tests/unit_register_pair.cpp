// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cocoboy/soc/memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

TEST_CASE("RegisterPair& RegisterPair::operator=(T val)", "[register_pair]")
{
    cocoboy::soc::Register<uint8_t> reg1(0x00);
    cocoboy::soc::Register<uint8_t> reg2(0x00);
    cocoboy::soc::RegisterPair<uint16_t, uint8_t> reg_pair(reg1, reg2);

    reg_pair = 0xDEAD; // NOLINT
    REQUIRE(reg_pair == 0xDEAD); // NOLINT
    REQUIRE(reg1 == 0xDE); // NOLINT
    REQUIRE(reg2 == 0xAD); // NOLINT

    reg1 = 0x12; // NOLINT
    reg2 = 0x34; // NOLINT
    REQUIRE(reg_pair == 0x1234); // NOLINT
}
