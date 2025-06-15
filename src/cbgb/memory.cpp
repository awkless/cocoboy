// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include <memory>

#include <spdlog/spdlog.h>

#include "cbgb/memory.hpp"

namespace cbgb {
MemoryBus::MemoryBus(std::shared_ptr<spdlog::logger> logger)
    : m_logger(logger)
{
    m_logger->trace("Construct new memory bus");
}

uint8_t MemoryBus::read(uint16_t address)
{
    uint8_t value = m_ram[address];
    m_logger->debug("Read {0:04X}: {1:02X}", address, value);
    return value;
}

void MemoryBus::write(uint16_t address, uint8_t value)
{
    m_logger->debug("Write {0:04X}: {1:02X}", address, value);
    m_ram[address] = value;
}
} // namespace cbgb
