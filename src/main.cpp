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

int main(int argc, char* argv[]) {

    int refreshMs = 1000;
    bool runOnce = false;
    int durationSeconds = -1;

    bool outputJsonFlag = false;
    bool outputCsvFlag  = false;

    // ---------------- ARG PARSING ----------------
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--once") {
            runOnce = true;
        }
        else if (arg == "--json") {
            outputJsonFlag = true;
        }
        else if (arg == "--csv") {
            outputCsvFlag = true;
        }
        else if (arg == "--interval" && i + 1 < argc) {
            int value = std::atoi(argv[i + 1]);
            if (value > 0) {
                refreshMs = value;
            }
            i++;
        }
        else if (arg == "--duration" && i + 1 < argc) {
            int value = std::atoi(argv[i + 1]);
            if (value > 0) {
                durationSeconds = value;
            }
            i++;
        }
    }

    // If --once is set, ignore duration
    if (runOnce) {
        durationSeconds = -1;
    }

    std::cout << std::fixed << std::setprecision(2);

    auto startTime = std::chrono::steady_clock::now();

    do {

        // Quit with Q (live mode only)
        if (!runOnce && !outputJsonFlag && !outputCsvFlag && _kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q') {
                break;
            }
        }

        if (!outputJsonFlag && !outputCsvFlag) {
            system("cls");
        }

        CpuInfo cpu = getCpuInfo();
        MemoryInfo mem = getMemoryInfo();
        std::vector<StorageDevice> storageDevices = getStorageDevices();
        SystemInfo sys = getSystemInfo();

        // ---------------- JSON / CSV OUTPUT ----------------
        if (outputJsonFlag) {
            outputJson(cpu, mem, storageDevices, sys);
            break;
        }

        if (outputCsvFlag) {
            outputCsv(cpu, mem, storageDevices);
            break;
        }

        // ---------------- LIVE TERMINAL OUTPUT ----------------
        std::cout << "=== SYSTEM BUDDY (v3.2) ===\n";

        // CPU
        std::cout << "\n---------- CPU ----------\n";
        std::cout << "Name            : " << cpu.name << "\n";
        std::cout << "Usage           : " << cpu.usage_percent << " %\n";
        std::cout << "Base Frequency  : " << cpu.base_freq_ghz << " GHz\n";
        std::cout << "Cores           : " << cpu.cores << "\n";
        std::cout << "Threads         : " << cpu.threads << "\n";

        // MEMORY
        double totalMemGB = mem.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedMemGB  = mem.used_bytes  / (1024.0 * 1024.0 * 1024.0);

        std::cout << "\n---------- MEMORY ----------\n";
        std::cout << "Total           : " << totalMemGB << " GB\n";
        std::cout << "Used            : " << usedMemGB << " GB\n";
        std::cout << "Usage           : " << mem.usage_percent << " %\n";

        // STORAGE
        std::cout << "\n---------- STORAGE ----------\n";

        for (const auto& dev : storageDevices) {
            double totalGB = dev.total_bytes / (1024.0 * 1024.0 * 1024.0);
            double usedGB  = dev.used_bytes  / (1024.0 * 1024.0 * 1024.0);
            double freeGB  = dev.free_bytes  / (1024.0 * 1024.0 * 1024.0);

            std::cout << dev.drive
                      << " [" << dev.label << "] "
                      << "(" << dev.type << ")\n";

            std::cout << "  Total      : " << totalGB << " GB\n";
            std::cout << "  Used       : " << usedGB  << " GB\n";
            std::cout << "  Free       : " << freeGB  << " GB\n";
            std::cout << "  Usage      : " << dev.usage_percent << " %\n\n";
        }

        // SYSTEM
        std::cout << "\n---------- SYSTEM ----------\n";
        std::cout << "OS              : " << sys.os << "\n";
        std::cout << "Uptime          : " << sys.uptime << "\n";

        if (!runOnce) {
            std::cout << "\n---------- PRESS Q TO QUIT ----------\n";
        }

        // Duration stop condition
        if (durationSeconds > 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed =
                std::chrono::duration_cast<std::chrono::seconds>(now - startTime)
                    .count();

            if (elapsed >= durationSeconds) {
                break;
            }
        }

        if (runOnce) {
            break;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(refreshMs)
        );

    } while (true);

    return 0;
}
