#include "pch.h"

#include "TeleportManager.h"

#include "common.h"

#include <regex>    // For parsing lines
#include <limits>   // For std::numeric_limits

#include "Util/StringUtils.h"

#include "defines.h"

#ifdef LOAD_TELEPORT_INI

TeleportManager& TeleportManager::GetInstance() {
    static TeleportManager instance;
    return instance;
}

TeleportManager::TeleportManager() : m_isLoaded(false) {
    LOG(INFO, "TeleportManager: Initializing singleton instance.");
}

void TeleportManager::LoadTeleportsFromFile(const std::string& filePath) {
    if (m_isLoaded) {
        LOG(WARN, "TeleportManager: Teleports already loaded. Call Clear() first if reloading.");
        return;
    }

    m_categories.clear(); // Clear any previous data

    std::ifstream file(filePath);
    if (!file.is_open()) {
        LOG(ERROR, "TeleportManager: Failed to open teleport file: %s", filePath.c_str());
        return;
    }

    LOG(INFO, "TeleportManager: Loading teleports from file: %s", filePath.c_str());

    std::string line;
    std::string currentCategoryName;
    TeleportCategory* currentCategory = nullptr;

    // Regex to match section headers: [Category Name]
    std::regex section_regex(R"pattern(\s*\[([^\]]+)\]\s*)pattern");
    // Regex to match key-value pairs: Key = X: 1.0 Y: 2.0 Z: 3.0 H: 4.0
    std::regex teleport_entry_regex(R"pattern(\s*([^=]+?)\s*=\s*X:\s*([-+]?\d*\.?\d+)\s*Y:\s*([-+]?\d*\.?\d+)\s*Z:\s*([-+]?\d*\.?\d+)(?:\s*H:\s*([-+]?\d*\.?\d+))?\s*)pattern");

    while (std::getline(file, line)) {
        line = std::regex_replace(line, std::regex(R"(\r)"), ""); // Remove carriage returns
        line = line.substr(0, line.find_first_of(";#")); // Strip comments (hash or semicolon)
        //line = line.strip(); // Remove leading/trailing whitespace

        // Use your new StringUtils::trim function here!
        StringUtils::trim(line); // <--- Apply trim to the line

        if (line.empty()) { // Skip empty lines
            continue;
        }

        std::smatch match;

        // Try to match section header
        if (std::regex_match(line, match, section_regex)) {
            currentCategoryName = match.str(1); // Captured category name
            m_categories.emplace_back(); // Add new category
            currentCategory = &m_categories.back(); // Point to the new category
            currentCategory->name = currentCategoryName;
            LOG(INFO, "TeleportManager: Found category: %s", currentCategoryName.c_str());
        }
        // Try to match teleport entry (only if a category is active)
        else if (currentCategory && std::regex_match(line, match, teleport_entry_regex)) {
            //TeleportLocation loc;
            TeleportLocationNew loc;
            //loc.name = match.str(1).strip(); // Teleport name

            // Apply trim to the captured name string
            loc.name = StringUtils::trim_copy(match.str(1)); // <--- Use trim_copy for captured string

            // Parse coordinates and heading
            try {
                loc.coords.x = std::stof(match.str(2));
                loc.coords.y = std::stof(match.str(3));
                loc.coords.z = std::stof(match.str(4));
                loc.heading = 0.0f; // Default heading

                if (match.size() > 5 && !match.str(5).empty()) { // Check if heading group exists and is not empty
                    loc.heading = std::stof(match.str(5));
                }

                currentCategory->locations.push_back(loc);
                LOG(DEBUG, "TeleportManager: Added '%s' to '%s' (%.2f, %.2f, %.2f)", loc.name.c_str(), currentCategory->name.c_str(), loc.coords.x, loc.coords.y, loc.coords.z);
            }
            catch (const std::exception& e) {
                LOG(ERROR, "TeleportManager: Error parsing coordinates/heading for '%s' in '%s': %s", loc.name.c_str(), currentCategory->name.c_str(), e.what());
            }
        }
        else {
            LOG(WARN, "TeleportManager: Unparsed line in teleport file: %s", line.c_str());
        }
    }

    if (m_categories.empty()) {
        LOG(WARN, "TeleportManager: No teleport categories loaded from file: %s", filePath.c_str());
    }
    else {
        size_t totalLocs = 0;
        for (const auto& cat : m_categories) {
            totalLocs += cat.locations.size();
        }
        LOG(INFO, "TeleportManager: Loaded %zu categories with %zu teleports.", m_categories.size(), totalLocs);
    }

    m_isLoaded = true;
}

const std::vector<TeleportCategory>& TeleportManager::GetAllCategories() const {
    return m_categories;
}

#endif // LOAD_TELEPORT_INI