#pragma once
#include <string>

struct GpuInfo {
    std::string name;
    double memory_used_mb;
    double memory_total_mb;

    // GPU usage is NOT reliable yet
    bool usage_supported = false;
    double usage_percent = 0.0;
};

GpuInfo getGpuInfo();