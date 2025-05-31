#include "pch.h"

#include "KCMenuScript.hpp"

// I think this slows down compiling time in here.
//#include "common.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/MiscScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"
#include "Scripts/WorldScripts.h"

#include "Scripts/TextScripts.h"
#include "Scripts/Stats.h"

#include "Util/Enums.h"

#include "Util/UI.hpp"

// New
//#include "Util/Paths.hpp"
//#include "Util/Logger.hpp"

//#include "ScriptMenu.hpp"

// Marker
#include "Scripts/Markers/MarkerScripts.h"

// Misc
#include "Scripts/Misc/MiscMusicScripts.h"
#include "Scripts/Misc/MiscExtraFeatures.h"
#include "Scripts/Misc/IDGun.h"


// TODO Move most items out of Tick and into another file
// Might make this neater.
// Either make an Events.cpp file, or GameTick, Tick file?
// Make multiple functions such as PedTicks, PlayerTicks,
// VehicleTicks, and more to organize it a bit better.

// This tick event below uses flags for certain actions so it doesn't constantly run.
// Such as when the vehicle invincibility is turned on, it won't constantly run the functions.

void KCMainScript::Tick() {
    // Scripts
    auto& textScripts = TextScripts::GetInstance();

    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();

    auto& worldScripts = WorldScripts::GetInstance();

    auto& pedScripts = PedScripts::GetInstance();

    auto& markerScripts = Scripts::Marker::GetInstance();

    //TODO Re-enable these, testing with my tick items disabled.
#ifdef RUN_TICKS

    // Core script logic
    mDistance += ENTITY::GET_ENTITY_SPEED(PLAYER::PLAYER_PED_ID()) * MISC::GET_FRAME_TIME();

    // New format for the ticks, this seems to work
    playerScripts.Tick();
    pedScripts.Tick();

    textScripts.Tick();

    Stats::Cop::Tick();
    
    // MiscScripts
    MiscScripts::EXFeatures::Tick();
    MiscScripts::IDGun::Tick();
    
    vehicleScripts.Tick();
    worldScripts.Tick();

    markerScripts.Tick();

#endif // RUN_TICKS

}

// TODO Complete this, when I do I'll possibly add a new sub menu named 'Script settings'
#ifdef RELOAD_CONFIG
void KCMainScript::SetScriptMenu(CScriptMenu<KCMainScript>* menu)
{
    if (scriptMenuPtr)
    {
        scriptMenuPtr->LoadSettings(); // Call the menu's LoadSettings
        UI::Notify("Configuration reloaded successfully.");
    }
    else
    {
        UI::Notify("Error: Script menu instance not available for reload.");
    }
}


//void KCMainScript::LoadConfig()
//{
//    const auto settingsMenuPath = Paths::GetModPath() / "settings_menu.ini";
//    LOG(INFO, "Reloading configuration file from %s", settingsMenuPath.string().c_str());
//
//    MenuSettings::ReadSettings();
//
//    LOG(INFO, "Configuration reloaded successfully.");
//}
#endif

// TODO Fix this
//#ifdef LUA_TEST
//void KCMainScript::SetLuaTeleportLocation(Vector3 location)
//{
//    m_luaTeleportLocation = location;
//}
//
//Vector3 KCMainScript::GetLuaTeleportLocation() const
//{
//    return m_luaTeleportLocation;
//}
//#endif

std::string KCMainScript::GetPlayerHealth() {
    int health = ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID());

    return std::format("{}", health);
}

std::string KCMainScript::GetPlayerVehicleName() {
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

std::string KCMainScript::GetDistanceTraveled() {
    return std::format("{:.0f} meters", mDistance);
}

bool KCMainScript::IsPlayerInVehicle() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    return ENTITY::DOES_ENTITY_EXIST(playerVehicle);
}

void KCMainScript::ChangePlayerVehicleColor() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    if (ENTITY::DOES_ENTITY_EXIST(playerVehicle)) {
        VEHICLE::SET_VEHICLE_COLOUR_COMBINATION(playerVehicle,
            MISC::GET_RANDOM_INT_IN_RANGE(0,
                VEHICLE::GET_NUMBER_OF_VEHICLE_COLOURS(playerVehicle)
            )
        );
    }
}
