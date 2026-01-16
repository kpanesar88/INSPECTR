#include "storage.hpp"
#include <windows.h>

static std::string driveTypeToString(UINT type) {
    switch (type) {
        case DRIVE_FIXED:     return "Fixed";
        case DRIVE_REMOVABLE: return "Removable";
        case DRIVE_REMOTE:    return "Network";
        case DRIVE_CDROM:     return "CD-ROM";
        default:              return "Unknown";
    }
}

std::vector<StorageDevice> getStorageDevices() {
    std::vector<StorageDevice> devices;

    DWORD mask = GetLogicalDrives();
    if (mask == 0) {
        return devices;
    }

    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        if (!(mask & (1 << (letter - 'A')))) {
            continue;
        }

        std::string root;
        root += letter;
        root += ":\\";
        
        UINT type = GetDriveTypeA(root.c_str());
        if (type == DRIVE_UNKNOWN || type == DRIVE_NO_ROOT_DIR) {
            continue;
        }

        ULARGE_INTEGER freeBytes{}, totalBytes{}, freeAvail{};
        if (!GetDiskFreeSpaceExA(
                root.c_str(),
                &freeAvail,
                &totalBytes,
                &freeBytes)) {
            continue;
        }

        StorageDevice dev{};
        dev.drive = root;
        dev.type  = driveTypeToString(type);

        // Volume label
        char volumeName[MAX_PATH] = {};
        char fsName[MAX_PATH] = {};

        if (GetVolumeInformationA(
                root.c_str(),
                volumeName,
                MAX_PATH,
                nullptr,
                nullptr,
                nullptr,
                fsName,
                MAX_PATH)) {
            dev.label = volumeName[0] ? volumeName : "No Label";
        } else {
            dev.label = "Unknown";
        }

        dev.total_bytes = totalBytes.QuadPart;
        dev.free_bytes  = freeBytes.QuadPart;
        dev.used_bytes  = dev.total_bytes - dev.free_bytes;

        if (dev.total_bytes > 0) {
            dev.usage_percent =
                (static_cast<double>(dev.used_bytes) /
                 static_cast<double>(dev.total_bytes)) * 100.0;
        } else {
            dev.usage_percent = 0.0;
        }

        devices.push_back(dev);
    }

    return devices;
}
