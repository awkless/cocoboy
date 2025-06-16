// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include <cstdint>
#include <memory>
#include <string>

#include <fmt/format.h>
#include <spdlog/logger.h>

#include "cbgb/cpu.hpp"
#include "cbgb/memory.hpp"

namespace cbgb {
enum OpcodeKind : uint8_t {
    LD_B_B = 0x40,
    LD_B_N = 0x06,
    LD_C_N = 0x0E,
    LD_D_N = 0x16,
    LD_E_N = 0x1E,
    LD_H_N = 0x26,
    LD_L_N = 0x2E,
    LD_A_N = 0x3E,
};

void ld_b_b(Sm83State& cpu)
{
    cpu.b = cpu.b;
}

void ld_b_n(Sm83State& cpu)
{
    cpu.b = cpu.memory.read(cpu.pc++);
}

void ld_c_n(Sm83State& cpu)
{
    cpu.c = cpu.memory.read(cpu.pc++);
}
void ld_d_n(Sm83State& cpu)
{
    cpu.d = cpu.memory.read(cpu.pc++);
}

void ld_e_n(Sm83State& cpu)
{
    cpu.e = cpu.memory.read(cpu.pc++);
}

void ld_h_n(Sm83State& cpu)
{
    cpu.h = cpu.memory.read(cpu.pc++);
}

void ld_l_n(Sm83State& cpu)
{
    cpu.l = cpu.memory.read(cpu.pc++);
}

void ld_a_n(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(cpu.pc++);
}

struct Opcode final {
    std::string_view mnemonic;
    unsigned int length;
    unsigned int mcycle;
    void (*execute)(Sm83State&);
};

constexpr std::array<Opcode, 256> new_opcode_jump_table()
{
    std::array<Opcode, 256> table = {};
    table[OpcodeKind::LD_B_B] = Opcode { "LD B, B", 1, 1, ld_b_b };
    table[OpcodeKind::LD_B_N] = Opcode { "LD B, n", 2, 2, ld_b_n };
    table[OpcodeKind::LD_C_N] = Opcode { "LD C, n", 2, 2, ld_c_n };
    table[OpcodeKind::LD_D_N] = Opcode { "LD D, n", 2, 2, ld_d_n };
    table[OpcodeKind::LD_E_N] = Opcode { "LD E, n", 2, 2, ld_e_n };
    table[OpcodeKind::LD_H_N] = Opcode { "LD H, n", 2, 2, ld_h_n };
    table[OpcodeKind::LD_L_N] = Opcode { "LD L, n", 2, 2, ld_l_n };
    table[OpcodeKind::LD_A_N] = Opcode { "LD A, n", 2, 2, ld_a_n };
    return table;
}
constexpr std::array<Opcode, 256> opcode_jump_table = new_opcode_jump_table();

UndefinedOpcode::UndefinedOpcode(std::string what)
    : m_what(what)
{
}

const char* UndefinedOpcode::what() const noexcept
{
    return m_what.c_str();
}

Sm83State::Sm83State(MemoryBus& bus)
    : pc(0x0100)
    , sp(0xFFFE)
    , af(a, f)
    , bc(b, c)
    , de(d, e)
    , hl(h, l)
    , a(0x01)
    , f(0xB0)
    , b(0x00)
    , c(0x13)
    , d(0x00)
    , e(0xD8)
    , h(0x01)
    , l(0x4D)
    , fz(f)
    , fn(f)
    , fh(f)
    , fc(f)
    , memory(bus)
{
}

Sm83::Sm83(std::shared_ptr<spdlog::logger> log, MemoryBus& bus)
    : m_state(bus)
    , m_mcycles(0)
    , m_logger(log)
{
    m_logger->trace("Construct new SM83 CPU");
}

void Sm83::step()
{
    uint8_t target = m_state.memory.read(m_state.pc++);
    Opcode opcode = opcode_jump_table[target];
    if (opcode.execute == nullptr) {
        throw UndefinedOpcode(fmt::format(
            "Undefined opcode: [{0:04X}: {1:02X}] {2}", m_state.pc - 1, target, opcode.mnemonic
        ));
    }

    opcode.execute(m_state);
    m_logger->debug(
        "Execute [{0:04X}: {1:02X}] {2}", m_state.pc - opcode.length, target, opcode.mnemonic
    );
    m_mcycles += opcode.mcycle;
}

const Sm83State& Sm83::get_state()
{
    return m_state;
}

unsigned int Sm83::get_mcycle_count()
{
    return m_mcycles;
}
} // namespace cbgb
