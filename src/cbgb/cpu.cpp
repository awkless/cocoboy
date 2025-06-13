// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include <cstdint>
#include <memory>

#include "cbgb/cpu.hpp"

#include <spdlog/logger.h>

#include "cbgb/memory.hpp"

namespace cbgb::cpu {
RegisterFile::RegisterFile()
    : a(0x01)
    , f(0x00)
    , f_z(f)
    , f_n(f)
    , f_h(f)
    , f_c(f)
    , b(0xFF)
    , c(0x13)
    , d(0x00)
    , e(0xC1)
    , h(0x84)
    , l(0x03)
    , af(a, f)
    , bc(b, c)
    , de(d, e)
    , hl(h, l)
    , sp(0xFFFE)
    , pc(0x0100)
{
}

Opcode::Opcode(
    std::shared_ptr<spdlog::logger> logger, RegisterFile& reg, cbgb::memory::MemoryBus& bus
)
    : m_logger(logger)
    , m_reg(reg)
    , m_bus(bus)
{
}

static std::string reg8_name(uint8_t target)
{
    switch (target) {
    case 0:
        return "B";
    case 1:
        return "C";
    case 2:
        return "D";
    case 3:
        return "E";
    case 4:
        return "H";
    case 5:
        return "L";
    case 7:
        return "A";
    default:
        return "?";
    }
}

void Opcode::ld_r_r(uint8_t opcode)
{
    uint8_t regx = (opcode & 0x38) >> 3;
    uint8_t regy = opcode & 0x07;
    m_logger->debug(
        "Run: [{0:04X} -> {1:02X}] LD {2} {3}",
        m_reg.pc - 1,
        opcode,
        reg8_name(regx),
        reg8_name(regy)
    );
    *m_reg.r8[regx] = *m_reg.r8[regy];
}

void Opcode::ld_r_n(uint8_t opcode)
{
    uint8_t reg = (opcode & 0x38) >> 3;
    uint8_t data = m_bus[m_reg.pc];
    m_logger->debug(
        "Run: [{0:04X} -> {1:02X} {2:02X}] LD {3}, {2:02X}",
        m_reg.pc - 1,
        opcode,
        data,
        reg8_name(reg)
    );
    ++m_reg.pc;
    *m_reg.r8[reg] = data;
}

void Opcode::ld_r_hl(uint8_t opcode)
{
    uint8_t reg = (opcode & 0x38) >> 3;
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD {2}, (HL)", m_reg.pc - 1, opcode, reg8_name(reg));
    *m_reg.r8[reg] = m_bus[m_reg.hl];
}

void Opcode::ld_hl_r(uint8_t opcode)
{
    uint8_t reg = opcode & 0x03;
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (HL), {2}", m_reg.pc - 1, opcode, reg8_name(reg));
    m_bus[m_reg.hl] = *m_reg.r8[reg];
}

void Opcode::ld_hl_n(uint8_t opcode)
{
    uint8_t data = m_bus[m_reg.pc];
    m_logger->debug("Run: [{0:04X} -> {1:02X} {2:02X}] LD (HL) {2:02}", m_reg.pc - 1, opcode, data);
    ++m_reg.pc;
    m_bus[m_reg.hl] = data;
}

void Opcode::ld_a_bc(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (BC)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.bc];
}

void Opcode::ld_a_de(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (DE)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.de];
}

void Opcode::ld_bc_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (BC), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.bc] = m_reg.a;
}

void Opcode::ld_de_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (DE), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.de] = m_reg.a;
}

void Opcode::ld_a_nn(uint8_t opcode)
{
    uint8_t high = m_bus[m_reg.pc];
    uint8_t low = m_bus[m_reg.pc + 1];
    uint16_t addr = static_cast<uint16_t>((high << 8) | low);
    m_logger->debug(
        "Run: [{0:04X} -> {1:02X} {2:02X} {3:02X}] LD A, ({4:04X})",
        m_reg.pc - 1,
        opcode,
        high,
        low,
        addr
    );
    m_reg.pc = m_reg.pc + 2;
    m_reg.a = m_bus[addr];
}

void Opcode::ld_nn_a(uint8_t opcode)
{
    uint8_t high = m_bus[m_reg.pc];
    uint8_t low = m_bus[m_reg.pc + 1];
    uint16_t addr = static_cast<uint16_t>((high << 8) | low);
    m_logger->debug(
        "Run: [{0:04X} -> {1:02X} {2:02X} {3:02X}] LD ({4:04X}) A",
        m_reg.pc - 1,
        opcode,
        high,
        low,
        addr
    );
    m_reg.pc = m_reg.pc + 2;
    m_bus[addr] = m_reg.a;
}

void Opcode::ldh_a_c(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LDH A, (C)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[static_cast<uint16_t>((m_reg.c << 8) | 0xFF)];
}

void Opcode::ldh_c_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LDH (C), A", m_reg.pc - 1, opcode);
    m_bus[static_cast<uint16_t>((m_reg.c << 8) | 0xFF)] = m_reg.a;
}

void Opcode::ldh_a_n(uint8_t opcode)
{
    uint8_t offset = m_bus[m_reg.pc];
    m_logger->debug(
        "Run: [{0:04X} -> {1:02X} {2:02X}] LDH ({2:02X}) A", m_reg.pc - 1, opcode, offset
    );
    ++m_reg.pc;
    m_reg.a = m_bus[static_cast<uint16_t>((offset << 8) | 0xFF)];
}

void Opcode::ldh_n_a(uint8_t opcode)
{
    uint8_t offset = m_bus[m_reg.pc];
    m_logger->debug(
        "Run: [{0:04X} -> {1:02X} {2:02X}] LDH ({2:02X}) A", m_reg.pc - 1, opcode, offset
    );
    ++m_reg.pc;
    m_bus[static_cast<uint16_t>((offset << 8) | 0xFF)] = m_reg.a;
}

void Opcode::ld_a_hlm(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (HL-)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.hl];
    --m_reg.hl;
}

void Opcode::ld_hlm_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (HL-), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.hl] = m_reg.a;
    --m_reg.hl;
}

void Opcode::ld_a_hlp(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (HL+)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.hl];
    ++m_reg.hl;
}

void Opcode::ld_hlp_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (HL+), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.hl] = m_reg.a;
    ++m_reg.hl;
}

void Opcode::ld_rr_nn(uint8_t opcode) { }

Sm83::Sm83(std::shared_ptr<spdlog::logger> logger, cbgb::memory::MemoryBus& bus)
    : m_logger(logger)
    , m_bus(bus)
    , m_run(m_logger, m_reg, m_bus)
{
    m_logger->trace("Construct new SM83 CPU");
}

void Sm83::step()
{
    // Fetch opcode...
    uint8_t opcode = m_bus[m_reg.pc];
    ++m_reg.pc;

    // Decode and execute opcode...
    switch (opcode) {
    case OpcodeKind::LD_B_B:
    case OpcodeKind::LD_B_C:
    case OpcodeKind::LD_B_D:
    case OpcodeKind::LD_B_E:
    case OpcodeKind::LD_B_H:
    case OpcodeKind::LD_B_L:
    case OpcodeKind::LD_B_A:
    case OpcodeKind::LD_C_B:
    case OpcodeKind::LD_C_C:
    case OpcodeKind::LD_C_D:
    case OpcodeKind::LD_C_E:
    case OpcodeKind::LD_C_H:
    case OpcodeKind::LD_C_L:
    case OpcodeKind::LD_C_A:
    case OpcodeKind::LD_D_B:
    case OpcodeKind::LD_D_C:
    case OpcodeKind::LD_D_D:
    case OpcodeKind::LD_D_E:
    case OpcodeKind::LD_D_H:
    case OpcodeKind::LD_D_L:
    case OpcodeKind::LD_D_A:
    case OpcodeKind::LD_E_B:
    case OpcodeKind::LD_E_C:
    case OpcodeKind::LD_E_D:
    case OpcodeKind::LD_E_E:
    case OpcodeKind::LD_E_H:
    case OpcodeKind::LD_E_L:
    case OpcodeKind::LD_E_A:
    case OpcodeKind::LD_H_B:
    case OpcodeKind::LD_H_C:
    case OpcodeKind::LD_H_D:
    case OpcodeKind::LD_H_E:
    case OpcodeKind::LD_H_H:
    case OpcodeKind::LD_H_L:
    case OpcodeKind::LD_H_A:
    case OpcodeKind::LD_L_B:
    case OpcodeKind::LD_L_C:
    case OpcodeKind::LD_L_D:
    case OpcodeKind::LD_L_E:
    case OpcodeKind::LD_L_H:
    case OpcodeKind::LD_L_L:
    case OpcodeKind::LD_L_A:
    case OpcodeKind::LD_A_B:
    case OpcodeKind::LD_A_C:
    case OpcodeKind::LD_A_D:
    case OpcodeKind::LD_A_E:
    case OpcodeKind::LD_A_H:
    case OpcodeKind::LD_A_L:
    case OpcodeKind::LD_A_A:
        m_run.ld_r_r(opcode);
        break;
    case OpcodeKind::LD_B_N:
    case OpcodeKind::LD_C_N:
    case OpcodeKind::LD_D_N:
    case OpcodeKind::LD_E_N:
    case OpcodeKind::LD_H_N:
    case OpcodeKind::LD_L_N:
    case OpcodeKind::LD_A_N:
        m_run.ld_r_n(opcode);
        break;
    case OpcodeKind::LD_B_HL:
    case OpcodeKind::LD_C_HL:
    case OpcodeKind::LD_D_HL:
    case OpcodeKind::LD_E_HL:
    case OpcodeKind::LD_H_HL:
    case OpcodeKind::LD_L_HL:
    case OpcodeKind::LD_A_HL:
        m_run.ld_r_hl(opcode);
        break;
    case OpcodeKind::LD_HL_B:
    case OpcodeKind::LD_HL_C:
    case OpcodeKind::LD_HL_D:
    case OpcodeKind::LD_HL_E:
    case OpcodeKind::LD_HL_H:
    case OpcodeKind::LD_HL_L:
    case OpcodeKind::LD_HL_A:
        m_run.ld_hl_r(opcode);
        break;
    case OpcodeKind::LD_HL_N:
        m_run.ld_hl_n(opcode);
        break;
    case OpcodeKind::LD_A_BC:
        m_run.ld_a_bc(opcode);
        break;
    case OpcodeKind::LD_A_DE:
        m_run.ld_a_de(opcode);
        break;
    case OpcodeKind::LD_BC_A:
        m_run.ld_bc_a(opcode);
        break;
    case OpcodeKind::LD_DE_A:
        m_run.ld_de_a(opcode);
        break;
    case OpcodeKind::LD_A_NN:
        m_run.ld_a_nn(opcode);
        break;
    case OpcodeKind::LD_NN_A:
        m_run.ld_nn_a(opcode);
        break;
    case OpcodeKind::LDH_A_C:
        m_run.ldh_a_c(opcode);
        break;
    case OpcodeKind::LDH_C_A:
        m_run.ldh_c_a(opcode);
        break;
    case OpcodeKind::LDH_A_N:
        m_run.ldh_a_n(opcode);
        break;
    case OpcodeKind::LDH_N_A:
        m_run.ldh_n_a(opcode);
        break;
    case OpcodeKind::LD_A_HLM:
        m_run.ld_a_hlm(opcode);
        break;
    case OpcodeKind::LD_HLM_A:
        m_run.ld_hlm_a(opcode);
        break;
    case OpcodeKind::LD_A_HLP:
        m_run.ld_a_hlp(opcode);
        break;
    case OpcodeKind::LD_HLP_A:
        m_run.ld_hlp_a(opcode);
        break;
    default:
        m_logger->error("Invalid opcode 0x{:02X}", opcode);
        break;
    };
}
} // namespace cbgb::cpu
