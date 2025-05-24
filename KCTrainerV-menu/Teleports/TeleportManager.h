#pragma once
#include "Constants.hpp"

#include <vector>
#include <string>
#include <string_view> // For std::string_view
#include <map>


#include <inc/natives.h>
#include <inc/types.h>

#ifdef LOAD_TELEPORT_INI

// Structure for a single teleport entry
struct TeleportLocationNew {
    std::string name;       // The name from the INI (e.g., "Michael's House")
    Vector3 coords;         // X, Y, Z coordinates
    float heading;          // Heading (optional, but good to include)
};

// Structure for a category of teleports (maps to an INI section)
struct TeleportCategory {
    std::string name; // The category name (e.g., "Player Houses", "Custom Spots")
    std::vector<TeleportLocationNew> locations;
};

// Manager class for loading and accessing teleport data
class TeleportManager {
public:
    static TeleportManager& GetInstance(); // Singleton access

    void LoadTeleportsFromFile(const std::string& filePath);
    const std::vector<TeleportCategory>& GetAllCategories() const;

private:
    TeleportManager();
    TeleportManager(const TeleportManager&) = delete;
    TeleportManager& operator=(const TeleportManager&) = delete;

    std::vector<TeleportCategory> m_categories;
    bool m_isLoaded = false;
};

#endif // LOAD_TELEPORT_INI