#include "pch.h"

#include "../Constants.hpp"
#include "MiscMenu.h"
#include "../Scripts/MiscScripts.h"

#include "../common.h"

// Menyoo
#include "../Scripts/Extras/Game.h"

/// <summary>
/// Misc Menu - Main Menu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void MiscMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& fileFunctions = FileFunctions::GetInstance();
    auto& textScripts = TextScripts::GetInstance();

    mbCtx.Title("Misc");

    mbCtx.MenuOption("Debug Sub Menu", "MiscDebugSubmenu", { "Debug testing menu." });

}

/// <summary>
/// Misc Menu - Debug Submenu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void MiscMenu::BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Debug Sub Menu");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& textScripts = TextScripts::GetInstance();

    // Functions
    auto& fileFunctions = FileFunctions::GetInstance();

    if (mbCtx.Option("Notify", { "Test notification" }))
    {
        UI::Notify("Test notification");
    }

    if (mbCtx.Option("Notify phone sound", { "Test notification with phone sound" }))
    {
        textScripts.NotificationBottomLeft("Test notification");
    }

    mbCtx.BoolOption("ID Gun test", MiscScripts::IDGun::isIdGunEnabled, { "Toggle the ID Gun test from FiveM" });

#ifdef DEBUG_MODE
    if (mbCtx.Option("Menyoo test", { "Run a test with Menyoo classes" }))
    {
        MiscScripts::EXFeatures::MenyooTest();
    }
#endif

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
        MiscScripts::Music::PlayTestMusic(static_cast<MusicTracks>(currentMusicTrack));
    }

    // This stops all the music currently playing with the TRIGGER_MUSIC_EVENT native.
    // Plays MP_MC_CMH_IAA_FINALE_START music event.
    if (mbCtx.Option("Stop music", { "Stops the music currently playing" }))
    {
        MiscScripts::Music::StopMusic();
    }

    if (mbCtx.Option("AW Lobby music", { "Play the arena war lobby music" }))
    {
        MiscScripts::Music::PlayArenaWarLobbyMusic();
    }

    // This didn't seem to play the end credits music.
    // Taken from MiscRollCredits.cpp in Chaos Mod
    //if (mbCtx.Option("Start credits music", { "Start the end credits music" }))
    //{
    //    MiscScripts::Music::StartCreditsMusic();
    //}

    //if (mbCtx.Option("Stop credits music", { "Stop the end credits music" }))
    //{
    //    MiscScripts::Music::StopCreditsMusic();
    //}

#ifdef CHAOSMOD_FEATURES
    if (mbCtx.Option("Set peds in mowers", { "Place all peds in the area into lawn mowers" }))
    {
        miscScripts.SetAllPedsInMowers();
    }
#endif // CHAOSMOD_FEATURES

    mbCtx.BoolOption("Toggle airstrike test", MiscScripts::EXFeatures::airStrikeRunning, { "Toggle the airstrikes on/off" });

    mbCtx.BoolOption("Draw text on screen", textScripts.drawText, { "Toggle test text to draw on screen." });

#ifdef EXTRA_FEATURES

    // Toggle force field
    mbCtx.BoolOption("Toggle forcefield", MiscScripts::EXFeatures::isForceFieldEnabled, { "Turn on/off the forcefield for the player." });

    // I would set a boolean for this but it requires items that I don't think they can be run in a tick all the time.
    // Toggle tv on/off
    if (mbCtx.Option("Enable TV"))
    {
        MiscScripts::EXFeatures::EnableTv();
    }

    if (mbCtx.Option("Disable TV"))
    {
        MiscScripts::EXFeatures::DisableTv();
    }

#endif //EXTRA_FEATURES

    // TODO Figure out why this function doesn't work.
    if (mbCtx.Option("Test Fade out/in", { "Test for fading the screen out and back in" }))
    {
        playerScripts.TestFade();
    }

#ifdef DEBUG_MODE

    // Test for on screen keyboard
    // So far this shows up but takes a bit to show up, and the text doesn't get logged anywhere.
    // It isn't working like in Menyoo.
    if (mbCtx.Option("On screen keyboard", { "Show a on screen keyboard" }))
    {
        std::string inputString = Game::InputBox("DEFAULT", 64);
        if (inputString.length() > 0)
        {
            std::cout << inputString << std::endl;
        }
        else {
            std::cout << "Input string doesn't exist!" << std::endl;
        }

    }

    // Not sure how to use this yet.
    //if (mbCtx.Option("Reclass CVehicle test", { "Test for displaying the current vehicle name in memory" }))
    //{
    //    Ped playerPed = PLAYER::PLAYER_PED_ID();
    //    Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

    //    if (ENTITY::DOES_ENTITY_EXIST(currentVehicle))
    //    {
    //        //CVehicle* vehiclePtr = reinterpret_cast<CVehicle*>(GET_ENTITY_ADDRESS)

    //    }
    //}
#endif

            // TODO Figure out implementation for this, shouldn't be too hard.
            //if (mbCtx.Option("Test reload menu"), { "Test for reloading the menu config, may crash." })
            //{
            //    
            //}
}