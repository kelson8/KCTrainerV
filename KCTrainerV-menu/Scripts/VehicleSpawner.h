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

#ifdef VEHICLE_SPAWNER_TEST
	VehicleSpawner();
#endif // VEHICLE_SPAWNER_TEST

	// Spawning vehicle
	void SpawnVehicle(Hash hash);
	Vehicle SpawnVehicle(Hash hash, Vector3 coords, float heading);

	// Toggle for spawning into vehicle, defaults is off.
	bool spawnInsideVehicle = false;

	// TODO Test this

	void VehicleSpawnTick();

	void InitiateSpawn(const std::string& modelName, const Vector3& coords, float heading);


	// End new vehicle spawn testing

private:
	//VehicleSpawner() {} // Private constructor to prevent external instantiation
	//VehicleSpawner(const VehicleSpawner&) = delete;
	//VehicleSpawner& operator=(const VehicleSpawner&) = delete;

#ifdef VEHICLE_SPAWNER_TEST
	// Member variables to hold the state of the current spawn operation
	VehicleSpawnState m_currentSpawnState;
	Hash m_modelToSpawnHash;
	Vector3 m_spawnCoords;
	float m_spawnHeading;
	Vehicle m_lastSpawnedVehicle; // Stores the handle of the vehicle once spawned
#endif // VEHICLE_SPAWNER_TEST
};

#ifdef VEHICLE_SPAWNER_TEST
// Declare a global instance
extern VehicleSpawner g_vehicleSpawner;
#endif // VEHICLE_SPAWNER_TEST
