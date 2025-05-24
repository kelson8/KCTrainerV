#include "TeleportLocations.h"

#include "Scripts/PlayerScripts.h"

#include "Util/UI.hpp"
#include "Util/Logger.hpp"

#include <inc/natives.h>
#include <format>

#include "Util/Enums.h"

// From Menyoo, should make it to where I don't have to type the namespaces for the natives.
#include "Natives/natives2.h"
#include "Scripts/Extras/Classes/GTAblip.h"
#include "Scripts/Extras/Classes/GTAvehicle.h"
#include "Scripts/Extras/Game.h"

namespace Teleports
{
    namespace Positions
    {
        // To access these in other files:
        // Look through the std::map, specify the name of the warp.
        //Teleports::Positions::vSafeHouseLocations.at("Michael's House")
        //Teleports::Positions::vAirportLocations.at("Airport Runway")


#ifndef NEW_TELEPORT_CATEGORIES
        std::map<std::string, Vector3> locations =
        {
            {"Micheals house", Vector3(-813.603f, 179.474f, 72.1548f)},

        };

        // Moved out of NEW_TELEPORTS preprocessor
        const std::map<std::string, Vector3> vAirportLocations = {
            {"Airport Runway", Vector3(-1336.0f, -3044.0f, 13.9f)},
            {"Los Santos Intl Terminal", Vector3(-1088.6f, -2979.5f, 13.9f)}
        };

        const std::map<std::string, Vector3> vSafeHouseLocations = {
            {"Michael's House", Vector3(-813.603f, 179.474f, 72.1548f)},
            //{"Franklin's House (New)", {7.11903f, 536.615f, 176.028f}, 0.0f, {"v_franklinshouse"_sv, "unlocked"_sv, "locked"_sv}},
            {"Franklin's House (New)", Vector3(7.11903f, 536.615f, 176.028f)},
            {"Franklin's House (Old)", Vector3(-14.3803f, -1438.51f, 31.1073f)},

            {"Trevor's House", Vector3(1972.61f, 3817.04f, 33.4278f)},
            {"Trevor's House 2", Vector3(-1151.77f, -1518.14f, 10.6327f)},
            {"Trevor's Office", Vector3(97.2707f, -1290.994f, 29.2688f)},
            // ... more safehouses
        };
#endif

#ifdef NEW_TELEPORT_CATEGORIES
        // TODO Fix the heading for below, these are just random.
        const std::vector<TeleportInfo> vAirportLocations = 
        {
            {"Airport Runway", Vector3(-1336.0f, -3044.0f, 13.9f), 22.0f},
            {"Los Santos Intl Terminal", Vector3(-1088.6f, -2979.5f, 13.9f), 22.0f}
        };

        const std::vector<TeleportInfo> vSafeHouseLocations =
        {
            {"Michael's House", Vector3(-813.603f, 179.474f, 72.1548f), 22.0f},
            //{"Franklin's House (New)", {7.11903f, 536.615f, 176.028f}, 0.0f, {"v_franklinshouse"_sv, "unlocked"_sv, "locked"_sv}},
            {"Franklin's House (New)", Vector3(7.11903f, 536.615f, 176.028f), 22.0f},
            {"Franklin's House (Old)", Vector3(-14.3803f, -1438.51f, 31.1073f), 22.0f},

            {"Trevor's House", Vector3(1972.61f, 3817.04f, 33.4278f), 22.0f},
            {"Trevor's House 2", Vector3(-1151.77f, -1518.14f, 10.6327f), 22.0f},
            {"Trevor's Office", Vector3(97.2707f, -1290.994f, 29.2688f), 22.0f},
        };

        
        // This format works! TODO Switch to it for all the others later.
        // I can automatically build the menus like I do with the vehicles, whatever order they are
        // in here, is what order they get displayed with.
        const std::vector<TeleportInfo> vTestLocations =
        {
            {"Mount Chilliad", Vector3(2.0f, 2.0f, 2.0f), 22.0f},
            {"Airport Runway", Vector3(-1336.0f, -3044.0f, 13.9f), 22.0f},
        };


#endif // NEW_TELEPORT_CATEGORIES
    }
}

#ifdef LOAD_IPLS

// Copied from Menyoo
const std::vector<std::string_view> vYacht_Smboat1
{
    { "smboat"_sv },
    { "smboat_lod"_sv }
};
const std::vector<std::string_view> vYacht_Heist1
{
    //{ "smboat_lod"_sv },
    { "hei_yacht_heist"_sv },
    { "hei_yacht_heist_enginrm"_sv },
    { "hei_yacht_heist_Lounge"_sv },
    { "hei_yacht_heist_Bridge"_sv },
    { "hei_yacht_heist_Bar"_sv },
    { "hei_yacht_heist_Bedrm"_sv },
    { "hei_yacht_heist_DistantLights"_sv },
    { "hei_yacht_heist_LODLights"_sv }
};

#endif // LOAD_IPLS

// TODO Setup this file to use this new format for some of this for category sorting:
#ifdef NEW_TELEPORTS
const std::vector<TeleportInfo> vAirportLocations = {
    {"Airport Runway", {-1336.0f, -3044.0f, 13.9f}, 320.0f, {}},
    {"Los Santos Intl Terminal", {-1088.6f, -2979.5f, 13.9f}, 180.0f, {}}
};

const std::vector<TeleportInfo> vSafeHouseLocations = {
    {"Michael's House", {-813.603f, 179.474f, 72.1548f}, 0.0f, {}},
    //{"Franklin's House (New)", {7.11903f, 536.615f, 176.028f}, 0.0f, {"v_franklinshouse"_sv, "unlocked"_sv, "locked"_sv}},
    {"Franklin's House (New)", {7.11903f, 536.615f, 176.028f}, 0.0f, {}},
    {"Franklin's House (Old)", {-14.3803f, -1438.51f, 31.1073f}, 0.0f, {}},
    // TODO Implement these, they will just need added to the TeleportLocations.h and the menu.
    {"Trevor's House", {1972.61f, 3817.04f, 33.4278f}, 0.0f, {}},
    {"Trevor's House 2", {-1151.77f, -1518.14f, 10.6327f}, 0.0f, {}},
    {"Trevor's Office", {97.2707f, -1290.994f, 29.2688f}, 0.0f, {}},
    // ... more safehouses
};


#pragma region safehouses
// TODO Make this work.
//const std::vector<TeleportInfo> vSafeHousesMenyoo =
//{
//    {"Franklin's House (New)", {7.11903f, 536.615f, 176.028f}, 0.0f, {"v_franklinshouse"_sv, "unlocked"_sv, "locked"_sv}},
//    {"Franklin's House (Old)", {-14.3803f, -1438.51f, 31.1073f}, 0.0f, {}},
//    {"Trevor's House", {1972.61f, 3817.04f, 33.4278f}, 0.0f, {}},
//    {"Trevor's House 2", {-1151.77f, -1518.14f, 10.6327f}, 0.0f, {}},
//    {"Trevor's Office", {97.2707f, -1290.994f, 29.2688f}, 0.0f, {}},
//};
#pragma endregion
//

const std::vector<TeleportInfo> vOtherLocations = {
    // ... other locations
};
#endif //NEW_TELEPORTS

#ifdef LOAD_IPLS

extern std::vector<std::string_view> g_loadedIpls; // Declaration (extern)

/// <summary>
/// Unload all currenty loaded IPLs that are in the global above
/// </summary>
void TeleportLocations::UnloadAllLoadedIpls() {
    for (const auto& ipl : g_loadedIpls) {
        //if (ENTITY::DOES_ENTITY_EXIST(GetPlayerPed())) {
            REMOVE_IPL(ipl.data());
#ifdef IPL_LOGGING
            LOG(INFO, std::format("Unloading IPL: {}", ipl.data()));
#endif //IPL_LOGGING
        //}
    }
    g_loadedIpls.clear();
}



#endif //LOAD_IPLS



// Static (internal linkage) vector to store teleport locations
// This is where new teleport locations get added for the menu.
static const std::vector<TeleportInfo> teleportLocations = {

#ifdef LOAD_IPLS
    // TODO Add later, for IPL loading.
    {"Airport Runway", {-1336.0f, -3044.0f, 13.9f}, 320.0f, {}}, // No IPLs needed for the runway
    {"Hospital LS1 Rooftop" ,{331.20f, -1409.31f, 77.53f}, 46.10f, {}}, // No IPLs needed
    //{ "Dignity Heist (Vespucci Beach)", {-2023.455f, -1038.181f, 8.0629f}, 90.0f, {vYacht_Heist1}}, // SP_YACHT1
    // SP_YACHT1 - IPLs seem to auto load on start, I'll leave them alone.
    { "Dignity Heist (Vespucci Beach)", {-2023.455f, -1038.181f, 8.0629f}, 90.0f, {}},
    {"Richard's Majestic Movie Studio", {-1038.258423, -494.781830, 36.322914}, 89.88f, {}},
    {"Train station #1", {247.965f, -1201.17f, 38.92f}, 10.0f, {}},
    {"Train station #2", {670.2056f, -685.7708f, 25.15311f}, 10.0f, {}},
    //{ "Dignity Party (Vespucci Beach)", {-2023.455f, -1038.181f, 8.0629f}, 90.0f, {vYacht_Smboat1} },
    // ... more locations
#else
    {"Airport Runway", {-1336.0f, -3044.0f, 13.9f}, 320.0f}, // No IPLs needed for the runway
    {"Hospital LS1 Rooftop" ,{331.20f, -1409.31f, 77.53f}, 46.10f}, // No IPLs needed
    //{ "Dignity Heist (Vespucci Beach)", {-2023.455f, -1038.181f, 8.0629f}, 90.0f, {vYacht_Heist1}}, // SP_YACHT1
    // SP_YACHT1 - IPLs seem to auto load on start, I'll leave them alone.
    { "Dignity Heist (Vespucci Beach)", {-2023.455f, -1038.181f, 8.0629f}, 90.0f},
    {"Richard's Majestic Movie Studio", {-1038.258423, -494.781830, 36.322914}, 89.88f},
    {"Train station #1", {247.965f, -1201.17f, 38.92f}, 10.0f},
    {"Train station #2", {670.2056f, -685.7708f, 25.15311f}, 10.0f},
    //{ "Dignity Party (Vespucci Beach)", {-2023.455f, -1038.181f, 8.0629f}, 90.0f, {vYacht_Smboat1} },
    // ... more locations
#endif // LOAD_IPLS
};

#ifndef NEW_TELEPORTS
/// <summary>
/// Get a list of teleport locations
/// </summary>
/// <returns></returns>
const std::vector<TeleportInfo>& TeleportLocations::GetTeleportLocations() {
    return teleportLocations;
}
#endif //!NEW_TELEPORTS



#ifdef NEW_TELEPORTS
/// <summary>
/// Get the teleport location coordinates
/// </summary>
/// <param name="location">The category to check for from the TeleportLocationCategory enum.</param>
/// <param name="id">The id to check for from the TeleportLocationID enum.</param>
/// <returns></returns>
const TeleportInfo& TeleportLocations::GetTeleportLocationInfo(TeleportLocationCategory category, TeleportLocationID id) {
    
    //LOG(INFO, std::format("GetTeleportLocationInfo: Category={}, ID={}", static_cast<int>(category), static_cast<int>(id)));
    switch (category) {
    case AIRPORTS:
        if (static_cast<size_t>(id) < vAirportLocations.size()) 
            return vAirportLocations[id];
        break;
    case SAFEHOUSES:
        //LOG(INFO, std::format("GetTeleportLocationInfo: vSafeHouseLocations.size() = {}", vSafeHouseLocations.size()));
        //LOG(INFO, std::format("GetTeleportLocationInfo: id (size_t) = {}", static_cast<size_t>(id)));
        if (static_cast<size_t>(id) < vSafeHouseLocations.size()) 
            return vSafeHouseLocations[id];
        break;
    case OTHER_LOCATIONS:
        if (static_cast<size_t>(id) < vOtherLocations.size()) 
            return vOtherLocations[id];
        break;
    }
    static const TeleportInfo invalidInfo = { "Invalid", {0.0f, 0.0f, 0.0f}, 0.0f, {} };
    LOG(ERROR, "Error: Invalid TeleportLocation Category or ID.");
    //std::cerr << "Error: Invalid TeleportLocation Category or ID." << std::endl;
    return invalidInfo;
}

#else

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
#ifdef LOAD_IPLS
        static const TeleportInfo invalidInfo = { "Invalid", {0.0f, 0.0f, 0.0f}, 0.0f, {} };
#else
        static const TeleportInfo invalidInfo = { "Invalid", {0.0f, 0.0f, 0.0f}, 0.0f };
#endif //LOAD_IPLS
        //return invalidCoords;
        return invalidInfo;
    }
}

#endif //NEW_TELEPORTS


#ifdef NEW_TELEPORTS
/// <summary>
/// Get the teleport location name
/// </summary>
/// <param name="category">The category from the TeleportLocationCategory enum.</param>
/// <param name="id">The id from the TeleportLocationID enum.</param>
/// <returns></returns>
const std::string& TeleportLocations::GetTeleportLocationName(TeleportLocationCategory category, TeleportLocationID id) {
    const auto& info = GetTeleportLocationInfo(category, id);
    return info.name;
}

#else

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

#endif // NEW_TELEPORTS

/// <summary>
/// Adapted from TeleMethods.cpp in Menyoo, original function: ToWaypoint
/// This seems to work, I set my warping functions to use this.
/// TODO Make this place the player on the ground and not crash, for some reason the Z ground check doesn't work.
/// </summary>
/// <param name="ped"></param>
void TeleportLocations::WarpToWaypoint(GTAped ped)
{
    float gtaGroundCheckHeight[] = {
    100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
    450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850.0
    };
    // This looks useful for checking if a waypoint is active and getting a proper Z value.
    // Hmm, I could use this in FiveM.
    // We don't need the namespace for these? I always had to use stuff like HUD::IS_WAYPOINT_ACTIVE()
    if (IS_WAYPOINT_ACTIVE())
    {
        Vector3 blipCoords = GTAblip(GET_FIRST_BLIP_INFO_ID(BlipIcon::Waypoint)).Position_get();

        GTAentity e = ped;
        if (ped.IsInVehicle())
            e = ped.CurrentVehicle();

        //GET_GROUND_Z_FOR_3D_COORD(blipCoords.x, blipCoords.y, 810.0, &blipCoords.z, 0, 0);
        GET_GROUND_Z_FOR_3D_COORD(Vector3(blipCoords.x, blipCoords.y, 810.0), &blipCoords.z, 0, 0);

        //Game::RequestControlOfId(e.NetID());
        e.RequestControl(1000);

        // Why was this an int?
        //for (int height : gtaGroundCheckHeight)
        for (float height : gtaGroundCheckHeight)
        {
            //SET_ENTITY_COORDS(e.Handle(), blipCoords.x, blipCoords.y, height, 0, 0, 0, 1);
            SET_ENTITY_COORDS(e.Handle(), Vector3(blipCoords.x, blipCoords.y, height), false, false, false, true);
            WAIT(100);
            //if (GET_GROUND_Z_FOR_3D_COORD(blipCoords.x, blipCoords.y, height, &blipCoords.z, 0, 0))
            if (GET_GROUND_Z_FOR_3D_COORD(Vector3(blipCoords.x, blipCoords.y, height), &blipCoords.z, false, false))
                break;
        }
        //SET_ENTITY_COORDS(e.Handle(), blipCoords.x, blipCoords.y, blipCoords.z, 0, 0, 0, 1);
        SET_ENTITY_COORDS(e.Handle(), Vector3(blipCoords.x, blipCoords.y, blipCoords.z), false, false, false, true);
    }
    else {
        Game::Print::PrintBottomCentre("~r~Error:~s~ No Waypoint set.");
    }
}