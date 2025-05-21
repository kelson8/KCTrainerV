#include "pch.h"
#include "../Constants.hpp"
#include "VehicleMenu.h"

#include "../Scripts/Extras/Game.h"

#include "../Util/UI.hpp"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/VehicleScripts.h"
#include "../Scripts/VehicleSpawner.h"

void VehicleMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Vehicle");

    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    // This didn't seem to do anything
    //mbCtx.drawInstructionalButtons();

    mbCtx.BoolOption("Toggle Invincible vehicle", vehicleScripts.isInvincibleVehicleEnabled, { "Toggle invincibility for your current vehicle" });

    if (mbCtx.Option("Repair vehicle", { "Fully fix your vehicle and tires." }))
    {
        vehicleScripts.RepairVehicle();
    }

    //-----
    // Bullet proof tires toggle
    //-----
    mbCtx.BoolOption("Bulletproof tires", vehicleScripts.isBulletProofEnabled, { "Set vehicle to have bulletproof tires" });


    //-----
    // Vehicle armor options
    //-----
    mbCtx.IntOption("Vehicle Armor Level", vehicleScripts.vehicleArmorLevel, 0, 4, 1, { "Armor level selector" });
    if (mbCtx.Option("Set armor"))
    {
        vehicleScripts.SetArmor(vehicleScripts.vehicleArmorLevel);
    }

    // Toggles the boolean to spawn in the vehicle and remove the old one.
    //mbCtx.BoolOption("Spawn into vehicle", vehicleScripts.spawnInsideVehicle, { "Toggle spawning in the spawned vehicle" });
    mbCtx.BoolOption("Spawn into vehicle", vehicleSpawner.spawnInsideVehicle, { "Toggle spawning in the spawned vehicle" });

    // This crashes it, I'll try to fix it later.
#ifdef _TEST
    if (mbCtx.Option("Create Test Train", {"Create a train as a test at Train Station #1 warp."}))
    {
        Hash trainHash = "freight"_hash;
        float trainX = 247.9364f;
        float trainY = -1198.597f;
        float trainZ = 37.4482f;
        Vector3 trainPos = Vector3(trainX, trainY, trainZ);

        //vehicleScripts.CreateMissionTrain(0, trainPos, true);
        vehicleScripts.CreateMissionTrain(trainHash, trainPos, true);
    }
#endif //_TEST

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
    mbCtx.MenuOption("Categories", "VehicleCategorySubmenu", { "Show the list of vehicle categories." });    
}

#pragma region VehicleCategories
void VehicleMenu::BuildVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Categories");

    auto& vehicleScripts = VehicleScripts::GetInstance();

    mbCtx.MenuOption("Sports", "SportsVehicleCategorySubmenu");
    mbCtx.MenuOption("Super", "SuperVehicleCategorySubmenu");

    // TODO Set these up
    // mbCtx.MenuOption("Sports Classics", "SportsClassicsVehicleCategorySubmenu");
    //mbCtx.MenuOption("Coupes", "SportsClassicsVehicleCategorySubmenu");
    //mbCtx.MenuOption("Muscle", "SportsClassicsVehicleCategorySubmenu");
    //mbCtx.MenuOption("Offroad", "SportsClassicsVehicleCategorySubmenu");
    
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

#pragma region SportsVehicleHashes
    static const Hash cheetahHash = "cheetah"_hash;
#pragma endregion

    // Player position, mostly for testing the SpawnVehicle option with custom coords like this:
    // vehicleSpawner.SpawnVehicle(cheetahHash, playerPosNew, playerHeading);
    Vector3 playerPos = playerScripts.GetPlayerCoords();
    float playerX = playerPos.x + 3;
    float playerY = playerPos.y + 3;
    float playerZ = playerPos.z;
    Vector3 playerPosNew = Vector3(playerX, playerY, playerZ);
    float playerHeading = playerScripts.GetPlayerHeading();

    std::map<std::string, std::function<void()>> optionActions =
    {
        {"Spawn Cheetah", [&]()
        {
            //vehicleSpawner.SpawnVehicle(cheetahHash, playerPosNew, playerHeading);
            vehicleSpawner.SpawnVehicle(cheetahHash);
        }},
    };

    for (const auto& pair : optionActions)
    {
        if (mbCtx.Option(pair.first))
        {
            pair.second();
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

#pragma region SuperVehicleHashes
    static const Hash scramJetHash = "scramjet"_hash;
    static const Hash t20Hash = "t20"_hash;
    static const Hash zentornoHash = "zentorno"_hash;
#pragma endregion

    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& vehicleSpawner = VehicleSpawner::GetInstance();

    std::map<std::string, std::function<void()>> optionActions =
    {
        {"Scramjet", [&]()
        {
            vehicleSpawner.SpawnVehicle(scramJetHash);
        }},
        {"T20", [&]()
        {
            vehicleSpawner.SpawnVehicle(t20Hash);
        }},
        {"Zentorno", [&]()
        {
             vehicleSpawner.SpawnVehicle(zentornoHash);
        }},
    };

    for (const auto& pair : optionActions)
    {
        if (mbCtx.Option(pair.first))
        {
            pair.second();
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

#pragma region SuvVehicleHashes
    static const Hash ballerLeHash = "baller3"_hash;
#pragma endregion

    std::map<std::string, std::function<void()>> optionActions =
    {
        {"Baller LE", [&]()
        {
            vehicleSpawner.SpawnVehicle(ballerLeHash);
        }},
    };

    for (const auto& pair : optionActions)
    {
        if (mbCtx.Option(pair.first))
        {
            pair.second();
        }
    }
}

#pragma endregion //SuvVehicles
