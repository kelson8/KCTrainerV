#pragma once
#include <string>

class PlayerScripts
{
public:

    std::string GetPlayerHealth();
    std::string GetPlayerVehicleName();
    std::string GetDistanceTraveled();
    static void SetPlayerCoords();

    bool IsPlayerInVehicle();

private:
    float mDistance{ 0.0f };
};

