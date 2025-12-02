#pragma once
// DisplayInfo.h
// ============================
// Project: BinaryFetch
// Module: DisplayInfo
// Description: Retrieve system display information including brand, resolution, and refresh rate.
// Author: Maruf Hasan
// Date: 2025-10-20
// Version: 1.1
// Dependencies: Windows.h, Gdi32.lib
// ============================

#include <windows.h>
#include <string>
#include <vector>

// Link with Gdi32.lib for device context and display functions
#pragma comment(lib, "Gdi32.lib")

using namespace std;

/**
 * @struct MonitorInfo
 * Stores information about a single monitor.
 */
struct MonitorInfo {
    string brand_name;   // Monitor brand or name (e.g., "DELL U2719D")
    string resolution;   // Monitor resolution in format "WIDTHxHEIGHT" (e.g., "3840x2160")
    int refresh_rate;    // Refresh rate in Hz (e.g., 60, 144)
};

/**
 * @class DisplayInfo
 * Provides methods to retrieve information about all connected displays.
 */
class DisplayInfo {
public:
    /**
     * Retrieves all connected monitors with their brand, resolution, and refresh rate.
     *
     * @return vector of MonitorInfo structures, one per monitor
     */
    vector<MonitorInfo> get_all_displays();
};
