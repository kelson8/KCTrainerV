#include "pch.h"
#include "MiscMusicScripts.h"

#include "Constants.hpp"

#include "Util/UI.hpp"

#include "Util/Random.h"

namespace MiscScripts 
{
	namespace Music
	{
		/// <summary>
		/// Play a sound on the player
		/// TODO Figure out if this works.
		/// </summary>
		/// <param name="audioName"></param>
		/// <param name="audioRef"></param>
		//void PlaySoundOnPlayer(std::string audioName, std::string audioRef)
		void PlaySoundOnPlayer(const char* audioName, const char* audioRef)
		{
			int myPlayer = PLAYER::PLAYER_ID();

			Ped player = PLAYER::GET_PLAYER_PED(myPlayer);

			Player playerPedId = PLAYER_PED_ID();

			//PLAY_SOUND_FROM_ENTITY(-1, audioName.c_str(), player, audioRef.c_str(), false, 0);
			PLAY_SOUND_FROM_ENTITY(-1, audioName, playerPedId, audioRef, false, 0);
		}

		/// <summary>
		/// This works for playing music with the TRIGGER_MUSIC_EVENT native
		/// Here is a list of these: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/musicEventNames.json
		/// These can be found in the decompiled scripts by searching for 'TRIGGER_MUSIC_EVENT'
		/// </summary>
		/// <param name="track"></param>
		//void MiscScripts::PlayTestMusic(int track)
		void PlayTestMusic(MusicTracks track)
		{

			// This method should work
			//switch (track) {
			//case 1:
			//	TRIGGER_MUSIC_EVENT("GTA_ONLINE_STOP_SCORE");
			//	break;
			//case 2:
			//	TRIGGER_MUSIC_EVENT("HEIST_CELEB_STRIP_CLUB");
			//	break;
			//}

			// Better to use std::map for a lot of these:
			// Keep this in sync with the MusicTracks enum in Enums.h
			// If anything is added to this, also add to the enum.
			static const std::map<int, std::string> musicTracks =
			{
				{1, "GTA_ONLINE_STOP_SCORE"},
				{2, "HEIST_CELEB_STRIP_CLUB"},
				{3, "HEIST_CELEB_APARTMENT"},
				{4, "HEIST_STATS_SCREEN_STOP_PREP"},
				{5, "SHOP_TILL_YOU_DROP"},
				{6, "MP_LOBBY_AMBIENCE"},
				{7, "MICHAELS_HOUSE"},
				{8, "AC_START"}, //Altruist Cult
				{9, "AC_END"}, //Altruist Cult
				{10, "AC_DELIVERED"}, //Altruist Cult
				{11, "AC_DONE"}, //Altruist Cult
				{12, "RE51A_SHOP"}, //Shop robbery
				{13, "PEYOTE_TRIPS_START"}, //Something to do with Peyote plants
				{14, "PEYOTE_TRIPS_STOP"}, //Something to do with Peyote plants

				// Flight school, there are probably a few I missed, the fail ones probably don't do anything.
				{15, "FS_FORMATION_START"},
				{16, "FS_FORMATION_FAIL"},
				{17, "FS_LOOP_START"},
				{18, "FS_LOOP_FAIL"},
				{19, "FS_MOVING_LANDING_START"},
				{20, "FS_MOVING_LANDING_FAIL"},
				{21, "CHASE_PARACHUTE_START"},
				{22, "CHASE_PARACHUTE_DEPLOY"}, // Doesn't play anything
				{23, "SHOOTING_RANGE_START"},
				{24, "SHOOTING_RANGE_TIMED"},

				{25, "GROUND_LEVEL_START"}, // Seems to be the sound that plays when you get cops

				{26, "FS_OBSTACLE_START"}, // This one is another one that occurs when flying
				// TODO Test
				{27, "MP_LEADERBOARD_SCENE"},

				// TODO Test this
				//{27, "CHARACTER_CHANGE_UP_MASTER"},
				//{28, "CHARACTER_CHANGE_SKY_MASTER"},
				//{29, "SHORT_PLAYER_SWITCH_SOUND_SET"},
				//{30, "CHARACTER_CHANGE_DOWN_MASTER"},

				// Doesn't do anything
				/*
				* 	//{25, "CITY_LANDING_ENGINE"},
					//{26, "CITY_LANDING_FAIL"},
					//{28, "GROUND_LEVEL_FAIL"},
					//{30, "FS_OBSTACLE_FAIL"},
				*/

				// End flight school

			};

			auto it = musicTracks.find(track);
			if (it != musicTracks.end()) {
				TRIGGER_MUSIC_EVENT(it->second.c_str());
			}
			else {
				// Handle invalid track number
				UI::Notify("Track number invalid.");
			}

		}

		/// <summary>
		/// Stop current music playing.
		/// </summary>
		void StopTestMusic()
		{
			AUDIO::TRIGGER_MUSIC_EVENT("MP_MC_CMH_IAA_FINALE_START");
		}

		/// <summary>
		/// Play sound effects
		/// TODO Figure out how to make these work, they don't play in here.
		/// </summary>
		/// <param name="track"></param>
		void PlaySoundEffect(SoundEffects track)
		{
			// Better to use std::map for a lot of these:
			// https://github.com/DurtyFree/gta-v-data-dumps/blob/master/soundNames.json
			static const std::map<int, std::string> soundEffects =
			{
				{1, "CHARACTER_CHANGE_UP_MASTER"},
				{2, "CHARACTER_CHANGE_SKY_MASTER"},
				{3, "SHORT_PLAYER_SWITCH_SOUND_SET"},
				{4, "CHARACTER_CHANGE_DOWN_MASTER"},
			};

			// Sound effect ID
			auto soundEffectID = AUDIO::GET_SOUND_ID();

			auto it = soundEffects.find(track);
			if (it != soundEffects.end()) {
				//TRIGGER_MUSIC_EVENT(it->second.c_str());
				PLAY_SOUND(soundEffectID, it->second.c_str(), 0, false, false, false);
				//PLAY_SOUND_FRONTEND(100, it->second.c_str(), 0, false);
				//PLAY_SOUND_FRONTEND(soundEffectID, it->second.c_str(), 0, false);
			}
			else {
				// Handle invalid track number
				UI::Notify("Track number invalid.");
			}
		}

		// Some of these below were taken from the Chaos Mod.

		// Taken from MiscIntenseMusic.cpp in Chaos Mod

		void PlayArenaWarLobbyMusic()
		{
			AUDIO::TRIGGER_MUSIC_EVENT("AW_LOBBY_MUSIC_START");
		}

		void StopMusic()
		{
			AUDIO::TRIGGER_MUSIC_EVENT("MP_MC_CMH_IAA_FINALE_START");
		}

		// This didn't seem to play the end credits music.
		// Taken from MiscRollCredits.cpp in Chaos Mod

		void StartCreditsMusic()
		{
			PLAY_END_CREDITS_MUSIC(true);
			SET_MOBILE_PHONE_RADIO_STATE(true);

			int song = g_Random.GetRandomInt(0, 2);
			if (song == 0)
				SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_SAVE_MICHAEL_TREVOR", 1);
			else if (song == 1)
				SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_KILL_MICHAEL", 1);
			else
				SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_KILL_TREVOR", 1);
		}


		void StopCreditsMusic()
		{
			PLAY_END_CREDITS_MUSIC(false);
			SET_MOBILE_PHONE_RADIO_STATE(false);

			SET_USER_RADIO_CONTROL_ENABLED(true);
		}
	} // namespace Music
}