#pragma once
#include <string>
#include "json.hpp"

class CentralControl
{
private:
    nlohmann::json config;
    std::string color_scheme = "default";
    std::string highlight_color = "default";

public:
    CentralControl(const std::string& config_path);

    void loadConfig(const std::string& path);

    // Checks if a module is enabled
    bool isModuleEnabled(const std::string& module_name) const;

    // Checks if a submodule or nested field is enabled
    bool isSubModuleEnabled(const std::string& module, const std::string& submodule = "", const std::string& subfield = "") const;

    std::string getColorScheme() const;
    std::string getHighlightColor() const;

    std::string format(const std::string& text, const std::string& type) const;
};
