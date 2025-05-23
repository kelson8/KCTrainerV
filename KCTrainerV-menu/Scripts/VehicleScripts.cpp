#include "pch.h"

#include "../Constants.hpp"
#include "VehicleScripts.h"
#include "MiscScripts.h"
#include "PlayerScripts.h"

#include "../Util/Enums.h"

#include "Util/UI.hpp"

// Some parts from GTAVAddonLoader
#include "Util/Util.hpp"

#include "../Natives/natives2.h"

#include <iostream>
#include <format>


/// <summary>
/// Tick event for VehicleScripts
/// </summary>
void VehicleScripts::Tick()
{
    //-----
    // Invincibility toggle
    //-----
    if (VehicleScripts::isInvincibleVehicleEnabled && !VehicleScripts::invincibilityFlag)
    {
        VehicleScripts::EnableInvincibility();
        VehicleScripts::invincibilityFlag = true;
    }
    else if (!VehicleScripts::isInvincibleVehicleEnabled && VehicleScripts::invincibilityFlag)
    {
        VehicleScripts::DisableInvincibility();
        VehicleScripts::invincibilityFlag = false;
    }

    //-----
    // Bullet proof toggle
    // TODO Rename to bullet proof tires, rename flag and toggle also.
    //-----
    if (VehicleScripts::isBulletProofEnabled && !VehicleScripts::bulletProofFlag)
    {
        VehicleScripts::EnableBulletProof();
        VehicleScripts::bulletProofFlag = true;
    }
    else if (!VehicleScripts::isBulletProofEnabled && VehicleScripts::bulletProofFlag)
    {
        VehicleScripts::DisableBulletProof();
        VehicleScripts::bulletProofFlag = false;
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

#pragma region VehicleMods

// TODO Move these into VehicleMods.cpp

/// <summary>
/// This seems to work, tested by going into LS Customs
/// Set the vehicle armor
/// Values should range from 0-3 or 0-4?
/// </summary>
/// <param name="value"></param>
void VehicleScripts::SetArmor(int value)
{
    Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    // Make sure the vehicle exists first
    if (ENTITY::DOES_ENTITY_EXIST(currentVeh))
        //if (currentVeh != 0)
    {
        SET_VEHICLE_MOD(currentVeh, VehicleMod::Armor, value, false);
        std::string armorLevelString = std::format("Set armor level to {}", value);
        UI::Notify(armorLevelString);
    }
}

// TODO Set these up later.

void SetSpoiler()
{

}

void SetWheelType()
{

}

void SetWheels()
{

}

void SetTranismission()
{

}

void SetEngine()
{

}

void SetBrakes()
{

}


#pragma endregion //VehicleMods

#pragma region RepairVehicle


/// <summary>
/// Check if the player is in a vehicle.
/// If the vehicle exists, set it to fixed, set dirt level to 0.
/// Set engine health to 2000, set petrol tank health to 2000.
/// Set vehicle body health to 2000.
/// Clear the vehicle undriveable flag.
/// Set the vehicle engine to be on.
/// Repair the vehicle tires.
/// </summary>
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

        VEHICLE::SET_VEHICLE_UNDRIVEABLE(currentVeh, false);
        VEHICLE::SET_VEHICLE_ENGINE_ON(currentVeh, true, true, false);

        // Fix the tires
        RepairTires();
    }
    else {
        UI::Notify("Not in a vehicle");
    }
}

/// <summary>
/// Fix all the current vehicles tires.
/// </summary>
void VehicleScripts::RepairTires()
{
    auto& playerScripts = PlayerScripts::GetInstance();
    Ped playerPed = playerScripts.GetPlayerPed();

    if (playerScripts.IsPlayerInVehicle())
    {
        Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

        //-----
        // Repair the tires.
        // TODO Setup switch for checking if this is a car
        //-----
        /*if(VEHICLE::IS_VEHICLE_A_CAR)*/

        for (int i = 0; i < 8; i++)
        {
            VEHICLE::SET_VEHICLE_TYRE_FIXED(currentVeh, i);
        }
    }
}

#pragma endregion

#pragma region VehicleProofs

/// <summary>
/// Enable bullet proof tires for the current vehicle
/// Also, this now is set to repair the vehicle tires.
/// Why does this also work for the last vehicle, like if I'm out of it? 
/// My vehicle check function shouldn't check for that.
/// TODO Rename function to EnableBulletProofTires
/// </summary>
void VehicleScripts::EnableBulletProof()
{
    auto& playerScripts = PlayerScripts::GetInstance();
    Ped playerPed = playerScripts.GetPlayerPed();

    if (playerScripts.IsPlayerInVehicle())
    {
        Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
        //SET_ENTITY_PROOFS(currentVeh, true, false, false, false, false, false, false, false);

        //-----
        // Repair the tires.
        //-----
        RepairTires();

        SET_VEHICLE_TYRES_CAN_BURST(currentVeh, false);

        //UI::Notify("Vehicle is now bullet proof");
        UI::Notify("Vehicle tires is now bullet proof");
    }
}

/// <summary>
/// Disable bullet proof tires for the current vehicle
/// Why does this also work for the last vehicle, like if I'm out of it? 
/// My vehicle check function shouldn't check for that.
/// TODO Rename function to DisableBulletProofTires
/// </summary>
void VehicleScripts::DisableBulletProof()
{
    auto& playerScripts = PlayerScripts::GetInstance();
    Ped playerPed = playerScripts.GetPlayerPed();

    if (playerScripts.IsPlayerInVehicle())
    {
        Vehicle currentVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
        
        //SET_VEHICLE_MOD(currentVeh, VehicleMod::BackWheels, )
        SET_VEHICLE_TYRES_CAN_BURST(currentVeh, true);
        //SET_ENTITY_PROOFS(currentVeh, false, false, false, false, false, false, false, false);
        //UI::Notify("Vehicle is no longer bullet proof");
        UI::Notify("Vehicle tires is no longer bullet proof");
    }
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

#pragma endregion // VehicleProofs

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

// I have a list of hashes in Enums.h for the train models.
//void VehicleScripts::CreateMissionTrain(Hash model, Vector3 pos, bool direction
#ifdef TRAIN_TEST
void VehicleScripts::CreateMissionTrain(Hash model, bool direction)
{
    Ped playerPedId = PLAYER_PED_ID();
    Vector3 trainStationCoords = Vector3(626.68f, 6442.31f, 30.88f);
    float trainStationHeading = -177.0f;

    Vector3 trainPos = Vector3(613.0f, 6438.0f, 31.0f);

    std::string trainCreatedMessage = std::format("Train model '{}' created at X: {}, Y: {}, Z:{}.", model, trainPos.x, trainPos.y, trainPos.z);

#pragma region TrainHashKeys
    Hash freight = "freight"_hash;
    Hash freightCar = "freightcar"_hash;
    Hash freightGrain = "freightgrain"_hash;
    Hash freightCont1 = "freightcont1"_hash;
    Hash freightCont2 = "freightcont2"_hash;

    Hash tankerCar = "tankercar"_hash;
    Hash metroTrain = "metrotrain"_hash;
#pragma endregion

    DWORD startTime = GetTickCount();
    DWORD timeout = 3000; // in millis

    //-----
    // First, delete all trains
    //-----
    DELETE_ALL_TRAINS();
    // This can also be toggled if needed
    //SET_RANDOM_TRAINS(false);

    
    //-----
    // Set the player position
    //-----
    SET_ENTITY_COORDS(playerPedId, trainStationCoords, false, false, false, false);
    SET_ENTITY_HEADING(playerPedId, trainStationHeading);

    //-----
    // Request the model
    // I finally fixed the wait function.
    //-----
    //VehicleScripts::RequestModel(model);

    MiscScripts::Model::Request(model);
    // Request train models
    //MiscScripts::Model::Request(freight);
    //MiscScripts::Model::Request(freightCar);
    //MiscScripts::Model::Request(freightGrain);
    //MiscScripts::Model::Request(freightCont1);
    //MiscScripts::Model::Request(freightCont2);
    //
    //MiscScripts::Model::Request(tankerCar);
    //MiscScripts::Model::Request(metroTrain);


    //-----
    // Create the train
    //-----
    //if (HAS_MODEL_LOADED(model))
    //{
        missionTrain = CREATE_MISSION_TRAIN(model, trainPos, direction, 0, 1);
    //}
    

    //-----
    // Log the train created message and display it
    //-----
    UI::Notify(trainCreatedMessage);
    log_output(trainCreatedMessage);

}

#endif // TRAIN_TEST

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
