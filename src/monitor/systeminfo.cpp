#include "systeminfo.hpp"
#include <windows.h>
#include <sstream>

// ==========================
// OS INFO
// ==========================

#include <windows.h>
#include <winternl.h>

typedef LONG (WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

OSInfo getOSInfo() {
    OSInfo info;

    RTL_OSVERSIONINFOW osvi{};
    osvi.dwOSVersionInfoSize = sizeof(osvi);

    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        auto rtlGetVersion =
            (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");

        if (rtlGetVersion && rtlGetVersion(&osvi) == 0) {
            info.build = std::to_string(osvi.dwBuildNumber);
            info.name = "Windows";
            info.version =
                (osvi.dwBuildNumber >= 22000) ? "11" : "10";
            return info;
        }
    }

    // Fallback (should almost never hit)
    info.name = "Windows";
    info.version = "Unknown";
    info.build = "Unknown";
    return info;
}


// ==========================
// SYSTEM UPTIME
// ==========================

std::string getSystemUptime() {
    ULONGLONG uptimeMs = GetTickCount64();

    ULONGLONG totalSeconds = uptimeMs / 1000;
    ULONGLONG days    = totalSeconds / 86400;
    ULONGLONG hours   = (totalSeconds % 86400) / 3600;
    ULONGLONG minutes = (totalSeconds % 3600) / 60;

    std::ostringstream oss;
    oss << days << "d "
        << hours << "h "
        << minutes << "m";

    return oss.str();
}
