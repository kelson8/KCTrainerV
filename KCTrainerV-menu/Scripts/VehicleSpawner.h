#pragma once
//#include <inc/natives.h>
//#include <inc/types.h>

#include "../Constants.hpp"

class VehicleSpawner
{
public:
	static VehicleSpawner& GetInstance()
	{
		static VehicleSpawner instance;
		return instance;
	}

	enum class VehicleSpawnState {
		Idle,
		RequestingModel,
		CreatingVehicle,
		SpawnComplete,
	};

	// Spawning vehicle
	//void SpawnVehicle(Hash hash);
	void SpawnVehicle(Hash hash, bool clearArea);
	Vehicle SpawnVehicle(Hash hash, Vector3 coords, float heading);

	// Spawn player into jet in the air
	void SpawnJetInAir();

	// Toggle for spawning into vehicle, defaults is off.
	bool spawnInsideVehicle = false;

private:
	VehicleSpawner();
	VehicleSpawner(const VehicleSpawner&) = delete;
	VehicleSpawner& operator=(const VehicleSpawner&) = delete;

	bool vehicleSpawned = false;

	Vehicle m_vehicleToSpawn;
	Vehicle m_jetToSpawn;
};
