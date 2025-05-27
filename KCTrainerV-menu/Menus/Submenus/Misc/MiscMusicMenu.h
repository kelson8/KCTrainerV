#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

namespace Misc
{
	class MusicMenu : public IMenuBuilder
	{
	public:
		// Instance of MusicMenu
		static MusicMenu& GetInstance()
		{
			static MusicMenu instance;
			return instance;
		}

		void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	private:
		MusicMenu() {} // Private constructor to prevent external instantiation
		MusicMenu(const MusicMenu&) = delete;
		MusicMenu& operator=(const MusicMenu&) = delete;

		// Set for playing the music tracks
		// This doesn't need to be accessed outside this class.
		int currentMusicTrack = 1;

		// Set the current sound effect for PLAY_SOUND native
		int currentSoundEffect = 1;
	};
} // namespace Misc

