#pragma once
#include <inc/natives.h>

class VehicleScripts
{
public:

	// Instance of the VehicleScripts
	static VehicleScripts& GetInstance()
	{
		static VehicleScripts instance;
		return instance;
	}

	void ChangePlayerVehicleColor();
	static void ToggleBombBayDoors();

	Vehicle GetPlayerVehicle();

	void RepairVehicle();

	// Spawning
	void SpawnVehicle(Hash hash);
	Vehicle SpawnVehicle(Hash hash, Vector3 coords, float heading, DWORD timeout);

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
	VehicleScripts() {} // Private constructor to prevent external instantiation
	VehicleScripts(const VehicleScripts&) = delete;
	VehicleScripts& operator=(const VehicleScripts&) = delete;
};

