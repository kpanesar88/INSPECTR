#include <iostream>
#include <windows.h>
#include <cstdint>
#include <iomanip>





#include <monitor/cpu.hpp>
#include <monitor/memory.hpp>
#include <monitor/systeminfo.hpp>



int main() {

    CpuTimes prev = getCpuTimes();
    Sleep(500);
    CpuTimes curr = getCpuTimes();

    double usage = getCpuUsage(prev, curr);
    double freq  = getBaseCpuFrequencyGHz();
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

    std::cout << std::fixed << std::setprecision(2);

    OSInfo os = getOSInfo();
    std::string uptime = getSystemUptime();

    std::cout << "\n----------CPU----------\n";
    std::cout << "CPU NAME: " << cpuName << "\n";
    std::cout << "CPU USAGE: " << usage << "%\n";
    std::cout << "BASE CPU FREQ (GHz): " << freq << "\n";
    std::cout << "CPU CORES: " << cores << "\n";
    std::cout << "CPU THREADS: " << threads << "\n";

    std::cout << "\n----------MEMORY----------\n";
    std::cout << "TOTAL MEMORY: " << totalMemGB << " GB\n";
    std::cout << "USED MEMORY: " << usedMemGB<< " GB\n";
    std::cout << "RAM USAGE %: " << ramUsagePercentage << "%\n";

    std::cout << "\n----------SYSTEM----------\n";
    std::cout << "OS: Windows " << os.version
          << " (Build " << os.build << ")\n";
    std::cout << "Uptime: " << uptime << "\n";




    return 0;
}
