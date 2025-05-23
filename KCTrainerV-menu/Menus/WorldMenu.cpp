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

#pragma region WeatherList

const std::vector<WeatherInfo> WorldMenu::g_weatherList =
{
    // { "Display Name", "NATIVE_WEATHER_STRING" }
    {"Blizzard", "BLIZZARD"},
    {"Clear", "CLEAR"},
    {"Clearing", "CLEARING"},
    {"Clouds", "CLOUDS"},
    {"Extra Sunny", "EXTRASUNNY"},
    {"Foggy", "FOGGY"},
    {"Halloween", "HALLOWEEN"}, // Verify exact string from native docs
    {"Neutral", "NEUTRAL"},
    {"Overcast", "OVERCAST"},
    {"Rain", "RAIN"},
    {"Rain Halloween", "RAIN_HALLOWEEN"}, // Verify exact string
    {"Smog", "SMOG"},
    {"Snow", "SNOW"},
    {"Snow Light", "SNOWLIGHT"},
    {"Snow Halloween", "SNOW_HALLOWEEN"}, // Verify exact string
    {"Thunder", "THUNDER"},
    // Disabled, this weather type does nothing
    //{"Unknown", "UNKNOWN"},
};

#pragma endregion

void WorldMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& worldScripts = WorldScripts::GetInstance();

    // Respawn locations, TODO add these to a different file.
    Vector3 michealsHouseCoords = Vector3(-813.603f, 179.474f, 72.1548f);
    float  michealsHouseHeading = 0.0f;

    mbCtx.Title("World");

    //--------------
    // Weather sub menu
    //--------------
    mbCtx.MenuOption("Weather", "WorldWeatherSubmenu", { "Display a list of weather types to change to" });


    //--------------
    // Time sub menu
    //--------------
    mbCtx.MenuOption("Time", "WorldTimeSubmenu", { "Display a set of hours and minutes to switch to." });

    
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

/// <summary>
/// World Menu - Weather sub menu
/// TODO Test this
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void WorldMenu::BuildWeatherMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Weather");

    for (const auto& weatherInfo : WorldMenu::g_weatherList) 
    {
        if (mbCtx.Option(weatherInfo.name)) 
        {
            SET_WEATHER_TYPE_NOW(weatherInfo.weatherName.c_str());
        }
    }
}

void WorldMenu::BuildTimeMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Time");

    mbCtx.IntOption("Hour", hourToSet, minHourToSet, maxHourToSet, 1, { "Set the hours for the in game clock." });
    mbCtx.IntOption("Minute", minuteToSet, minMinuteToSet, maxMinuteToSet, 1, { "Set the minutes for the in game clock." });
    mbCtx.IntOption("Second", secondToSet, minSecondToSet, maxSecondToSet, 1, { "Set the seconds for the in game clock." });

    if (mbCtx.Option("Set Time", { "Set the time to the above values." }))
    {
        SET_CLOCK_TIME(hourToSet, minuteToSet, secondToSet);
    }
}