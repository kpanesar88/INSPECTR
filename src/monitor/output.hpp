#pragma once

#include <vector>
#include "cpu.hpp"
#include "memory.hpp"
#include "storage.hpp"
#include "systeminfo.hpp"
#include "process.hpp"

void outputJson(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storage,
    const SystemInfo& sys,
    const std::vector<TopProcess>& topMem,
    double avgCpu,
    double maxMem,
    int samples,
    int intervalMs
);

void outputCsv(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storage,
    const std::vector<TopProcess>& topMem,
    double avgCpu,
    double maxMem,
    int samples,
    int intervalMs
);
