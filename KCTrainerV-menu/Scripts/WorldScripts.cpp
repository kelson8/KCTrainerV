#include "pch.h"

#include "../common.h"
#include "WorldScripts.h"

#include "../Util/Enums.h"
#include "../Util/EntityIterator.h"


#include "../Natives/natives2.h"

/// <summary>
/// Run all WorldScripts tick events.
/// </summary>
void WorldScripts::Tick()
{
	// Turn on/off restricted areas
	// This works but doesn't turn back off, disabled for now.
	//if (WorldScripts::isRestrictedAreasDisabled)
	//{
	//    WorldScripts::DisableRestrictedAreas();
	//}

	//-----
	// Adapted from Menyoo
	// Set fireworks in a loop near the player
	//-----
	if (WorldScripts::isFireworksStarted)
	{
		WorldScripts::FireworksLoop();
	}

#ifdef MEMORY_TESTING
	//-----
	// Make all the peds in the area calm, meaning they shouldn't run from anything.
	//-----
	if (WorldScripts::isPedsCalmActive)
	{
		WorldScripts::SetPedsCalm();
	}
#endif // MEMORY_TESTING
	// End adapted from Menyoo

	//-----
	// Black out/EMP mode toggle
	// Well this toggle format works, I may have to do that with the other tick options.
	// Instead of making a enable/disable function for each one.
	//-----
	if (WorldScripts::isBlackoutActive && !WorldScripts::blackoutFlag)
	{
		WorldScripts::ToggleBlackout();
		//WorldScripts::EnableBlackoutMode();
		WorldScripts::blackoutFlag = true;
	}
	else if (!WorldScripts::isBlackoutActive && WorldScripts::blackoutFlag)
	{
		WorldScripts::ToggleBlackout();
		//WorldScripts::DisableBlackoutMode();       
		WorldScripts::blackoutFlag = false;
	}
}

/// <summary>
/// Kill all peds in the area.
/// </summary>
void WorldScripts::KillAllPedsInArea()
{
#ifdef MEMORY_TESTING
	auto& playerScripts = PlayerScripts::GetInstance();
	auto& worldScripts = WorldScripts::GetInstance();
	Ped playerPed = playerScripts.GetPlayerPed();

	//std::list<Entity> entities;
	// Can be used like this in the for loop:
	// entities.push_back(ped);
	for (auto ped : GetAllPeds())
	{
		// Check if they are the player and not dead, if so do nothing
		if (!PED::IS_PED_A_PLAYER(ped) && !ENTITY::IS_ENTITY_DEAD(ped, false))
		{
			ENTITY::SET_ENTITY_HEALTH(ped, 0, 0);
		}

	}

	// These can also be used.
	//entities.push_back(ped);
	//for (auto veh : GetAllVehs())
	//    entities.push_back(veh);
	//for (auto prop : GetAllProps())
	//    entities.push_back(prop);

#endif
}

/// <summary>
/// Blow up all vehicles in the area
/// </summary>
void WorldScripts::BlowUpAllVehiclesInArea()
{
#ifdef MEMORY_TESTING
	auto& playerScripts = PlayerScripts::GetInstance();

	Ped playerPed = playerScripts.GetPlayerPed();
	//Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerScripts.GetPlayerID(), false);
	Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

	// Possible fix for this being buggy and crashing, taken from Chaos Mod.
	int count = 3;

	for (Vehicle veh : GetAllVehs())
	{
		if (veh != playerVeh)
		{
			VEHICLE::EXPLODE_VEHICLE(veh, true, false);

			if (--count = 0)
			{
				count = 3;
				WAIT(0);
			}
		}
	}
#endif // MEMORY_TESTING
}

#pragma region Respawns

/// <summary>
/// Disable all hospital respawns.
/// </summary>
void WorldScripts::DisableHospitalSpawns()
{
	// This should possibly disable all hospital respawns
	// TODO Test this
	for (int i = 0; i < 10; i++)
	{
		DISABLE_HOSPITAL_RESTART(i, true);
		//std::cout << "Hospital " << i << " disabled." << std::endl;
		std::string hospistalDisabledString = std::format("Hospital {} disabled", i);
		log_output(hospistalDisabledString);
	}

	UI::Notify("All hospitals disabled");
}

/// <summary>
/// TODO Fix this
/// Set a new hospital respawn.
/// https://gtaforums.com/topic/839812-creating-a-respawn-point-hospital/
/// Well this didn't work.
/// </summary>
/// <param name="coords"></param>
/// <param name="heading"></param>
void WorldScripts::SetRespawnLocation(Vector3 coords, float heading)
{

	// I wonder what this will do?
	// Well these did nothing, does one of these need to be in a loop?
	// _DISABLE_AUTOMATIC_RESPAWN
	//PAUSE_DEATH_ARREST_RESTART(true);
	//IGNORE_NEXT_RESTART(true);

	// DisableHospitalSpawns();


	ADD_HOSPITAL_RESTART(coords, heading, false);

	UI::Notify("Set new spawnpoint");
}

#pragma endregion // Respawns

// Adapted from Menyoo, Routine.cpp

/// <summary>
/// Disable restricted areas, seems to work but this doesn't turn back off so I disabled it for now.
/// </summary>
void WorldScripts::DisableRestrictedAreas()
{
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
	TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_doors");
}

// Hmm, do I need to re-enable these?
//void WorldScripts::EnableRestrictedAreas()
//{
//
//}

/// <summary>
/// Loop for the fireworks tick.
/// </summary>
void WorldScripts::FireworksLoop()
{
	// Usage in Menyoo:
	/*
	//	start_fireworks_at_coord(GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player2.GetPed().Handle(),
	GET_RANDOM_FLOAT_IN_RANGE(-10.0f, 10.0f), GET_RANDOM_FLOAT_IN_RANGE(-6.0f, 27.0f), GET_RANDOM_FLOAT_IN_RANGE(-9.0f, 3.5f)),
	*/

	auto& playerScripts = PlayerScripts::GetInstance();
	auto& worldScripts = WorldScripts::GetInstance();

	Player playerPed = playerScripts.GetPlayerPed();

	Vector3 randomOffset = Vector3(0, MISC::GET_RANDOM_FLOAT_IN_RANGE(-90.0f, 90.0f), MISC::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 2.45f));
	Vector3 playerOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, randomOffset);

	float randomScale = MISC::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 2.45f);

	worldScripts.StartFireworksAtCoord(playerOffset, Vector3(0.0f, 0.0f, 0.0f), randomScale);

	Vector3 randomExplosionPos = Vector3(
		MISC::GET_RANDOM_FLOAT_IN_RANGE(9.0f, 25.0f),
		MISC::GET_RANDOM_FLOAT_IN_RANGE(5.0f, 25.0f),
		MISC::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 20.0f));

	// This seems to work.
	worldScripts.SetExplosionAtCoords(
		randomExplosionPos, EXPLOSION::DIR_WATER_HYDRANT, 8.0f, 0.0f, true, false);
}

void WorldScripts::StartFireworksAtCoord(const Vector3& pos, const Vector3& rot, float scale)
{
	if (!HAS_NAMED_PTFX_ASSET_LOADED("scr_indep_fireworks"))
		REQUEST_NAMED_PTFX_ASSET("scr_indep_fireworks");
	{
		std::vector<std::string> fw{ "scr_indep_firework_starburst", "scr_indep_firework_fountain", "scr_indep_firework_shotburst", "scr_indep_firework_trailburst" };
		//_9C720B61("scr_indep_fireworks");
		USE_PARTICLE_FX_ASSET("scr_indep_fireworks");
		SET_PARTICLE_FX_NON_LOOPED_COLOUR(GET_RANDOM_FLOAT_IN_RANGE(0.0f, 1.0f), GET_RANDOM_FLOAT_IN_RANGE(0.0f, 1.0f), GET_RANDOM_FLOAT_IN_RANGE(0.0f, 1.0f));
		//START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD(fw[rand() % 4].c_str(), pos.x, pos.y, pos.z, rot.x, rot.y, rot.z, scale, 0, 0, 0, false);
		START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD(fw[rand() % 4].c_str(), pos, rot, scale, 0, 0, 0, false);
	}
}

void WorldScripts::SetExplosionAtCoords(Vector3 pos, UINT8 type, float radius, float camshake, bool sound, bool invis)
{
	ADD_EXPLOSION(pos, type, radius, sound, invis, camshake, false);
}

void WorldScripts::SetPedsCalm()
{
#ifdef MEMORY_TESTING
	for (auto ped : GetAllPeds())
	{
		// Check if they are the player and not dead, if so do nothing
		if (!PED::IS_PED_A_PLAYER(ped) && !ENTITY::IS_ENTITY_DEAD(ped, false))
		{
			SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, 1);
			SET_PED_FLEE_ATTRIBUTES(ped, 0, 0);
			SET_PED_COMBAT_ATTRIBUTES(ped, 17, 1);
		}

	}
#endif // MEMORY_TESTING
}

// End adapted from Menyoo.

/// <summary>
/// Toggle blackout/EMP mode.
/// </summary>
void WorldScripts::ToggleBlackout()
{
	if (WorldScripts::isBlackoutActive) 
	{
		SET_ARTIFICIAL_LIGHTS_STATE(true);
	}
	else 
	{
		SET_ARTIFICIAL_LIGHTS_STATE(false);
	}
}

// TODO Adapt these from Menyoo

// Misc
/*
void set_blackoutEmp_mode()
{
	SET_ARTIFICIAL_LIGHTS_STATE(TRUE);

	for (auto& vehicle : _nearbyVehicles)
	{
		if (vehicle == g_myVeh) continue;

		NETWORK_REQUEST_CONTROL_OF_ENTITY(vehicle);
		SET_VEHICLE_ENGINE_ON(vehicle, 0, 1, 0);
		//SET_VEHICLE_LIGHTS(vehicle, 4);

	}

	ScrHandle tempSeq;
	OPEN_SEQUENCE_TASK(&tempSeq);
	TASK_LEAVE_ANY_VEHICLE(0, 0, 0);
	TASK_CLEAR_LOOK_AT(0);
	TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(0, 0);
	TASK_STAND_STILL(0, 300);
	TASK_START_SCENARIO_IN_PLACE(0, "WORLD_HUMAN_STAND_IMPATIENT", 800, 1);
	TASK_USE_MOBILE_PHONE_TIMED(0, 6000);
	//TASK_START_SCENARIO_IN_PLACE(0, "WORLD_HUMAN_STAND_MOBILE", 6000, 1);
	TASK_WANDER_STANDARD(0, 0x471c4000, 0);
	CLOSE_SEQUENCE_TASK(tempSeq);

	for (auto& ped : _nearbyPeds)
	{
		if (GET_SEQUENCE_PROGRESS(ped) < 0)
		{
			if (!IS_PED_IN_ANY_VEHICLE(ped, 0))
				continue;
			if (GET_ENTITY_SPEED(GET_VEHICLE_PED_IS_IN(ped, 0)) > 0.6f)
				continue;

			NETWORK_REQUEST_CONTROL_OF_ENTITY(ped);
			TASK_PERFORM_SEQUENCE(ped, tempSeq);
			SET_PED_KEEP_TASK(ped, 1);
			SET_PED_FLEE_ATTRIBUTES(ped, 0, 1);
			SET_PED_FLEE_ATTRIBUTES(ped, 1024, 1);
			SET_PED_FLEE_ATTRIBUTES(ped, 131072, 1);
		}
	}
	CLEAR_SEQUENCE_TASK(&tempSeq);
	//SET_ALL_RANDOM_PEDS_FLEE(PLAYER_ID(), 1);

}
void set_blackout_mode()
{
	SET_ARTIFICIAL_LIGHTS_STATE(TRUE);
	SET_ARTIFICIAL_VEHICLE_LIGHTS_STATE(FALSE);
}

*/
