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

VehicleSpawner::VehicleSpawner() 
    : m_vehicleToSpawn(0),
    m_jetToSpawn(0)
{

}


// Taken from GTAVAddonLoader
// https://github.com/ikt32/GTAVAddonLoader/blob/master/GTAVAddonLoader/script.cpp


// TODO Possibly setup settings for this using mINI
// settings.Persistent

/// <summary>
/// Spawns a vehicle with the chosen model hash.
/// This removes the old vehicle when being run.
/// Can optionally clear the area.
/// </summary>
/// <param name="hash">The model hash</param>
/// <param name="clearArea">If the area gets cleared</param>
void VehicleSpawner::SpawnVehicle(Hash hash, bool clearArea) {
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

    // Make clearing the area toggleable in the function.
    if (clearArea)
    {
        CLEAR_AREA_OF_VEHICLES(currentPos, clearAreaRadius, true, false, false, false, false, false, false);
    }
    


    //-----
    // Make sure the model exists and is a vehicle.
    //-----
    if (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash)) {

        //-----
        // Check if the previous spawned vehicle exists
        // Remove it if so.
        //-----
        if (DOES_ENTITY_EXIST(m_vehicleToSpawn))
        {
            // I fixed this, had to use hash instead of the vehicleToSpawn.
            //MiscScripts::Model::MarkAsNoLongerNeeded(vehicleToSpawn);
            MiscScripts::Model::MarkAsNoLongerNeeded(hash);
            //SET_MODEL_AS_NO_LONGER_NEEDED(vehicleToSpawn);
            DELETE_VEHICLE(&m_vehicleToSpawn);
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
        m_vehicleToSpawn = VEHICLE::CREATE_VEHICLE(hash, pos, currentHeading, false, true, false);
        //VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 5.0f);
        VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(m_vehicleToSpawn, 5.0f);
        //VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.0f);
        VEHICLE::SET_VEHICLE_DIRT_LEVEL(m_vehicleToSpawn, 0.0f);

        std::string vehicleName = util.GetGxtName(hash);

        //-----
        // Set the player to spawn in the vehicle, turn the engine on.
        //-----
        if (spawnInsideVehicle) {
            //ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
            ENTITY::SET_ENTITY_HEADING(m_vehicleToSpawn, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
            
            // Also turn the engine on
            //VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
            VEHICLE::SET_VEHICLE_ENGINE_ON(m_vehicleToSpawn, true, true, false);

            //PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
            PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), m_vehicleToSpawn, -1);

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
    //auto& vehicleScripts = VehicleScripts::GetInstance();

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

/// <summary>
/// Spawn the player into a P-996 Lazer jet in the air at speed.
/// TODO Fix this, it seems to work well, but when the jet gets set in the air it faces the ground.
/// Also, make an option somewhere that two enemy jets chase me and try to blow me up.
/// </summary>
void VehicleSpawner::SpawnJetInAir()
{
    Util util = Util();
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();

    // P-996 Lazer
    Hash jetHash = VEHICLE_LAZER;

    //-----
    // Make sure the model exists and is a vehicle.
    // If not break out of the function.
    //-----
    if (!STREAMING::IS_MODEL_IN_CDIMAGE(jetHash) && !STREAMING::IS_MODEL_A_VEHICLE(jetHash))
    {
        UI::Notify("Model doesn't exist!");
        return;
    }

    // I had to change playerPed to this, logging to the console showed the coords were indeed wrong.
    Ped playerPed = PLAYER_PED_ID();

    Vector3 currentPos = ENTITY::GET_ENTITY_COORDS(playerPed, true);

    // TODO Switch to using this
    // This function should convert miles per hour and there is also another function that converts kilometers per hour
    // to meters per second.
    //float jetSpeed = Util::MphToMeters(30.0f);
    float jetSpeed = 30.0f;

    // Middle of Los Santos
    Vector3 skyPos = Vector3(169.737f, -515.997f, 801.751f);
    float skyHeading = 27.335f;

    // Hmm, what exactly is the velocity?
    // Not sure what this does
    //Vector3 jetVelocity = Vector3(0.0f, -50.0f, 0.0f);

    int startTick = GET_GAME_TIMER();
    int timeToWait = 2000;
    
    //-----
    // Request the model
    //-----
    MiscScripts::Model::Request(jetHash);

    //-----
    // Create the jet
    //-----
    m_jetToSpawn = VEHICLE::CREATE_VEHICLE(jetHash, skyPos, skyHeading, false, true, false);
    
    //-----
    // Setup the message and wait a few seconds
    //------
    util.ShowSubtitle("Be prepared to fly!");

    // Works as a basic timer for waiting a bit, in milliseconds.
    // Although it does disable the menu input for that amount of time.
    while (GET_GAME_TIMER() - startTick < timeToWait)
    {
    // Just make this wait and do nothing.
    WAIT(0);
    }

    // Make sure the jet exists
    if (DOES_ENTITY_EXIST(m_jetToSpawn))
    {
        // Turn the engine on
        SET_VEHICLE_ENGINE_ON(m_jetToSpawn, true, true, false);

        // Check if the landing gear is closed, probably not needed
        //if (GET_LANDING_GEAR_STATE(m_jetToSpawn) == 3)
        //{
        // Make the landing gear go up
        // This works! I didn't know this one was a native.
        CONTROL_LANDING_GEAR(m_jetToSpawn, static_cast<int>(LandingGearStates::CLOSING));
        //}

        // I'm not exactly sure what this is doing.
        // Trying to make the plane up right instead of flying down at the start.
        //SET_ENTITY_VELOCITY(m_jetToSpawn, jetVelocity);


        // Put the player in the jet
        PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), m_jetToSpawn, -1);

        // Make sure the jet is going at speed
        SET_VEHICLE_FORWARD_SPEED(m_jetToSpawn, jetSpeed);

        // Mark the model as no longer needed
        STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(jetHash);

    }
}
