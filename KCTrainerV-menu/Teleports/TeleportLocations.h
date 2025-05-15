#pragma once


#include <vector>
#include <string>
#include <inc/natives.h>

// Forward declaration of the enum
enum TeleportLocation;

// Structure to hold a teleport location with a name, location, and heading
struct TeleportInfo
{
	std::string name;
	Vector3 coordinates;
	float heading;
};

// Enum for teleport locations
enum TeleportLocation
{
	AIRPORT_RUNWAY = 0,
	HOSPITAL_LS1_ROOFTOP = 1,
	MOUNT_CHILLIAD = 2,

	// Police stations
	POLICE_STATION1 = 3,
	POLICE_STATION2 = 4,
	POLICE_STATION3 = 5,
};

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
	const std::vector<TeleportInfo>& GetTeleportLocations();

	// Function to get a specific teleport location by its enum value
	//const Vector3& GetTeleportLocationInfo(TeleportLocation location);
	const TeleportInfo& GetTeleportLocationInfo(TeleportLocation location);

	// Optional: Function to get the name of a teleport location
	// I might use this later.
	const std::string& GetTeleportLocationName(TeleportLocation location);

private:
	TeleportLocations() {} // Private constructor to prevent external instantiation
	TeleportLocations(const TeleportLocations&) = delete;
	TeleportLocations& operator=(const TeleportLocations&) = delete;
};

