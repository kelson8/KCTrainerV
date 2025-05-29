#include "pch.h"
#include "Constants.hpp"
#include "VehicleMenu.h"

#include "Scripts/Extras/Game.h"
#include "Scripts/PlayerScripts.h"

// Util
#include "Util/UI.hpp"
#include "Util/Hash.h"
#include "Util/FileUtil.h"

// Vehicle
#include "Scripts/Vehicles/VehicleData.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/VehicleSpawner.h"

void VehicleMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Vehicle");

    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    Ped playerPed = PLAYER_PED_ID();

    //--------------
    // Vehicle options sub menu
    //---------------
    mbCtx.MenuOption("Options", "VehicleOptionsSubmenu", { "Show a list of vehicle options" });

    // This seems to mostly work fine.
    if (mbCtx.Option("Spawn jet into air", { "Spawn you into a jet in the air with some speed." }))
    {
        vehicleSpawner.SpawnJetInAir();
    }

    // TODO Move into vehicle scripts
    // TODO Fix this to work, not sure how to change flags just yet
    //mbCtx.BoolOption("Ramp buggy flag", rampBuggyFlag);
    //if (mbCtx.Option("Toggle ramp buggy flag", { "Test for setting the ramp buggy flag on your vehicle." }))
    //{
    //    if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) 
    //    {
    //        Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
                //if (rampBuggyFlag)
                //{

                //}
    //        
    //    } else 
    //    {
    //        UI::Notify("You are not in a vehicle!");
    //    }
    //}


    // Toggles the boolean to spawn in the vehicle and remove the old one.
    //mbCtx.BoolOption("Spawn into vehicle", vehicleScripts.spawnInsideVehicle, { "Toggle spawning in the spawned vehicle" });
    mbCtx.BoolOption("Spawn into vehicle", vehicleSpawner.spawnInsideVehicle, { "Toggle spawning in the spawned vehicle" });

//#define TRAIN_TEST
    // This crashes it, I'll try to fix it later.
#ifdef TRAIN_TEST
    if (mbCtx.Option("Create Test Train", {"Create a train as a test at Train Station #1 warp."}))
    {
        Hash trainHash = "freight"_hash;
        float trainX = 247.9364f;
        float trainY = -1198.597f;
        float trainZ = 37.4482f;
        Vector3 trainPos = Vector3(trainX, trainY, trainZ);

        //vehicleScripts.CreateMissionTrain(0, trainPos, true);
        //vehicleScripts.CreateMissionTrain(trainHash, trainPos, true);
        vehicleScripts.CreateMissionTrain(trainHash, true);
    }
#endif //TRAIN_TEST
#undef TRAIN_TEST

    if (mbCtx.Option("Vehicle spawn keyboard", { "Display a in game keyboard to spawn a vehicle with." }))
    {
        std::string inputString = Game::InputBox("DEFAULT", 64);
        if (inputString.length() > 0)
        {
            Hash vehicleHash = MISC::GET_HASH_KEY(inputString.c_str());
            //UI::Notify(inputString);
            //UI::Notify(spawnedVehicleString);
            
            //std::cout << inputString << std::endl;
            log_output(inputString);
            vehicleSpawner.SpawnVehicle(vehicleHash, true);
        }
        else {
            std::string invalidInputString = "Input string not valid!";
            UI::Notify(invalidInputString);
            log_output(invalidInputString);
            //std::cout << invalidInputString << std::endl;

        }
        
    }

    // Moved vehicle spawning code into the categories.
    // TODO Move Vehicle categories into VehicleCategoryMenu.cpp in Submenus/Vehicle folder.
    mbCtx.MenuOption("Categories", "VehicleCategorySubmenu", { "Show the list of vehicle categories." });    
}

#pragma region VehicleCategories
void VehicleMenu::BuildVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    //mbCtx.Title("Categories");
    mbCtx.Title("Vehicle Categories"); // More descriptive title

    auto& vehicleScripts = VehicleScripts::GetInstance();    

    // Get the categorized vehicle data
    const auto& categorizedVehicles = File::Vehicle::GetCategorizedVehicles();

    // --- Define categories to exclude ---
    // You can make this a std::set<std::string> for multiple exclusions,
    // and convert to lowercase for case-insensitive comparison if needed.
    const std::string EXCLUDE_CATEGORY = "RAIL"; // Or "RAIL" if your JSON uses that exact casing


    // Iterate through all available categories and create a menu option for each
    for (const auto& categoryPair : categorizedVehicles)
    {
        const std::string& categoryName = categoryPair.first; // e.g., "Sports", "Super", "Muscle"

        if (categoryName == EXCLUDE_CATEGORY) {
            //std::cout << "DEBUG: Skipping category '" << categoryName << "' from menu display.\n";
            continue; // Skip this iteration, don't add to menu
        }

        // This creates an option that, when selected, will provide 'categoryName'
        // as the identifier to your menu system for dispatching.
        //mbCtx.MenuOption(categoryName, categoryName); // Use the category name as the identifier

        // New for making this look nicer.
        // --- Use the new utility function for display ---
        std::string displayCategoryName = File::Util::FormatCategoryForDisplay(categoryName);

         //mbCtx.MenuOption takes (display_text, identifier)
        mbCtx.MenuOption(displayCategoryName, categoryName); // Identifier remains original "OFF_ROAD"

        // Note: If you need to append a suffix like "VehicleCategorySubmenu" to the identifier
        // for your menu's dispatching system, you would do:
        // mbCtx.MenuOption(categoryName, categoryName + "VehicleCategorySubmenu");
        // But using just the categoryName is cleaner if your dispatcher can handle it.
    }

}

// This method works for multiple vehicle categories.
void VehicleMenu::BuildVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context, const std::string& categoryToBuild)
{
    mbCtx.Title(categoryToBuild); // Set the submenu title to the category name

    auto& playerScripts = PlayerScripts::GetInstance(); // Your existing instances
    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    // Use the case-insensitive lookup for the category name itself
    // This handles cases where user types "sports" but map key is "Sports" or "SPORTS"
    std::string lowerCaseCategoryToBuild = File::Util::ToLower(categoryToBuild);

    // --- Access the case-insensitive category map via accessor ---
    const auto& caseInsensitiveCategoryMap = File::Vehicle::GetCaseInsensitiveVehicleCategories();
    auto it_ci_category = caseInsensitiveCategoryMap.find(lowerCaseCategoryToBuild);

    if (it_ci_category != caseInsensitiveCategoryMap.end())
    {
        const std::string& actualCategoryKey = it_ci_category->second; // Get the original casing

        // --- Access the main categorized data map via accessor ---
        const auto& categorizedVehicles = File::Vehicle::GetCategorizedVehicles();
        auto it_category_map = categorizedVehicles.find(actualCategoryKey);

        if (it_category_map != categorizedVehicles.end())
        {
            const auto& vehiclesInThisCategory = it_category_map->second;

            for (const auto& pair : vehiclesInThisCategory)
            {
                const std::string& vehicleName = pair.first;
                unsigned int vehicleHash = pair.second;

                if (mbCtx.Option(vehicleName)) {
                    vehicleSpawner.SpawnVehicle(static_cast<VehicleHash>(vehicleHash), true);
                }
            }
        }
        else
        {
            UI::Notify("Error: Internal category map inconsistency!");
            std::cerr << "Internal Error: Category '" << actualCategoryKey << "' found in lookup but not in main data.\n";
        }
    }
    else
    {
        UI::Notify(std::format("Error: Category '{}' not found!", categoryToBuild));
        std::cerr << "Error: Attempted to build menu for unknown category: " << categoryToBuild << "\n";
    }
}
#pragma endregion



#pragma region SportsVehicles

// This is another way to load these on there own without the above:
/// <summary>
/// Sports vehicles sub menu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
//void VehicleMenu::BuildSportsVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
//{
//    mbCtx.Title("Sports");
//
//    // Scripts
//    auto& playerScripts = PlayerScripts::GetInstance();
//    auto& vehicleScripts = VehicleScripts::GetInstance();
//
//    auto& vehicleSpawner = VehicleSpawner::GetInstance();
//
//    std::string categoryName = "SPORT"; // The exact string key in your gta_vehicle_data.h (e.g., "Sports", "SPORTS")
//
//    // Optional: If you want to use case-insensitive lookup for the category name itself:
//    // std::string lowerCaseCategoryName = File::Util::ToLower(categoryName);
//    // auto it_ci_category = File::Vehicle::g_caseInsensitiveCategoryNames.find(lowerCaseCategoryName);
//    // if (it_ci_category == File::Vehicle::g_caseInsensitiveCategoryNames.end()) {
//    //     // Handle error: Category not found (shouldn't happen if data is consistent)
//    //     mbCtx.Text("Error: 'Sports' category data not found!");
//    //     return;
//    // }
//    // const std::string& actualCategoryKey = it_ci_category->second; // Get the original casing from the lookup map
//    // auto it_category_map = GTA_Vehicles::CategorizedVehicleHashes.find(actualCategoryKey);
//
//    // For simplicity, assuming "Sports" is the exact key in gta_vehicle_data.h as per your example.
//    // If your gta_vehicle_data.h uses "SPORTS" (all caps), change "Sports" below to "SPORTS".
//    // Or use the case-insensitive lookup as commented out above.
//    // This works! Now to make this work on all categories without duplicating this code over and over.
//    auto it_category_map = GTA_Vehicles::CategorizedVehicleHashes.find(categoryName);
//
//    if (it_category_map != GTA_Vehicles::CategorizedVehicleHashes.end())
//    {
//        // Get the inner map for Sports vehicles (name -> hash)
//        const auto& sportsVehiclesMap = it_category_map->second;
//
//        // Iterate through the map of Sports vehicles
//        for (const auto& pair : sportsVehiclesMap)
//        {
//            const std::string& vehicleName = pair.first;  // The vehicle name (e.g., "Jester Classic")
//            unsigned int vehicleHash = pair.second;       // The vehicle hash (unsigned int)
//
//            if (mbCtx.Option(vehicleName)) {
//                // Assuming SpawnVehicle can take an unsigned int directly, or VehicleHash is compatible.
//                // If VehicleHash is a distinct enum type, you might need a static_cast:
//                vehicleSpawner.SpawnVehicle(static_cast<VehicleHash>(vehicleHash), true);
//                // Or ideally, modify SpawnVehicle to accept unsigned int:
//                // vehicleSpawner.SpawnVehicle(vehicleHash, true);
//            }
//        }
//    }
//    else
//    {
//        // This case indicates that the "Sports" category was not found in your generated data.
//        // Double-check the exact string "Sports" (or "SPORTS") in your gta_vehicle_data.h
//        //mbCtx.Text("Error: 'Sports' vehicle category data not loaded or found!");
//        UI::Notify("~r~Error:~s Sports category doesn't exist");
//    }
//
//
//}
#pragma endregion


