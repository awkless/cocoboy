// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

TEST_CASE("assignment operator works", "[register_pair]")
{
    cocoboy::Register<uint8_t> r1(0x00);
    cocoboy::Register<uint8_t> r2(0x00);
    cocoboy::RegisterPair<uint16_t, uint8_t> rp(r1, r2);

    rp = 0xDEAD;
    REQUIRE(rp == 0xDEAD);
    REQUIRE(r1 == 0xDE);
    REQUIRE(r2 == 0xAD);

    r1 = 0x12;
    r2 = 0x34;
    REQUIRE(rp == 0x1234);
}
