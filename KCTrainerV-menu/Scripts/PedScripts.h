#pragma once

#include "IScriptBuilder.h"
#include "Util/Enums.h"

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

	// TODO Test some of these.
	//void CreatePed(ePedType pedType, Hash modelHash, Vector3 position, float heading, bool isNetwork, bool bScriptHostPed);
	void CreatePed(ePedType pedType, Hash modelHash, Vector3 position, float heading, bool isEnemy);
	// This seems to work
	//void CreateHelicopterPed(ePedType pedType, Hash modelHash, Vector3 position, float heading);
	void CreateHelicopterPed(ePedType pedType, Hash modelHash, Vector3 spawnPosition, Vector3 targetPosition, float heading);

	void RemoveHeliPed();
	void BlowupHelicopter();

	void SetPedAsEnemy(Ped ped);

	
	bool DoesEntityExist(Entity entity);
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
	//PedScripts() {} // Private constructor to prevent external instantiation
	PedScripts();
	PedScripts(const PedScripts&) = delete;
	PedScripts& operator=(const PedScripts&) = delete;

	// This below seems to work, I still need to make some modifications to this.
	// I would like to spawn a ped, make them drive around and make the player warp into the passenger seat.
	Ped m_pedToSpawn = 0;
	Ped m_helicopterPed1 = 0;
	Ped m_helicopterPed2 = 0;

	Vehicle m_helicopterVehicle1 = 0;

	// Helicopter model
	//Hash m_buzzardModel = "buzzard2"_hash;
	Hash m_pedHash = 0;

	bool m_enemyIsDead;
};
