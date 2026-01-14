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

int main(int argc, char* argv[]) {

    int refreshMs = 1000;

    for (int i = 1; i < argc; ++i){
        std::string arg = argv[i];

        if (arg == "--interval" && i + 1 < argc){
            int value = std::atoi(argv[i+1]);

            if (value > 0){
                refreshMs = value;
            }

            i++;
        }
    }


    constexpr int REFRESH_MS = 1000;

    std::cout << std::fixed << std::setprecision(2);

    while (true) {

          if (_kbhit()) {
        char ch = _getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        }
         }

        system("cls");

        CpuInfo cpu     = getCpuInfo();
        MemoryInfo mem  = getMemoryInfo();
        SystemInfo sys  = getSystemInfo();
        StorageInfo storage = getStorageInfo();


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

        double totalDiskGB = storage.total_bytes / (1024.0 * 1024.0 * 1024.0);
        double usedDiskGB  = storage.used_bytes  / (1024.0 * 1024.0 * 1024.0);
        double availableDiskGB  = totalDiskGB - usedDiskGB;

        std::cout << "\n---------- STORAGE ----------\n";
        std::cout << "Total           : " << totalDiskGB << " GB\n";
        std::cout << "Used            : " << usedDiskGB << " GB\n";
        std::cout << "Available       : " << availableDiskGB << " GB\n";
        std::cout << "Usage           : " << storage.usage_percent << " %\n";


        // ---------------- SYSTEM ----------------
        std::cout << "\n---------- SYSTEM ----------\n";
        std::cout << "OS              : " << sys.os << "\n";
        std::cout << "Uptime          : " << sys.uptime << "\n";

        std::cout << "\n---------- PRESS Q TO QUIT ----------\n";


        std::this_thread::sleep_for(
            std::chrono::milliseconds(refreshMs)
        );
    }

    return 0;
}