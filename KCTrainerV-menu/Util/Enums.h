#pragma once

// Menyoo seems to have a lot of these in their enums.h file, I might just use that instead of making my own.
// There are a lot of these

enum MusicTracks
{
	GTA_ONLINE_STOP_SCORE = 1,
	HEIST_CELEB_STRIP_CLUB = 2,
	HEIST_CELEB_APARTMENT = 3,
	HEIST_STATS_SCREEN_STOP_PREP = 4,
	SHOP_TILL_YOU_DROP = 5,
	MP_LOBBY_AMBIENCE = 6,
	MICHAELS_HOUSE = 7,
	AC_START = 8, // Altruist Cult
	AC_END = 9,   // Altruist Cult
	AC_DELIVERED = 10, // Altruist Cult
	AC_DONE = 11,    // Altruist Cult
	RE51A_SHOP = 12, // Shop robbery
	PEYOTE_TRIPS_START = 13, // Something to do with Peyote plants
	PEYOTE_TRIPS_STOP = 14,  // Something to do with Peyote plants

	// Flight school, there are probably a few I missed, the fail ones probably don't do anything.
	FS_FORMATION_START = 15,
	FS_FORMATION_FAIL = 16,
	FS_LOOP_START = 17,
	FS_LOOP_FAIL = 18,
	FS_MOVING_LANDING_START = 19,
	FS_MOVING_LANDING_FAIL = 20,
	CHASE_PARACHUTE_START = 21,
	CHASE_PARACHUTE_DEPLOY = 22,
	SHOOTING_RANGE_START = 23,
	SHOOTING_RANGE_TIMED = 24,
	// New

	GROUND_LEVEL_START = 25,
	FS_OBSTACLE_START = 26,

	// These do nothing
	/*
	//CITY_LANDING_ENGINE,
	//CITY_LANDING_FAIL,
	//GROUND_LEVEL_FAIL,
	//FS_OBSTACLE_FAIL,
	*/

	MUSIC_TRACKS_COUNT // Sentinel value
};

// To get the maximum value (the last actual track's value):
constexpr int maxMusicTrackValue = static_cast<int>(MusicTracks::SHOOTING_RANGE_TIMED);

// To get the count of actual enum members:
constexpr int musicTracksCount = static_cast<int>(MusicTracks::MUSIC_TRACKS_COUNT) - 1; // Subtract 1 because the values start at 1
	

// https://nativedb.dotindustries.dev/gta5/natives/0x29439776AAA00A62?search=GET_VEHICLE_CLASS
// Use this with the native GET_VEHICLE_CLASS
enum VehicleClass 
{
	COMPACTS = 0,
	SEDANS = 1,
	SUVS = 2,
	COUPES = 3,
	MUSCLE = 4,
	SPORTS_CLASSICS = 5,
	SPORTS = 6,
	SUPER = 7,
	MOTORCYCLES = 8,
	OFF_ROAD = 9,
	INDUSTRIAL = 10,
	UTILITY = 11,
	VANS = 12,
	CYCLES = 13,
	BOATS = 14,
	HELICOPTERS = 15,
	PLANES = 16,
	SERVICE = 17,
	EMERGENCY = 18,
	MILITARY = 19,
	COMMERCIAL = 20,
	TRAINS = 21,
};

// https://nativedb.dotindustries.dev/gta5/natives/0xBF25EB89375A37AD?search=SET_RELATIONSHIP_BETWEEN_GROUPS
// These can be used with the native SET_RELATIONSHIP_BETWEEN_GROUPS
enum RelationShipGroup 
{
	COMPANION = 0,
	RESPECT = 1,
	LIKE = 2,
	NEUTRAL = 3,
	DISLIKE = 4,
	HATE = 5,
	PEDESTRAINS = 255, // or neutral
};