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
    int durationSeconds = -1; // NEW: -1 = infinite

    // ---------------- ARG PARSING ----------------
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--once") {
            runOnce = true;
        }
        else if (arg == "--interval" && i + 1 < argc) {
            int value = std::atoi(argv[i + 1]);
            if (value > 0) {
                refreshMs = value;
            }
            i++;
        }
        else if (arg == "--duration" && i + 1 < argc) { // NEW
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

    // NEW: start time for duration tracking
    auto startTime = std::chrono::steady_clock::now();

    do {

        // Quit with Q (live mode only)
        if (!runOnce && _kbhit()) {
            char ch = _getch();
            if (ch == 'q' || ch == 'Q') {
                break;
            }
        }

        system("cls");

        CpuInfo cpu           = getCpuInfo();
        MemoryInfo mem        = getMemoryInfo();
        StorageInfo storage   = getStorageInfo();
        SystemInfo sys        = getSystemInfo();

        std::cout << "=== SYSTEM BUDDY (v2.0) ===\n";

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

        // ---------------- STORAGE ----------------
        double totalDiskGB = storage.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedDiskGB  = storage.used_bytes  / (1024.0 * 1024.0 * 1024.0);
        double availableDiskGB = totalDiskGB - usedDiskGB;

        std::cout << "\n---------- STORAGE ----------\n";
        std::cout << "Total           : " << totalDiskGB << " GB\n";
        std::cout << "Used            : " << usedDiskGB << " GB\n";
        std::cout << "Available       : " << availableDiskGB << " GB\n";
        std::cout << "Usage           : " << storage.usage_percent << " %\n";

        // ---------------- SYSTEM ----------------
        std::cout << "\n---------- SYSTEM ----------\n";
        std::cout << "OS              : " << sys.os << "\n";
        std::cout << "Uptime          : " << sys.uptime << "\n";

        if (!runOnce) {
            std::cout << "\n---------- PRESS Q TO QUIT ----------\n";
        }

        // NEW: duration stop condition
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
