#include "pch.h"
#include "WeaponMenu.h"

#include "../Constants.hpp"

#include "../common.h"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/PedScripts.h"

#include "../Scripts/Stats.h"
#include "../Scripts/MiscScripts.h"

// TODO Move Weapon file loading into WeaponScripts, remove this include later
#include "Script.hpp"

/// <summary>
/// Build the weapon menu.
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void WeaponMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Weapons");

    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();

    Ped playerPed = playerScripts.GetPlayerPed();

    // Set the weaponAmmo for the give weapon function to 1000 for now.
    int weaponAmmo = 1000;

    //------
    // Weapon hashes
    // List of weapons with images: https://wiki.rage.mp/wiki/Weapons
    // Another useful list for these: https://docs.fivem.net/docs/game-references/weapon-models/
    //------
    Hash militaryRifleHash = "weapon_militaryrifle"_hash;

    //-----
    // Weapon component hashes
    // List of weapon components: https://wiki.rage.mp/wiki/Weapons_Components
    //-----
    Hash militaryRifleExtendedClip = "COMPONENT_MILITARYRIFLE_CLIP_02"_hash;

    // Tests
    // This gives me a weapon
    // TODO Make this get value from lua, should easily be possible if I run my lua tests.
    if (mbCtx.Option("Give weapon"))
    {
        //pedScripts.GivePedWeapon(playerPed, "weapon_flashlight"_hash, -1, false, false);
        pedScripts.GivePedWeapon(playerPed, militaryRifleHash, weaponAmmo, false, false);
    }

    // Hmm.. I really wonder if this will work
    // Oops this just crashes it.
    //if (mbCtx.Option("Give all weapons"))
    //{

    //    //for (DWORD i = 0; i < WEAPON_TOTAL; i++)
    //    //{
    //    //    pedScripts.GivePedWeapon(playerPed, i, weaponAmmo, false, false);
    //    //}
    //}

    // This works too, I need to make a list of these later but for now I am just testing them.
    if (mbCtx.Option("Give attachment"))
    {
        pedScripts.GiveWeaponComponent(playerPed, militaryRifleHash, militaryRifleExtendedClip);
    }


    //------
    // Weapon categories menu
    // TODO Make WeaponScripts file, move weapon json data load into there and out of Script.cpp.
    //------
    mbCtx.MenuOption("Categories", "WeaponCategoriesSubmenu", {"Show a list of weapon categories to choose from."});
}

/// <summary>
/// Build the weapon category menu
/// </summary>
void WeaponMenu::BuildWeaponMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Weapons");

    // This works!
    mbCtx.MenuOption("Melee", "MeleeWeaponsSubmenu");
    mbCtx.MenuOption("Handguns", "HandgunWeaponsSubmenu");
    mbCtx.MenuOption("SMG", "SmgWeaponsSubmenu");
    mbCtx.MenuOption("Shotguns", "ShotgunWeaponsSubmenu");

    mbCtx.MenuOption("Assault Rifles", "AssaultRifleWeaponsSubmenu");
    mbCtx.MenuOption("Machine Guns", "MachineGunsWeaponsSubmenu");
    mbCtx.MenuOption("Sniper Rifles", "SniperRiflesWeaponsSubmenu");
    mbCtx.MenuOption("Heavy Weapons", "HeavyWeaponsSubmenu");

    mbCtx.MenuOption("Throwables", "ThrowableWeaponsSubmenu");
    mbCtx.MenuOption("Misc", "MiscWeaponsSubmenu");
}


/// <summary>
/// Build all weapon submenus
/// To use in the KCMainMenu:
/// Under a subcategory: 'playerMenu.BuildWeaponCategorySubmenu(mbCtx, context, "melee"); // Pass category key'
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
/// <param name="categoryKey">The category key from the weapons json file, such as 'heavy_weapons'</param>
void WeaponMenu::BuildWeaponCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context, const std::string& categoryKey)
{
    // Look up the actual weapons data using the categoryKey
    auto it = KCMenu::g_weaponCategories.find(categoryKey);
    if (it == KCMenu::g_weaponCategories.end())
    {
        LOG(ERROR, std::format("BuildWeaponCategorySubmenu: Category '{}' not found in g_weaponCategories!", categoryKey.c_str()));
        mbCtx.Title("Error: " + categoryKey); // Display an error title
        mbCtx.Option("No weapons found.");
        return;
    }

    const std::string& categoryDisplayName = it->first; // Use the actual key as display name for now
    const std::vector<WeaponInfo>& weaponsInThisCategory = it->second;

    // --- ADD LOGS HERE ---
    //LOG(DEBUG, std::format("Submenu Build: Entering {} submenu (key: {})", categoryDisplayName.c_str(), categoryKey.c_str()));
    mbCtx.Title(categoryDisplayName); // Set submenu title
    //LOG(DEBUG, std::format("Submenu Build: Title set to '{}'.", categoryDisplayName.c_str()));

    if (weaponsInThisCategory.empty())
    {
        LOG(WARN, std::format("Submenu Build: WARNING - 'weaponsInThisCategory' is EMPTY for category '{}'!", categoryDisplayName.c_str()));
        mbCtx.Option("No weapons in this category."); // Display a message if empty
    }
    else
    {
        //LOG(DEBUG, std::format("Submenu Build: Category '{}' has {} weapons to display.", categoryDisplayName.c_str(),
            //(int)weaponsInThisCategory.size()));
    }

    for (const auto& weaponInfo : weaponsInThisCategory)
    {
        //LOG(DEBUG, std::format("Submenu Build: Attempting to add option for weapon: '{}' (Hash: 0x{})",
            //weaponInfo.name.c_str(), weaponInfo.hash));
        if (weaponInfo.name.empty())
        {
            //LOG(WARN, std::format("Submenu Build: WARNING - Weapon name is empty for hash 0x{} in category {}.",
                //weaponInfo.hash, categoryDisplayName.c_str()));
        }

        if (mbCtx.Option(weaponInfo.name))
        {
            GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), weaponInfo.hash, 999, false, true);
            //LOG(DEBUG, std::format("Submenu Build: Gave weapon '{}' to player.", weaponInfo.name.c_str()));
            //mbCtx.CloseMenu(); // Optionally close menu
        }
    }
    // --- END ADD LOGS ---
}