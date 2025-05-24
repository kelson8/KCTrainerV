#pragma once
#include "IScriptBuilder.h"

class WorldScripts : public IScriptBuilder
{
public:
	// Instance of the WorldScripts
	static WorldScripts& GetInstance()
	{
		static WorldScripts instance;
		return instance;
	}

	void Tick() override;

	void KillAllPedsInArea();
	void BlowUpAllVehiclesInArea();

	// Respawn points
	void DisableHospitalSpawns();
	void SetRespawnLocation(Vector3 coords, float heading);

	// From Menyoo
	void DisableRestrictedAreas();

	void FireworksLoop();
	void StartFireworksAtCoord(const Vector3& pos, const Vector3& rot, float scale);
	void SetExplosionAtCoords(Vector3 pos, UINT8 type, float radius, float camshake, bool sound, bool invis);

	void SetPedsCalm();
	//

	void ToggleBlackout();

	bool isRestrictedAreasDisabled = false;
	bool areasDisabledFlag = false;

	bool isFireworksStarted = false;

	bool isPedsCalmActive = false;

	bool isBlackoutActive = false;
	bool blackoutFlag = false;

private:
	WorldScripts() {} // Private constructor to prevent external instantiation
	WorldScripts(const WorldScripts&) = delete;
	WorldScripts& operator=(const WorldScripts&) = delete;
};