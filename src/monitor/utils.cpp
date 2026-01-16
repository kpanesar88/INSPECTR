#include "utils.hpp"
#include <windows.h>
#include <sstream>
#include <iomanip>

std::string getIsoTimestamp() {
    SYSTEMTIME st;
    GetSystemTime(&st);

    std::ostringstream oss;
    oss << std::setfill('0')
        << st.wYear << "-"
        << std::setw(2) << st.wMonth << "-"
        << std::setw(2) << st.wDay << "T"
        << std::setw(2) << st.wHour << ":"
        << std::setw(2) << st.wMinute << ":"
        << std::setw(2) << st.wSecond << "Z";

    return oss.str();
}

std::string getHostname() {
    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(buffer);
    GetComputerNameA(buffer, &size);
    return buffer;
}
