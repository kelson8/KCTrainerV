#include "pch.h"
#include "../Constants.hpp"
#include "TeleportMenu.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/TextScripts.h"

#include "../Util/Paths.hpp"
#include "../Util/Logger.hpp"
#include "../Util/UI.hpp"

#include "../Util/FileFunctions.h"

// Menyoo
#include "../Scripts/Extras/Game.h"

// Teleports
#include "Teleports/TeleportLocations.h"
#include "Teleports/TeleportManager.h"


#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

//Vector3 _customTeleLoc = Vector3(0.0, 0.0, 0.0);
Vector3 _customTeleLoc;

// TODO Setup sub menu for the custom teleport later
// Add X, Y, and Z float options in the menu.
bool grabbedCoords = false;

void TeleportMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& fileFunctions = FileFunctions::GetInstance();
    auto& textScripts = TextScripts::GetInstance();
    auto& playerScripts = PlayerScripts::GetInstance();

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

    // TODO Setup sub menu for the custom teleport later
    // TODO Add X, Y, and Z float options in the menu.
    // This seems to work for teleporting the player to a specific X, Y, and Z.
    // This idea is coming from Menyoo, Sub_CustomCoords function in Teleport_Submenus.cpp.
    if (mbCtx.Option("Custom Teleport", { "Teleport to a custom X, Y, and Z using an on screen keyboard" }))
    {
        
        Ped playerPedID = PLAYER_PED_ID();
        //if (!grabbedCoords)
        //{
        //    _customTeleLoc = GET_ENTITY_COORDS(playerPedID, 0);
        //    grabbedCoords = true;
        //}

        try {
            //_customTeleLoc.x = std::stof(Game::InputBox(std::to_string(_customTeleLoc.x), 11U, std::string(), std::to_string(_customTeleLoc.x)));
            _customTeleLoc.x = std::stof(Game::InputBox(std::to_string(_customTeleLoc.x), 11U));
        }
        catch (...)
        {

        }

        try {
            //_customTeleLoc.y = std::stof(Game::InputBox(std::to_string(_customTeleLoc.y), 11U, std::string(), std::to_string(_customTeleLoc.y)));
            _customTeleLoc.y = std::stof(Game::InputBox(std::to_string(_customTeleLoc.y), 11U));
        }
        catch (...)
        {

        }

        try {
            //_customTeleLoc.z = std::stof(Game::InputBox(std::to_string(_customTeleLoc.z), 11U, std::string(), std::to_string(_customTeleLoc.z)));
            _customTeleLoc.z = std::stof(Game::InputBox(std::to_string(_customTeleLoc.z), 11U));
        }
        catch (...)
        {

        }

        playerScripts.SetPlayerCoords(_customTeleLoc);

        std::string valuesOutputString = std::format("Values: X: {}, Y: {}, Z: {}", _customTeleLoc.x, _customTeleLoc.y, _customTeleLoc.z);

        //std::cout << valuesOutputString << std::endl;
        log_output(valuesOutputString);


        // TODO Replicate this from Menyoo
        /*
        			GTAentity thisEntity = Static_241;

			if (!GrabbedCoords)
			{
				_customTeleLoc = GET_ENTITY_COORDS(PLAYER_PED_ID(), 0);
				GrabbedCoords = true;
			}

			bool x_plus = 0, x_minus = 0,
				y_plus = 0, y_minus = 0,
				z_plus = 0, z_minus = 0,
				x_custom = 0, y_custom = 0, z_custom = 0, apply = 0, update = 0;

			AddTitle("Custom Coordinates");
			AddOption("Update to current", update);
			AddNumber("  X", _customTeleLoc.x, 4, x_custom, x_plus, x_minus);
			AddNumber("  Y", _customTeleLoc.y, 4, y_custom, y_plus, y_minus);
			AddNumber("  Z", _customTeleLoc.z, 4, z_custom, z_plus, z_minus);
			AddOption("Apply", apply);


			if (x_plus) { _customTeleLoc.x += 0.1f; return; }
			if (y_plus) { _customTeleLoc.y += 0.1f; return; }
			if (z_plus) { _customTeleLoc.z += 0.1f; return; }
			if (x_minus) { _customTeleLoc.x -= 0.1f; return; }
			if (y_minus) { _customTeleLoc.y -= 0.1f; return; }
			if (z_minus) { _customTeleLoc.z -= 0.1f; return; }

			if (apply)
			{
				GrabbedCoords = false;
				teleport_net_ped(thisEntity, _customTeleLoc.x, _customTeleLoc.y, _customTeleLoc.z);
			}

			if (update)
			{
				GrabbedCoords = false;
			}
        */

    }



    // Debug functions for the teleport menu, disabled in release builds.
#ifdef DEBUG_MODE
    mbCtx.MenuOption("Teleport Debug", "debugteleportmenu", { "Debug menu for teleport functions." });
#endif //DEBUG_MODE

}

void TeleportMenu::BuildTeleportLocationsSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& teleportLocations = TeleportLocations::GetInstance();
    mbCtx.Title("Teleport Locations");

#ifdef NEW_TELEPORTS
    mbCtx.MenuOption("Airport", "AirportsTeleportSubmenu", { "Show a list of airport teleport locations." });
    mbCtx.MenuOption("Safehouses", "SafehousesTeleportSubmenu", { "Show a list of safehouse teleport locations." });

#endif //NEW_TELEPORTS

    // TODO Make these use the sub menus I have defined, should make this file a bit more organized.
    // Just don't enable the NEW_TELEPORTS preprocessor, I may remove it later.
    // I could just move these into the other functions for sub menus.
#ifndef NEW_TELEPORTS

#ifndef NEW_TELEPORT_CATEGORIES
    if (mbCtx.Option("Airport"))
    {
        playerScripts.WarpToLocation(TeleportLocation::AIRPORT_RUNWAY);
    }
    if (mbCtx.Option("Hospital LS1 Rooftop"))
    {
        playerScripts.WarpToLocation(TeleportLocation::HOSPITAL_LS1_ROOFTOP);
    }

    // This seems to autoload? At least with my other mods it does, not sure if it's supposed to.
//#ifdef LOAD_IPLS
    if (mbCtx.Option("Singleplayer Yacht"))
    {
        playerScripts.WarpToLocation(TeleportLocation::SP_YACHT1);
    }
//#endif

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
        GTAped playerPed = PLAYER_PED_ID();
        //Vector3 waypointCoords = playerScripts.GetWaypointCoords();
        //playerScripts.SetPlayerCoords(waypointCoords);
        teleportLocations.WarpToWaypoint(playerPed);
    }

   

#endif // !NEW_TELEPORT_CATEGORIES

    // This format works
#ifdef NEW_TELEPORT_CATEGORIES
    mbCtx.MenuOption("Airports", "AirportsSubmenu", { "List of airport locations." });
    mbCtx.MenuOption("Safehouses", "SafehousesSubmenu", { "List of safehouse locations." });
#endif

#endif //!NEW_TELEPORTS
}

#ifdef NEW_TELEPORT_CATEGORIES
// TODO Rename once I remove the old one.

/// <summary>
/// Build the airports sub menu
/// This method can auto build the menu with a list of locations.
/// So I don't have to manually type them all out.
/// Uses the TeleportInfo struct
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void TeleportMenu::BuildNewAirportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Airports");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    //auto& teleportLocations = TeleportLocations::GetInstance();

    // Get teleports from the std::vector<TeleportInfo>
    for (const auto& teleportInfo : Teleports::Positions::vAirportLocations)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            playerScripts.SetPlayerCoords(teleportInfo.coordinates);
            playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }
}

#endif

#ifdef NEW_TELEPORT_CATEGORIES
// TODO Rename once I remove the old one.

/// <summary>
/// Build the safe houses sub menu
/// This method can auto build the menu with a list of locations.
/// So I don't have to manually type them all out.
/// Uses the TeleportInfo struct
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void TeleportMenu::BuildNewSafehousesSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Safehouses");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    //auto& teleportLocations = TeleportLocations::GetInstance();

    // Get teleports from the std::vector<TeleportInfo>
    for (const auto& teleportInfo : Teleports::Positions::vSafeHouseLocations)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            playerScripts.SetPlayerCoords(teleportInfo.coordinates);
            playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }
}

#endif


/// <summary>
/// Build the test teleport sub menu
/// This method can auto build the menu with a list of locations.
/// So I don't have to manually type them all out.
/// Uses the TeleportInfo struct
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
#ifdef NEW_TELEPORT_CATEGORIES
void TeleportMenu::BuildTestTeleportSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Test Locations");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    //auto& teleportLocations = TeleportLocations::GetInstance();

    // Get teleports from the std::vector<TeleportInfo>
    for (const auto& teleportInfo : Teleports::Positions::vTestLocations)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            playerScripts.SetPlayerCoords(teleportInfo.coordinates);
            playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }
}
#endif

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

#ifdef LOAD_TELEPORT_INI
// TODO Move this, and attempt to fix it.
// It should load the teleport locations from a file.
#pragma region TeleportIniTest
// Define a function to build the entire Teleport Menu
void BuildTeleportCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context, const std::string& categoryName) {
    // Get the specific category data from the manager
    const TeleportCategory* category = TeleportManager::GetInstance().GetCategory(categoryName);

    if (!category || category->locations.empty()) {
        mbCtx.Title(categoryName); // Use category name as submenu title
        mbCtx.Option("No locations in this category.");
        return;
    }

    mbCtx.Title(category->name); // Set the submenu title to the category name

    // Iterate through each teleport location in the current category
    for (const auto& loc : category->locations) {
        // Display the location name as a regular Option (no submenu after this)
        std::vector<std::string> details;
        details.push_back(std::format("X: {:.2f} Y: {:.2f} Z: {:.2f}", loc.coords.x, loc.coords.y, loc.coords.z));
        if (loc.heading != 0.0f) { // Only add heading if it's not default
            details.push_back(std::format("H: {:.2f}", loc.heading));
        }

        if (mbCtx.Option(loc.name, details)) { // Use Option, as this is the final action
            // --- Teleport Logic (using your fade-to-black system) ---
            // Assuming KCMenu::Fade::InitiateTeleportFade(coords, heading) is available
            KCMenu::Fade::InitiateTeleportFade(loc.coords, loc.heading);
            // OR if you want immediate, non-fading teleport for testing:
            // Ped playerPed = PLAYER_PED_ID();
            // SET_ENTITY_COORDS(playerPed, loc.coords.x, loc.coords.y, loc.coords.z, FALSE, FALSE, FALSE, TRUE);
            // if (loc.heading != 0.0f) {
            //     ENTITY::SET_ENTITY_HEADING(playerPed, loc.heading);
            // }
            // UI::Notify(std::format("Teleported to: {}", loc.name).c_str());
        }
    }
}
#pragma endregion

#endif // LOAD_TELEPORT_INI