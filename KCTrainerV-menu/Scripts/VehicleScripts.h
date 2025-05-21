#pragma once
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


	std::string GetVehicleName(Entity vehicle);

private:
#ifdef VEHICLE_SCRIPTS_SINGLETON
	// TODO Re-enable
	VehicleScripts() {} // Private constructor to prevent external instantiation
	VehicleScripts(const VehicleScripts&) = delete;
	VehicleScripts& operator=(const VehicleScripts&) = delete;
#endif
};
