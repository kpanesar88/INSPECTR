#include "cpu.hpp"
#include <windows.h>
#include <intrin.h>
#include <string>

CpuTimes getCpuTimes(){
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime,&kernelTime,&userTime);  // get system times

    auto toULL = [](FILETIME ft){   // convert to 64 (combining two 32 numbers inot one 64 bit number)
        return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    };


    return {
        toULL(idleTime), // return big idle number
        toULL(kernelTime), // return big kernel number
        toULL(userTime) // return big user number
    };
}


double getCpuUsage( const CpuTimes& prev, const CpuTimes& curr ){

    ULONGLONG idleDiff = curr.idle - prev.idle;   // calculate updated idle time
    ULONGLONG totalPrev = prev.kernel + prev.user;  // find total prev usage
    ULONGLONG totalCurr = curr.kernel + curr.user;  // find total current usage
    ULONGLONG totalDiff = totalCurr - totalPrev; // find difference

if (totalDiff == 0) return 0.0;
return 100.0 * (totalDiff - idleDiff) / totalDiff;
}

double getCpuFrequencyGHz() {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    // freq is ticks per second
    return freq.QuadPart / 1e9; // GHz-ish scale
}

std::string getCpuName(){
    int cpuInfo[4] = {};
    char name[49] = {};

    __cpuid(cpuInfo, 0x80000000);
    unsigned int maxId = cpuInfo[0];

    if (maxId >= 0x80000004) {
        __cpuid(reinterpret_cast<int*>(name),     0x80000002);
        __cpuid(reinterpret_cast<int*>(name + 16), 0x80000003);
        __cpuid(reinterpret_cast<int*>(name + 32), 0x80000004);
    }

     return std::string(name);
}