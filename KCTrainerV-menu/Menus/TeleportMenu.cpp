#include "pch.h"
#include "TeleportMenu.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/TextScripts.h"

#include "../Util/Paths.hpp"
#include "../Util/Logger.hpp"
#include "../Util/UI.hpp"

#include "../Util/FileFunctions.h"

// Teleports
#include "Teleports/TeleportLocations.h"

#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

void TeleportMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& fileFunctions = FileFunctions::GetInstance();
    auto& textScripts = TextScripts::GetInstance();

    // Hmm, I guess all sub menus need a title, it broke without one.
    mbCtx.Title("Teleport");

    // TODO Make teleport sub menu.
    //mbCtx.StringArray("--Teleports--", { "" }, nothing);

    mbCtx.MenuOption("Locations", "teleportlocations", { "Show a list of teleport locations" });

    if (mbCtx.Option("Save coords to file", { "Save current coordinates and heading to CurrentCoords.txt." }))
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

    mbCtx.BoolOption("Display coords", textScripts.drawCoords, { "Toggle drawing coordinates and heading on screen." });

    // Debug functions for the teleport menu, disabled in release builds.
#ifdef DEBUG_MODE
    mbCtx.MenuOption("Teleport Debug", "debugteleportmenu", { "Debug menu for teleport functions." });
#endif //DEBUG_MODE

}

void TeleportMenu::BuildTeleportLocationsSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    mbCtx.Title("Teleport Locations");

#ifdef NEW_TELEPORTS
    mbCtx.MenuOption("Airport", "AirportsTeleportSubmenu", { "Show a list of airport teleport locations." });
    mbCtx.MenuOption("Safehouses", "SafehousesTeleportSubmenu", { "Show a list of safehouse teleport locations." });

#endif //NEW_TELEPORTS

#ifndef NEW_TELEPORTS
    if (mbCtx.Option("Airport"))
    {
        playerScripts.WarpToLocation(TeleportLocation::AIRPORT_RUNWAY);
    }
    if (mbCtx.Option("Hospital LS1 Rooftop"))
    {
        playerScripts.WarpToLocation(TeleportLocation::HOSPITAL_LS1_ROOFTOP);
    }

#ifdef LOAD_IPLS
    if (mbCtx.Option("Singleplayer Yacht"))
    {
        playerScripts.WarpToLocation(TeleportLocation::SP_YACHT1);
    }
#endif

    if (mbCtx.Option("Richards Majestic Movie Studio"))
    {
        playerScripts.WarpToLocation(TeleportLocation::RICHARDS_MAJESTIC_MOVIE_STUDIO);
    }
        

    if (mbCtx.Option("Train station #1"))
    {
        playerScripts.WarpToLocation(TeleportLocation::TRAIN_STATION1);
    }

    if (mbCtx.Option("Train station #2"))
    {
        playerScripts.WarpToLocation(TeleportLocation::TRAIN_STATION2);
    }

    if (mbCtx.Option("Waypoint"))
    {
        Vector3 waypointCoords = playerScripts.GetWaypointCoords();
        playerScripts.SetPlayerCoords(waypointCoords);
    }
#endif //!NEW_TELEPORTS
}


// teleportlocations

#ifdef NEW_TELEPORTS

// TODO Migrate to using this format for teleports. 
// First I'll need to fix it in TeleportLocations.cpp, TeleportLocations.h, PlayerScripts.cpp, and PlayerScripts.h:
void TeleportMenu::BuildAirportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
        mbCtx.Title("Airports");
        if (mbCtx.Option("Airport Runway"))
        {
            playerScripts.WarpToLocation(TeleportLocationCategory::AIRPORTS, TeleportLocationID::AIRPORT_RUNWAY);
        }
        if (mbCtx.Option("Los Santos Intl Terminal"))
        {
            playerScripts.WarpToLocation(TeleportLocationCategory::AIRPORTS, TeleportLocationID::LOS_SANTOS_INTL_TERMINAL);
        }
}

void TeleportMenu::BuildSafehousesSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    mbCtx.Title("Safehouses");
    if (mbCtx.Option("Michael's House"))
    {
        playerScripts.WarpToLocation(TeleportLocationCategory::SAFEHOUSES, TeleportLocationID::MICHAELS_HOUSE); // 0
    }
    if (mbCtx.Option("Franklin's House (New)"))
    {
        playerScripts.WarpToLocation(TeleportLocationCategory::SAFEHOUSES, TeleportLocationID::FRANKLINS_HOUSE_NEW); // 1
    }
    if (mbCtx.Option("Franklin's House (Old)"))
    {
        playerScripts.WarpToLocation(TeleportLocationCategory::SAFEHOUSES, TeleportLocationID::FRANKLINS_HOUSE_OLD); // Value 2
    }

    if (mbCtx.Option("Trevor's House #1"))
    {
        playerScripts.WarpToLocation(TeleportLocationCategory::SAFEHOUSES, TeleportLocationID::TREVORS_HOUSE); // Value 3
    }

    if (mbCtx.Option("Trevor's House #2"))
    {
        playerScripts.WarpToLocation(TeleportLocationCategory::SAFEHOUSES, TeleportLocationID::TREVORS_HOUSE2); // Value 4
    }

    if (mbCtx.Option("Trevor's Office"))
    {
        playerScripts.WarpToLocation(TeleportLocationCategory::SAFEHOUSES, TeleportLocationID::TREVORS_OFFICE); // Value 5
    }
}

#endif //NEW_TELEPORTS

void TeleportMenu::BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& textScripts = TextScripts::GetInstance();
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

#ifdef LUA_TEST
void TeleportMenu::BuildLuaTeleportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
#ifdef LUA_TEST
    auto& luaManager = LuaManager::GetInstance();
#endif //LUA_TEST

        mbCtx.Title("Teleports (Lua)");

        if (mbCtx.Option("Teleport to Airport (Lua)"))
        {
            sol::function teleport_func = luaManager.get_state()["teleport_player"];
            if (teleport_func.valid()) {
                Vector3 targetPosition(-1336.0f, -3044.0f, 13.9f);
                sol::protected_function_result result = teleport_func(targetPosition);
                if (!result.valid()) {
                    sol::error err = result;
                    LOG(ERROR, std::format("Lua Error: {}", err.what()));
                }
            }
            else {
                LOG(ERROR, "Lua function 'teleport_player' not found.");
            }
        }

        //if (mbCtx.Option("Teleport to House (Lua - Table)"))
        //{
        //    sol::function teleport_func = luaManager.get_state()["teleport_to_location"];
        //    if (teleport_func.valid()) {
        //        sol::protected_function_result result = teleport_func("house");
        //        if (!result.valid()) {
        //            sol::error err = result;
        //            LOG(ERROR, std::format("Lua Error (teleport_to_location): {}", err.what()));
        //        }
        //    }
        //    else {
        //        LOG(ERROR, "Lua function 'teleport_to_location' not found.");
        //    }
        //}
}
#endif
