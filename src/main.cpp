#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <vector>

#include <monitor/cpu.hpp>
#include <monitor/memory.hpp>
#include <monitor/systeminfo.hpp>
#include <monitor/storage.hpp>
#include <monitor/output.hpp>
#include <monitor/process.hpp>
#include <monitor/gpu.hpp>

// ---------- ANSI COLORS ----------
constexpr const char* CLR_RESET  = "\033[0m";
constexpr const char* CLR_BLUE   = "\033[34m";
constexpr const char* CLR_PINK   = "\033[95m";
constexpr const char* CLR_DIM    = "\033[90m";

int main(int argc, char* argv[]) {

    // Prevent stdio sync artifacts
    std::ios::sync_with_stdio(false);

    int refreshMs = 1000;
    bool runOnce = false;
    bool outputJsonFlag = false;
    bool outputCsvFlag  = false;

    // ---------------- ARG PARSING ----------------
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--once") runOnce = true;
        else if (arg == "--json") outputJsonFlag = true;
        else if (arg == "--csv")  outputCsvFlag  = true;
        else if (arg == "--interval" && i + 1 < argc) {
            int value = std::atoi(argv[i + 1]);
            if (value > 0) refreshMs = value;
            ++i;
        }
    }

    const bool interactive = !outputJsonFlag && !outputCsvFlag;

    std::cout << std::fixed << std::setprecision(2);

    // ---------------- COLLECT DATA (ONCE) ----------------
    CpuInfo cpu = getCpuInfo();
    MemoryInfo mem = getMemoryInfo();
    auto storageDevices = getStorageDevices();
    SystemInfo sys = getSystemInfo();
    GpuInfo gpu = getGpuInfo();
    auto topMem = getTopMemoryProcesses(5);

    // ---------------- JSON / CSV (EARLY EXIT) ----------------
    if (outputJsonFlag) {
        outputJson(
            cpu,
            mem,
            storageDevices,
            sys,
            topMem,
            gpu,
            cpu.usage_percent,
            mem.usage_percent,
            1,
            refreshMs
        );
        return 0;   // 🚫 NO UI, NO CLS
    }

    if (outputCsvFlag) {
        outputCsv(
            cpu,
            mem,
            storageDevices,
            topMem,
            gpu,
            cpu.usage_percent,
            mem.usage_percent,
            1,
            refreshMs
        );
        return 0;   // 🚫 NO UI, NO CLS
    }

    // ---------------- LIVE UI LOOP ----------------
    do {
        if (!runOnce && _kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q') break;
        }

        if (interactive) {
            system("cls");   // ✅ SAFE: never runs during export
        }

        // Refresh live stats
        cpu = getCpuInfo();
        mem = getMemoryInfo();
        storageDevices = getStorageDevices();
        sys = getSystemInfo();
        gpu = getGpuInfo();
        topMem = getTopMemoryProcesses(5);

        // ---------------- HEADER ----------------
        std::cout << CLR_BLUE
                  << "=== INSPECTR v4.1 ==="
                  << CLR_RESET << "\n";

        // ---------------- CPU ----------------
        std::cout << CLR_BLUE << "\n[ CPU ]" << CLR_RESET << "\n";
        std::cout << "Name     : " << CLR_PINK << cpu.name << CLR_RESET << "\n";
        std::cout << "Usage    : " << CLR_PINK << cpu.usage_percent << " %" << CLR_RESET << "\n";
        std::cout << "Cores    : " << CLR_PINK << cpu.cores << CLR_RESET << "\n";
        std::cout << "Threads  : " << CLR_PINK << cpu.threads << CLR_RESET << "\n";
        std::cout << "BaseFreq : " << CLR_PINK << cpu.base_freq_ghz << " GHz" << CLR_RESET << "\n";

        // ---------------- MEMORY ----------------
        double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

        std::cout << CLR_BLUE << "\n[ MEMORY ]" << CLR_RESET << "\n";
        std::cout << "Total    : " << CLR_PINK << totalMemGB << " GB" << CLR_RESET << "\n";
        std::cout << "Used     : " << CLR_PINK << usedMemGB << " GB" << CLR_RESET << "\n";
        std::cout << "Usage    : " << CLR_PINK << mem.usage_percent << " %" << CLR_RESET << "\n";

        // ---------------- GPU ----------------
        std::cout << CLR_BLUE << "\n[ GPU ]" << CLR_RESET << "\n";
        std::cout << "Name     : " << CLR_PINK << gpu.name << CLR_RESET << "\n";
        std::cout << "VRAM     : " << CLR_PINK
                  << gpu.memory_used_mb << " / "
                  << gpu.memory_total_mb << " MB"
                  << CLR_RESET << "\n";

        if (gpu.usage_supported) {
            std::cout << "Usage    : " << CLR_PINK
                      << gpu.usage_percent << " %"
                      << CLR_RESET << "\n";
        } else {
            std::cout << "Usage    : " << CLR_DIM
                      << "Not supported"
                      << CLR_RESET << "\n";
        }

        // ---------------- STORAGE ----------------
        std::cout << CLR_BLUE << "\n[ STORAGE ]" << CLR_RESET << "\n";
        for (const auto& dev : storageDevices) {
            double totalGB = dev.total_bytes / (1024.0 * 1024.0 * 1024.0);
            double usedGB  = dev.used_bytes  / (1024.0 * 1024.0 * 1024.0);

            std::cout << CLR_PINK << dev.drive << CLR_RESET
                      << " [" << dev.label << "] "
                      << "(" << dev.type << ")\n";

            std::cout << "  Used   : " << CLR_PINK
                      << usedGB << " / " << totalGB << " GB"
                      << CLR_RESET << "\n";
            std::cout << "  Usage  : " << CLR_PINK
                      << dev.usage_percent << " %"
                      << CLR_RESET << "\n";
        }

        // ---------------- TOP MEMORY PROCESSES ----------------
        std::cout << CLR_BLUE << "\n[ TOP MEMORY PROCESSES ]" << CLR_RESET << "\n";
        for (const auto& p : topMem) {
            std::cout << CLR_PINK << p.name << CLR_RESET
                      << " (PID " << p.pid << ") - "
                      << p.mem_mb << " MB\n";
        }

        // ---------------- SYSTEM ----------------
        std::cout << CLR_BLUE << "\n[ SYSTEM ]" << CLR_RESET << "\n";
        std::cout << "OS       : " << CLR_PINK << sys.os << CLR_RESET << "\n";
        std::cout << "Uptime   : " << CLR_PINK << sys.uptime << CLR_RESET << "\n";

        std::cout << CLR_DIM << "\nPress Q to quit" << CLR_RESET << "\n";

        std::this_thread::sleep_for(
            std::chrono::milliseconds(refreshMs)
        );

    } while (!runOnce);

    return 0;
}
