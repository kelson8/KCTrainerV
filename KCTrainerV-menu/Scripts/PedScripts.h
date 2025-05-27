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
