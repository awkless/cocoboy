// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#ifndef CBGB_CPU_HPP
#define CBGB_CPU_HPP

#include <array>
#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <string_view>

#include <spdlog/logger.h>

#include "cbgb/memory.hpp"

namespace cbgb {
class UndefinedOpcode final : public std::exception {
public:
    UndefinedOpcode(std::string what);

    const char* what() const noexcept;

private:
    std::string m_what;
};

struct Sm83State {
    Register<uint16_t> pc;
    Register<uint16_t> sp;
    RegisterPair<uint16_t, uint8_t> af;
    RegisterPair<uint16_t, uint8_t> bc;
    RegisterPair<uint16_t, uint8_t> de;
    RegisterPair<uint16_t, uint8_t> hl;
    Register<uint8_t> a;
    Register<uint8_t> f;
    Register<uint8_t> b;
    Register<uint8_t> c;
    Register<uint8_t> d;
    Register<uint8_t> e;
    Register<uint8_t> h;
    Register<uint8_t> l;
    RegisterBitField<7, 1, uint8_t> fz;
    RegisterBitField<6, 1, uint8_t> fn;
    RegisterBitField<5, 1, uint8_t> fh;
    RegisterBitField<4, 1, uint8_t> fc;
    MemoryBus& memory;
    Sm83State(MemoryBus& bus);
};

class Sm83 final {
public:
    Sm83(std::shared_ptr<spdlog::logger> log, MemoryBus& bus);
    void step();
    const Sm83State& get_state();
    unsigned int get_mcycle_count();

private:
    Sm83State m_state;
    unsigned int m_mcycles;
    std::shared_ptr<spdlog::logger> m_logger;
};
} // namespace cbgb

#endif // CBGB_CPU_HPP
