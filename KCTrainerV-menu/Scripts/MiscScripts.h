#pragma once
#include <inc/natives.h>

class MiscScripts
{
public:

	// Instance of the MiscScripts
	static MiscScripts& GetInstance()
	{
		static MiscScripts instance;
		return instance;
	}

	void PlayArenaWarLobbyMusic();
	void StopArenaWarLobbyMusic();
	
	void StartCreditsMusic();
	void StopCreditsMusic();

	// Tests
	void StartAirstrikeTest();
	void StopAirstrikeTest();
	

	bool airStrikeRunning;

private:
	MiscScripts() {} // Private constructor to prevent external instantiation
	MiscScripts(const MiscScripts&) = delete;
	MiscScripts& operator=(const MiscScripts&) = delete;

	bool hasAirstrikeLoaded = false;
};
