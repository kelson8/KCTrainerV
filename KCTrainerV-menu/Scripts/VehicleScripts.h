#pragma once
#include "../Constants.hpp"

#include <inc/natives.h>

class VehicleScripts
{
public:

	// Instance of the VehicleScripts
	// TODO Re-enable
#ifdef VEHICLE_SCRIPTS_SINGLETON
	static VehicleScripts& GetInstance()
	{
		static VehicleScripts instance;
		return instance;
	}
#endif

	// Request vehicle model
	//void RequestModel(Hash model);
	bool RequestModel(Hash model);

	void ChangePlayerVehicleColor();
	static void ToggleBombBayDoors();

	Vehicle GetPlayerVehicle();

	void RepairVehicle();

	// Spawning
	void SpawnVehicle(Hash hash);
	//Vehicle SpawnVehicle(Hash hash, Vector3 coords, float heading, DWORD timeout);
	Vehicle SpawnVehicle(Hash hash, Vector3 coords, float heading);

	// Toggle for spawning into vehicle, defaults is off.
	bool spawnInsideVehicle = false;

	// Invincible vehicle toggles
	void EnableInvincibility();
	void DisableInvincibility();

	bool isInvincibleVehicleEnabled = false;
	// This will make the loop not constantly run
	bool invincibilityFlag = false;

	//
	void CreateMissionTrain(Hash model, Vector3 pos, bool direction);
	Vehicle missionTrain = 0;
	//


private:
#ifdef VEHICLE_SCRIPTS_SINGLETON
	// TODO Re-enable
	VehicleScripts() {} // Private constructor to prevent external instantiation
	VehicleScripts(const VehicleScripts&) = delete;
	VehicleScripts& operator=(const VehicleScripts&) = delete;
#endif
};

