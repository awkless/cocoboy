// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cpu.hpp"
#include "memory.hpp"

#include <spdlog/logger.h>
#include <cstdint>
#include <memory>

namespace cocoboy {
Sm83RegisterFile::Sm83RegisterFile()
    : a(0x00),
      f(0x00),
      f_z(f),
      f_n(f),
      f_h(f),
      f_c(f),
      b(0x00),
      c(0x00),
      d(0x00),
      e(0x00),
      h(0x00),
      l(0x00),
      af(a, f),
      bc(b, c),
      de(d, e),
      hl(h, l),
      sp(0x0000),
      pc(0x0000)
{
}

Sm83::Sm83(std::shared_ptr<spdlog::logger> logger, MemoryBus& bus) : m_logger(logger), m_bus(bus)
{
    m_logger->trace("Construct new SM83 CPU");
}
}  // namespace cocoboy
