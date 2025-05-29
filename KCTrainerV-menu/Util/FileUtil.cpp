// FileUtil.cpp
#include "FileUtil.h"
#include <iostream>
#include <fstream> // For std::ifstream
#include <algorithm>
#include <cctype>

// Include nlohmann/json for JSON parsing.
// Make sure you have this library in your project and its include path set up.
#include "nlohmann/json.hpp" // For nlohmann::json

// Define the global maps declared in the header
namespace File
{
    std::map<std::string, std::map<std::string, unsigned int>> g_categorizedData;
    std::map<std::string, std::string> g_caseInsensitiveItemNames;
    std::map<std::string, std::string> g_caseInsensitiveCategoryNames;

    namespace Util
    {
        std::string ToLower(const std::string& str)
        {
            std::string lower_str = str;
            std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(),
                [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); });
            return lower_str;
        }

        void ClearInputBuffer()
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::string FormatCategoryForDisplay(const std::string& categoryName) {
            std::string formattedName = categoryName;

            // Replace underscores with spaces
            size_t pos = formattedName.find('_');
            while (pos != std::string::npos) {
                formattedName.replace(pos, 1, " ");
                pos = formattedName.find('_', pos + 1);
            }

            // Capitalize the first letter of each word
            bool capitalizeNext = true;
            for (char& c : formattedName) {
                if (std::isspace(c)) {
                    capitalizeNext = true;
                }
                else if (capitalizeNext) {
                    c = static_cast<char>(std::toupper(c));
                    capitalizeNext = false;
                }
                else {
                    c = static_cast<char>(std::tolower(c)); // Ensure rest of word is lowercase
                }
            }
            return formattedName;
        }

    } // namespace Util

    // Generic data loading function implementation
    bool LoadCategorizedDataFromJson(const std::string& filename, const std::string& itemType)
    {
        std::cout << "Attempting to load " << itemType << " data from: " << filename << std::endl;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "ERROR: Failed to open " << itemType << " file at " << filename << std::endl;
            return false;
        }

        nlohmann::json jsonData;
        try {
            file >> jsonData;
            std::cout << "SUCCESS: Parsed " << itemType << " JSON." << std::endl;
        }
        catch (const nlohmann::json::parse_error& e) {
            std::cerr << "ERROR: JSON parsing error in " << itemType << " file: " << e.what() << std::endl;
            return false;
        }

        if (jsonData.empty()) {
            std::cerr << "WARN: Parsed " << itemType << " JSON data is empty." << std::endl;
            return true; // Not an error, just empty data
        }

        return true; // Placeholder, as this generic function will be restructured
    }

    // --- Specific Data Type Initialization Functions ---

    // Moved maps from global File namespace to specific Vehicle/Weapon namespaces as static members
    // This allows loading both vehicles and weapons without overwriting

    // Vehicle-specific maps (defined here in .cpp)
    static std::map<std::string, std::map<std::string, unsigned int>> s_vehicles_categorized;
    static std::map<std::string, std::string> s_vehicles_caseInsensitiveNames;
    static std::map<std::string, std::string> s_vehicles_caseInsensitiveCategories;

    namespace Vehicle
    {
        void InitializeVehicleData(const std::string& filename)
        {
            std::cout << "Attempting to load Vehicle data from: " << filename << std::endl;

            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "ERROR: Failed to open Vehicle file at " << filename << std::endl;
                return;
            }

            nlohmann::json jsonData;
            try {
                file >> jsonData;
                std::cout << "SUCCESS: Parsed Vehicle JSON." << std::endl;
            }
            catch (const nlohmann::json::parse_error& e) {
                std::cerr << "ERROR: JSON parsing error in Vehicle file: " << e.what() << std::endl;
                return;
            }

            if (jsonData.empty()) {
                std::cerr << "WARN: Parsed Vehicle JSON data is empty." << std::endl;
                return;
            }

            // Clear previous data
            s_vehicles_categorized.clear();
            s_vehicles_caseInsensitiveNames.clear();
            s_vehicles_caseInsensitiveCategories.clear();

            for (auto const& [categoryName, categoryValue] : jsonData.items())
            {
                s_vehicles_categorized[categoryName] = {}; // Initialize inner map
                s_vehicles_caseInsensitiveCategories[File::Util::ToLower(categoryName)] = categoryName;

                if (categoryValue.is_object()) {
                    for (auto const& [itemName, itemHash] : categoryValue.items())
                    {
                        unsigned int hash = itemHash.get<unsigned int>(); // Directly get hash as unsigned int
                        s_vehicles_categorized[categoryName][itemName] = hash;
                        s_vehicles_caseInsensitiveNames[File::Util::ToLower(itemName)] = itemName;
                    }
                }
            }
            std::cout << "DEBUG: Loaded " << s_vehicles_categorized.size() << " vehicle categories.\n";
            std::cout << "DEBUG: Initialized " << s_vehicles_caseInsensitiveNames.size() << " case-insensitive vehicle names.\n";
        }

        // --- NEW: Accessor Function Implementations ---
        const std::map<std::string, std::map<std::string, unsigned int>>& GetCategorizedVehicles() {
            return s_vehicles_categorized;
        }

        const std::map<std::string, std::string>& GetCaseInsensitiveVehicleNames() {
            return s_vehicles_caseInsensitiveNames;
        }

        const std::map<std::string, std::string>& GetCaseInsensitiveVehicleCategories() {
            return s_vehicles_caseInsensitiveCategories;
        }
        // --- END NEW ---

        // --- Console App Functions (adapted to use s_vehicles_categorized maps) ---
        void ReadVehicleFromName()
        {
            std::cout << "1. Looking up vehicle hash by name:\n";
            std::string vehicleNameInput;
            std::cout << "   Enter vehicle name (e.g., Adder, Banshee, Zentorno): ";
            std::getline(std::cin, vehicleNameInput);

            std::string lowerCaseInput = File::Util::ToLower(vehicleNameInput);
            std::cout << "   DEBUG: Searching for lowercase input: '" << lowerCaseInput << "'\n";

            auto it_ci_name = s_vehicles_caseInsensitiveNames.find(lowerCaseInput);

            if (it_ci_name != s_vehicles_caseInsensitiveNames.end()) {
                const std::string& originalName = it_ci_name->second;
                // We need to search through all categories to find the vehicle by its original name
                for (const auto& categoryPair : s_vehicles_categorized) {
                    const auto& vehiclesInThisCategory = categoryPair.second;
                    auto it_vehicle = vehiclesInThisCategory.find(originalName);
                    if (it_vehicle != vehiclesInThisCategory.end()) {
                        std::cout << "   Found '" << it_vehicle->first << "': Hash = " << it_vehicle->second << " (Category: " << categoryPair.first << ")\n";
                        return; // Found and displayed, exit function
                    }
                }
                std::cout << "   Internal error: Original name '" << originalName << "' not found in s_vehicles_categorized.\n";
            }
            else {
                std::cout << "   Vehicle '" << vehicleNameInput << "' not found (case-insensitive lookup).\n";
            }
            std::cout << "\n";
        }

        void ReadVehicleFromHash()
        {
            std::cout << "2. Looking up vehicle name by hash:\n";
            unsigned int hashInput;
            std::cout << "   Enter vehicle hash (e.g., 3078201489 for Adder): ";
            std::cin >> hashInput;
            File::Util::ClearInputBuffer();

            // We need to search all categories to find the name by hash
            for (const auto& categoryPair : s_vehicles_categorized) {
                for (const auto& vehiclePair : categoryPair.second) {
                    if (vehiclePair.second == hashInput) {
                        std::cout << "   Found hash " << hashInput << ": Name = '" << vehiclePair.first << "' (Category: " << categoryPair.first << ")\n";
                        return; // Found and displayed, exit function
                    }
                }
            }
            std::cout << "   Hash " << hashInput << " not found.\n";
            std::cout << "\n";
        }

        void ReadVehiclesByCategory()
        {
            std::cout << "3. Accessing a specific vehicle category:\n";
            std::string categoryInput;
            std::cout << "   Enter category name (e.g., Super, Sports, Muscle): ";
            std::getline(std::cin, categoryInput);

            std::string lowerCaseCategoryInput = File::Util::ToLower(categoryInput);
            auto it_ci_category = s_vehicles_caseInsensitiveCategories.find(lowerCaseCategoryInput);

            if (it_ci_category != s_vehicles_caseInsensitiveCategories.end()) {
                const std::string& originalCategoryName = it_ci_category->second;
                auto it_category_map = s_vehicles_categorized.find(originalCategoryName);

                if (it_category_map != s_vehicles_categorized.end()) {
                    std::cout << "   Vehicles in category '" << it_category_map->first << "':\n";
                    const auto& vehiclesInThisCategory = it_category_map->second;
                    if (vehiclesInThisCategory.empty()) {
                        std::cout << "     (No vehicles found in this category)\n";
                    }
                    else {
                        for (const auto& vehiclePair : vehiclesInThisCategory) {
                            std::cout << "     - " << vehiclePair.first << " (Hash: " << vehiclePair.second << ")\n";
                        }
                    }
                }
                else {
                    std::cerr << "Internal error: Original category '" << originalCategoryName << "' not found in s_vehicles_categorized.\n";
                }
            }
            else {
                std::cout << "   Category '" << categoryInput << "' not found (case-insensitive lookup).\n";
            }
            std::cout << "\n";
        }

        void ListAllVehicles()
        {
            std::cout << "4. Listing ALL vehicle categories and their vehicles:\n";
            if (s_vehicles_categorized.empty()) {
                std::cout << "   No categorized vehicle data available.\n";
            }
            else {
                for (const auto& categoryPair : s_vehicles_categorized) {
                    const std::string& categoryName = categoryPair.first;
                    const auto& vehiclesInCurrentCategory = categoryPair.second;
                    std::cout << "   --- Category: " << categoryName << " (" << vehiclesInCurrentCategory.size() << " vehicles) ---\n";
                    for (const auto& vehiclePair : vehiclesInCurrentCategory) {
                        std::cout << "     - " << vehiclePair.first << " (Hash: " << vehiclePair.second << ")\n";
                    }
                }
            }
            std::cout << "\n";
        }
    } // namespace Vehicle


    // Weapon-specific maps (defined here in .cpp)
    // TODO Move weapon file management into here out of Script.cpp
    //static std::map<std::string, std::map<std::string, unsigned int>> s_weapons_categorized;
    //static std::map<std::string, std::string> s_weapons_caseInsensitiveNames;
    //static std::map<std::string, std::string> s_weapons_caseInsensitiveCategories;

    //namespace Weapon
    //{
    //    void InitializeWeaponData(const std::string& filename)
    //    {
    //        std::cout << "Attempting to load Weapon data from: " << filename << std::endl;

    //        std::ifstream file(filename);
    //        if (!file.is_open()) {
    //            std::cerr << "ERROR: Failed to open Weapon file at " << filename << std::endl;
    //            return;
    //        }

    //        nlohmann::json jsonData;
    //        try {
    //            file >> jsonData;
    //            std::cout << "SUCCESS: Parsed Weapon JSON." << std::endl;
    //        }
    //        catch (const nlohmann::json::parse_error& e) {
    //            std::cerr << "ERROR: JSON parsing error in Weapon file: " << e.what() << std::endl;
    //            return;
    //        }

    //        if (jsonData.empty()) {
    //            std::cerr << "WARN: Parsed Weapon JSON data is empty." << std::endl;
    //            return;
    //        }

    //        // Clear previous data
    //        s_weapons_categorized.clear();
    //        s_weapons_caseInsensitiveNames.clear();
    //        s_weapons_caseInsensitiveCategories.clear();

    //        for (auto const& [categoryName, categoryValue] : jsonData.items())
    //        {
    //            s_weapons_categorized[categoryName] = {}; // Initialize inner map
    //            s_weapons_caseInsensitiveCategories[File::Util::ToLower(categoryName)] = categoryName;

    //            if (categoryValue.is_object()) {
    //                for (auto const& [itemName, itemHash] : categoryValue.items())
    //                {
    //                    unsigned int hash = itemHash.get<unsigned int>(); // Directly get hash as unsigned int
    //                    s_weapons_categorized[categoryName][itemName] = hash;
    //                    s_weapons_caseInsensitiveNames[File::Util::ToLower(itemName)] = itemName;
    //                }
    //            }
    //        }
    //        std::cout << "DEBUG: Loaded " << s_weapons_categorized.size() << " weapon categories.\n";
    //        std::cout << "DEBUG: Initialized " << s_weapons_caseInsensitiveNames.size() << " case-insensitive weapon names.\n";
    //    }

    //    // --- NEW: Accessor Function Implementations ---
    //    const std::map<std::string, std::map<std::string, unsigned int>>& GetCategorizedWeapons() {
    //        return s_weapons_categorized;
    //    }

    //    const std::map<std::string, std::string>& GetCaseInsensitiveWeaponNames() {
    //        return s_weapons_caseInsensitiveNames;
    //    }

    //    const std::map<std::string, std::string>& GetCaseInsensitiveWeaponCategories() {
    //        return s_weapons_caseInsensitiveCategories;
    //    }
    //    // --- END NEW ---

    //    // --- Console App Functions (adapted to use s_weapons_categorized maps) ---
    //    void ReadWeaponFromName()
    //    {
    //        std::cout << "1. Looking up weapon hash by name:\n";
    //        std::string weaponNameInput;
    //        std::cout << "   Enter weapon name (e.g., Pistol, Carbine Rifle): ";
    //        std::getline(std::cin, weaponNameInput);

    //        std::string lowerCaseInput = File::Util::ToLower(weaponNameInput);
    //        std::cout << "   DEBUG: Searching for lowercase input: '" << lowerCaseInput << "'\n";

    //        auto it_ci_name = s_weapons_caseInsensitiveNames.find(lowerCaseInput);

    //        if (it_ci_name != s_weapons_caseInsensitiveNames.end()) {
    //            const std::string& originalName = it_ci_name->second;
    //            for (const auto& categoryPair : s_weapons_categorized) {
    //                const auto& weaponsInThisCategory = categoryPair.second;
    //                auto it_weapon = weaponsInThisCategory.find(originalName);
    //                if (it_weapon != weaponsInThisCategory.end()) {
    //                    std::cout << "   Found '" << it_weapon->first << "': Hash = " << it_weapon->second << " (Category: " << categoryPair.first << ")\n";
    //                    return;
    //                }
    //            }
    //            std::cout << "   Internal error: Original name '" << originalName << "' not found in s_weapons_categorized.\n";
    //        }
    //        else {
    //            std::cout << "   Weapon '" << weaponNameInput << "' not found (case-insensitive lookup).\n";
    //        }
    //        std::cout << "\n";
    //    }

    //    void ReadWeaponFromHash()
    //    {
    //        std::cout << "2. Looking up weapon name by hash:\n";
    //        unsigned int hashInput;
    //        std::cout << "   Enter weapon hash (e.g., 453432689 for Pistol): ";
    //        std::cin >> hashInput;
    //        File::Util::ClearInputBuffer();

    //        for (const auto& categoryPair : s_weapons_categorized) {
    //            for (const auto& weaponPair : categoryPair.second) {
    //                if (weaponPair.second == hashInput) {
    //                    std::cout << "   Found hash " << hashInput << ": Name = '" << weaponPair.first << "' (Category: " << categoryPair.first << ")\n";
    //                    return;
    //                }
    //            }
    //        }
    //        std::cout << "   Hash " << hashInput << " not found.\n";
    //        std::cout << "\n";
    //    }

    //    void ReadWeaponsByCategory()
    //    {
    //        std::cout << "3. Accessing a specific weapon category:\n";
    //        std::string categoryInput;
    //        std::cout << "   Enter category name (e.g., GROUP_PISTOL, GROUP_RIFLE): ";
    //        std::getline(std::cin, categoryInput);

    //        std::string lowerCaseCategoryInput = File::Util::ToLower(categoryInput);
    //        auto it_ci_category = s_weapons_caseInsensitiveCategories.find(lowerCaseCategoryInput);

    //        if (it_ci_category != s_weapons_caseInsensitiveCategories.end()) {
    //            const std::string& originalCategoryName = it_ci_category->second;
    //            auto it_category_map = s_weapons_categorized.find(originalCategoryName);

    //            if (it_category_map != s_weapons_categorized.end()) {
    //                std::cout << "   Weapons in category '" << it_category_map->first << "':\n";
    //                const auto& weaponsInThisCategory = it_category_map->second;
    //                if (weaponsInThisCategory.empty()) {
    //                    std::cout << "     (No weapons found in this category)\n";
    //                }
    //                else {
    //                    for (const auto& weaponPair : weaponsInThisCategory) {
    //                        std::cout << "     - " << weaponPair.first << " (Hash: " << weaponPair.second << ")\n";
    //                    }
    //                }
    //            }
    //            else {
    //                std::cerr << "Internal error: Original category '" << originalCategoryName << "' not found in s_weapons_categorized.\n";
    //            }
    //        }
    //        else {
    //            std::cout << "   Category '" << categoryInput << "' not found (case-insensitive lookup).\n";
    //        }
    //        std::cout << "\n";
    //    }

    //    void ListAllWeapons()
    //    {
    //        std::cout << "4. Listing ALL weapon categories and their weapons:\n";
    //        if (s_weapons_categorized.empty()) {
    //            std::cout << "   No categorized weapon data available.\n";
    //        }
    //        else {
    //            for (const auto& categoryPair : s_weapons_categorized) {
    //                const std::string& categoryName = categoryPair.first;
    //                const auto& weaponsInCurrentCategory = categoryPair.second;
    //                std::cout << "   --- Category: " << categoryName << " (" << weaponsInCurrentCategory.size() << " weapons) ---\n";
    //                for (const auto& weaponPair : weaponsInCurrentCategory) {
    //                    std::cout << "     - " << weaponPair.first << " (Hash: " << weaponPair.second << ")\n";
    //                }
    //            }
    //        }
    //        std::cout << "\n";
    //    }
    //} // namespace Weapon

} // namespace File