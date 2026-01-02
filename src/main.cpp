#include <iostream>
#include <windows.h>
#include <cstdint>
#include <monitor/cpu.hpp>
#include <monitor/memory.hpp>

int main() {

    CpuTimes prev = getCpuTimes();
    Sleep(500);
    CpuTimes curr = getCpuTimes();

    double usage = getCpuUsage(prev, curr);
    double freq  = getCpuFrequencyGHz();
    std::string cpuName = getCpuName();

    int cores = 0;
    int threads = 0;
    getCpuCoresAndThreads(cores, threads);

    uint64_t totalMemBytes = getTotalMemoryBytes();
    double totalMemGB = totalMemBytes / (1024.0 * 1024.0 * 1024.0);

    uint64_t usedMemBytes = getUsedMemoryBytes();
    double usedMemGB = usedMemBytes / (1024.0 * 1024.0 * 1024.0);

    uint64_t availMemBytes = getAvailableMemoryBytes();
    double availMemGB = availMemBytes / (1024.0 * 1024.0 * 1024.0);

    double ramUsagePercentage = (usedMemGB/totalMemGB) * 100;


    std::cout << "CPU NAME: " << cpuName << "\n";
    std::cout << "CPU USAGE: " << usage << "%\n";
    std::cout << "CPU FREQ (GHz): " << freq << "\n";
    std::cout << "CPU CORES: " << cores << "\n";
    std::cout << "CPU THREADS: " << threads << "\n";
    std::cout << "TOTAL MEMORY: " << totalMemGB << " GB\n";
    std::cout << "USED MEMORY: " << usedMemGB<< " GB\n";
    std::cout << "USAGE %: " << ramUsagePercentage << " %\n";


    return 0;
}
