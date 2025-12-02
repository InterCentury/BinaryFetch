#include "DisplayInfo.h"
#include <sstream>  // For string manipulation
#include <string>

/**
 * Callback function for EnumDisplayMonitors.
 * Retrieves monitor resolution, refresh rate, and brand.
 */
BOOL CALLBACK MonitorEnumProc(HMONITOR hMon, HDC, LPRECT, LPARAM pData) {
    vector<MonitorInfo>* monitors = reinterpret_cast<vector<MonitorInfo>*>(pData);

    MONITORINFOEX mi;
    mi.cbSize = sizeof(MONITORINFOEX);

    if (!GetMonitorInfo(hMon, &mi))
        return TRUE;

    // Create a device context for the monitor to get capabilities
    HDC hdc = CreateDC(mi.szDevice, mi.szDevice, NULL, NULL);
    if (hdc) {
        int width = GetDeviceCaps(hdc, HORZRES);
        int height = GetDeviceCaps(hdc, VERTRES);
        int refresh = GetDeviceCaps(hdc, VREFRESH);
        if (refresh <= 0) refresh = 60;
        DeleteDC(hdc);

        // Get the display brand using EnumDisplayDevices
        DISPLAY_DEVICE dd;
        dd.cb = sizeof(dd);
        string brand = "Unknown";
        if (EnumDisplayDevices(mi.szDevice, 0, &dd, 0)) {
#ifdef UNICODE
            wstring wbrand(dd.DeviceString);
            brand = string(wbrand.begin(), wbrand.end());
#else
            brand = dd.DeviceString;
#endif
        }

        // Store monitor info
        MonitorInfo info;
        info.brand_name = brand;
        info.resolution = to_string(width) + "x" + to_string(height);
        info.refresh_rate = refresh;

        monitors->push_back(info);
    }

    return TRUE;  // Continue enumeration
}

/**
 * Retrieves information of all connected displays.
 *
 * @return Vector containing brand, resolution, and refresh rate for each monitor
 */
vector<MonitorInfo> DisplayInfo::get_all_displays() {
    vector<MonitorInfo> monitors;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitors));
    return monitors;
}
