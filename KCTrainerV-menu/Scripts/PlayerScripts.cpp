#include "pch.h"

#include "PlayerScripts.h"
#include "TextScripts.h"

#include "Util/UI.hpp"
#include "Util/Logger.hpp"

#include <inc/natives.h>
#include <format>

#include "../Util/Hash.h"
#include "../Util/Util.hpp"



// From Menyoo, makes it to where I don't have to type the namespaces for the natives.
#include "Natives/natives2.h"

//void FadeScreenIn(int ms);
//void FadeScreenOut(int ms);

// I will be moving some player scripts into the Scripts/Player folder 
// so I don't make this file super massive and hard to go through

// Booleans

// Ints
int wantedLevel = 0;

// Getting the current player
// TODO Fix these, should make it to where I don't have to manually specify the player for the stats.
//const std::array<PlayerData, static_cast<size_t>(PlayerModels::COUNT)> PlayerScripts::playerData = {
//    { PlayerModels::MICHEAL, "SP0_", "Michael" },
//    { PlayerModels::FRANKLIN, "SP1_", "Franklin" },
//    { PlayerModels::TREVOR, "SP2_", "Trevor" }
//};

// Get the stat hash
//static inline Hash GetStatHash(PlayerModels character, const std::string& statName)
//{
//    return (playerData[static_cast<size_t>(character)].prefix + statName).c_str()_hash;
//}

//

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
/// I added extra code in this from the Chaos Mod in Player.h.
/// Now this should teleport the players vehicle, and possibly make them not fall through the ground.
/// TODO Fix this, sometimes crashes and doesn't get the ground coordinate.
/// </summary>
/// <param name="position"></param>
void PlayerScripts::SetPlayerCoords(Vector3 position)
{
    // Based on teleport implementation in Chaos Mod (Player.h)
    auto playerPed = this->GetPlayerPed();
    bool isInVeh = IS_PED_IN_ANY_VEHICLE(playerPed, false);
    bool isInFlyingVeh = IS_PED_IN_FLYING_VEHICLE(playerPed);
    auto playerVeh = GET_VEHICLE_PED_IS_IN(playerPed, false);
    // TODO Figure out what exactly the velocity is used for 
    auto vel = GET_ENTITY_VELOCITY(isInVeh ? playerVeh : playerPed);
    float groundHeight = GET_ENTITY_HEIGHT_ABOVE_GROUND(playerVeh);
    float forwardSpeed;

    if (isInVeh)
    {
        forwardSpeed = GET_ENTITY_SPEED(playerVeh);
    }

    STREAMING::LOAD_SCENE(position);

    // TODO Implement fade functions for this.
    // Fade time in miliseconds for fading functions
    //int fadeTime = 500;

    // Oops, I inverted the fading...

    //FadeScreenOut(fadeTime);
    //WAIT(500);

    // TODO Add this from TeleportPlayerFindZ
    // This code should allow me to place the player on the ground without falling through the map.
    
    // Well this didn't seem to work.
    //   float groundZ;
	//bool useGroundZ;
	//for (int i = 0; i < 100; i++)
	//{
	//	float testZ = (i * 10.f) - 100.f;

 //       SET_ENTITY_COORDS(isInVeh ? playerVeh : playerPed, Vector3(position.x, position.y, testZ),
 //           false, false, false, false);
	//	if (i % 5 == 0)
	//		WAIT(0);

	//	useGroundZ = GET_GROUND_Z_FOR_3D_COORD(Vector3(position.x, position.y, testZ), &groundZ, false, false);
	//	if (useGroundZ)
	//		break;
	//}

 //   if (useGroundZ)
 //   {
 //       SET_ENTITY_COORDS(isInVeh ? playerVeh : playerPed, Vector3(position.x, position.y, groundZ),
 //           false, false, false, false);
 //   }
 //   else {
 //       // New addition for this checks if the player is in a vehicle, if so it also teleports the vehicle.
 //       // And it checks if the player is in a flying vehicle.
 //       SET_ENTITY_COORDS(isInVeh ? playerVeh : playerPed,
 //           Vector3(position.x, position.y, isInFlyingVeh ? position.z + groundHeight : position.z), false, false, false, false);
 //   }
    
    
    // New addition for this checks if the player is in a vehicle, if so it also teleports the vehicle.
    // And it checks if the player is in a flying vehicle.
    SET_ENTITY_COORDS(isInVeh ? playerVeh : playerPed,
        Vector3(position.x, position.y, isInFlyingVeh ? position.z + groundHeight : position.z), false, false, false, false);
    // TODO Figure out what this part does
    SET_ENTITY_VELOCITY(isInVeh ? playerVeh : playerPed, Vector3(vel.x, vel.y, vel.z));

    // Set the vehicle to the speed it previously was, this doesn't seem to work.
    if (isInVeh)
    {
        SET_VEHICLE_FORWARD_SPEED(playerVeh, forwardSpeed);
    }

    // Original method:
    //ENTITY::SET_ENTITY_COORDS(GetPlayerPed(), position, false, false, false, false);
    
    //FadeScreenIn(fadeTime);
}

/// <summary>
/// Set the players heading.
/// </summary>
/// <param name="heading"></param>
void PlayerScripts::SetPlayerHeading(float heading)
{
    ENTITY::SET_ENTITY_HEADING(GetPlayerPed(), heading);
}

/// <summary>
/// Get the current waypoint coordinates if it exists.
/// Taken from my KCTestScripts implementation in Menyoo.
/// </summary>
/// <returns>A Vector3 of the coords, such as this: Vector3(22.5, 15.2, 30.2)</returns>
Vector3 PlayerScripts::GetWaypointCoords()
{
    Vector3 coords;
    if (IS_WAYPOINT_ACTIVE())
    {
        coords = GET_BLIP_COORDS(GET_FIRST_BLIP_INFO_ID(8));
        return coords;
    }

    // Return all 0's if no waypoint found, TODO Setup check for this value.
    return Vector3(0, 0, 0);
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
/// TODO Test this
/// Taken from PlayerSuicide.cpp in Chaos Mod, I didn't know this was an animation
/// Oops, this crashes it.
/// </summary>
void PlayerScripts::KillPlayerMP()
{
    Util util;

    DWORD startTime = GetTickCount();
    DWORD timeout = 3000; // in millis

    Ped playerPed = PLAYER_PED_ID();
    if (!IS_PED_IN_ANY_VEHICLE(playerPed, false) && IS_PED_ON_FOOT(playerPed)
        && GET_PED_PARACHUTE_STATE(playerPed) == -1)
    {
        REQUEST_ANIM_DICT("mp_suicide");
        // Added this check like in the vehicle spawner, if it doens't load this shouldn't crash the game
        // This at least makes it not crash, but it doesn't work like my vehicle spawner doesn't.
        // TODO Make a function to wait on animations and play them
        // Possibly make a AnimScripts.cpp?
        while (!HAS_ANIM_DICT_LOADED("mp_suicide")) {
            WAIT(0);

            if (GetTickCount() > startTime + timeout) 
            {
                UI::Notify("Couldn't load animation");

                util.ShowSubtitle("Couldn't load animation");
                WAIT(0);

                REMOVE_ANIM_DICT("mp_suicide");

                // Make this break out of the function, it shouldn't continue.
                return;
            }
        }

        // Give the player a pistol
        Hash pistolHash = "WEAPON_PISTOL"_hash;

        GIVE_WEAPON_TO_PED(playerPed, pistolHash, 9999, true, true);
        
        // Play the MP Suicide animation
        TASK_PLAY_ANIM(playerPed, "mp_suicide", "pistol", 8.0f, -1.0f, 800, 1, 0.f, false, false, false);
        
        WAIT(750);
        //SET_PED_SHOOTS_AT_COORD(playerPed, 0, 0, 0, true);
        // Make the player shoot
        SET_PED_SHOOTS_AT_COORD(playerPed, Vector3(0.0f, 0.0f, 0.0f), true);
        REMOVE_ANIM_DICT("mp_suicide");
    }

    // Set the players health to 0.
    SET_ENTITY_HEALTH(playerPed, 0, 0);
}

#pragma endregion // HealthArmorAndKilling


#pragma region FadeFunctions

/// <summary>
/// Fade the screen in
/// </summary>
/// <param name="ms"></param>
void PlayerScripts::FadeScreenIn(int ms)
{
    //if (!IS_SCREEN_FADED_IN())
    //{
        DO_SCREEN_FADE_IN(ms);
    //}
}

/// <summary>
/// Fade the screen out
/// </summary>
/// <param name="ms"></param>
void PlayerScripts::FadeScreenOut(int ms)
{
    //if (!IS_SCREEN_FADED_OUT())
    //{
        DO_SCREEN_FADE_OUT(ms);
    //}
}

void PlayerScripts::TestFade() {
    //DO_SCREEN_FADE_OUT(1000);
    ////WAIT(1000);
    //WAIT(2000);
    //DO_SCREEN_FADE_IN(1000);

    int fadeTime = 500;

    // Fade out if not already faded out or fading out
    if (!IS_SCREEN_FADED_OUT() && !IS_SCREEN_FADING_OUT()) {
        DO_SCREEN_FADE_OUT(fadeTime);
        WAIT(550); // Wait for fade out
    }

    // Fade in if not already faded in or fading in
    if (!IS_SCREEN_FADED_IN() && !IS_SCREEN_FADING_IN()) {
        DO_SCREEN_FADE_IN(fadeTime);
    }
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


//----------- Begin TeleportToLocation --------------//

#ifdef LOAD_IPLS
// Global vector to store currently loaded IPLs (optional, for more advanced management)
std::vector<std::string_view> g_loadedIpls;
#endif//LOAD_IPLS



// TODO Adapt TeleportToLocation function to new format for categories:
#ifdef NEW_TELEPORTS
/// <summary>
/// Teleport to the location specified by the category and ID.
/// </summary>
/// <param name="category">The category of the teleport location.</param>
/// <param name="id">The ID of the teleport location within the category.</param>
void PlayerScripts::TeleportToLocation(TeleportLocationCategory category, TeleportLocationID id) {
    auto& teleportLocations = TeleportLocations::GetInstance();
    const TeleportInfo teleportInfo = teleportLocations.GetTeleportLocationInfo(category, id);

#ifdef LOAD_IPLS
    // Logging for IPL unloading (if enabled)
#ifdef IPL_LOGGING
    LOG(INFO, "TeleportToLocation: Checking IPLs to unload");
    LOG(INFO, std::format("TeleportToLocation: Currently loaded IPLs count: {}", g_loadedIpls.size()));
    for (const auto& loadedIpl : g_loadedIpls) {
        LOG(INFO, std::format("TeleportToLocation: Loaded IPL: {}", loadedIpl));
    }
#endif //IPL_LOGGING

    int fadeTime = 500;

    //FadeScreenOut(fadeTime);
    DO_SCREEN_FADE_OUT(fadeTime);
    // Wait for the fade out to complete
    WAIT(500);

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

    // Unload the IPLs
    for (const auto& iplToRemove : iplsToRemove) {
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
    }

    SetPlayerCoords(teleportInfo.coordinates);
    SetPlayerHeading(teleportInfo.heading);

    // Request the IPLs for the new location
    for (const auto& ipl : teleportInfo.iplsToLoad) {
        REQUEST_IPL(ipl.data());
        g_loadedIpls.push_back(ipl); // Keep track of loaded IPLs
    }

    //FadeScreenIn(fadeTime);
    DO_SCREEN_FADE_IN(fadeTime);
#else
    SetPlayerCoords(teleportInfo.coordinates);
    SetPlayerHeading(teleportInfo.heading);
#endif
}
#else

/// <summary>
/// Teleport to the location in the TeleportLocation enum within TeleportLocations.h
/// </summary>
/// <param name="locationToTeleport"></param>
void PlayerScripts::TeleportToLocation(TeleportLocation locationToTeleport) {
    auto& teleportLocations = TeleportLocations::GetInstance();
    //const Vector3& teleportCoords = teleportLocations.GetTeleportLocationInfo(locationToTeleport);
    const TeleportInfo teleportInfo = teleportLocations.GetTeleportLocationInfo(locationToTeleport);
    //SetPlayerCoords(teleportCoords);

    // TODO Test IPL loading, not sure if this will work.
#ifdef LOAD_IPLS

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

    SetPlayerCoords(teleportInfo.coordinates);
    SetPlayerHeading(teleportInfo.heading);

    // Request the IPLs for the new location
    for (const auto& ipl : teleportInfo.iplsToLoad) {
        REQUEST_IPL(ipl.data());
        g_loadedIpls.push_back(ipl); // Keep track of loaded IPLs
    }
#else
    SetPlayerCoords(teleportInfo.coordinates);
    SetPlayerHeading(teleportInfo.heading);
#endif // LOAD_IPLS
}

#endif //NEW_TELEPORTS

//----------- End TeleportToLocation --------------//


//----------- Begin WarpToLocation --------------//

/// <summary>
/// Set the player coords
/// </summary>
// TODO Setup to use new format for categories, make this easier to sort through and add to.:
#ifdef NEW_TELEPORTS
void PlayerScripts::WarpToLocation(TeleportLocationCategory category, TeleportLocationID id)
{
    if (!PLAYER::IS_PLAYER_PLAYING(GetPlayerID()))
    {
        return;
    }

    // Locations are now accessed using category and ID.
    TeleportToLocation(category, id);
}
#else
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
#endif

//----------- End WarpToLocation --------------//

#pragma region Stats

//----------- Begin Stats --------------//
// TODO Possibly move these into StatScripts.cpp?

// TODO Setup option to get police killed and cop cars blown up stats
// When getting the stats, try to subtract the difference, 
// so if there is 1000 cops killed in total, and 100 cops killed without dying, set the value to 100.

/// <summary>
/// TODO Set this up
/// Also, compact these, possibly make a for loop?
/// Credit to this gtaforums thread for some of these stats: https://gtaforums.com/topic/802525-changing-stats/
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
    // From Menyoo:
    // std::pair<std::string, std::string> vCharNames[3] = { { "SP0_", "Michael" },{ "SP1_", "Franklin" },{ "SP2_", "Trevor" } };

    // SP0 = Michael
    // SP1 = Franklin
    // SP2 = Trevor
    // I removed the extra values out of here, there was a bunch of duplicates.
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
    */
}

//     //CARS_COPS_EXPLODED
    //    CARS_EXPLODED
    //    COPS_KILLS_ON_SPREE

    //    KILLS
    //    KILLS_ARMED
    //    KILLS_BY_OTHERS
    //    KILLS_COP

// So, SP0 is actually Micheal? I had these swapped..
// TODO Set these below to get which player is playing, Michael = SP0, Franklin = SP1, Trevor = SP2
// TODO Compact these, I'm sure there is a better way to do this then what I am doing.

// From Menyoo:
// std::pair<std::string, std::string> vCharNames[3] = { { "SP0_", "Michael" },{ "SP1_", "Franklin" },{ "SP2_", "Trevor" } };

/// <summary>
/// Get the cops kiled stat
/// <param name="character">The player to select the stat for, using the PlayerModels enum class.</param>
/// </summary>
/// <returns>The amount of cops killed</returns>
//int PlayerScripts::GetCopsKilledStat()
//int PlayerScripts::GetCopsKilledStat(PlayerModels character)
int PlayerScripts::GetCopsKilledStat()
{
    int copsKilledStat;

    // TODO Test this, this should get the current player instead of me manually specifying it.
    PlayerModels currentPlayer = PlayerScripts::GetCurrentPlayerModel();

    //switch (character)
    switch (currentPlayer)
    {
    case PlayerModels::MICHEAL: // SP0
        STAT_GET_INT("SP0_KILLS_COP"_hash, &copsKilledStat, -1);
        break;

    case PlayerModels::FRANKLIN: // SP1
        STAT_GET_INT("SP1_KILLS_COP"_hash, &copsKilledStat, -1);
        break;

    case PlayerModels::TREVOR: // SP2
        STAT_GET_INT("SP2_KILLS_COP"_hash, &copsKilledStat, -1);
        break;

    default:
        copsKilledStat = 0;
        break;
    }

    return copsKilledStat;
    

}

/// <summary>
/// Get the cops cars blown up.
/// </summary>
/// <returns>The amount of cop cars blown up</returns>
/// <param name="character">The player to select the stat for, using the PlayerModels enum class.</param>
//int PlayerScripts::GetCopsVehiclesBlownUpStat()
/// <returns></returns>
//int PlayerScripts::GetCopsVehiclesBlownUpStat(PlayerModels character)
int PlayerScripts::GetCopsVehiclesBlownUpStat()
{
    int copsVehiclesBlownUpStat;

    PlayerModels currentPlayer = PlayerScripts::GetCurrentPlayerModel();

    //switch (character)
    switch (currentPlayer)
    {
    case PlayerModels::MICHEAL: // SP0
        STAT_GET_INT("SP0_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);
        break;

    case PlayerModels::FRANKLIN: // SP1
        STAT_GET_INT("SP1_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);
        break;

    case PlayerModels::TREVOR: // SP2
        STAT_GET_INT("SP2_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);
        break;

    default:
        copsVehiclesBlownUpStat = 0;
        break;
    }

    //STAT_GET_INT("SP0_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);

    return copsVehiclesBlownUpStat;

}

#pragma region StatLoops


// Stat loops

//------------ Cops cars blown up Logic --------------/
// Set default value for this to 0
int PlayerScripts::copvehiclesBlownUpBeforeDying = 0;

/// <summary>
/// Add one to the cop copvehiclesBlownUpBeforeDying stat
/// </summary>
void PlayerScripts::IncrementCopVehiclesBlownUp() {
    copvehiclesBlownUpBeforeDying++;
}

/// <summary>
/// Reset the cop vehicles blown up stat back to 0
/// </summary>
void PlayerScripts::ResetCopVehiclesBlownUpBeforeDying() {
    copvehiclesBlownUpBeforeDying = 0;
}

/// <summary>
/// Get the current copvehiclesBlownUpBeforeDying value.
/// </summary>
int PlayerScripts::GetCopVehiclesBlownUpBeforeDying() {
    return copvehiclesBlownUpBeforeDying;
}


//------------ Cops killed logic --------------/

// Set default value for this to 0
int PlayerScripts::copsKilledBeforeDying = 0;

/// <summary>
/// Add one to the cops killed stat
/// </summary>
void PlayerScripts::IncrementCopsKilled() {
    copsKilledBeforeDying++;
}

/// <summary>
/// Reset the cops killed stat back to 0
/// </summary>
void PlayerScripts::ResetCopsKilledBeforeDying() {
    copsKilledBeforeDying = 0;
}

/// <summary>
/// Get the current copsKilledBeforeDying value.
/// </summary>
int PlayerScripts::GetCopsKilledBeforeDying() {
    return copsKilledBeforeDying;
}

//------------ End Cops killed logic --------------/

//------------ Cops killed stat display -------------/ 
//------------ And Cop vehicles blown up stat display --------------/

/// <summary>
/// This works as a system that increments 
/// depending on how many cops/swat you kill, for now this only prints to the console with std::cout.
/// Eventually, I'll make a function that tracks how many cop vehicles you blow up.
/// This will draw to the screen and print to the console with std::cout also.
/// </summary>
void PlayerScripts::ProcessCopsKilled()
{
    auto& textScripts = TextScripts::GetInstance();

    // Cops killed position on menu, this value is now in the header
    //float copsKilledMenuPosX = 0.190f;
    //float copsKilledMenuPosY = 0.949f;

    PlayerModels currentPlayer = PlayerScripts::GetCurrentPlayerModel();

    // TODO Make these into functions in this class.
    bool isPlayerDead = IS_ENTITY_DEAD(PlayerScripts::GetPlayerPed(), false);
    bool isPlayerBeingArrested = IS_PLAYER_BEING_ARRESTED(PlayerScripts::GetPlayerID(), true);

    //---
    // Get the "KILLS_COP" and "KILLS_SWAT" stats.
    //---
    //int copsKilled = 0;
    //int swatKilled = 0;

    int copsKilled = PlayerScripts::GetPlayerStat(currentPlayer, "KILLS_COP");
    int swatKilled = PlayerScripts::GetPlayerStat(currentPlayer, "KILLS_SWAT");

    int copCarsBlownUp = PlayerScripts::GetPlayerStat(currentPlayer, "CARS_COPS_EXPLODED");

    // This might be the stat to get the total helicopters exploded:
    // HELIS_EXPLODED

    // Combine the copsKilled and swatKilled stats
    int totalCopsKilled = copsKilled + swatKilled;

    //---
    // Keep track of previous value
    //---
    //static int previousKills = copsKilled;
    static int previousKills = totalCopsKilled;

    int totalCopCarsBlownUp = copCarsBlownUp;
    static int previousCopCarsBlownUp = totalCopCarsBlownUp;
    
    

    //---
    // Check if the cops/swat killed stat has been increased.
    //---
    //if (copsKilled > previousKills)
    if (totalCopsKilled > previousKills)
    {
        PlayerScripts::IncrementCopsKilled();
        // Update the previousKills value
        //previousKills = copsKilled;
        previousKills = totalCopsKilled;
    }

    //---
    // Check if the cops blown up stat has been increased
    //---
    if (totalCopCarsBlownUp > previousCopCarsBlownUp)
    {
        PlayerScripts::IncrementCopVehiclesBlownUp();
        // Update the previousCopCarsBlownUp value
        previousCopCarsBlownUp = totalCopCarsBlownUp;
    }

    //---
    // Check if the player has died or been busted.
    //---
    if (IS_ENTITY_DEAD(PlayerScripts::GetPlayerPed(), false) || IS_PLAYER_BEING_ARRESTED(PlayerScripts::GetPlayerID(), true))
    {
        // Reset the previous kills back to 0.
        this->ResetCopsKilledBeforeDying();
        previousKills = totalCopsKilled;

        // Reset the previous cop cars blown up to 0
        this->ResetCopVehiclesBlownUpBeforeDying();
        previousCopCarsBlownUp = totalCopCarsBlownUp;
    }

    //---
    // Display the current kills count
    //---
    int copsKilledThisLife = PlayerScripts::GetCopsKilledBeforeDying();
    // Store it in a string stream
    std::stringstream ss;
    ss << "Cops killed this life: " << copsKilledThisLife;
    std::string copsKilledString = ss.str();

    // Print to console
    std::cout << copsKilledString << std::endl;

    // Display to screen
    // This displays the value to the screen

    // This shouldn't draw if the player is dead or being arrested.
    if (!isPlayerDead || !isPlayerBeingArrested)
    {
        textScripts.SetTextEntry(copsKilledString.c_str(), 255, 255, 255, 255);

        textScripts.TextPosition(copsKilledMenuPosX, copsKilledMenuPosY);
    }

    //---
    // Display the current cop vehicles blown up
    //---
    // Well this part isn't working yet, this displays but the value doesn't update.
    int copCarsBlownUpThisLife = PlayerScripts::GetCopVehiclesBlownUpBeforeDying();
    std::stringstream ss1;
    ss1 << "Cop cars blown up this life: " << copCarsBlownUpThisLife;
    // TODO Test
    //ss1 << "Total cop cars blown up: " << copCarsBlownUp;
    std::string copCarsBlownUpString = ss1.str();

    // Disabled, print to console
    //std::cout << copCarsBlownUpString << std::endl;

    // This shouldn't draw if the player is dead or being arrested.
    if (!isPlayerDead || !isPlayerBeingArrested)
    {
        // Display to screen
        textScripts.SetTextEntry(copCarsBlownUpString.c_str(), 255, 255, 255, 255);

        textScripts.TextPosition(copsCarsBlownUpMenuPosX, copsCarsBlownUpMenuPosY);

    }
    
}

#pragma endregion // StatLoops

//------------ End Cops killed stat display --------------/

//----------- End Stats --------------//

#pragma endregion //Stats

#pragma region MobileRadio

//----------- Begin Mobile radio toggling --------------//

void PlayerScripts::EnableMobileRadio()
{
    SET_MOBILE_PHONE_RADIO_STATE(true);
    SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
    UI::Notify("Mobile radio enabled");
    this->mobileRadioFlag = true;

}

void PlayerScripts::DisableMobileRadio()
{
    SET_MOBILE_PHONE_RADIO_STATE(false);
    SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
    UI::Notify("Mobile radio disabled");
    this->mobileRadioFlag = false;
}

//----------- End Mobile radio toggling --------------//

#pragma endregion // MobileRadio