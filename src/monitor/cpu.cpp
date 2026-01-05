#include "cpu.hpp"

#include <windows.h>
#include <intrin.h>
#include <string>
#include <vector>
#include <cstdint>

// ==========================
// INTERNAL CPU TYPES
// ==========================

struct CpuTimes {
    ULONGLONG idle;
    ULONGLONG kernel;
    ULONGLONG user;
};

// ==========================
// INTERNAL HELPERS
// ==========================

static int popcount64(uint64_t value) {
    int count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}

// ==========================
// CPU USAGE (INTERNAL)
// ==========================

static CpuTimes getCpuTimes() {
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    auto toULL = [](FILETIME ft) {
        return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) |
               ft.dwLowDateTime;
    };

    return {
        toULL(idleTime),
        toULL(kernelTime),
        toULL(userTime)
    };
}

static double getCpuUsage(const CpuTimes& prev, const CpuTimes& curr) {

    if (curr.idle   < prev.idle ||
        curr.kernel < prev.kernel ||
        curr.user   < prev.user) {
        return 0.0;
    }

    ULONGLONG idleDiff = curr.idle - prev.idle;

    ULONGLONG totalDiff =
        (curr.kernel + curr.user) -
        (prev.kernel + prev.user);

    if (totalDiff == 0) {
        return 0.0;
    }

    double usage =
        1.0 - (static_cast<double>(idleDiff) /
               static_cast<double>(totalDiff));

    return usage * 100.0;
}

// ==========================
// CPU BASE FREQUENCY (GHz)
// ==========================

static double getBaseCpuFrequencyGHz() {
    HKEY hKey;
    DWORD mhz = 0;
    DWORD size = sizeof(DWORD);

    if (RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0,
            KEY_READ,
            &hKey) != ERROR_SUCCESS) {
        return 0.0;
    }

    RegQueryValueExA(hKey, "~MHz", nullptr, nullptr,
                     reinterpret_cast<LPBYTE>(&mhz), &size);
    RegCloseKey(hKey);

    return mhz / 1000.0;
}

// ==========================
// CPU NAME
// ==========================

static std::string getCpuName() {
    int cpuInfo[4] = {};
    char name[49] = {};

    __cpuid(cpuInfo, 0x80000000);
    unsigned int maxId = cpuInfo[0];

    if (maxId >= 0x80000004) {
        __cpuid(reinterpret_cast<int*>(name),      0x80000002);
        __cpuid(reinterpret_cast<int*>(name + 16), 0x80000003);
        __cpuid(reinterpret_cast<int*>(name + 32), 0x80000004);
    }

    return std::string(name);
}

// ==========================
// CPU CORES & THREADS
// ==========================

static void getCpuCoresAndThreads(int& cores, int& threads) {
    cores = 0;
    threads = 0;

    DWORD length = 0;
    GetLogicalProcessorInformationEx(
        RelationProcessorCore, nullptr, &length);

    std::vector<uint8_t> buffer(length);
    auto info =
        reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(
            buffer.data());

    if (!GetLogicalProcessorInformationEx(
            RelationProcessorCore, info, &length)) {
        return;
    }

    uint8_t* ptr = buffer.data();
    uint8_t* end = buffer.data() + length;

    while (ptr < end) {
        auto current =
            reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);

        if (current->Relationship == RelationProcessorCore) {
            cores++;
            threads += popcount64(
                static_cast<uint64_t>(
                    current->Processor.GroupMask[0].Mask
                )
            );
        }

        ptr += current->Size;
    }
}

// ==========================
// PUBLIC API (v1.1)
// ==========================

CpuInfo getCpuInfo() {
    CpuInfo info{};

    CpuTimes prev = getCpuTimes();
    Sleep(500);
    CpuTimes curr = getCpuTimes();

    info.usage_percent = getCpuUsage(prev, curr);
    info.base_freq_ghz = getBaseCpuFrequencyGHz();
    info.name          = getCpuName();

    getCpuCoresAndThreads(info.cores, info.threads);

    return info;
}
