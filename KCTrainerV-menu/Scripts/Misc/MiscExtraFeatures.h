#pragma once
#include "Constants.hpp"

#include "Util/Enums.h"

namespace MiscScripts
{
	namespace EXFeatures
	{
		void Tick();

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
	} // namespace EXFeatures

} // namespace MiscScripts

