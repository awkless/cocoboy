// SPDX-FileCopyrightText: 2025 Jason Pena <jasonpena@awkless.com>
// SPDX-License-Identifier: MIT

#include "cocoboy/soc/memory.hpp"

#include <spdlog/spdlog.h>
#include <memory>

namespace cocoboy::soc {
MemoryBus::MemoryBus(std::shared_ptr<spdlog::logger> logger) : m_logger(logger)
{
    m_logger->trace("Construct new memory bus");
}

uint8_t MemoryBus::read_byte(uint16_t address)
{
    uint8_t byte = m_ram[address];
    m_logger->debug("Read 0x{0:02X} from address 0x{1:04X}", byte, address);
    return m_ram[address];
}

void MemoryBus::write_byte(uint16_t address, uint8_t byte)
{
    m_logger->debug("Write 0x{0:02X} to address 0x{1:04X}", byte, address);
    m_ram[address] = byte;
}
}  // namespace cocoboy::soc
