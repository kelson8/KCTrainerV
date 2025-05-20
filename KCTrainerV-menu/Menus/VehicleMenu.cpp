#include "pch.h"
#include "VehicleMenu.h"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/VehicleScripts.h"

void VehicleMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Vehicle");

#ifdef VEHICLE_SCRIPTS_SINGLETON
    auto& vehicleScripts = VehicleScripts::GetInstance();
#else
    VehicleScripts vehicleScripts = VehicleScripts();
#endif

    // This didn't seem to do anything
    //mbCtx.drawInstructionalButtons();

    mbCtx.BoolOption("Toggle Invincible vehicle", vehicleScripts.isInvincibleVehicleEnabled, { "Toggle invincibility for your current vehicle" });

    if (mbCtx.Option("Repair vehicle", { "Fully fix your vehicle and tires." }))
    {
        vehicleScripts.RepairVehicle();
    }

    // Toggles the boolean to spawn in the vehicle and remove the old one.
    mbCtx.BoolOption("Spawn into vehicle", vehicleScripts.spawnInsideVehicle, { "Toggle spawning in the spawned vehicle" });

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

    // Moved vehicle spawning code into the categories.
    // TODO Fix vehicle spawners to work again.
    mbCtx.MenuOption("Categories", "VehicleCategorySubmenu", { "Show the list of vehicle categories." });
}

#pragma region VehicleCategories
void VehicleMenu::BuildVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
#ifdef VEHICLE_SCRIPTS_SINGLETON
    auto& vehicleScripts = VehicleScripts::GetInstance();
#else
    VehicleScripts vehicleScripts = VehicleScripts();
#endif
    mbCtx.Title("Categories");

    mbCtx.MenuOption("Sports", "SportsVehicleCategorySubmenu");
    mbCtx.MenuOption("Super", "SuperVehicleCategorySubmenu");
    // Add more categories here
}
#pragma endregion

#pragma region SportsVehicles
void VehicleMenu::BuildSportsVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
#ifdef VEHICLE_SCRIPTS_SINGLETON
    auto& vehicleScripts = VehicleScripts::GetInstance();
#else
    VehicleScripts vehicleScripts = VehicleScripts();
#endif

    Vector3 playerPos = playerScripts.GetPlayerCoords();
    float playerX = playerPos.x + 3;
    float playerY = playerPos.y + 3;
    float playerZ = playerPos.z;
    Vector3 playerPosNew = Vector3(playerX, playerY, playerZ);
    float playerHeading = playerScripts.GetPlayerHeading();

    mbCtx.Title("Sports");

    std::map<std::string, std::function<void()>> optionActions =
    {
        {"Spawn Cheetah", [&]()
        {
        //UI::Notify("Spawn Cheetah menu item selected!");
        //std::cout << "[Menu Debug] Spawn Cheetah menu item selected!" << std::endl;
        static const Hash cheetahHash = MISC::GET_HASH_KEY("cheetah");
        //UI::Notify(std::format("Attempting to spawn cheetah with hash: {}", cheetahHash).c_str());
        std::cout << "[Menu Debug] Attempting to spawn cheetah with hash: " << cheetahHash << std::endl;

        Vector3 playerPos = PlayerScripts::GetInstance().GetPlayerCoords();
        float playerX = playerPos.x + 3;
        float playerY = playerPos.y + 3;
        float playerZ = playerPos.z + 3;

        //Vector3 playerPosNew = PlayerScripts::GetInstance().GetPlayerCoords() + Vector3(3.0f, 3.0f, 0.0f);
        float playerHeading = PlayerScripts::GetInstance().GetPlayerHeading();
        //UI::Notify(std::format("Spawning at: X={}, Y={}, Z={}, H={}", playerPosNew.x, playerPosNew.y, playerPosNew.z, playerHeading).c_str());
        std::cout << "[Menu Debug] Spawning at: X=" << playerPosNew.x << ", Y=" << playerPosNew.y << ", Z=" << playerPosNew.z << ", H=" << playerHeading << std::endl;
        //vehicleScripts.SpawnVehicle(cheetahHash, playerPosNew, playerHeading);
        vehicleScripts.SpawnVehicle(cheetahHash);
        //UI::Notify("SpawnVehicle function called.");
        std::cout << "[Menu Debug] SpawnVehicle function called." << std::endl;

    }},

        //{"Spawn Cheetah", [&]()
        //{
        //    //static const Hash cheetahHash = "cheetah"_hash;
        //    static const Hash cheetahHash = MISC::GET_HASH_KEY("cheetah");
        //    std::cout << std::format("Cheetah hash: {}", cheetahHash) << std::endl;
        //    vehicleScripts.SpawnVehicle(cheetahHash, playerPosNew, playerHeading);
        //    //vehicleScripts.SpawnVehicle(cheetahHash);
        //}},
        // Add more sports vehicles
    };

    for (const auto& pair : optionActions)
    {
        if (mbCtx.Option(pair.first))
        {
            pair.second();
        }
    }

    //if (mbCtx.Option("Spawn Cheetah", { "Spawn a Cheetah near you" }))
    //{
    //    //static const Hash cheetahHash = "Cheetah"_hash;
    //    //static const Hash cheetahHash = "cheetah"_hash;
    //    static const Hash cheetahHash = MISC::GET_HASH_KEY("cheetah");
    //    vehicleScripts.SpawnVehicle(cheetahHash);
    //}
    // Add more sports vehicles
}
#pragma endregion

#pragma region SuperVehicles
void VehicleMenu::BuildSuperVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
#ifdef VEHICLE_SCRIPTS_SINGLETON
    auto& vehicleScripts = VehicleScripts::GetInstance();
#else
    VehicleScripts vehicleScripts = VehicleScripts();
#endif
    mbCtx.Title("Super");

    //std::map<std::string, std::function<void()>> optionActions =
    //{
    //    {"Spawn Scramjet", [&]()
    //    {
    //        static const Hash scramJetHash = "scramjet"_hash;
    //        vehicleScripts.SpawnVehicle(scramJetHash);
    //    }},
    //    // Add more super vehicles
    //};

    //for (const auto& pair : optionActions)
    //{
    //    if (mbCtx.Option(pair.first))
    //    {
    //        pair.second();
    //    }
    //}

    if (mbCtx.Option("Scramjet"))
    {
        //static const Hash scramJetHash = "scramjet"_hash;
        static const Hash scramJetHash = MISC::GET_HASH_KEY("scramjet");
        // Well the baller3 works?
        //static const Hash scramJetHash = MISC::GET_HASH_KEY("baller3");
        vehicleScripts.SpawnVehicle(scramJetHash);
    }

    // TODO Refactor this if it is working right, move everything into the for loop.

    if (mbCtx.Option("T20"))
    {
        //static const Hash t20Hash = MISC::GET_HASH_KEY("t20");
        static const Hash t20Hash = VEHICLE_T20;
        
        vehicleScripts.SpawnVehicle(t20Hash);
    }

    if (mbCtx.Option("Zentorno"))
    {
        //static const Hash zentornoHash = VEHICLE_ZENTORNO;
        static const Hash zentornoHash = MISC::GET_HASH_KEY("zentorno");

        vehicleScripts.SpawnVehicle(zentornoHash);
    }

    if (mbCtx.Option("Baller3"))
    {
        // Well the baller3 works?
        static const Hash baller3Hash = MISC::GET_HASH_KEY("baller3");
        vehicleScripts.SpawnVehicle(baller3Hash);
    }
}
#pragma endregion

