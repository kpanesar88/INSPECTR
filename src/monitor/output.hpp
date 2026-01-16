#pragma once

#include <vector>
#include "cpu.hpp"
#include "memory.hpp"
#include "storage.hpp"
#include "systeminfo.hpp"

void outputJson(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storageDevices,
    const SystemInfo& sys
);

void outputCsv(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storageDevices
);
