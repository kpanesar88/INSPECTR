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

// ---------- ANSI COLORS (very minimal) ----------
constexpr const char* CLR_RESET  = "\033[0m";
constexpr const char* CLR_TITLE  = "\033[36m"; // cyan
constexpr const char* CLR_HEADER = "\033[34m"; // blue
constexpr const char* CLR_LABEL  = "\033[37m"; // light gray

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

        CpuInfo cpu = getCpuInfo();
        MemoryInfo mem = getMemoryInfo();
        auto storageDevices = getStorageDevices();
        SystemInfo sys = getSystemInfo();

        // JSON / CSV MODE
        if (outputJsonFlag) {
            outputJson(cpu, mem, storageDevices, sys);
            break;
        }
        if (outputCsvFlag) {
            outputCsv(cpu, mem, storageDevices);
            break;
        }

        // ---------------- OUTPUT ----------------
        std::cout << "\n"
                  << CLR_TITLE << "=== SYSTEM BUDDY v3.2 ===" << CLR_RESET << "\n";

        // CPU
        std::cout << CLR_HEADER << "\n[ CPU ]" << CLR_RESET << "\n";
        std::cout << "Name     : " << cpu.name << "\n";
        std::cout << "Usage    : " << cpu.usage_percent << " %\n";
        std::cout << "Cores    : " << cpu.cores << "\n";
        std::cout << "Threads  : " << cpu.threads << "\n";
        std::cout << "Base Freq: " << cpu.base_freq_ghz << " GHz\n";

        // MEMORY
        double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

        std::cout << CLR_HEADER << "\n[ MEMORY ]" << CLR_RESET << "\n";
        std::cout << "Total    : " << totalMemGB << " GB\n";
        std::cout << "Used     : " << usedMemGB  << " GB\n";
        std::cout << "Usage    : " << mem.usage_percent << " %\n";

        // STORAGE
        std::cout << CLR_HEADER << "\n[ STORAGE ]" << CLR_RESET << "\n";

        for (const auto& dev : storageDevices) {
            double totalGB = dev.total_bytes / (1024.0 * 1024.0 * 1024.0);
            double usedGB  = dev.used_bytes  / (1024.0 * 1024.0 * 1024.0);

            std::cout << dev.drive
                      << " [" << dev.label << "] "
                      << "(" << dev.type << ")\n";
            std::cout << "  Used  : " << usedGB  << " / "
                      << totalGB << " GB\n";
            std::cout << "  Usage : " << dev.usage_percent << " %\n";
        }

        // SYSTEM
        std::cout << CLR_HEADER << "\n[ SYSTEM ]" << CLR_RESET << "\n";
        std::cout << "OS       : " << sys.os << "\n";
        std::cout << "Uptime   : " << sys.uptime << "\n";

        if (!runOnce) {
            std::cout << CLR_LABEL
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
