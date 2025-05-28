#include "pch.h"

#include "MiscMusicMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

// My scripts

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

#include "Scripts/VehicleScripts.h"
#include "Scripts/TextScripts.h"

#include "Util/FileFunctions.h"

#include "Scripts/Misc/MiscMusicScripts.h"

namespace Misc
{
	/// <summary>
	/// Build the misc blips submenu.
	/// </summary>
	/// <param name="mbCtx"></param>
	/// <param name="context"></param>
	void MusicMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
	{
        mbCtx.Title("Music");

        // Scripts
        auto& playerScripts = PlayerScripts::GetInstance();
        auto& vehicleScripts = VehicleScripts::GetInstance();
        auto& pedScripts = PedScripts::GetInstance();
        auto& textScripts = TextScripts::GetInstance();

        // Functions
        auto& fileFunctions = FileFunctions::GetInstance();


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


        if (mbCtx.Option("Play sound", { "Play the sound from list." }))
        {
            MiscScripts::Music::PlaySoundOnPlayer("Amanda_Pulls_Away", "FAMILY_6_SOUNDS");
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

        // Why doesn't this work?
        if (mbCtx.Option("Play slot machine sounds", { "Play the slot machine sounds on the player" }))
        {
            MiscScripts::Music::PlaySoundOnPlayer("jackpot", "dlc_vw_casino_slot_machine_td_player_sounds");
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
	}
} // namespace Misc