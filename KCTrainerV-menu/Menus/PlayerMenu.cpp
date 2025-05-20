#include "pch.h"

#include "../Constants.hpp"

#include "../common.h"

#include "PlayerMenu.h"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/PedScripts.h"
#include "../Scripts/Tasks.h"
#include "../Scripts/Stats.h"
#include "../Scripts/MiscScripts.h"

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

    // This sets the health but not the armor
    if (mbCtx.Option("Heal yourself", { "Set your health and armor to max" }))
    {
        playerScripts.HealPlayer(playerScripts.GetPlayerPed());
    }

    if (mbCtx.Option("Suicide", { "Kill the player, explosions will also be added." }))
    {
        //playerScripts.HealPlayer(playerScripts.GetPlayerPed());
        playerScripts.SetPlayerHealth(playerScripts.GetPlayerPed(), 0);
        playerScripts.SetPlayerArmor(playerScripts.GetPlayerPed(), 0);
    }


    // TODO Fix this to work.
    if (mbCtx.Option("MP Suicide", { "Kill yourself with the multiplayer animation." }))
    {
        playerScripts.KillPlayerMP();
    }


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

#ifdef DEBUG_MODE
    if (mbCtx.Option("Menyoo test", { "Run a test with Menyoo classes" }))
    {
        MiscScripts::EXFeatures::MenyooTest();
    }

    mbCtx.MenuOption("Player Debug", "PlayerDebugSubmenu", { "Debug menu for player functions." });
    //if (mbCtx.Option("Log cops killed stat", { "Log the cops killed stat to the console." }))
    //{
    //    std::cout << playerScripts.GetCopsKilledStat() << std::endl;
    //}

    //if (mbCtx.Option("Log cops killed stat", { "Log the cops killed stat to the console." }))
    //{
    //    std::cout << playerScripts.GetCopsKilledStat() << std::endl;
    //}
#endif

}

void PlayerMenu::BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
#ifdef DEBUG_MODE
    mbCtx.Title("Player Debug");

    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& tasks = Tasks::GetInstance();

    Ped playerPed = playerScripts.GetPlayerPed();

    //int copsKilledStat = playerScripts.GetCopsKilledStat(PlayerModels::FRANKLIN);

    int copsKilledStat = Stats::Cop::GetCopsKilledStat();
    int copVehiclesBlownUpStat = Stats::Cop::GetCopsVehiclesBlownUpStat();

    PlayerModels currentPlayerModel = playerScripts.GetCurrentPlayerModel();

    int nothing = 0;
    mbCtx.StringArray("--Logging--", { "" }, nothing);
    // TODO Setup player selector for these
    if (mbCtx.Option("Cops killed stat", { "Log the cops killed stat to the console." }))
    {
        // My code defaults to 0 if the value is invalid, possibly change this?
        if (copsKilledStat != 0)
        {
            std::cout << "Cops killed: " << copsKilledStat << std::endl;
        }
        
    }

    if (mbCtx.Option("Cop vehicles exploded", { "Log the cops blown up stat to the console." }))
    {
        std::cout << "Cops cars blown up: " << copVehiclesBlownUpStat << std::endl;
    }

    if (mbCtx.Option("Current player model", {"Log the currently selected player, Micheal, Franklin, or Trevor."}))
    {
        switch (currentPlayerModel)
        {
        case PlayerModels::MICHEAL:
            std::cout << "Current player model: Micheal" << std::endl;
            break;

        case PlayerModels::FRANKLIN:
            std::cout << "Current player model: Franklin" << std::endl;
            break;

        case PlayerModels::TREVOR:
            std::cout << "Current player model: Trevor" << std::endl;
            break;
        }
    }

    mbCtx.BoolOption("Display cops killed", Stats::Cop::isCopsKilledDisplayActive, { "Log the amount of cops killed before death to the console and the screen." });


    //float stepValue = 0.015f;
    float stepValue = 0.001f;
    mbCtx.StringArray("--Cops killed debug--", { "" }, nothing, { "These below items will only show up in debug builds." });
    
    // Cops killed on menu
    // Player X position on menu
    mbCtx.FloatOption("Cops killed menu X", Stats::Cop::copsKilledMenuPosX, 0.f, 1.0f, stepValue);
    mbCtx.FloatOption("Cops killed menu Y", Stats::Cop::copsKilledMenuPosY, 0.f, 1.0f, stepValue);


    //Cop cars blown up on menu
    mbCtx.FloatOption("Cops cars blown up menu X", Stats::Cop::copsCarsBlownUpMenuPosX, 0.f, 1.0f, stepValue);
    mbCtx.FloatOption("Cops cars blown up menu Y", Stats::Cop::copsCarsBlownUpMenuPosY, 0.f, 1.0f, stepValue);

#else
    // Do nothing in release.
    return;
#endif //DEBUG_MODE
}


#endif