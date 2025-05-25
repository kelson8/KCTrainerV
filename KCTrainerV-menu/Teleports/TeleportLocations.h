#pragma once
// For checking if LOAD_IPLS is enabled, I still need to work on that feature.
#include "Constants.hpp"

#include <vector>
#include <string>
#include <string_view> // For std::string_view
#include <map>


#include <inc/natives.h>
#include <inc/types.h>

#include "Scripts/Extras/Classes/GTAped.h"

// Taken from TeleLocation.h in Menyoo
constexpr std::string_view operator ""_sv(const char* str, std::size_t length)  //Maybe not the best location for this but every file that requiers this includes this header file so why not 
{
	return str;
}
//

/// <summary>
/// This takes these values
/// 
/// String for teleport name
/// Vector3 for teleport coordinates
/// float for heading
/// </summary>
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

#ifdef NEW_LOAD_IPLS
/// <summary>
/// Different struct for loading IPLs, I'll separate this so I don't effect the rest of my teleport system.
/// </summary>
struct TeleportIplInfo
{
	std::string name;
	Vector3 coordinates;
	float heading;
	//std::vector<std::string_view> iplsToLoad;
	std::vector<std::string> iplsToLoad;
	std::vector<std::string> iplsToUnload;
};
#endif // NEW_LOAD_IPLS

namespace Teleports 
{
	namespace Positions
	{

		// New teleport formats below
		extern const std::vector<TeleportInfo> vAirportLocations;
		extern const std::vector<TeleportInfo> vSafeHouseLocations;
		extern const std::vector<TeleportInfo> vOtherLocations;

		// New from Menyoo
		extern const std::vector<TeleportInfo> vApartmentInteriors;

		// New
		extern const std::vector<TeleportIplInfo> vCasinoLocations;
	}
}

class TeleportLocations
{
public:

	// Instance of the TeleportLocations
	static TeleportLocations& GetInstance()
	{
		static TeleportLocations instance;
		return instance;
	}

#ifdef LOAD_IPLS
	void UnloadAllLoadedIpls(); // New function to unload all loaded IPLs
#endif

	void WarpToWaypoint(GTAped ped);

private:
	TeleportLocations() {} // Private constructor to prevent external instantiation
	TeleportLocations(const TeleportLocations&) = delete;
	TeleportLocations& operator=(const TeleportLocations&) = delete;
};

