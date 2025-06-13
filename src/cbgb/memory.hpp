// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#ifndef CBGB_MEMORY_HPP
#define CBGB_MEMORY_HPP

#include <array>
#include <cstdint>
#include <limits>
#include <memory>

#include <spdlog/spdlog.h>

namespace cbgb::memory {
/// @brief Shared physical system memory.
///
/// Original GameBoy hardware utilizes a 16-bit address bus, and an 8-bit data
/// bus[1]. This means that the full memory bus is only 64 kilobytes large.
/// Despite operating on such a small amount of memory, Nintendo chose to employ
/// memory mapping in order to communicate with peripherals like the joypad,
/// PPU, MBCs, etc[1].
///
/// Thus, this type is meant to replicate the behaviour and quirks of the
/// GameBoy memory bus, and is meant to be a shared resource between peripheral
/// implemenations for the GameBoy SoC.
///
/// [1]: https://gbdev.io/pandocs/Memory_Map.html
class MemoryBus final {
public:
    /// @brief Construct new memory bus.
    ///
    /// @param logger Shared logger to document internal state.
    /// @return Newly constructed memory bus.
    explicit MemoryBus(std::shared_ptr<spdlog::logger> logger);

    /// @brief Immutable memory access.
    ///
    /// @param address Addess to access data from.
    /// @return Immutable data from target address.
    const uint8_t& operator[](uint16_t address) const;

    /// @brief Mutable memory access.
    ///
    /// @param addrss Address to access data from.
    /// @return Mutable data from target address.
    uint8_t& operator[](uint16_t address);

private:
    /// Internal logger used for logging important memory states.
    std::shared_ptr<spdlog::logger> m_logger;

    /// Full addressable random access memory representation.
    std::array<uint8_t, std::numeric_limits<uint16_t>::max()> m_ram;
};

/// @brief Register representation.
///
/// Represents a register either meant to be controlled by the SM83 CPU, or a peripheral on the
/// GameBoy SoC. Recommended to to use types with explicit bit sizes to make bit manipulation
/// more predictable.
template <typename T>
class Register final {
public:
    /// @brief Construct new register with initial state.
    ///
    /// @return New instance of register.
    explicit Register(T initial)
        : m_data(initial)
    {
    }

    Register& operator=(T data)
    {
        m_data = data;
        return *this;
    }

    operator T() const { return static_cast<T>(m_data); }

    Register& operator&=(T val)
    {
        m_data &= val;
        return *this;
    }

    Register& operator|=(T val)
    {
        m_data |= val;
        return *this;
    }

    Register& operator++() { return *this = *this + 1; }

    Register& operator--() { return *this = *this - 1; }

private:
    T m_data;
};

/// @brief Control bits of register.
template <unsigned int bitno, unsigned int nbits, typename T = uint8_t>
class RegisterBit final {
public:
    /// @brief Construct new register bit controller.
    explicit RegisterBit(Register<T>& reg)
        : m_reg(reg)
    {
    }

    /// @brief Assign value into bits of register.
    ///
    /// @return Return self with new value.
    RegisterBit& operator=(T val)
    {
        m_reg &= static_cast<T>(~(mask << bitno));
        m_reg |= static_cast<T>(((val & mask) << bitno));
        return *this;
    }

    /// @brief Implicitly convert bits of register into taraget type.
    ///
    /// @return Bits implicitly converted from register.
    operator T() const { return (m_reg >> bitno) & mask; }

private:
    static constexpr T mask = (T(1) << nbits) - T(1);
    Register<T>& m_reg;
};

/// @brief Register pair representation.
///
/// Uses two registers of the same type to form one larger register.
///
/// @invariant Bit length of P must be T * 2 bit length.
template <typename P, typename T>
class RegisterPair final {
public:
    /// @brief Construct new register pair through target register types.
    ///
    /// @return Newly constructed instance of register pair.
    RegisterPair(Register<T>& high, Register<T>& low)
        : m_high(high)
        , m_low(low)
    {
        constexpr unsigned int max_bits = std::numeric_limits<P>::digits;
        constexpr unsigned int pair_bits = std::numeric_limits<T>::digits * 2;
        static_assert(
            max_bits == pair_bits,
            "invalid bit length, need r1 bit length + r2 bit length = rp bit length"
        );
    }

    /// @brief Assign data into register pair.
    RegisterPair& operator=(P data)
    {
        m_high = static_cast<T>(data >> shift);
        m_low = static_cast<T>(data);
        return *this;
    }

    /// @brief Implicitly convert register pair to target type.
    ///
    /// @return Data of register pair as target type.
    operator P() const { return static_cast<P>((m_high << shift) | m_low); }

    /// @brief Prefix increment register pair.
    ///
    /// @return Prefix incremented self.
    RegisterPair& operator++()
    {
        P data = static_cast<P>(*this);
        return *this = ++data;
    }

    /// @brief Prefix decrement register pair.
    ///
    /// @return Prefix decremented self.
    RegisterPair& operator--()
    {
        P data = static_cast<P>(*this);
        return *this = --data;
    }

private:
    static constexpr unsigned int shift = std::numeric_limits<P>::digits / 2;
    Register<T>& m_high;
    Register<T>& m_low;
};
} // namespace cbgb::memory

namespace fmt {
template <typename T>
struct formatter<cbgb::memory::Register<T>> : formatter<T> {
    template <typename FormatContext>
    auto format(const cbgb::memory::Register<T>& reg, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "0x{:X}", static_cast<T>(reg));
    }
};

template <unsigned int bitno, unsigned int nbits, typename T>
struct formatter<cbgb::memory::RegisterBit<bitno, nbits, T>> : formatter<T> {
    template <typename FormatContext>
    auto format(const cbgb::memory::RegisterBit<bitno, nbits, T>& reg, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "0x{:X}", static_cast<T>(reg));
    }
};

template <typename P, typename T>
struct formatter<cbgb::memory::RegisterPair<P, T>> : formatter<T> {
    template <typename FormatContext>
    auto format(const cbgb::memory::RegisterPair<P, T>& reg, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "0x{:X}", static_cast<P>(reg));
    }
};
} // namespace fmt

#endif // CBGB_MEMORY_HPP
