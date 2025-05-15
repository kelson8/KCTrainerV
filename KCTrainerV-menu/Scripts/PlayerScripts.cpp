#include "PlayerScripts.h"

#include "Util/UI.hpp"
#include "Util/Logger.hpp"

#include <inc/natives.h>
#include <format>

// TODO Test this, seems to build with this enabled.
// From Menyoo, should make it to where I don't have to type the namespaces for the natives.
#include "Natives/natives2.h"

// Booleans

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

#pragma region PlayerPositions
/// <summary>
/// Get the players current coords
/// </summary>
/// <returns>Players current coords as a vector3</returns>
Vector3 PlayerScripts::GetPlayerCoords()
{
    Vector3 playerCoords = GET_ENTITY_COORDS(GetPlayerPed(), false);
    return playerCoords;
}

/// <summary>
/// Get the players heading
/// </summary>
/// <returns>The players heading as a float</returns>
float PlayerScripts::GetPlayerHeading()
{
    float playerHeading = GET_ENTITY_HEADING(GetPlayerPed());
    return playerHeading;
}

/// <summary>
/// Teleport player to the specified coords, and a heading
/// </summary>
/// <param name="position"></param>
void PlayerScripts::SetPlayerCoords(Vector3 position)
{
    STREAMING::LOAD_SCENE(position);

    ENTITY::SET_ENTITY_COORDS(GetPlayerPed(), position, false, false, false, false);
}

/// <summary>
/// Set the players heading.
/// </summary>
/// <param name="heading"></param>
void PlayerScripts::SetPlayerHeading(float heading)
{
    ENTITY::SET_ENTITY_HEADING(GetPlayerPed(), heading);
}

#pragma endregion

/// <summary>
/// Toggle the player invincibility
/// TODO Setup this to work by toggling the boolean instead of running this function.
/// </summary>
void PlayerScripts::ToggleInvincibility()
{
    this->invincibilityEnabled = !this->invincibilityEnabled;
    if (this->invincibilityEnabled)
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

// Alternative functions for never wanted, runs in the game tick if the flag is enabled.
void PlayerScripts::EnableNeverWanted()
{
    // TODO Possibly remove these two below from the loop that'll run
    PLAYER::SET_PLAYER_WANTED_LEVEL(GetPlayerID(), 0, false);
    PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(GetPlayerID(), false);

    SET_MAX_WANTED_LEVEL(0);
    SET_WANTED_LEVEL_MULTIPLIER(0.0f);
}


void PlayerScripts::DisableNeverWanted()
{
    SET_MAX_WANTED_LEVEL(6);
    SET_WANTED_LEVEL_MULTIPLIER(1.0f);
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
/// Turn on/off heat vision.
/// </summary>
/// <param name="toggle"></param>
//void SetHeatVision(bool toggle)
void PlayerScripts::ToggleHeatVision()
{
    //GRAPHICS::SET_SEETHROUGH(toggle);
    heatVisionToggled = !heatVisionToggled;
    GRAPHICS::SET_SEETHROUGH(heatVisionToggled);
}

/// <summary>
/// Turn on/off night vision.
/// </summary>
/// <param name="toggle"></param>
//void SetHeatVision(bool toggle)
void PlayerScripts::ToggleNightVision()
{
    nightVisionToggled = !nightVisionToggled;
    GRAPHICS::SET_NIGHTVISION(nightVisionToggled);
}

/// <summary>
/// Teleport to the location in the TeleportLocation enum within TeleportLocations.h
/// </summary>
/// <param name="locationToTeleport"></param>
void PlayerScripts::TeleportToLocation(TeleportLocation locationToTeleport) {
    auto& teleportLocations = TeleportLocations::GetInstance();
    //const Vector3& teleportCoords = teleportLocations.GetTeleportLocationInfo(locationToTeleport);
    const TeleportInfo teleportInfo = teleportLocations.GetTeleportLocationInfo(locationToTeleport);
    //SetPlayerCoords(teleportCoords);
    SetPlayerCoords(teleportInfo.coordinates);
    SetPlayerHeading(teleportInfo.heading);
}


/// <summary>
/// Set the player coords
/// </summary>
void PlayerScripts::WarpToLocation(TeleportLocation locationToTeleport)
{
    if (!PLAYER::IS_PLAYER_PLAYING(GetPlayerID()))
    {
        return;
    }

    // Initalize the teleport locations vector, required for my new teleport method.
    //InitializeTeleportLocations(); // Make sure to call this somewhere before the switch

    // Locations are now defined in the TeleportLocations.cpp file.
    TeleportToLocation(locationToTeleport);

}

/// <summary>
/// TODO Set this up
/// Also, compact these, possibly make a for loop?
/// Credit to this link for the stats: https://gtaforums.com/topic/802525-changing-stats/
/// Full stat list: https://vespura.com/fivem/gta-stats/
/// </summary>
void SetStats()
{
    // Useful stats:
    /*
        CARS_COPS_EXPLODED
        CARS_EXPLODED
        COPS_KILLS_ON_SPREE

        KILLS
        KILLS_ARMED
        KILLS_BY_OTHERS
        KILLS_COP
        KILLS_ENEMY_GANG_MEMBERS
        KILLS_FRIENDLY_GANG_MEMBERS
        KILLS_IN_FREE_AIM
        KILLS_INNOCENTS - Number of peds killed
        KILLS_STEALTH
        KILLS_SWAT

    */
    // SP0 = Franklin
    // SP1 = Micheal
    // SP2 = Trevor
    /*
    //MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(GAMEPLAY::GET_HASH_KEY("stats_controller"));
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_SPECIAL_ABILITY_UNLOCKED"), 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STAMINA"),                  100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STEALTH_ABILITY"),          100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_LUNG_CAPACITY"),            100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_FLYING_ABILITY"),           100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_SHOOTING_ABILITY"),         100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STRENGTH"),                 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_WHEELIE_ABILITY"),          100, true);

    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_SPECIAL_ABILITY_UNLOCKED"), 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_STAMINA"),                  100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_STEALTH_ABILITY"),          100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_LUNG_CAPACITY"),            100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_FLYING_ABILITY"),           100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_SHOOTING_ABILITY"),         100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_STRENGTH"),                 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_WHEELIE_ABILITY"),          100, true);

    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_SPECIAL_ABILITY_UNLOCKED"), 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_STAMINA"),                  100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_STEALTH_ABILITY"),          100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_LUNG_CAPACITY"),            100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_FLYING_ABILITY"),           100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_SHOOTING_ABILITY"),         100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_STRENGTH"),                 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_WHEELIE_ABILITY"),          100, true);

    */
}