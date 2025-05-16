#pragma once

// Taken from Chaos Mod

//#include "Util/Events.h"

namespace rage
{
	class scrThread;
}

namespace Hooks
{
	//inline ChaosCancellableEvent<rage::scrThread *> OnScriptThreadRun;

	void EnableScriptThreadBlock();

	void DisableScriptThreadBlock();
}