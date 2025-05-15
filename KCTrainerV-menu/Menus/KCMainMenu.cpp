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
            mbCtx.MenuOption("Player", "playermenu", { "Show the player menu." });
            mbCtx.MenuOption("Vehicle", "vehiclemenu", { "Show the vehicle menu." });
            mbCtx.MenuOption("Teleport", "teleportmenu", { "Show the teleport menu." });

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
            mbCtx.Title("Player Menu");



            // This seems to work fine for an invincibility toggle in here like this.
            mbCtx.BoolOption("Invincibility", playerScripts.invincibilityEnabled, { " Turn on/off invincibility" });
            
            
            // For now, I'll just make this a button

            //if (mbCtx.Option("Toggle invincibility", { " Turn on/off invincibility" }))
            //{
            //    playerScripts.ToggleInvincibility();
            //}

            //if (mbCtx.Option("Toggle Never Wanted", { "Turn on/off never wanted" }))
            //{
            //    playerScripts.ToggleNeverWanted();
            //}


            mbCtx.IntOption("Wanted level", playerScripts.wantedLevel, 0, 5, 1, {"Wanted level to set"});
            if (mbCtx.Option("Set Wanted Level", { "Set your wanted level" }))
            {
                playerScripts.SetWantedLevel();
            }

            //mbCtx.BoolOption("BoolOption", checkBoxStatus, { std::string("Boolean is ") + (checkBoxStatus ? "checked" : "not checked") + "." });
            //mbCtx.BoolOption("Heat vision");

            mbCtx.BoolOption("Never wanted", playerScripts.neverWantedEnabled, { "Test for toggling never wanted" });

            // TODO Test this, might work as like a separator.
            // This works kind of like one, I would like to replicate the separators in Menyoo if possible.
            int nothing = 0;
            mbCtx.StringArray("--Visions--", { "" }, nothing);

            if (mbCtx.Option("Toggle Heat vision", { "Turn on/off heat vision" }))
            {
                playerScripts.ToggleHeatVision();
            }


            if (mbCtx.Option("Toggle Night vision", { "Turn on/off night vision" }))
            {
                playerScripts.ToggleNightVision();
            }

            // Moved this out of the player menu, I can use this as a future reference.
            // This works as a submenu nested within a sub menu, can be useful for later.
            // TODO Use this and create a teleport list.
            //mbCtx.MenuOption("Test Sub Menu", "submenutest", { "Show a test submenu within this menu." });

            //if (mbCtx.Option("Toggle bomb bay", { "This will open/close the bomb bay doors in a plane." })) 
            // {
            //    //KCMainScript::ToggleBombBayDoors();
            //    VehicleScripts::ToggleBombBayDoors();
            //}
        }
    );

#pragma endregion




#pragma region VehicleMenu
    submenus.emplace_back("vehiclemenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Vehicle");

            // This didn't seem to do anything
            mbCtx.drawInstructionalButtons();

            if (mbCtx.Option("Repair vehicle", { "Fully fix your vehicle and tires." }))
            {
                vehicleScripts.RepairVehicle();
            }

            // This seems to work
            // TODO Make a vehicle selector menu later.
            if (mbCtx.Option("Spawn Cheetah", { "Spawn a Cheetah near you" }))
            {
                Ped playerPed = playerScripts.GetPlayerPed();
                //vehicleScripts.SpawnVehicle(GAMEPLAY::GET_HASH_KEY(""));
                // Hmm, this Chaos mod function will be very useful.
                static const Hash cheetahHash = "Cheetah"_hash;
                vehicleScripts.SpawnVehicle(cheetahHash);
            }
        }
    );
#pragma endregion

#pragma region TeleportMenu
    submenus.emplace_back("teleportmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            // Hmm, I guess all sub menus need a title, it broke without one.
            mbCtx.Title("Teleport");

            // TODO Make teleport sub menu.
            //mbCtx.StringArray("--Teleports--", { "" }, nothing);
            if (mbCtx.Option("Airport"))
            {
                //KCMainScript::WarpToLocation;
                //PlayerScripts::WarpToLocation(PlayerScripts::AIRPORT_RUNWAY);
                playerScripts.WarpToLocation(PlayerScripts::AIRPORT_RUNWAY);
            }

            if (mbCtx.Option("Save coords to file", {"Save current coordinates and heading to CurrentCoords.txt."}))
            {
                // This function appends the mod path, doing it twice will break it.
                fileFunctions.SaveCoordinatesToFile(Constants::coordsFileName);
            }

            if (mbCtx.Option("Teleport to saved coords", { "Teleport to current coordinates and heading in CurrentCoords.txt." }))
            {
                // Get the mod path and file name, to check if it exists.
                std::filesystem::path modPath = Paths::GetModPath();
                std::string pathString = (modPath / Constants::coordsFileName).string();

                std::string fileErrorString = std::format("{} doesn't exist!", Constants::coordsFileName);

                // I want this to give the user an error if the file doesn't exist.
                // Might crash if it tries to read a null file.
                if (fileFunctions.DoesFileExist(pathString))
                {
                    // This function appends the mod path, doing it twice will break it.
                    fileFunctions.TeleportToSavedCoords(Constants::coordsFileName);
                }
                // If the file doesn't exist
                else {
                    UI::Notify(fileErrorString);
                }
            }

            mbCtx.BoolOption("Display coords", textScripts.drawCoords, {"Toggle drawing coordinates and heading on screen."});

            // Debug functions for the teleport menu, disabled in release builds.
#ifdef DEBUG_MODE
                 mbCtx.MenuOption("Teleport Debug", "debugteleportmenu", { "Debug menu for teleport functions." });
#endif //DEBUG_MODE
            


            // TODO will this work?
            //switch (mbCtx.Option(""))
            //{

            //}
        }
    );
    //

#pragma endregion

#pragma region DebugTeleportSubMenu
#ifdef DEBUG_MODE
    submenus.emplace_back("debugteleportmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            // This works for moving the menu Y coords!!! It'll be very useful for debugging this.

            mbCtx.Title("Teleport Debug");
            // BottomX for headingMenuPosX = 0.205
            // BottomY for headingMenuPosY = 0.965

            int nothing = 0;
            //float stepValue = 0.015f;
            float stepValue = 0.001f;
            mbCtx.StringArray("--Coords display debug--", { "" }, nothing, { "These below items will only show up in debug builds." });
            // Player X position on menu
            mbCtx.FloatOption("Player X position menu X", textScripts.playerXMenuPosX, 0.f, 1.0f, stepValue);
            mbCtx.FloatOption("Player X position menu Y", textScripts.playerXMenuPosY, 0.f, 1.0f, stepValue);

            // Player Y position on menu
            mbCtx.FloatOption("Player Y position menu X", textScripts.playerYMenuPosX, 0.f, 1.0f, stepValue);
            mbCtx.FloatOption("Player Y position menu Y", textScripts.playerYMenuPosY, 0.f, 1.0f, stepValue);

            // Player Z position on menu
            mbCtx.FloatOption("Player Z position menu X", textScripts.playerZMenuPosX, 0.f, 1.0f, stepValue);
            mbCtx.FloatOption("Player Z position menu Y", textScripts.playerZMenuPosY, 0.f, 1.0f, stepValue);

            mbCtx.FloatOption("Heading X position", textScripts.headingMenuPosX, 0.f, 1.0f, stepValue);
            mbCtx.FloatOption("Heading Y position", textScripts.headingMenuPosY, 0.f, 1.0f, stepValue);

        }
    );
#endif //DEBUG_MODE

    //
#pragma endregion



#pragma region PedMenu
    submenus.emplace_back("pedmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Peds");

            if (mbCtx.Option("Set all as cops", { "Set all peds in the area to cops" }))
            {
                pedScripts.SetAllPedsAsCops();
            }
        }
    );
#pragma endregion


#pragma region WorldMenu
    submenus.emplace_back("worldmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("World");

            if (mbCtx.Option("Kill all peds", { "Kill all peds in the area" }))
            {
                Ped playerPed = playerScripts.GetPlayerPed();

                //std::list<Entity> entities;
                // Can be used like this in the for loop:
                // entities.push_back(ped);
                for (auto ped : GetAllPeds())
                {
                    // Check if they are the player and not dead, if so do nothing
                    if (!PED::IS_PED_A_PLAYER(ped) && !ENTITY::IS_ENTITY_DEAD(ped, false))
                    {
                        ENTITY::SET_ENTITY_HEALTH(ped, 0, 0);
                    }

                }

                //entities.push_back(ped);
                //for (auto veh : GetAllVehs())
                //    entities.push_back(veh);
                //for (auto prop : GetAllProps())
                //    entities.push_back(prop);
            }

            if (mbCtx.Option("Blow up vehicles", { "Blow up all vehicles in the area" }))
            {
                Ped playerPed = playerScripts.GetPlayerPed();
                Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerScripts.GetPlayerID(), false);

                for (Vehicle veh : GetAllVehs())
                {
                    // Check if they are the player and not dead, if so do nothing
  /*                  if (!PED::IS_PED_A_PLAYER(ped) && !ENTITY::IS_ENTITY_DEAD(ped, false))
                    {
                        ENTITY::SET_ENTITY_HEALTH(ped, 0, 0);
                    }*/

                    // TODO Add check for if player is in vehicle
                    // This doesn't work, should detect if it is the player vehicle and not blow it up.
                    if (veh != playerVeh)
                    {
                        VEHICLE::EXPLODE_VEHICLE(veh, true, false);
                    }
                    
                }

                //entities.push_back(ped);
                //for (auto veh : GetAllVehs())
                //    entities.push_back(veh);
                //for (auto prop : GetAllProps())
                //    entities.push_back(prop);
            }


        }
    );

#pragma endregion

    // Moved down here to reflect position in menu.
#pragma region SubMenuTest
    submenus.emplace_back("submenutest",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Test Sub menu");

            // TODO Move some of these into MiscScripts.cpp once I create it.

            if (mbCtx.Option("Notify", { "Test notification" }))
            {
                UI::Notify("Test notification");
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


        });
#pragma endregion
#endif //_TEST


  

    return submenus;
}
