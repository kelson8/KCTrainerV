#include "PlayerScripts.h"

#include <inc/natives.h>
#include <format>

std::string PlayerScripts::GetPlayerHealth() {
    int health = ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID());

    return std::format("{}", health);
}

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

std::string PlayerScripts::GetDistanceTraveled() {
    return std::format("{:.0f} meters", mDistance);
}

bool PlayerScripts::IsPlayerInVehicle() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    return ENTITY::DOES_ENTITY_EXIST(playerVehicle);

}

// TODO Test these
int player = 0;
int playerID = 0;

int GetPlayerPed()
{
    int myPlayer = PLAYER::PLAYER_ID();
    player = PLAYER::GET_PLAYER_PED(myPlayer);
    return player;
}

int GetPlayerID()
{
    int myPlayer = PLAYER::PLAYER_ID();
    playerID = myPlayer;
    return playerID;
}

/// <summary>
/// Warp player to the specified coords, and a heading
/// </summary>
/// <param name="position"></param>
/// <param name="heading"></param>
void WarpPlayerToCoords(Vector3 position, float heading)
{
    STREAMING::LOAD_SCENE(position);

    //ENTITY::SET_ENTITY_COORDS(GetPlayerPed(), position.x, position.y, position.z, false, false, false, false);
    ENTITY::SET_ENTITY_COORDS(GetPlayerPed(), position, false, false, false, false);
    ENTITY::SET_ENTITY_HEADING(GetPlayerPed(), heading);

}

/// <summary>
/// Set the player coords, TODO Make a teleport menu.
/// </summary>
void PlayerScripts::SetPlayerCoords()
{
    //if (PLAYER::IS_PLAYER_DEAD)
    if (!PLAYER::IS_PLAYER_PLAYING)
    {
        return;
    }

    Vector3 airportRunway = Vector3(-1336.0f, -3044.0f, 13.9f);
    WarpPlayerToCoords(airportRunway, 0.0f);


}