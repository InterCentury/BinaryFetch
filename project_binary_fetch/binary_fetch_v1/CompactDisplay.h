#pragma once
#include <string>
#include <vector>

class CompactDisplay {
public:
    struct DisplayInfo {
        std::string brand_name;   // monitor model / brand (e.g. "HP M22f")
        std::string resolution;   // "1920 x 1080"
        int refresh_rate;         // e.g. 60
    };

private:
    std::vector<DisplayInfo> displays;

public:
    CompactDisplay();                       // constructor fetches displays
    std::vector<DisplayInfo> get_displays() const;
};
