#include "pch.h"

#include "Constants.hpp"
#include "PedScripts.h"


// Player
#include "PlayerScripts.h"
#include "Peds/PedTaskScripts.h"

#include "MiscScripts.h"

#include "Util/UI.hpp"

// Chaos Mod
#include "Util/EntityIterator.h"
#include "Memory/Memory.h"
#include "Memory/WeaponPool.h"
#include "Util/Random.h"

PedScripts::PedScripts()
	: m_pedToSpawn(0),
	m_helicopterPed1(0),
	m_helicopterPed2(0),
	m_helicopterVehicle1(0),
	m_pedHash(0),
	m_enemyIsDead(false)
{

}

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
/// Ped scripts tick event, some items run constantly and some are toggling flags.
/// </summary>
void PedScripts::Tick()
{
#ifdef EXTRA_FEATURES

	//-----
	// Make peds attack player
	//-----
#ifdef MEMORY_TESTING
	if (PedScripts::isPedsAttackEnabled)
	{
		PedScripts::PedsAttackPlayer();
	}
#endif // MEMORY_TESTING
#endif //EXTRA_FEATURES

	//-----
	// Make all peds drive crazy
	//-----
#ifdef MEMORY_TESTING
	if (PedScripts::isCrazyPedDrivingEnabled)
	{
		PedScripts::MakeAllPedsDriveCrazy();
	}
#endif //

	//-----
	// Cleanup for the regular ped spawner
	//-----

	// I got this to work, I had to assign m_pedHash to modelHash in the ped spawner function.
	if (DoesEntityExist(m_pedToSpawn) && IS_ENTITY_DEAD(m_pedToSpawn, false) && !m_enemyIsDead)
	{
		MiscScripts::Model::MarkAsNoLongerNeeded(m_pedHash);
		//MiscScripts::Model::MarkPedAsNoLongerNeeded(m_pedToSpawn);
		//log_output("Ped cleanup ran");
		
		// Reset values back to defaults, although m_enemyIsDead defaults to false, this just breaks it out of the loop.
		m_enemyIsDead = true;
		m_pedHash = 0;
		m_pedToSpawn = 0;
	}

}

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

#pragma region CreatingPeds


/// <summary>
/// Create a ped, if using this function be sure to load the ped with the MiscScripts::LoadModel, or ModelScripts::LoadModel
/// Once I create it.
/// TODO Make a loop that runs if this ped is alive, if they die mark them as no longer needed.
/// </summary>
/// <param name="pedType">The type for the ped: https://alloc8or.re/gta5/doc/enums/ePedType.txt, now uses values from my Enum
/// Like this: PED_TYPE_CIVMALE</param>
/// <param name="modelHash">The model hash or name, can use my hash function like this: "a_m_y_stwhi_02"_hash or 
/// MISC::GET_HASH_KEY("a_m_y_stwhi_02"), list of ped model ids: https://docs.fivem.net/docs/game-references/ped-models/
/// </param>
/// <param name="position">A Vector3 of the position to spawn the ped.</param>
/// <param name="heading">The heading to spawn the ped.</param>
/// <param name="isEnemy">Make the ped hate the player if enabled.</param>
//void PedScripts::CreatePed(ePedType pedType, Hash modelHash, Vector3 position, float heading, bool isNetwork, bool bScriptHostPed)
void PedScripts::CreatePed(ePedType pedType, Hash modelHash, Vector3 position, float heading, bool isEnemy)
{
	auto& playerScripts = PlayerScripts::GetInstance();
	//Ped player = playerScripts.GetPlayerPed();
	Ped player = PLAYER_PED_ID();

	// Setup for making the ped an enemy
	Hash hostilePedGroup;
	static const Hash playerGroup = "PLAYER"_hash;

	// Well, this should remove the old peds first
	if (DoesEntityExist(m_pedToSpawn))
	{
		MiscScripts::Model::MarkAsNoLongerNeeded(m_pedHash);
	}

	// Create the ped
	// Check if it exists in the game, then request it
	if (MiscScripts::Model::IsInCdImage(modelHash))
	{
		MiscScripts::Model::Request(modelHash);
		m_pedToSpawn = CREATE_PED(pedType, modelHash, position, heading, false, false);

		// Just make them wander around for now, for testing
		if (DoesEntityExist(m_pedToSpawn))
		{
			// Assign the ped hash variable, to be cleaned up.
			m_pedHash = modelHash;
			// Some of the enemy code in here was taken from PedsMercenaries.cpp in Chaos Mod
			if (isEnemy)
			{
				// Give them a blip
				SetPedAsEnemy(m_pedToSpawn);

				ADD_RELATIONSHIP_GROUP("_ENEMY_PED", &hostilePedGroup);
				// Make them hate the player
				SET_RELATIONSHIP_BETWEEN_GROUPS(5, hostilePedGroup, playerGroup);
				SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroup, hostilePedGroup);
				// This should make them not hate each other
				SET_RELATIONSHIP_BETWEEN_GROUPS(0, hostilePedGroup, hostilePedGroup);

				// Set their group
				SET_PED_RELATIONSHIP_GROUP_HASH(m_pedToSpawn, hostilePedGroup);

				// Give them a weapon
				GIVE_WEAPON_TO_PED(m_pedToSpawn, WEAPON_CANDYCANE, -1, false, true);
				//SET_PED_ACCURACY(m_pedToSpawn, 50);

				// Set the combat attributes
				SET_PED_COMBAT_ATTRIBUTES(m_pedToSpawn, 0, true);
				SET_PED_COMBAT_ATTRIBUTES(m_pedToSpawn, 1, true);
				SET_PED_COMBAT_ATTRIBUTES(m_pedToSpawn, 2, true);
				SET_PED_COMBAT_ATTRIBUTES(m_pedToSpawn, 3, true);
				SET_PED_COMBAT_ATTRIBUTES(m_pedToSpawn, 5, true);
				SET_PED_COMBAT_ATTRIBUTES(m_pedToSpawn, 46, true);

				// Some final values for this
				REGISTER_TARGET(m_pedToSpawn, player);
				TASK_COMBAT_PED(m_pedToSpawn, player, 0, 16);
			}
			else {
				TASK_WANDER_STANDARD(m_pedToSpawn, 10.0f, 10.0f);
				//TASK_SKY_DIVE
			}
		}
		
	}

	
	/*
	// Other useful things to do for the ped:
			SET_ENTITY_HAS_GRAVITY(ped, false);
			SET_PED_CAN_RAGDOLL(ped, false);
			SET_ENTITY_COLLISION(ped, false, true);
			SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ped, player, false);
	*/
}




/// <summary>
/// Spawn a ped inside of a helicopter, make them fly to the middle of the map.
/// TODO Make this function accept a targetCoords along with the spawn position.
/// Also, rename position to spawnPosition.
/// Adapted from my FiveM lua scripts.
/// </summary>
/// <param name="pedType">The ped type from my enum</param>
/// <param name="modelHash">The ped model hash</param>
/// <param name="spawnPosition">The spawn position for the ped and helicopter</param>
/// <param name="targetPosition">The position to fly to</param>
/// <param name="heading">The spawn heading</param>
/// 
//void PedScripts::CreateHelicopterPed(ePedType pedType, Hash modelHash, Vector3 position, float heading)
void PedScripts::CreateHelicopterPed(ePedType pedType, Hash modelHash, Vector3 spawnPosition, Vector3 targetPosition, float heading)
{	
	auto& playerScripts = PlayerScripts::GetInstance();
	auto& pedTaskScripts = Scripts::Ped::Tasks::GetInstance();
	
	Ped player = playerScripts.GetPlayerPed();
	
	//Add 5 to the first set of coords and 10 to the other set, leave the z coord alone
	//This is where the helicopters spawn, and the peds teleport into them
	float heliSpawnPosX = spawnPosition.x + 5;
	float heliSpawnPosY = spawnPosition.y + 5;
	
	float pedSpawnPosX = spawnPosition.x + 10;
	float pedSpawnPosY = spawnPosition.y + 10;

	// TODO Move this into the function
	//Vector3 targetCoords = Vector3(-283.72, 806.09, 250.5);
	//

	// Helicopter speed and other values

	float speed = 20.0f; // Speed in meters per second
	float stopRange = 20.0f;
	float straightLineDistance = 20.0f;

	bool crashIntoRandomVehicle = false;
	bool landHelicopter = false;
	
	// Helicopter model
	Hash buzzardModel = "buzzard2"_hash;

	// Assign the ped to the hash value
	m_pedHash = modelHash;

	// Moved into these functions
	RemoveHeliPed();
	BlowupHelicopter();


	//Ped ped = CREATE_PED(pedType, modelHash, position, heading, isNetwork, bScriptHostPed);
	
	// Create the ped
	// First check if it exists in the game, then request it
	if (MiscScripts::Model::IsInCdImage(modelHash))
	{
		MiscScripts::Model::Request(modelHash);
		//m_helicopterPed1 = CREATE_PED(pedType, modelHash, position, heading, false, false);
		//m_helicopterPed1 = CREATE_PED(pedType, modelHash, Vector3(posX2, posY2, position.z), heading, false, false);
		m_helicopterPed1 = CREATE_PED(pedType, modelHash, Vector3(pedSpawnPosX, pedSpawnPosY, spawnPosition.z), heading, false, false);
		//pedTaskScripts.WarpPedIntoVehicle(m_helicopterPed1, )

		// Also add a blip onto them so I can find them..
		// TODO Make these into functions later
		
		SET_PED_HAS_AI_BLIP(m_helicopterPed1, true);

		// TODO Make an enum of these later, there are over 900 of these so I'll need to automate it.
		int helicopterRadarSprite = 64;
		SET_PED_AI_BLIP_SPRITE(m_helicopterPed1, helicopterRadarSprite);
		SET_PED_AI_BLIP_HAS_CONE(m_helicopterPed1, false);
	}

	// Create the helicopter
	// First check if it exists in the game, then request it
	if (MiscScripts::Model::IsInCdImage(buzzardModel))
	{
		MiscScripts::Model::Request(buzzardModel);
		//m_helicopterVehicle1 = CREATE_VEHICLE(buzzardModel, Vector3(posX1, posY1, position.z), 20.0f, false, false, false);
		m_helicopterVehicle1 = CREATE_VEHICLE(buzzardModel, Vector3(heliSpawnPosX, heliSpawnPosY, spawnPosition.z), 20.0f, false, false, false);
	}

	// Make sure both the helicopter ped and helicopter exist
	// If so, warp the ped into it and make them wander to the target.
	// TODO Test this
	if (DoesEntityExist(m_helicopterPed1) && DoesEntityExist(m_helicopterVehicle1))
	{
		pedTaskScripts.WarpPedIntoVehicle(m_helicopterPed1, m_helicopterVehicle1, 0);
		//pedTaskScripts.DriveToCoord(m_helicopterPed1, m_helicopterVehicle1, targetCoords, speed,
		pedTaskScripts.DriveToCoord(m_helicopterPed1, m_helicopterVehicle1, targetPosition, speed,
			false, buzzardModel, DrivingStyleIgnoreLights, stopRange, straightLineDistance);
	}
}

/// <summary>
/// Mark the heli ped as no longer needed, and remove them.
/// </summary>
void PedScripts::RemoveHeliPed()
{
	if (DoesEntityExist(m_helicopterPed1))
	{
		MiscScripts::Model::MarkAsNoLongerNeeded(m_pedHash);
		// TODO Test this
		//SET_PED_AS_NO_LONGER_NEEDED(&m_helicopterPed1);
		DELETE_PED(&m_helicopterPed1);

		m_helicopterPed1 = 0;
	}
}

/// <summary>
/// Mark the helicopter as no longer needed, and blow it up.
/// </summary>
void PedScripts::BlowupHelicopter()
{
	Hash buzzardModel = "buzzard2"_hash;

	// Values for explosion on the vehicles
	int gaspumpExplosion = 9;
	int railgunExplosion = 36;
	float damageScale = 100.0;
	float cameraShake = 1.0f;

	// This works for blowing up the helicopter.
	if (DoesEntityExist(m_helicopterVehicle1))
	{
		Vector3 heliCoords = GET_ENTITY_COORDS(m_helicopterVehicle1, false);
		MiscScripts::Model::MarkAsNoLongerNeeded(buzzardModel);
		DELETE_ENTITY(&m_helicopterVehicle1);
		ADD_EXPLOSION(heliCoords, gaspumpExplosion, damageScale, true, false, cameraShake, false);

		m_helicopterVehicle1 = 0;
	}
}

/// <summary>
/// TODO Set this up, make the ped spawned hate the player and attack them.
/// Also, give them a red blip on the map.
/// </summary>
/// <param name="ped"></param>
void PedScripts::SetPedAsEnemy(Ped ped)
{
	// First, give them a blip
	SET_PED_HAS_AI_BLIP(ped, true);

	// TODO Make an enum of these later, there are over 900 of these so I'll need to automate it.
	// Disabled for this function
	//int helicopterRadarSprite = 64;
	//SET_PED_AI_BLIP_SPRITE(ped, helicopterRadarSprite);
	SET_PED_AI_BLIP_HAS_CONE(ped, false);

	// Then, make them an enemy
	// TODO Figure this part out.


}

#pragma endregion


// TODO Test these.
bool PedScripts::DoesEntityExist(Ped ped)
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



// TODO Possibly put these into their own files, such as VehicleTasks.cpp, and so on.
// Actually, I created a Tasks.cpp, I may move these later.


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