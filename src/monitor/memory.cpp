#include "memory.hpp"
#include <windows.h>

uint64_t getTotalMemoryBytes() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (!GlobalMemoryStatusEx(&memStatus)) {
        return 0;
    }

    return memStatus.ullTotalPhys;
}

uint64_t getAvailableMemoryBytes() {
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (!GlobalMemoryStatusEx(&memStatus)) {
        return 0;
    }

    return memStatus.ullAvailPhys;
}

uint64_t getUsedMemoryBytes() {
    uint64_t total = getTotalMemoryBytes();
    uint64_t available = getAvailableMemoryBytes();

    if (total == 0 || available == 0) return 0;

    return total - available;
}
