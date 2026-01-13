#include "storage.hpp"
#include <windows.h>

StorageInfo getStorageInfo() {
    StorageInfo info{};

    ULARGE_INTEGER freeBytesAvailable{};
    ULARGE_INTEGER totalBytes{};
    ULARGE_INTEGER freeBytes{};

    if (!GetDiskFreeSpaceExA(
            "C:\\",
            &freeBytesAvailable,
            &totalBytes,
            &freeBytes)) {
        return info;
    }

    info.total_bytes = totalBytes.QuadPart;
    info.free_bytes  = freeBytes.QuadPart;
    info.used_bytes  = info.total_bytes - info.free_bytes;

    if (info.total_bytes > 0) {
        info.usage_percent =
            (static_cast<double>(info.used_bytes) /
             static_cast<double>(info.total_bytes)) * 100.0;
    }

    return info;
}
