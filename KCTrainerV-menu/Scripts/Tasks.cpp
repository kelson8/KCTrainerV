#include "pch.h"
#include "Constants.hpp"

#include "Tasks.h"

#include "../Natives/natives2.h"

// TODO Set these up
/*
* Anim tasks
TASK_PLAY_ANIM_ADVANCED

* Streaming anim tasks
DOES_ANIM_DICT_EXIST
REQUEST_ANIM_DICT
HAS_ANIM_DICT_LOADED
REMOVE_ANIM_DICT

REQUEST_ANIM_SET
HAS_ANIM_SET_LOADED
REMOVE_ANIM_SET
*/

#pragma region AnimTasks

bool Tasks::IsPlayingAnimation(Entity entity, const char* animDict, const char* animName)
{
	// taskFlag seems to always be 3 in the scripts.
	if (IS_ENTITY_PLAYING_ANIM(entity, animDict, animName, 3)) 
	{
		return true;
	}

	return false;
}

/// <summary>
/// Play an animation on a ped/yourself
/// Some of the idea for this was taken from Tasks.cpp in Menyoo.
/// </summary>
/// <param name="ped"></param>
/// <param name="animDict"></param>
/// <param name="animationName"></param>
/// <param name="blendInSpeed"></param>
/// <param name="blendOutSpeed"></param>
/// <param name="duration"></param>
/// <param name="flag"></param>
/// <param name="playbackRate"></param>
/// <param name="lockX"></param>
/// <param name="lockY"></param>
/// <param name="lockZ"></param>
void Tasks::PlayAnimation(Ped ped, const char* animDict, const char* animationName, 
	float blendInSpeed, float blendOutSpeed, int duration, int flag, float playbackRate, bool lockX, bool lockY, bool lockZ)
{
	// First, request the Animation Dictionary
	REQUEST_ANIM_DICT(animDict);
	
	// Wait on it to load
	for (DWORD timeOut = GetTickCount() + 1650; GetTickCount() < timeOut;)
	{
		// If the anim has loaded, break out of this loop.
		if (HAS_ANIM_DICT_LOADED(animDict))
			break;
		WAIT(0);
	}

	TASK_PLAY_ANIM(ped, animDict, animationName, blendInSpeed, blendOutSpeed, duration, flag, playbackRate, lockX, lockY, lockZ);
}

void Tasks::StopAnimation(Entity entity, const char* animDict, const char* animationName, float blendDelta)
{
	// First, check if they are playing the animation.
	if (this->IsPlayingAnimation(entity, animDict, animationName))
	{
		STOP_ANIM_TASK(entity, animDict, animationName, blendDelta);
	}
	
}

#pragma endregion