#pragma once
#include <string>
#include <vector>

class CompactScreen {
public:
    struct ScreenInfo {
        std::string brand_name;   // monitor model / brand (e.g. "HP M22f")
        std::string resolution;   // "1920 x 1080"
        int refresh_rate;         // e.g. 60
    };

private:
    std::vector<ScreenInfo> screens;

public:
    CompactScreen();                              // populates screens on construction
    std::vector<ScreenInfo> get_screens() const;  // returns vector of screens

    // optional convenience: get a formatted multiline string using "Screen" wording
    std::string get_formatted() const;
};
