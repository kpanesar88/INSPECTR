#pragma once
#include <string>
#include <windows.h>
#include <vector>

struct TopProcess {
    std::string name;
    DWORD pid;
    double cpu_percent; // valid for CPU list
    size_t mem_mb;      // valid for memory list
};

std::vector<TopProcess> getTopMemoryProcesses(size_t maxCount);