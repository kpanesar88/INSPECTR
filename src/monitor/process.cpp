#include "process.hpp"

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <vector>
#include <algorithm>
#include <string>

// ==========================
// Helpers
// ==========================

static double bytesToMB(SIZE_T b) {
    return b / (1024.0 * 1024.0);
}

// Convert WCHAR* → UTF-8 std::string
static std::string wideToUtf8(const wchar_t* wstr) {
    if (!wstr) return {};

    int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr,
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );

    std::string result(size - 1, '\0');

    WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr,
        -1,
        result.data(),
        size,
        nullptr,
        nullptr
    );

    return result;
}

// ==========================
// TOP MEMORY PROCESSES
// ==========================

std::vector<TopProcess> getTopMemoryProcesses(size_t limit) {
    std::vector<TopProcess> result;

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return result;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);

    if (Process32FirstW(snap, &pe)) {
        do {
            HANDLE hProc = OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE,
                pe.th32ProcessID
            );

            if (hProc) {
                PROCESS_MEMORY_COUNTERS pmc{};
                if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))) {
                    TopProcess p;
                    p.name = wideToUtf8(pe.szExeFile);
                    p.pid  = pe.th32ProcessID;
                    p.mem_mb = bytesToMB(pmc.WorkingSetSize);
                    p.cpu_percent = 0.0;
                    result.push_back(p);
                }
                CloseHandle(hProc);
            }
        } while (Process32NextW(snap, &pe));
    }

    CloseHandle(snap);

    std::sort(result.begin(), result.end(),
        [](const TopProcess& a, const TopProcess& b) {
            return a.mem_mb > b.mem_mb;
        });

    if (result.size() > limit)
        result.resize(limit);

    return result;
}

// ==========================
// TOP CPU PROCESSES (rough)
// ==========================

std::vector<TopProcess> getTopCpuProcesses(size_t limit) {
    std::vector<TopProcess> result;

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return result;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);

    if (Process32FirstW(snap, &pe)) {
        do {
            HANDLE hProc = OpenProcess(
                PROCESS_QUERY_INFORMATION,
                FALSE,
                pe.th32ProcessID
            );

            if (hProc) {
                FILETIME ftCreate{}, ftExit{}, ftKernel{}, ftUser{};
                if (GetProcessTimes(hProc, &ftCreate, &ftExit, &ftKernel, &ftUser)) {

                    ULONGLONG kernel =
                        (static_cast<ULONGLONG>(ftKernel.dwHighDateTime) << 32) |
                        ftKernel.dwLowDateTime;

                    ULONGLONG user =
                        (static_cast<ULONGLONG>(ftUser.dwHighDateTime) << 32) |
                        ftUser.dwLowDateTime;

                    TopProcess p;
                    p.name = wideToUtf8(pe.szExeFile);
                    p.pid  = pe.th32ProcessID;
                    p.cpu_percent = (kernel + user) / 1e7; // seconds
                    p.mem_mb = 0.0;
                    result.push_back(p);
                }
                CloseHandle(hProc);
            }
        } while (Process32NextW(snap, &pe));
    }

    CloseHandle(snap);

    std::sort(result.begin(), result.end(),
        [](const TopProcess& a, const TopProcess& b) {
            return a.cpu_percent > b.cpu_percent;
        });

    if (result.size() > limit)
        result.resize(limit);

    return result;
}
