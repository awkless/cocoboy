// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cbgb/memory.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>

TEST_CASE("RegisterBit& RegisterBit::operator=(T val)", "[register_bit]")
{
    cbgb::memory::Register<uint8_t> reg(0x00);
    cbgb::memory::RegisterBit<0, 1, uint8_t> bit1(reg);
    cbgb::memory::RegisterBit<1, 3, uint8_t> bit2(reg);
    cbgb::memory::RegisterBit<4, 4, uint8_t> bit3(reg);

    bit1 = 0x1;
    bit2 = 0x2;
    bit3 = 0xF;
    REQUIRE(reg == 0b11110101);
}

TEST_CASE("RegisterBit::operator T() const", "[register_bit]")
{
    cbgb::memory::Register<uint8_t> reg(0x00);
    cbgb::memory::RegisterBit<0, 1, uint8_t> bit1(reg);
    cbgb::memory::RegisterBit<1, 3, uint8_t> bit2(reg);
    cbgb::memory::RegisterBit<4, 4, uint8_t> bit3(reg);


    bit1 = 0x1;
    bit2 = 0x2;
    bit3 = 0xF;
    uint8_t val1 = bit1;
    uint8_t val2 = bit2;
    uint8_t val3 = bit3;
    REQUIRE(val1 == 0x1);
    REQUIRE(val2 == 0x2);
    REQUIRE(val3 == 0xF);
}

TEST_CASE("RegisterPair& RegisterPair::operator=(T val)", "[register_pair]")
{
    cbgb::memory::Register<uint8_t> reg1(0x00);
    cbgb::memory::Register<uint8_t> reg2(0x00);
    cbgb::memory::RegisterPair<uint16_t, uint8_t> reg_pair(reg1, reg2);

    reg_pair = 0xDEAD;
    REQUIRE(reg_pair == 0xDEAD);
    REQUIRE(reg1 == 0xDE);
    REQUIRE(reg2 == 0xAD);

    reg1 = 0x12;
    reg2 = 0x34;
    REQUIRE(reg_pair == 0x1234);
}

TEST_CASE("RegisterPair& RegisterPair::operator P() const", "[register_pair]")
{
    cbgb::memory::Register<uint8_t> reg1(0x00);
    cbgb::memory::Register<uint8_t> reg2(0x00);
    cbgb::memory::RegisterPair<uint16_t, uint8_t> reg_pair(reg1, reg2);

    reg1 = 0x12;
    reg2 = 0x34;
    uint16_t val = reg_pair;
    REQUIRE(val == 0x1234);
}

TEST_CASE("RegisterPair& RegisterPair::operator++()", "[register_pair]")
{
    cbgb::memory::Register<uint8_t> reg1(0x00);
    cbgb::memory::Register<uint8_t> reg2(0x00);
    cbgb::memory::RegisterPair<uint16_t, uint8_t> reg_pair(reg1, reg2);

    ++reg_pair;
    REQUIRE(reg_pair == 0x0001);

    reg1 = 0x01;
    reg2 = 0xFF;
    ++reg_pair;
    REQUIRE(reg_pair == 0x0200);

    reg_pair = 0x0100;
    ++reg_pair;
    REQUIRE(reg_pair == 0x0101);
}

TEST_CASE("RegisterPair& RegisterPair::operator--()", "[register_pair]")
{
    cbgb::memory::Register<uint8_t> reg1(0x00);
    cbgb::memory::Register<uint8_t> reg2(0x01);
    cbgb::memory::RegisterPair<uint16_t, uint8_t> reg_pair(reg1, reg2);

    --reg_pair;
    REQUIRE(reg_pair == 0x0000);

    reg1 = 0x01;
    reg2 = 0xFF;
    --reg_pair;
    REQUIRE(reg_pair == 0x01FE);

    reg_pair = 0x0100;
    --reg_pair;
    REQUIRE(reg_pair == 0x00FF);
}
