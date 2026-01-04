#pragma once
#include <string>

struct OSInfo {
    std::string name;
    std::string version;
    std::string build;
};

OSInfo getOSInfo();
std::string getSystemUptime();
