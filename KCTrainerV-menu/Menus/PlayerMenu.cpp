#include "pch.h"

#include "../Constants.hpp"

#include "../common.h"

#include "PlayerMenu.h"

#include "../Util/Hash.h"

#include "../Scripts/PlayerScripts.h"
#include "../Scripts/PedScripts.h"

#include "../Scripts/Stats.h"
#include "../Scripts/MiscScripts.h"

#ifdef MOVE_PLAYER_MENU

void PlayerMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Player Menu");

    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();

    Ped playerPed = playerScripts.GetPlayerPed();

    // Player options sub menu
    mbCtx.MenuOption("Player Options", "PlayerOptionsSubmenu", { "Show a list of options for the player." });
    //

    int nothing = 0;
    mbCtx.StringArray("--Extras--", { "" }, nothing);
    mbCtx.BoolOption("Toggle Mobile radio", playerScripts.isMobileRadioEnabled, { "Turn on/off radio outside vehicles" });

    // Moved this out of the player menu, I can use this as a future reference.
    // This works as a submenu nested within a sub menu, can be useful for later.
    // TODO Use this and create a teleport list.
    //mbCtx.MenuOption("Test Sub Menu", "submenutest", { "Show a test submenu within this menu." });

    //if (mbCtx.Option("Toggle bomb bay", { "This will open/close the bomb bay doors in a plane." })) 
    // {
    //    //KCMainScript::ToggleBombBayDoors();
    //    VehicleScripts::ToggleBombBayDoors();
    //}

    //------
    // Player Model changer, located in Submenus/Player/ModelChanger.cpp
#ifdef PLAYER_SKIN_CHANGER_NEW
    mbCtx.MenuOption("Model changer", "PlayerModelChangerSubmenu", { "Change your skin model from a list" });
#endif // PLAYER_SKIN_CHANGER_NEW
    //------

#ifdef DEBUG_MODE
    mbCtx.MenuOption("Player Debug", "PlayerDebugSubmenu", { "Debug menu for player functions." });
#endif

}

/// <summary>
/// Build the player options menu
/// TODO Move into Submenus/Player/PlayerOptionsMenu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
//void PlayerMenu::BuildOptionsMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
//{
//    auto& playerScripts = PlayerScripts::GetInstance();
//    auto& pedScripts = PedScripts::GetInstance();
//
//    Ped playerPed = playerScripts.GetPlayerPed();
//
//    mbCtx.Title("Player Options");
//
//    // This seems to work fine for an invincibility toggle in here like this.
//    mbCtx.BoolOption("Invincibility", playerScripts.invincibilityEnabled, { " Turn on/off invincibility" });
//
//    // This sets the health but not the armor
//    if (mbCtx.Option("Heal yourself", { "Set your health and armor to max" }))
//    {
//        playerScripts.HealPlayer(playerScripts.GetPlayerPed());
//    }
//
//    if (mbCtx.Option("Suicide", { "Kill the player, explosions will also be added." }))
//    {
//        //playerScripts.HealPlayer(playerScripts.GetPlayerPed());
//        playerScripts.SetPlayerHealth(playerScripts.GetPlayerPed(), 0);
//        playerScripts.SetPlayerArmor(playerScripts.GetPlayerPed(), 0);
//    }
//
//
//    //-----
//    // Kill the player with a pistol just like in online.
//    //-----
//    if (mbCtx.Option("MP Suicide", { "Kill yourself with the multiplayer animation." }))
//    {
//        playerScripts.KillPlayerMP();
//    }
//
//
//    mbCtx.IntOption("Wanted level", playerScripts.wantedLevel, 0, 5, 1, { "Wanted level to set" });
//    if (mbCtx.Option("Set Wanted Level", { "Set your wanted level" }))
//    {
//        playerScripts.SetWantedLevel();
//    }
//
//    //mbCtx.BoolOption("BoolOption", checkBoxStatus, { std::string("Boolean is ") + (checkBoxStatus ? "checked" : "not checked") + "." });
//    //mbCtx.BoolOption("Heat vision");
//
//    mbCtx.BoolOption("Never wanted", playerScripts.neverWantedEnabled, { "Test for toggling never wanted" });
//
//    // Display cops and cop vehicles blown up, moved out of debug menu.
//    mbCtx.BoolOption("Display cops killed", Stats::Cop::isCopsKilledDisplayActive, { "Display amount of cops killed and cop vehicles blown up on screen." });
//
//    int nothing = 0;
//    mbCtx.StringArray("--Visions--", { "" }, nothing);
//
//    if (mbCtx.Option("Toggle Heat vision", { "Turn on/off heat vision" }))
//    {
//        playerScripts.ToggleHeatVision();
//    }
//
//
//    if (mbCtx.Option("Toggle Night vision", { "Turn on/off night vision" }))
//    {
//        playerScripts.ToggleNightVision();
//    }
//
//}

void PlayerMenu::BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
#ifdef DEBUG_MODE
    mbCtx.Title("Player Debug");

    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();

    Ped playerPed = playerScripts.GetPlayerPed();

    //int copsKilledStat = playerScripts.GetCopsKilledStat(PlayerModels::FRANKLIN);

    int copsKilledStat = Stats::Cop::GetCopsKilledStat();
    int copVehiclesBlownUpStat = Stats::Cop::GetCopsVehiclesBlownUpStat();

    PlayerModels currentPlayerModel = playerScripts.GetCurrentPlayerModel();

    // Strings for menu
    std::string michealPlayerModelString = "Current player model: Micheal";
    std::string franklinPlayerModelString = "Current player model: Franklin";
    std::string trevorPlayerModelString = "Current player model: Trevor";

    std::string copsKilledString = std::format("Cops killed: {}", copsKilledStat);
    std::string copsCarsBlownUpString = std::format("Cop cars blown up: {}", copVehiclesBlownUpStat);

    int nothing = 0;
    mbCtx.StringArray("--Logging--", { "" }, nothing);
    // TODO Setup player selector for these
    if (mbCtx.Option("Cops killed stat", { "Log the cops killed stat to the console." }))
    {
        // My code defaults to 0 if the value is invalid, possibly change this?
        if (copsKilledStat != 0)
        {
            //std::cout << copsKilledString << std::endl;
            log_output(copsKilledString);
        }
        
    }

    if (mbCtx.Option("Cop vehicles exploded", { "Log the cops blown up stat to the console." }))
    {
        //std::cout << copsCarsBlownUpString << std::endl;
        log_output(copsCarsBlownUpString);
    }

    if (mbCtx.Option("Current player model", {"Log the currently selected player, Micheal, Franklin, or Trevor."}))
    {
        switch (currentPlayerModel)
        {
        case PlayerModels::MICHEAL:
            //std::cout << michealPlayerModelString << std::endl;
            log_output(michealPlayerModelString);
            break;

        case PlayerModels::FRANKLIN:
            //std::cout << franklinPlayerModelString << std::endl;
            log_output(franklinPlayerModelString);
            break;

        case PlayerModels::TREVOR:
            //std::cout << trevorPlayerModelString << std::endl;
            log_output(trevorPlayerModelString);
            break;
        }
    }

    // Moved out of Debug menu, this should be toggleable in release builds.
    //mbCtx.BoolOption("Display cops killed", Stats::Cop::isCopsKilledDisplayActive, { "Log the amount of cops killed before death to the console and the screen." });


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

    if (mbCtx.Option("Reset to defaults", { "Reset the menu position back to defaults." }))
    {
        Stats::Cop::ResetCopsKilledMenuPositions();
    }

#else
    // Do nothing in release.
    return;
#endif //DEBUG_MODE
}


#endif