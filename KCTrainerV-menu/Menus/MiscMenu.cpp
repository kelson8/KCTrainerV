#include "pch.h"

#include "../Constants.hpp"
#include "MiscMenu.h"
#include "../Scripts/MiscScripts.h"

#include "../common.h"

#include "../Teleports/TeleportLocations.h"

#include "defines.h"

// Menyoo
#include "../Scripts/Extras/Game.h"
#include "../Scripts/Extras/Classes/GTAblip.h"
#include "../Scripts/Extras/World.h"


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

    mbCtx.MenuOption("Blips", { "MiscBlipsSubmenu" }, { "Testing with blips, adding/removing and more." });

    mbCtx.MenuOption("Debug Sub Menu", "MiscDebugSubmenu", { "Debug testing menu." });

}



/// <summary>
/// Misc Menu - Blips submenu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void MiscMenu::BuildBlipsSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Blips");

    // Scripts
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& textScripts = TextScripts::GetInstance();

    // Functions
    auto& fileFunctions = FileFunctions::GetInstance();

    // https://forum.cfx.re/t/help-how-do-i-add-blips-to-my-fivem-map-the-gta-v-map/84873/2

    // Hmm, I didn't know the World namespace had creating blips.
    //GTAblip blip = World::CreateBlip(Teleports::vSafeHouseLocations.at("Michael's House"));
    //GTAblip blip;

    // This seems to work
    // TODO Figure out how to make this a different icon.
    // For some reason it's only the yellow marker with the name 'destination'
    if (mbCtx.Option("Create blip"))
    {
        if (!blip.Exists())
        {
            //blip = blip.Handle();

            // Now in here it prints off the blip handle, but I cannot remove it from the function below.
            // It just says the blip is 0 and doesn't exist.
            blip = World::CreateBlip(Teleports::Positions::vSafeHouseLocations.at("Michael's House"));

            blip.AddBlipForCoord(Teleports::Positions::vSafeHouseLocations.at("Michael's House"));
        
            //blip.Position_set();
            blip.SetBlipDisplay(blip, 3);
            blip.SetScale(1.0f);
            //blip.SetColour(5);

            //blip.SetShortRange(true);
            //blip.SetIcon(BlipIcon::AmmuNation);
            // Premium Deluxe Motorsport icon
            blip.SetIcon(BlipIcon::SportsCar);
            blip.SetBlipName("Death point");

            log_output(std::format("Added blip with handle: {}.", blip.Handle()));
        }
    }

    // TODO Figure out how to remove these blips
    // Well this says it gets removed but doesn't delete the blip.
    if (mbCtx.Option("Remove blip"))
    {
        if (blip.Exists())
        {
            log_output(std::format("Removed blip with handle: {}", blip.Handle()));
            blip.Remove();
        }
        else {
            log_output(std::format("Blip handle {} is invalid or doesn't exist.", blip.Handle()));
        }
    }
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

    //-----
    // Music/Sound effects
    //-----

    int nothing = 0;
    mbCtx.StringArray("--Music--", { "" }, nothing);

    // Incremement the max number for this as I add more into the Enums.h and MiscScripts.cpp
    // To add more to this:
    // 1. Add a value with a number into Enums.h
    // 2. Add a value into the std::map in MiscScript.cpp in the PlayTestMusic function.
    //mbCtx.IntOption("Music track", currentMusicTrack, 1, 26, 1, {"List of music tracks within Enums.h in the code."});
    mbCtx.IntOption("Music track", currentMusicTrack, 1, musicTracksCount, 1, { "List of music tracks within Enums.h in the code." });

    // Play some music from the games list with TRIGGER_MUSIC_EVENT native.
    if (mbCtx.Option("Play Test music"))
    {

        //miscScripts.PlayTestMusic(21);
        // Seems to be the music that sometimes happens when flying.
        //miscScripts.PlayTestMusic(CHASE_PARACHUTE_START);
        //miscScripts.PlayTestMusic(SHOOTING_RANGE_START);
        MiscScripts::Music::PlayTestMusic(static_cast<MusicTracks>(currentMusicTrack));
    }

    //-----
    // Play sound effects
    // This doesn't do anything, disabled for now
    //-----
    //mbCtx.IntOption("Sound effect", currentSoundEffect, 1, soundEffectsCount, 1, { "List of sound tracks within Enums.h in the code." });
    //
    //if (mbCtx.Option("Play test sound"))
    //{
    //    MiscScripts::Music::PlaySoundEffect(static_cast<SoundEffects>(currentSoundEffect));
    //}

    
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
        //playerScripts.TestFade();
        Vector3 targetLocation = Teleports::Positions::vSafeHouseLocations.at("Michael's House");
        MiscScripts::Fade::InitiateTeleportFade(targetLocation);
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