#pragma once
#include <inc/natives.h>

class PedScripts
{
public:

	// Instance of the PedScripts
	static PedScripts& GetInstance()
	{
		static PedScripts instance;
		return instance;
	}

	void SetAllPedsAsCops();

private:
	PedScripts() {} // Private constructor to prevent external instantiation
	PedScripts(const PedScripts&) = delete;
	PedScripts& operator=(const PedScripts&) = delete;
};
