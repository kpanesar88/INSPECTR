#include "process.hpp"
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <algorithm>

std::vector<TopProcess> getTopMemoryProcesses(size_t maxCount) {
    DWORD processes[1024], bytesReturned;
    EnumProcesses(processes, sizeof(processes), &bytesReturned);

    size_t count = bytesReturned / sizeof(DWORD);
    std::vector<TopProcess> result;

    for (size_t i = 0; i < count; ++i) {
        DWORD pid = processes[i];
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (!hProc) continue;

        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))) {
            char name[MAX_PATH] = "<unknown>";
            GetModuleBaseNameA(hProc, nullptr, name, sizeof(name));

            result.push_back({
                name,
                pid,
                0.0,
                pmc.WorkingSetSize / (1024 * 1024)
            });
        }
        CloseHandle(hProc);
    }

    std::sort(result.begin(), result.end(),
        [](const TopProcess& a, const TopProcess& b) {
            return a.mem_mb > b.mem_mb;
        });

    if (result.size() > maxCount)
        result.resize(maxCount);

    return result;
}
