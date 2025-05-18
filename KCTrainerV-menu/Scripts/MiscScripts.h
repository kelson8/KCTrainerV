#pragma once
#include "Constants.hpp"

#include "Util/Enums.h"

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

	// ID Gun tests from FiveM
	void IdGun();
	Entity GetEntityIsAimingAt(Ped ped);
	bool isIdGunEnabled = false;

	float entityIdMenuPosX = 0.190f;
	//float entityIdMenuPosY = 0.6f;
	float entityIdMenuPosY = 0.75f;

	float entityCoordsMenuPosX = 0.190f;
	//float entityCoordsMenuPosY = 0.7f;
	float entityCoordsMenuPosY = 0.80f;

	float entityHeadingMenuPosX = 0.190f;
	//float entityHeadingMenuPosY = 0.8f;
	float entityHeadingMenuPosY = 0.85f;

	float entityModelMenuPosX = 0.190f;
	//float entityModelMenuPosY = 0.973f;
	float entityModelMenuPosY = 0.9f;

	//

	// Play test music
	//void PlayTestMusic(int track);
	void PlayTestMusic(MusicTracks track);
	void StopTestMusic();

	// Ped tests
	void MakeAllPedsDriveCrazy();
	bool isCrazyPedDrivingEnabled = false;
	//

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

	// Peds
	// Attacking player and other stuff
	void PedsAttackPlayer();
	bool isPedsAttackEnabled = false;
	
#ifdef CHAOSMOD_FEATURES
	void SetAllPedsInMowers();
#endif //CHAOSMOD_FEATURES
	//


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
