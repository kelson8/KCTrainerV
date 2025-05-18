#include "pch.h"
#include "PlayerMenu.h"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/PedScripts.h"
#include "../Scripts/Tasks.h"

#ifdef MOVE_PLAYER_MENU

void PlayerMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Player Menu");

    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& tasks = Tasks::GetInstance();

    Ped playerPed = playerScripts.GetPlayerPed();

    Hash militaryRifleHash = "weapon_militaryrifle"_hash;
    Hash militaryRifleExtendedClip = "COMPONENT_MILITARYRIFLE_CLIP_02"_hash;

    // This seems to work fine for an invincibility toggle in here like this.
    mbCtx.BoolOption("Invincibility", playerScripts.invincibilityEnabled, { " Turn on/off invincibility" });

    mbCtx.IntOption("Wanted level", playerScripts.wantedLevel, 0, 5, 1, { "Wanted level to set" });
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


    mbCtx.StringArray("--Extras--", { "" }, nothing);
    mbCtx.BoolOption("Toggle Mobile radio", playerScripts.isMobileRadioEnabled, { "Turn on/off radio outside vehicles" });

    // Tests
    // TODO Move these into WeaponMenu.cpp, need to create it first.
    // This gives me a weapon
    // TODO Make this get value from lua, should easily be possible if I run my lua tests.
    if (mbCtx.Option("Give weapon"))
    {
        //pedScripts.GivePedWeapon(playerPed, "weapon_flashlight"_hash, -1, false, false);
        pedScripts.GivePedWeapon(playerPed, militaryRifleHash, -1, false, false);
    }

    // This works too, I need to make a list of these later but for now I am just testing them.
    if (mbCtx.Option("Give attachment"))
    {
        pedScripts.GiveWeaponComponent(playerPed, militaryRifleHash, militaryRifleExtendedClip);
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
#endif