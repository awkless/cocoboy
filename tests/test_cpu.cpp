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
        uint8_t regx = (target & 0x38) >> 3; // NOLINT
        uint8_t regy = target & 0x07; // NOLINT

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
        bus.write_byte(reg_file.pc, ld_r_n_targets[i]);
        ++reg_file.pc;
        bus.write_byte(reg_file.pc, values[i]);
        ++reg_file.pc;
    }

    reg_file.pc = 0x0100;
    for (size_t i = 0; i < max_ld_r_n_targets; ++i) {
        uint8_t target = bus.read_byte(reg_file.pc);
        ++reg_file.pc;

        uint8_t regx = (target & 0x38) >> 3; // NOLINT
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

    reg_file.hl = 0xBEEF; // NOLINT
    bus.write_byte(reg_file.hl, 0x42); // NOLINT

    reg_file.pc = 0x0100; // NOLINT
    for (size_t i = 0; i < max_ld_r_hl_targets; ++i) {
        bus.write_byte(reg_file.pc, ld_r_hl_targets[i]);
        ++reg_file.pc;
    }

    reg_file.pc = 0x0100; // NOLINT
    for (size_t i = 0; i < max_ld_r_hl_targets; ++i) {
        uint8_t target = bus.read_byte(reg_file.pc);
        ++reg_file.pc;

        uint8_t regx = (target & 0x38) >> 3; // NOLINT
        opcode.ld_r_hl(target);
        CHECK(static_cast<uint8_t>(*reg_file.r8[regx]) == 0x42); // NOLINT
        reg_file.hl = 0xBEEF; // NOLINT
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

    reg_file.pc = 0x0100; // NOLINT
    for (size_t i = 0; i < max_ld_hl_r_targets; ++i) {
        bus.write_byte(reg_file.pc, ld_hl_r_targets[i]);
        ++reg_file.pc;
    }

    reg_file.hl = 0xBEEF; // NOLINT
    reg_file.pc = 0x0100; // NOLINT
    for (size_t i = 0; i < max_ld_r_hl_targets; ++i) {
        uint8_t target = bus.read_byte(reg_file.pc);
        ++reg_file.pc;

        uint8_t regx = target & 0x03;
        opcode.ld_hl_r(target);
        CHECK(bus.read_byte(reg_file.hl) == values[regx]); // NOLINT
        reg_file.hl = 0xBEEF; // NOLINT
    }
}

TEST_CASE("void Sm83OpcodeRunner::ld_hl_n(uint8_t opcode)", "[sm83_opcode_runner]")
{
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("void Sm83::ld_hl_n() test");
    cocoboy::soc::MemoryBus bus(logger);
    cocoboy::soc::Sm83RegisterFile reg_file;
    cocoboy::soc::Sm83OpcodeRunner opcode(logger, reg_file, bus);

    reg_file.pc = 0x0100; // NOLINT
    constexpr uint8_t values[3] = { 0xBF, 0x42, 0xFF };
    for (size_t i = 0; i < 3; ++i) {
        bus.write_byte(reg_file.pc, cocoboy::soc::Sm83Opcode::LD_HL_N);
        ++reg_file.pc;
        bus.write_byte(reg_file.pc, values[i]);
        ++reg_file.pc;
    }

    reg_file.hl = 0xBEEF;
    reg_file.pc = 0x0100; // NOLINT
    for (size_t i = 0; i < 3; ++i) {
        uint8_t target = bus.read_byte(reg_file.pc);
        ++reg_file.pc;

        opcode.ld_hl_n(target);
        CHECK(bus.read_byte(reg_file.hl) == values[i]); // NOLINT
    }
}
