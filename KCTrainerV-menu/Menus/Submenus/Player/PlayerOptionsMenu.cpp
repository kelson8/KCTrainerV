#include "pch.h"

#include "PlayerOptionsMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

/// <summary>
/// Build the player options menu.
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void PlayerOptionsMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();

    Ped playerPed = playerScripts.GetPlayerPed();

    mbCtx.Title("Player Options");

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


    //-----
    // Kill the player with a pistol just like in online.
    //-----
    if (mbCtx.Option("MP Suicide", { "Kill yourself with the multiplayer animation." }))
    {
        playerScripts.KillPlayerMP();
    }

    //-----
    // Wanted levels
    //-----
    mbCtx.IntOption("Wanted level", playerScripts.wantedLevel, 0, 5, 1, { "Wanted level to set" });
    if (mbCtx.Option("Set Wanted Level", { "Set your wanted level" }))
    {
        playerScripts.SetWantedLevel();
    }

    mbCtx.BoolOption("Never wanted", playerScripts.neverWantedEnabled, { "Toggle never wanted" });

    // Display cops and cop vehicles blown up, moved out of debug menu.
    mbCtx.BoolOption("Display cops killed", Stats::Cop::isCopsKilledDisplayActive, 
        { "Display amount of cops killed and cop vehicles blown up on screen." });

    //-----
    // Visions
    //-----

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

    //-----
    // Bullet options
    //-----
    mbCtx.StringArray("--Bullet Options--", { "" }, nothing);
    mbCtx.BoolOption("Explosive Ammo", playerScripts.isExplosiveBulletsEnabled, {"Toggle explosive bullets"});

    mbCtx.BoolOption("Explosive Melee", playerScripts.isExplosiveMeleeEnabled, {"Toggle explosive melee"});
    mbCtx.BoolOption("Infinite Ammo", playerScripts.isInfiniteAmmoEnabled, {"Toggle infinite ammo"});

    // TODO Fix this to turn back off.
    //mbCtx.BoolOption("Slow time when aiming", playerScripts.isSlowAimEnabled,
    //    { "Toggle slower time when aiming, like Micheals slow down special effect." });

    //-----
    // Other cheat options
    //-----
    mbCtx.StringArray("--Other Cheat Options--", { "" }, nothing);
    mbCtx.BoolOption("Super jump", playerScripts.isSuperJumpEnabled, { "Toggle super jump cheat" });
    //mbCtx.BoolOption("Super run", playerScripts.isSuperRunEnabled, { "Toggle faster running." });
}