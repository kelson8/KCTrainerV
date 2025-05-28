#pragma once
#include <string>

#include "Util/Enums.h"

#include "Teleports/TeleportLocations.h"
#include "IScriptBuilder.h"

class PlayerScripts : public IScriptBuilder
{
public:

    // Instance of the PlayerScripts for use with changing wanted levels and other values.
    static PlayerScripts& GetInstance()
    {
        static PlayerScripts instance;
        return instance;
    }


    std::string GetPlayerHealth();
    std::string GetPlayerVehicleName();
    std::string GetDistanceTraveled();

    void Tick() override;

    void DisableControls();
    void EnableControls();

    // Health armor and killing
    int GetPlayerHealth(Ped player);
    int GetPlayerArmor(Ped player);

    void SetPlayerHealth(Ped player, int health);
    void SetPlayerArmor(Ped player, int armor);
    void HealPlayer(Ped player);

    // Kill the player with the multiplayer suicide animation.
    void KillPlayerMP();

    // Disabled in the code for now, may reuse later.
    // 
    // Teleport player to location specified in the enum
    //void WarpToLocation(TeleportLocation locationToTeleport);
    //void TeleportToLocation(TeleportLocation locationToTeleport);

    
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
    Ped GetPlayerPed();
    int GetPlayerID();

    // Get the players stat
    int GetPlayerStat(PlayerModels character, const char* statName);

    // Get the current player model
    static PlayerModels GetCurrentPlayerModel();
    //

    bool IsPlayerInVehicle();

    // Vision toggles
    void ToggleHeatVision();
    void ToggleNightVision();
    //

    // Fade functions
    // TODO Move to MiscScripts or something
    void FadeScreenOut(int fadeTime);
    void FadeScreenIn(int fadeTime);
    // Test for fading the screen in/out.
    void TestFade();

    
    //

    // Mobile radio toggling
    void EnableMobileRadio();
    void DisableMobileRadio();
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
    
    // Mobile radio
    bool isMobileRadioEnabled = false;
    bool mobileRadioFlag = false;

    // Weapon toggles
    bool isExplosiveBulletsEnabled = false;
    bool isExplosiveMeleeEnabled = false;
    bool isInfiniteAmmoEnabled = false;

    bool isSlowAimEnabled = false;
    bool slowAimFlag = false;

    // Other toggles
    bool isSuperJumpEnabled = false;
    bool isSuperRunEnabled = false;


private:
    PlayerScripts() {} // Private constructor to prevent external instantiation
    PlayerScripts(const PlayerScripts&) = delete;
    PlayerScripts& operator=(const PlayerScripts&) = delete;

    // Stat misc
    // TODO Fix these, should make it to where I don't have to manually specify the player for the stats.
    // Direct mapping from the PlayerModel enum to the PlayerData struct.
    //static const std::array<PlayerData, static_cast<size_t>(PlayerModels::COUNT)> playerData;

    // Get the stat hash
    //static inline Hash GetStatHash(PlayerModels character, const std::string& statName);

    // Is the screen fading?, doesn't need to be public.
    bool fading = false;
    //

    float mDistance{ 0.0f };

    // Toggles
    bool heatVisionToggled;
    bool nightVisionToggled;

    // I don't think I'll use this outside this class.
    void SetSuperRun();
};

