// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#ifndef COCOBOY_CPU_HPP
#define COCOBOY_CPU_HPP

#include "memory.hpp"

#include <fmt/format.h>
#include <spdlog/logger.h>
#include <cstdint>
#include <limits>
#include <memory>

namespace cocoboy {
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

private:
    /// Logger to log internal state for debugging.
    std::shared_ptr<spdlog::logger> m_logger;

    /// Register file for CPU.
    Sm83RegisterFile m_register;

    /// Memory bus to send and receive computations from instruction set.
    MemoryBus& m_bus;
};
}  // namespace cocoboy

#endif  // COCOBOY_CPU_HPP
