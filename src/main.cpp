#include <iostream>
#include <iomanip>

#include <monitor/cpu.hpp>
#include <monitor/memory.hpp>
#include <monitor/systeminfo.hpp>

int main() {

    // Snapshot APIs (v1.1)
    CpuInfo cpu     = getCpuInfo();
    MemoryInfo mem  = getMemoryInfo();
    SystemInfo sys  = getSystemInfo();

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "=== SYSTEM BUDDY ===\n";

    // ---------------- CPU ----------------
    std::cout << "\n---------- CPU ----------\n";
    std::cout << "Name            : " << cpu.name << "\n";
    std::cout << "Usage           : " << cpu.usage_percent << " %\n";
    std::cout << "Base Frequency  : " << cpu.base_freq_ghz << " GHz\n";
    std::cout << "Cores           : " << cpu.cores << "\n";
    std::cout << "Threads         : " << cpu.threads << "\n";

    // ---------------- MEMORY ----------------
    double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    std::cout << "\n---------- MEMORY ----------\n";
    std::cout << "Total           : " << totalMemGB << " GB\n";
    std::cout << "Used            : " << usedMemGB << " GB\n";
    std::cout << "Usage           : " << mem.usage_percent << " %\n";

    // ---------------- SYSTEM ----------------
    std::cout << "\n---------- SYSTEM ----------\n";
    std::cout << "OS              : " << sys.os << "\n";
    std::cout << "Uptime          : " << sys.uptime << "\n";

    return 0;
}
