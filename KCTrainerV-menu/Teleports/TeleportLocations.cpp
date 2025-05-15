#include "TeleportLocations.h"

#include "Scripts/PlayerScripts.h"

#include "Util/UI.hpp"
#include "Util/Logger.hpp"

#include <inc/natives.h>
#include <format>

// From Menyoo, should make it to where I don't have to type the namespaces for the natives.
#include "Natives/natives2.h"

// Static (internal linkage) vector to store teleport locations
static const std::vector<TeleportInfo> teleportLocations = {
    {"Airport Runway", {-1336.0f, -3044.0f, 13.9f}, 320.0f},
    {"Hospital LS1 Rooftop" ,{331.20f, -1409.31f, 77.53f}, 46.10f},
};

/// <summary>
/// Get a list of teleport locations
/// </summary>
/// <returns></returns>
const std::vector<TeleportInfo>& TeleportLocations::GetTeleportLocations() {
    return teleportLocations;
}

/// <summary>
/// Get the teleport location coordinates
/// </summary>
/// <param name="location">The location to check for from the TeleportLocation enum.</param>
/// <returns></returns>
//const Vector3& TeleportLocations::GetTeleportLocationInfo(TeleportLocation location) {
const TeleportInfo& TeleportLocations::GetTeleportLocationInfo(TeleportLocation location) {
    if (location >= 0 && location < teleportLocations.size()) {
        //return teleportLocations[location].coordinates;
        return teleportLocations[location];
    }
    else {
        std::string errorMsg = std::format("Error: Invalid TeleportLocation enum value: {}", static_cast<int>(location));
        LOG(ERROR, errorMsg);
        // Return a default value or handle the error appropriately
        //static const Vector3 invalidCoords = { 0.0f, 0.0f, 0.0f };
        static const TeleportInfo invalidInfo = { "Invalid", {0.0f, 0.0f, 0.0f}, 0.0f };
        //return invalidCoords;
        return invalidInfo;
    }
}

/// <summary>
/// Get the teleport location name
/// </summary>
/// <param name="location">The location to check for from the TeleportLocation enum.</param>
/// <returns></returns>
const std::string& TeleportLocations::GetTeleportLocationName(TeleportLocation location) {
    if (location >= 0 && location < teleportLocations.size()) {
        return teleportLocations[location].name;
    }
    else {
        static const std::string invalidName = "Invalid Location";
        return invalidName;
    }
}
