#pragma once

namespace Stats 
{
    namespace Cop 
    {
        // Functions related to cop stats
        int GetCopsKilledStat();
        int GetCopsVehiclesBlownUpStat();
        void ProcessCopsKilledDisplay(); // Renamed to clarify its purpose

        // Position on menu
        // Cops killed
        // Menu position X
        extern float copsKilledMenuPosX;
        // Menu Position Y
        extern float copsKilledMenuPosY;

        // Cop cars blown up
        // Menu position X
        extern float copsCarsBlownUpMenuPosX;
        // Menu position Y
        extern float copsCarsBlownUpMenuPosY;

        // Is the display active        
        extern bool isCopsKilledDisplayActive;

        // Internal counters for before dying (these would be global within the namespace)
        extern int copvehiclesBlownUpBeforeDying;
        extern int copsKilledBeforeDying;

        void IncrementCopVehiclesBlownUp();
        void ResetCopVehiclesBlownUpBeforeDying();
        int GetCopVehiclesBlownUpBeforeDying();

        void IncrementCopsKilled();
        void ResetCopsKilledBeforeDying();
        int GetCopsKilledBeforeDying();
    } // namespace Cop

    namespace Kills 
    {

    } // namespace Kills

    // Any top-level Stats functions that don't fit into sub-namespaces
    // void ResetAllStats();
} // namespace Stats
