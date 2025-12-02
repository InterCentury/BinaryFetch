#include "CompactDisplay.h"
#include <windows.h>
#include <string>
#include <vector>

// Helper: convert WCHAR* (UTF-16) to UTF-8 std::string
static std::string WideCharToUtf8(const WCHAR* wstr) {
    if (!wstr) return std::string();
    int sizeNeeded = ::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (sizeNeeded <= 0) return std::string();
    std::string result;
    result.resize(sizeNeeded);
    ::WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], sizeNeeded, NULL, NULL);
    if (!result.empty() && result.back() == '\0') result.pop_back();
    return result;
}

// file-local callback to avoid multiple-definition/linker issues
static BOOL CALLBACK MonitorEnumProc(HMONITOR hMon, HDC /*hdc*/, LPRECT /*lprcMonitor*/, LPARAM pData) {
    auto monitors = reinterpret_cast<std::vector<CompactDisplay::DisplayInfo>*>(pData);
    if (!monitors) return TRUE;

    MONITORINFOEXW mi;
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoW(hMon, reinterpret_cast<LPMONITORINFO>(&mi))) {
        return TRUE; // continue enumeration
    }

    // Create DC for this monitor device to query capabilities
    HDC hdc = CreateDCW(mi.szDevice, mi.szDevice, NULL, NULL);
    if (!hdc) {
        return TRUE;
    }

    int width = GetDeviceCaps(hdc, HORZRES);
    int height = GetDeviceCaps(hdc, VERTRES);
    int refresh = GetDeviceCaps(hdc, VREFRESH);
    if (refresh <= 0) refresh = 60;
    DeleteDC(hdc);

    // Try to get the monitor brand/model using EnumDisplayDevicesW
    DISPLAY_DEVICEW dd;
    ZeroMemory(&dd, sizeof(dd));
    dd.cb = sizeof(dd);

    std::string brand = "Unknown";
    if (EnumDisplayDevicesW(mi.szDevice, 0, &dd, 0)) {
        brand = WideCharToUtf8(dd.DeviceString);
        if (brand.empty()) brand = "Unknown";
    }

    CompactDisplay::DisplayInfo info;
    info.brand_name = brand;
    info.resolution = std::to_string(width) + " x " + std::to_string(height);
    info.refresh_rate = refresh;

    monitors->push_back(info);
    return TRUE; // continue enumeration
}

CompactDisplay::CompactDisplay() {
    // Populate displays vector by enumerating monitors
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&displays));
}

std::vector<CompactDisplay::DisplayInfo> CompactDisplay::get_displays() const {
    return displays;
}
