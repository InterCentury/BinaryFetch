#include "GPUInfo.h"
#include <windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wbemidl.h>
#include <comdef.h>
#include <iostream>
#include <sstream>
#include "nvapi.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "nvapi64.lib")

using namespace std;

// ----------------------------------------------------
// Helper: convert wide string ? UTF-8 std::string
static std::string wstr_to_utf8(const std::wstring& w)
{
    if (w.empty()) return {};
    int sz = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string r(sz - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &r[0], sz, nullptr, nullptr);
    return r;
}

// ----------------------------------------------------
// Helper: query float values via WMI
static bool query_wmi_float(const wchar_t* wql, const wchar_t* field, float& outVal)
{
    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr)) return false;

    hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE, NULL);
    if (FAILED(hr) && hr != RPC_E_TOO_LATE)
    {
        CoUninitialize();
        return false;
    }

    IWbemLocator* locator = nullptr;
    IWbemServices* services = nullptr;
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&locator);
    if (FAILED(hr)) { CoUninitialize(); return false; }

    hr = locator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &services);
    locator->Release();
    if (FAILED(hr)) { CoUninitialize(); return false; }

    CoSetProxyBlanket(services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    IEnumWbemClassObject* enumerator = nullptr;
    hr = services->ExecQuery(bstr_t("WQL"), bstr_t(wql),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator);
    if (FAILED(hr)) { services->Release(); CoUninitialize(); return false; }

    IWbemClassObject* obj = nullptr;
    ULONG returned = 0;
    bool ok = false;
    while (enumerator && SUCCEEDED(enumerator->Next(WBEM_INFINITE, 1, &obj, &returned)) && returned)
    {
        VARIANT val;
        if (SUCCEEDED(obj->Get(field, 0, &val, 0, 0)) && (val.vt == VT_R8 || val.vt == VT_I4))
        {
            outVal = (val.vt == VT_R8) ? (float)val.dblVal : (float)val.intVal;
            ok = true;
            VariantClear(&val);
            obj->Release();
            break;
        }
        VariantClear(&val);
        obj->Release();
    }
    if (enumerator) enumerator->Release();
    services->Release();
    CoUninitialize();
    return ok;
}

// ----------------------------------------------------
// WMI-based GPU usage
float GPUInfo::get_gpu_usage()
{
    float val = 0.0f;
    query_wmi_float(
        L"SELECT UtilizationPercentage FROM Win32_PerfFormattedData_GPUPerformanceCounters_GPUEngine WHERE Name LIKE '%_3D%'",
        L"UtilizationPercentage",
        val);
    return val;
}

// ----------------------------------------------------
// WMI-based GPU temperature
float GPUInfo::get_gpu_temperature()
{
    float val = 0.0f;
    query_wmi_float(
        L"SELECT CurrentTemperature FROM MSAcpi_ThermalZoneTemperature",
        L"CurrentTemperature",
        val);
    if (val > 2732.0f)
        val = (val / 10.0f) - 273.15f; // Kelvin ? Celsius
    return val;
}

// ----------------------------------------------------
// Estimate core count
int GPUInfo::get_gpu_core_count()
{
    ID3D12Device* device = nullptr;
    IDXGIFactory4* factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return 0;

    IDXGIAdapter1* adapter = nullptr;
    int cores = 0;
    if (SUCCEEDED(factory->EnumAdapters1(0, &adapter)))
    {
        if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
        {
            cores = 2560; // Generic estimate
            device->Release();
        }
        adapter->Release();
    }
    factory->Release();
    return cores;
}

// ----------------------------------------------------
// NVAPI helpers
static bool nvapi_available()
{
    HMODULE nv = LoadLibraryA("nvapi64.dll");
    if (!nv) return false;
    FreeLibrary(nv);
    return true;
}

static bool is_nvidia_gpu(UINT vendorId)
{
    return (vendorId == 0x10DE); // NVIDIA vendor ID
}

// Modern + fallback NVAPI temperature getter
static float get_nvapi_temperature(NvPhysicalGpuHandle handle)
{
#ifdef NV_GPU_CLIENT_THERMAL_SENSOR_INFO_VER
    // --- Modern method for RTX GPUs ---
    NV_GPU_CLIENT_THERMAL_SENSOR_INFO sensorInfo = { 0 };
    sensorInfo.version = NV_GPU_CLIENT_THERMAL_SENSOR_INFO_VER;
    NV_GPU_CLIENT_THERMAL_SENSOR_STATUS status = { 0 };
    status.version = NV_GPU_CLIENT_THERMAL_SENSOR_STATUS_VER;
    NvU32 sensorCount = 0;

    if (NvAPI_GPU_ClientThermalSensorsGetInfo(handle, &sensorCount, &sensorInfo) == NVAPI_OK &&
        sensorCount > 0 &&
        NvAPI_GPU_ClientThermalSensorsGetStatus(handle, &status) == NVAPI_OK)
    {
        for (NvU32 i = 0; i < status.sensorCount; ++i)
        {
            if (status.sensor[i].sensorType == NVAPI_THERMAL_TARGET_GPU)
                return static_cast<float>(status.sensor[i].currentTemp);
        }
    }
#endif

    // --- Fallback for older GPUs ---
    NV_GPU_THERMAL_SETTINGS thermalSettings = { 0 };
    thermalSettings.version = NV_GPU_THERMAL_SETTINGS_VER;
    if (NvAPI_GPU_GetThermalSettings(handle, NVAPI_THERMAL_TARGET_GPU, &thermalSettings) == NVAPI_OK)
        return static_cast<float>(thermalSettings.sensor[0].currentTemp);

    return -1.0f;
}

static float get_nvapi_usage(NvPhysicalGpuHandle handle)
{
    NV_GPU_DYNAMIC_PSTATES_INFO_EX pStates = { 0 };
    pStates.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
    if (NvAPI_GPU_GetDynamicPstatesInfoEx(handle, &pStates) == NVAPI_OK)
        return static_cast<float>(pStates.utilization[0].percentage); // GPU Core usage
    return -1.0f;
}

static int get_nvapi_core_count(NvPhysicalGpuHandle handle)
{
    NvU32 count = 0;
    if (NvAPI_GPU_GetGpuCoreCount(handle, &count) == NVAPI_OK)
        return static_cast<int>(count);
    return 0;
}

// ----------------------------------------------------
// Main GPU info collector
std::vector<gpu_data> GPUInfo::get_all_gpu_info()
{
    std::vector<gpu_data> list;
    IDXGIFactory6* factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return list;

    IDXGIAdapter4* adapter = nullptr;
    for (UINT i = 0; factory->EnumAdapters1(i, (IDXGIAdapter1**)&adapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC3 desc{};
        if (FAILED(adapter->GetDesc3(&desc)))
        {
            adapter->Release();
            continue;
        }

        gpu_data d;
        d.gpu_name = wstr_to_utf8(desc.Description);

        double memGB = static_cast<double>(desc.DedicatedVideoMemory) / (1024.0 * 1024.0 * 1024.0);
        std::ostringstream memStream;
        memStream.precision(1);
        memStream << fixed << memGB;
        d.gpu_memory = memStream.str() + " GB";

        // Driver version
        LARGE_INTEGER driverVersion{};
        if (SUCCEEDED(adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion)))
        {
            unsigned int part1 = HIWORD(driverVersion.HighPart);
            unsigned int part2 = LOWORD(driverVersion.HighPart);
            unsigned int part3 = HIWORD(driverVersion.LowPart);
            unsigned int part4 = LOWORD(driverVersion.LowPart);

            std::ostringstream oss;
            oss << part1 << "." << part2 << "." << part3 << "." << part4;
            d.gpu_driver_version = oss.str();
        }
        else d.gpu_driver_version = "Unknown";

        // Vendor
        d.gpu_vendor = (desc.VendorId == 0x10DE) ? "NVIDIA" :
            (desc.VendorId == 0x1002 || desc.VendorId == 0x1022) ? "AMD" :
            (desc.VendorId == 0x8086) ? "Intel" : "Unknown";

        // ---------------- Runtime Info ----------------
        if (is_nvidia_gpu(desc.VendorId) && nvapi_available())
        {
            if (NvAPI_Initialize() == NVAPI_OK)
            {
                NvPhysicalGpuHandle handles[NVAPI_MAX_PHYSICAL_GPUS] = { 0 };
                NvU32 gpuCount = 0;
                if (NvAPI_EnumPhysicalGPUs(handles, &gpuCount) == NVAPI_OK && gpuCount > 0)
                {
                    d.gpu_usage = get_nvapi_usage(handles[0]);
                    d.gpu_temperature = get_nvapi_temperature(handles[0]);
                    d.gpu_core_count = get_nvapi_core_count(handles[0]);
                }
                NvAPI_Unload();
            }
            else
            {
                d.gpu_usage = get_gpu_usage();
                d.gpu_temperature = get_gpu_temperature();
                d.gpu_core_count = get_gpu_core_count();
            }
        }
        else
        {
            d.gpu_usage = get_gpu_usage();
            d.gpu_temperature = get_gpu_temperature();
            d.gpu_core_count = get_gpu_core_count();
        }

        list.push_back(d);
        adapter->Release();
    }

    factory->Release();
    return list;
}



