// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cocoboy/soc/memory.hpp"
#include "cocoboy/soc/cpu.hpp"

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

Sm83::Sm83(std::shared_ptr<spdlog::logger> logger, MemoryBus& bus) : m_logger(logger), m_bus(bus)
{
    m_logger->trace("Construct new SM83 CPU");
}

/* static void ld_r_n(Sm83& cpu, uint8_t opcode) */
/* { */
/*     uint8_t reg = (opcode & 0x38) >> 3; */
/*     uint8_t imm8 = cpu.m_bus.read_byte(cpu.m_reg.pc); */
/*     ++cpu.m_reg.pc; */
/*     cpu.m_reg.r8[reg] = imm8; */
/* } */

void Sm83::execute()
{
    uint8_t opcode = m_bus.read_byte(m_reg.pc);
    ++m_reg.pc;

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
        ld_r_r(opcode);
        break;
    default:
        m_logger->error("Invalid opcode 0x{:02X}", opcode);
        break;
    };
}

void Sm83::ld_r_r(uint8_t opcode)
{
    uint8_t reg1 = (opcode & 0x38) >> 3;
    uint8_t reg2 = opcode & 0x07;
    m_reg.r8[reg1] = m_reg.r8[reg2];
}
}  // namespace cocoboy
