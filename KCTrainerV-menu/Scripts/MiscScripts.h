#pragma once
#include "Constants.hpp"

#include <inc/natives.h>

class MiscScripts
{
public:

	// Instance of the MiscScripts
	static MiscScripts& GetInstance()
	{
		static MiscScripts instance;
		return instance;
	}

	// Lua tests
#ifdef LUA_TEST
	void InitializeLuaMusic();
	void PlayLuaMusic(const std::string& track_id);
#endif //LUA_TEST

	// Play test music
	void PlayTestMusic(int track);
	void StopTestMusic();

	void PlayArenaWarLobbyMusic();
	void StopArenaWarLobbyMusic();
	
	void StartCreditsMusic();
	void StopCreditsMusic();

	// Tests
	void StartAirstrikeTest();
	void StopAirstrikeTest();

	// Toggle sky
#ifdef EXTRA_FEATURES
	void EnableSky();
	void DisableSky();

	// Toggle snow

	void EnableSnow();
	void DisableSnow();

	// Toggle forcefield
	void EnableForceField();
	void DisableForceField();

	// Toggle TV
	void EnableTv();
	void DisableTv();
	// Tick event for the TV
	void TvTick();

	bool isTVRunning = false;
#endif //EXTRA_FEATURES

	bool airStrikeRunning;

#ifdef EXTRA_FEATURES
	bool isForceFieldEnabled = false;
#endif //EXTRA_FEATURES

private:
	MiscScripts() {} // Private constructor to prevent external instantiation
	MiscScripts(const MiscScripts&) = delete;
	MiscScripts& operator=(const MiscScripts&) = delete;

	bool hasAirstrikeLoaded = false;

	bool isSkyDisabled = false;
	bool isSnowEnabled = false;
};
