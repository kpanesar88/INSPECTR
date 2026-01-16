#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <conio.h>
#include <string>
#include <cstdlib>

#include <monitor/cpu.hpp>
#include <monitor/memory.hpp>
#include <monitor/systeminfo.hpp>
#include <monitor/storage.hpp>
#include <monitor/output.hpp>

// ---------- ANSI COLORS ----------
constexpr const char* CLR_RESET  = "\033[0m";
constexpr const char* CLR_BLUE   = "\033[34m";
constexpr const char* CLR_PINK   = "\033[95m";
constexpr const char* CLR_DIM    = "\033[90m";

int main(int argc, char* argv[]) {

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
            i++;
        }
    }

    std::cout << std::fixed << std::setprecision(2);

    do {
        if (!runOnce && !outputJsonFlag && !outputCsvFlag && _kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q') break;
        }

        system("cls");

        CpuInfo cpu = getCpuInfo();
        MemoryInfo mem = getMemoryInfo();
        auto storageDevices = getStorageDevices();
        SystemInfo sys = getSystemInfo();

        // ---------------- JSON / CSV ----------------
                if (outputJsonFlag) {
            std::vector<TopProcess> topMem;   // empty for now
            outputJson(
                cpu,
                mem,
                storageDevices,
                sys,
                topMem,
                cpu.usage_percent, // avgCpu (placeholder)
                mem.usage_percent, // maxMem (placeholder)
                1,                 // samples
                refreshMs
            );
            break;
        }

                if (outputCsvFlag) {
            std::vector<TopProcess> topMem;   // empty for now
            outputCsv(
                cpu,
                mem,
                storageDevices,
                topMem,
                cpu.usage_percent,
                mem.usage_percent,
                1,
                refreshMs
            );
            break;
        }


        // ---------------- HEADER ----------------
        std::cout << CLR_BLUE
                  << "=== SYSTEM BUDDY v4.0 ==="
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

        // ---------------- STORAGE ----------------
        std::cout << CLR_BLUE << "\n[ STORAGE ]" << CLR_RESET << "\n";

        for (const auto& dev : storageDevices) {
            double totalGB = dev.total_bytes / (1024.0 * 1024.0 * 1024.0);
            double usedGB  = dev.used_bytes  / (1024.0 * 1024.0 * 1024.0);

            std::cout << CLR_PINK << dev.drive << CLR_RESET
                      << " [" << dev.label << "] "
                      << "(" << dev.type << ")\n";

            std::cout << "  Used   : " << CLR_PINK << usedGB << " / "
                      << totalGB << " GB" << CLR_RESET << "\n";
            std::cout << "  Usage  : " << CLR_PINK << dev.usage_percent << " %" << CLR_RESET << "\n";
        }

        // ---------------- SYSTEM ----------------
        std::cout << CLR_BLUE << "\n[ SYSTEM ]" << CLR_RESET << "\n";
        std::cout << "OS       : " << CLR_PINK << sys.os << CLR_RESET << "\n";
        std::cout << "Uptime   : " << CLR_PINK << sys.uptime << CLR_RESET << "\n";

        if (!runOnce) {
            std::cout << CLR_DIM
                      << "\nPress Q to quit"
                      << CLR_RESET << "\n";
        }

        if (runOnce) break;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(refreshMs)
        );

    } while (true);

    return 0;
}
