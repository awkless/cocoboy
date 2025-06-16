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
};

void ld_b_b(Sm83State& cpu)
{
    cpu.b = cpu.b;
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

    m_logger->debug(
        "Execute [{0:04X}: {1:02X}]: {2}", m_state.pc - opcode.length, target, opcode.mnemonic
    );
    opcode.execute(m_state);
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
