// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include <catch2/catch_test_macros.hpp>

uint32_t
factorial(uint32_t number)
{
    return number <= 1 ? number : factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]")
{
    REQUIRE(factorial(1) == 1); // NOLINT
    REQUIRE(factorial(2) == 2); // NOLINT
    REQUIRE(factorial(3) == 6); // NOLINT
    REQUIRE(factorial(10) == 3'628'800); // NOLINT
}
