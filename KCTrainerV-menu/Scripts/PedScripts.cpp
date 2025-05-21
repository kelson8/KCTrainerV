#include "pch.h"

#include "Constants.hpp"
#include "PedScripts.h"

#include "PlayerScripts.h"

#include "Util/UI.hpp"

// Chaos Mod
#include "Util/EntityIterator.h"
#include "Memory/Memory.h"
#include "Memory/WeaponPool.h"
#include "Util/Random.h"



// TODO Set these up
/*


REQUEST_CLIP_SET
HAS_CLIP_SET_LOADED
REMOVE_CLIP_SET

TASK_SKY_DIVE
TASK_PARACHUTE
TASK_PARACHUTE_TO_TARGET
SET_PARACHUTE_TASK_TARGET
SET_PARACHUTE_TASK_THRUST

* Vehicle:
TASK_PLANE_GOTO_PRECISE_VTOL
TASK_SUBMARINE_GOTO_AND_STOP
TASK_HELI_MISSION
TASK_HELI_ESCORT_HELI
TASK_PLANE_MISSION
TASK_PLANE_TAXI
TASK_BOAT_MISSION
TASK_DRIVE_BY
SET_DRIVEBY_TASK_TARGET
TASK_LEAVE_ANY_VEHICLE

This one might be fun
TASK_RAPPEL_FROM_HELI

* Other
TASK_USE_MOBILE_PHONE
TASK_USE_MOBILE_PHONE_TIMED

TASK_LOOK_AT_COORD
TASK_LOOK_AT_ENTITY
TASK_HANDS_UP - Might be useful to mess around with.

* Sequence tasks
OPEN_SEQUENCE_TASK
CLOSE_SEQUENCE_TASK

TASK_PERFORM_SEQUENCE
TASK_PERFORM_SEQUENCE_LOCALLY
CLEAR_SEQUENCE_TASK
GET_IS_TASK_ACTIVE

CLEAR_PED_TASKS


* Weapon
TASK_AIM_GUN_SCRIPTED
TASK_AIM_GUN_AT_COORD
TASK_SHOOT_AT_COORD
TASK_AIM_GUN_AT_ENTITY
*/

/// <summary>
/// Set all peds as cops.
/// Makes all peds in the area have a cop icon on the radar.
/// Taken from Chaos Mod
/// </summary>
/// 
#ifdef MEMORY_TESTING
void PedScripts::SetAllPedsAsCops()
{

	for (Ped ped : GetAllPeds())
	{
		Ped pedType = PED::GET_PED_TYPE(ped);

		if (!PED::IS_PED_A_PLAYER(ped) && pedType != 0 && pedType != 1 && pedType != 2)
			PED::SET_PED_AS_COP(ped, true);
	}

}
#endif


// This below is untested.
// I would like to spawn a ped, make them drive around and make the player warp into the passenger seat.
// TODO Set this up to be used in the function, possibly move this into the header.
// TODO Figure out better way to do this.
Ped ped = 0;

/// <summary>
/// Create a ped, if using this function be sure to load the ped with the MiscScripts::LoadModel, or ModelScripts::LoadModel
/// Once I create it.
/// TODO Set this up, possibly make this set a Ped value to be modified later.
/// </summary>
/// <param name="pedType">The type for the ped: https://alloc8or.re/gta5/doc/enums/ePedType.txt</param>
/// <param name="modelHash">The model hash or name, can use my hash function like this: "a_m_y_stwhi_02"_hash or 
/// MISC::GET_HASH_KEY("a_m_y_stwhi_02"), list of ped model ids: https://docs.fivem.net/docs/game-references/ped-models/
/// </param>
/// <param name="position">A Vector3 of the position to spawn the ped.</param>
/// <param name="heading">The heading to spawn the ped.</param>
/// 
/// TODO Possibly make these set to false by default in the function below
/// <param name="isNetwork">Mostly set to false in original scripts, 
/// seems to be true in Chaos Mod, I'll experiment with this one later
/// </param>
/// <param name="bScriptHostPed">Mostly set to false in original scripts.</param>
void PedScripts::CreatePed(int pedType, Hash modelHash, Vector3 position, float heading, bool isNetwork, bool bScriptHostPed)
{
	auto& playerScripts = PlayerScripts::GetInstance();
	Ped player = playerScripts.GetPlayerPed();
	//Ped ped = CREATE_PED(pedType, modelHash, position, heading, isNetwork, bScriptHostPed);
	ped = CREATE_PED(pedType, modelHash, position, heading, isNetwork, bScriptHostPed);
	/*
	// Other useful things to do for the ped:
			SET_ENTITY_HAS_GRAVITY(ped, false);
			SET_PED_CAN_RAGDOLL(ped, false);
			SET_ENTITY_COLLISION(ped, false, true);
			SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ped, player, false);
	*/
}



// TODO Test these.
bool PedScripts::DoesPedExist(Ped ped)
{
	if (DOES_ENTITY_EXIST(ped))
	{
		return true;
	}

	return false;
}

bool PedScripts::DoesVehicleExist(Vehicle vehicle)
{
	if (DOES_ENTITY_EXIST(vehicle))
	{
		return true;
	}

	return false;
}

// All these TaskFunctions below are untested.
#pragma region TaskFunctions

// TODO Possibly put these into their own files, such as VehicleTasks.cpp, and so on.
// Actually, I created a Tasks.cpp, I may move these later.


#pragma region VehicleTasks

/// <summary>
/// Set the ped into a vehicle
/// TODO What's the difference in this and the below function?
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="seatIndex"></param>
void PedScripts::SetPedIntoVehicle(Ped ped, Vehicle vehicle, int seatIndex)
{
	// First, check if the ped and vehicle exist, TODO Make a function for this later
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		SET_PED_INTO_VEHICLE(ped, vehicle, seatIndex);
	}
}

/// <summary>
/// Warp the ped into a vehicle
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="seat"></param>
void PedScripts::WarpPedIntoVehicle(Ped ped, Vehicle vehicle, int seat)
{
	// First, check if the ped and vehicle exist
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		TASK_WARP_PED_INTO_VEHICLE(ped, vehicle, seat);
	}
}

/// <summary>
/// Enter a vehicle instead of warping directly into one.
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="timeout"></param>
/// <param name="seat"></param>
/// <param name="speed"></param>
/// <param name="flag"></param>
/// <param name="overrideEntryClipsetName"></param>
void PedScripts::EnterVehicle(Ped ped, Vehicle vehicle, int timeout, int seat, float speed, int flag, const char* overrideEntryClipsetName)
{
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		TASK_ENTER_VEHICLE(ped, vehicle, timeout, seat, speed, flag, overrideEntryClipsetName);
	}
}

/// <summary>
/// Leave the vehicle
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="flags"></param>
void PedScripts::LeaveVehicle(Ped ped, Vehicle vehicle, int flags)
{
	// First, check if the ped and vehicle exist, TODO Make a function for this later
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		TASK_LEAVE_VEHICLE(ped, vehicle, flags);
	}
}

/// <summary>
/// Drive to a coord
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="pos"></param>
/// <param name="speed"></param>
/// <param name="p6"></param>
/// <param name="vehicleModel"></param>
/// <param name="drivingMode"></param>
/// <param name="stopRange"></param>
/// <param name="straightLineDistance"></param>
void PedScripts::DriveToCoord(Ped ped, Vehicle vehicle, Vector3 pos, float speed, Any p6, Hash vehicleModel, 
	int drivingMode, float stopRange, float straightLineDistance)
{
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		TASK_VEHICLE_DRIVE_TO_COORD(ped, vehicle, pos, speed, p6, vehicleModel, drivingMode, stopRange, straightLineDistance);
	}
}

/// <summary>
/// Drive to a coord long range
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="pos"></param>
/// <param name="speed"></param>
/// <param name="driveMode"></param>
/// <param name="stopRange"></param>
void PedScripts::DriveToCoordLongRange(Ped ped, Vehicle vehicle, Vector3 pos, float speed, int driveMode, float stopRange)
{
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(ped, vehicle, pos, speed, driveMode, stopRange);
	}
}

/// <summary>
/// Drive and wander around
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="speed"></param>
/// <param name="drivingStyle"></param>
void PedScripts::DriveWander(Ped ped, Vehicle vehicle, float speed, int drivingStyle)
{
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		TASK_VEHICLE_DRIVE_WANDER(ped, vehicle, speed, drivingStyle);
	}
}

/// <summary>
/// Find a spot to park the vehicle.
/// </summary>
/// <param name="ped"></param>
/// <param name="vehicle"></param>
/// <param name="pos"></param>
/// <param name="heading"></param>
/// <param name="mode"></param>
/// <param name="radius"></param>
/// <param name="keepEngineOn"></param>
void PedScripts::ParkVehicle(Ped ped, Vehicle vehicle, Vector3 pos, float heading, int mode, float radius, bool keepEngineOn)
{
	if (DOES_ENTITY_EXIST(ped) && DOES_ENTITY_EXIST(vehicle))
	{
		TASK_VEHICLE_PARK(ped, vehicle, pos, heading, mode, radius, keepEngineOn);
	}
}

void PedScripts::SetDrivingStyle(Ped ped, int drivingStyle)
{
	if (DOES_ENTITY_EXIST(ped))
	{
		SET_DRIVE_TASK_DRIVING_STYLE(ped, drivingStyle);
	}
}

void PedScripts::SetCruiseSpeed(Ped driver, float cruiseSpeed)
{
	if (DOES_ENTITY_EXIST(ped))
	{
		SET_DRIVE_TASK_CRUISE_SPEED(driver, cruiseSpeed);
	}

}

void PedScripts::SetMaxCruiseSpeed(Ped ped, float speed)
{
	if (DOES_ENTITY_EXIST(ped))
	{
		SET_DRIVE_TASK_MAX_CRUISE_SPEED(ped, speed);
	}
}

#pragma endregion // VehicleTasks



#pragma endregion // TaskFunctions

#pragma region WeaponFunctions

/// <summary>
/// Give the ped a weapon, tested working by giving weapon to myself.
/// https://nativedb.dotindustries.dev/gta5/natives/0xBF0FD6E56C964FCB?search=GIVE_WEAPON_TO_PED
/// https://www.vespura.com/fivem/weapons/stats/
/// https://wiki.rage.mp/wiki/Weapons
/// </summary>
/// <param name="ped"></param>
/// <param name="weaponHash"></param>
/// <param name="ammoCount"></param>
/// <param name="isHidden"></param>
/// <param name="bForceInHand"></param>
void PedScripts::GivePedWeapon(Ped ped, Hash weaponHash, int ammoCount, bool isHidden, bool bForceInHand)
{
	// First check if they have this weapon
	if (!HAS_PED_GOT_WEAPON(ped, weaponHash, false))
	{
		// Does this break it? It works fine without it...
		// TODO Figure out how to check if the model is valid
		//if (IS_MODEL_IN_CDIMAGE(weaponHash))
		//{
			GIVE_WEAPON_TO_PED(ped, weaponHash, ammoCount, isHidden, bForceInHand);
		//}
		//else {
		//	UI::Notify("Weapon doesn't exist");
		//}
	}

	// TODO Make a boolean for this.
	//if (IS_MODEL_IN_CDIMAGE(weaponHash))
	//{
		SET_CURRENT_PED_WEAPON(ped, weaponHash, bForceInHand);
	//}
}

#ifdef MEMORY_TESTING

// TODO Move this out of MiscScripts
/// <summary>
/// Make all peds in the area attack the player, PedsAttackPlayer.cpp in Chaos Mod
/// </summary>
void PedScripts::PedsAttackPlayer()
{
	static const Hash enemyGroupHash = MISC::GET_HASH_KEY("_ATTACK_PLAYER");
	static const Hash playerGroupHash = MISC::GET_HASH_KEY("PLAYER");

	// Set everyone to hate the player
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, enemyGroupHash, playerGroupHash);
	SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroupHash, enemyGroupHash);

	Player player = PLAYER_ID();
	Ped playerPed = PLAYER_PED_ID();
	int playerGroup = GET_PLAYER_GROUP(player);

	for (Ped ped : GetAllPeds())
	{
		if (!IS_PED_A_PLAYER(ped))
		{
			if (IS_PED_IN_GROUP(ped) && GET_PED_GROUP_INDEX(ped) == playerGroup)
				REMOVE_PED_FROM_GROUP(ped);

			SET_PED_RELATIONSHIP_GROUP_HASH(ped, enemyGroupHash);

			// https://nativedb.dotindustries.dev/gta5/natives/0x9F7794730795E019?search=SET_PED_COMBAT_ATTRIBUTES
			// 5 = BF_AlwaysFight
			// 46 = BF_CanFightArmedPedsWhenNotArmed
			SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
			SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

			// 2 = Unknown, not listed on the native website
			SET_PED_FLEE_ATTRIBUTES(ped, 2, true);

			// https://nativedb.dotindustries.dev/gta5/natives/0xF166E48407BAC484?search=TASK_COMBAT_PED
			// combatFlags seems to always be 0
			// threatResponseFlags seems to always be 16
			TASK_COMBAT_PED(ped, playerPed, 0, 16);
		}
	}
}

#endif // MEMORY_TESTING

/// <summary>
/// Give the ped a weapon attachment, tested working by giving attachment to myself.
/// https://nativedb.dotindustries.dev/gta5/natives/0xD966D51AA5B28BB9?search=GIVE_WEAPON_COMPONENT_TO_PED
/// https://wiki.rage.mp/wiki/Weapons_Components
/// </summary>
/// <param name="ped"></param>
/// <param name="weaponHash"></param>
/// <param name="componentHash"></param>
void PedScripts::GiveWeaponComponent(Ped ped, Hash weaponHash, Hash componentHash)
{ 
	if (HAS_PED_GOT_WEAPON(ped, weaponHash, false))
	{
		GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, componentHash);
	}
}


#ifdef MEMORY_TESTING
/// <summary>
/// TODO Test this
/// This should give the peds random weapons.
/// From Chaos Mod, originally in PedsEveryoneWepController.cpp
/// </summary>
/// <param name="ped">The ped to give a random weapon</param>
void PedScripts::GivePedRandomWeapons(Ped ped)
{
	const auto& weapons = Memory::GetAllWeapons();
	
	//Hash wep = weapons[g_Random.GetRandomInt(0, weapons.size() - 1)];
	// TODO Test this, should fix this warning.
	Hash wep = weapons[g_Random.GetRandomInt(0, static_cast<int>(weapons.size()) - 1)];

	GIVE_WEAPON_TO_PED(ped, wep, 9999, true, true);
	SET_CURRENT_PED_WEAPON(ped, wep, true);

}

/// <summary>
/// Another variation of the above function, this one gives random weapons to all peds.
/// </summary>
void PedScripts::GiveAllPedsRandomWeapons()
{
	const auto& weapons = Memory::GetAllWeapons();
	for (Ped ped : GetAllPeds())
	{
		//Hash wep = weapons[g_Random.GetRandomInt(0, weapons.size() - 1)];
		// TODO Test this, should fix this warning.
		Hash wep = weapons[g_Random.GetRandomInt(0, static_cast<int>(weapons.size()) - 1)];

		GIVE_WEAPON_TO_PED(ped, wep, 9999, true, true);
		SET_CURRENT_PED_WEAPON(ped, wep, true);
	}

	UI::Notify("All peds given random weapons");

}

/// <summary>
/// This seems to work.
/// Set all peds in the area to the rushed driving style.
/// </summary>
void PedScripts::MakeAllPedsDriveCrazy()
{

	Ped player = PLAYER_PED_ID();
	std::vector<Entity> entities;

	for (Ped ped : GetAllPeds())
		if (ped != player)
			entities.push_back(ped);

	//for (Vehicle veh : GetAllVehs())
	//	if (!IS_PED_IN_VEHICLE(player, veh, false))
	//		entities.push_back(veh);


	Vector3 playerCoord = GET_ENTITY_COORDS(player, false);
	for (Entity entity : entities)
	{
		// First check if the entity is a Ped
		if (IS_ENTITY_A_PED(entity))
		{
			// If so, set driving styles to rushed
			SET_DRIVE_TASK_DRIVING_STYLE(entity, DrivingStyle::Rushed);
		}
	}

}

#endif // MEMORY_TESTING


#pragma endregion // WeaponFunctions