#include "pch.h"
#include "WorldMenu.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"

#include "Util/FileFunctions.h"

// Chaos mod
#include "Util/EntityIterator.h"
#include "Util/Hash.h"
#include "Util/Random.h"

void WorldMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    mbCtx.Title("World");

    if (mbCtx.Option("Kill all peds", { "Kill all peds in the area" }))
    {
        Ped playerPed = playerScripts.GetPlayerPed();

        //std::list<Entity> entities;
        // Can be used like this in the for loop:
        // entities.push_back(ped);
        for (auto ped : GetAllPeds())
        {
            // Check if they are the player and not dead, if so do nothing
            if (!PED::IS_PED_A_PLAYER(ped) && !ENTITY::IS_ENTITY_DEAD(ped, false))
            {
                ENTITY::SET_ENTITY_HEALTH(ped, 0, 0);
            }

        }

        //entities.push_back(ped);
        //for (auto veh : GetAllVehs())
        //    entities.push_back(veh);
        //for (auto prop : GetAllProps())
        //    entities.push_back(prop);
    }

    if (mbCtx.Option("Blow up vehicles", { "Blow up all vehicles in the area" }))
    {
        Ped playerPed = playerScripts.GetPlayerPed();
        //Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerScripts.GetPlayerID(), false);
        Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

        // Possible fix for this being buggy and crashing, taken from Chaos Mod.
        int count = 3;

        for (Vehicle veh : GetAllVehs())
        {
            if (veh != playerVeh)
            {
                VEHICLE::EXPLODE_VEHICLE(veh, true, false);

                if (--count = 0)
                {
                    count = 3;
                    WAIT(0);
                }
            }

        }

        //entities.push_back(ped);
        //for (auto veh : GetAllVehs())
        //    entities.push_back(veh);
        //for (auto prop : GetAllProps())
        //    entities.push_back(prop);
    }

    // These were in MPMenu, adding to my menu I'll probably just remove MPMenu.
    //if (mbCtx.Option("Load MP Maps", { "Enable Multiplayer maps" }))
    //{
    //    ON_ENTER_MP();
    //}

    //if (mbCtx.Option("Unload MP Maps", { "Disable Multiplayer maps" }))
    //{
    //    ON_ENTER_SP();
    //}


}
