#include "pch.h"
#include "VehicleMenu.h"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/VehicleScripts.h"

void VehicleMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Vehicle");

    auto& vehicleScripts = VehicleScripts::GetInstance();

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
    mbCtx.MenuOption("Categories", "VehicleCategorySubmenu", { "Show the list of vehicle categories." });
}

#pragma region VehicleCategories
void VehicleMenu::BuildVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& vehicleScripts = VehicleScripts::GetInstance();
    mbCtx.Title("Categories");

    mbCtx.MenuOption("Sports", "SportsVehicleCategorySubmenu");
    mbCtx.MenuOption("Super", "SuperVehicleCategorySubmenu");
    // Add more categories here
}
#pragma endregion

#pragma region SportsVehicles
void VehicleMenu::BuildSportsVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& vehicleScripts = VehicleScripts::GetInstance();
    mbCtx.Title("Sports");

    std::map<std::string, std::function<void()>> optionActions =
    {
        {"Spawn Cheetah", [&]()
        {
            static const Hash cheetahHash = "Cheetah"_hash;
            vehicleScripts.SpawnVehicle(cheetahHash);
        }},
        // Add more super vehicles
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
    //    static const Hash cheetahHash = "Cheetah"_hash;
    //    vehicleScripts.SpawnVehicle(cheetahHash);
    //}
    // Add more sports vehicles
}
#pragma endregion

#pragma region SuperVehicles
void VehicleMenu::BuildSuperVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& vehicleScripts = VehicleScripts::GetInstance();
    mbCtx.Title("Super");

    std::map<std::string, std::function<void()>> optionActions =
    {
        {"Spawn Scramjet", [&]()
        {
            static const Hash scramJetHash = "Scramjet"_hash;
            vehicleScripts.SpawnVehicle(scramJetHash);
        }},
        // Add more super vehicles
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

