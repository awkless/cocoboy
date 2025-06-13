// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#ifndef CBGB_CPU_HPP
#define CBGB_CPU_HPP

#include <array>
#include <cstdint>
#include <limits>
#include <memory>

#include <fmt/format.h>
#include <spdlog/logger.h>

#include "cbgb/memory.hpp"

namespace cbgb::cpu {
/// @brief SM83 register file representation.
struct RegisterFile final {
    /// @brief Construct new register file.
    ///
    /// @return New instance of register file.
    RegisterFile();

    /// Accumulator regsiter.
    cbgb::memory::Register<uint8_t> a;

    /// Flags register.
    cbgb::memory::Register<uint8_t> f;

    /// Zero flag.
    cbgb::memory::RegisterBit<7, 1, uint8_t> f_z;

    /// Subtraction flag (BCD).
    cbgb::memory::RegisterBit<6, 1, uint8_t> f_n;

    /// Half carry flag (BCD).
    cbgb::memory::RegisterBit<5, 1, uint8_t> f_h;

    /// Carry flag.
    cbgb::memory::RegisterBit<4, 1, uint8_t> f_c;

    /// General purpse register.
    cbgb::memory::Register<uint8_t> b;

    /// General purpse register.
    cbgb::memory::Register<uint8_t> c;

    /// General purpse register.
    cbgb::memory::Register<uint8_t> d;

    /// General purpse register.
    cbgb::memory::Register<uint8_t> e;

    /// High byte of memory address.
    cbgb::memory::Register<uint8_t> h;

    /// Low byte of memory address.
    cbgb::memory::Register<uint8_t> l;

    /// Accumulator flags special purpose register pair.
    cbgb::memory::RegisterPair<uint16_t, uint8_t> af;

    /// General purpose register pair.
    cbgb::memory::RegisterPair<uint16_t, uint8_t> bc;

    /// General purpose register pair.
    cbgb::memory::RegisterPair<uint16_t, uint8_t> de;

    /// Memory address reference register pair.
    cbgb::memory::RegisterPair<uint16_t, uint8_t> hl;

    /// Stack pointer register.
    cbgb::memory::Register<uint16_t> sp;

    /// Program counter register.
    cbgb::memory::Register<uint16_t> pc;

    std::array<cbgb::memory::Register<uint8_t>*, 8> r8 = { &b, &c, &d, &e, &h, &l, &f, &a };
};

enum OpcodeKind : uint8_t {
    // Misc.
    NOP = 0x00,
    HALT = 0x76,
    STOP = 0x10,

    // LD R, R'
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

    // LD R, N
    LD_B_N = 0x06,
    LD_C_N = 0x0E,
    LD_D_N = 0x16,
    LD_E_N = 0x1E,
    LD_H_N = 0x26,
    LD_L_N = 0x2E,
    LD_A_N = 0x3E,

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

    // LD A, (BC)
    LD_A_BC = 0x0A,

    // LD A, (DE)
    LD_A_DE = 0x1A,

    // LD (BC), A
    LD_BC_A = 0x02,

    // LD (DE), A
    LD_DE_A = 0x12,

    // LD A, (NN)
    LD_A_NN = 0xFA,

    // LD (NN), A
    LD_NN_A = 0xEA,

    // LDH A, (C)
    LDH_A_C = 0xF2,

    // LDH (C), A
    LDH_C_A = 0xE2,

    // LDH A, (N)
    LDH_A_N = 0xF0,

    // LDH (N), A
    LDH_N_A = 0xE0,

    // LD A, (HL-)
    LD_A_HLM = 0x3A,

    // LD (HL-), A
    LD_HLM_A = 0x32,

    // LD A, (HL+)
    LD_A_HLP = 0x2A,

    // LD (HL+), A
    LD_HLP_A = 0x22,

    // LD rr, nn
    LD_BC_NN = 0x01,
    LD_DE_NN = 0x11,
    LD_HL_NN = 0x21,
    LD_SP_NN = 0x31,
};

class Opcode final {
public:
    /// @brief Construct new opcode runner instance.
    ///
    /// @param logger Logger to log internal state.
    /// @param reg Register file to use.
    /// @param bus Memory bus to use.
    ///
    /// @return Instance of opcode runner.
    Opcode(std::shared_ptr<spdlog::logger> logger, RegisterFile& reg, cbgb::memory::MemoryBus& bus);

    /// @brief LD r, r': Load register (register).
    ///
    /// Load to 8-bit register __r__ with data from 8-bit register __r'__.
    ///
    /// - Opcode: 0b01xxxyyy
    /// - Length: 1 byte
    /// - Duration: 1m cycle
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_r_r(uint8_t opcode);

    /// @brief LD r, n: Load register (immediate).
    ///
    /// Load to 8-bit register __r__ with 8-bit immediate data __n__.
    ///
    /// - Opcode: 0b00xxx110
    /// - Length: 2 bytes
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_r_n(uint8_t opcode);

    /// @brief LD r, (HL): Load register (indirect HL).
    ///
    /// Load to 8-bit register __r__ with data from the absolute address specified by __HL__
    /// register.
    ///
    /// - Opcode: 0b01xxx110
    /// - Length: 1 bytes
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_r_hl(uint8_t opcode);

    /// @brief LD (HL), r: Load from register (indirect HL).
    ///
    /// Load to the absolute address specified by the 16-bit register __HL__ with data from 8-bit
    /// register __r__.
    ///
    /// - Opcode: 0b01110xxx
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_hl_r(uint8_t opcode);

    /// @brief LD (HL), n: Load from immediate data (indirect HL).
    ///
    /// Load to the absolute address specified by the 16-bit register __HL__ with immediate data
    /// __n__.
    ///
    /// - Opcode: 0b00110110
    /// - Length: 2 byte
    /// - Duration: 3m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_hl_n(uint8_t opcode);

    /// @brief LD A, (BC): Load accumulator (indirect BC).
    ///
    /// Load to 8-bit A register, data from absolute address specified by 16-bit register BC.
    ///
    /// - Opcode: 0b00001010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_a_bc(uint8_t opcode);

    /// @brief LD A, (DE): Load accumulator (indirect DE).
    ///
    /// Load to 8-bit A register, data from absolute address specified by 16-bit register DE.
    ///
    /// - Opcode: 0b00011010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_a_de(uint8_t opcode);

    /// @brief LD (BC), A: Load from accumulator (indirect BC).
    ///
    /// Load to the absolute address specified by 16-bit register BC, data from 8-bit A register.
    ///
    /// - Opcode: 0b00000010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_bc_a(uint8_t opcode);

    /// @brief LD (DE), A: Load from accumulator (indirect DE).
    ///
    /// Load to the absolute address specified by 16-bit register DE, data from 8-bit A register.
    ///
    /// - Opcode: 0b00010010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_de_a(uint8_t opcode);

    /// @brief LD A, (NN): Load accumulator (direct).
    ///
    /// Load to 8-bit A register, data from absolute address specified by 16-bit operand NN.
    ///
    /// - Opcode: 0b11111010
    /// - Length: 3 byte
    /// - Duration: 4m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_a_nn(uint8_t opcode);

    /// @brief LD (NN), A: Load from accumulator (direct).
    ///
    /// Load to absolute address specified by 16-bit operand NN, data from the 8-bit A register.
    ///
    /// - Opcode: 0b11101010
    /// - Length: 3 byte
    /// - Duration: 4m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_nn_a(uint8_t opcode);

    /// @brief LDH A, (C): Load accumulator (indirect 0xFF00 + C).
    ///
    /// Load to 8-bit A register, data from addrss specified by the 8-bit C register as an offset
    /// from address 0xFF00. Possible range is 0xFF00 - 0xFFFF inclusive.
    ///
    /// - Opcode: 0b11110010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ldh_a_c(uint8_t opcode);

    /// @brief LDH (C), A: Load from accumulator (indirect 0xFF00 + C).
    ///
    /// Load to address specified by the 8-bit C register as an offset of 0xFF00, data from 8-bit
    /// A register. Possible range is 0xFF00 - 0xFFFF inclusive.
    ///
    /// - Opcode: 0b11100010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ldh_c_a(uint8_t opcode);

    /// @brief LDH A, (N): Load accumulator (direct 0xFF00 + N).
    ///
    /// Load to 8-bit register A, data from the address specified by 8-bit immediate data N as an
    /// offset of 0xFF00. Possible range is 0xFF00 - 0xFFFF inclusive.
    ///
    /// - Opcode: 0b11110000
    /// - Length: 2 byte
    /// - Duration: 3m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ldh_a_n(uint8_t opcode);

    /// @brief LDH (N), A: Load from accumulator (direct 0xFF00 + N).
    ///
    /// Load to address specified by the 8-bit immediate data as an offset of 0xFF00, data from
    /// the 8-bit A register. Possible range is 0xFF00 - 0xFFFF inclusive.
    ///
    /// - Opcode: 0b11100000
    /// - Length: 2 byte
    /// - Duration: 3m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ldh_n_a(uint8_t opcode);

    /// @brief LD A, (HL-): Load accumulator (indirect HL, decrement).
    ///
    /// Load to 8-bit A register, data from the absolute addrss specified by the 16-bit register
    /// HL. The value of HL is decremented after the memory read.
    ///
    /// - Opcode: 0b00111010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_a_hlm(uint8_t opcode);

    /// @brief LD (HL-), A: Load from accumulator (indirect HL, decrement).
    ///
    /// Load to absolute address specified by 16-bit register HL, data form 8-bit A register.
    /// The value of HL is decremented after the memory write.
    ///
    /// - Opcode: 0b00110010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_hlm_a(uint8_t opcode);

    /// @brief LD A, (HL+): Load accumulator (indirect HL, increment).
    ///
    /// Load to 8-bit A register, data from the absolute address specified by the 16-bit regster
    /// HL. The value of HL is incremented after the memory read.
    ///
    /// - Opcode: 0b00101010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_a_hlp(uint8_t opcode);

    /// @brief LD (HL+), A: Load from accumulator (indirect HL, increment).
    ///
    /// Load to the absolute addrss specified by the 16-bit register HL, data from the 8-bit A
    /// register. The value of HL is decremented after the memory write.
    ///
    /// - Opcode: 0b00100010
    /// - Length: 1 byte
    /// - Duration: 2m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_hlp_a(uint8_t opcode);

    /// @brief LD rr, nn: Load 16-bit register pair.
    ///
    /// Load to 16-bit register __rr__ to the immediate 16-bit data __nn__.
    ///
    /// - Opcode: 0b00xx0001
    /// - Length: 3 byte
    /// - Duration: 3m cycles
    /// - Flags: None
    ///
    /// @param opcode Fetched and decoded opcode to process.
    void ld_rr_nn(uint8_t opcode);

private:
    /// Logger to log internal state for debugging.
    std::shared_ptr<spdlog::logger> m_logger;

    /// Register file for CPU.
    RegisterFile& m_reg;

    /// Memory bus to send and receive computations from instruction set.
    cbgb::memory::MemoryBus& m_bus;
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
    Sm83(std::shared_ptr<spdlog::logger> logger, cbgb::memory::MemoryBus& bus);

    /// @brief Execute instruction pointed to by PC register.
    ///
    /// Performs the fetch, decode, and execute cycle of the SM83 CPU for one instruction that the
    /// PC register is pointing to in memory.
    void step();

private:
    /// Logger to log internal state for debugging.
    std::shared_ptr<spdlog::logger> m_logger;

    /// Register file for CPU.
    RegisterFile m_reg;

    /// Memory bus to send and receive computations from instruction set.
    cbgb::memory::MemoryBus& m_bus;

    /// Opcode implementations to execute after decoding.
    Opcode m_run;
};
} // namespace cbgb::cpu

#endif // CBGB_CPU_HPP
