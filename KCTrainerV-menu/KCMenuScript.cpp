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

    //TODO Re-enable these, testing with my tick items disabled.
#ifdef RUN_TICKS

    // If the player has died, run this
    // TODO Set this up.
    //if (!playerScripts.IsPlayerAlive() && playerScripts.playerAliveFlag)
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

    //-----
    // Run the airstrikes
    //-----
    if (MiscScripts::EXFeatures::airStrikeRunning)
    {
        MiscScripts::EXFeatures::StartAirstrikeTest();
    }

    // Core script logic
    mDistance += ENTITY::GET_ENTITY_SPEED(PLAYER::PLAYER_PED_ID()) * MISC::GET_FRAME_TIME();

    // Add other items that need to run constantly.
    //if (playerScripts.IsNeverWantedEnabled())
    //{
    //    PLAYER::SET_MAX_WANTED_LEVEL(0);
    //}

    // Well this flag seems to make this work just fine, if it is disabled it'll enable it, if disabled it'll enable it.
    // I did this to prevent this from constantly running in a loop, 
    // at least the notification doesn't constantly show up so I think this works!
    if (playerScripts.invincibilityEnabled && !playerScripts.invincibilityFlag)
    {
        ENTITY::SET_ENTITY_PROOFS(playerScripts.GetPlayerPed(), true, true, true, true, true, true, true, true);
        UI::Notify("Invincibility enabled");
        playerScripts.invincibilityFlag = true;
    }
    else if (!playerScripts.invincibilityEnabled && playerScripts.invincibilityFlag) {
        ENTITY::SET_ENTITY_PROOFS(playerScripts.GetPlayerPed(), false, false, false, false, false, false, false, false);
        UI::Notify("Invincibility disabled");
        playerScripts.invincibilityFlag = false;
    }

    if (playerScripts.neverWantedEnabled)
    {
        playerScripts.EnableNeverWanted();
        playerScripts.neverWantedFlag = true;
    }
    //-----
    // This should only run once, the above needs to be in a loop
    //-----
    else if (!playerScripts.neverWantedEnabled && playerScripts.neverWantedFlag)
    {
        playerScripts.DisableNeverWanted();
        // This should stop this from constantly running
        // I added a debug notify line here to test that.
        UI::Notify("Never wanted off.");
        playerScripts.neverWantedFlag = false;
    }

    //-----
    // New stat features
    //-----
    if (Stats::Cop::isCopsKilledDisplayActive)
    {
        Stats::Cop::ProcessCopsKilledDisplay();
    }
    
#ifdef EXTRA_FEATURES
    //-----
    // Player force field
    //-----
#ifdef MEMORY_TESTING
    // Seems to crash, disabled.
    //if (MiscScripts::EXFeatures::isForceFieldEnabled)
    //{
    //    MiscScripts::EXFeatures::EnableForceField();
    //}
#endif // MEMORY_TESTING

    //-----
    // Run the tick event for the TV if enabled.
    //-----
    if (MiscScripts::EXFeatures::isTVRunning)
    {
        MiscScripts::EXFeatures::TvTick();
    }

    //-----
    // Make peds attack player
    //-----
#ifdef MEMORY_TESTING
    if (pedScripts.isPedsAttackEnabled)
    {
        pedScripts.PedsAttackPlayer();
    }
#endif // MEMORY_TESTING

#endif //EXTRA_FEATURES

    //-----
    // Make all peds drive crazy
    //-----
#ifdef MEMORY_TESTING
    if (pedScripts.isCrazyPedDrivingEnabled)
    {
        pedScripts.MakeAllPedsDriveCrazy();
    }
#endif //

    //-----
    // Invincibility toggle
    //-----
    if(vehicleScripts.isInvincibleVehicleEnabled && !vehicleScripts.invincibilityFlag)
    {
        vehicleScripts.EnableInvincibility();
        vehicleScripts.invincibilityFlag = true;
    } else if(!vehicleScripts.isInvincibleVehicleEnabled && vehicleScripts.invincibilityFlag)
    {
        vehicleScripts.DisableInvincibility();
        vehicleScripts.invincibilityFlag = false;
    }
    
    //-----
    // Mobile radio toggle
    //-----
    if(playerScripts.isMobileRadioEnabled && !playerScripts.mobileRadioFlag)
    {
        playerScripts.EnableMobileRadio();
        playerScripts.mobileRadioFlag = true;
    } else if(!playerScripts.isMobileRadioEnabled && playerScripts.mobileRadioFlag)
    {
        playerScripts.DisableMobileRadio();
        playerScripts.mobileRadioFlag = false;
    }
    
    
    //-----
    // Display text on screen
    // I got this to work
    //-----
    if (textScripts.drawText)
    {
        textScripts.SetupText();
    }

    //-----
    // Draw coordinates to the screen
    //-----
    if (textScripts.drawCoords)
    {
        // Will this slow down the whole menu?
        // Oops, this wait breaks the menu..
        // TODO, figure out how to slow down the coordinates display, I guess it's fine for now.
        //WAIT(100);
        textScripts.DisplayCoordinates();
    }

    //-----
    // This mostly works, draw entity id, entity x,y,z, entity heading, and entity model hash to the screen.
    // Also draw vehicle name to the screen.
    //-----
    if (MiscScripts::IDGun::isIdGunEnabled)
    {
        MiscScripts::IDGun::IdGun();
    }

    // Turn on/off restricted areas
    // This works but doesn't turn back off, disabled for now.
    //if (worldScripts.isRestrictedAreasDisabled)
    //{
    //    worldScripts.DisableRestrictedAreas();
    //}
    //
    
    //-----
    // Adapted from Menyoo
    // Set fireworks in a loop near the player
    //-----
    if (worldScripts.isFireworksStarted)
    {
        worldScripts.FireworksLoop();
    }


#ifdef MEMORY_TESTING
    //-----
    // Make all the peds in the area calm, meaning they shouldn't run from anything.
    //-----
    if (worldScripts.isPedsCalmActive)
    {
        worldScripts.SetPedsCalm();
    }
#endif // MEMORY_TESTING
    // End adapted from Menyoo
   
    //textScripts.SetTextEntry();

    //HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Test");
    //textScripts.SetTextPosition();
    
    //-----
    // Bullet proof toggle
    //-----
    if (vehicleScripts.isBulletProofEnabled && !vehicleScripts.bulletProofFlag)
    {
        vehicleScripts.EnableBulletProof();
        vehicleScripts.bulletProofFlag = true;
    }
    else if (!vehicleScripts.isBulletProofEnabled && vehicleScripts.bulletProofFlag)
    {
        vehicleScripts.DisableBulletProof();
        vehicleScripts.bulletProofFlag = false;
    }

    //-----
    // Mobile radio toggle
    //-----
    if (playerScripts.isMobileRadioEnabled && !playerScripts.mobileRadioFlag)
    {
        playerScripts.EnableMobileRadio();
        playerScripts.mobileRadioFlag = true;
    }
    else if (!playerScripts.isMobileRadioEnabled && playerScripts.mobileRadioFlag)
    {
        playerScripts.DisableMobileRadio();
        playerScripts.mobileRadioFlag = false;
    }


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
