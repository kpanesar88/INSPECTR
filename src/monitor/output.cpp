#include "output.hpp"
#include "utils.hpp"

#include <iostream>
#include <iomanip>

// ==========================
// JSON OUTPUT
// ==========================

void outputJson(
    const CpuInfo& cpu,
    const MemoryInfo& mem,
    const std::vector<StorageDevice>& storageDevices,
    const SystemInfo& sys,
    const std::vector<TopProcess>& topMem,
    double avgCpu,
    double maxMem,
    int samples,
    int intervalMs
) {
    double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    std::cout << "{\n";
    std::cout << "  \"timestamp\": \"" << getIsoTimestamp() << "\",\n";
    std::cout << "  \"hostname\": \"" << getHostname() << "\",\n";

    // CPU
    std::cout << "  \"cpu\": {\n";
    std::cout << "    \"name\": \"" << cpu.name << "\",\n";
    std::cout << "    \"usage_percent\": " << cpu.usage_percent << ",\n";
    std::cout << "    \"cores\": " << cpu.cores << ",\n";
    std::cout << "    \"threads\": " << cpu.threads << "\n";
    std::cout << "  },\n";

    // MEMORY
    std::cout << "  \"memory\": {\n";
    std::cout << "    \"total_gb\": " << totalMemGB << ",\n";
    std::cout << "    \"used_gb\": " << usedMemGB << ",\n";
    std::cout << "    \"usage_percent\": " << mem.usage_percent << "\n";
    std::cout << "  },\n";

    // STORAGE
    std::cout << "  \"storage\": [\n";
    for (size_t i = 0; i < storageDevices.size(); ++i) {
        const auto& dev = storageDevices[i];

        double totalGB = dev.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedGB  = dev.used_bytes  / (1024.0 * 1024.0 * 1024.0);

        std::cout << "    {\n";
        std::cout << "      \"drive\": \"" << dev.drive << "\",\n";
        std::cout << "      \"label\": \"" << dev.label << "\",\n";
        std::cout << "      \"type\": \"" << dev.type << "\",\n";
        std::cout << "      \"total_gb\": " << totalGB << ",\n";
        std::cout << "      \"used_gb\": " << usedGB << ",\n";
        std::cout << "      \"usage_percent\": " << dev.usage_percent << "\n";
        std::cout << "    }";
        if (i + 1 < storageDevices.size()) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "  ],\n";

    // SAMPLING
    std::cout << "  \"sampling\": {\n";
    std::cout << "    \"samples\": " << samples << ",\n";
    std::cout << "    \"interval_ms\": " << intervalMs << ",\n";
    std::cout << "    \"avg_cpu_percent\": " << avgCpu << ",\n";
    std::cout << "    \"max_mem_percent\": " << maxMem << "\n";
    std::cout << "  },\n";

    // TOP MEMORY PROCESSES
    std::cout << "  \"top_memory_processes\": [\n";
    for (size_t i = 0; i < topMem.size(); ++i) {
        const auto& p = topMem[i];
        std::cout << "    {\n";
        std::cout << "      \"name\": \"" << p.name << "\",\n";
        std::cout << "      \"pid\": " << p.pid << ",\n";
        std::cout << "      \"used_mb\": " << p.mem_mb << "\n";
        std::cout << "    }";
        if (i + 1 < topMem.size()) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "  ],\n";

    // SYSTEM
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
    const std::vector<StorageDevice>& storageDevices,
    const std::vector<TopProcess>& topMem,
    double avgCpu,
    double maxMem,
    int samples,
    int intervalMs
) {
      (void)storageDevices;
      
    double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
    double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

    // Header
    std::cout
        << "timestamp,hostname,"
        << "cpu_usage,cpu_cores,cpu_threads,"
        << "mem_used_gb,mem_total_gb,"
        << "avg_cpu,max_mem,"
        << "samples,interval_ms";

    for (size_t i = 0; i < topMem.size(); ++i) {
        std::cout
            << ",topmem" << (i + 1) << "_name"
            << ",topmem" << (i + 1) << "_pid"
            << ",topmem" << (i + 1) << "_mb";
    }

    std::cout << "\n";

    // Row
    std::cout
        << getIsoTimestamp() << ","
        << getHostname() << ","
        << cpu.usage_percent << ","
        << cpu.cores << ","
        << cpu.threads << ","
        << usedMemGB << ","
        << totalMemGB << ","
        << avgCpu << ","
        << maxMem << ","
        << samples << ","
        << intervalMs;

    for (const auto& p : topMem) {
        std::cout << ","
                  << p.name << ","
                  << p.pid << ","
                  << p.mem_mb;
    }

    std::cout << "\n";
}
