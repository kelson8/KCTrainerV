#include "ScriptMenu.hpp"
#include "Constants.hpp"
#include "KCMenuScript.hpp"
#include "MenuTexture.hpp"
#include "Script.hpp"

#include "Util/Logger.hpp"
#include "Util/Paths.hpp"
#include "Util/UI.hpp"

#include <inc/main.h>
#include <inc/natives.h>

// Menyoo
#include "Natives/natives2.h"

// My scripts
#include "Scripts/PlayerScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"
#include "Scripts/MiscScripts.h"
#include "Scripts/TextScripts.h"

#include "Util/FileFunctions.h"

// Menus
#include "PlayerMenu.h"
#include "PedMenu.h"
#include "TeleportMenu.h"
#include "VehicleMenu.h"
#include "WorldMenu.h"

// Teleports
#include "Teleports/TeleportLocations.h"

// Chaos mod
#include "Util/EntityIterator.h"
#include "Util/Hash.h"
#include "Util/Random.h"

namespace 
{
    // A few state variables for demoing the menu
    bool checkBoxStatus = false;
    float someFloat = 0.0f;
    int someInt = 0;
    int intStep = 1;
    int stringsPos = 0;
    int stepChoice = 0;
    int numberOfOptions = 16;

    // Choice of step size to demonstrate variable precision display
    const std::vector<float> floatSteps = 
    {
        1.0f,
        0.1f,
        0.01f,
        0.001f,
        0.0001f,
        0.00001f,
        0.000001f,
    };

    // Random words to go through in the menu
    const std::vector<std::string> strings = 
    {
        "Hello",
        "World!",
    };
}

// Set for playing the music tracks
int currentMusicTrack;


// Booleans for this file

//void ToggleInvincibility()
//{
//    if (invincibilityEnabled)
//    {
//        
//    }
//}

/*
 * This function builds the menu's submenus, and the submenus are passed into the CScriptMenu constructor.
 * While this provides a cleaner way to define the menu, dynamically created submenu are not possible.
 * CScriptMenu would need to be changed to allow adding and removing submenus on-the-fly.
 */
// TODO Make this a bit neater like in GTAVAddonLoader, not sure how they are doing their menus.
std::vector<CScriptMenu<KCMainScript>::CSubmenu> KCMenu::BuildMenu() 
{

    //PlayerScripts playerScripts;

    // Instances of these objects, this is a great way to use booleans and stuff between files.
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& miscScripts = MiscScripts::GetInstance();

    auto& textScripts = TextScripts::GetInstance();

    auto& fileFunctions = FileFunctions::GetInstance();

    // Menus
    auto& playerMenu = PlayerMenu::GetInstance();
    auto& pedMenu = PedMenu::GetInstance();
    auto& teleportMenu = TeleportMenu::GetInstance();
    auto& vehicleMenu = VehicleMenu::GetInstance();
    auto& worldMenu = WorldMenu::GetInstance();

    std::vector<CScriptMenu<KCMainScript>::CSubmenu> submenus;
    submenus.emplace_back("mainmenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            // Title and Subtitle are required on each submenu.
            mbCtx.Title(Constants::ScriptName);
            mbCtx.Subtitle(std::string("~b~") + Constants::DisplayVersion);

            // This is a normal option. It'll return true when "select" is presed.
            //if (mbCtx.Option("Click me!", { "This will log something to " + Paths::GetModuleNameWithoutExtension(Paths::GetOurModuleHandle()) + ".log" })) 
            //{
            //    UI::Notify("Check the log file!");
            //    LOG(INFO, "\"Click me!\" was selected!");
            //}

            // This will open a submenu with the name "submenu"
            // Oh, I forgot to add the sub menu up here

            // Sub menus
            // These are now located in each of the menu files under the 'Menus' folder, I moved them out of this file.
            // I still have the implementations but that's just for running the build functions in the other files.
            mbCtx.MenuOption("Player", "playermenu", { "Show the player menu." });
            mbCtx.MenuOption("Vehicle", "vehiclemenu", { "Show the vehicle menu." });
            mbCtx.MenuOption("Teleport", "teleportmenu", { "Show the teleport menu." });

#ifdef LUA_TEST
            mbCtx.MenuOption("Teleports (Lua)", "luateleportmenu", { "Show the lua teleports" });
#endif //LUA_TEST

            mbCtx.MenuOption("Ped", "pedmenu", { "Show the ped menu." });
            mbCtx.MenuOption("World", "worldmenu", { "This submenu contains items for the world menu." });

            mbCtx.MenuOption("Test Sub Menu", "submenutest", { "Testing menu." });

            // Showing a non-scrolling aligned item is also possible, if the vector only contains one element.
            int nothing = 0;
            mbCtx.StringArray("Version", { Constants::DisplayVersion }, nothing,
                { "Thanks for checking out this menu!", 
                "Author: kelson8", 
                "Menu base by: ikt", 
                "Some features from Menyoo and Chaos Mod."});
        });


    // This should enable my test features.
#define _TEST
#ifdef _TEST
    // This fixes the issue with using some of these variables without static:
    // I added the & symbol where this is:  [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
    // https://stackoverflow.com/questions/26903602/an-enclosing-function-local-variable-cannot-be-referenced-in-a-lambda-body-unles

#pragma region PlayerMenu

    submenus.emplace_back("playermenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            // Run the menu from PlayerMenu.cpp, seems to work fine.
            playerMenu.Build(mbCtx, context);
        }
    );


#pragma endregion

#pragma region VehicleMenu
    submenus.emplace_back("vehiclemenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.Build(mbCtx, context);
        }
    );

#pragma region VehicleCategorySubMenu

    // This works
    submenus.emplace_back("VehicleCategorySubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildVehicleCategorySubmenu(mbCtx, context);
        }
    );

    submenus.emplace_back("SportsVehicleCategorySubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildSportsVehicleCategorySubmenu(mbCtx, context);
        }
    );

    submenus.emplace_back("SuperVehicleCategorySubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildSuperVehicleCategorySubmenu(mbCtx, context);
        }
    );

#pragma region TeleportMenu
    submenus.emplace_back("teleportmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.Build(mbCtx, context);
        }
    );
    //

#pragma endregion

    // Moved teleport locations down here, so it doesn't clutter up the teleport menu.
#pragma region TeleportLocationsSubMenu

    submenus.emplace_back("teleportlocations",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildTeleportLocationsSubMenu(mbCtx, context);
        }
    );

            // TODO Migrate to using this format for teleports. 
            // First I'll need to fix it in TeleportLocations.cpp, TeleportLocations.h, PlayerScripts.cpp, and PlayerScripts.h:
#ifdef NEW_TELEPORTS
#pragma region AirportsTeleportSubMenu
        submenus.emplace_back("AirportsTeleportSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildAirportSubMenu(mbCtx, context);
        });

#endif //!NEW_TELEPORTS

#pragma endregion


#pragma region SafehousesTeleportSubmenu
#ifdef NEW_TELEPORTS
    submenus.emplace_back("SafehousesTeleportSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildSafehousesSubMenu(mbCtx, context);
            
#pragma endregion


        });

#endif //NEW_TELEPORTS

#pragma endregion

#pragma region DebugTeleportSubMenu
#ifdef DEBUG_MODE
    submenus.emplace_back("debugteleportmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildDebugSubMenu(mbCtx, context);
        }
    );
#endif //DEBUG_MODE

    //
#pragma endregion

#pragma region LuaTeleportMenu
#ifdef LUA_TEST
    submenus.emplace_back("luateleportmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildLuaTeleportSubMenu(mbCtx, context);
        }
    );
#endif //LUA_TEST

#pragma endregion




#pragma region PedMenu
    submenus.emplace_back("pedmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            pedMenu.Build(mbCtx, context);
        }
    );
#pragma endregion


#pragma region WorldMenu
    submenus.emplace_back("worldmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            worldMenu.Build(mbCtx, context);
        }
    );

#pragma endregion

    // Moved down here to reflect position in menu.
    // TODO Figure out what menu to move this into, for now I'll leave it in the KCMainMenu file.
    // ---
    // TODO Move chaos mode features into their own sub menu, possibly name it Chaos Mod or Chaos Features?
    // TODO Also add credits to the Chaos mod features menu, such as an about button or something at the bottom.
    
#pragma region SubMenuTest
    submenus.emplace_back("submenutest",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Test Sub menu");

            if (mbCtx.Option("Notify", { "Test notification" }))
            {
                UI::Notify("Test notification");
            }

            int nothing = 0;
            mbCtx.StringArray("--Music--", { "" }, nothing);

            // Incremement the max number for this as I add more into the Enums.h and MiscScripts.cpp
            // To add more to this:
            // 1. Add a value with a number into Enums.h
            // 2. Add a value into the std::map in MiscScript.cpp in the PlayTestMusic function.
            //mbCtx.IntOption("Music track", currentMusicTrack, 1, 26, 1, {"List of music tracks within Enums.h in the code."});
            mbCtx.IntOption("Music track", currentMusicTrack, 1, musicTracksCount, 1, {"List of music tracks within Enums.h in the code."});

            // TODO Test this, figure out a good way to do this.
            if (mbCtx.Option("Play Test sound"))
            {
                
                //miscScripts.PlayTestMusic(21);
                // Seems to be the music that sometimes happens when flying.
                //miscScripts.PlayTestMusic(CHASE_PARACHUTE_START);
                //miscScripts.PlayTestMusic(SHOOTING_RANGE_START);
                miscScripts.PlayTestMusic(static_cast<MusicTracks>(currentMusicTrack));
            }


            if (mbCtx.Option("AW Lobby music", { "Play the arena war lobby music" }))
            {
                miscScripts.PlayArenaWarLobbyMusic();
            }

            if (mbCtx.Option("Stop music", { "Stops the music currently playing" }))
            {
                miscScripts.StopArenaWarLobbyMusic();
            }

            // This didn't seem to play the end credits music.
            // Taken from MiscRollCredits.cpp in Chaos Mod
            if (mbCtx.Option("Start credits music", { "Start the end credits music" }))
            {
                miscScripts.StartCreditsMusic();
            }

            if (mbCtx.Option("Stop credits music", { "Stop the end credits music" }))
            {
                miscScripts.StopCreditsMusic();
            }

            // Ped tests
            mbCtx.BoolOption("Peds attack player", miscScripts.isPedsAttackEnabled, { "Make all peds in the area attack you" });
            mbCtx.BoolOption("Peds drive crazy", miscScripts.isCrazyPedDrivingEnabled, {
                "Make all peds in the area drive with the rushed driving style." });

#ifdef CHAOSMOD_FEATURES
            if (mbCtx.Option("Set peds in mowers", { "Place all peds in the area into lawn mowers" }))
            {
                miscScripts.SetAllPedsInMowers();
            }
#endif // CHAOSMOD_FEATURES
            

            mbCtx.BoolOption("Toggle airstrike test", miscScripts.airStrikeRunning, { "Toggle the airstrikes on/off" });

            //if (mbCtx.Option("Start airstrike test", { "Start a test for an airstrike" }))
            //{
            //    //miscScripts.StartAirstrikeTest();
            //    miscScripts.airStrikeRunning = true;
            //}

            //if (mbCtx.Option("Stop airstrike test", { "Stop the test for an airstrike" }))
            //{
            //    miscScripts.StopAirstrikeTest();
            //    miscScripts.airStrikeRunning = false;
            //}

            mbCtx.BoolOption("Draw text on screen", textScripts.drawText, { "Toggle test text to draw on screen." });


#ifdef EXTRA_FEATURES
            // TODO Add this boolean option
            //mbCtx.BoolOption("Toggle sky", &miscScripts.toggleSky);
            
            // Toggle sky
            if (mbCtx.Option("Enable Sky"))
            {
                miscScripts.EnableSky();
            }

            if (mbCtx.Option("Disable Sky"))
            {
                miscScripts.DisableSky();
            }

            // Toggle snow
            if (mbCtx.Option("Enable Snow"))
            {
                miscScripts.EnableSnow();
            }

            if (mbCtx.Option("Disable Snow"))
            {
                miscScripts.DisableSnow();
            }

            // Toggle force field
            mbCtx.BoolOption("Toggle forcefield", miscScripts.isForceFieldEnabled, { "Turn on/off the forcefield for the player." });

            // I would set a boolean for this but it requires items that I don't think they can be run in a tick all the time.
            // Toggle tv on/off
            if (mbCtx.Option("Enable TV"))
            {
                miscScripts.EnableTv();
            }

            if (mbCtx.Option("Disable TV"))
            {
                miscScripts.DisableTv();
            }

            //if (mbCtx.Option("Enable Forcefield"))
            //{
            //    miscScripts.EnableSky();
            //}

            //if (mbCtx.Option("Disable Forcefield"))
            //{
            //    miscScripts.EnableSky();
            //}
#endif //EXTRA_FEATURES

            if (mbCtx.Option("Test Fade out/in", { "Test for fading the screen out and back in" }))
            {
                playerScripts.TestFade();
            }

            // TODO Figure out implementation for this, shouldn't be too hard.
            //if (mbCtx.Option("Test reload menu"), { "Test for reloading the menu config, may crash." })
            //{
            //    
            //}


        });
#pragma endregion
#endif //_TEST


  

    return submenus;
}
