#pragma once
#ifdef MEMORY_TESTING

// Taken from Chaos Mod

typedef int Entity;

namespace Hooks
{
	void ProxyEntityHandle(Entity origHandle, Entity newHandle);
}

#endif