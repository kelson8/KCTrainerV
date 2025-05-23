#pragma once

namespace Stats 
{
    namespace Cop 
    {

        // TODO Add settings class in here for separating the menu positions and other items
#ifdef STAT_CLASS_TEST
        class Settings 
        {
        public:
            static Settings& GetInstance() {
                static Settings instance;
                return instance;
            }

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

            bool isCopsKilledDisplayActive = false;

        private:
            Settings() = default;
            Settings(const Settings&) = delete;
            Settings& operator=(const Settings&) = delete;

        };

#endif

        void Tick();

        // Functions related to cop stats
        int GetCopsKilledStat();
        int GetCopsVehiclesBlownUpStat();

        void ResetCopsKilledMenuPositions();
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
