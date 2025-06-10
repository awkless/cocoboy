// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#ifndef COCOBOY_MEMORY_HPP
#define COCOBOY_MEMORY_HPP

#include <spdlog/spdlog.h>
#include <array>
#include <cstdint>
#include <limits>
#include <memory>

namespace cocoboy {
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
    explicit MemoryBus(const std::shared_ptr<spdlog::logger>& logger);

    /// @brief Read byte from target address.
    ///
    /// @param address Valid 16-bit address.
    /// @return Current byte from target address.
    uint8_t read_byte(uint16_t address);

    /// @brief Write byte to target address.
    ///
    /// @param address Valid 16-bit address.
    /// @param byte Byte to write to target address.
    void write_byte(uint16_t address, uint8_t byte);

private:
    /// Internal logger used for logging important memory states.
    const std::shared_ptr<spdlog::logger>& m_logger;

    /// Full addressable random access memory representation.
    std::array<uint8_t, std::numeric_limits<uint16_t>::max()> m_ram;
};

/// @brief Representation of an 8-bit register.
class ByteRegister final {
public:
    /// @brief Construct new byte register from initial data.
    explicit ByteRegister(uint8_t initial);

    /// @brief Load contents of register through assignment operator.
    void operator=(uint8_t data);

    /// @brief Implicit conversion to `uint8_t`.
    ///
    /// @return Contents of register to `uint8_t` variable implicitly.
    operator uint8_t() const;

    /// @brief Prefix increment operation.
    ///
    /// @return Instance of self.
    ByteRegister& operator++();

    /// @brief Prefix decrement operation.
    ///
    /// @return Instance of self.
    ByteRegister& operator--();

private:
    /// Contents of byte register.
    uint8_t m_data;

    /// Custom formatter for byte register, automatically uses hexadecimal.
    friend struct fmt::formatter<ByteRegister>;
};
}  // namespace cocoboy

namespace fmt {
template <>
struct formatter<cocoboy::ByteRegister> : formatter<uint8_t> {
    template <typename FormatContext>
    auto format(const cocoboy::ByteRegister& reg, FormatContext& ctx) const
    {
        return format_to(ctx.out(), "0x{:02X}", static_cast<uint8_t>(reg));
    }
};
}  // namespace fmt

#endif  // COCOBOY_MEMORY_HPP
