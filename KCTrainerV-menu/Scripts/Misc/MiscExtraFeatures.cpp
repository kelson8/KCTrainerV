#include "pch.h"
#include "MiscExtraFeatures.h"

#include "Constants.hpp"

//#include "MiscScripts.h"
#include "Scripts/PlayerScripts.h"
#include "Scripts/TextScripts.h"
#include "Scripts/VehicleScripts.h"

//#include <format>

#include "inc/types.h"

#include "Util/UI.hpp"
#include "Util/Util.hpp"
#include "Script.hpp"

//#include "../Teleports/TeleportLocations.h"
#include "Util/Random.h"

// Test for lua
#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

// Menyoo
#include "GTAped.h"
#include "GTAentity.h"
#include "GTAvehicle.h"
#include "Tasks.h"

// Memory
#include "Memory/Physics.h"
#include "Memory/Memory.h"
#include "Memory/Misc.h"
#include "Memory/Snow.h"
#include "Util/EntityIterator.h"

namespace MiscScripts
{
	namespace EXFeatures
	{
		bool isTVRunning = false;

		bool hasAirstrikeLoaded = false;
		int lastAirStrike = 0;
		bool airStrikeRunning = false;

		bool isSkyDisabled = false;
		bool isSnowEnabled = false;

		bool isForceFieldEnabled = false;

		/// <summary>
		/// Main tick for EXFeatures
		/// </summary>
		void Tick()
		{
			//-----
			// Run the airstrikes
			//-----
			if (airStrikeRunning)
			{
				StartAirstrikeTest();
			}

#ifdef EXTRA_FEATURES
			//-----
			// Player force field
			//-----
#ifdef MEMORY_TESTING
			// Seems to crash, disabled.
			//if (isForceFieldEnabled)
			//{
			//    EnableForceField();
			//}
#endif // MEMORY_TESTING

			//-----
			// Run the tick event for the TV if enabled.
			//-----
			if (isTVRunning)
			{
				TvTick();
			}
#endif //EXTRA_FEATURES
		}

		// Taken from MiscAirStrike.cpp in Chaos Mod
		static Vector3 getRandomOffsetCoord(Vector3 startCoord, float maxOffset)
		{
			return Vector3::Init(startCoord.x + g_Random.GetRandomFloat(-maxOffset, maxOffset),
				startCoord.y + g_Random.GetRandomFloat(-maxOffset, maxOffset),
				startCoord.z + g_Random.GetRandomFloat(-maxOffset, maxOffset));
		}

		Hash airstrikeHash;
		void StartAirstrikeTest()
		{
			airstrikeHash = MISC::GET_HASH_KEY("WEAPON_AIRSTRIKE_ROCKET");
			//airstrikeHash = "WEAPON_AIRSTRIKE_ROCKET"_hash;
			// Don't want this loading multiple times.
			if (!hasAirstrikeLoaded)
			{
				REQUEST_WEAPON_ASSET(airstrikeHash, 31, 0);
				while (!HAS_WEAPON_ASSET_LOADED(airstrikeHash))
					WAIT(0);

				hasAirstrikeLoaded = true;
			}

			int current_time = GET_GAME_TIMER();
			if (current_time - lastAirStrike > 1000)
				//if (current_time - lastAirStrike > 100)
			{
				lastAirStrike = current_time;
				Ped player = PLAYER_PED_ID();
				Vector3 playerPos = GET_ENTITY_COORDS(player, false);
				Vector3 startPosition = getRandomOffsetCoord(playerPos, 10);
				Vector3 targetPosition = getRandomOffsetCoord(playerPos, 50);
				float groundZ = 0;
				//if (GET_GROUND_Z_FOR_3D_COORD(targetPosition.x, targetPosition.y, targetPosition.z, &groundZ, false, false))
				if (GET_GROUND_Z_FOR_3D_COORD(startPosition, &groundZ, false, false))
				{
					/*
					SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startPosition.x, startPosition.y, startPosition.z + 200,
					targetPosition.x, targetPosition.y, groundZ, 200, true, airstrikeHash, 0,
					true, false, 5000);
					*/
					SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Vector3(startPosition.x, startPosition.y, startPosition.z + 200),
						Vector3(targetPosition.x, targetPosition.y, groundZ), 200, true, airstrikeHash, 0,
						true, false, 5000);
				}
			}
		}

		void StopAirstrikeTest()
		{
			REMOVE_WEAPON_ASSET(airstrikeHash);
			hasAirstrikeLoaded = false;
		}

#ifdef EXTRA_FEATURES

#ifdef MEMORY_TESTING
		void EnableSky()
		{
			Memory::SetSkyDisabled(false);
			isSkyDisabled = false;
		}
#endif

#ifdef MEMORY_TESTING
		void DisableSky()
		{
			Memory::SetSkyDisabled(true);
			isSkyDisabled = true;
		}
#endif

#ifdef MEMORY_TESTING
		void EnableSnow()
		{
			// TODO Implement extra shader from Chaos Mod.
			// Will require some more memory stuff and modifications.
			Memory::SetSnow(true);
			isSnowEnabled = true;
		}
#endif

#ifdef MEMORY_TESTING
		void DisableSnow()
		{
			Memory::SetSnow(false);
			isSnowEnabled = false;

		}
#endif // MEMORY_TESTING

#endif //EXTRA_FEATURES

		// Taken from PlayerForceField.cpp in Chaos Mod.
#ifdef EXTRA_FEATURES

#pragma region ForceFieldFunctions

#ifdef MEMORY_TESTING

		void EnableForceField()
		{
			Ped player = PLAYER_PED_ID();
			std::vector<Entity> entities;

			for (Ped ped : GetAllPeds())
				if (ped != player)
					entities.push_back(ped);

			for (Vehicle veh : GetAllVehs())
				if (!IS_PED_IN_VEHICLE(player, veh, false))
					entities.push_back(veh);

			for (Entity prop : GetAllProps())
				entities.push_back(prop);

			Vector3 playerCoord = GET_ENTITY_COORDS(player, false);
			for (Entity entity : entities)
			{
				static const float startDistance = 15;
				static const float maxForceDistance = 10;
				static const float maxForce = 100;
				Vector3 entityCoord = GET_ENTITY_COORDS(entity, false);
				float distance = GET_DISTANCE_BETWEEN_COORDS(playerCoord, entityCoord, true);
				if (distance < startDistance)
				{
					if (IS_ENTITY_A_PED(entity) && !IS_PED_RAGDOLL(entity))
						SET_PED_TO_RAGDOLL(entity, 5000, 5000, 0, true, true, false);
					float forceDistance = std::min(std::max(0.f, (startDistance - distance)), maxForceDistance);
					float force = (forceDistance / maxForceDistance) * maxForce;
					Memory::ApplyForceToEntity(entity, 3, entityCoord.x - playerCoord.x, entityCoord.y - playerCoord.y,
						entityCoord.z - playerCoord.z, 0, 0, 0, false, false, true, true, false, true);
				}
			}
			//isForceFieldEnabled = true;

		}

		void DisableForceField()
		{
			isForceFieldEnabled = false;
		}

#pragma endregion

#pragma region PedFunctions



#endif // MEMORY_TESTING

#ifdef CHAOSMOD_FEATURES
		void MiscScripts::SetAllPedsInMowers()
		{
			static const Hash mowerHash = MISC::GET_HASH_KEY("MOWER");
			SetSurroundingPedsInVehicles(mowerHash, 120);
		}
#endif

#pragma endregion

#pragma region TVFunctions

		// Tv channels
		const char* TV_PLAYLISTS[] = {
			"PL_WEB_KFLF",    // Kung Fu Rainbow Lazerforce
			"PL_WEB_RANGERS", // Republican Space Rangers
			"PL_WEB_PRB2",    // Princess Robot Bubblegum
			"PL_WEB_FOS",     // Fame or Shame
			"PL_WEB_CAS",     // Diamond Casino DLC intro
			"PL_WEB_FOS",
			"PL_WEB_HOWITZER", // Howitzer Documentary
			"PL_WEB_KFLF",
			"PL_WEB_PRB2",
			"PL_WEB_RS",
			"PL_STD_CNT",
			"PL_STD_WZL",
			"PL_LO_CNT",
			"PL_LO_WZL",
			"PL_SP_WORKOUT",
			"PL_SP_INV",
			"PL_SP_INV_EXP",
			"PL_LO_RS",
			"PL_LO_RS_CUTSCENE",
			"PL_SP_PLSH1_INTRO",
			"PL_LES1_FAME_OR_SHAME",
			"PL_STD_WZL_FOS_EP2",
			"PL_MP_WEAZEL",
			"PL_MP_CCTV",
			"PL_CINEMA_CARTOON",
			"PL_CINEMA_ARTHOUSE",
			"PL_CINEMA_ACTION",
			"PL_CINEMA_MULTIPLAYER",
			"PL_CINEMA_MULTIPLAYER_NO_MELTDOWN",
		};
		//

		// Tv options
		float ms_PosX = 0.f;
		float ms_PosY = 0.f;

		/// <summary>
		/// Enable the in game TV with a random position and enable the isTVRunning boolean.
		/// </summary>
		void EnableTv()
		{
			// First check if the tv is not running
			if (!isTVRunning)
			{
				auto playlist = TV_PLAYLISTS[g_Random.GetRandomInt(0, sizeof(TV_PLAYLISTS) / sizeof(TV_PLAYLISTS[0]) - 1)];

				GRAPHICS::SET_TV_CHANNEL_PLAYLIST_AT_HOUR(0, playlist, g_Random.GetRandomInt(0, 23));
				GRAPHICS::SET_TV_AUDIO_FRONTEND(true);
				GRAPHICS::SET_TV_VOLUME(1.0f); // 0.0 is actually the highest the player can normally tune to.
				GRAPHICS::ATTACH_TV_AUDIO_TO_ENTITY(PLAYER_PED_ID());
				GRAPHICS::SET_TV_CHANNEL(0);
				GRAPHICS::ENABLE_MOVIE_SUBTITLES(true);

				ms_PosX = g_Random.GetRandomFloat(0.3f, 0.7f);
				ms_PosY = g_Random.GetRandomFloat(0.3f, 0.7f);
				isTVRunning = true;
			}
		}

		/// <summary>
		/// This gets run in the KCMenuScript main tick.
		/// </summary>
		void TvTick()
		{
			GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(4);
			GRAPHICS::SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU(1);
			//GRAPHICS::DRAW_TV_CHANNEL(ms_PosX, ms_PosY, 0.3f, 0.3f, 0.0f, 255, 255, 255, 255);
			GRAPHICS::DRAW_TV_CHANNEL(Vector2(ms_PosX, ms_PosY), Vector2(0.3f, 0.3f), 0.0f, 255, 255, 255, 255);
		}

		/// <summary>
		/// Turn off the TV and disable the isTVRunning boolean.
		/// </summary>
		void DisableTv()
		{
			// First check if the tv is running
			if (isTVRunning)
			{
				GRAPHICS::SET_TV_CHANNEL(-1);
				GRAPHICS::SET_TV_CHANNEL_PLAYLIST(0, nullptr, false);
				GRAPHICS::ENABLE_MOVIE_SUBTITLES(false);
				isTVRunning = false;
			}
		}

#pragma endregion

#endif //EXTRA_FEATURES

#pragma region MenyooTest

		//----------- Begin Menyoo tests --------------//

		// Moved out of PlayerScripts.cpp.
		// TODO Test this, using new GTAped and GTAentity from Menyoo
		// This works for teleporting, I just had to adapt the way Menyoo was using these.
		// Look into GTAentity.cpp for some of these, GTAped.cpp, and GTAplayer.cpp are also useful.
		void MenyooTest()
		{
			Ped playerPedID = PLAYER_PED_ID();
			//GTAped playerPed = playerPedID;
			GTAped playerPed = PLAYER_PED_ID();


			// Testing with tasks
			bool taskTest = true;

			// Playing animations on the player
			bool playAnimation = false;

			// If enabled, this applies random force to vehicles in the area pretty much throwing them.
			bool throwVehicles = false;

			const Vector3& playerPos = playerPed.Position_get();

			// This seems to work, has a short delay to it.
			if (playAnimation)
			{
				if (!playerPed.Task().IsPlayingAnimation("mini@strip_club@idles@dj@idle_01", "idle_01"))
					playerPed.Task().PlayAnimation("mini@strip_club@idles@dj@idle_01", "idle_01", 1, 1, -1, AnimFlag::UpperBodySecondTask, 0, false);
			}

			if (taskTest)
			{
				for (GTAped ped : GetAllPeds())
				{
					// Oops, forgot the player check.
					if (!ped.IsPlayer())
					{
						//ped.Task().Cower(2500);
						//ped.Task().HandsUp(2500);
						//ped.Task().WanderAround();

						// This is fun, I can set them all on fire and extinguish them
						if (ped.IsOnFire())
						{
							ped.SetOnFire(false);
						}
						else
						{
							ped.SetOnFire(true);
						}
					}
				}
			}

			// TODO Test later
			//GTAvehicle playerVeh;
			//VehicleSeat currVehSeat;
			//if (playerPed.IsInVehicle())
			//{
			//    playerVeh = playerPed.CurrentVehicle();
			////    currVehSeat = playerPed.CurrentVehicleSeat_get();
			//}

			 //playerPed.CurrentVehicle();

			//playerPed.Armour_set(250);

			// This teleport seems to work fine.
			Vector3 michealsHouseCoords = Vector3(-813.603f, 179.474f, 72.1548f);
			float michealsHouseHeading = 0.0f;

			// Can get/set position:
			//playerPed.Position_get();
			//playerPed.Position_set(michealsHouseCoords);


			// Can get/set rotation:
			//playerPed.Rotation_get();
			//playerPed.Rotation_set(Vector3(0.0f, 0.0f, 0.0f));

			//playerPed.CanFlyThroughWindscreen_get();
			//playerPed.CanFlyThroughWindscreen_set(true);

			// Hmm, I can use GTAped in my for loops? Might be fun

			// This seems to work, set the peds driving speeds to random.
			// TODO Move this into VehicleScripts.
			// Set the min and max speeds for the below, moved out of the loop I only want it randomized once.
			//float minSpeed = 25.0f;
			//float maxSpeed = 40.0f;

			//// Get a random value from the two values above.
			//float randomSpeed = g_Random.GetRandomFloat(minSpeed, maxSpeed);

			//for (GTAped ped : GetAllPeds())
			//{
			//    // Check if they are the player and not dead, if so do nothing
			//    if (!ped.IsPlayer() || ped.IsAlive())
			//    {
			//        if (ped.IsInVehicle())
			//        {
			//            //ped.DrivingSpeed_set(40.0f);
			//            ped.DrivingSpeed_set(randomSpeed);
			//        }
			//    }
			//    
			//    std::string drivingSpeedString = std::format("All peds driving speed set to {}", randomSpeed);

			//    std::cout << drivingSpeedString << std::endl;
			//    UI::Notify(drivingSpeedString);
			//}


			// Well this seems to work.

			if (throwVehicles)
			{
				float minRandomNumber = 10.0f;
				float maxRandomNumber = 40.0f;
				float randomNumber = g_Random.GetRandomFloat(minRandomNumber, maxRandomNumber);
				Vector3 randomCoords = Vector3(randomNumber, randomNumber, randomNumber);

				// Vehicle testing
				for (GTAvehicle vehicle : GetAllVehs())
				{
					//vehicle.AlarmActive_set(true);
					// Well this works so other stuff should work here.
					//vehicle.Explode();
					vehicle.ApplyForce(randomCoords, ForceType::MaxForceRot);
				}
			}



			//for (Ped ped : GetAllPeds())
			//    if (ped != player)
			//        entities.push_back(ped);

			//for (Vehicle veh : GetAllVehs())
			//    if (!IS_PED_IN_VEHICLE(player, veh, false))
			//        entities.push_back(veh);

			//for (Entity prop : GetAllProps())
			//    entities.push_back(prop);

		}

		//----------- End Menyoo tests --------------//

#pragma endregion

	} // namespace EXFeatures
} // namespace MiscScripts