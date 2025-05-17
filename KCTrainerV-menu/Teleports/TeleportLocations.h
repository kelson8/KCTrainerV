#pragma once
// For checking if LOAD_IPLS is enabled, I still need to work on that feature.
#include "Constants.hpp"

#include <vector>
#include <string>
#include <string_view> // For std::string_view

#include <inc/natives.h>

// Taken from TeleLocation.h in Menyoo
constexpr std::string_view operator ""_sv(const char* str, std::size_t length)  //Maybe not the best location for this but every file that requiers this includes this header file so why not 
{
	return str;
}
//

// Forward declaration of the enum
enum TeleportLocation;

// Structure to hold a teleport location with a name, location, and heading
struct TeleportInfo
{
	std::string name;
	Vector3 coordinates;
	float heading;
#ifdef LOAD_IPLS
	// TODO Add later, for IPL loading.
	std::vector<std::string_view> iplsToLoad;
#endif //LOAD_IPLS
};



// TODO Setup these, should fix categories to work in here and be able to use the converted Menyoo items easier.:
#ifdef NEW_TELEPORTS
enum TeleportLocationCategory {
	AIRPORTS, // 0
	SAFEHOUSES, // 1
	OTHER_LOCATIONS // 2
};

enum TeleportLocationID {
	// Airports
	AIRPORT_RUNWAY = 0,
	LOS_SANTOS_INTL_TERMINAL = 1,
	
	MICHAELS_HOUSE = 0,
	FRANKLINS_HOUSE_NEW = 1,
	FRANKLINS_HOUSE_OLD = 2,
	TREVORS_HOUSE = 3,
	TREVORS_HOUSE2 = 4,
	TREVORS_OFFICE = 5
	// ... and so on
};

// Define your separate vectors in TeleportLocations.cpp
extern const std::vector<TeleportInfo> vAirportLocations;
extern const std::vector<TeleportInfo> vSafeHouseLocations;
extern const std::vector<TeleportInfo> vOtherLocations;

#else
// Enum for teleport locations
enum TeleportLocation
{
	AIRPORT_RUNWAY = 0,
	HOSPITAL_LS1_ROOFTOP = 1,

#ifdef LOAD_IPLS
	// Yacht for the mission when Micheal has to get Tracy.
	// I thought this was the MP_YACHT
	SP_YACHT1 = 2,
#endif
	RICHARDS_MAJESTIC_MOVIE_STUDIO = 3, // RICHARDS_MAJESTIC

	// TODO Renumber these
	//MOUNT_CHILLIAD = 2,

	//// Police stations
	//POLICE_STATION1 = 3,
	//POLICE_STATION2 = 4,
	//POLICE_STATION3 = 5,


};

#endif //NEW_TELEPORTS


class TeleportLocations
{
public:

	// Instance of the TeleportLocations
	static TeleportLocations& GetInstance()
	{
		static TeleportLocations instance;
		return instance;
	}

	// Function to get the map of teleport locations
#ifndef NEW_TELEPORTS
	const std::vector<TeleportInfo>& GetTeleportLocations();
#endif //!NEW_TELEPORTS

#ifdef NEW_TELEPORTS
	// Function to get a specific teleport location with a category and id.
	const TeleportInfo& GetTeleportLocationInfo(TeleportLocationCategory category, TeleportLocationID id);

	// Optional: Function to get the name of a teleport location
	// I might use this later.
	const std::string& GetTeleportLocationName(TeleportLocationCategory category, TeleportLocationID id);

#else

	// Function to get a specific teleport location by its enum value
	//const Vector3& GetTeleportLocationInfo(TeleportLocation location);
	const TeleportInfo& GetTeleportLocationInfo(TeleportLocation location);

	// Optional: Function to get the name of a teleport location
	// I might use this later.
	const std::string& GetTeleportLocationName(TeleportLocation location);
#endif //NEW_TELEPORTS

	void UnloadAllLoadedIpls(); // New function to unload all loaded IPLs

private:
	TeleportLocations() {} // Private constructor to prevent external instantiation
	TeleportLocations(const TeleportLocations&) = delete;
	TeleportLocations& operator=(const TeleportLocations&) = delete;
};

