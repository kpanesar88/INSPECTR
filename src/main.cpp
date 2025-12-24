#include <iostream>
#include <windows.h>
#include <cpu.hpp>



int main (){

    CpuTimes prev = getCpuTimes();
    sleep(100);
    CpuTimes curr = getCpuTimes();


    double usage = getCpuUsage(prev, curr);
    std::cout << "CPU USAGE: " << usage << "%\n";

}