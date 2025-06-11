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


static void ld_r_r(Sm83& cpu, uint8_t opcode)
{
    uint8_t reg1 = (opcode & 0x38) >> 3;
    uint8_t reg2 = opcode & 0x07;
    cpu.m_reg.r8[reg1] = cpu.m_reg.r8[reg2];
}

/* static void ld_r_n(Sm83& cpu, uint8_t opcode) */
/* { */
/*     uint8_t reg = (opcode & 0x38) >> 3; */
/*     uint8_t imm8 = cpu.m_bus.read_byte(cpu.m_reg.pc); */
/*     ++cpu.m_reg.pc; */
/*     cpu.m_reg.r8[reg] = imm8; */
/* } */

enum Sm83Opcode : uint8_t
{
    // Misc.
    NOP     = 0x00,
    HALT    = 0x76,
    STOP    = 0x10,

    // LD R, R'
    LD_B_B  = 0x40,
    LD_B_C  = 0x41,
    LD_B_D  = 0x42,
    LD_B_E  = 0x43,
    LD_B_H  = 0x44,
    LD_B_L  = 0x45,
    LD_B_A  = 0x47,
    LD_C_B  = 0x48,
    LD_C_C  = 0x49,
    LD_C_D  = 0x4A,
    LD_C_E  = 0x4B,
    LD_C_H  = 0x4C,
    LD_C_L  = 0x4D,
    LD_C_A  = 0x4F,
    LD_D_B  = 0x50,
    LD_D_C  = 0x51,
    LD_D_D  = 0x52,
    LD_D_E  = 0x53,
    LD_D_H  = 0x54,
    LD_D_L  = 0x55,
    LD_D_A  = 0x57,
    LD_E_B  = 0x58,
    LD_E_C  = 0x59,
    LD_E_D  = 0x5A,
    LD_E_E  = 0x5B,
    LD_E_H  = 0x5C,
    LD_E_L  = 0x5D,
    LD_E_A  = 0x5F,
    LD_H_B  = 0x60,
    LD_H_C  = 0x61,
    LD_H_D  = 0x62,
    LD_H_E  = 0x63,
    LD_H_H  = 0x64,
    LD_H_L  = 0x65,
    LD_H_A  = 0x67,
    LD_L_B  = 0x68,
    LD_L_C  = 0x69,
    LD_L_D  = 0x6A,
    LD_L_E  = 0x6B,
    LD_L_H  = 0x6C,
    LD_L_L  = 0x6D,
    LD_L_A  = 0x6F,
    LD_A_B  = 0x78,
    LD_A_C  = 0x79,
    LD_A_D  = 0x7A,
    LD_A_E  = 0x7B,
    LD_A_H  = 0x7C,
    LD_A_L  = 0x7D,
    LD_A_A  = 0x7F,

    // LD R, N
    LD_B_N  = 0x06,
    LD_C_N  = 0x0E,
    LD_D_N  = 0x16,
    LD_E_N  = 0x1E,
    LD_H_N  = 0x26,
    LD_L_N  = 0x2E,
    LD_A_N  = 0x3E,

    // LD R, (HL)
    LD_B_HL = 0x46,
    LD_C_HL = 0x4E,
    LD_D_HL = 0x56,
    LD_E_HL = 0x5E,
    LD_H_HL = 0x66,
    LD_L_HL = 0x6E,
    LD_A_HL = 0x7E,

    // LD (HL) R
    LD_HL_B = 0x70,
    LD_HL_C = 0x71,
    LD_HL_D = 0x72,
    LD_HL_E = 0x73,
    LD_HL_H = 0x74,
    LD_HL_L = 0x75,
    LD_HL_A = 0x77,

    // LD (HL) N
    LD_HL_N = 0x36,
};

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
        ld_r_r(*this, opcode);
        break;
    default:
        m_logger->error("Invalid opcode 0x{:02X}", opcode);
        break;
    };
}
}  // namespace cocoboy
