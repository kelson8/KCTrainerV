#pragma once
#include <string>

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

    bool IsPlayerInVehicle();
    void ChangePlayerVehicleColor();
private:
    float mDistance{ 0.0f };
};
