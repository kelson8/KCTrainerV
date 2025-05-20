#include "pch.h"

#include "../Constants.hpp"
#include "MiscMenu.h"

#include "../common.h"

void MiscMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
#ifdef MOVE_MENUS
    auto& fileFunctions = FileFunctions::GetInstance();
    auto& textScripts = TextScripts::GetInstance();

    mbCtx.Title("Misc");

    mbCtx.MenuOption("Test Sub Menu", "submenutest", { "Testing menu." });

#endif //MOVE_MENUS

}

#ifdef MOVE_MENUS
// Set for playing the music tracks
int currentMusicTrack;
#endif

// TODO Fix this to work, the teleport sub menus seem to work fine but this one doesn't.
// I am doing it the same way, even building the test menu on the main menu.
void MiscMenu::BuildTestSubMenu(NativeMenu::Menu mbCtx, KCMainScript& context)
{
#ifdef MOVE_MENUS
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& miscScripts = MiscScripts::GetInstance();

    auto& textScripts = TextScripts::GetInstance();

    // Functions
    auto& fileFunctions = FileFunctions::GetInstance();

    mbCtx.Title("Test Menu");

    if (mbCtx.Option("Notify", { "Test notification" }))
    {
        UI::Notify("Test notification");
    }

    mbCtx.BoolOption("ID Gun test", miscScripts.isIdGunEnabled, { "Toggle the ID Gun test from FiveM" });

    int nothing = 0;
    mbCtx.StringArray("--Music--", { "" }, nothing);

    // Incremement the max number for this as I add more into the Enums.h and MiscScripts.cpp
    // To add more to this:
    // 1. Add a value with a number into Enums.h
    // 2. Add a value into the std::map in MiscScript.cpp in the PlayTestMusic function.
    //mbCtx.IntOption("Music track", currentMusicTrack, 1, 26, 1, {"List of music tracks within Enums.h in the code."});
    mbCtx.IntOption("Music track", currentMusicTrack, 1, musicTracksCount, 1, { "List of music tracks within Enums.h in the code." });

    // TODO Test this, figure out a good way to do this.
    if (mbCtx.Option("Play Test sound"))
    {

        //miscScripts.PlayTestMusic(21);
        // Seems to be the music that sometimes happens when flying.
        //miscScripts.PlayTestMusic(CHASE_PARACHUTE_START);
        //miscScripts.PlayTestMusic(SHOOTING_RANGE_START);
        miscScripts.PlayTestMusic(static_cast<MusicTracks>(currentMusicTrack));
    }


    if (mbCtx.Option("AW Lobby music", { "Play the arena war lobby music" }))
    {
        miscScripts.PlayArenaWarLobbyMusic();
    }

    if (mbCtx.Option("Stop music", { "Stops the music currently playing" }))
    {
        miscScripts.StopArenaWarLobbyMusic();
    }

    // This didn't seem to play the end credits music.
    // Taken from MiscRollCredits.cpp in Chaos Mod
    if (mbCtx.Option("Start credits music", { "Start the end credits music" }))
    {
        miscScripts.StartCreditsMusic();
    }

    if (mbCtx.Option("Stop credits music", { "Stop the end credits music" }))
    {
        miscScripts.StopCreditsMusic();
    }

    // Ped tests
    mbCtx.BoolOption("Peds attack player", miscScripts.isPedsAttackEnabled, { "Make all peds in the area attack you" });
    mbCtx.BoolOption("Peds drive crazy", miscScripts.isCrazyPedDrivingEnabled, {
        "Make all peds in the area drive with the rushed driving style." });

#ifdef CHAOSMOD_FEATURES
    if (mbCtx.Option("Set peds in mowers", { "Place all peds in the area into lawn mowers" }))
    {
        miscScripts.SetAllPedsInMowers();
    }
#endif // CHAOSMOD_FEATURES


    mbCtx.BoolOption("Toggle airstrike test", miscScripts.airStrikeRunning, { "Toggle the airstrikes on/off" });

    //if (mbCtx.Option("Start airstrike test", { "Start a test for an airstrike" }))
    //{
    //    //miscScripts.StartAirstrikeTest();
    //    miscScripts.airStrikeRunning = true;
    //}

    //if (mbCtx.Option("Stop airstrike test", { "Stop the test for an airstrike" }))
    //{
    //    miscScripts.StopAirstrikeTest();
    //    miscScripts.airStrikeRunning = false;
    //}

    mbCtx.BoolOption("Draw text on screen", textScripts.drawText, { "Toggle test text to draw on screen." });



#ifdef EXTRA_FEATURES
    // TODO Add this boolean option
    //mbCtx.BoolOption("Toggle sky", &miscScripts.toggleSky);

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

    // Toggle force field
    mbCtx.BoolOption("Toggle forcefield", miscScripts.isForceFieldEnabled, { "Turn on/off the forcefield for the player." });

    // I would set a boolean for this but it requires items that I don't think they can be run in a tick all the time.
    // Toggle tv on/off
    if (mbCtx.Option("Enable TV"))
    {
        miscScripts.EnableTv();
    }

    if (mbCtx.Option("Disable TV"))
    {
        miscScripts.DisableTv();
    }

    //if (mbCtx.Option("Enable Forcefield"))
    //{
    //    miscScripts.EnableSky();
    //}

    //if (mbCtx.Option("Disable Forcefield"))
    //{
    //    miscScripts.EnableSky();
    //}
#endif //EXTRA_FEATURES

    if (mbCtx.Option("Test Fade out/in", { "Test for fading the screen out and back in" }))
    {
        playerScripts.TestFade();
    }

    // TODO Figure out implementation for this, shouldn't be too hard.
    //if (mbCtx.Option("Test reload menu"), { "Test for reloading the menu config, may crash." })
    //{
    //    
    //}

#endif //MOVE_MENUS

}