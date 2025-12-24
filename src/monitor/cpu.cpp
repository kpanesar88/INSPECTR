#include "cpu.hpp"

#include <windows.h>
#include <ntpoapi.h>   // 🔑 THIS IS THE KEY HEADER
#include <powrprof.h>

#pragma comment(lib, "PowrProf.lib")


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

    return 100.0 * (totalDiff - idleDiff) / totalDiff; //final cpu usage
}

double getCpuFrequencyGHZ(){
    PROCESSOR_POWER_INFORMATION info;


    CallNtPowerInformation(ProcessorInformation, nullptr, 0, &info, sizeof(info));

    return info.CurrentMhz / 1000.0;        //mhz to ghz conversion

}
