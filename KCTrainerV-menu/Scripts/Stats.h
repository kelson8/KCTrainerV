#pragma once
#include "Constants.hpp"

class Stats
{

public:
    // Instance of the Stats
    static Stats& GetInstance()
    {
        static Stats instance;
        return instance;
    }

    // Stat tests
    //int GetCopsKilledStat(PlayerModels character);
    int GetCopsKilledStat();
    //int GetCopsVehiclesBlownUpStat(PlayerModels character);
    int GetCopsVehiclesBlownUpStat();

    // For new cops killed stat system, this now displays on the screen.
    void ProcessCopsKilled();

    // Position on menu
    // Menu position X
    float copsKilledMenuPosX = 0.204f;
    // Menu Position Y
    float copsKilledMenuPosY = 0.963f;

    // Cop cars blown up
    //float copsCarsBlownUpMenuPosX = 0.150f;
    float copsCarsBlownUpMenuPosX = 0.220f;
    //float copsCarsBlownUpMenuPosY = 0.870f;
    float copsCarsBlownUpMenuPosY = 0.935f;

    // Is the display active
    bool isCopsKilledDisplayActive = false;

private:
    Stats() {} // Private constructor to prevent external instantiation
    Stats(const Stats&) = delete;
    Stats& operator=(const Stats&) = delete;

    // New methods for tracking cops killed before dying
    // Cop cars blown up
    static int copvehiclesBlownUpBeforeDying;
    void IncrementCopVehiclesBlownUp();
    void ResetCopVehiclesBlownUpBeforeDying();
    int GetCopVehiclesBlownUpBeforeDying();

    // Cop kills
    static void IncrementCopsKilled();
    static void ResetCopsKilledBeforeDying();
    static int GetCopsKilledBeforeDying();

    static int copsKilledBeforeDying; // Static variable to track the count
};

