#pragma once
#include "Constants.hpp"

#include "Util/Enums.h"

namespace MiscScripts
{
	namespace Music
	{
		// Play sound on player
		void PlaySoundOnPlayer(const char* audioName, const char* audioRef);

		// Play test music
		//void PlayTestMusic(int track);
		void PlayTestMusic(MusicTracks track);
		void StopTestMusic();

		// Play test sound effect
		void PlaySoundEffect(SoundEffects track);


		void PlayArenaWarLobbyMusic();
		void StopMusic();

		void StartCreditsMusic();
		void StopCreditsMusic();
	} // namespace Music
} // namespace MiscScripts

