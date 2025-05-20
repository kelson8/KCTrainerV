#include "pch.h"
#include "WorldMenu.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"
#include "Scripts/WorldScripts.h"
#include "Scripts/MiscScripts.h"

#include "Util/FileFunctions.h"

// Chaos mod
#include "Util/EntityIterator.h"
#include "Util/Hash.h"
#include "Util/Random.h"

void WorldMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& worldScripts = WorldScripts::GetInstance();
    auto& miscScripts = MiscScripts::GetInstance();

    // Respawn locations, TODO add these to a different file.
    Vector3 michealsHouseCoords = Vector3(-813.603f, 179.474f, 72.1548f);
    float  michealsHouseHeading = 0.0f;

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

    // TODO Move toggle sky and toggle snow into here from submenu in KCMainMenu.

#ifdef EXTRA_FEATURES

    // TODO Add this boolean option
    //mbCtx.BoolOption("Toggle sky", &miscScripts.toggleSky);

#ifdef MEMORY_TESTING
    // Toggle sky
    if (mbCtx.Option("Enable Sky"))
    {
        miscScripts.EnableSky();
    }

    if (mbCtx.Option("Disable Sky"))
    {
        miscScripts.DisableSky();
    }

    // Toggle snow
    if (mbCtx.Option("Enable Snow"))
    {
        miscScripts.EnableSnow();
    }

    if (mbCtx.Option("Disable Snow"))
    {
        miscScripts.DisableSnow();
    }
#endif // MEMORY_TESTING

#endif // EXTRA_FEATURES

    // TODO Fix these to work, I would like to change the respawns.
    //if (mbCtx.Option("Delete hospital respawns.", { "Disable all hospitals on the map." }))
    //{
    //    worldScripts.DisableHospitalSpawns();
    //}

    //// Well this didn't work.
    //if (mbCtx.Option("Set respawn to micheals house.", {"Set respawn coords with heading."}))
    //{
    //    // Micheals house
    //    worldScripts.SetRespawnLocation(michealsHouseCoords, michealsHouseHeading);
    //}




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
