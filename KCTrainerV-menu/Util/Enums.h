#pragma once

// Menyoo seems to have a lot of these in their enums.h file, I might just use that instead of making my own.
// There are a lot of these

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