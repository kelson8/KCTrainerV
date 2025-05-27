#include "pch.h"
#include "WeaponMenu.h"

#include "../Constants.hpp"

#include "../common.h"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/PedScripts.h"

#include "../Scripts/Stats.h"
#include "../Scripts/MiscScripts.h"

#include "Util/Util.hpp"

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
