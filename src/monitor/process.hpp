#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct TopProcess {
    std::string name;
    uint32_t pid;
    double cpu_percent;
    double mem_mb;
};

// Returns top N by memory usage
std::vector<TopProcess> getTopMemoryProcesses(size_t limit);

// Returns top N by CPU usage
std::vector<TopProcess> getTopCpuProcesses(size_t limit);
