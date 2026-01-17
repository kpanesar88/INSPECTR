#include "gpu.hpp"

#include <windows.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#pragma comment(lib, "dxgi.lib")


using Microsoft::WRL::ComPtr;

GpuInfo getGpuInfo() {
    GpuInfo gpu{};
    gpu.usage_percent = 0.0;
    gpu.memory_used_mb = 0.0;
    gpu.memory_total_mb = 0.0;
    gpu.name = "Unknown GPU";

    ComPtr<IDXGIFactory6> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return gpu;

    ComPtr<IDXGIAdapter1> adapter;
    if (FAILED(factory->EnumAdapters1(0, &adapter)))
        return gpu;

    DXGI_ADAPTER_DESC1 desc{};
    adapter->GetDesc1(&desc);

    // Convert GPU name (wide → UTF-8)
    char name[128];
    WideCharToMultiByte(
        CP_UTF8,
        0,
        desc.Description,
        -1,
        name,
        sizeof(name),
        nullptr,
        nullptr
    );
    gpu.name = name;

    ComPtr<IDXGIAdapter3> adapter3;
    if (FAILED(adapter.As(&adapter3)))
        return gpu;

    DXGI_QUERY_VIDEO_MEMORY_INFO memInfo{};
    if (SUCCEEDED(adapter3->QueryVideoMemoryInfo(
            0,
            DXGI_MEMORY_SEGMENT_GROUP_LOCAL,
            &memInfo))) {

        gpu.memory_used_mb  = memInfo.CurrentUsage / (1024.0 * 1024.0);
        gpu.memory_total_mb = memInfo.Budget / (1024.0 * 1024.0);
    }

    // ⚠️ GPU usage % is NOT directly exposed
    // We approximate using memory pressure
    if (gpu.memory_total_mb > 0) {
        gpu.usage_percent =
            (gpu.memory_used_mb / gpu.memory_total_mb) * 100.0;
    }

    return gpu;
}
