#pragma once
#include "Constants.hpp"

#include <string>
#include <vector>
#include <map> // Optional: if you want a map to quickly look up categories by name
#include <inc/types.h> // For Hash (assuming this is your DWORD/unsigned long typedef)

#ifdef PLAYER_SKIN_CHANGER


// Structure to hold individual ped information
struct PedInfo {
    std::string modelName;  // The 'name' attribute from XML (e.g., "player_zero")
    std::string caption;    // The 'caption' attribute from XML (e.g., "Michael")
    Hash hash;              // The runtime hash of the modelName (calculated from modelName)
};

// Structure to hold a category of peds
struct PedCategory {
    std::string name; // The 'name' attribute from XML (e.g., "Player")
    std::vector<PedInfo> peds; // List of peds in this category
};

// Manager class to load and provide access to ped data
class PedModelManager {
public:
    // Instance of the PedModelManager
    static PedModelManager& GetInstance()
    {
        static PedModelManager instance;
        return instance;
    }

    PedModelManager(); // Constructor

    // Call this once during your mod's initialization to load data from XML
    void LoadPedsFromFile(const std::string& filePath);

    // Getters for accessing the loaded data
    const std::vector<PedCategory>& GetAllCategories() const;
    const PedCategory* GetCategory(const std::string& categoryName) const; // Optional: Get a specific category by name

private:
    //PedModelManager();
    PedModelManager(const PedModelManager&) = delete;
    PedModelManager& operator=(const PedModelManager&) = delete;

    std::vector<PedCategory> m_categories; // Stores all loaded categories
    bool m_isLoaded = false; // Flag to prevent multiple loads
};

#endif