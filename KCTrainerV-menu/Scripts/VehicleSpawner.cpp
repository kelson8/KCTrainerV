#include "pch.h"

#include "../Constants.hpp"

#include "VehicleSpawner.h"

#include "VehicleScripts.h"

#include "PlayerScripts.h"
#include "../Util/Enums.h"

#include "Util/UI.hpp"

// Some parts from GTAVAddonLoader
#include "Util/Util.hpp"

#include "../Natives/natives2.h"

#include <iostream>
#include <format>


#ifdef VEHICLE_SPAWNER_TEST
// Define the global instance (if declared in header)
VehicleSpawner g_vehicleSpawner;

// Constructor implementation
VehicleSpawner::VehicleSpawner()
    : m_currentSpawnState(VehicleSpawnState::Idle), // Initialize state
    m_modelToSpawnHash(0),
    m_spawnCoords(0.0f, 0.0f, 0.0f), // Assuming Vector3 has a constructor
    m_spawnHeading(0.0f),
    m_lastSpawnedVehicle(0)
{
    // Any other initial setup
}


// TODO Test this, for spawning vehicles
// InitiateSpawn method implementation
void VehicleSpawner::InitiateSpawn(const std::string& modelName, const Vector3& coords, float heading) {
    if (m_currentSpawnState == VehicleSpawnState::Idle) { // Only start if not already busy
        m_modelToSpawnHash = MISC::GET_HASH_KEY(modelName.c_str());
        m_spawnCoords = coords;
        m_spawnHeading = heading;
        m_currentSpawnState = VehicleSpawnState::RequestingModel; // Move to the first stage
        m_lastSpawnedVehicle = 0; // Clear previous handle

        UI::Notify("Vehicle spawn initiated..."); // Provide immediate feedback
    }
    else {
        UI::Notify("Vehicle spawner is busy. Please wait.");
    }
}

void VehicleSpawner::VehicleSpawnTick() {
    switch (m_currentSpawnState) {
    case VehicleSpawnState::Idle:
        // Do nothing
        break;

    case VehicleSpawnState::RequestingModel: // This state initiates the request and checks
        STREAMING::REQUEST_MODEL(m_modelToSpawnHash); // Request the model
        if (STREAMING::HAS_MODEL_LOADED(m_modelToSpawnHash)) {
            m_currentSpawnState = VehicleSpawnState::CreatingVehicle; // If loaded, move to next state
        }
        // *** IMPORTANT: If not loaded, we simply FALL THROUGH and EXIT this function.
        // The main KCMenu::Thread2() loop will call VehicleSpawnTick() again next frame.
        // And KCMenu::Thread2() has the WAIT(0) that yields. ***
        break;

    case VehicleSpawnState::CreatingVehicle:

        // ... (rest of your vehicle creation logic)
        break;

    case VehicleSpawnState::SpawnComplete:
        // ...
        break;
    }
}

#endif // VEHICLE_SPAWNER_TEST


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
void VehicleSpawner::SpawnVehicle(Hash hash) {
    Util util = Util();
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();
    //WAIT(100);


    //if (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash)) {
        //Ped playerPed = PLAYER::PLAYER_PED_ID();
        //Ped playerPed = playerScripts.GetPlayerPed();
    Ped playerPed = playerScripts.GetPlayerID();

    // This function seems to work for this.
    // Make this do nothing if the model isn't loaded
    //if (!VehicleScripts::RequestModel(hash))
    if (!vehicleScripts.RequestModel(hash))
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
    //if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) || !vehicleScripts.spawnInsideVehicle) {
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
    //if (vehicleScripts.spawnInsideVehicle && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
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
    //if (vehicleScripts.spawnInsideVehicle) {
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
Vehicle VehicleSpawner::SpawnVehicle(Hash hash, Vector3 coords, float heading) {
    auto& vehicleScripts = VehicleScripts::GetInstance();

    // Vehicle doesn't exist
    if (!(STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash))) {
        return 0;
    }

    // This function seems to work for this.
    // Make this do nothing if the model isn't loaded

    // TODO Test in this function, I mostly use the other one but this one might work.
    //if (!VehicleScripts::RequestModel(hash))
    if (!vehicleScripts.RequestModel(hash))
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
