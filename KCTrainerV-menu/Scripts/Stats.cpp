#include "pch.h"

#include "Constants.hpp"
#include "Stats.h"

#include "PlayerScripts.h"
#include "TextScripts.h"

#include "Util/UI.hpp"
#include "Util/Logger.hpp"

#include <inc/natives.h>
#include <format>

#include "../Util/Hash.h"
#include "../Util/Util.hpp"

// Chaos Mod
#include "Util/EntityIterator.h"

// Menyoo
#include "GTAped.h"
#include "GTAentity.h"

// From Menyoo, makes it to where I don't have to type the namespaces for the natives.
#include "Natives/natives2.h"

//----------- Begin Stats --------------//
// TODO Possibly move these into StatScripts.cpp?

// TODO Setup option to get police killed and cop cars blown up stats
// When getting the stats, try to subtract the difference, 
// so if there is 1000 cops killed in total, and 100 cops killed without dying, set the value to 100.

/// <summary>
/// TODO Set this up
/// Also, compact these, possibly make a for loop?
/// Credit to this gtaforums thread for some of these stats: https://gtaforums.com/topic/802525-changing-stats/
/// Full stat list: https://vespura.com/fivem/gta-stats/
/// </summary>
void SetStats()
{
    // Useful stats:
    /*
        CARS_COPS_EXPLODED
        CARS_EXPLODED
        COPS_KILLS_ON_SPREE

        KILLS
        KILLS_ARMED
        KILLS_BY_OTHERS
        KILLS_COP
        KILLS_ENEMY_GANG_MEMBERS
        KILLS_FRIENDLY_GANG_MEMBERS
        KILLS_IN_FREE_AIM
        KILLS_INNOCENTS - Number of peds killed
        KILLS_STEALTH
        KILLS_SWAT

    */
    // From Menyoo:
    // std::pair<std::string, std::string> vCharNames[3] = { { "SP0_", "Michael" },{ "SP1_", "Franklin" },{ "SP2_", "Trevor" } };

    // SP0 = Michael
    // SP1 = Franklin
    // SP2 = Trevor
    // I removed the extra values out of here, there was a bunch of duplicates.
    /*
    //MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(GAMEPLAY::GET_HASH_KEY("stats_controller"));
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_SPECIAL_ABILITY_UNLOCKED"), 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STAMINA"),                  100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STEALTH_ABILITY"),          100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_LUNG_CAPACITY"),            100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_FLYING_ABILITY"),           100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_SHOOTING_ABILITY"),         100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STRENGTH"),                 100, true);
    STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_WHEELIE_ABILITY"),          100, true);
    */
}

//     //CARS_COPS_EXPLODED
    //    CARS_EXPLODED
    //    COPS_KILLS_ON_SPREE

    //    KILLS
    //    KILLS_ARMED
    //    KILLS_BY_OTHERS
    //    KILLS_COP

// So, SP0 is actually Micheal? I had these swapped..
// TODO Set these below to get which player is playing, Michael = SP0, Franklin = SP1, Trevor = SP2
// TODO Compact these, I'm sure there is a better way to do this then what I am doing.

// From Menyoo:
// std::pair<std::string, std::string> vCharNames[3] = { { "SP0_", "Michael" },{ "SP1_", "Franklin" },{ "SP2_", "Trevor" } };

namespace Stats 
{
    namespace Cop
    {
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

        /// <summary>
        /// Get the cops kiled stat
        /// <param name="character">The player to select the stat for, using the PlayerModels enum class.</param>
        /// </summary>
        /// <returns>The amount of cops killed</returns>
        //int PlayerScripts::GetCopsKilledStat()
        //int PlayerScripts::GetCopsKilledStat(PlayerModels character)
        int GetCopsKilledStat()
        {
            int copsKilledStat;

            // TODO Test this, this should get the current player instead of me manually specifying it.
            PlayerModels currentPlayer = PlayerScripts::GetCurrentPlayerModel();

            //switch (character)
            switch (currentPlayer)
            {
            case PlayerModels::MICHEAL: // SP0
                STAT_GET_INT("SP0_KILLS_COP"_hash, &copsKilledStat, -1);
                break;

            case PlayerModels::FRANKLIN: // SP1
                STAT_GET_INT("SP1_KILLS_COP"_hash, &copsKilledStat, -1);
                break;

            case PlayerModels::TREVOR: // SP2
                STAT_GET_INT("SP2_KILLS_COP"_hash, &copsKilledStat, -1);
                break;

            default:
                copsKilledStat = 0;
                break;
            }

            return copsKilledStat;
        }

        /// <summary>
        /// Get the cops cars blown up.
        /// </summary>
        /// <returns>The amount of cop cars blown up</returns>
        /// <param name="character">The player to select the stat for, using the PlayerModels enum class.</param>
        //int PlayerScripts::GetCopsVehiclesBlownUpStat()
        /// <returns></returns>
        //int PlayerScripts::GetCopsVehiclesBlownUpStat(PlayerModels character)
        int GetCopsVehiclesBlownUpStat()

        {
            int copsVehiclesBlownUpStat;

            PlayerModels currentPlayer = PlayerScripts::GetCurrentPlayerModel();

            //switch (character)
            switch (currentPlayer)
            {
            case PlayerModels::MICHEAL: // SP0
                STAT_GET_INT("SP0_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);
                break;

            case PlayerModels::FRANKLIN: // SP1
                STAT_GET_INT("SP1_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);
                break;

            case PlayerModels::TREVOR: // SP2
                STAT_GET_INT("SP2_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);
                break;

            default:
                copsVehiclesBlownUpStat = 0;
                break;
            }

            //STAT_GET_INT("SP0_CARS_COPS_EXPLODED"_hash, &copsVehiclesBlownUpStat, -1);

            return copsVehiclesBlownUpStat;

        }

#pragma region StatLoops


        // Stat loops

        //------------ Cops cars blown up Logic --------------/
        // Set default value for this to 0
        int copvehiclesBlownUpBeforeDying = 0;


        /// <summary>
        /// Add one to the cop copvehiclesBlownUpBeforeDying stat
        /// </summary>

        void IncrementCopVehiclesBlownUp()
        {
            copvehiclesBlownUpBeforeDying++;
        }

        /// <summary>
        /// Reset the cop vehicles blown up stat back to 0
        /// </summary>
        void ResetCopVehiclesBlownUpBeforeDying()
        {
            copvehiclesBlownUpBeforeDying = 0;
        }

        /// <summary>
        /// Get the current copvehiclesBlownUpBeforeDying value.
        /// </summary>
        int GetCopVehiclesBlownUpBeforeDying() {
            return copvehiclesBlownUpBeforeDying;
        }


        //------------ Cops killed logic --------------/

        // Set default value for this to 0
        int copsKilledBeforeDying = 0;

        /// <summary>
        /// Add one to the cops killed stat
        /// </summary>
        void IncrementCopsKilled() {
            copsKilledBeforeDying++;
        }

        /// <summary>
        /// Reset the cops killed stat back to 0
        /// </summary>
        void ResetCopsKilledBeforeDying() {
            copsKilledBeforeDying = 0;
        }

        /// <summary>
        /// Get the current copsKilledBeforeDying value.
        /// </summary>
        int GetCopsKilledBeforeDying() {
            return copsKilledBeforeDying;
        }

        //------------ End Cops killed logic --------------/

        //------------ Cops killed stat display -------------/ 
        //------------ And Cop vehicles blown up stat display --------------/

        /// <summary>
        /// This works as a system that increments 
        /// depending on how many cops/swat you kill, for now this only prints to the console with std::cout.
        /// Eventually, I'll make a function that tracks how many cop vehicles you blow up.
        /// This will draw to the screen and print to the console with std::cout also.
        /// </summary>
        void ProcessCopsKilledDisplay()
        {
            auto& textScripts = TextScripts::GetInstance();

            // Cops killed position on menu, this value is now in the header
            //float copsKilledMenuPosX = 0.190f;
            //float copsKilledMenuPosY = 0.949f;
            auto& playerScripts = PlayerScripts::GetInstance();

            PlayerModels currentPlayer = PlayerScripts::GetCurrentPlayerModel();

            Ped playerPed = playerScripts.GetPlayerPed();
            int playerID = playerScripts.GetPlayerID();


            // TODO Make these into functions in this class.
            bool isPlayerDead = IS_ENTITY_DEAD(playerPed, false);
            bool isPlayerBeingArrested = IS_PLAYER_BEING_ARRESTED(playerID, true);

            //---
            // Get the "KILLS_COP" and "KILLS_SWAT" stats.
            //---
            //int copsKilled = 0;
            //int swatKilled = 0;

            //int copsKilled = PlayerScripts::GetPlayerStat(currentPlayer, "KILLS_COP");
            int copsKilled = playerScripts.GetPlayerStat(currentPlayer, "KILLS_COP");
            int swatKilled = playerScripts.GetPlayerStat(currentPlayer, "KILLS_SWAT");

            int copCarsBlownUp = playerScripts.GetPlayerStat(currentPlayer, "CARS_COPS_EXPLODED");

            // This might be the stat to get the total helicopters exploded:
            // HELIS_EXPLODED

            // Combine the copsKilled and swatKilled stats
            int totalCopsKilled = copsKilled + swatKilled;

            //---
            // Keep track of previous value
            //---
            //static int previousKills = copsKilled;
            static int previousKills = totalCopsKilled;

            int totalCopCarsBlownUp = copCarsBlownUp;
            static int previousCopCarsBlownUp = totalCopCarsBlownUp;



            //---
            // Check if the cops/swat killed stat has been increased.
            //---
            //if (copsKilled > previousKills)
            if (totalCopsKilled > previousKills)
            {
                IncrementCopsKilled();
                // Update the previousKills value
                //previousKills = copsKilled;
                previousKills = totalCopsKilled;
            }

            //---
            // Check if the cops blown up stat has been increased
            //---
            if (totalCopCarsBlownUp > previousCopCarsBlownUp)
            {
                IncrementCopVehiclesBlownUp();
                // Update the previousCopCarsBlownUp value
                previousCopCarsBlownUp = totalCopCarsBlownUp;
            }

            //---
            // Check if the player has died or been busted.
            //---
            if (IS_ENTITY_DEAD(playerPed, false) || IS_PLAYER_BEING_ARRESTED(playerID, true))
            {
                // Reset the previous kills back to 0.
                ResetCopsKilledBeforeDying();
                previousKills = totalCopsKilled;

                // Reset the previous cop cars blown up to 0
                ResetCopVehiclesBlownUpBeforeDying();
                previousCopCarsBlownUp = totalCopCarsBlownUp;
            }

            //---
            // Display the current kills count
            //---
            int copsKilledThisLife = GetCopsKilledBeforeDying();
            // Store it in a string stream
            std::stringstream ss;
            ss << "Cops killed this life: " << copsKilledThisLife;
            std::string copsKilledString = ss.str();

            // Print to console
            std::cout << copsKilledString << std::endl;

            // Display to screen
            // This displays the value to the screen

            // This shouldn't draw if the player is dead or being arrested.
            if (!isPlayerDead || !isPlayerBeingArrested)
            {
                textScripts.SetTextEntry(copsKilledString.c_str(), 255, 255, 255, 255);

                textScripts.TextPosition(copsKilledMenuPosX, copsKilledMenuPosY);
            }

            //---
            // Display the current cop vehicles blown up
            //---
            // Well this part isn't working yet, this displays but the value doesn't update.
            int copCarsBlownUpThisLife = GetCopVehiclesBlownUpBeforeDying();
            std::stringstream ss1;
            ss1 << "Cop cars blown up this life: " << copCarsBlownUpThisLife;
            // TODO Test
            //ss1 << "Total cop cars blown up: " << copCarsBlownUp;
            std::string copCarsBlownUpString = ss1.str();

            // Disabled, print to console
            //std::cout << copCarsBlownUpString << std::endl;

            // This shouldn't draw if the player is dead or being arrested.
            if (!isPlayerDead || !isPlayerBeingArrested)
            {
                // Display to screen
                textScripts.SetTextEntry(copCarsBlownUpString.c_str(), 255, 255, 255, 255);

                textScripts.TextPosition(copsCarsBlownUpMenuPosX, copsCarsBlownUpMenuPosY);

            }

        }
    } // namespace Cop

} // namespace Stats

#pragma endregion // StatLoops

//------------ End Cops killed stat display --------------/

//----------- End Stats --------------//

#pragma endregion //Stats
