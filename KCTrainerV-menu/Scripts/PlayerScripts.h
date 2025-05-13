#pragma once
#include <string>

#include <inc/natives.h>

class PlayerScripts
{
public:

    enum TeleportLocations
    {
        AIRPORT_RUNWAY = 1,
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

    //void SetWantedLevel(int wantedLevel);
    void SetWantedLevel();

    // Geting the player char and id
    int GetPlayerPed();
    int GetPlayerID();

    bool IsPlayerInVehicle();

    
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

    int wantedLevel;

private:
    PlayerScripts() {} // Private constructor to prevent external instantiation
    PlayerScripts(const PlayerScripts&) = delete;
    PlayerScripts& operator=(const PlayerScripts&) = delete;

    float mDistance{ 0.0f };

    // TODO Make getters and setters for this
    static bool invincibilityEnabled;
    static bool neverWantedEnabled;

};

