#pragma once

#include "IScriptBuilder.h"

class PedScripts : public IScriptBuilder
{
public:

	// Instance of the PedScripts
	static PedScripts& GetInstance()
	{
		static PedScripts instance;
		return instance;
	}

	void Tick() override;

	// TODO Test some of these, most of these are untested.
	void CreatePed(int pedType, Hash modelHash, Vector3 position, float heading, bool isNetwork, bool bScriptHostPed);
	
	bool DoesPedExist(Ped ped);
	bool DoesVehicleExist(Vehicle vehicle);

	// Weapon
	void GivePedWeapon(Ped ped, Hash weaponHash, int ammoCount, bool isHidden, bool bForceInHand);
	void GiveWeaponComponent(Ped ped, Hash weaponHash, Hash componentHash);

	// Vehicle
	void SetPedIntoVehicle(Ped ped, Vehicle vehicle, int seatIndex);
	void WarpPedIntoVehicle(Ped ped, Vehicle vehicle, int seat);

	void EnterVehicle(Ped ped, Vehicle vehicle, int timeout, int seat, float speed, int flag, const char* overrideEntryClipsetName);
	void LeaveVehicle(Ped ped, Vehicle vehicle, int flags);

	void DriveToCoord(Ped ped, Vehicle vehicle, Vector3 pos, float speed, Any p6, Hash vehicleModel,
		int drivingMode, float stopRange, float straightLineDistance);

	void DriveToCoordLongRange(Ped ped, Vehicle vehicle, Vector3 pos, float speed, int driveMode, float stopRange);
	void DriveWander(Ped ped, Vehicle vehicle, float speed, int drivingStyle);

	void ParkVehicle(Ped ped, Vehicle vehicle, Vector3 pos, float heading, int mode, float radius, bool keepEngineOn);
	void SetDrivingStyle(Ped ped, int drivingStyle);

	void SetCruiseSpeed(Ped driver, float cruiseSpeed);
	void SetMaxCruiseSpeed(Ped ped, float speed);
	//

#ifdef MEMORY_TESTING
	// Peds
	// Attacking player and other stuff

	void SetAllPedsAsCops();
	void PedsAttackPlayer();
	bool isPedsAttackEnabled = false;

	void MakeAllPedsDriveCrazy();
	bool isCrazyPedDrivingEnabled = false;

	// Weapons
	void GivePedRandomWeapons(Ped ped);
	void GiveAllPedsRandomWeapons();
#endif

private:
	PedScripts() {} // Private constructor to prevent external instantiation
	PedScripts(const PedScripts&) = delete;
	PedScripts& operator=(const PedScripts&) = delete;
};
