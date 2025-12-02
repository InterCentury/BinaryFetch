#include "CentralControl.h"
#include <fstream>
#include <iostream>

/**
 * @brief Constructs a CentralControl object and loads configuration.
 *
 * @param config_path Path to the JSON configuration file.
 */
CentralControl::CentralControl(const std::string& config_path) {
    loadConfig(config_path);
}

/**
 * @brief Loads configuration from a JSON file.
 *
 * Reads the JSON configuration file specified by `path` and initializes
 * preferences such as color schemes and module settings.
 *
 * @param path Path to the configuration file.
 */
void CentralControl::loadConfig(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << path << "\n";
        return;
    }

    // Parse JSON configuration
    file >> config;

    // Load user preferences if available
    if (config.contains("preferences")) {
        color_scheme = config["preferences"].value("color_scheme", "default");
        highlight_color = config["preferences"].value("highlight_color", "default");
    }
}

/**
 * @brief Checks if a main module is enabled in the configuration.
 *
 * @param module_name Name of the module to check.
 * @return true if the module exists and is enabled; false otherwise.
 */
bool CentralControl::isModuleEnabled(const std::string& module_name) const
{
    if (!config.contains("main_modules") || !config["main_modules"].contains(module_name))
        return false;

    return config["main_modules"][module_name].get<bool>();
}

/**
 * @brief Checks if a submodule or nested field is enabled.
 *
 * Supports checking nested fields using dot notation (e.g., "cache.l1").
 * Returns false if any part of the hierarchy does not exist or is disabled.
 *
 * @param module Main module name.
 * @param submodule Submodule name. Pass empty string to check only main module.
 * @param subfield Nested field within submodule. Supports dot notation.
 * @return true if the submodule/subfield exists and is enabled; false otherwise.
 */
bool CentralControl::isSubModuleEnabled(
    const std::string& module,
    const std::string& submodule,
    const std::string& subfield
) const
{
    // First check if main module is enabled
    if (!isModuleEnabled(module)) return false;

    // Check if submodules section exists
    if (!config.contains("submodules") || !config["submodules"].contains(module))
        return false;

    const auto& mod = config["submodules"][module];

    // If no submodule specified, main module enabled ? return true
    if (submodule.empty()) return true;

    if (!mod.contains(submodule)) return false;

    const nlohmann::json* current = &mod[submodule];

    // If no subfield specified, return submodule value as bool
    if (subfield.empty())
        return current->get<bool>();

    // Handle nested fields using dot notation (e.g., "cache.l1")
    std::string temp = subfield;
    size_t pos = 0;
    while ((pos = temp.find('.')) != std::string::npos) {
        std::string key = temp.substr(0, pos);
        if (!current->contains(key)) return false;
        current = &(*current)[key];
        temp.erase(0, pos + 1);
    }

    return current->value(temp, false);
}

/**
 * @brief Retrieves the configured color scheme.
 *
 * @return std::string The name of the color scheme.
 */
std::string CentralControl::getColorScheme() const {
    return color_scheme;
}

/**
 * @brief Retrieves the configured highlight color.
 *
 * @return std::string The highlight color.
 */
std::string CentralControl::getHighlightColor() const {
    return highlight_color;
}

/**
 * @brief Formats a text string with the configured highlight color.
 *
 * This method is used for visual emphasis in logs or UI.
 *
 * @param text The text to format.
 * @param type Type of formatting. Only "highlight" is currently supported.
 * @return std::string Formatted text.
 */
std::string CentralControl::format(const std::string& text, const std::string& type) const
{
    if (type == "highlight")
        return "[" + highlight_color + "] " + text + " [/" + highlight_color + "]";

    return text;
}
