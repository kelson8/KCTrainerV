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

private:
	VehicleScripts() {} // Private constructor to prevent external instantiation
	VehicleScripts(const VehicleScripts&) = delete;
	VehicleScripts& operator=(const VehicleScripts&) = delete;
};

