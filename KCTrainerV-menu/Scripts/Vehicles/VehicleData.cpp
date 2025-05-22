#include "pch.h"

#include "VehicleData.h"

#include "Constants.hpp"

#include "Util/Enums.h"

// Chaos mod
#include "Util/Hash.h"

// Using the Enums from Menyoo and matching car names from the FiveM website:
// https://docs.fivem.net/docs/game-references/vehicle-references/vehicle-models/

// This is my new vehicle list system.

#pragma region SportsVehicleList

const std::vector<VehicleInfo> VehicleData::g_sportsVehicles =
{
    {"Jester Classic", VEHICLE_JESTER2},
    {"Elegy RH8", VEHICLE_ELEGY2},
    {"Cheetah", VEHICLE_CHEETAH},
    
};

#pragma endregion

#pragma region SportsClassicsVehicleList

// This list is mostly complete.
const std::vector<VehicleInfo> VehicleData::g_sportsClassicsVehicles =
{

     {"190z", VEHICLE_Z190},
     {"Ardent", VEHICLE_ARDENT},

     {"Cheetah Classic", VEHICLE_CHEETAH2},
     {"Coquette Classic", VEHICLE_COQUETTE2},

     {"Deluxo", VEHICLE_DELUXO},

     {"Fränken Stange", VEHICLE_BTYPE2},

     {"GT500", VEHICLE_GT500},
     {"Infernus Classic", VEHICLE_INFERNUS2},
     {"JB 700", VEHICLE_JB700},
     {"JB 700W", VEHICLE_JB7002},

     {"Roosevelt", VEHICLE_BTYPE},
     {"Roosevelt Valor", VEHICLE_BTYPE3},

     {"Stinger", VEHICLE_STINGER},
     {"Stinger GT", VEHICLE_STINGERGT},
     {"Stromberg", VEHICLE_STROMBERG},

     {"Toreador", VEHICLE_TOREADOR},
     {"Turismo Classic", VEHICLE_TURISMO2},
     
     {"Viseris", VEHICLE_VISERIS},
     
     {"Zion Classic", VEHICLE_ZION3},
     {"Z-Type", VEHICLE_ZTYPE},
};

#pragma endregion

#pragma region SuperVehicleList

const std::vector<VehicleInfo> VehicleData::g_superVehicles =
{
    {"Scramjet", VEHICLE_SCRAMJET},
    {"T20",      VEHICLE_T20},
    {"Zentorno", VEHICLE_ZENTORNO},

};

#pragma endregion

#pragma region SuvVehiclesList

const std::vector<VehicleInfo> VehicleData::g_suvVehicles =
{
    {"Baller LE", VEHICLE_BALLER3},
};

#pragma endregion
