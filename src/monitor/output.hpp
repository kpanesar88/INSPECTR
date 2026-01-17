#pragma once

#include <vector>
#include "cpu.hpp"
#include "memory.hpp"
#include "storage.hpp"
#include "systeminfo.hpp"
#include "process.hpp"
#include "gpu.hpp"

void outputJson(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storageDevices,
    const SystemInfo& sys,
    const std::vector<TopProcess>& topMem,
    const GpuInfo& gpu,
    double avgCpu,
    double maxMem,
    int samples,
    int intervalMs
);

void outputCsv(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storageDevices,
    const std::vector<TopProcess>& topMem,
    const GpuInfo& gpu,
    double avgCpu,
    double maxMem,
    int samples,
    int intervalMs
);
