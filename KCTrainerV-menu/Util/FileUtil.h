#pragma once
#include <string>
#include <map>
#include <vector> // Potentially useful if you still use vector for internal temporary storage

#include "nlohmann/json.hpp"

// Forward declarations for nlohmann::json types to avoid including the full header here
//namespace nlohmann {
//    class json;
//}

namespace File
{
    namespace Util
    {
        std::string ToLower(const std::string& str);
        void ClearInputBuffer();

        // --- NEW: Function to format category names for display ---
        std::string FormatCategoryForDisplay(const std::string& categoryName);

    } // namespace Util

    // Generic data structure for any categorized item (vehicle, weapon, ped)
    struct ItemInfo
    {
        std::string name; // Original mixed-case name
        unsigned int hash;
    };

    // Global maps for case-insensitive lookup, now generic
    // These will be populated at runtime.
    extern std::map<std::string, std::string> g_caseInsensitiveItemNames;
    extern std::map<std::string, std::string> g_caseInsensitiveCategoryNames;


    // Generic function to load categorized data
    // categoryName is the category name (e.g., "GROUP_PISTOL")
    // itemsInThisCategory is a map of displayName -> hash
    extern std::map<std::string, std::map<std::string, unsigned int>> g_categorizedData;

    // A generic function to load data from a processed JSON file
    bool LoadCategorizedDataFromJson(const std::string& filename, const std::string& itemType);

    // Specific functions for vehicles and weapons for your menu/script logic
    namespace Vehicle
    {
        // These will now just call the generic LoadCategorizedDataFromJson
        void InitializeVehicleData(const std::string& filename);

        // --- NEW: Accessor Functions for Vehicle Data ---
        const std::map<std::string, std::map<std::string, unsigned int>>& GetCategorizedVehicles();
        const std::map<std::string, std::string>& GetCaseInsensitiveVehicleNames();
        const std::map<std::string, std::string>& GetCaseInsensitiveVehicleCategories();
        // --- END NEW ---

        // (Your console app functions if you still want them for testing,
        // but they will rely on g_categorizedData instead of GTA_Vehicles:: maps)
        //void ReadVehicleFromName();
        //void ReadVehicleFromHash();
        //void ReadVehiclesByCategory();
        //void ListAllVehicles();
    } // namespace Vehicle

    //namespace Weapon
    //{
    //    // These will now just call the generic LoadCategorizedDataFromJson
    //    void InitializeWeaponData(const std::string& filename);

    //    // --- NEW: Accessor Functions for Weapon Data ---
    //    const std::map<std::string, std::map<std::string, unsigned int>>& GetCategorizedWeapons();
    //    const std::map<std::string, std::string>& GetCaseInsensitiveWeaponNames();
    //    const std::map<std::string, std::string>& GetCaseInsensitiveWeaponCategories();
    //    // --- END NEW ---

    //    // (Your console app functions)
    //    void ReadWeaponFromName();
    //    void ReadWeaponFromHash();
    //    void ReadWeaponsByCategory();
    //    void ListAllWeapons();
    //} // namespace Weapon

} // namespace File