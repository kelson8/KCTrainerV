#pragma once
#include <string>

// I think disabling this header might have fixed this? For now at least..
// #include "Natives/natives2.h"

#include "Constants.hpp"

#include <inc/types.h>

/*
 * The class for the "script" that runs.
 * This may serve as the core logic of the script.
 * For example, doing something with the player or world every tick.
 */
class KCMainScript 
{
public:
    KCMainScript() = default;
    ~KCMainScript() = default;
    void Tick();

    std::string GetPlayerHealth();
    std::string GetPlayerVehicleName();
    std::string GetDistanceTraveled();

    // TODO Fix this
#ifdef RELOAD_CONFIG
    //void LoadConfig(); // Reload the config
    void SetScriptMenu(CScriptMenu<KCMainScript>* menu); // Setter for the menu
#endif

    // TODO Fix this
//#ifdef LUA_TEST
//    void SetLuaTeleportLocation(Vector3 location);
//    Vector3 GetLuaTeleportLocation() const;
//
//#endif

    bool IsPlayerInVehicle();
    void ChangePlayerVehicleColor();
private:
#ifdef LUA_TEST
    Vector3 m_luaTeleportLocation;
#endif

#ifdef RELOAD_CONFIG
    CScriptMenu<KCMainScript>* scriptMenuPtr = nullptr; // Store a pointer to the menu
#endif

    float mDistance{ 0.0f };
};
