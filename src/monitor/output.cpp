#include "output.hpp"
#include <iostream>

// ==========================
// JSON OUTPUT
// ==========================

void outputJson(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const StorageInfo& storage,
    const SystemInfo& sys
) {
    double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    double totalDiskGB = storage.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedDiskGB  = storage.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    std::cout << "{\n";

    std::cout << "  \"cpu\": {\n";
    std::cout << "    \"name\": \"" << cpu.name << "\",\n";
    std::cout << "    \"usage_percent\": " << cpu.usage_percent << ",\n";
    std::cout << "    \"cores\": " << cpu.cores << ",\n";
    std::cout << "    \"threads\": " << cpu.threads << "\n";
    std::cout << "  },\n";

    std::cout << "  \"memory\": {\n";
    std::cout << "    \"total_gb\": " << totalMemGB << ",\n";
    std::cout << "    \"used_gb\": " << usedMemGB << ",\n";
    std::cout << "    \"usage_percent\": " << mem.usage_percent << "\n";
    std::cout << "  },\n";

    std::cout << "  \"storage\": {\n";
    std::cout << "    \"total_gb\": " << totalDiskGB << ",\n";
    std::cout << "    \"used_gb\": " << usedDiskGB << ",\n";
    std::cout << "    \"usage_percent\": " << storage.usage_percent << "\n";
    std::cout << "  },\n";

    std::cout << "  \"system\": {\n";
    std::cout << "    \"os\": \"" << sys.os << "\",\n";
    std::cout << "    \"uptime\": \"" << sys.uptime << "\"\n";
    std::cout << "  }\n";

    std::cout << "}\n";
}

// ==========================
// CSV OUTPUT
// ==========================

void outputCsv(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const StorageInfo& storage
) {
    double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    double totalDiskGB = storage.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedDiskGB  = storage.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    std::cout
        << "cpu_usage,"
        << "cpu_cores,"
        << "cpu_threads,"
        << "mem_used_gb,"
        << "mem_total_gb,"
        << "disk_used_gb,"
        << "disk_total_gb\n";

    std::cout
        << cpu.usage_percent << ","
        << cpu.cores << ","
        << cpu.threads << ","
        << usedMemGB << ","
        << totalMemGB << ","
        << usedDiskGB << ","
        << totalDiskGB << "\n";
}
