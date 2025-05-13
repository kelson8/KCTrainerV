#include "VehicleScripts.h"
#include <inc/natives.h>
#include <format>


void VehicleScripts::ToggleBombBayDoors()
{
    Player player = PLAYER::PLAYER_PED_ID();
    // I forgot about GET_ENTITY_MODEL
    // https://www.unknowncheats.me/forum/grand-theft-auto-v/144819-grand-theft-auto-scripting-125.html
    Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(player, false);
    Hash playerVehHash = ENTITY::GET_ENTITY_MODEL(playerVeh);

    if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && VEHICLE::IS_THIS_MODEL_A_PLANE(playerVehHash))
    {
        if (VEHICLE::GET_ARE_BOMB_BAY_DOORS_OPEN(playerVeh)) {
            VEHICLE::OPEN_BOMB_BAY_DOORS(playerVeh);
        }
        else
        {
            VEHICLE::CLOSE_BOMB_BAY_DOORS(playerVeh);
        }
    }

}

void VehicleScripts::ChangePlayerVehicleColor() {
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    if (ENTITY::DOES_ENTITY_EXIST(playerVehicle)) {
        VEHICLE::SET_VEHICLE_COLOUR_COMBINATION(playerVehicle,
            MISC::GET_RANDOM_INT_IN_RANGE(0,
                VEHICLE::GET_NUMBER_OF_VEHICLE_COLOURS(playerVehicle)
            )
        );
    }
}