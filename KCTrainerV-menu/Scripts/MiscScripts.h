#pragma once
#include "Constants.hpp"

#include "Util/Enums.h"

namespace MiscScripts
{
	namespace Lua
	{
		// Lua tests
#ifdef LUA_TEST
		void InitializeLuaMusic();
		void PlayLuaMusic(const std::string& track_id);
#endif //LUA_TEST
	}

	namespace Fade
	{
		// --- STATIC VARIABLES FOR THE FADE STATE MACHINE ---
		// These static variables persist across calls to TeleportFadeTick().
		// They are only visible within the MiscScripts namespace.
		enum FadeState {
			Idle,
			InitiateFadeOut,
			FadingOut,
			FadedOutWaiting,
			InitiateFadeIn,
			FadingIn,
		};

		// Declare the static variables (extern for definition in .cpp)
		extern FadeState s_currentFadeState;
		extern DWORD s_fadeWaitStartTime;
		extern Vector3 s_teleportTargetLocation;

		void InitiateTeleportFade(const Vector3& target);

		void FadeThread();
	} // namespace Fade

	namespace Model
	{
		void Request(Hash hash);
		void MarkAsNoLongerNeeded(Hash model);
		void MarkPedAsNoLongerNeeded(Ped ped);
		bool IsInCdImage(Hash model);
	} // namespace Model

} // Namespace MiscScripts
