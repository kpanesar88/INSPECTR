#pragma once
#include <string>

struct CpuTimes {
    unsigned long long idle;
    unsigned long long kernel;
    unsigned long long user;
};

CpuTimes getCpuTimes();
double getCpuUsage(const CpuTimes& prev, const CpuTimes& curr);
double getCpuFrequencyGHz();

// ✅ ONE declaration, exactly this
std::string getCpuName();
