#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "../Scripts/Extras/Classes/GTAblip.h"

#include "IMenuBuilder.h"

class MiscMenu : public IMenuBuilder
{
public:
	// Instance of the MiscMenu
	static MiscMenu& GetInstance()
	{
		static MiscMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;
	void BuildBlipsSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

private:
	MiscMenu() {} // Private constructor to prevent external instantiation
	MiscMenu(const MiscMenu&) = delete;
	MiscMenu& operator=(const MiscMenu&) = delete;

	// Set for playing the music tracks
	// This doesn't need to be accessed outside this class.
	int currentMusicTrack = 1;

	// Set the current sound effect for PLAY_SOUND native
	int currentSoundEffect = 1;

	// Current set blip for the blip menu

	// Hmm, I didn't know the World namespace had creating blips.
	//GTAblip blip = World::CreateBlip(Teleports::vSafeHouseLocations.at("Michael's House"));
	// Well no wonder this never worked, it kept getting changed in the loop.
	GTAblip blip;
};

