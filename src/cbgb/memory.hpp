// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

//! @brief GameBoy SoC memory manipulation and management.
//!
//! This area of CocoBoy's codebase is responsible for providing API's,
//! general utilities, and abstractions that are needed for emulating the
//! GameBoy's memory model.
//!
//! ## Memory Map
//!
//! The original GameBoy hardware utilizes a 16-bit address bus, and an 8-bit
//! data bus \[[1]\]. This means that the GameBoy only has access to 16 KiB of
//! addressable memory to work with. Despite this small amount of memory, all
//! peripherals apart of the GameBoy SoC are memory mapped to the memory bus.
//! Plus, certain regions of the GameBoy memory map are either only used for
//! ROM or RAM. Here is a basic overview of what the memory map looks like:
//!
//! | Start  | End   | Description               | Notes                                         |
//! | ------ | ----- | ------------------------- | --------------------------------------------- |
//! | $0000  | $3FFF | 16 KiB ROM bank 00        | From cartridge, usually a fixed bank.         |
//! | $4000  | $7FFF | 16 KiB ROM bank 00 - NN   | From cartridge, switchable bank.              |
//! | $8000  | $9FFF | 8 KiB Video RAM (VRAM)    | In CGB mode, switchable bank 0/1.             |
//! | $A000  | $BFFF | 8 KiB External RAM        | From cartidge, switchable bank if any.        |
//! | $C000  | $CFFF | 4 KiB Work RAM            | Always available                              |
//! | $D000  | $DFFF | 4 KiB Work RAM            | In CGB mode, switchable bank 1-7.             |
//! | $E000  | $FDFF | Echo RAM                  | Nintendo says use of this area is prohibited. |
//! | $FE00  | $FE9F | Object attribute memory   | Important memory segment for PPU.             |
//! | $FEA0  | $FEFF | Not usable                | Nintendo says use of this area is prohibited. |
//! | $FF00  | $FF7F | I/O registers             | Peripheral communication.                     |
//! | $FF80  | $FFFE | High RAM (HRAM)           | Simliar to page 0 on 6502.                    |
//! | $FFFF  | $FFFF | Interupt Enable register. | Control interupts for peripherals.            |
//!
//! ## Memory Banking
//!
//! The GameBoy uses memory banking to address the issue of limited memory for
//! ROM and RAM. Certain address ranges allow for GameBoy cartridges to swap out
//! their contents with a specificly sized memory bank through the use of a
//! _mapper_/_memory bank controller (MBC)_ \[[2]\]. These mappers also come
//! with special registers that a programmer can write data into to activate
//! the bank swapping at any time. Nintendo specifies a number of different
//! MBCs which can be idenfied by a byte at address $0147 \[[2]\].
//!
//! [1]: https://gbdev.io/pandocs/Memory_Map.html
//! [2]: https://gbdev.io/pandocs/MBCs.html#mbcs

#ifndef CBGB_MEMORY_HPP
#define CBGB_MEMORY_HPP

#include <array>
#include <cstdint>
#include <limits>
#include <memory>

#include <spdlog/spdlog.h>

namespace cbgb {
/// @brief Shared physical system memory.
///
/// This type emulates the behaviour of the GameBoy memory bus, and is meant
/// to be a shared resource between peripheral implemenations for the GameBoy
/// SoC.
class MemoryBus final {
public:
    explicit MemoryBus(std::shared_ptr<spdlog::logger> logger);
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::array<uint8_t, std::numeric_limits<uint16_t>::max()> m_ram;
};

/// @brief Hardware register.
///
/// Represents a register either meant to be controlled by the SM83 CPU, or a
/// peripheral on the GameBoy SoC. Recommended to to use types with explicit bit
/// sizes.
template <typename T>
struct Register final {
    T value;

    explicit Register(T initial)
        : value(initial)
    {
    }

    Register& operator=(T data)
    {
        value = data;
        return *this;
    }

    operator T() const
    {
        return static_cast<T>(value);
    }

    Register& operator++()
    {
        return *this = *this + 1;
    }

    Register& operator--()
    {
        return *this = *this - 1;
    }

    Register operator++(int)
    {
        Register temp(*this);
        *this = *this + 1;
        return temp;
    }

    Register operator--(int)
    {
        Register temp(*this);
        *this = *this - 1;
        return temp;
    }
};

/// @brief Control bits of target #Register.
///
/// Basic wrapper over #Register. Allows the caller to manipulate target bits
/// of a given #Register with relative ease. Should generally be used to control
/// target bits that either represent flags, or special controls for a
/// peripheral.
///
/// @invariant Target bit range must fall within accessable bits of register.
/// @invariant Target bit position is in range of accessable bits of register.
template <unsigned int position, unsigned int length, typename T = uint8_t>
struct RegisterBitField final {
    Register<T>& control;
    static constexpr T mask = (T(1) << length) - T(1);

    constexpr explicit RegisterBitField(Register<T>& target)
        : control(target)
    {
        constexpr unsigned int max_bits = std::numeric_limits<T>::digits;
        static_assert(length <= max_bits, "Bit range exceeds accessable bits of register");
        static_assert(position < max_bits, "Bit position exceeds accessable bits of register");
    }

    RegisterBitField& operator=(T value)
    {
        control
            = static_cast<T>((control & ~(mask << position)) | ((value & mask) << position));
        return *this;
    }

    operator T() const
    {
        return (control >> position) & mask;
    }
};

/// @brief Hardware register pair.
///
/// Uses two #Registers of the same type to form one larger register.
///
/// @invariant Bit length of P must be T * 2 bit length.
template <typename P, typename T>
struct RegisterPair final {
    Register<T>& high;
    Register<T>& low;
    static constexpr unsigned int shift = std::numeric_limits<P>::digits / 2;

    RegisterPair(Register<T>& target1, Register<T>& target2)
        : high(target1)
        , low(target2)
    {
        constexpr unsigned int max_bits = std::numeric_limits<P>::digits;
        constexpr unsigned int pair_bits = std::numeric_limits<T>::digits * 2;
        static_assert(
            max_bits == pair_bits,
            "invalid bit length, need r1 bit length + r2 bit length = rp bit length"
        );
    }

    RegisterPair& operator=(P data)
    {
        high = static_cast<T>(data >> shift);
        low = static_cast<T>(data);
        return *this;
    }

    operator P() const
    {
        return static_cast<P>((high << shift) | low);
    }

    RegisterPair& operator++()
    {
        return *this = static_cast<P>(*this) + 1;
    }

    RegisterPair& operator--()
    {
        return *this = static_cast<P>(*this) - 1;
    }

    P operator++(int)
    {
        P temp = static_cast<P>(*this);
        *this = static_cast<P>(*this) + 1;
        return temp;
    }

    P operator--(int)
    {
        P temp = static_cast<P>(*this);
        *this = static_cast<P>(*this) - 1;
        return temp;
    }
};
} // namespace cbgb

namespace fmt {
template <typename T>
struct formatter<cbgb::Register<T>> : formatter<T> {
    template <typename FormatContext>
    auto format(const cbgb::Register<T>& reg, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "0x{:X}", static_cast<T>(reg));
    }
};

template <unsigned int position, unsigned int length, typename T>
struct formatter<cbgb::RegisterBitField<position, length, T>> : formatter<T> {
    template <typename FormatContext>
    auto format(const cbgb::RegisterBitField<position, length, T>& reg, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "0x{:X}", static_cast<T>(reg));
    }
};

template <typename P, typename T>
struct formatter<cbgb::RegisterPair<P, T>> : formatter<T> {
    template <typename FormatContext>
    auto format(const cbgb::RegisterPair<P, T>& reg, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "0x{:X}", static_cast<P>(reg));
    }
};
} // namespace fmt

#endif // CBGB_MEMORY_HPP
