// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cbgb/memory.hpp"

#include <spdlog/spdlog.h>
#include <memory>

namespace cbgb::memory {
MemoryBus::MemoryBus(std::shared_ptr<spdlog::logger> logger) : m_logger(logger)
{
    m_logger->trace("Construct new memory bus");
}

const uint8_t& MemoryBus::operator[](uint16_t address) const
{
    m_logger->debug("Access address {0:04X}");
    return m_ram[address];
}

uint8_t& MemoryBus::operator[](uint16_t address)
{
    m_logger->debug("Access address {0:04X}", address);
    return m_ram[address];
}
}  // namespace cbgb::memory
