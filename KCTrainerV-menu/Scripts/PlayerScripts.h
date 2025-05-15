#pragma once
#include <string>

#include <inc/natives.h>

class PlayerScripts
{
public:

    enum TeleportLocations
    {
        AIRPORT_RUNWAY = 1,
        HOSPITAL_LS1_ROOFTOP = 2,
        MOUNT_CHILLIAD = 3,
        
        // Police stations
        POLICE_STATION1 = 4,
        POLICE_STATION2 = 5,
        POLICE_STATION3 = 6,

    };

    // Instance of the PlayerScripts for use with changing wanted levels and other values.
    static PlayerScripts& GetInstance()
    {
        static PlayerScripts instance;
        return instance;
    }


    std::string GetPlayerHealth();
    std::string GetPlayerVehicleName();
    std::string GetDistanceTraveled();
    //static void SetPlayerCoords(TeleportLocations locationToTeleport);

    // Teleports
    // TODO Come up with better names for these.
    // This is in use for the teleport function
    void WarpPlayerToCoords(Vector3 position, float heading);
    // Teleport player to location
    void SetPlayerCoords(TeleportLocations locationToTeleport);

    // Toggles
    //static void ToggleInvincibility();
    //static void ToggleNeverWanted();
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

