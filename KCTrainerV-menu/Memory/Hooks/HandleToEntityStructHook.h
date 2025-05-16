#pragma once

// Taken from Chaos Mod

typedef int Entity;

namespace Hooks
{
	void ProxyEntityHandle(Entity origHandle, Entity newHandle);
}