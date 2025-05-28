#include "pch.h"

#include "Constants.hpp"

#include "MiscScripts.h"
#include "Scripts/PlayerScripts.h"
#include "Scripts/TextScripts.h"
#include "Scripts/VehicleScripts.h"

#include <format>

#include "inc/types.h"

#include "Util/UI.hpp"
#include "Util/Util.hpp"
#include "Script.hpp"

#include "../Teleports/TeleportLocations.h"

// Test for lua
#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

// Menyoo
#include "GTAped.h"
#include "GTAentity.h"
#include "GTAvehicle.h"
#include "Tasks.h"


// Chaos Mod
// Breaks this in here? I'll just manually use GET_HASH_KEY then..
//#include "Util/Hash.h"

#include "Util/EntityIterator.h"
#include "Util/Random.h"

#ifdef EXTRA_FEATURES
#include "Memory/Physics.h"
#include "Memory/Snow.h"
#include "Memory/Entity.h"
#include "Memory/Handle.h"

#include "Util/Vehicle.h"
#endif //EXTRA_FEATURES

#include "Memory/Misc.h"

// Lua tests
#ifdef LUA_TEST
#include "../Components/LuaManager.h"

namespace MiscScripts
{
	namespace Lua
	{
		void InitializeLuaMusic()
		{
			auto& luaManager = LuaManager::GetInstance();
			luaManager.load_script("scripts/music_config.lua");
		}

		void PlayLuaMusic(const std::string& track_id)
		{
			auto& luaManager = LuaManager::GetInstance();
			sol::optional<sol::function> play_func = luaManager.get_function("play_music");
			if (play_func) {
				(*play_func)(track_id);
			}
			else {
				// Handle error: play_music function not found
			}
		}
	}
}


#endif //LUA_TEST


namespace MiscScripts
{

#pragma region FadeFunctions
	namespace Fade
	{
		FadeState s_currentFadeState = Idle; // Initialize directly
		static DWORD s_fadeWaitStartTime = 0; // For tracking wait duration
		static Vector3 s_teleportTargetLocation; // To store the destination

		void InitiateTeleportFade(const Vector3& target) {
			if (s_currentFadeState == Idle) {
				s_teleportTargetLocation = target;
				s_currentFadeState = InitiateFadeOut;
			}
		}

		/// <summary>
		/// TODO Test this, run it in the DLLMain
		/// </summary>
		
		// --- THE TICK FUNCTION FOR THE FADE STATE MACHINE ---
		// This function must be called repeatedly, once per game frame,
		// from your main script loop (e.g., Thread_menu_loops2).
		void FadeThread() {
			Ped playerPed = PLAYER_PED_ID(); // Get player ped every tick as it might change

			// Define your fade durations (can be consts or variables)
			const int fadeOutDuration = 500; // Milliseconds
			const int waitAfterFadeOut = 500; // Milliseconds
			const int fadeInDuration = 500;  // Milliseconds

			switch (s_currentFadeState) {
			case Idle:
				// Do nothing when idle.
				break;

			case InitiateFadeOut:
				// Start the fade out if not already fading/faded
				if (!IS_SCREEN_FADED_OUT() && !IS_SCREEN_FADING_OUT()) {
					DO_SCREEN_FADE_OUT(fadeOutDuration);
					FREEZE_ENTITY_POSITION(playerPed, true);
					s_currentFadeState = FadingOut;
				}
				else {
					// If already fading or faded (e.g., button spammed),
					// transition to the next appropriate state immediately.
					if (IS_SCREEN_FADED_OUT()) {
						s_currentFadeState = FadedOutWaiting;
						s_fadeWaitStartTime = GetTickCount();
					}
					else if (IS_SCREEN_FADING_OUT()) {
						s_currentFadeState = FadingOut;
					}
				}
				break;

			case FadingOut:
				// Wait for the screen to be fully faded out
				if (IS_SCREEN_FADED_OUT()) {
					// Screen is fully black. Perform the teleport.
					// THIS IS THE CRITICAL POINT FOR TELEPORTATION!
					SET_ENTITY_COORDS(playerPed, s_teleportTargetLocation, false, false, false, true);

					// Set timer for waiting in blackness
					s_fadeWaitStartTime = GetTickCount();
					s_currentFadeState = FadedOutWaiting;
				}
				break;

			case FadedOutWaiting:
				// Wait for the desired duration while the screen is black
				if (GetTickCount() - s_fadeWaitStartTime >= waitAfterFadeOut) {
					s_currentFadeState = InitiateFadeIn;
				}
				break;

			case InitiateFadeIn:
				// Start the fade in if not already fading/faded
				if (!IS_SCREEN_FADED_IN() && !IS_SCREEN_FADING_IN()) {
					DO_SCREEN_FADE_IN(fadeInDuration);
					FREEZE_ENTITY_POSITION(playerPed, false); // Unfreeze player
					s_currentFadeState = FadingIn;
				}
				else {
					// If already fading or faded in, go back to Idle
					s_currentFadeState = Idle;
				}
				break;

			case FadingIn:
				// Wait for the screen to be fully faded in
				if (IS_SCREEN_FADED_IN()) {
					s_currentFadeState = Idle; // Fade sequence complete
				}
				break;
			}
		}
	} // namespace Fade


	namespace Model
	{
		/// <summary>
		/// Taken from Menyoo
		/// Request the vehicle model for spawning in, this actually should work on any model
		/// </summary>
		/// <param name="hash"></param>
		void Request(Hash hash)
		{
			REQUEST_MODEL(hash);
			while (!HAS_MODEL_LOADED(hash))
			{
				WAIT(0);
			}

			log_output(std::format("Model {} requested for game script.", hash));
		}

		
		/// <summary>
		/// Set model as no longer needed, let the game cleanup the model for later.
		/// </summary>
		/// <param name="model"></param>
		void MarkAsNoLongerNeeded(Hash model) 
		{
			SET_MODEL_AS_NO_LONGER_NEEDED(model);
			log_output(std::format("Model {} set as no longer needed, to be cleaned up.", model));
		}

		/// <summary>
		/// Set ped as no longer needed, let the game cleanup the ped for later.
		/// </summary>
		/// <param name="ped"></param>
		void MarkPedAsNoLongerNeeded(Ped ped)
		{
			SET_PED_AS_NO_LONGER_NEEDED(&ped);
			log_output(std::format("Ped {} set as no longer needed, to be cleaned up.", ped));
		}

		// TODO Add these to the header

		/// <summary>
		/// Is the model in the cdimage.
		/// </summary>
		/// <param name="model"></param>
		/// <returns></returns>
		bool IsInCdImage(Hash model)
		{
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model))
			{
				return true;
			}

			return false;
		}

		/// <summary>
		/// Is the model in the cdimage, and is it a vehicle?
		/// </summary>
		/// <param name="model"></param>
		/// <returns></returns>
		bool IsAVehicle(Hash model)
		{
			//if (!(STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))) {
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model)) {
				return true;
			}

			return false;
		}

	} // namespace Model

#pragma endregion
} // namespace MiscScripts