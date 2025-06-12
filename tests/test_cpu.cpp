// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cocoboy/soc/memory.hpp"
#include "cocoboy/soc/cpu.hpp"

#include <catch2/catch_test_macros.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <cstdint>
#include <memory>

static constexpr size_t max_ld_r_r_targets = 49;
static const cocoboy::soc::Sm83Opcode ld_r_r_targets[max_ld_r_r_targets] = {
    cocoboy::soc::Sm83Opcode::LD_B_B,
    cocoboy::soc::Sm83Opcode::LD_B_C,
    cocoboy::soc::Sm83Opcode::LD_B_D,
    cocoboy::soc::Sm83Opcode::LD_B_E,
    cocoboy::soc::Sm83Opcode::LD_B_H,
    cocoboy::soc::Sm83Opcode::LD_B_L,
    cocoboy::soc::Sm83Opcode::LD_B_A,
    cocoboy::soc::Sm83Opcode::LD_C_B,
    cocoboy::soc::Sm83Opcode::LD_C_C,
    cocoboy::soc::Sm83Opcode::LD_C_D,
    cocoboy::soc::Sm83Opcode::LD_C_E,
    cocoboy::soc::Sm83Opcode::LD_C_H,
    cocoboy::soc::Sm83Opcode::LD_C_L,
    cocoboy::soc::Sm83Opcode::LD_C_A,
    cocoboy::soc::Sm83Opcode::LD_D_B,
    cocoboy::soc::Sm83Opcode::LD_D_C,
    cocoboy::soc::Sm83Opcode::LD_D_D,
    cocoboy::soc::Sm83Opcode::LD_D_E,
    cocoboy::soc::Sm83Opcode::LD_D_H,
    cocoboy::soc::Sm83Opcode::LD_D_L,
    cocoboy::soc::Sm83Opcode::LD_D_A,
    cocoboy::soc::Sm83Opcode::LD_E_B,
    cocoboy::soc::Sm83Opcode::LD_E_C,
    cocoboy::soc::Sm83Opcode::LD_E_D,
    cocoboy::soc::Sm83Opcode::LD_E_E,
    cocoboy::soc::Sm83Opcode::LD_E_H,
    cocoboy::soc::Sm83Opcode::LD_E_L,
    cocoboy::soc::Sm83Opcode::LD_E_A,
    cocoboy::soc::Sm83Opcode::LD_H_B,
    cocoboy::soc::Sm83Opcode::LD_H_C,
    cocoboy::soc::Sm83Opcode::LD_H_D,
    cocoboy::soc::Sm83Opcode::LD_H_E,
    cocoboy::soc::Sm83Opcode::LD_H_H,
    cocoboy::soc::Sm83Opcode::LD_H_L,
    cocoboy::soc::Sm83Opcode::LD_H_A,
    cocoboy::soc::Sm83Opcode::LD_L_B,
    cocoboy::soc::Sm83Opcode::LD_L_C,
    cocoboy::soc::Sm83Opcode::LD_L_D,
    cocoboy::soc::Sm83Opcode::LD_L_E,
    cocoboy::soc::Sm83Opcode::LD_L_H,
    cocoboy::soc::Sm83Opcode::LD_L_L,
    cocoboy::soc::Sm83Opcode::LD_L_A,
    cocoboy::soc::Sm83Opcode::LD_A_B,
    cocoboy::soc::Sm83Opcode::LD_A_C,
    cocoboy::soc::Sm83Opcode::LD_A_D,
    cocoboy::soc::Sm83Opcode::LD_A_E,
    cocoboy::soc::Sm83Opcode::LD_A_H,
    cocoboy::soc::Sm83Opcode::LD_A_L,
    cocoboy::soc::Sm83Opcode::LD_A_A,
};

static constexpr size_t max_ld_r_n_targets = 7;
static const cocoboy::soc::Sm83Opcode ld_r_n_targets[max_ld_r_n_targets] = {
    cocoboy::soc::Sm83Opcode::LD_B_N,
    cocoboy::soc::Sm83Opcode::LD_C_N,
    cocoboy::soc::Sm83Opcode::LD_D_N,
    cocoboy::soc::Sm83Opcode::LD_E_N,
    cocoboy::soc::Sm83Opcode::LD_H_N,
    cocoboy::soc::Sm83Opcode::LD_L_N,
    cocoboy::soc::Sm83Opcode::LD_A_N,
};

static constexpr size_t max_ld_r_hl_targets = 7;
static const cocoboy::soc::Sm83Opcode ld_r_hl_targets[max_ld_r_hl_targets] = {
    cocoboy::soc::Sm83Opcode::LD_B_HL,
    cocoboy::soc::Sm83Opcode::LD_C_HL,
    cocoboy::soc::Sm83Opcode::LD_D_HL,
    cocoboy::soc::Sm83Opcode::LD_E_HL,
    cocoboy::soc::Sm83Opcode::LD_H_HL,
    cocoboy::soc::Sm83Opcode::LD_L_HL,
    cocoboy::soc::Sm83Opcode::LD_A_HL,
};

static constexpr size_t max_ld_hl_r_targets = 7;
static const cocoboy::soc::Sm83Opcode ld_hl_r_targets[max_ld_hl_r_targets] = {
    cocoboy::soc::Sm83Opcode::LD_HL_B,
    cocoboy::soc::Sm83Opcode::LD_HL_C,
    cocoboy::soc::Sm83Opcode::LD_HL_D,
    cocoboy::soc::Sm83Opcode::LD_HL_E,
    cocoboy::soc::Sm83Opcode::LD_HL_H,
    cocoboy::soc::Sm83Opcode::LD_HL_L,
    cocoboy::soc::Sm83Opcode::LD_HL_A,
};

TEST_CASE("void Sm83OpcodeRunner::ld_r_r(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("void Sm83::ld_r_r() test");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    uint8_t values[reg_file.r8.size()] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    for (size_t i = 0; i < reg_file.r8.size(); ++i) {
        *reg_file.r8[i] = values[i];
    }

    for (unsigned int i = 0; i < max_ld_r_r_targets; ++i) {
        uint8_t target = ld_r_r_targets[i];
        uint8_t regx = (target & 0x38) >> 3;
        uint8_t regy = target & 0x07;

        opcode.ld_r_r(target);
        CHECK(static_cast<uint8_t>(*reg_file.r8[regx]) == values[regy]);

        for (size_t j = 0; j < reg_file.r8.size(); ++j) {
            *reg_file.r8[j] = values[j];
        }
    }
}

TEST_CASE("void Sm83OpcodeRunner::ld_r_n(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("void Sm83::ld_r_n() test");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    uint8_t values[max_ld_r_n_targets] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    reg_file.pc = 0x0100;
    for (size_t i = 0; i < max_ld_r_n_targets; ++i) {
        bus[reg_file.pc] = ld_r_n_targets[i];
        ++reg_file.pc;
        bus[reg_file.pc] = values[i];
        ++reg_file.pc;
    }

    reg_file.pc = 0x0100;
    for (size_t i = 0; i < max_ld_r_n_targets; ++i) {
        uint8_t target = bus[reg_file.pc];
        ++reg_file.pc;

        uint8_t regx = (target & 0x38) >> 3;
        opcode.ld_r_n(target);
        CHECK(static_cast<uint8_t>(*reg_file.r8[regx]) == values[i]);
    }
}

TEST_CASE("void Sm83OpcodeRunner::ld_r_hl(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("void Sm83::ld_r_hl() test");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.hl = 0xBEEF;
    bus[reg_file.hl] = 0x42;

    reg_file.pc = 0x0100;
    for (size_t i = 0; i < max_ld_r_hl_targets; ++i) {
        bus[reg_file.pc] = ld_r_hl_targets[i];
        ++reg_file.pc;
    }

    reg_file.pc = 0x0100;
    for (size_t i = 0; i < max_ld_r_hl_targets; ++i) {
        uint8_t target = bus[reg_file.pc];
        ++reg_file.pc;

        uint8_t regx = (target & 0x38) >> 3;
        opcode.ld_r_hl(target);
        CHECK(static_cast<uint8_t>(*reg_file.r8[regx]) == 0x42);
        reg_file.hl = 0xBEEF;
    }
}

TEST_CASE("void Sm83OpcodeRunner::ld_hl_r(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("void Sm83::ld_hl_r() test");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    constexpr uint8_t values[reg_file.r8.size()] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    for (size_t i = 0; i < reg_file.r8.size(); ++i) {
        *reg_file.r8[i] = values[i];
    }

    reg_file.pc = 0x0100;
    for (size_t i = 0; i < max_ld_hl_r_targets; ++i) {
        bus[reg_file.pc] = ld_hl_r_targets[i];
        ++reg_file.pc;
    }

    reg_file.hl = 0xBEEF;
    reg_file.pc = 0x0100;
    for (size_t i = 0; i < max_ld_r_hl_targets; ++i) {
        uint8_t target = bus[reg_file.pc];
        ++reg_file.pc;

        uint8_t regx = target & 0x03;
        opcode.ld_hl_r(target);
        CHECK(bus[reg_file.hl] == values[regx]);
        reg_file.hl = 0xBEEF;
    }
}

TEST_CASE("void Sm83OpcodeRunner::ld_hl_n(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("void Sm83::ld_hl_n() test");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.pc = 0x0100;
    constexpr uint8_t values[3] = { 0xBF, 0x42, 0xFF };
    for (size_t i = 0; i < 3; ++i) {
        bus[reg_file.pc] = cocoboy::soc::Sm83Opcode::LD_HL_N;
        ++reg_file.pc;
        bus[reg_file.pc] = values[i];
        ++reg_file.pc;
    }

    reg_file.hl = 0xBEEF;
    reg_file.pc = 0x0100;
    for (size_t i = 0; i < 3; ++i) {
        uint8_t target = bus[reg_file.pc];
        ++reg_file.pc;

        opcode.ld_hl_n(target);
        CHECK(bus[reg_file.hl] == values[i]);
    }
}

TEST_CASE("void Sm83OpcodeRunner::ld_a_bc(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD A, (BC)");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.bc = 0xBEEF;
    reg_file.pc = 0x0100;
    bus[reg_file.bc] = 0x42;
    bus[reg_file.pc] = cocoboy::soc::Sm83Opcode::LD_A_BC;
    opcode.ld_a_bc(bus[reg_file.pc]);
    CHECK(static_cast<uint8_t>(reg_file.a) == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ld_a_de(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD A, (DE)");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.de = 0xBEEF;
    reg_file.pc = 0x0100;
    bus[reg_file.de] = 0x42;
    bus[reg_file.pc] = cocoboy::soc::Sm83Opcode::LD_A_DE;
    opcode.ld_a_de(bus[reg_file.pc]);
    CHECK(static_cast<uint8_t>(reg_file.a) == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ld_bc_a(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD (BC), A");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.bc = 0xBEEF;
    reg_file.pc = 0x0100;
    reg_file.a = 0x42;
    bus[reg_file.pc] = cocoboy::soc::Sm83Opcode::LD_BC_A;
    opcode.ld_bc_a(bus[reg_file.pc]);
    CHECK(bus[reg_file.bc] == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ld_de_a(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD (DE), A");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.de = 0xBEEF;
    reg_file.pc = 0x0100;
    reg_file.a = 0x42;
    bus[reg_file.pc] = cocoboy::soc::Sm83Opcode::LD_DE_A;
    opcode.ld_de_a(bus[reg_file.pc]);
    CHECK(bus[reg_file.de] == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ld_a_nn(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD A, (NN)");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    bus[0x0101] = 0xBE;
    bus[0x0102] = 0xEF;
    bus[0xBEEF] = 0x42;

    reg_file.pc = 0x0101;
    opcode.ld_a_nn(cocoboy::soc::Sm83Opcode::LD_A_NN);
    CHECK(static_cast<uint8_t>(reg_file.a) == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ld_nn_a(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD (NN), A");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    bus[0x0101] = 0xBE;
    bus[0x0102] = 0xEF;
    reg_file.a = 0x42;

    reg_file.pc = 0x0101;
    opcode.ld_nn_a(cocoboy::soc::Sm83Opcode::LD_NN_A);
    CHECK(bus[0xBEEF] == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ldh_a_c(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LDH A, (C)");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.c = 0x01;
    bus[0x01FF] = 0x42;
    opcode.ldh_a_c(cocoboy::soc::Sm83Opcode::LDH_A_C);
    CHECK(static_cast<uint8_t>(reg_file.a) == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ldh_c_a(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD (C), A");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.c = 0x01;
    reg_file.a = 0x42;
    opcode.ldh_c_a(cocoboy::soc::Sm83Opcode::LDH_C_A);
    CHECK(bus[0x01FF] == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ldh_a_n(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD A, (N)");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    bus[0x0100] = 0xBF;
    bus[0xBFFF] = 0x42;
    opcode.ldh_a_n(cocoboy::soc::Sm83Opcode::LDH_A_N);
    CHECK(static_cast<uint8_t>(reg_file.a) == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ldh_n_a(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD (N), A");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    bus[0x0100] = 0xBF;
    reg_file.a = 0x42;
    opcode.ldh_n_a(cocoboy::soc::Sm83Opcode::LDH_N_A);
    CHECK(bus[0xBFFF] == 0x42);
}

TEST_CASE("void Sm83OpcodeRunner::ld_a_hlm(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD A, (HL-)");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.hl = 0xFFE0;
    bus[reg_file.hl] = 0x42;
    opcode.ld_a_hlm(cocoboy::soc::Sm83Opcode::LD_A_HLM);
    CHECK(static_cast<uint8_t>(reg_file.a) == 0x42);
    CHECK(static_cast<uint16_t>(reg_file.hl) == 0xFFDF);
}

TEST_CASE("void Sm83OpcodeRunner::ld_hlm_a(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD (HL-), A");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.hl = 0xFFE0;
    reg_file.a = 0x42;
    opcode.ld_hlm_a(cocoboy::soc::Sm83Opcode::LD_HLM_A);
    CHECK(bus[0xFFE0] == 0x42);
    CHECK(static_cast<uint16_t>(reg_file.hl) == 0xFFDF);
}

TEST_CASE("void Sm83OpcodeRunner::ld_a_hlp(uint8_t opcode)", "sm83_opcode_runner")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD A, (HL+)");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.hl = 0xFFE0;
    bus[reg_file.hl] = 0x42;
    opcode.ld_a_hlp(cocoboy::soc::Sm83Opcode::LD_A_HLP);
    CHECK(static_cast<uint8_t>(reg_file.a) == 0x42);
    CHECK(static_cast<uint16_t>(reg_file.hl) == 0xFFE1);
}

TEST_CASE("void Sm83OpcodeRunner::ld_hlp_a(uint8_t opcode)", "sm83_opcode_runner")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("LD (HL+), A");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.hl = 0xFFE0;
    reg_file.a = 0x42;
    opcode.ld_hlp_a(cocoboy::soc::Sm83Opcode::LD_HLP_A);
    CHECK(bus[0xFFE0] == 0x42);
    CHECK(static_cast<uint16_t>(reg_file.hl) == 0xFFE1);
}
