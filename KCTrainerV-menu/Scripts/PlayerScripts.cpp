#include "pch.h"

#include "Constants.hpp"

#include "PlayerScripts.h"
#include "TextScripts.h"

#include "Util/UI.hpp"
#include "Util/Logger.hpp"

#include <inc/natives.h>
#include <format>

#include "../Util/Hash.h"
#include "../Util/Util.hpp"

// Chaos Mod
#include "Util/EntityIterator.h"
#include "Util/Random.h"


// Menyoo
// Enable the new animation test for the MP Suicide animation using Menyoo.
// Currently it gives a linker error and is disabled
//#define NEW_TEST

#include "GTAped.h"
#include "GTAentity.h"
#include "GTAvehicle.h"
#include "Tasks.h"

//void FadeScreenIn(int ms);
//void FadeScreenOut(int ms);

// I will be moving some player scripts into the Scripts/Player folder 
// so I don't make this file super massive and hard to go through

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

#pragma region PlayerTickEvents

/// <summary>
/// Run the tick event for player related items.
/// </summary>
void PlayerScripts::Tick()
{
    // If the player has died, run this
    // TODO Set this up.
    //if (!PlayerScripts::IsPlayerAlive() && PlayerScripts::playerAliveFlag)
    //{
        // playerAliveFlag = false;
    //} else {
        // playerAliveFlag = true;
    //

    // Respawn locations, TODO add these to a different file.
    //Vector3 michealsHouseCoords = Vector3(-813.603f, 179.474f, 72.1548f);
    //float  michealsHouseHeading = 0.0f;

    //if (PLAYER::IS_PLAYER_DEAD(playerScripts.GetPlayerID()))
    //{
    //    // Well this works.. Just spams the console lol.
    //    // At least I know this is firing off, now how do I change the respawn locations?
    //    //std::cout << "Player is dead";
    //    worldScripts.SetRespawnLocation(michealsHouseCoords, michealsHouseHeading);
    //}

    if (PlayerScripts::invincibilityEnabled && !PlayerScripts::invincibilityFlag)
    {
        ENTITY::SET_ENTITY_PROOFS(PlayerScripts::GetPlayerPed(), true, true, true, true, true, true, true, true);
        UI::Notify("Invincibility enabled");
        PlayerScripts::invincibilityFlag = true;
    }
    else if (!PlayerScripts::invincibilityEnabled && PlayerScripts::invincibilityFlag) {
        ENTITY::SET_ENTITY_PROOFS(PlayerScripts::GetPlayerPed(), false, false, false, false, false, false, false, false);
        UI::Notify("Invincibility disabled");
        PlayerScripts::invincibilityFlag = false;
    }

    if (PlayerScripts::neverWantedEnabled)
    {
        PlayerScripts::EnableNeverWanted();
        PlayerScripts::neverWantedFlag = true;
    }

    //-----
    // This should only run once, the above needs to be in a loop
    //-----
    else if (!PlayerScripts::neverWantedEnabled && PlayerScripts::neverWantedFlag)
    {
        PlayerScripts::DisableNeverWanted();
        // This should stop this from constantly running
        // I added a debug notify line here to test that.
        UI::Notify("Never wanted off.");
        PlayerScripts::neverWantedFlag = false;
    }

    //-----
    // Mobile radio toggle
    //-----
    if (PlayerScripts::isMobileRadioEnabled && !PlayerScripts::mobileRadioFlag)
    {
        PlayerScripts::EnableMobileRadio();
        PlayerScripts::mobileRadioFlag = true;
    }
    else if (!PlayerScripts::isMobileRadioEnabled && PlayerScripts::mobileRadioFlag)
    {
        PlayerScripts::DisableMobileRadio();
        PlayerScripts::mobileRadioFlag = false;
    }


    //-----
    // Disable player controls when teleporting
    // Got the idea for putting this in a loop from here:
    // https://forum.cfx.re/t/help-disable-control/43052
    // Well this didn't work, I'll leave it here for later.
    //-----
    //if (PlayerScripts::fading)
    //{
    //    PlayerScripts::DisableControls();
    //}

    //-----
    // Set explosive bullets
    //-----
    if (PlayerScripts::isExplosiveBulletsEnabled)
    {
        SET_EXPLOSIVE_AMMO_THIS_FRAME(PLAYER_ID());
    }

    //-----
    // Set explosive melee
    //-----
    if (PlayerScripts::isExplosiveMeleeEnabled)
    {
        SET_EXPLOSIVE_MELEE_THIS_FRAME(PLAYER_ID());
    }

    //-----
    // Set infinite ammo
    //-----
    if (PlayerScripts::isInfiniteAmmoEnabled)
    {
        SET_PED_INFINITE_AMMO_CLIP(PLAYER_PED_ID(), true);
    }

    //-----
    // Set super jump
    //-----
    if (PlayerScripts::isSuperJumpEnabled)
    {
        SET_SUPER_JUMP_THIS_FRAME(PLAYER_ID());
    }


    //-----
    // Set super run
    // Function for this adapted from Menyoo.
    //-----
    if (PlayerScripts::isSuperRunEnabled)
    {
        PlayerScripts::SetSuperRun();
    }

    //-----
    // If the player is shooting
    //-----
    if (IS_PED_SHOOTING(PLAYER_PED_ID()))
    {
        //-----
        // Slow down time scale to 0.2 if enabled.
        // TODO Fix this, it cannot be turned back off.
        //-----
        //if (PlayerScripts::isSlowAimEnabled && !PlayerScripts::slowAimFlag)
        //{
        //    SET_TIME_SCALE(0.2f);
        //    PlayerScripts::slowAimFlag = true;
        //}
    }

    // Oops, this should always be able to turn off.
    //if (!PlayerScripts::isSlowAimEnabled && PlayerScripts::slowAimFlag)
    //{
    //    // Set back to default time scale.
    //    SET_TIME_SCALE(1.0f);
    //    PlayerScripts::slowAimFlag = false;
    //}

}
#pragma endregion

#pragma region ControlActions

/// <summary>
/// Disable most player controls
/// </summary>
void PlayerScripts::DisableControls()
{
    int maxControls = 360;
    // Taken from here: https://nativedb.dotindustries.dev/gta5/natives/0xFE99B66D079CF6BC?search=DISABLE_CONTROL_ACTION
    // Control list: https://docs.fivem.net/docs/game-references/controls/
    // Buttons are labeled with keyboard and xbox controller labels.

    PAD::DISABLE_CONTROL_ACTION(0, 21, 1);      // INPUT_SPRINT / Left Shift / A
    PAD::DISABLE_CONTROL_ACTION(0, 37, 1);      // INPUT_SELECT_WEAPON / TAB / LB
    PAD::DISABLE_CONTROL_ACTION(0, 25, 1);      // INPUT_AIM / Right Mouse Button / LT
    PAD::DISABLE_CONTROL_ACTION(0, 141, 1);     // INPUT_MELEE_ATTACK_HEAVY / Q (PC) / A (Xbox)
    PAD::DISABLE_CONTROL_ACTION(0, 140, 1);     // INPUT_MELEE_ATTACK_LIGHT / R (PC) / B (Xbox)
    PAD::DISABLE_CONTROL_ACTION(0, 24, 1);      // INPUT_ATTACK / Left Mouse Button / RT
    PAD::DISABLE_CONTROL_ACTION(0, 257, 1);     // INPUT_ATTACK2 / Left Mouse Button / RT
    PAD::DISABLE_CONTROL_ACTION(0, 22, 1);      // INPUT_JUMP / Spacebar / X
    PAD::DISABLE_CONTROL_ACTION(0, 23, 1);      // INPUT_ENTER / F (PC) / Y (Xbox)

    // Movement controls
    PAD::DISABLE_CONTROL_ACTION(0, 32, 1);      // INPUT_MOVE_UP_ONLY / W / Left Stick
    PAD::DISABLE_CONTROL_ACTION(0, 33, 1);      // INPUT_MOVE_DOWN_ONLY / S / Left Stick
    PAD::DISABLE_CONTROL_ACTION(0, 34, 1);      // INPUT_MOVE_LEFT_ONLY / A / Left Stick
    PAD::DISABLE_CONTROL_ACTION(0, 35, 1);      // INPUT_MOVE_RIGHT_ONLY / D / Left Stick

    // This didn't work either.
    //for (int i = 0; i < maxControls; i++)
    //{
    //    PAD::DISABLE_CONTROL_ACTION(0, i, 1);
    //}

}

/// <summary>
/// Enable most player controls
/// </summary>
void PlayerScripts::EnableControls()
{
    // Taken from here: https://nativedb.dotindustries.dev/gta5/natives/0xFE99B66D079CF6BC?search=DISABLE_CONTROL_ACTION
    // Control list: https://docs.fivem.net/docs/game-references/controls/
    // Buttons are labeled with keyboard and xbox controller labels.

    PAD::ENABLE_CONTROL_ACTION(0, 21, 1);      // INPUT_SPRINT / Left Shift / A
    PAD::ENABLE_CONTROL_ACTION(0, 37, 1);      // INPUT_SELECT_WEAPON / TAB / LB
    PAD::ENABLE_CONTROL_ACTION(0, 25, 1);      // INPUT_AIM / Right Mouse Button / LT
    PAD::ENABLE_CONTROL_ACTION(0, 141, 1);     // INPUT_MELEE_ATTACK_HEAVY / Q (PC) / A (Xbox)
    PAD::ENABLE_CONTROL_ACTION(0, 140, 1);     // INPUT_MELEE_ATTACK_LIGHT / R (PC) / B (Xbox)
    PAD::ENABLE_CONTROL_ACTION(0, 24, 1);      // INPUT_ATTACK / Left Mouse Button / RT
    PAD::ENABLE_CONTROL_ACTION(0, 257, 1);     // INPUT_ATTACK2 / Left Mouse Button / RT
    PAD::ENABLE_CONTROL_ACTION(0, 22, 1);      // INPUT_JUMP / Spacebar / X
    PAD::ENABLE_CONTROL_ACTION(0, 23, 1);      // INPUT_ENTER / F (PC) / Y (Xbox)

    // Movement controls
    PAD::ENABLE_CONTROL_ACTION(0, 32, 1);      // INPUT_MOVE_UP_ONLY / W / Left Stick
    PAD::ENABLE_CONTROL_ACTION(0, 33, 1);      // INPUT_MOVE_DOWN_ONLY / S / Left Stick
    PAD::ENABLE_CONTROL_ACTION(0, 34, 1);      // INPUT_MOVE_LEFT_ONLY / A / Left Stick
    PAD::ENABLE_CONTROL_ACTION(0, 35, 1);      // INPUT_MOVE_RIGHT_ONLY / D / Left Stick
}


#pragma endregion


#pragma region PlayerPedAndChar

/// <summary>
/// Get the player ped
/// </summary>
/// <returns></returns>
Ped PlayerScripts::GetPlayerPed()
{
    int myPlayer = PLAYER::PLAYER_ID();
    Ped player = PLAYER::GET_PLAYER_PED(myPlayer);
    return player;
}

/// <summary>
/// Get the player ID
/// TODO Change this from int to Ped
/// </summary>
/// <returns></returns>
int PlayerScripts::GetPlayerID()
{
    int myPlayer = PLAYER::PLAYER_ID();
    int playerID = myPlayer;
    return playerID;
}

/// <summary>
/// This should get the stat string for the statName, this converts it into a hash.
/// </summary>
/// <param name="character">The character from the PlayerModels enum.</param>
/// <param name="statName">The name of the stat, such as 'KILLS_COP'</param>
/// <returns></returns>
int PlayerScripts::GetPlayerStat(PlayerModels character, const char* statName) {
    int statValue = 0;

    std::string statString;

    switch (character) {
    case PlayerModels::MICHEAL:
        statString = "SP0_" + std::string(statName);
        break;
    case PlayerModels::FRANKLIN:
        statString = "SP1_" + std::string(statName);
        break;
    case PlayerModels::TREVOR:
        statString = "SP2_" + std::string(statName);
        break;
    default:
        statValue = 0;
        return statValue;
    }

    STAT_GET_INT(MISC::GET_HASH_KEY(statString.c_str()), &statValue, -1);
    return statValue;
}


/// <summary>
/// Get the current player model of which character you have selected.
/// Sets it to Micheal, Franklin, or Trevor
/// This works!
/// </summary>
/// <returns>The current player model that is playing, checks between Micheal, Franklin, and Trevor.</returns>
PlayerModels PlayerScripts::GetCurrentPlayerModel()
{
    // Get the game's player ID for the current player
    Player playerId = PLAYER::PLAYER_ID();

    // Get the ped index (handle) of the player's character
    Ped playerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(playerId);

    // Compare the ped index with known models
    Hash michaelModel = "player_zero"_hash;
    Hash franklinModel = "player_one"_hash;
    Hash trevorModel = "player_two"_hash;

    if (ENTITY::GET_ENTITY_MODEL(playerPed) == michaelModel)
    {
        return PlayerModels::MICHEAL;
    }
    else if (ENTITY::GET_ENTITY_MODEL(playerPed) == franklinModel)
    {
        return PlayerModels::FRANKLIN;
    }
    else if (ENTITY::GET_ENTITY_MODEL(playerPed) == trevorModel)
    {
        return PlayerModels::TREVOR;
    }
    else
    {
        // Set to micheal by default.
        return PlayerModels::MICHEAL;
    }
}

#pragma endregion

#pragma region HealthArmorAndKilling

// TODO Test these functions under the pragma region HealthArmorAndKilling, they are all untested.

/// <summary>
/// Get the players current health
/// </summary>
/// <param name="player"></param>
/// <returns>The current health as an int</returns>
int PlayerScripts::GetPlayerHealth(Ped player)
{
    int currentPlayerHealth = GET_ENTITY_HEALTH(player);
    return currentPlayerHealth;
}

/// <summary>
/// Get the players current armor
/// </summary>
/// <param name="player"></param>
/// <returns>The current armor as an int</returns>
int PlayerScripts::GetPlayerArmor(Ped player)
{
    int currentPlayerArmor = GET_PED_ARMOUR(player);
    return currentPlayerArmor;
}

/// <summary>
/// Set the players health to a specific value
/// </summary>
/// <param name="player"></param>
/// <param name="health"></param>
void PlayerScripts::SetPlayerHealth(Ped player, int health)
{
    SET_ENTITY_HEALTH(player, health, 0);
}

void PlayerScripts::SetPlayerArmor(Ped player, int armor)
{
    SET_PED_ARMOUR(player, armor);
}

/// <summary>
/// Set the player health and armor to max.
/// </summary>
/// <param name="player"></param>
void PlayerScripts::HealPlayer(Ped player)
{
    int playerMaxHealth = GET_ENTITY_MAX_HEALTH(player);
    int playerMaxArmor = GET_PLAYER_MAX_ARMOUR(player);
    
    SET_ENTITY_HEALTH(player, playerMaxHealth, 0);
    // This doesn't seem to run.
    SET_PED_ARMOUR(player, playerMaxArmor);
}

/// <summary>
/// This works now.
/// Taken from PlayerSuicide.cpp in Chaos Mod, I didn't know this was an animation
/// </summary>
void PlayerScripts::KillPlayerMP()
{
    DWORD startTime = GetTickCount();
    DWORD timeout = 3000; // in millis

    std::string animDict = "mp_suicide";
    //std::string animName = "mp_suicide";
    std::string animName = "pistol";

#ifndef NEW_TEST
    Ped playerPed = PLAYER_PED_ID();
    //Util util;
#else
    GTAped playerPed = PLAYER_PED_ID();
    if(!playerPed.IsInVehicle() && playerPed.IsOnFoot())
#endif // !NEW_TEST

#ifndef NEW_TEST
    if (!IS_PED_IN_ANY_VEHICLE(playerPed, false) && IS_PED_ON_FOOT(playerPed)
        && GET_PED_PARACHUTE_STATE(playerPed) == -1)
    {
#endif // !NEW_TEST

#ifdef NEW_TEST
            if (!playerPed.Task().IsPlayingAnimation(animDict, animName))
                playerPed.Task().PlayAnimation(animDict, animName, 1, 1, -1, AnimFlag::UpperBodySecondTask, 0, false);
#endif // NEW_TEST

#ifndef NEW_TEST

        REQUEST_ANIM_DICT("mp_suicide");
        // TODO Make a function to wait on animations and play them
        // Possibly make a AnimScripts.cpp?
        while (!HAS_ANIM_DICT_LOADED("mp_suicide")) {
            WAIT(0);
        }



#endif // !NEW_TEST

        // Give the player a pistol
        Hash pistolHash = "WEAPON_PISTOL"_hash;

#ifdef NEW_TEST

        // TODO Fix this, Task().ShootAt gives a linker error, I am including the Task.h
        playerPed.Weapon_set(pistolHash);

        WAIT(750);
        //playerPed.Task();
        // Hmm, not sure how to fix this unresolved linker error, works in MiscScripts..
        // Well I think it's just this and not the whole Task class being affected.
        playerPed.Task().ShootAt(Vector3(0.0f, 0.0f, 0.0f), 10, 0);
        // Set the players health to 0.
        playerPed.Health_set(0);

#endif // NEW_TEST

#ifndef NEW_TEST
        GIVE_WEAPON_TO_PED(playerPed, pistolHash, 9999, true, true);
        
        // Play the MP Suicide animation
        TASK_PLAY_ANIM(playerPed, "mp_suicide", "pistol", 8.0f, -1.0f, 800, 1, 0.f, false, false, false);
        //
        WAIT(750);
        //SET_PED_SHOOTS_AT_COORD(playerPed, 0, 0, 0, true);
        // Make the player shoot
        SET_PED_SHOOTS_AT_COORD(playerPed, Vector3(0.0f, 0.0f, 0.0f), true);
        REMOVE_ANIM_DICT("mp_suicide");
    }

    // Set the players health to 0.
    SET_ENTITY_HEALTH(playerPed, 0, 0);
#endif // !NEW_TEST
}

#pragma endregion // HealthArmorAndKilling


#pragma region FadeFunctions

/// <summary>
/// Fade the screen out
/// </summary>
/// <param name="ms"></param>
void PlayerScripts::FadeScreenOut(int fadeTime)
{
    Ped playerPed = PLAYER_PED_ID();
    // Enable this to disable player control, incomplete.
    fading = true;

    //if (!IS_SCREEN_FADED_OUT())
    //{
    // This didn't work
    //DISABLE_ALL_CONTROL_ACTIONS(0);
    
    FREEZE_ENTITY_POSITION(playerPed, true);
    DO_SCREEN_FADE_OUT(fadeTime);
    WAIT(fadeTime);

    //}
}

/// <summary>
/// Fade the screen in
/// </summary>
/// <param name="ms"></param>
void PlayerScripts::FadeScreenIn(int fadeTime)
{
    Ped playerPed = PLAYER_PED_ID();
    //if (!IS_SCREEN_FADED_IN())
    //{

    // Disable this to re-enable player control, incomplete.
    fading = false;

    // This didn't work
    //ENABLE_ALL_CONTROL_ACTIONS(0);

    FREEZE_ENTITY_POSITION(playerPed, false);
    DO_SCREEN_FADE_IN(fadeTime);
    WAIT(fadeTime);
    //}
}

/// <summary>
/// Test running the fade in and out for teleports.
/// This seems to work fine now.
/// TODO Move into MiscScripts::Fade namespace
/// TODO Make fadeOutTime and fadeInTime variables as int for this function.
/// </summary>
void PlayerScripts::TestFade() {
    //DO_SCREEN_FADE_OUT(1000);
    ////WAIT(1000);
    //WAIT(2000);
    //DO_SCREEN_FADE_IN(1000);

    Ped playerPed = PLAYER_PED_ID();

    //int fadeOutTime = 5000;
    // 4000 seems to work ok for the fade out
    int fadeOutTime = 4000;

    // 3000 seems to work ok for the fade in.
    int fadeInTime = 3000;

    // This seems to mostly work now
    
    DO_SCREEN_FADE_OUT(fadeOutTime);
     // Original wait:
    //WAIT(4000);
    WAIT(fadeOutTime);
     //New
    FREEZE_ENTITY_POSITION(playerPed, true);
    DO_SCREEN_FADE_IN(fadeInTime);
     //New
    //WAIT(500);
    //WAIT(2500);
    WAIT(fadeInTime);
    FREEZE_ENTITY_POSITION(playerPed, false);

    // These seem to somewhat work but still not properly.
    // Fade out if not already faded out or fading out
    //if (!IS_SCREEN_FADED_OUT() && !IS_SCREEN_FADING_OUT()) {
    //    DO_SCREEN_FADE_OUT(fadeOutTime);
    //    FREEZE_ENTITY_POSITION(playerPed, true);
    //    // Wait for the fade out
    //    //WAIT(550);
    //    //WAIT(2000);
    //}

    //// Fade in if not already faded in or fading in
    //if (!IS_SCREEN_FADED_IN() && !IS_SCREEN_FADING_IN()) {
    //    DO_SCREEN_FADE_IN(fadeInTime);
    //    FREEZE_ENTITY_POSITION(playerPed, false);
    //    // Wait for the fade in.
    //    WAIT(1000);
    //}
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
    SET_MAX_WANTED_LEVEL(5);
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
    // Named these to be a bit more clear
    int minWantedLevel = 0;
    int maxWantedLevel = 5;

    // Make sure the wanted level is valid first
    // This is more readable like this, and it still works.
    if (wantedLevel < maxWantedLevel || wantedLevel > minWantedLevel)
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


//----------- Begin TeleportToLocation --------------//

#ifdef LOAD_IPLS
// Global vector to store currently loaded IPLs (optional, for more advanced management)
std::vector<std::string_view> g_loadedIpls;
#endif//LOAD_IPLS



/// <summary>
/// Teleport to the location in the TeleportLocation enum within TeleportLocations.h
/// </summary>
/// <param name="locationToTeleport"></param>
/// TODO Reuse this for IPL support if possible.
/// 
// TODO Test IPL loading, not sure if this will work.
#ifdef LOAD_IPLS
void PlayerScripts::TeleportToLocation(TeleportLocation locationToTeleport) {
    auto& teleportLocations = TeleportLocations::GetInstance();
    //const Vector3& teleportCoords = teleportLocations.GetTeleportLocationInfo(locationToTeleport);
    const TeleportInfo teleportInfo = teleportLocations.GetTeleportLocationInfo(locationToTeleport);
    //SetPlayerCoords(teleportCoords);

    // TODO Test
    GTAped playerPed = PLAYER_PED_ID();



    // New tests for logging
#ifdef IPL_LOGGING
    LOG(INFO, "TeleportToLocation: Checking IPLs to unload");
    LOG(INFO, std::format("TeleportToLocation: Currently loaded IPLs count: {}", g_loadedIpls.size()));
#endif //IPL_LOGGING
    for (const auto& loadedIpl : g_loadedIpls) {
        LOG(INFO, std::format("TeleportToLocation: Loaded IPL: {}", loadedIpl));
    }
    //

    // Unload IPLs that are currently loaded but not needed for the new location
    std::vector<std::string_view> iplsToRemove;
    for (const auto& loadedIpl : g_loadedIpls) {
        bool shouldKeep = false;
        for (const auto& iplToLoad : teleportInfo.iplsToLoad) {
            if (loadedIpl == iplToLoad) {
                shouldKeep = true;
                break;
            }
        }
        if (!shouldKeep) {
            iplsToRemove.push_back(loadedIpl);
#ifdef IPL_LOGGING
            LOG(INFO, std::format("TeleportToLocation: Marking for removal: {}", loadedIpl));
#endif //IPL_LOGGING
        }
    }

    // New method for unloading ipls.
    for (const auto& iplToRemove : iplsToRemove) {
        //if (DOES_ENTITY_EXIST(GetPlayerPed())) {
        REMOVE_IPL(iplToRemove.data());

#ifdef IPL_LOGGING
        LOG(INFO, std::format("TeleportToLocation: Unloading IPL: {}", iplToRemove.data()));
#endif //IPL_LOGGING
        auto it = std::find(g_loadedIpls.begin(), g_loadedIpls.end(), iplToRemove);
        if (it != g_loadedIpls.end()) {
            g_loadedIpls.erase(it);
#ifdef IPL_LOGGING
            LOG(INFO, std::format("TeleportToLocation: Removed from loaded list: {}", iplToRemove));
#endif
        }
        //}
    }

    // Remove any previously loaded IPLs (optional, but recommended for cleanup)
    //for (const auto& ipl : g_loadedIpls) {
    //    //if (DOES_ENTITY_EXIST(GetPlayerPed())) {
    //        REMOVE_IPL(ipl.data());
    //    //}
    //}
    //g_loadedIpls.clear();

    // These seem to work, using GTAped from Menyoo now.
    playerPed.Position_set(teleportInfo.coordinates);
    playerPed.Heading_set(teleportInfo.heading);

    //SetPlayerCoords(teleportInfo.coordinates);
    //SetPlayerHeading(teleportInfo.heading);

    // Request the IPLs for the new location
    for (const auto& ipl : teleportInfo.iplsToLoad) {
        REQUEST_IPL(ipl.data());
        g_loadedIpls.push_back(ipl); // Keep track of loaded IPLs
    }
}
#else
    // These seem to work, using GTAped from Menyoo now.
    //playerPed.Position_set(teleportInfo.coordinates);
    //playerPed.Heading_set(teleportInfo.heading);

    //SetPlayerCoords(teleportInfo.coordinates);
    //SetPlayerHeading(teleportInfo.heading);

#endif // LOAD_IPLS


//----------- End TeleportToLocation --------------//


//----------- Begin WarpToLocation --------------//

/// <summary>
/// Set the player coords
/// </summary>
//void PlayerScripts::WarpToLocation(TeleportLocation locationToTeleport)
//{
//    if (!PLAYER::IS_PLAYER_PLAYING(GetPlayerID()))
//    {
//        return;
//    }
//
//    // Initalize the teleport locations vector, required for my new teleport method.
//    //InitializeTeleportLocations(); // Make sure to call this somewhere before the switch
//
//    // Locations are now defined in the TeleportLocations.cpp file.
//    TeleportToLocation(locationToTeleport);
//}

//----------- End WarpToLocation --------------//

#pragma region MobileRadio

//----------- Begin Mobile radio toggling --------------//

void PlayerScripts::EnableMobileRadio()
{
    SET_MOBILE_PHONE_RADIO_STATE(true);
    SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
    UI::Notify("Mobile radio enabled");
    mobileRadioFlag = true;

}

void PlayerScripts::DisableMobileRadio()
{
    SET_MOBILE_PHONE_RADIO_STATE(false);
    SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
    UI::Notify("Mobile radio disabled");
    mobileRadioFlag = false;
}

//----------- End Mobile radio toggling --------------//

#pragma endregion // MobileRadio

// Taken from Menyoo, Routine.cpp
// original name: set_local_button_super_run
void PlayerScripts::SetSuperRun()
{
    auto ped = PLAYER_PED_ID();
    bool isInAir = IS_ENTITY_IN_AIR(ped) != 0;

    if (!isInAir)
    {
        if (IS_CONTROL_PRESSED(0, INPUT_SPRINT))
            APPLY_FORCE_TO_ENTITY(ped, true, Vector3(0.0, 3.4, 0.0), Vector3(0.0, 0.0, 0.0), true, true, true, true, false, true);

        else if (IS_CONTROL_JUST_RELEASED(0, INPUT_SPRINT))
        {
            FREEZE_ENTITY_POSITION(ped, true);
            FREEZE_ENTITY_POSITION(ped, true);
        }
    }

}