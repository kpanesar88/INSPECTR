#pragma once
#include <cstdint>

struct StorageInfo {
    uint64_t total_bytes;
    uint64_t used_bytes;
    uint64_t free_bytes;
    double usage_percent;
};

StorageInfo getStorageInfo();
