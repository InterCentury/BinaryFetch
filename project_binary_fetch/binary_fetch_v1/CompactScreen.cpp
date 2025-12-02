#include "CompactScreen.h"
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>

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

// file-local callback: enumerates monitors and fills the vector
static BOOL CALLBACK MonitorEnumProc(HMONITOR hMon, HDC /*hdc*/, LPRECT /*lprcMonitor*/, LPARAM pData) {
    auto screens = reinterpret_cast<std::vector<CompactScreen::ScreenInfo>*>(pData);
    if (!screens) return TRUE;

    MONITORINFOEXW mi;
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoW(hMon, reinterpret_cast<LPMONITORINFO>(&mi))) {
        return TRUE; // continue
    }

    // Create DC for the monitor device to query resolution/refresh
    HDC hdc = CreateDCW(mi.szDevice, mi.szDevice, NULL, NULL);
    if (!hdc) {
        return TRUE; // continue if DC creation fails
    }

    int width = GetDeviceCaps(hdc, HORZRES);
    int height = GetDeviceCaps(hdc, VERTRES);
    int refresh = GetDeviceCaps(hdc, VREFRESH);
    if (refresh <= 0) refresh = 60; // fallback
    DeleteDC(hdc);

    // Get monitor brand/model via EnumDisplayDevicesW using the monitor device name
    DISPLAY_DEVICEW dd;
    ZeroMemory(&dd, sizeof(dd));
    dd.cb = sizeof(dd);

    std::string brand = "Unknown";
    if (EnumDisplayDevicesW(mi.szDevice, 0, &dd, 0)) {
        std::string tmp = WideCharToUtf8(dd.DeviceString);
        if (!tmp.empty()) brand = tmp;
    }

    CompactScreen::ScreenInfo info;
    info.brand_name = brand;
    info.resolution = std::to_string(width) + " x " + std::to_string(height);
    info.refresh_rate = refresh;

    screens->push_back(info);
    return TRUE; // continue enumeration
}

CompactScreen::CompactScreen() {
    screens.clear();
    ::EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&screens));
}

std::vector<CompactScreen::ScreenInfo> CompactScreen::get_screens() const {
    return screens;
}

std::string CompactScreen::get_formatted() const {
    std::ostringstream ss;
    int idx = 1;
    for (const auto& s : screens) {
        ss << "[Screen " << idx++ << "] -> "
            << s.brand_name << " (" << s.resolution << ") @"
            << s.refresh_rate << "Hz";
        if (idx - 1 < static_cast<int>(screens.size())) ss << "\n";
    }
    return ss.str();
}
