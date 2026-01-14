#pragma once

#include <monitor/cpu.hpp>
#include <monitor/memory.hpp>
#include <monitor/storage.hpp>
#include <monitor/systeminfo.hpp>

void outputJson(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const StorageInfo& storage,
    const SystemInfo& sys
);

void outputCsv(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const StorageInfo& storage
);
