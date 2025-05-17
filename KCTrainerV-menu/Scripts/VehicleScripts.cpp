#include "VehicleScripts.h"

#include "PlayerScripts.h"

#include "Util/UI.hpp"

#include <format>

// Some parts from GTAVAddonLoader
#include "Util/Util.hpp"

#include "../Natives/natives2.h"


/// <summary>
/// Get the players current vehicle.
/// TODO Fix this, it doesn't work in some of my other functions.
/// </summary>
/// <returns></returns>
Vehicle VehicleScripts::GetPlayerVehicle()
{
    auto& playerScripts = PlayerScripts::GetInstance();
    Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerScripts.GetPlayerID(), false);

    return playerVeh;

    // Check if the player is in a vehicle and it exists
    //if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(playerScripts.GetPlayerID()) && ENTITY::DOES_ENTITY_EXIST(playerVeh))
    //{
    //    return playerVeh;
    //}

    //return 0;
    
}

void VehicleScripts::ToggleBombBayDoors()
{
    Player player = PLAYER::PLAYER_PED_ID();
    // I forgot about GET_ENTITY_MODEL
    // https://www.unknowncheats.me/forum/grand-theft-auto-v/144819-grand-theft-auto-scripting-125.html
    Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(player, false);
    Hash playerVehHash = ENTITY::GET_ENTITY_MODEL(playerVeh);

    if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && VEHICLE::IS_THIS_MODEL_A_PLANE(playerVehHash))
    {
        if (VEHICLE::GET_ARE_BOMB_BAY_DOORS_OPEN(playerVeh)) {
            VEHICLE::OPEN_BOMB_BAY_DOORS(playerVeh);
        }
        else
        {
            VEHICLE::CLOSE_BOMB_BAY_DOORS(playerVeh);
        }
    }

}

void VehicleScripts::ChangePlayerVehicleColor() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    if (ENTITY::DOES_ENTITY_EXIST(playerVehicle)) {
        VEHICLE::SET_VEHICLE_COLOUR_COMBINATION(playerVehicle,
            MISC::GET_RANDOM_INT_IN_RANGE(0,
                VEHICLE::GET_NUMBER_OF_VEHICLE_COLOURS(playerVehicle)
            )
        );
    }
}

void VehicleScripts::RepairVehicle()
{

    // TODO Fix this.
    //Vehicle currentVeh = GetPlayerVehicle();
    // This works
    Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    // Make sure the vehicle exists first
    if(ENTITY::DOES_ENTITY_EXIST(currentVeh))
    //if (currentVeh != 0)
    {
        // Copied some of these from GTAcehicle.cpp in Menyoo
        VEHICLE::SET_VEHICLE_FIXED(currentVeh);
        VEHICLE::SET_VEHICLE_DIRT_LEVEL(currentVeh, 0.f);

        // TODO What does this do?
        // VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE

        // Set vehicle health
        VEHICLE::SET_VEHICLE_ENGINE_HEALTH(currentVeh, 2000.0F);
        VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(currentVeh, 2000.0f);
        VEHICLE::SET_VEHICLE_BODY_HEALTH(currentVeh, 2000.0F);

        VEHICLE::SET_VEHICLE_UNDRIVEABLE(currentVeh, 0);
        VEHICLE::SET_VEHICLE_ENGINE_ON(currentVeh, true, true, false);

        // Fix the tires

        // TODO Setup switch for checking if this is a car
        for (int i = 0; i < 8; i++)
        {
            VEHICLE::SET_VEHICLE_TYRE_FIXED(currentVeh, i);
        }

        /*if(VEHICLE::IS_VEHICLE_A_CAR)*/
    }
    else {
        UI::Notify("Not in a vehicle");
    }
}

// Taken from GTAVAddonLoader
// https://github.com/ikt32/GTAVAddonLoader/blob/master/GTAVAddonLoader/script.cpp


// TODO Possibly setup settings for this using mINI
// settings.Persistent

/*
 * Spawns a vehicle with the chosen model hash. Put it on the player when not
 * already in a vehicle, and puts it to the right when a vehicle is already
 * occupied. Bounding-box dependent, so spawning two jumbojets should have
 * clearance for non-explodiness, and two bikes are spaced without too much
 * distance between 'em.
 */
void VehicleScripts::SpawnVehicle(Hash hash) {
    Util util;
    if (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash)) {
        Ped playerPed = PLAYER::PLAYER_PED_ID();
        STREAMING::REQUEST_MODEL(hash);
        DWORD startTime = GetTickCount();
        DWORD timeout = 3000; // in millis

        while (!STREAMING::HAS_MODEL_LOADED(hash)) {
            WAIT(0);
            if (GetTickCount() > startTime + timeout) {
                UI::Notify("Couldn't load model");

                util.ShowSubtitle("Couldn't load model");
                //Util::ShowSubtitle("Couldn't load model");
                WAIT(0);
                STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
                return;
            }
        }

        // TODO Possibly setup ini file for loading the SpawnInside value?
        //spawnInsideVehicle = settings.SpawnInside;
        //if (findStringInNames("trailer", hash) || findStringInNames("train", hash)) {
        //    spawnInside = false;
        //}

        float offsetX = 0.0f;
        if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) || !spawnInsideVehicle) {
            Vehicle oldVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
            Hash oldHash = ENTITY::GET_ENTITY_MODEL(oldVeh);
            Vector3 newMin, newMax;
            Vector3 oldMin, oldMax;
            MISC::GET_MODEL_DIMENSIONS(hash, &newMin, &newMax);
            MISC::GET_MODEL_DIMENSIONS(oldHash, &oldMin, &oldMax);
            if (!ENTITY::DOES_ENTITY_EXIST(oldVeh)) {
                oldMax.x = oldMin.x = 0.0f;
            }
            // to the right
            // width + margin + width again 
            offsetX = ((newMax.x - newMin.x) / 2.0f) + 1.0f + ((oldMax.x - oldMin.x) / 2.0f);
        }

        Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, { offsetX, 0.0f, 0.0f });

        //if (spawnInside && settings.SpawnInplace && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
        if (spawnInsideVehicle && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
            Vehicle oldVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
            Vector3 oldVehiclePos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
            oldVehiclePos = ENTITY::GET_ENTITY_COORDS(oldVeh, true);
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(oldVeh, true, true);
            VEHICLE::DELETE_VEHICLE(&oldVeh);
            pos = oldVehiclePos;
        }


        Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 1, 0);
        VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.0f);
        VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.0f);

        if (spawnInsideVehicle) {
            ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
            // Also turn the engine on
            VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
            PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);

        }

        WAIT(0);
        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

        //if (settings.Persistent) {
        //    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
        //    g_persistentVehicles.push_back(veh);
        //}
        //else {
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, false, true);
            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
        //}

        //util.ShowSubtitle("Spawned " + util.GetGxtName(hash) + " (" + util.GetModelName(hash) + ")");
        util.ShowSubtitle("Spawned " + util.GetGxtName(hash));
    }
    else {
        util.ShowSubtitle("Vehicle doesn't exist");
    }
}

Vehicle VehicleScripts::SpawnVehicle(Hash hash, Vector3 coords, float heading, DWORD timeout) {
    if (!(STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash))) {
        // Vehicle doesn't exist
        return 0;
    }
    STREAMING::REQUEST_MODEL(hash);
    DWORD startTime = GetTickCount();

    while (!STREAMING::HAS_MODEL_LOADED(hash)) {
        WAIT(0);
        if (GetTickCount() > startTime + timeout) {
            // Couldn't load model
            WAIT(0);
            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
            return 0;
        }
    }

    Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, coords, heading, 0, 1, 0);
    VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.0f);
    WAIT(0);
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, false, true);
    ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);

    return veh;
}

/// <summary>
/// Enable invincibility for the current vehicle
/// </summary>
void VehicleScripts::EnableInvincibility()
{
    auto& playerScripts = PlayerScripts::GetInstance();
    Ped playerPed = playerScripts.GetPlayerPed();

    if (playerScripts.IsPlayerInVehicle())
    {
        Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
        SET_ENTITY_PROOFS(currentVeh, true, true, true, true, true, true, true, true);
        UI::Notify("Vehicle is now invincible");
    }
}

/// <summary>
/// Disable invincibility for the current vehicle
/// </summary>
void VehicleScripts::DisableInvincibility()
{
    auto& playerScripts = PlayerScripts::GetInstance();
    Ped playerPed = playerScripts.GetPlayerPed();

    if (playerScripts.IsPlayerInVehicle())
    {
        Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
        SET_ENTITY_PROOFS(currentVeh, false, false, false, false, false, false, false, false);
        UI::Notify("Vehicle is no longer invincible");
    }
}