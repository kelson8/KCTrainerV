#include "pch.h"
#include "../Constants.hpp"
#include "VehicleMenu.h"

#include "../Scripts/Extras/Game.h"

#include "../Util/UI.hpp"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/VehicleScripts.h"
#include "../Scripts/VehicleSpawner.h"

#include "Scripts/Vehicles/VehicleData.h"

void VehicleMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Vehicle");

    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    //--------------
    // Vehicle options sub menu
    //---------------
    mbCtx.MenuOption("Options", "VehicleOptionsSubmenu", { "Show a list of vehicle options" });


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
            vehicleSpawner.SpawnVehicle(vehicleHash);
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
    mbCtx.Title("Categories");

    auto& vehicleScripts = VehicleScripts::GetInstance();

    mbCtx.MenuOption("Sports", "SportsVehicleCategorySubmenu");
    mbCtx.MenuOption("Sports Classics", "SportsClassicsVehicleCategorySubmenu");
    mbCtx.MenuOption("Super", "SuperVehicleCategorySubmenu");

    // TODO Set these up
    // 
    //mbCtx.MenuOption("Coupes", "CoupesVehicleCategorySubmenu");
    //mbCtx.MenuOption("Muscle", "MuscleVehicleCategorySubmenu");
    //mbCtx.MenuOption("Offroad", "OffroadVehicleCategorySubmenu");
    
    mbCtx.MenuOption("SUV's", "SuvVehicleCategorySubmenu");
    
    // TODO Set these up.
    //mbCtx.MenuOption("Sedans", "SedansVehicleCategorySubmenu");
    //mbCtx.MenuOption("Compacts", "CompactsVehicleCategorySubmenu");
    
    // Add more categories here
}
#pragma endregion

#pragma region SportsVehicles
/// <summary>
/// Sports vehicles sub menu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void VehicleMenu::BuildSportsVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Sports");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();

    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    for (const auto& vehicleInfo : VehicleData::g_sportsVehicles) 
    {
        if (mbCtx.Option(vehicleInfo.name)) {
            vehicleSpawner.SpawnVehicle(vehicleInfo.hashEnum);
        }
    }
}
#pragma endregion

#pragma region SportsClassicVehicles
void VehicleMenu::BuildSportsClassicsVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Sports Classics");

    auto& vehicleSpawner = VehicleSpawner::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();

    for (const auto& vehicleInfo : VehicleData::g_sportsClassicsVehicles) 
    {
        if (mbCtx.Option(vehicleInfo.name)) {
            vehicleSpawner.SpawnVehicle(vehicleInfo.hashEnum);
        }
    }
}

#pragma endregion

#pragma region SuperVehicles
/// <summary>
/// Super vehicles sub menu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void VehicleMenu::BuildSuperVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Super");

    auto& vehicleSpawner = VehicleSpawner::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();

    for (const auto& vehicleInfo : VehicleData::g_superVehicles) 
    {
        if (mbCtx.Option(vehicleInfo.name)) {
            // Use the hash from the VehicleInfo struct
            vehicleSpawner.SpawnVehicle(vehicleInfo.hashEnum);
        }
    }
}
#pragma endregion

#pragma region SuvVehicles
/// <summary>
/// SUV category sub menu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void VehicleMenu::BuildSuvCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("SUV's");

    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    for (const auto& vehicleInfo : VehicleData::g_suvVehicles) 
    {
        if (mbCtx.Option(vehicleInfo.name)) {
            // Use the hash from the VehicleInfo struct
            vehicleSpawner.SpawnVehicle(vehicleInfo.hashEnum);
        }
    }

}

#pragma endregion //SuvVehicles
