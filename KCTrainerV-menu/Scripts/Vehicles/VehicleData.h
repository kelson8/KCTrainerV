#pragma once

#include "Constants.hpp"

#include "Util/Enums.h"

#include <string>
#include <vector>
#include <map>
#include <functional> // For std::function if you decide to store functions here directly
#include <inc/types.h> // Assuming Hash is defined here (like "scramjet"_hash)

struct VehicleInfo
{
	std::string name;
	//Hash hash;
	VehicleHash hashEnum;
};

namespace VehicleData
{
	// Declare your vehicle lists as extern const std::vector<VehicleInfo>
	// This tells the compiler these lists exist, but are defined elsewhere (in VehicleData.cpp)
	extern const std::vector<VehicleInfo> g_superVehicles;

	extern const std::vector<VehicleInfo> g_sportsVehicles;
	extern const std::vector<VehicleInfo> g_sportsClassicsVehicles;
	
	extern const std::vector<VehicleInfo> g_suvVehicles;
}
