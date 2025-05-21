#include "pch.h"

#include "../Constants.hpp"

#include "VehicleSpawner.h"

// Scripts
#include "VehicleScripts.h"
#include "PlayerScripts.h"
#include "MiscScripts.h"

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
 * 
 * I adapted this and got it working.
 * I set this up to store the current vehicle to the 'vehicleToSpawn' variable.
 * Now this removes the old vehicle when being run.
 */
void VehicleSpawner::SpawnVehicle(Hash hash) {
    Util util = Util();
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();

    // I had to change playerPed to this, logging to the console showed the coords were indeed wrong.
    Ped playerPed = PLAYER_PED_ID();

    //GET_ENTITY_MODEL()

    //-----
    // Clear the area
    //-----

    Vector3 currentPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
    float clearAreaRadius = 25;

    CLEAR_AREA_OF_VEHICLES(currentPos, clearAreaRadius, true, false, false, false, false, false, false);


    //-----
    // Make sure the model exists and is a vehicle.
    //-----
    if (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash)) {

        //-----
        // Check if the previous spawned vehicle exists
        // Remove it if so.
        //-----
        if (DOES_ENTITY_EXIST(vehicleToSpawn))
        {
            // I fixed this, had to use hash instead of the vehicleToSpawn.
            //MiscScripts::Model::MarkAsNoLongerNeeded(vehicleToSpawn);
            MiscScripts::Model::MarkAsNoLongerNeeded(hash);
            //SET_MODEL_AS_NO_LONGER_NEEDED(vehicleToSpawn);
            DELETE_VEHICLE(&vehicleToSpawn);
        }

        //-----
        // Request the model
        //-----
        //vehicleScripts.RequestModel(hash);
        MiscScripts::Model::Request(hash);

        // TODO Possibly setup ini file for loading the SpawnInside value?
        //spawnInsideVehicle = settings.SpawnInside;
        //if (findStringInNames("trailer", hash) || findStringInNames("train", hash)) {
        //    spawnInside = false;
        //}
        
        //-----
        // Set the position
        //-----
        
        float currentX = currentPos.x + 3;
        float currentY = currentPos.y + 3;
        float currentZ = currentPos.z;
        Vector3 newPos = Vector3(currentX, currentY, currentZ);

        Vector3 pos = newPos;

        float currentHeading = GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());

        //-----
        // If set to spawn inside vehicle and player is in vehicle.
        // Remove the old one.
        //-----
        if (spawnInsideVehicle && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
            Vehicle oldVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
            Vector3 oldVehiclePos = ENTITY::GET_ENTITY_COORDS(playerPed, true);

            oldVehiclePos = ENTITY::GET_ENTITY_COORDS(oldVeh, true);
            ENTITY::SET_ENTITY_AS_MISSION_ENTITY(oldVeh, true, true);
            VEHICLE::DELETE_VEHICLE(&oldVeh);
            pos = oldVehiclePos;
        }

        //-----
        // Create the vehicle
        //-----
        //Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 1, 0);
        //vehicleToSpawn = VEHICLE::CREATE_VEHICLE(hash, pos, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 0, 1, 0);
        vehicleToSpawn = VEHICLE::CREATE_VEHICLE(hash, pos, currentHeading, false, true, false);
        //VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.0f);
        VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicleToSpawn, 5.0f);
        //VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.0f);
        VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicleToSpawn, 0.0f);

        std::string vehicleName = util.GetGxtName(hash);

        //-----
        // Set the player to spawn in the vehicle, turn the engine on.
        //-----
        if (spawnInsideVehicle) {
            //ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
            ENTITY::SET_ENTITY_HEADING(vehicleToSpawn, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
            
            // Also turn the engine on
            //VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
            VEHICLE::SET_VEHICLE_ENGINE_ON(vehicleToSpawn, true, true, false);

            //PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
            PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), vehicleToSpawn, -1);

        }

        // Set vehicle as spawned, I might use this for something
        // For now, it is not in use.
        vehicleSpawned = true;

        WAIT(0);
        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

        // TODO Look into how this is working.
        //if (settings.Persistent) {
        //    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
        //    g_persistentVehicles.push_back(veh);
        //}
        //else {
            //ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, false, true);
            //ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
        //}

        //-----
        // Display the spawned message on the screen and log to console.
        //-----
        //util.ShowSubtitle("Spawned " + util.GetGxtName(hash) + " (" + util.GetModelName(hash) + ")");
        util.ShowSubtitle("Spawned " + vehicleName);

        std::string consoleLogString = std::format("Spawned a {} at coords: X: {}, Y: {}, Z: {}", vehicleName, currentX, currentY, currentZ);
        std::string currentCoordsConsoleLogString = std::format("Current coords: X: {}, Y: {}, Z: {}", currentX, currentY, currentZ);

        log_output(consoleLogString);

        // This was just an extra test
        //log_output(currentCoordsConsoleLogString);
    }
    else {
        //-----
        // Display the error message.
        //-----
        util.ShowSubtitle("Vehicle doesn't exist");

        std::string consoleErrorString = std::format("Error, vehicle with hash {} doesn't exist!", hash);
        log_output(consoleErrorString);
    }
}

//Vehicle VehicleScripts::SpawnVehicle(Hash hash, Vector3 coords, float heading, DWORD timeout) {

/// <summary>
/// Spawn a vehicle at a specified coords and heading
/// It is a more basic function then the other one 
/// that spawns it on the player or puts the player inside.
/// </summary>
/// <param name="hash"></param>
/// <param name="coords"></param>
/// <param name="heading"></param>
/// <returns></returns>
Vehicle VehicleSpawner::SpawnVehicle(Hash hash, Vector3 coords, float heading) {
    auto& vehicleScripts = VehicleScripts::GetInstance();

    // Vehicle doesn't exist
    if (!(STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash))) {
        return 0;
    }

    // I finally fixed the wait function.
    //vehicleScripts.RequestModel(hash);
    MiscScripts::Model::Request(hash);

    //-----
    // Create the vehicle
    //-----
    Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, coords, heading, 0, 1, 0);
    VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.0f);
    WAIT(0);
    STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

    ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, false, true);
    ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);

    return veh;
}
