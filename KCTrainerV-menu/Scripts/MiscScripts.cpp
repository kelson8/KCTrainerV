#include "MiscScripts.h"

#include "inc/types.h"

// Chaos Mod
// Breaks this in here? I'll just manually use GET_HASH_KEY then..
//#include "Util/Hash.h"

#include "Util/EntityIterator.h"
#include "Util/Random.h"


// Menyoo
#include "Natives/natives2.h"

// Some of these below were taken from the Chaos Mod.


// Taken from MiscIntenseMusic.cpp in Chaos Mod

void MiscScripts::PlayArenaWarLobbyMusic()
{
	AUDIO::TRIGGER_MUSIC_EVENT("AW_LOBBY_MUSIC_START");
}
void MiscScripts::StopArenaWarLobbyMusic()
{
	AUDIO::TRIGGER_MUSIC_EVENT("MP_MC_CMH_IAA_FINALE_START");
}

// This didn't seem to play the end credits music.
// Taken from MiscRollCredits.cpp in Chaos Mod

void MiscScripts::StartCreditsMusic() 
{
    PLAY_END_CREDITS_MUSIC(true);
    SET_MOBILE_PHONE_RADIO_STATE(true);

    int song = g_Random.GetRandomInt(0, 2);
    if (song == 0)
        SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_SAVE_MICHAEL_TREVOR", 1);
    else if (song == 1)
        SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_KILL_MICHAEL", 1);
    else
        SET_CUSTOM_RADIO_TRACK_LIST("RADIO_16_SILVERLAKE", "END_CREDITS_KILL_TREVOR", 1);
}


void MiscScripts::StopCreditsMusic()
{
    PLAY_END_CREDITS_MUSIC(false);
    SET_MOBILE_PHONE_RADIO_STATE(false);

    SET_USER_RADIO_CONTROL_ENABLED(true);
}

// Taken from MiscAirStrike.cpp in Chaos Mod
static Vector3 getRandomOffsetCoord(Vector3 startCoord, float maxOffset)
{
    return Vector3::Init(startCoord.x + g_Random.GetRandomFloat(-maxOffset, maxOffset),
        startCoord.y + g_Random.GetRandomFloat(-maxOffset, maxOffset),
        startCoord.z + g_Random.GetRandomFloat(-maxOffset, maxOffset));
}



int lastAirStrike = 0;
Hash airstrikeHash;
void MiscScripts::StartAirstrikeTest()
{

	airstrikeHash = MISC::GET_HASH_KEY("WEAPON_AIRSTRIKE_ROCKET");
    //airstrikeHash = "WEAPON_AIRSTRIKE_ROCKET"_hash;
	// Don't want this loading multiple times.
	if (!hasAirstrikeLoaded)
	{
		REQUEST_WEAPON_ASSET(airstrikeHash, 31, 0);
		while (!HAS_WEAPON_ASSET_LOADED(airstrikeHash))
			WAIT(0);

		hasAirstrikeLoaded = true;
	}
    
    



	int current_time = GET_GAME_TIMER();
	if (current_time - lastAirStrike > 1000)
	//if (current_time - lastAirStrike > 100)
	{
		lastAirStrike = current_time;
		Ped player = PLAYER_PED_ID();
		Vector3 playerPos = GET_ENTITY_COORDS(player, false);
		Vector3 startPosition = getRandomOffsetCoord(playerPos, 10);
		Vector3 targetPosition = getRandomOffsetCoord(playerPos, 50);
		float groundZ = 0;
		//if (GET_GROUND_Z_FOR_3D_COORD(targetPosition.x, targetPosition.y, targetPosition.z, &groundZ, false, false))
		if (GET_GROUND_Z_FOR_3D_COORD(startPosition, &groundZ, false, false))
		{
			/*
			SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startPosition.x, startPosition.y, startPosition.z + 200,
			targetPosition.x, targetPosition.y, groundZ, 200, true, airstrikeHash, 0,
			true, false, 5000);
			*/
			SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Vector3(startPosition.x, startPosition.y, startPosition.z + 200),
				Vector3(targetPosition.x, targetPosition.y, groundZ), 200, true, airstrikeHash, 0,
				true, false, 5000);
		}
	}
}

void MiscScripts::StopAirstrikeTest()
{
	REMOVE_WEAPON_ASSET(airstrikeHash);
	hasAirstrikeLoaded = false;
}


