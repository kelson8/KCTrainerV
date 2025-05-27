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

#include <iostream>

// Menyoo
#include "Natives/natives2.h"
// New for game functions from Menyoo
#include "../Scripts/Extras/Game.h"

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
#include "WeaponMenu.h"
#include "Submenus/Vehicle/VehicleOptionsMenu.h"

// Misc
#include "MiscMenu.h"
#include "Submenus/Misc/GlobalsMenu.h"
#include "Submenus/Misc/MiscBlipsMenu.h"
#include "Submenus/Misc/MiscDebugMenu.h"
#include "Submenus/Misc/MiscIdGunMenu.h"
#include "Submenus/Misc/MiscMusicMenu.h"

#include "Submenus/Player/ModelChangerMenu.h"
#include "Submenus/Player/PlayerOptionsMenu.h"


// TODO Setup
//#include "Submenus/Misc/ChaosMenu.h"

// Teleports
#include "Teleports/TeleportLocations.h"

// Chaos mod
#include "Util/EntityIterator.h"
#include "Util/Hash.h"
#include "Util/Random.h"

// GTA V Reclass classes
// TODO Test this
//#include "GTAV-Classes/vehicle/CVehicle.hpp"

/*
 * This function builds the menu's submenus, and the submenus are passed into the CScriptMenu constructor.
 * While this provides a cleaner way to define the menu, dynamically created submenu are not possible.
 * CScriptMenu would need to be changed to allow adding and removing submenus on-the-fly.
 */
std::vector<CScriptMenu<KCMainScript>::CSubmenu> KCMenu::BuildMenu() 
{
    //-----
    // Instances of these objects, this is a great way to use booleans and stuff between files.
    // Scripts
    //-----
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();

    auto& pedScripts = PedScripts::GetInstance();

    auto& textScripts = TextScripts::GetInstance();

    //-----
    // Functions
    auto& fileFunctions = FileFunctions::GetInstance();
    //-----

    //-----
    // Menus
    //-----

    // Player
    auto& playerMenu = PlayerMenu::GetInstance();
    auto& playerOptionsMenu = PlayerOptionsMenu::GetInstance();

#ifdef PLAYER_SKIN_CHANGER_NEW
    auto& modelChangerMenu = ModelChangerMenu::GetInstance();
#endif // PLAYER_SKIN_CHANGER_NEW


    auto& pedMenu = PedMenu::GetInstance();
    auto& teleportMenu = TeleportMenu::GetInstance();
    
    auto& vehicleMenu = VehicleMenu::GetInstance();
    auto& vehicleOptionsMenu = VehicleOptionsMenu::GetInstance();
    
    auto& worldMenu = WorldMenu::GetInstance();

    // Misc menu
    auto& miscMenu = MiscMenu::GetInstance();
    auto& miscGlobalsMenu = Misc::GlobalsMenu::GetInstance();
    auto& miscDebugMenu = Misc::DebugMenu::GetInstance();
    auto& miscIdGunMenu = Misc::IdGunMenu::GetInstance();
    auto& miscBlipsMenu = Misc::BlipsMenu::GetInstance();

    auto& miscMusicMenu = Misc::MusicMenu::GetInstance();

    //

    auto& weaponMenu = WeaponMenu::GetInstance();

    std::vector<CScriptMenu<KCMainScript>::CSubmenu> submenus;
    //-----
    // Main Menu
    // This is where all the menus are added
    //-----
    submenus.emplace_back("mainmenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            // Title and Subtitle are required on each submenu.
            mbCtx.Title(Constants::ScriptName);
            mbCtx.Subtitle(std::string("~b~") + Constants::DisplayVersion);

            // Draw the buttons on the screen for navigation.
            mbCtx.drawInstructionalButtons();

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
            mbCtx.MenuOption("Weapon", "WeaponMenu", { "Show the weapon menu." });
            mbCtx.MenuOption("Vehicle", "vehiclemenu", { "Show the vehicle menu." });
            mbCtx.MenuOption("Teleport", "teleportmenu", { "Show the teleport menu." });

//#ifdef LUA_TEST
//            mbCtx.MenuOption("Teleports (Lua)", "luateleportmenu", { "Show the lua teleports" });
//#endif //LUA_TEST

            mbCtx.MenuOption("Ped", "pedmenu", { "Show the ped menu." });
            mbCtx.MenuOption("World", "worldmenu", { "This submenu contains items for the world menu." });

            mbCtx.MenuOption("Misc", "miscMenu", { "Misc menu." });

            // Showing a non-scrolling aligned item is also possible, if the vector only contains one element.
            int nothing = 0;
            mbCtx.StringArray("Version", { Constants::DisplayVersion }, nothing,
                { "Thanks for checking out this menu!", 
                "Author: kelson8", 
                "Menu base by: ikt", 
                "Some features from Menyoo and Chaos Mod."});
        });

    // This fixes the issue with using some of these variables without static:
    // I added the & symbol where this is:  [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
    // https://stackoverflow.com/questions/26903602/an-enclosing-function-local-variable-cannot-be-referenced-in-a-lambda-body-unles

#pragma region PlayerMenu

    //-----
    // Player Menu
    //-----
    submenus.emplace_back("playermenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            // Run the menu from PlayerMenu.cpp, seems to work fine.
            playerMenu.Build(mbCtx, context);
        }
    );

    //-----
    // Player options sub menu
    //-----
    
    submenus.emplace_back("PlayerOptionsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            //playerMenu.BuildOptionsMenu(mbCtx, context);
            playerOptionsMenu.Build(mbCtx, context);
        }
    );
    //

    //-----
    // Player Model changer sub Menu
    // TODO Fix this, it shows a list of the categories but opening it does nothing.
    // Also, I will need to figure out a way to select each model.
    // Switched to using a basic test for now.
    // This now has a clothes selector for the female multiplayer ped.
    //-----
#ifdef PLAYER_SKIN_CHANGER_NEW
    submenus.emplace_back("PlayerModelChangerSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            modelChangerMenu.Build(mbCtx, context);
        }
    );

    submenus.emplace_back("PlayerModelClothesSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            modelChangerMenu.BuildClothesChangerSubmenu(mbCtx, context);
        }
    );

#endif // PLAYER_SKIN_CHANGER_NEW

    //-----
    // Player debug sub menu
    //-----
    submenus.emplace_back("PlayerDebugSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            playerMenu.BuildDebugSubMenu(mbCtx, context);
        }
    );



#pragma endregion

#pragma region WeaponMenu
    //-----
    // Weapon menu
    //-----
    submenus.emplace_back("WeaponMenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.Build(mbCtx, context);
        }
    );



    //-----
    // Weapon category menu
    //-----

#pragma region WeaponCategories
    submenus.emplace_back("WeaponCategoriesSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponMenu(mbCtx, context);
        }
    );



#pragma region MeleeWeapons
    submenus.emplace_back("MeleeWeaponsSubmenu", // Unique ID for Melee Weapons
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "melee");
        }
    );
#pragma endregion

    submenus.emplace_back("HandgunWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "handguns"); // Pass category key
        }
    );

    submenus.emplace_back("SmgWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "smg"); // Pass category key
        }
    );

    submenus.emplace_back("ShotgunWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "shotguns"); // Pass category key
        }
    );

    submenus.emplace_back("AssaultRifleWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "assault_rifles"); // Pass category key
        }
    );

    //

    submenus.emplace_back("MachineGunsWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "machine_guns"); // Pass category key
        }
    );

    submenus.emplace_back("SniperRiflesWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "sniper_rifles"); // Pass category key
        }
    );

    submenus.emplace_back("HeavyWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "heavy_weapons"); // Pass category key
        }
    );

    submenus.emplace_back("ThrowableWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "throwables"); // Pass category key
        }
    );

    submenus.emplace_back("MiscWeaponsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            weaponMenu.BuildWeaponCategorySubmenu(mbCtx, context, "misc"); // Pass category key
        }
    );

#pragma endregion // WeaponCategories


    // 
#pragma endregion

#pragma region VehicleMenu
    //-----
    // Vehicle menu
    //-----
    submenus.emplace_back("vehiclemenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.Build(mbCtx, context);
        }
    );

    //-----
    // Vehicle options sub menu
    //-----
    submenus.emplace_back("VehicleOptionsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleOptionsMenu.Build(mbCtx, context);
        }
    );

#pragma region VehicleCategorySubMenu

    //-----
    // Vehicle category sub menu
    //-----
    submenus.emplace_back("VehicleCategorySubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildVehicleCategorySubmenu(mbCtx, context);
        }
    );

    //-----
    // Sports vehicle category sub menu
    //-----
    submenus.emplace_back("SportsVehicleCategorySubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildSportsVehicleCategorySubmenu(mbCtx, context);
        }
    );

    //-----
    // Sports classics vehicle category sub menu
    //-----
    submenus.emplace_back("SportsClassicsVehicleCategorySubmenu",

        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildSportsClassicsVehicleCategorySubmenu(mbCtx, context);
        }
    );

    //

    //-----
    // Super vehicle category sub menu
    //-----
    submenus.emplace_back("SuperVehicleCategorySubmenu",

        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildSuperVehicleCategorySubmenu(mbCtx, context);
        }
    );

    //-----
    // SUV vehicle category sub menu
    //-----
    submenus.emplace_back("SuvVehicleCategorySubmenu",

        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            vehicleMenu.BuildSuvCategorySubmenu(mbCtx, context);
        }
    );

#pragma endregion // VehicleCategorySubMenu
#pragma endregion // VehicleMenu

#pragma region TeleportMenu
    //-----
    // Teleport menu
    //-----
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

    //-----
    // Teleport locations sub menu
    //-----
    submenus.emplace_back("teleportlocations",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildTeleportLocationsSubMenu(mbCtx, context);
        }
    );

    // New AirportsTeleportSubmenu format
    submenus.emplace_back("AirportsTeleportSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildAirportSubMenu(mbCtx, context);
        }
    );

    // New SafehousesTeleportSubmenu format
    submenus.emplace_back("SafehousesTeleportSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildSafehousesSubMenu(mbCtx, context);
        }
    );

    // Apartment interiors
    submenus.emplace_back("ApartmentInteriorsTeleportSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildApartmentInteriorsSubMenu(mbCtx, context);
        }
    );

#ifdef NEW_LOAD_IPLS
    // Online teleports sub menu
    submenus.emplace_back("OnlineTeleportSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildOnlineSubmenu(mbCtx, context);
        }
    );
#endif // NEW_LOAD_IPLS

    // Other teleports sub menu
    submenus.emplace_back("OtherTeleportSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildOtherSubmenu(mbCtx, context);
        }
    );

#pragma endregion // TeleportLocationsSubMenu

#pragma region DebugTeleportSubMenu
#ifdef DEBUG_MODE
    //-----
    // Debug teleport functions sub menu
    //-----
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
    //-----
    // Lua Test for teleport menu
    //-----
    submenus.emplace_back("luateleportmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            teleportMenu.BuildLuaTeleportSubMenu(mbCtx, context);
        }
    );
#endif //LUA_TEST

#pragma endregion




#pragma region PedMenu
    //-----
    // Ped Menu
    //-----
    submenus.emplace_back("pedmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            pedMenu.Build(mbCtx, context);
        }
    );
#pragma endregion


#pragma region WorldMenu
    //-----
    // World Menu
    //-----
    submenus.emplace_back("worldmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            worldMenu.Build(mbCtx, context);
        }
    );

#pragma endregion


    
#pragma region MiscMenu
    // Moved down here to reflect position in menu.
    // Moved this into MiscMenu.
    // ---
    // TODO Move chaos mode features into their own sub menu, possibly name it Chaos Mod or Chaos Features?
    // TODO Also add credits to the Chaos mod features menu, such as an about button or something at the bottom.

    submenus.emplace_back("miscMenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            miscMenu.Build(mbCtx, context);
        });


    //-----
    // Misc Music sub menu
    //-----
    submenus.emplace_back("MiscMusicSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            miscMusicMenu.Build(mbCtx, context);
        });

    //-----
    // Misc Chaos sub menu
    // TODO Create this sub menu, make a Misc folder in Menus/Submenus, create ChaosMenu.cpp and ChaosMenu.h in there.
    // Make them use the IMenuBuilder interface and be classes.
    //-----
    //submenus.emplace_back("MiscChaosSubmenu",
    //    [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
    //    {
    //        chaosMenu.Build(mbCtx, context);

    //    });

    //-----
    // Misc Blips sub menu
    //-----
    submenus.emplace_back("MiscBlipsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            miscBlipsMenu.Build(mbCtx, context);

        });

    //-----
    // Misc Sub menu test
    //-----
    submenus.emplace_back("MiscDebugSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            miscDebugMenu.Build(mbCtx, context);
        });


    //-----
    // Misc IDGun Debug sub menu
    //-----
    submenus.emplace_back("MiscIDGunDebugSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            miscIdGunMenu.Build(mbCtx, context);

        });


    //-----
    // Misc Globals sub menu
    //-----
    submenus.emplace_back("MiscGlobalsSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            miscGlobalsMenu.Build(mbCtx, context);

        });

    //

#pragma endregion

#pragma region WorldMenu

    //-----
    // World weather sub menu
    //-----
    submenus.emplace_back("WorldWeatherSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            worldMenu.BuildWeatherMenu(mbCtx, context);

        });

    //-----
    // World time sub menu
    //-----
    submenus.emplace_back("WorldTimeSubmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            worldMenu.BuildTimeMenu(mbCtx, context);

        });

#pragma endregion


    return submenus;
}
