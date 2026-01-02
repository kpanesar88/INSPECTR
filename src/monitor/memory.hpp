#pragma once
#include <cstdint>

struct MemoryInfo {
    uint64_t total_ram;
    uint64_t used_ram;
    uint64_t available_ram;
    double ramUsagePercent;
};

// Total physical RAM in bytes
uint64_t getTotalMemoryBytes();
uint64_t getAvailableMemoryBytes();
uint64_t getUsedMemoryBytes();