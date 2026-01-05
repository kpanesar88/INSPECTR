#pragma once
#include <string>

struct SystemInfo {
    std::string os;      // e.g. "Windows 11 (Build 22631)"
    std::string uptime; // e.g. "2d 5h 41m"
};

SystemInfo getSystemInfo();
