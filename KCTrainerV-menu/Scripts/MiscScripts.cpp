#include "pch.h"

#include "Constants.hpp"

#include "MiscScripts.h"
#include "Scripts/PlayerScripts.h"
#include "Scripts/TextScripts.h"

#include <format>

#include "inc/types.h"

#include "Util/UI.hpp"

#include "Util/Util.hpp"

// Test for lua
#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

// Chaos Mod
// Breaks this in here? I'll just manually use GET_HASH_KEY then..
//#include "Util/Hash.h"

#include "Util/EntityIterator.h"
#include "Util/Random.h"

#ifdef EXTRA_FEATURES
#include "Memory/Physics.h"
#include "Memory/Snow.h"
#include "Memory/Entity.h"
#include "Memory/Handle.h"

#include "Util/Vehicle.h"
#endif //EXTRA_FEATURES

#include "Memory/Misc.h"

// Lua tests
#ifdef LUA_TEST
#include "../Components/LuaManager.h"

void MiscScripts::InitializeLuaMusic() 
{
	auto& luaManager = LuaManager::GetInstance();
	luaManager.load_script("scripts/music_config.lua");
}

void MiscScripts::PlayLuaMusic(const std::string& track_id) 
{
	auto& luaManager = LuaManager::GetInstance();
	sol::optional<sol::function> play_func = luaManager.get_function("play_music");
	if (play_func) {
		(*play_func)(track_id);
	}
	else {
		// Handle error: play_music function not found
	}
}


#endif //LUA_TEST


// Begin adapted from pun_idgun FiveM

Entity MiscScripts::GetEntityIsAimingAt(Ped ped)
{
	// Blank value for function
	Entity entity;
	Entity isAimingAt = GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(ped, &entity);
	//return isAimingAt;
	return entity;
}

/// <summary>
/// TODO Finalize some changes with this, from pun_idgun in FiveM
/// This seems to mostly work other then getting the ped names and vehicle names.
/// Shows these values:
/// Entity ID
/// Entity X, Y, Z
/// Entity Heading
/// Entity Hash (Model name hash)
/// Adapted from here: https://forum.cfx.re/t/id-gun-find-out-object-coords-headings-and-hashes/984257
/// </summary>
void MiscScripts::IdGun()
{
	auto& textScripts = TextScripts::GetInstance();
	
	auto& playerScripts = PlayerScripts::GetInstance();

	Util util = Util();

	//Ped playerPed = playerScripts.GetPlayerPed();
	// I think this fixed it, I think I needed the playerID
	Player playerPed = PLAYER_ID();

	if (IS_PLAYER_FREE_AIMING(playerPed))
	{
		// Aimed at entity
		Entity entity = this->GetEntityIsAimingAt(playerPed);
		Vector3 entityCoords = GET_ENTITY_COORDS(entity, false);
		float entityHeading = GET_ENTITY_HEADING(entity);
		Hash entityHash = GET_ENTITY_MODEL(entity);

		// Entity ID
		std::string entityIdString = std::format("Entity ID: {}", std::to_string(entity));
		textScripts.SetTextEntry(entityIdString.c_str());

		// TODO Remove, debug lines
		//std::cout << entityString << std::endl;
		//

		textScripts.TextPosition(entityIdMenuPosX, entityIdMenuPosY);

		// Entity coords
		//std::string entityCoordsString = std::format("Entity Coords: X: {} Y: {} Z: {}",
		//	std::to_string(entityCoords.x), std::to_string(entityCoords.y), std::to_string(entityCoords.z));
		//textScripts.SetTextEntry(entityCoordsString.c_str());

		std::string entityCoordsString = std::format("Entity Coords: X: {:.2f} Y: {:.2f} Z: {:.2f}",
			entityCoords.x, entityCoords.y, entityCoords.z);
		textScripts.SetTextEntry(entityCoordsString.c_str());

		textScripts.TextPosition(entityCoordsMenuPosX, entityCoordsMenuPosY);

		// Entity heading
		std::string entityHeadingString = std::format("Entity Heading: {}" ,std::to_string(entityHeading));
		textScripts.SetTextEntry(entityHeadingString.c_str());

		textScripts.TextPosition(entityHeadingMenuPosX, entityHeadingMenuPosY);

		// If the entity is a vehicle, show the name of the vehicle instead of the entity model hash.
		// Well this doesn't seem to work.
//#define DISPLAY_VEHICLE_NAME
#ifdef DISPLAY_VEHICLE_NAME
		if (IS_ENTITY_A_VEHICLE(entity))
		{
			//Vehicle vehicle = GET_VEHICLE_PED_IS_IN(entity, false);
			// This didn't work either.
			//std::string vehicleName = util.GetGxtName(vehicle);
			std::string vehicleName = util.GetGxtName(entity);
			//Hmm, just says 'CARNOTFOUND'?
			//std::string vehicleName = GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehicle);

			std::string vehicleNameString = std::format("Vehicle Name: {}", vehicleName);
			textScripts.SetTextEntry(vehicleNameString.c_str());
			textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);
		}
		 //Show the entity model hash
		else 

		{
#endif
			 //Entity Model
			std::string entityModelString = std::format("Entity Model Hash: {}", std::to_string(entityHash));
			textScripts.SetTextEntry(entityModelString.c_str(), 255, 255, 255, 255);
			textScripts.TextPosition(entityModelMenuPosX, entityModelMenuPosY);

			// TODO Add door status if this is a door, I would probably have to check if this is a door model from a list.

#ifdef DISPLAY_VEHICLE_NAME
		}
#endif

		/*if (IS_ENTITY_A_PED(entity))
		{*/


			//Vehicle vehicle = GET_VEHICLE_PED_IS_IN(entity, false);
			//Hash vehicleModel = GET_ENTITY_MODEL(vehicle); // Get the hash of the vehicle

			//if (DOES_ENTITY_EXIST(vehicle))
			//{
				// TODO Replicate this
				/*
					if hashes[tostring(model_veh)] then
						modelVehText = hashes[tostring(model_veh)]         -- Set the modelText local with the actual name.
                   else
						modelVehText = model_veh                           -- Set the modelText local.
					end

				*/
			//}

		//}

	}
}

//void DrawInfos()
//{
//	/*
//	    local args = {...}
//    for k,v in pairs(args) do
//	*/
//	float ypos = 0.70;
//
//	SET_TEXT_COLOUR(255, 255, 255, 255);
//	SET_TEXT_FONT(0);
//	SET_TEXT_SCALE(0.4, 0.4);
//	SET_TEXT_WRAP(0.0, 1.0);
//	SET_TEXT_CENTRE(false);
//	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 255);
//	SET_TEXT_EDGE(50, 0, 0, 0, 255);
//	SET_TEXT_OUTLINE();
//	//SetTextEntry("STRING");
//	//AddTextComponentString(v);
//	//DRAW_TEXT(0.015, ypos);
//	ypos = ypos + 0.028;
//}

// End adapted from pun_idgun FiveM






/// <summary>
/// This works for playing sound tracks with the TRIGGER_MUSIC_EVENT native
/// Here is a list of these: https://github.com/DurtyFree/gta-v-data-dumps/blob/master/musicEventNames.json
/// These can be found in the decompiled scripts by searching for 'TRIGGER_MUSIC_EVENT'
/// </summary>
/// <param name="track"></param>
//void MiscScripts::PlayTestMusic(int track)
void MiscScripts::PlayTestMusic(MusicTracks track)
{

	// This method should work
	//switch (track) {
	//case 1:
	//	TRIGGER_MUSIC_EVENT("GTA_ONLINE_STOP_SCORE");
	//	break;
	//case 2:
	//	TRIGGER_MUSIC_EVENT("HEIST_CELEB_STRIP_CLUB");
	//	break;
	//}

	// Better to use std::map for a lot of these:
	static const std::map<int, std::string> musicTracks = 
	{
		{1, "GTA_ONLINE_STOP_SCORE"},
		{2, "HEIST_CELEB_STRIP_CLUB"},
		{3, "HEIST_CELEB_APARTMENT"},
		{4, "HEIST_STATS_SCREEN_STOP_PREP"},
		{5, "SHOP_TILL_YOU_DROP"},
		{6, "MP_LOBBY_AMBIENCE"},
		{7, "MICHAELS_HOUSE"},
		{8, "AC_START"}, //Altruist Cult
		{9, "AC_END"}, //Altruist Cult
		{10, "AC_DELIVERED"}, //Altruist Cult
		{11, "AC_DONE"}, //Altruist Cult
		{12, "RE51A_SHOP"}, //Shop robbery
		{13, "PEYOTE_TRIPS_START"}, //Something to do with Peyote plants
		{14, "PEYOTE_TRIPS_STOP"}, //Something to do with Peyote plants

		// Flight school, there are probably a few I missed, the fail ones probably don't do anything.
		{15, "FS_FORMATION_START"},
		{16, "FS_FORMATION_FAIL"},
		{17, "FS_LOOP_START"},
		{18, "FS_LOOP_FAIL"},
		{19, "FS_MOVING_LANDING_START"},
		{20, "FS_MOVING_LANDING_FAIL"},
		{21, "CHASE_PARACHUTE_START"},
		{22, "CHASE_PARACHUTE_DEPLOY"}, // Doesn't play anything
		{23, "SHOOTING_RANGE_START"},
		{24, "SHOOTING_RANGE_TIMED"},

		{25, "GROUND_LEVEL_START"}, // Seems to be the sound that plays when you get cops

		{26, "FS_OBSTACLE_START"}, // This one is another one that occurs when flying
		
		// Doesn't do anything
		/*
		* 	//{25, "CITY_LANDING_ENGINE"},
			//{26, "CITY_LANDING_FAIL"},
			//{28, "GROUND_LEVEL_FAIL"},
			//{30, "FS_OBSTACLE_FAIL"},
		*/
		
		// End flight school

	};

	auto it = musicTracks.find(track);
	if (it != musicTracks.end()) {
		TRIGGER_MUSIC_EVENT(it->second.c_str());
	}
	else {
		// Handle invalid track number
		UI::Notify("Track number invalid.");
	}

}

void MiscScripts::StopTestMusic()
{
	AUDIO::TRIGGER_MUSIC_EVENT("MP_MC_CMH_IAA_FINALE_START");
}

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

#ifdef EXTRA_FEATURES

#ifdef MEMORY_TESTING
void MiscScripts::EnableSky()
{
	Memory::SetSkyDisabled(false);
	isSkyDisabled = false;
}
#endif

#ifdef MEMORY_TESTING
void MiscScripts::DisableSky()
{
	Memory::SetSkyDisabled(true);
	isSkyDisabled = true;
}
#endif

#ifdef MEMORY_TESTING
void MiscScripts::EnableSnow()
{
	// TODO Implement extra shader from Chaos Mod.
	// Will require some more memory stuff and modifications.
	Memory::SetSnow(true);
	isSnowEnabled = true;
}
#endif

#ifdef MEMORY_TESTING
void MiscScripts::DisableSnow()
{

	Memory::SetSnow(false);
	isSnowEnabled = false;

}
#endif // MEMORY_TESTING

#endif //EXTRA_FEATURES

// Taken from PlayerForceField.cpp in Chaos Mod.
#ifdef EXTRA_FEATURES

#pragma region ForceFieldFunctions

#ifdef MEMORY_TESTING

void MiscScripts::EnableForceField()
{
	Ped player = PLAYER_PED_ID();
	std::vector<Entity> entities;

	for (Ped ped : GetAllPeds())
		if (ped != player)
			entities.push_back(ped);

	for (Vehicle veh : GetAllVehs())
		if (!IS_PED_IN_VEHICLE(player, veh, false))
			entities.push_back(veh);

	for (Entity prop : GetAllProps())
		entities.push_back(prop);

	Vector3 playerCoord = GET_ENTITY_COORDS(player, false);
	for (Entity entity : entities)
	{
		static const float startDistance = 15;
		static const float maxForceDistance = 10;
		static const float maxForce = 100;
		Vector3 entityCoord = GET_ENTITY_COORDS(entity, false);
		float distance = GET_DISTANCE_BETWEEN_COORDS(playerCoord, entityCoord, true);
		if (distance < startDistance)
		{
			if (IS_ENTITY_A_PED(entity) && !IS_PED_RAGDOLL(entity))
				SET_PED_TO_RAGDOLL(entity, 5000, 5000, 0, true, true, false);
			float forceDistance = std::min(std::max(0.f, (startDistance - distance)), maxForceDistance);
			float force = (forceDistance / maxForceDistance) * maxForce;
			Memory::ApplyForceToEntity(entity, 3, entityCoord.x - playerCoord.x, entityCoord.y - playerCoord.y,
				entityCoord.z - playerCoord.z, 0, 0, 0, false, false, true, true, false, true);
		}
	}
	//isForceFieldEnabled = true;

}

void MiscScripts::DisableForceField()
{
	isForceFieldEnabled = false;
}



#pragma endregion

#pragma region PedFunctions



#endif // MEMORY_TESTING

#ifdef CHAOSMOD_FEATURES
void MiscScripts::SetAllPedsInMowers()
{
	static const Hash mowerHash = MISC::GET_HASH_KEY("MOWER");
	SetSurroundingPedsInVehicles(mowerHash, 120);
}
#endif

#pragma endregion

#pragma region TVFunctions

// Tv channels
const char* TV_PLAYLISTS[] = {
	"PL_WEB_KFLF",    // Kung Fu Rainbow Lazerforce
	"PL_WEB_RANGERS", // Republican Space Rangers
	"PL_WEB_PRB2",    // Princess Robot Bubblegum
	"PL_WEB_FOS",     // Fame or Shame
	"PL_WEB_CAS",     // Diamond Casino DLC intro
	"PL_WEB_FOS",
	"PL_WEB_HOWITZER", // Howitzer Documentary
	"PL_WEB_KFLF",
	"PL_WEB_PRB2",
	"PL_WEB_RS",
	"PL_STD_CNT",
	"PL_STD_WZL",
	"PL_LO_CNT",
	"PL_LO_WZL",
	"PL_SP_WORKOUT",
	"PL_SP_INV",
	"PL_SP_INV_EXP",
	"PL_LO_RS",
	"PL_LO_RS_CUTSCENE",
	"PL_SP_PLSH1_INTRO",
	"PL_LES1_FAME_OR_SHAME",
	"PL_STD_WZL_FOS_EP2",
	"PL_MP_WEAZEL",
	"PL_MP_CCTV",
	"PL_CINEMA_CARTOON",
	"PL_CINEMA_ARTHOUSE",
	"PL_CINEMA_ACTION",
	"PL_CINEMA_MULTIPLAYER",
	"PL_CINEMA_MULTIPLAYER_NO_MELTDOWN",
};
//

// Tv options
float ms_PosX = 0.f;
float ms_PosY = 0.f;

/// <summary>
/// Enable the in game TV with a random position and enable the isTVRunning boolean.
/// </summary>
void MiscScripts::EnableTv()
{
	// First check if the tv is not running
	if (!isTVRunning)
	{
		auto playlist = TV_PLAYLISTS[g_Random.GetRandomInt(0, sizeof(TV_PLAYLISTS) / sizeof(TV_PLAYLISTS[0]) - 1)];

		GRAPHICS::SET_TV_CHANNEL_PLAYLIST_AT_HOUR(0, playlist, g_Random.GetRandomInt(0, 23));
		GRAPHICS::SET_TV_AUDIO_FRONTEND(true);
		GRAPHICS::SET_TV_VOLUME(1.0f); // 0.0 is actually the highest the player can normally tune to.
		GRAPHICS::ATTACH_TV_AUDIO_TO_ENTITY(PLAYER_PED_ID());
		GRAPHICS::SET_TV_CHANNEL(0);
		GRAPHICS::ENABLE_MOVIE_SUBTITLES(true);

		ms_PosX = g_Random.GetRandomFloat(0.3f, 0.7f);
		ms_PosY = g_Random.GetRandomFloat(0.3f, 0.7f);
		isTVRunning = true;
	}
}

/// <summary>
/// This gets run in the KCMenuScript main tick.
/// </summary>
void MiscScripts::TvTick()
{
	GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(4);
	GRAPHICS::SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU(1);
	//GRAPHICS::DRAW_TV_CHANNEL(ms_PosX, ms_PosY, 0.3f, 0.3f, 0.0f, 255, 255, 255, 255);
	GRAPHICS::DRAW_TV_CHANNEL(Vector2(ms_PosX, ms_PosY), Vector2(0.3f, 0.3f), 0.0f, 255, 255, 255, 255);
}

/// <summary>
/// Turn off the TV and disable the isTVRunning boolean.
/// </summary>
void MiscScripts::DisableTv()
{
	// First check if the tv is running
	if (isTVRunning)
	{
		GRAPHICS::SET_TV_CHANNEL(-1);
		GRAPHICS::SET_TV_CHANNEL_PLAYLIST(0, nullptr, false);
		GRAPHICS::ENABLE_MOVIE_SUBTITLES(false);
		isTVRunning = false;
	}
}

#pragma endregion

#endif //EXTRA_FEATURES
