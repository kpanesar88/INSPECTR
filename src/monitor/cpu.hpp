#pragma once

double getCpuFrequencyGHZ();

struct CpuTimes {
    unsigned long long idle;
    unsigned long long kernel;
    unsigned long long user;
};

CpuTimes getCpuTimes();
double getCpuUsage(const CpuTimes& prev, const CpuTimes& curr);
