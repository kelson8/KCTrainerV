#pragma once
#include "Util/Enums.h"

// TODO Refactor this file a bit, possibly make a Config class under the IDGun namespace for the variables like before.
namespace MiscScripts
{
	namespace IDGun
	{
		// ID Gun tests from FiveM
		void IdGun();
		Entity GetEntityIsAimingAt(Ped ped);
		//bool isIdGunEnabled = false;
		extern bool isIdGunEnabled;

		//float entityIdMenuPosX = 0.190f;
		extern float entityIdMenuPosX;
		//float entityIdMenuPosY = 0.75f;
		extern float entityIdMenuPosY;

		//float entityCoordsMenuPosX = 0.190f;
		extern float entityCoordsMenuPosX;
		//float entityCoordsMenuPosY = 0.80f;
		extern float entityCoordsMenuPosY;

		//float entityHeadingMenuPosX = 0.190f;
		extern float entityHeadingMenuPosX;
		//float entityHeadingMenuPosY = 0.85f;
		extern float entityHeadingMenuPosY;

		//float entityModelMenuPosX = 0.190f;
		extern float entityModelMenuPosX;
		//float entityModelMenuPosY = 0.9f;
		extern float entityModelMenuPosY;

		//
	}

	namespace Music
	{
		// Lua tests
#ifdef LUA_TEST
		void InitializeLuaMusic();
		void PlayLuaMusic(const std::string& track_id);
#endif //LUA_TEST

		// Play test music
		//void PlayTestMusic(int track);
		void PlayTestMusic(MusicTracks track);
		void StopTestMusic();

		void PlayArenaWarLobbyMusic();
		void StopArenaWarLobbyMusic();

		void StartCreditsMusic();
		void StopCreditsMusic();
	} // namespace Music

	namespace EXFeatures
	{
		// Toggle sky
#ifdef EXTRA_FEATURES
#ifdef MEMORY_TESTING
		extern int lastAirstrike;
		extern bool airStrikeRunning;

		// Tests
		void StartAirstrikeTest();
		void StopAirstrikeTest();

		void EnableSky();
		void DisableSky();

		// Toggle snow

		void EnableSnow();
		void DisableSnow();

		// Toggle forcefield
		void EnableForceField();
		void DisableForceField();

#endif // MEMORY_TESTING

#ifdef CHAOSMOD_FEATURES
		void SetAllPedsInMowers();
#endif //CHAOSMOD_FEATURES
		//


		// Toggle TV
		void EnableTv();
		void DisableTv();
		// Tick event for the TV
		void TvTick();

		//bool isTVRunning = false;
		extern bool isTVRunning;
#endif //EXTRA_FEATURES

		

#ifdef EXTRA_FEATURES
		//bool isForceFieldEnabled = false;
		extern bool isForceFieldEnabled;
#endif //EXTRA_FEATURES

		//bool hasAirstrikeLoaded = false;
		extern bool hasAirstrikeLoaded;

		//bool isSkyDisabled = false;
		extern bool isSkyDisabled;
		//bool isSnowEnabled = false;
		extern bool isSnowEnabled;

		// Testing for Menyoo
		void MenyooTest();
	}
} // Namespace MiscScripts
