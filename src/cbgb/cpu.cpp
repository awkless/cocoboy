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
    LD_B_C = 0x41,
    LD_B_D = 0x42,
    LD_B_E = 0x43,
    LD_B_H = 0x44,
    LD_B_L = 0x45,
    LD_B_A = 0x47,
    LD_C_B = 0x48,
    LD_C_C = 0x49,
    LD_C_D = 0x4A,
    LD_C_E = 0x4B,
    LD_C_H = 0x4C,
    LD_C_L = 0x4D,
    LD_C_A = 0x4F,
    LD_D_B = 0x50,
    LD_D_C = 0x51,
    LD_D_D = 0x52,
    LD_D_E = 0x53,
    LD_D_H = 0x54,
    LD_D_L = 0x55,
    LD_D_A = 0x57,
    LD_E_B = 0x58,
    LD_E_C = 0x59,
    LD_E_D = 0x5A,
    LD_E_E = 0x5B,
    LD_E_H = 0x5C,
    LD_E_L = 0x5D,
    LD_E_A = 0x5F,
    LD_H_B = 0x60,
    LD_H_C = 0x61,
    LD_H_D = 0x62,
    LD_H_E = 0x63,
    LD_H_H = 0x64,
    LD_H_L = 0x65,
    LD_H_A = 0x67,
    LD_L_B = 0x68,
    LD_L_C = 0x69,
    LD_L_D = 0x6A,
    LD_L_E = 0x6B,
    LD_L_H = 0x6C,
    LD_L_L = 0x6D,
    LD_L_A = 0x6F,
    LD_A_B = 0x78,
    LD_A_C = 0x79,
    LD_A_D = 0x7A,
    LD_A_E = 0x7B,
    LD_A_H = 0x7C,
    LD_A_L = 0x7D,
    LD_A_A = 0x7F,
    LD_B_N = 0x06,
    LD_C_N = 0x0E,
    LD_D_N = 0x16,
    LD_E_N = 0x1E,
    LD_H_N = 0x26,
    LD_L_N = 0x2E,
    LD_A_N = 0x3E,
    LD_B_HL = 0x46,
    LD_C_HL = 0x4E,
    LD_D_HL = 0x56,
    LD_E_HL = 0x5E,
    LD_H_HL = 0x66,
    LD_L_HL = 0x6E,
    LD_A_HL = 0x7E,
    LD_HL_B = 0x70,
    LD_HL_C = 0x71,
    LD_HL_D = 0x72,
    LD_HL_E = 0x73,
    LD_HL_H = 0x74,
    LD_HL_L = 0x75,
    LD_HL_A = 0x77,
    LD_HL_N = 0x36,
    LD_A_BC = 0x0A,
    LD_A_DE = 0x1A,
    LD_BC_A = 0x02,
    LD_DE_A = 0x12,
    LD_A_NN = 0xFA,
    LD_NN_A = 0xEA,
    LDH_A_C = 0xF2,
    LDH_C_A = 0xE2,
    LDH_A_N = 0xF0,
    LDH_N_A = 0xE0,
    LD_A_HLD = 0x3A,
    LD_HLD_A = 0x32,
    LD_A_HLI = 0x2A,
    LD_HLI_A = 0x22,
};

static inline void ld_b_b(Sm83State& cpu)
{
    cpu.b = cpu.b;
}

static inline void ld_b_c(Sm83State& cpu)
{
    cpu.b = cpu.c;
}

static inline void ld_b_d(Sm83State& cpu)
{
    cpu.b = cpu.d;
}

static inline void ld_b_e(Sm83State& cpu)
{
    cpu.b = cpu.e;
}

static inline void ld_b_h(Sm83State& cpu)
{
    cpu.b = cpu.h;
}

static inline void ld_b_l(Sm83State& cpu)
{
    cpu.b = cpu.l;
}

static inline void ld_b_a(Sm83State& cpu)
{
    cpu.b = cpu.a;
}

static inline void ld_c_b(Sm83State& cpu)
{
    cpu.c = cpu.b;
}

static inline void ld_c_c(Sm83State& cpu)
{
    cpu.c = cpu.c;
}

static inline void ld_c_d(Sm83State& cpu)
{
    cpu.c = cpu.d;
}

static inline void ld_c_e(Sm83State& cpu)
{
    cpu.c = cpu.e;
}

static inline void ld_c_h(Sm83State& cpu)
{
    cpu.c = cpu.h;
}

static inline void ld_c_l(Sm83State& cpu)
{
    cpu.c = cpu.l;
}

static inline void ld_c_a(Sm83State& cpu)
{
    cpu.c = cpu.a;
}

static inline void ld_d_b(Sm83State& cpu)
{
    cpu.d = cpu.b;
}

static inline void ld_d_c(Sm83State& cpu)
{
    cpu.d = cpu.c;
}

static inline void ld_d_d(Sm83State& cpu)
{
    cpu.d = cpu.d;
}

static inline void ld_d_e(Sm83State& cpu)
{
    cpu.d = cpu.e;
}

static inline void ld_d_h(Sm83State& cpu)
{
    cpu.d = cpu.h;
}

static inline void ld_d_l(Sm83State& cpu)
{
    cpu.d = cpu.l;
}

static inline void ld_d_a(Sm83State& cpu)
{
    cpu.d = cpu.a;
}

static inline void ld_e_b(Sm83State& cpu)
{
    cpu.e = cpu.b;
}

static inline void ld_e_c(Sm83State& cpu)
{
    cpu.e = cpu.c;
}

static inline void ld_e_d(Sm83State& cpu)
{
    cpu.e = cpu.d;
}

static inline void ld_e_e(Sm83State& cpu)
{
    cpu.e = cpu.e;
}

static inline void ld_e_h(Sm83State& cpu)
{
    cpu.e = cpu.h;
}

static inline void ld_e_l(Sm83State& cpu)
{
    cpu.e = cpu.l;
}

static inline void ld_e_a(Sm83State& cpu)
{
    cpu.e = cpu.a;
}

static inline void ld_h_b(Sm83State& cpu)
{
    cpu.h = cpu.b;
}

static inline void ld_h_c(Sm83State& cpu)
{
    cpu.h = cpu.c;
}

static inline void ld_h_d(Sm83State& cpu)
{
    cpu.h = cpu.d;
}

static inline void ld_h_e(Sm83State& cpu)
{
    cpu.h = cpu.e;
}

static inline void ld_h_h(Sm83State& cpu)
{
    cpu.h = cpu.h;
}

static inline void ld_h_l(Sm83State& cpu)
{
    cpu.h = cpu.l;
}

static inline void ld_h_a(Sm83State& cpu)
{
    cpu.h = cpu.a;
}

static inline void ld_l_b(Sm83State& cpu)
{
    cpu.l = cpu.b;
}

static inline void ld_l_c(Sm83State& cpu)
{
    cpu.l = cpu.c;
}

static inline void ld_l_d(Sm83State& cpu)
{
    cpu.l = cpu.d;
}

static inline void ld_l_e(Sm83State& cpu)
{
    cpu.l = cpu.e;
}

static inline void ld_l_h(Sm83State& cpu)
{
    cpu.l = cpu.h;
}

static inline void ld_l_l(Sm83State& cpu)
{
    cpu.l = cpu.l;
}

static inline void ld_l_a(Sm83State& cpu)
{
    cpu.l = cpu.a;
}

static inline void ld_a_b(Sm83State& cpu)
{
    cpu.a = cpu.b;
}

static inline void ld_a_c(Sm83State& cpu)
{
    cpu.a = cpu.c;
}

static inline void ld_a_d(Sm83State& cpu)
{
    cpu.a = cpu.d;
}

static inline void ld_a_e(Sm83State& cpu)
{
    cpu.a = cpu.e;
}

static inline void ld_a_h(Sm83State& cpu)
{
    cpu.a = cpu.h;
}

static inline void ld_a_l(Sm83State& cpu)
{
    cpu.a = cpu.l;
}

static inline void ld_a_a(Sm83State& cpu)
{
    cpu.a = cpu.a;
}

static inline void ld_b_n(Sm83State& cpu)
{
    cpu.b = cpu.memory.read(cpu.pc++);
}

static inline void ld_c_n(Sm83State& cpu)
{
    cpu.c = cpu.memory.read(cpu.pc++);
}
static inline void ld_d_n(Sm83State& cpu)
{
    cpu.d = cpu.memory.read(cpu.pc++);
}

static inline void ld_e_n(Sm83State& cpu)
{
    cpu.e = cpu.memory.read(cpu.pc++);
}

static inline void ld_h_n(Sm83State& cpu)
{
    cpu.h = cpu.memory.read(cpu.pc++);
}

static inline void ld_l_n(Sm83State& cpu)
{
    cpu.l = cpu.memory.read(cpu.pc++);
}

static inline void ld_a_n(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(cpu.pc++);
}

static inline void ld_b_hl(Sm83State& cpu)
{
    cpu.b = cpu.memory.read(cpu.hl);
}

static inline void ld_c_hl(Sm83State& cpu)
{
    cpu.c = cpu.memory.read(cpu.hl);
}

static inline void ld_d_hl(Sm83State& cpu)
{
    cpu.d = cpu.memory.read(cpu.hl);
}

static inline void ld_e_hl(Sm83State& cpu)
{
    cpu.e = cpu.memory.read(cpu.hl);
}

static inline void ld_h_hl(Sm83State& cpu)
{
    cpu.h = cpu.memory.read(cpu.hl);
}

static inline void ld_l_hl(Sm83State& cpu)
{
    cpu.l = cpu.memory.read(cpu.hl);
}

static inline void ld_a_hl(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(cpu.hl);
}

static inline void ld_hl_b(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.b);
}

static inline void ld_hl_c(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.c);
}

static inline void ld_hl_d(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.d);
}

static inline void ld_hl_e(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.e);
}

static inline void ld_hl_h(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.h);
}

static inline void ld_hl_l(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.l);
}

static inline void ld_hl_a(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.a);
}

static inline void ld_hl_n(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl, cpu.memory.read(cpu.pc++));
}

static inline void ld_a_bc(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(cpu.bc);
}

static inline void ld_a_de(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(cpu.de);
}

static inline void ld_bc_a(Sm83State& cpu)
{
    cpu.memory.write(cpu.bc, cpu.a);
}

static inline void ld_de_a(Sm83State& cpu)
{
    cpu.memory.write(cpu.de, cpu.a);
}

static inline void ld_a_nn(Sm83State& cpu)
{
    uint8_t lsb = cpu.memory.read(cpu.pc++);
    uint8_t msb = cpu.memory.read(cpu.pc++);
    cpu.a = cpu.memory.read(static_cast<uint16_t>((lsb << 8) | msb));
}

static inline void ld_nn_a(Sm83State& cpu)
{
    uint8_t lsb = cpu.memory.read(cpu.pc++);
    uint8_t msb = cpu.memory.read(cpu.pc++);
    cpu.memory.write(static_cast<uint16_t>((lsb << 8) | msb), cpu.a);
}

static inline void ldh_a_c(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(static_cast<uint16_t>((cpu.c << 8) | 0xFF));
}

static inline void ldh_c_a(Sm83State& cpu)
{
    cpu.memory.write(static_cast<uint16_t>((cpu.c << 8) | 0xFF), cpu.a);
}

static inline void ldh_a_n(Sm83State& cpu)
{
    uint8_t lsb = cpu.memory.read(cpu.pc++);
    cpu.a = cpu.memory.read(static_cast<uint16_t>((lsb << 8) | 0xFF));
}

static inline void ldh_n_a(Sm83State& cpu)
{
    uint8_t lsb = cpu.memory.read(cpu.pc++);
    cpu.memory.write(static_cast<uint16_t>((lsb << 8) | 0xFF), cpu.a);
}

static inline void ld_a_hld(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(cpu.hl--);
}

static inline void ld_hld_a(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl--, cpu.a);
}

static inline void ld_a_hli(Sm83State& cpu)
{
    cpu.a = cpu.memory.read(cpu.hl++);
}

static inline void ld_hli_a(Sm83State& cpu)
{
    cpu.memory.write(cpu.hl++, cpu.a);
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
    table[OpcodeKind::LD_B_C] = Opcode { "LD B, C", 1, 1, ld_b_c };
    table[OpcodeKind::LD_B_D] = Opcode { "LD B, D", 1, 1, ld_b_d };
    table[OpcodeKind::LD_B_E] = Opcode { "LD B, E", 1, 1, ld_b_e };
    table[OpcodeKind::LD_B_H] = Opcode { "LD B, H", 1, 1, ld_b_h };
    table[OpcodeKind::LD_B_L] = Opcode { "LD B, L", 1, 1, ld_b_l };
    table[OpcodeKind::LD_B_A] = Opcode { "LD B, A", 1, 1, ld_b_a };
    table[OpcodeKind::LD_C_B] = Opcode { "LD B, B", 1, 1, ld_c_b };
    table[OpcodeKind::LD_C_C] = Opcode { "LD C, C", 1, 1, ld_c_c };
    table[OpcodeKind::LD_C_D] = Opcode { "LD C, D", 1, 1, ld_c_d };
    table[OpcodeKind::LD_C_E] = Opcode { "LD C, E", 1, 1, ld_c_e };
    table[OpcodeKind::LD_C_H] = Opcode { "LD C, H", 1, 1, ld_c_h };
    table[OpcodeKind::LD_C_L] = Opcode { "LD C, L", 1, 1, ld_c_l };
    table[OpcodeKind::LD_C_A] = Opcode { "LD C, A", 1, 1, ld_c_a };
    table[OpcodeKind::LD_D_B] = Opcode { "LD D, B", 1, 1, ld_d_b };
    table[OpcodeKind::LD_D_C] = Opcode { "LD D, C", 1, 1, ld_d_c };
    table[OpcodeKind::LD_D_D] = Opcode { "LD D, D", 1, 1, ld_d_d };
    table[OpcodeKind::LD_D_E] = Opcode { "LD D, E", 1, 1, ld_d_e };
    table[OpcodeKind::LD_D_H] = Opcode { "LD D, H", 1, 1, ld_d_h };
    table[OpcodeKind::LD_D_L] = Opcode { "LD D, L", 1, 1, ld_d_l };
    table[OpcodeKind::LD_D_A] = Opcode { "LD D, A", 1, 1, ld_d_a };
    table[OpcodeKind::LD_E_B] = Opcode { "LD E, B", 1, 1, ld_e_b };
    table[OpcodeKind::LD_E_C] = Opcode { "LD E, C", 1, 1, ld_e_c };
    table[OpcodeKind::LD_E_D] = Opcode { "LD E, D", 1, 1, ld_e_d };
    table[OpcodeKind::LD_E_E] = Opcode { "LD E, E", 1, 1, ld_e_e };
    table[OpcodeKind::LD_E_H] = Opcode { "LD E, H", 1, 1, ld_e_h };
    table[OpcodeKind::LD_E_L] = Opcode { "LD E, L", 1, 1, ld_e_l };
    table[OpcodeKind::LD_E_A] = Opcode { "LD E, A", 1, 1, ld_e_a };
    table[OpcodeKind::LD_H_B] = Opcode { "LD H, B", 1, 1, ld_h_b };
    table[OpcodeKind::LD_H_C] = Opcode { "LD H, C", 1, 1, ld_h_c };
    table[OpcodeKind::LD_H_D] = Opcode { "LD H, D", 1, 1, ld_h_d };
    table[OpcodeKind::LD_H_E] = Opcode { "LD H, E", 1, 1, ld_h_e };
    table[OpcodeKind::LD_H_H] = Opcode { "LD H, H", 1, 1, ld_h_h };
    table[OpcodeKind::LD_H_L] = Opcode { "LD H, L", 1, 1, ld_h_l };
    table[OpcodeKind::LD_H_A] = Opcode { "LD H, A", 1, 1, ld_h_a };
    table[OpcodeKind::LD_L_B] = Opcode { "LD L, B", 1, 1, ld_l_b };
    table[OpcodeKind::LD_L_C] = Opcode { "LD L, C", 1, 1, ld_l_c };
    table[OpcodeKind::LD_L_D] = Opcode { "LD L, D", 1, 1, ld_l_d };
    table[OpcodeKind::LD_L_E] = Opcode { "LD L, E", 1, 1, ld_l_e };
    table[OpcodeKind::LD_L_H] = Opcode { "LD L, H", 1, 1, ld_l_h };
    table[OpcodeKind::LD_L_L] = Opcode { "LD L, L", 1, 1, ld_l_l };
    table[OpcodeKind::LD_L_A] = Opcode { "LD L, A", 1, 1, ld_l_a };
    table[OpcodeKind::LD_A_B] = Opcode { "LD A, B", 1, 1, ld_a_b };
    table[OpcodeKind::LD_A_C] = Opcode { "LD A, C", 1, 1, ld_a_c };
    table[OpcodeKind::LD_A_D] = Opcode { "LD A, D", 1, 1, ld_a_d };
    table[OpcodeKind::LD_A_E] = Opcode { "LD A, E", 1, 1, ld_a_e };
    table[OpcodeKind::LD_A_H] = Opcode { "LD A, H", 1, 1, ld_a_h };
    table[OpcodeKind::LD_A_L] = Opcode { "LD A, L", 1, 1, ld_a_l };
    table[OpcodeKind::LD_A_A] = Opcode { "LD A, A", 1, 1, ld_a_a };
    table[OpcodeKind::LD_B_N] = Opcode { "LD B, n", 2, 2, ld_b_n };
    table[OpcodeKind::LD_C_N] = Opcode { "LD C, n", 2, 2, ld_c_n };
    table[OpcodeKind::LD_D_N] = Opcode { "LD D, n", 2, 2, ld_d_n };
    table[OpcodeKind::LD_E_N] = Opcode { "LD E, n", 2, 2, ld_e_n };
    table[OpcodeKind::LD_H_N] = Opcode { "LD H, n", 2, 2, ld_h_n };
    table[OpcodeKind::LD_L_N] = Opcode { "LD L, n", 2, 2, ld_l_n };
    table[OpcodeKind::LD_A_N] = Opcode { "LD A, n", 2, 2, ld_a_n };
    table[OpcodeKind::LD_B_HL] = Opcode { "LD B, (HL)", 1, 2, ld_b_hl };
    table[OpcodeKind::LD_C_HL] = Opcode { "LD C, (HL)", 1, 2, ld_c_hl };
    table[OpcodeKind::LD_D_HL] = Opcode { "LD D, (HL)", 1, 2, ld_d_hl };
    table[OpcodeKind::LD_E_HL] = Opcode { "LD E, (HL)", 1, 2, ld_e_hl };
    table[OpcodeKind::LD_H_HL] = Opcode { "LD H, (HL)", 1, 2, ld_h_hl };
    table[OpcodeKind::LD_L_HL] = Opcode { "LD L, (HL)", 1, 2, ld_l_hl };
    table[OpcodeKind::LD_A_HL] = Opcode { "LD A, (HL)", 1, 2, ld_a_hl };
    table[OpcodeKind::LD_HL_B] = Opcode { "LD (HL), B", 1, 2, ld_hl_b };
    table[OpcodeKind::LD_HL_B] = Opcode { "LD (HL), C", 1, 2, ld_hl_c };
    table[OpcodeKind::LD_HL_B] = Opcode { "LD (HL), D", 1, 2, ld_hl_d };
    table[OpcodeKind::LD_HL_B] = Opcode { "LD (HL), E", 1, 2, ld_hl_e };
    table[OpcodeKind::LD_HL_B] = Opcode { "LD (HL), H", 1, 2, ld_hl_h };
    table[OpcodeKind::LD_HL_B] = Opcode { "LD (HL), L", 1, 2, ld_hl_l };
    table[OpcodeKind::LD_HL_B] = Opcode { "LD (HL), A", 1, 2, ld_hl_a };
    table[OpcodeKind::LD_HL_N] = Opcode { "LD (HL), n", 2, 3, ld_hl_n };
    table[OpcodeKind::LD_A_BC] = Opcode { "LD A, (BC)", 1, 2, ld_a_bc };
    table[OpcodeKind::LD_A_DE] = Opcode { "LD A, (DE)", 1, 2, ld_a_de };
    table[OpcodeKind::LD_BC_A] = Opcode { "LD (BC), A", 1, 2, ld_bc_a };
    table[OpcodeKind::LD_DE_A] = Opcode { "LD (DE), A", 1, 2, ld_de_a };
    table[OpcodeKind::LD_A_NN] = Opcode { "LD A, (NN)", 3, 4, ld_a_nn };
    table[OpcodeKind::LD_NN_A] = Opcode { "LD (NN), A", 3, 4, ld_nn_a };
    table[OpcodeKind::LDH_A_C] = Opcode { "LDH A, (C)", 1, 2, ldh_a_c };
    table[OpcodeKind::LDH_C_A] = Opcode { "LDH (C), A", 1, 2, ldh_c_a };
    table[OpcodeKind::LDH_A_N] = Opcode { "LDH A, (n)", 2, 3, ldh_a_n };
    table[OpcodeKind::LDH_N_A] = Opcode { "LDH (n), A", 2, 3, ldh_n_a };
    table[OpcodeKind::LD_A_HLD] = Opcode { "LD A, (HL-)", 1, 2, ld_a_hld };
    table[OpcodeKind::LD_HLD_A] = Opcode { "LD (HL-), A", 1, 2, ld_hld_a };
    table[OpcodeKind::LD_A_HLI] = Opcode { "LD A, (HL+)", 1, 2, ld_a_hli };
    table[OpcodeKind::LD_HLI_A] = Opcode { "LD (HL+), A", 1, 2, ld_hli_a };
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
