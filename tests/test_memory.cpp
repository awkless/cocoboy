// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cocoboy/soc/memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

TEST_CASE("RegisterBit& RegisterBit::operator=(T val)", "[register_bit]")
{
    cocoboy::soc::Register<uint8_t> reg(0x00);
    cocoboy::soc::RegisterBit<0, 1, uint8_t> bit1(reg);
    cocoboy::soc::RegisterBit<1, 3, uint8_t> bit2(reg);
    cocoboy::soc::RegisterBit<4, 4, uint8_t> bit3(reg);

    bit1 = 0x1; // NOLINT
    bit2 = 0x2; // NOLINT
    bit3 = 0xF; // NOLINT
    REQUIRE(reg == 0b11110101); // NOLINT
}

TEST_CASE("RegisterBit::operator T() const", "[register_bit]")
{
    cocoboy::soc::Register<uint8_t> reg(0x00);
    cocoboy::soc::RegisterBit<0, 1, uint8_t> bit1(reg);
    cocoboy::soc::RegisterBit<1, 3, uint8_t> bit2(reg);
    cocoboy::soc::RegisterBit<4, 4, uint8_t> bit3(reg);


    bit1 = 0x1; // NOLINT
    bit2 = 0x2; // NOLINT
    bit3 = 0xF; // NOLINT
    uint8_t val1 = bit1;
    uint8_t val2 = bit2;
    uint8_t val3 = bit3;
    REQUIRE(val1 == 0x1); // NOLINT
    REQUIRE(val2 == 0x2); // NOLINT
    REQUIRE(val3 == 0xF); // NOLINT
}

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

TEST_CASE("RegisterPair& RegisterPair::operator P() const", "[register_pair]")
{
    cocoboy::soc::Register<uint8_t> reg1(0x00);
    cocoboy::soc::Register<uint8_t> reg2(0x00);
    cocoboy::soc::RegisterPair<uint16_t, uint8_t> reg_pair(reg1, reg2);

    reg1 = 0x12; // NOLINT
    reg2 = 0x34; // NOLINT
    uint16_t val = reg_pair;
    REQUIRE(val == 0x1234); // NOLINT
}
