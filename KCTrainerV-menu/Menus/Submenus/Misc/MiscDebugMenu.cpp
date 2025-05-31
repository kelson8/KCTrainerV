#include "pch.h"

#include "MiscDebugMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

// My scripts

#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"
#include "Scripts/MiscScripts.h"
#include "Scripts/TextScripts.h"

#include "Scripts/WorldScripts.h"

#include "Scripts/Stats.h"

#include "Scripts/NotificationManager.h"
#include "Util/FileFunctions.h"
#include "defines.h"

// Player
#include "Scripts/PlayerScripts.h"
#include "Scripts/Player/PlayerTeleportScripts.h"

// Marker
#include "Scripts/Markers/MarkerScripts.h"

// Menyoo
#include "Scripts/Extras/Game.h"
#include "Scripts/Extras/Classes/GTAblip.h"
#include "Scripts/Extras/World.h"
#include "Scripts/Extras/Classes/Model.h"
#include "Memory/GTAmemory.h"

// Misc
#include "Scripts/Misc/MiscMusicScripts.h"
#include "Scripts/Misc/MiscExtraFeatures.h"
#include "Scripts/Misc/IDGun.h"

namespace Misc
{
	/// <summary>
	/// Build the misc debug submenu.
	/// </summary>
	/// <param name="mbCtx"></param>
	/// <param name="context"></param>
    void DebugMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
    {
        mbCtx.Title("Debug Sub Menu");

        // Scripts
        auto& vehicleScripts = VehicleScripts::GetInstance();
        auto& pedScripts = PedScripts::GetInstance();
        auto& textScripts = TextScripts::GetInstance();

        // Player
        auto& playerScripts = PlayerScripts::GetInstance();
        auto& playerTeleportScripts = Scripts::Player::Positions::GetInstance();

        // Functions
        auto& fileFunctions = FileFunctions::GetInstance();

        Ped player = PLAYER_PED_ID();

        // List of text formats:
        // https://docs.fivem.net/docs/game-references/text-formatting/

        // TODO Make this get text from a file.
        if (mbCtx.Option("Notify", { "Test notification" }))
        {
            //UI::Notify("Test notification");
            // Shows a wanted star and RockStar logo
            //UI::Notify("Wanted: ~ws~ ~EX_R*~");
            // Shows 6 wanted stars
            //UI::Notify("~ws~ ~ws~ ~ws~ ~ws~ ~ws~ ~ws~");
            std::string notificationText = NotificationManager::GetRandomNotification();
            UI::Notify(notificationText);
        }

        if (mbCtx.Option("Notify phone sound", { "Test notification with phone sound" }))
        {
            textScripts.NotificationBottomLeft("Test notification");
        }

        mbCtx.BoolOption("ID Gun test", MiscScripts::IDGun::isIdGunEnabled, { "Toggle the ID Gun test from FiveM" });

#ifdef DEBUG_MODE
        if (mbCtx.Option("Menyoo test", { "Run a test with Menyoo classes" }))
        {
            MiscScripts::EXFeatures::MenyooTest();
        }
#endif


#ifdef CHAOSMOD_FEATURES
        if (mbCtx.Option("Set peds in mowers", { "Place all peds in the area into lawn mowers" }))
        {
            miscScripts.SetAllPedsInMowers();
        }
#endif // CHAOSMOD_FEATURES

        mbCtx.BoolOption("Toggle airstrike test", MiscScripts::EXFeatures::airStrikeRunning, { "Toggle the airstrikes on/off" });

#ifdef EXTRA_FEATURES

        // Toggle force field
        // Seems to crash, disabled.
        //mbCtx.BoolOption("Toggle forcefield", MiscScripts::EXFeatures::isForceFieldEnabled, { "Turn on/off the forcefield for the player." });

        // I would set a boolean for this but it requires items that I don't think they can be run in a tick all the time.
        // Toggle tv on/off
        if (mbCtx.Option("Enable TV"))
        {
            MiscScripts::EXFeatures::EnableTv();
        }

        if (mbCtx.Option("Disable TV"))
        {
            MiscScripts::EXFeatures::DisableTv();
        }

#endif //EXTRA_FEATURES

        // TODO Figure out why this function doesn't work.
        if (mbCtx.Option("Test Fade out/in", { "Test for fading the screen out and back in" }))
        {
            playerScripts.TestFade();
            //Vector3 targetLocation = Teleports::Positions::vSafeHouseLocations.at("Michael's House");
            //MiscScripts::Fade::InitiateTeleportFade(targetLocation);
        }

#ifdef DEBUG_MODE

        // This works for outputting text to the console, fixing the wait fixed this also.
        if (mbCtx.Option("On screen keyboard", { "Show a on screen keyboard" }))
        {
            std::string inputString = Game::InputBox("DEFAULT", 64);
            if (inputString.length() > 0)
            {
                UI::Notify(inputString);
                log_output(inputString);
                //std::cout << inputString << std::endl;
            }
            else {
                log_output("Input string doesn't exist!");
                //std::cout << "Input string doesn't exist!" << std::endl;
            }

        }

        // This task seems to give the player a parachute, I wonder if it works on other peds?
        if (mbCtx.Option("Skydive test", { "Skydive cheat replicated" }))
        {
            Vector3 currentCoords = playerTeleportScripts.GetPlayerCoords();
            Vector3 newPos = Vector3(currentCoords.x, currentCoords.y, currentCoords.z + 100);
            // Set the player into the air first
            playerTeleportScripts.SetPlayerCoords(newPos);
            // Then set the task
            TASK_SKY_DIVE(player, true);
            //WAIT(1000);
            // Well this didn't work, they should automatically use the parachute 
            //TASK_PARACHUTE(player, false, false);
        }
    }

#endif // DEBUG_MODE

} // namespace Misc