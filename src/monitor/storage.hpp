#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct StorageDevice {
    std::string drive;      // "C:\\"
    std::string label;      // "Windows", "USB_DISK"
    std::string type;       // Fixed, Removable, Network, etc.
    uint64_t total_bytes;
    uint64_t used_bytes;
    uint64_t free_bytes;
    double usage_percent;
};

std::vector<StorageDevice> getStorageDevices();
