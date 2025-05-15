#include "KCMenuScript.hpp"

#include "Scripts/PlayerScripts.h"
#include "Scripts/MiscScripts.h"

#include "Scripts/TextScripts.h"

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

    // TODO Possibly add check for invincibility in here.
    // Add other items that need to run constantly.
    //if (playerScripts.IsNeverWantedEnabled())
    //{
    //    PLAYER::SET_MAX_WANTED_LEVEL(0);
    //}

    // TODO Figure out how to draw text on the screen without breaking the menu.
    // TODO Add a toggle for this, for now I'll just test it without one.
    // Display text on screen

    // Oops, this just breaks the menu fully.
    // This breaks the menu if i turn the checkbox on? I probably am not using this right or something..
    if (textScripts.drawText)
    {
        textScripts.SetupText();
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
