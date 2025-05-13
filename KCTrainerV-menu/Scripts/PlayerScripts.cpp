#include "PlayerScripts.h"

#include "Util/UI.hpp"

#include <inc/natives.h>
#include <format>

// Booleans
bool PlayerScripts::invincibilityEnabled = false;
bool PlayerScripts::neverWantedEnabled = false;

// Ints
int wantedLevel = 0;

/// <summary>
/// Get the players current health as a string.
/// </summary>
/// <returns>The current health as a string.</returns>
std::string PlayerScripts::GetPlayerHealth() {
    int health = ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID());

    return std::format("{}", health);
}

/// <summary>
/// Get the players current vehicle name as a string.
/// </summary>
/// <returns>The current vehicle name as a string, or "No vehicle" if it doesn't exist.</returns>
std::string PlayerScripts::GetPlayerVehicleName() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    if (ENTITY::DOES_ENTITY_EXIST(playerVehicle)) {
        Hash model = ENTITY::GET_ENTITY_MODEL(playerVehicle);
        std::string makeName = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_MAKE_NAME_FROM_VEHICLE_MODEL(model));
        std::string modelName = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(model));
        return std::format("{} {}", makeName, modelName);
    }
    else {
        return "No vehicle";
    }
}

/// <summary>
/// Get the players distance traveled, updated by the main script.
/// </summary>
/// <returns></returns>
std::string PlayerScripts::GetDistanceTraveled() {
    return std::format("{:.0f} meters", mDistance);
}

/// <summary>
/// Check if the player is in a vehicle
/// </summary>
/// <returns>If the player vehicle exists, and the player is in one.</returns>
bool PlayerScripts::IsPlayerInVehicle() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    return ENTITY::DOES_ENTITY_EXIST(playerVehicle);
}


// These get set in the below functions
int player = 0;
int playerID = 0;

/// <summary>
/// Get the player ped
/// </summary>
/// <returns></returns>
int PlayerScripts::GetPlayerPed()
{
    int myPlayer = PLAYER::PLAYER_ID();
    player = PLAYER::GET_PLAYER_PED(myPlayer);
    return player;
}

/// <summary>
/// Get the player ID
/// </summary>
/// <returns></returns>
int PlayerScripts::GetPlayerID()
{
    int myPlayer = PLAYER::PLAYER_ID();
    playerID = myPlayer;
    return playerID;
}

/// <summary>
/// Toggle the player invincibility
/// TODO Setup this to work by toggling the boolean instead of running this function.
/// </summary>
void PlayerScripts::ToggleInvincibility()
{
    PlayerScripts::invincibilityEnabled = !PlayerScripts::invincibilityEnabled;
    if (PlayerScripts::invincibilityEnabled)
    {
        ENTITY::SET_ENTITY_PROOFS(GetPlayerPed(), true, true, true, true, true, true, true, true);
        UI::Notify("Invincibility enabled");
    }
    else
    {
        ENTITY::SET_ENTITY_PROOFS(GetPlayerPed(), false, false, false, false, false, false, false, false);
        UI::Notify("Invincibility disabled");
    }
}

/// <summary>
/// Toggle the players never wanted status.
/// </summary>
void PlayerScripts::ToggleNeverWanted()
{
    PlayerScripts::neverWantedEnabled = !PlayerScripts::neverWantedEnabled;

    if (PlayerScripts::neverWantedEnabled)
    {
        PLAYER::SET_MAX_WANTED_LEVEL(0);

        if (PLAYER::GET_PLAYER_WANTED_LEVEL(GetPlayerID() > 1)) {
            PLAYER::SET_PLAYER_WANTED_LEVEL(GetPlayerID(), 0, false);
            PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(GetPlayerID(), false);
        }
        UI::Notify("Never wanted enabled");
    }
    else {
        PLAYER::SET_MAX_WANTED_LEVEL(5);
        UI::Notify("Never wanted disabled");
    }
}

/// <summary>
/// Set the players wanted level
/// This param isn't in use anymore, I changed the command to
/// get the wanted level from the menu.
/// </summary>
/// <param name="wantedLevel">Wanted level to set to</param>
//void PlayerScripts::SetWantedLevel(int wantedLevel)
void PlayerScripts::SetWantedLevel()
{
    // Make sure the wanted level is valid first
    if (wantedLevel >= 0 || wantedLevel <= 5) 
    {
        PLAYER::SET_PLAYER_WANTED_LEVEL(GetPlayerID(), wantedLevel, false);
        PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(GetPlayerID(), false);
    }
    else {
        UI::Notify("Invalid wanted level.");
    }
}

/// <summary>
/// Warp player to the specified coords, and a heading
/// </summary>
/// <param name="position"></param>
/// <param name="heading"></param>
void PlayerScripts::WarpPlayerToCoords(Vector3 position, float heading)
{
    STREAMING::LOAD_SCENE(position);

    //ENTITY::SET_ENTITY_COORDS(GetPlayerPed(), position.x, position.y, position.z, false, false, false, false);
    ENTITY::SET_ENTITY_COORDS(GetPlayerPed(), position, false, false, false, false);
    ENTITY::SET_ENTITY_HEADING(GetPlayerPed(), heading);
}



/// <summary>
/// Set the player coords, TODO Make a teleport menu.
/// </summary>
void PlayerScripts::SetPlayerCoords(TeleportLocations locationToTeleport)
{
    //if (PLAYER::IS_PLAYER_DEAD)
    //if (!PLAYER::IS_PLAYER_PLAYING(GetPlayerPed()))
    if (!PLAYER::IS_PLAYER_PLAYING(GetPlayerID()))
    {
        return;
    }

    Vector3 teleportCoords = Vector3(0, 0, 0);

    switch (locationToTeleport)
    {
    case AIRPORT_RUNWAY:
        teleportCoords = Vector3(-1336.0f, -3044.0f, 13.9f);
        WarpPlayerToCoords(teleportCoords, 0.0f);
        break;
    }

    //Vector3 airportRunway = Vector3(-1336.0f, -3044.0f, 13.9f);
    /*WarpPlayerToCoords(airportRunway, 0.0f);*/
}