#include "systeminfo.hpp"

#include <windows.h>
#include <winternl.h>
#include <sstream>
#include <string>

// ==========================
// INTERNAL NT VERSION QUERY
// ==========================

typedef LONG (WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

// ==========================
// PUBLIC API (v1.1)
// ==========================

SystemInfo getSystemInfo() {
    SystemInfo info{};

    // -------- OS VERSION --------
    RTL_OSVERSIONINFOW osvi{};
    osvi.dwOSVersionInfoSize = sizeof(osvi);

    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        auto rtlGetVersion =
            reinterpret_cast<RtlGetVersionPtr>(
                GetProcAddress(hMod, "RtlGetVersion"));

        if (rtlGetVersion && rtlGetVersion(&osvi) == 0) {
            std::string version =
                (osvi.dwBuildNumber >= 22000) ? "11" : "10";

            info.os =
                "Windows " + version +
                " (Build " + std::to_string(osvi.dwBuildNumber) + ")";
        }
    }

    if (info.os.empty()) {
        info.os = "Windows (Unknown Version)";
    }

    // -------- SYSTEM UPTIME --------
    ULONGLONG uptimeMs = GetTickCount64();
    ULONGLONG totalSeconds = uptimeMs / 1000;

    ULONGLONG days    = totalSeconds / 86400;
    ULONGLONG hours   = (totalSeconds % 86400) / 3600;
    ULONGLONG minutes = (totalSeconds % 3600) / 60;

    std::ostringstream oss;
    oss << days << "d "
        << hours << "h "
        << minutes << "m";

    info.uptime = oss.str();

    return info;
}
