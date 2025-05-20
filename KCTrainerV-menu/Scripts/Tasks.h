#pragma once
#include "Util/Enums.h"

class Tasks
{
public:
	// Instance of the Tasks
	static Tasks& GetInstance()
	{
		static Tasks instance;
		return instance;
	}

	// Animations
	bool IsPlayingAnimation(Entity entity, const char* animDict, const char* animName);

	void PlayAnimation(Ped ped, const char* animDict, const char* animationName,
		float blendInSpeed, float blendOutSpeed, int duration, int flag, float playbackRate, bool lockX, bool lockY, bool lockZ);

	void StopAnimation(Entity entity, const char* animDict, const char* animationName, float blendDelta);

private:
	Tasks() {} // Private constructor to prevent external instantiation
	Tasks(const Tasks&) = delete;
	Tasks& operator=(const Tasks&) = delete;
};

