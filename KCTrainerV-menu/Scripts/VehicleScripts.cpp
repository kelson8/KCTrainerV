#include "pch.h"

#include "../Constants.hpp"
#include "VehicleScripts.h"

#include "PlayerScripts.h"
#include "../Util/Enums.h"

#include "Util/UI.hpp"

// Some parts from GTAVAddonLoader
#include "Util/Util.hpp"

#include "../Natives/natives2.h"

#include <iostream>
#include <format>

/// <summary>
/// Requesting a vehicle model for use in the game spawning functions.
/// This function will timeout after 3 seconds to prevent crashing.
/// 
/// Usage: Make a vehicle spawner do nothing if this is false.
/// if(!VehicleScripts::RequestModel(model) return;
/// </summary>
/// <param name="model"></param>
//void VehicleScripts::RequestModel(Hash model)
//bool VehicleScripts::RequestModel(Hash model)
//{
//    WAIT(0);
//    Util util;
//
//    DWORD startTime = GetTickCount();
//    //DWORD timeout = 3000; // in millis
//    DWORD timeout = 10; // in millis
//
//    REQUEST_MODEL(model);
//    while (!HAS_MODEL_LOADED(model))
//    {
//        WAIT(50);
//
//        // Add crash protection to this, seems to work for the vehicle and other items.
//        if (GetTickCount() > startTime + timeout) {
//            // Couldn't load model
//            //WAIT(0);
//            UI::Notify("Couldn't load model");
//
//            util.ShowSubtitle("Couldn't load model");
//
//            std::cout << std::format("Couldn't load model: {}", model) << std::endl;
//
//            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
//            return false;
//        }
//    }
//
//    // Should only ever return true if this was a success
//    return true;
//}

// Taken from Menyoo
// This seems to have mostly fixed the vehicle spawning, 
// I still need to fix it to place it above ground, and not fall through the map.
// This does lag a little bit but seems to work.
bool VehicleScripts::RequestModel(Hash hash)
{
    int timeOut = 3000;
    if (HAS_MODEL_LOADED(hash)) return true;
    else
    {
        REQUEST_MODEL(hash);

        for (timeOut += GetTickCount(); GetTickCount() < timeOut;)
        {
            if (HAS_MODEL_LOADED(hash))
                return true;
            WAIT(0);
        }
        return false;
    }
}

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
    Util util = Util();
    auto& playerScripts = PlayerScripts::GetInstance();
    //WAIT(100);
    

    //if (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash)) {
        //Ped playerPed = PLAYER::PLAYER_PED_ID();
        //Ped playerPed = playerScripts.GetPlayerPed();
        Ped playerPed = playerScripts.GetPlayerID();
        
        // This function seems to work for this.
        // Make this do nothing if the model isn't loaded
        if (!VehicleScripts::RequestModel(hash))
        {
            return;
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
        //STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

        //if (settings.Persistent) {
        //    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
        //    g_persistentVehicles.push_back(veh);
        //}
        //else {
            //ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, false, true);
            //ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
        //}

        //util.ShowSubtitle("Spawned " + util.GetGxtName(hash) + " (" + util.GetModelName(hash) + ")");
        util.ShowSubtitle("Spawned " + util.GetGxtName(hash));
    //}
    //else {
    //    util.ShowSubtitle("Vehicle doesn't exist");
    //}
}

//Vehicle VehicleScripts::SpawnVehicle(Hash hash, Vector3 coords, float heading, DWORD timeout) {
Vehicle VehicleScripts::SpawnVehicle(Hash hash, Vector3 coords, float heading) {
    // Vehicle doesn't exist
    if (!(STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash))) {
        return 0;
    }
    
    // This function seems to work for this.
    // Make this do nothing if the model isn't loaded

    // TODO Test in this function, I mostly use the other one but this one might work.
    if (!VehicleScripts::RequestModel(hash))
    {
        return 0;
    }

    //STREAMING::REQUEST_MODEL(hash);
    
    //DWORD startTime = GetTickCount();

    //while (!STREAMING::HAS_MODEL_LOADED(hash)) {
    //    WAIT(0);
    //    if (GetTickCount() > startTime + timeout) {
    //        // Couldn't load model
    //        WAIT(0);
    //        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
    //        return 0;
    //    }
    //}

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

// TODO Fix this to work, I might experiment with this in FiveM
// TODO Figure out which direction the bool makes you face.
    // Models can be one of these:
    // I made an enum to possibly use later: TrainModels
    /*
        freight
        freightcar
        freightgrain
        freightcont1
        freightcont2
        freighttrailer
        tankercar"
        metrotrain
        "s_m_m_lsmetro_01
    */

// I have a list of hashses in Enums.h for the train models.
void VehicleScripts::CreateMissionTrain(Hash model, Vector3 pos, bool direction)
{
    //Util util;

    DWORD startTime = GetTickCount();
    DWORD timeout = 3000; // in millis

    // First, delete all trains
    DELETE_ALL_TRAINS();
    // This can also be toggled if needed
    //SET_RANDOM_TRAINS(false);

    // Request the model, if it doesn't load do nothing.
    if (!VehicleScripts::RequestModel(model))
    {
        return;
    }


    /*
        while (!STREAMING::HAS_MODEL_LOADED(hash)) {
        WAIT(0);
        if (GetTickCount() > startTime + timeout) {
            // Couldn't load model
            WAIT(0);
            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
            return 0;
        }
    }
    */

    missionTrain = CREATE_MISSION_TRAIN(model, pos, direction, 0, 1);

}

/// <summary>
/// Will be in use for the ped id gun.
/// Get the name of the vehicle that is an entity.
/// </summary>
/// <param name="vehicle"></param>
/// <returns>The name of the vehicle like this: "Vehicle Name: Cheetah", or returns blank if nothing found.</returns>
std::string VehicleScripts::GetVehicleName(Entity vehicle)
{
    // Make sure the entity exists and is a vehicle.
    if (ENTITY::DOES_ENTITY_EXIST(vehicle) && IS_ENTITY_A_VEHICLE(vehicle))
    {
        Hash model = ENTITY::GET_ENTITY_MODEL(vehicle);
        std::string makeName = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_MAKE_NAME_FROM_VEHICLE_MODEL(model));
        std::string modelName = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));

        std::string vehicleNameString = std::format("Vehicle Name: {}", modelName);
        return vehicleNameString;
    }

    return "";
}