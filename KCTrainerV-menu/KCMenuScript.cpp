#include "KCMenuScript.hpp"

#include "Scripts/PlayerScripts.h"
#include "Scripts/MiscScripts.h"

#include "Scripts/TextScripts.h"

#include "Util/UI.hpp"

#include <inc/natives.h>
#include <format>

void KCMainScript::Tick() {
    //PlayerScripts playerScripts;
    auto& textScripts = TextScripts::GetInstance();

    auto& playerScripts = PlayerScripts::GetInstance();

    auto& miscScripts = MiscScripts::GetInstance();

    // Hmm, this seems to work if I toggle it in my menu.
    if (miscScripts.airStrikeRunning)
    {
        miscScripts.StartAirstrikeTest();
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

    // TODO Add never wanted toggle, should run in a loop?
    if (playerScripts.neverWantedEnabled)
    {
        playerScripts.EnableNeverWanted();
        playerScripts.neverWantedFlag = true;
    }
    // This should only run once, the above needs to be in a loop
    else if (!playerScripts.neverWantedEnabled && playerScripts.neverWantedFlag)
    {
        playerScripts.DisableNeverWanted();
        // This should stop this from constantly running
        // I added a debug notify line here to test that.
        UI::Notify("Never wanted off.");
        playerScripts.neverWantedFlag = false;
    }

    // Display text on screen
    // I got this to work
    if (textScripts.drawText)
    {
        textScripts.SetupText();
    }

    if (textScripts.drawCoords)
    {
        // Will this slow down the whole menu?
        // Oops, this wait breaks the menu..
        // TODO, figure out how to slow down the coordinates display, I guess it's fine for now.
        //WAIT(100);
        textScripts.DisplayCoordinates();
    }
    
    //textScripts.SetTextEntry();

    //HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Test");
    //textScripts.SetTextPosition();
    


}

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
