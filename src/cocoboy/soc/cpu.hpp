// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#ifndef COCOBOY_SOC_CPU_HPP
#define COCOBOY_SOC_CPU_HPP

#include "cocoboy/soc/memory.hpp"

#include <fmt/format.h>
#include <spdlog/logger.h>
#include <cstdint>
#include <limits>
#include <memory>
#include <array>

namespace cocoboy::soc {
/// @brief SM83 register file representation.
struct Sm83RegisterFile final {
    /// @brief Construct new register file.
    ///
    /// @return New instance of register file.
    Sm83RegisterFile();

    /// Accumulator regsiter.
    Register<uint8_t> a;

    /// Flags register.
    Register<uint8_t> f;

    /// Zero flag.
    RegisterBit<7, 1, uint8_t> f_z;

    /// Subtraction flag (BCD).
    RegisterBit<6, 1, uint8_t> f_n;

    /// Half carry flag (BCD).
    RegisterBit<5, 1, uint8_t> f_h;

    /// Carry flag.
    RegisterBit<4, 1, uint8_t> f_c;

    /// General purpse register.
    Register<uint8_t> b;

    /// General purpse register.
    Register<uint8_t> c;

    /// General purpse register.
    Register<uint8_t> d;

    /// General purpse register.
    Register<uint8_t> e;

    /// High byte of memory address.
    Register<uint8_t> h;

    /// Low byte of memory address.
    Register<uint8_t> l;

    /// Accumulator flags special purpose register pair.
    RegisterPair<uint16_t, uint8_t> af;

    /// General purpose register pair.
    RegisterPair<uint16_t, uint8_t> bc;

    /// General purpose register pair.
    RegisterPair<uint16_t, uint8_t> de;

    /// Memory address reference register pair.
    RegisterPair<uint16_t, uint8_t> hl;

    /// Stack pointer register.
    Register<uint16_t> sp;

    /// Program counter register.
    Register<uint16_t> pc;

    std::array<Register<uint8_t>*, 8> r8 = { &b, &c, &d, &e, &h, &l, &a };
};

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

/// @brief Implementation of SM83 CPU for GameBoy SoC.
///
/// The brain and heart of the GameBoy SoC. This CPU has never been given a public name by either
/// Sharp or Nintendo, but datasheets and databooks identify it as a __Sharp SM83__ CPU core [1].
/// The SM83 is an 8-bit CPU with a 16-bit address bus [1]. It utilizes a unique instruction set
/// that takes pieces from the Intel 8080 and Zilog Z80 CPU architectures. However, it resembles
/// the Zilog Z80 a little more. Despite the similarities, the SM83 is its own unique architecture
/// custom built by Sharp [1].
///
/// [1]: https://gekkio.fi/files/gb-docs/gbctr.pdf
class Sm83 final {
public:
    /// @brief Construct new SM83 CPU instance.
    ///
    /// Requires logger to log internal state for debugging, and memory bus to
    /// send and receive computations from its instruction set.
    ///
    /// @return New SM83 CPU instance.
    Sm83(std::shared_ptr<spdlog::logger> logger, MemoryBus& bus);

    void execute();

private:
    void ld_r_r(uint8_t opcode);

    /// Logger to log internal state for debugging.
    std::shared_ptr<spdlog::logger> m_logger;

    /// Register file for CPU.
    Sm83RegisterFile m_reg;

    /// Memory bus to send and receive computations from instruction set.
    MemoryBus& m_bus;
};
}  // namespace cocoboy

#endif  // COCOBOY_SOC_CPU_HPP
