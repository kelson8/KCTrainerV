#pragma once
#include "Constants.hpp"

#include "TeleportLocations.h"

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

#endif // LOAD_TELEPORT_INI

// Possibly use namespace format for this?
//namespace Teleports
//{
//    namespace Ipls
//    {
//
//    }
//}

// Manager class for loading and accessing teleport data
class TeleportManager {
public:
    static TeleportManager& GetInstance()
    {
        static TeleportManager instance;
        return instance;
    }

#ifdef LOAD_TELEPORT_INI
    void LoadTeleportsFromFile(const std::string& filePath);
    const std::vector<TeleportCategory>& GetAllCategories() const;
#endif

    void LoadIPL(const std::string& iplName);
    void UnloadIPL(const std::string& iplName);

    void HandleTeleportIpls(const TeleportIplInfo& newLocation);

    void LoadAllIplsForLocation(const TeleportIplInfo& location);
    void UnloadAllIplsForLocation(const TeleportIplInfo& location);

    void LoadAllCasinoIpls();
    void UnloadAllCasinoIpls();

private:
    TeleportManager() {} // Private constructor to prevent external instantiation
    TeleportManager(const TeleportManager&) = delete;
    TeleportManager& operator=(const TeleportManager&) = delete;

#ifdef LOAD_TELEPORT_INI
    std::vector<TeleportCategory> m_categories;
    bool m_isLoaded = false;
#endif
};
