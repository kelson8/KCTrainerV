#pragma once

// Taken from Chaos Mod

#include "Constants.hpp"

#ifdef MEMORY_TESTING

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
#endif