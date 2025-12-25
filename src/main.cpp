#include <iostream>
#include <windows.h>
#include <monitor/cpu.hpp>

int main() {

    CpuTimes prev = getCpuTimes();
    Sleep(100);
    CpuTimes curr = getCpuTimes();

    double usage = getCpuUsage(prev, curr);
    double freq  = getCpuFrequencyGHz();
    std::string cpu_name = getCpuName();  

    std::cout << "CPU NAME: " << cpu_name << "\n";
    std::cout << "CPU USAGE: " << usage << "%\n";
    std::cout << "CPU FREQ (GHz): " << freq << "\n";
}
