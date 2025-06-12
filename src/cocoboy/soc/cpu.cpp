// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cocoboy/soc/cpu.hpp"
#include "cocoboy/soc/memory.hpp"

#include <spdlog/logger.h>
#include <cstdint>
#include <memory>

namespace cocoboy::soc {
Sm83RegisterFile::Sm83RegisterFile()
    : a(0x01),
      f(0x00),
      f_z(f),
      f_n(f),
      f_h(f),
      f_c(f),
      b(0xFF),
      c(0x13),
      d(0x00),
      e(0xC1),
      h(0x84),
      l(0x03),
      af(a, f),
      bc(b, c),
      de(d, e),
      hl(h, l),
      sp(0xFFFE),
      pc(0x0100)
{
}

Sm83OpcodeRunner::Sm83OpcodeRunner(std::shared_ptr<spdlog::logger> logger,
                                   Sm83RegisterFile& reg,
                                   MemoryBus& bus)
    : m_logger(logger), m_reg(reg), m_bus(bus)
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

void Sm83OpcodeRunner::ld_r_r(uint8_t opcode)
{
    uint8_t regx = (opcode & 0x38) >> 3;
    uint8_t regy = opcode & 0x07;
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD {2} {3}", m_reg.pc - 1, opcode, reg8_name(regx),
                    reg8_name(regy));
    *m_reg.r8[regx] = *m_reg.r8[regy];
}

void Sm83OpcodeRunner::ld_r_n(uint8_t opcode)
{
    uint8_t reg = (opcode & 0x38) >> 3;
    uint8_t data = m_bus[m_reg.pc];
    m_logger->debug("Run: [{0:04X} -> {1:02X} {2:02X}] LD {3}, {2:02X}", m_reg.pc - 1, opcode, data,
                    reg8_name(reg));
    ++m_reg.pc;
    *m_reg.r8[reg] = data;
}

void Sm83OpcodeRunner::ld_r_hl(uint8_t opcode)
{
    uint8_t reg = (opcode & 0x38) >> 3;
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD {2}, (HL)", m_reg.pc - 1, opcode, reg8_name(reg));
    *m_reg.r8[reg] = m_bus[m_reg.hl];
}

void Sm83OpcodeRunner::ld_hl_r(uint8_t opcode)
{
    uint8_t reg = opcode & 0x03;
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (HL), {2}", m_reg.pc - 1, opcode, reg8_name(reg));
    m_bus[m_reg.hl] = *m_reg.r8[reg];
}

void Sm83OpcodeRunner::ld_hl_n(uint8_t opcode)
{
    uint8_t data = m_bus[m_reg.pc];
    m_logger->debug("Run: [{0:04X} -> {1:02X} {2:02X}] LD (HL) {2:02}", m_reg.pc - 1, opcode, data);
    ++m_reg.pc;
    m_bus[m_reg.hl] = data;
}

void Sm83OpcodeRunner::ld_a_bc(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (BC)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.bc];
}

void Sm83OpcodeRunner::ld_a_de(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (DE)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.bc];
}

void Sm83OpcodeRunner::ld_bc_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (BC), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.bc] = m_reg.a;
}

void Sm83OpcodeRunner::ld_de_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (DE), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.de] = m_reg.a;
}

void Sm83OpcodeRunner::ld_a_nn(uint8_t opcode)
{
    uint8_t high = m_bus[m_reg.pc];
    uint8_t low = m_bus[m_reg.pc + 1];
    uint16_t data = (high << 8) | low;  // NOLINT
    m_logger->debug("Run: [{0:04X} -> {1:02X} {2:02X} {3:02X}] LD A, ({4:04X})", m_reg.pc - 1,
                    opcode, high, low, data);
    m_reg.pc = m_reg.pc + 2;
    m_reg.a = m_bus[data];
}

void Sm83OpcodeRunner::ldh_a_n(uint8_t opcode)
{
    uint8_t high = m_bus[m_reg.pc];
    uint8_t low = m_bus[m_reg.pc + 1];
    uint16_t data = (high << 8) | low;
    m_logger->debug("Run: [{0:04X} -> {1:02X} {2:02X} {3:02X}] LD ({4:04X}) A", m_reg.pc - 1,
                    opcode, high, low, data);
    m_reg.pc = m_reg.pc + 2;
    m_bus[data] = m_reg.a;
}

void Sm83OpcodeRunner::ldh_a_c(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LDH A, (C)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[(m_reg.c << 8) | 0xFF];
}

void Sm83OpcodeRunner::ldh_c_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LDH (C), A", m_reg.pc - 1, opcode);
    m_bus[(m_reg.c << 8) | 0xFF] = m_reg.a;
}

void Sm83OpcodeRunner::ldh_n_a(uint8_t opcode)
{
    uint8_t offset = m_bus[m_reg.pc];
    m_logger->debug("Run: [{0:04X} -> {1:02X} {2:02X}] LDH ({2:02X}) A", m_reg.pc - 1, opcode,
                    offset);
    ++m_reg.pc;
    m_bus[(offset << 8) | 0xFF] = m_reg.a;
}

void Sm83OpcodeRunner::ld_a_hlm(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (HL-)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.hl];
    --m_reg.hl;
}

void Sm83OpcodeRunner::ld_hlm_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (HL-), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.hl] = m_reg.a;
    --m_reg.hl;
}

void Sm83OpcodeRunner::ld_a_hlp(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD A, (HL+)", m_reg.pc - 1, opcode);
    m_reg.a = m_bus[m_reg.hl];
    ++m_reg.hl;
}

void Sm83OpcodeRunner::ld_hlp_a(uint8_t opcode)
{
    m_logger->debug("Run: [{0:04X} -> {1:02X}] LD (HL+), A", m_reg.pc - 1, opcode);
    m_bus[m_reg.hl] = m_reg.a;
    ++m_reg.hl;
}

Sm83::Sm83(std::shared_ptr<spdlog::logger> logger, MemoryBus& bus)
    : m_logger(logger), m_bus(bus), m_opcode(m_logger, m_reg, m_bus)
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
        case Sm83Opcode::LD_B_B:
        case Sm83Opcode::LD_B_C:
        case Sm83Opcode::LD_B_D:
        case Sm83Opcode::LD_B_E:
        case Sm83Opcode::LD_B_H:
        case Sm83Opcode::LD_B_L:
        case Sm83Opcode::LD_B_A:
        case Sm83Opcode::LD_C_B:
        case Sm83Opcode::LD_C_C:
        case Sm83Opcode::LD_C_D:
        case Sm83Opcode::LD_C_E:
        case Sm83Opcode::LD_C_H:
        case Sm83Opcode::LD_C_L:
        case Sm83Opcode::LD_C_A:
        case Sm83Opcode::LD_D_B:
        case Sm83Opcode::LD_D_C:
        case Sm83Opcode::LD_D_D:
        case Sm83Opcode::LD_D_E:
        case Sm83Opcode::LD_D_H:
        case Sm83Opcode::LD_D_L:
        case Sm83Opcode::LD_D_A:
        case Sm83Opcode::LD_E_B:
        case Sm83Opcode::LD_E_C:
        case Sm83Opcode::LD_E_D:
        case Sm83Opcode::LD_E_E:
        case Sm83Opcode::LD_E_H:
        case Sm83Opcode::LD_E_L:
        case Sm83Opcode::LD_E_A:
        case Sm83Opcode::LD_H_B:
        case Sm83Opcode::LD_H_C:
        case Sm83Opcode::LD_H_D:
        case Sm83Opcode::LD_H_E:
        case Sm83Opcode::LD_H_H:
        case Sm83Opcode::LD_H_L:
        case Sm83Opcode::LD_H_A:
        case Sm83Opcode::LD_L_B:
        case Sm83Opcode::LD_L_C:
        case Sm83Opcode::LD_L_D:
        case Sm83Opcode::LD_L_E:
        case Sm83Opcode::LD_L_H:
        case Sm83Opcode::LD_L_L:
        case Sm83Opcode::LD_L_A:
        case Sm83Opcode::LD_A_B:
        case Sm83Opcode::LD_A_C:
        case Sm83Opcode::LD_A_D:
        case Sm83Opcode::LD_A_E:
        case Sm83Opcode::LD_A_H:
        case Sm83Opcode::LD_A_L:
        case Sm83Opcode::LD_A_A:
            m_opcode.ld_r_r(opcode);
            break;
        case Sm83Opcode::LD_B_N:
        case Sm83Opcode::LD_C_N:
        case Sm83Opcode::LD_D_N:
        case Sm83Opcode::LD_E_N:
        case Sm83Opcode::LD_H_N:
        case Sm83Opcode::LD_L_N:
        case Sm83Opcode::LD_A_N:
            m_opcode.ld_r_n(opcode);
            break;
        case Sm83Opcode::LD_B_HL:
        case Sm83Opcode::LD_C_HL:
        case Sm83Opcode::LD_D_HL:
        case Sm83Opcode::LD_E_HL:
        case Sm83Opcode::LD_H_HL:
        case Sm83Opcode::LD_L_HL:
        case Sm83Opcode::LD_A_HL:
            m_opcode.ld_r_hl(opcode);
            break;
        case Sm83Opcode::LD_HL_B:
        case Sm83Opcode::LD_HL_C:
        case Sm83Opcode::LD_HL_D:
        case Sm83Opcode::LD_HL_E:
        case Sm83Opcode::LD_HL_H:
        case Sm83Opcode::LD_HL_L:
        case Sm83Opcode::LD_HL_A:
            m_opcode.ld_hl_r(opcode);
            break;
        case Sm83Opcode::LD_HL_N:
            m_opcode.ld_hl_n(opcode);
            break;
        case Sm83Opcode::LD_A_BC:
            m_opcode.ld_a_bc(opcode);
            break;
        case Sm83Opcode::LD_A_DE:
            m_opcode.ld_a_de(opcode);
            break;
        case Sm83Opcode::LD_BC_A:
            m_opcode.ld_bc_a(opcode);
            break;
        case Sm83Opcode::LD_DE_A:
            m_opcode.ld_de_a(opcode);
            break;
        case Sm83Opcode::LD_A_NN:
            m_opcode.ld_a_nn(opcode);
            break;
        case Sm83Opcode::LDH_A_C:
            m_opcode.ldh_a_c(opcode);
            break;
        case Sm83Opcode::LDH_C_A:
            m_opcode.ldh_c_a(opcode);
            break;
        case Sm83Opcode::LDH_A_N:
            m_opcode.ldh_a_n(opcode);
            break;
        case Sm83Opcode::LDH_N_A:
            m_opcode.ldh_n_a(opcode);
            break;
        case Sm83Opcode::LD_A_HLM:
            m_opcode.ld_a_hlm(opcode);
            break;
        case Sm83Opcode::LD_HLM_A:
            m_opcode.ld_hlm_a(opcode);
            break;
        case Sm83Opcode::LD_A_HLP:
            m_opcode.ld_a_hlp(opcode);
            break;
        case Sm83Opcode::LD_HLP_A:
            m_opcode.ld_hlp_a(opcode);
            break;
        default:
            m_logger->error("Invalid opcode 0x{:02X}", opcode);
            break;
    };
}
}  // namespace cocoboy::soc
