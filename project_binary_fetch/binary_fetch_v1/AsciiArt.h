#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <functional> // For std::function

class AsciiArt {
private:
    std::vector<std::string> artLines;
    int maxWidth;
    int height;
    bool enabled;
    int spacing;

public:
    AsciiArt();
    bool loadFromFile(const std::string& filename);
    bool isEnabled() const;
    void setEnabled(bool enable);
    void clear();
    void printWithArt(std::function<void()> infoPrinter);
};