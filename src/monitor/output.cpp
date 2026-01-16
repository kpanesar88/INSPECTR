#include "output.hpp"
#include <iostream>

// ==========================
// JSON OUTPUT (MULTI-DRIVE)
// ==========================

void outputJson(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storageDevices,
    const SystemInfo& sys
) {
    double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    std::cout << "{\n";

    // CPU
    std::cout << "  \"cpu\": {\n";
    std::cout << "    \"name\": \"" << cpu.name << "\",\n";
    std::cout << "    \"usage_percent\": " << cpu.usage_percent << ",\n";
    std::cout << "    \"cores\": " << cpu.cores << ",\n";
    std::cout << "    \"threads\": " << cpu.threads << "\n";
    std::cout << "  },\n";

    // Memory
    std::cout << "  \"memory\": {\n";
    std::cout << "    \"total_gb\": " << totalMemGB << ",\n";
    std::cout << "    \"used_gb\": " << usedMemGB << ",\n";
    std::cout << "    \"usage_percent\": " << mem.usage_percent << "\n";
    std::cout << "  },\n";

    // Storage (ARRAY)
    std::cout << "  \"storage\": [\n";

    for (size_t i = 0; i < storageDevices.size(); ++i) {
        const auto& dev = storageDevices[i];

        double totalGB = dev.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedGB  = dev.used_bytes  / (1024.0 * 1024.0 * 1024.0);

        std::cout << "    {\n";
        std::cout << "      \"drive\": \"" << dev.drive << "\",\n";
        std::cout << "      \"type\": \"" << dev.type << "\",\n";
        std::cout << "      \"total_gb\": " << totalGB << ",\n";
        std::cout << "      \"used_gb\": " << usedGB << ",\n";
        std::cout << "      \"usage_percent\": " << dev.usage_percent << "\n";
        std::cout << "    }";

        if (i + 1 < storageDevices.size()) {
            std::cout << ",";
        }
        std::cout << "\n";
    }

    std::cout << "  ],\n";

    // System
    std::cout << "  \"system\": {\n";
    std::cout << "    \"os\": \"" << sys.os << "\",\n";
    std::cout << "    \"uptime\": \"" << sys.uptime << "\"\n";
    std::cout << "  }\n";

    std::cout << "}\n";
}

// ==========================
// CSV OUTPUT (ONE ROW PER DRIVE)
// ==========================

void outputCsv(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storageDevices
) {
    double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    // Header
    std::cout
        << "cpu_usage,cpu_cores,cpu_threads,"
        << "mem_used_gb,mem_total_gb,"
        << "drive,drive_type,"
        << "disk_used_gb,disk_total_gb,disk_usage_percent\n";

    for (const auto& dev : storageDevices) {
        double totalGB = dev.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedGB  = dev.used_bytes  / (1024.0 * 1024.0 * 1024.0);

        std::cout
            << cpu.usage_percent << ","
            << cpu.cores << ","
            << cpu.threads << ","
            << usedMemGB << ","
            << totalMemGB << ","
            << dev.drive << ","
            << dev.type << ","
            << usedGB << ","
            << totalGB << ","
            << dev.usage_percent << "\n";
    }
}
