#pragma once
class VehicleScripts
{
public:

	// Instance of the VehicleScripts
	static VehicleScripts& GetInstance()
	{
		static VehicleScripts instance;
		return instance;
	}

	// Request vehicle model
	//void RequestModel(Hash model);

	void ChangePlayerVehicleColor();
	static void ToggleBombBayDoors();

	Vehicle GetPlayerVehicle();

	// Vehicle mods
	void SetArmor(int value);
	int vehicleArmorLevel = 0;
	//

	// Repair vehicle
	void RepairVehicle();
	void RepairTires();

	// Bullet proof
	void EnableBulletProof();
	void DisableBulletProof();
	// Flags
	bool isBulletProofEnabled = false;
	bool bulletProofFlag = false;

	// Invincible vehicle toggles
	void EnableInvincibility();
	void DisableInvincibility();

	bool isInvincibleVehicleEnabled = false;
	// This will make the loop not constantly run
	bool invincibilityFlag = false;



	//
	//void CreateMissionTrain(Hash model, Vector3 pos, bool direction);
#ifdef TRAIN_TEST
	void CreateMissionTrain(Hash model, bool direction);
	Vehicle missionTrain = 0;
#endif
	//


	std::string GetVehicleName(Entity vehicle);

private:
	VehicleScripts() {} // Private constructor to prevent external instantiation
	VehicleScripts(const VehicleScripts&) = delete;
	VehicleScripts& operator=(const VehicleScripts&) = delete;
};
