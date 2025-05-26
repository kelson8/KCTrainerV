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
#include "Teleports/TeleportManager.h"
#include "Teleports/TeleportLocations.h"



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
    auto& teleportLocations = TeleportLocations::GetInstance();

    // Hmm, I guess all sub menus need a title, it broke without one.
    mbCtx.Title("Teleport");


    mbCtx.MenuOption("Locations", "teleportlocations", { "Show a list of teleport locations" });

    // Moved this into here, out of the main menu.
#ifdef LUA_TEST
    mbCtx.MenuOption("Teleports (Lua)", "luateleportmenu", { "Show the lua teleports" });
#endif //LUA_TEST

    // Warp to waypoint, moved outside of locations teleport menu.
    if (mbCtx.Option("Waypoint"))
    {
        GTAped playerPed = PLAYER_PED_ID();
        teleportLocations.WarpToWaypoint(playerPed);
    }


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



    // This format works
    mbCtx.MenuOption("Airports", "AirportsTeleportSubmenu", { "List of airport locations." });
    mbCtx.MenuOption("Safehouses", "SafehousesTeleportSubmenu", { "List of safehouse locations." });
    
    // From Menyoo
    mbCtx.MenuOption("Apartment Interiors", "ApartmentInteriorsTeleportSubmenu", { "List of apartment interior locations." });
    //

#ifdef NEW_LOAD_IPLS
    mbCtx.MenuOption("Online", "OnlineTeleportSubmenu", { "Online teleport locations" });
#endif // NEW_LOAD_IPLS
    mbCtx.MenuOption("Other", "OtherTeleportSubmenu", { "Misc teleport locations." });

}

/// <summary>
/// Build the airports sub menu
/// This method can auto build the menu with a list of locations.
/// So I don't have to manually type them all out.
/// Uses the TeleportInfo struct
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void TeleportMenu::BuildAirportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Airports");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();

    // Get teleports from the std::vector<TeleportInfo>
    for (const auto& teleportInfo : Teleports::Positions::vAirportLocations)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            playerScripts.SetPlayerCoords(teleportInfo.coordinates, teleportInfo.heading, true);
            //playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }
}

/// <summary>
/// Build the safe houses sub menu
/// This method can auto build the menu with a list of locations.
/// So I don't have to manually type them all out.
/// Uses the TeleportInfo struct
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void TeleportMenu::BuildSafehousesSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Safehouses");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();

    // Get teleports from the std::vector<TeleportInfo>
    for (const auto& teleportInfo : Teleports::Positions::vSafeHouseLocations)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            playerScripts.SetPlayerCoords(teleportInfo.coordinates, teleportInfo.heading, true);
            //playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }
}

/// <summary>
/// Build the apartment interiors sub menu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void TeleportMenu::BuildApartmentInteriorsSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Apartment Interiors");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();

    // Get teleports from the std::vector<TeleportInfo>
    for (const auto& teleportInfo : Teleports::Positions::vApartmentInteriors)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            playerScripts.SetPlayerCoords(teleportInfo.coordinates, teleportInfo.heading, true);
            //playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }
}

/// <summary>
/// Build the online teleports sub menu.
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
#ifdef NEW_LOAD_IPLS
void TeleportMenu::BuildOnlineSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Online");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& teleportManager = TeleportManager::GetInstance();

    // Testing with IPLs here
    //Get teleports from the std::vector<TeleportIplInfo>
    for (const auto& teleportInfo : Teleports::Positions::vCasinoLocations)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            // Handle IPLs for the selected location
            teleportManager.HandleTeleportIpls(teleportInfo);

            playerScripts.SetPlayerCoords(teleportInfo.coordinates, teleportInfo.heading, true);
            //playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }

}

#endif // NEW_LOAD_IPLS


/// <summary>
/// Build the other teleports sub menu.
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void TeleportMenu::BuildOtherSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    // Change title for each of these!
    mbCtx.Title("Other");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& teleportManager = TeleportManager::GetInstance();

     //Get teleports from the std::vector<TeleportInfo>
    for (const auto& teleportInfo : Teleports::Positions::vOtherLocations)
    {
        if (mbCtx.Option(teleportInfo.name)) {
            playerScripts.SetPlayerCoords(teleportInfo.coordinates, teleportInfo.heading, true);
            //playerScripts.SetPlayerHeading(teleportInfo.heading);
        }
    }
}

/// <summary>
/// Debug for player menu
/// So far just change the player X, Y, Z, and heading on screen.
/// Seems to set the position for the 'TextScripts::DisplayCoordinates()' function.
/// TODO Move this into another file or somewhere else, I don't think it needs to be in TeleportMenu.
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void TeleportMenu::BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& textScripts = TextScripts::GetInstance();
    auto& fileFunctions = FileFunctions::GetInstance();
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

        // Save player debug menu positions to a file
        if (mbCtx.Option("Save to file", { "Save the list of positions to the MenuPositions.txt file in the mod folder." }))
        {
            if (fileFunctions.SavePlayerMenuTextPositions(Constants::menuPositionsFile))
            {
                UI::Notify("Menu positions saved to file.");
            }
            else {
                UI::Notify("Error, couldn't save menu positions.");
            }
            
        }

        // TODO Add a load menu positions from file option.
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