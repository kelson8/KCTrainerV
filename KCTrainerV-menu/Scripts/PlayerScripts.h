#pragma once
#include <string>

#include "Teleports/TeleportLocations.h"

#include <inc/natives.h>

class PlayerScripts
{
public:

    // Moved into TeleportLocations.h
    //enum TeleportLocations
    //{
    //    AIRPORT_RUNWAY = 1,
    //    HOSPITAL_LS1_ROOFTOP = 2,
    //    MOUNT_CHILLIAD = 3,
    //    
    //    // Police stations
    //    POLICE_STATION1 = 4,
    //    POLICE_STATION2 = 5,
    //    POLICE_STATION3 = 6,

    //};

    // Instance of the PlayerScripts for use with changing wanted levels and other values.
    static PlayerScripts& GetInstance()
    {
        static PlayerScripts instance;
        return instance;
    }


    std::string GetPlayerHealth();
    std::string GetPlayerVehicleName();
    std::string GetDistanceTraveled();

    // Teleports
    // This is in use for the teleport function
    //void SetPlayerCoords(Vector3 position, float heading);
    void SetPlayerCoords(Vector3 position);
    void SetPlayerHeading(float heading);
    // Teleport player to location specified in the enum
    // TODO Adapt below to new format once I set it up:
    // 
#ifdef NEW_TELEPORTS
    void WarpToLocation(TeleportLocationCategory category, TeleportLocationID id);
    void TeleportToLocation(TeleportLocationCategory category, TeleportLocationID id);
#else
    void WarpToLocation(TeleportLocation locationToTeleport);
    void TeleportToLocation(TeleportLocation locationToTeleport);
#endif //NEW_TELEPORTS

    
    //

    // Toggles
    void ToggleInvincibility();
    void ToggleNeverWanted();

    // Never wanted switches
    void EnableNeverWanted();
    void DisableNeverWanted();
    //

    //void SetWantedLevel(int wantedLevel);
    void SetWantedLevel();

    // Geting the player char and id
    int GetPlayerPed();
    int GetPlayerID();

    // Get player coords and heading
    Vector3 GetPlayerCoords();
    float GetPlayerHeading();
    //

    bool IsPlayerInVehicle();

    // Vision toggles
    void ToggleHeatVision();
    void ToggleNightVision();
    //

    // Fade functions
    // TODO Move to MiscScripts or something
    void FadeScreenOut(int ms);
    void FadeScreenIn(int ms);
    // Test for fading the screen in/out.
    void TestFade();
    //
    
    //static bool IsInvincibilityEnabled()
    //{
    //    return invincibilityEnabled;
    //}

    // Getters and setters
    bool IsNeverWantedEnabled() const
    {
        return neverWantedEnabled;
    }

    void SetNeverWanted(bool toggle) 
    {
        neverWantedEnabled = toggle;
    }

    // TODO Make a bool toggle for these in the menu, for now they aren't in use.
    
    // Heat vision
    bool IsHeatVisionEnabled() const
    {
        return heatVisionToggled;
    }

    void SetHeatVision(bool toggle)
    {
        heatVisionToggled = toggle;
    }

    // Night vision
    bool IsNightVisionEnabled() const
    {
        return nightVisionToggled;
    }

    void SetNightVision(bool toggle)
    {
        nightVisionToggled = toggle;
    }

    int wantedLevel;

    // TODO Make getters and setters for this
    // These flags are used for making some of these not run constantly or stop running.
    bool invincibilityEnabled = false;
    bool invincibilityFlag = false;

    bool neverWantedEnabled = false;
    bool neverWantedFlag = false;

private:
    PlayerScripts() {} // Private constructor to prevent external instantiation
    PlayerScripts(const PlayerScripts&) = delete;
    PlayerScripts& operator=(const PlayerScripts&) = delete;

    float mDistance{ 0.0f };

    // Toggles
    bool heatVisionToggled;
    bool nightVisionToggled;

};

