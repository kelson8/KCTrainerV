#include "pch.h"

#include "../Constants.hpp"
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

// Misc
#include "Scripts/Misc/MiscMusicScripts.h"
#include "Scripts/Misc/MiscExtraFeatures.h"
#include "Scripts/Misc/IDGun.h"

void WorldMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& worldScripts = WorldScripts::GetInstance();

    // Respawn locations, TODO add these to a different file.
    Vector3 michealsHouseCoords = Vector3(-813.603f, 179.474f, 72.1548f);
    float  michealsHouseHeading = 0.0f;
    
    // Time scale in game, set to 1.0 by default.
    static float timeScale = 1.0f;

    // Setting this to 0.0 is a bad idea, this seems to slow my menu down also.
    float minTimeScale = 0.1f;
    float maxTimeScale = 1.0f;
    float timeScaleStep = 0.1f;

    mbCtx.Title("World");

    //--------------
    // Weather sub menu
    //--------------
    mbCtx.MenuOption("Weather", "WorldWeatherSubmenu", { "Display a list of weather types to change to" });


    //--------------
    // Time sub menu
    //--------------
    mbCtx.MenuOption("Time", "WorldTimeSubmenu", { "Set the time." });

    
    //--------------

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
        MiscScripts::EXFeatures::EnableSky();
    }

    if (mbCtx.Option("Disable Sky"))
    {
        MiscScripts::EXFeatures::DisableSky();
    }

    // Toggle snow
    if (mbCtx.Option("Enable Snow"))
    {
        MiscScripts::EXFeatures::EnableSnow();
    }

    if (mbCtx.Option("Disable Snow"))
    {
        MiscScripts::EXFeatures::DisableSnow();
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

    // Enable blackout/emp mode
    // https://nativedb.dotindustries.dev/gta5/natives/0x1268615ACE24D504?search=SET_ARTIFICIAL_LIGHTS_STATE
    mbCtx.BoolOption("Toggle blackout", worldScripts.isBlackoutActive, { "Toggle EMP/blackout mode on/off." });

    // TODO Test these, they should work.
    mbCtx.BoolOption("Disable traffic", worldScripts.isVehiclesDisabled, { "Enable/Disable vehicles driving around and parked." });
    mbCtx.BoolOption("Disable peds", worldScripts.isPedsDisabled, { "Enable/Disable peds." });

    // Set the time scale
    mbCtx.FloatOption("Time scale", timeScale, minTimeScale, maxTimeScale, timeScaleStep);
    if (mbCtx.Option("Set time scale"))
    {
        SET_TIME_SCALE(timeScale);
    }
}
