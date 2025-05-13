#pragma once

//#include "Effects/EffectThreads.h"

// I don't think I need this file, I think it's something specific to Chaos mod.

// https://github.com/gta-chaos-mod/ChaosModV/blob/master/ChaosMod/Util/Script.h

#ifdef _TEST1

#include <inc/main.h>

using DWORD                           = unsigned long;

inline void *g_MainThread             = nullptr;
inline void *g_EffectDispatcherThread = nullptr;

inline void WAIT(DWORD timeMs)
{
	auto currentFiber = GetCurrentFiber();
	if (currentFiber == g_MainThread || currentFiber == g_EffectDispatcherThread)
		scriptWait(timeMs);
	else
	{
		//EffectThreads::PauseThisThread(timeMs);

		if (g_EffectDispatcherThread)
			SwitchToFiber(g_EffectDispatcherThread);
		else
			SwitchToFiber(g_MainThread);
	}
}

#endif // _TEST1