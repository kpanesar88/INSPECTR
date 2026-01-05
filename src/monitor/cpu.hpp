#pragma once
#include <string>

struct CpuInfo {
    std::string name;
    double usage_percent;
    double base_freq_ghz;
    int cores;
    int threads;
};

CpuInfo getCpuInfo();
