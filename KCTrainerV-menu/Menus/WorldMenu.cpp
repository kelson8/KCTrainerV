#include "pch.h"
#include "WorldMenu.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"
#include "Scripts/WorldScripts.h"

#include "Util/FileFunctions.h"

// Chaos mod
#include "Util/EntityIterator.h"
#include "Util/Hash.h"
#include "Util/Random.h"

void WorldMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& worldScripts = WorldScripts::GetInstance();

    mbCtx.Title("World");

    if (mbCtx.Option("Kill all peds", { "Kill all peds in the area" }))
    {
        worldScripts.KillAllPedsInArea();
    }

    // This should make the peds never scream or run away
    mbCtx.BoolOption("Make peds calm", worldScripts.isPedsCalmActive, { "Make peds never scream or run away." });


    if (mbCtx.Option("Blow up vehicles", { "Blow up all vehicles in the area" }))
    {
        worldScripts.BlowUpAllVehiclesInArea();
    }




    // I think this works but it doesn't seem to toggle back off, do the scripts stay dead?
    // I will try to mess with this later.
    //mbCtx.BoolOption("Restricted areas", worldScripts.isRestrictedAreasDisabled, {"Toggle restricted areas on/off."});

    // This spawns fireworks near the player.
    mbCtx.BoolOption("Toggle fireworks", worldScripts.isFireworksStarted, {"Toggle fireworks in the area."});
    
    // These were in MPMenu, adding to my menu I'll probably just remove MPMenu.
    //if (mbCtx.Option("Load MP Maps", { "Enable Multiplayer maps" }))
    //{
    //    ON_ENTER_MP();
    //}

    //if (mbCtx.Option("Unload MP Maps", { "Disable Multiplayer maps" }))
    //{
    //    ON_ENTER_SP();
    //}


}
