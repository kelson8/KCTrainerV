#include "TeleportLocations.h"
#include "IplLocations.h"

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
        // Format for these that don't require ipls:
        // String name
        // Vector3 coords
        // Float heading

        // To access these in other files:
        // First, include the TeleportLocations.h header.
        // Then access these like this: 'Teleports::Positions::vAirportLocations'

        // I can automatically build the menus like I do with the vehicles, whatever order they are
        // in here, is what order they get displayed with.

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

        // Some of these came from here:
        // https://gtaforums.com/topic/792877-list-of-over-100-coordinates-more-comming/
        const std::vector<TeleportInfo> vOtherLocations =
        {
            // Obtained from forum link
            // Put the player on top of an antenna.
            //{"Mount Chiliad", Vector3(450.75f, 5566.614f, 806.183f), 22.0f},
            // Put the player at a more safe height.
            {"Mount Chiliad", Vector3(465.29, 5561.18, 781.16f), 89.75f},
            {"IAA Roof", Vector3(134.08f, -637.85f, 262.851f)},
            {"FIB Roof",  Vector3(150.126, -754.591, 262.865)},

            // Taken from old teleportLocations below
            {"Hospital LS1 Rooftop", Vector3(331.20f, -1409.31f, 77.53f), 46.10f}, // No IPLs needed
            // SP_YACHT1 - IPLs seem to auto load on start, I'll leave them alone.
            { "Dignity Heist (Vespucci Beach)", Vector3(-2023.455f, -1038.181f, 8.0629f), 90.0f},
            {"Richard's Majestic Movie Studio", Vector3(-1038.258423, -494.781830, 36.322914), 89.88f},
            {"Train station #1", Vector3(247.965f, -1201.17f, 38.92f), 10.0f},
            {"Train station #2", Vector3(670.2056f, -685.7708f, 25.15311f), 10.0f},
        };


#pragma region ApartmentInteriors
        // Credit to Menyoo for the below locations.
        // Locations.cpp in Menyoo:
        const std::vector<TeleportInfo> vApartmentInteriors =
        {
            {"Low End Apartment", Vector3(260.532200f, -999.133900f, -99.008700f)},
            {"Mid Range Apartment", Vector3(343.850000f, -999.080000f, -99.197700f)},
            {"3 Alta Street Apt 10", Vector3(-262.460000f, -951.890000f, 75.830000f)},
            {"3 Alta Street Apt 57", Vector3(-280.740000f, -961.500000f, 91.110000f)},
            {"Weazel Plaza Apt 26", Vector3(-895.850000f, -433.900000f, 94.060000f)},
            {"Weazel Plaza Apt 70", Vector3(-909.054000f, -441.466000f, 120.205000f)},
            {"Weazel Plaza Apt 101", Vector3(-884.301000f, -454.515000f, 125.132000f)},
            {"Richard Majestic Apt 4", Vector3(-897.197000f, -369.246000f, 84.077900f)},
            {"Richard Majestic Apt 51", Vector3(-932.290000f, -385.880000f, 108.030000f)},
            {"Tinsel Towers Apt 29", Vector3(-575.305000f, 42.323300f, 92.223600f)},
            {"Tinsel Towers Apt 45", Vector3(-617.609000f, 63.024000f, 106.624000f)},
            {"Eclipse Towers Apt 5", Vector3(-795.040000f, 342.370000f, 206.220000f)},
            {"Eclipse Towers Apt 9", Vector3(-759.790000f, 315.710000f, 175.400000f)},
            {"Eclipse Towers Apt 31", Vector3(-797.095000f, 335.069000f, 158.599000f)},
            {"Eclipse Towers Apt 40", Vector3(-752.605000f, 320.821000f, 221.855000f)},
            {"4 Integrity Way Apt 30", Vector3(-37.410000f, -582.820000f, 88.710000f)},
            {"4 Integrity Way Apt 35", Vector3(-10.580000f, -581.260000f, 98.830000f)},
            {"Del Perro Heights Apt 7", Vector3(-1477.140000f, -538.750000f, 55.526400f)},
            {"Del Perro Heights Apt 20", Vector3(-1474.170000f, -528.124000f, 68.154100f)},
            {"4 Integrity Way Apt 28", Vector3(-14.796400f, -581.709000f, 79.430700f)},
            {"Del Perro Heights Apt 4", Vector3(-1468.140000f, -541.815000f, 73.444200f)},
            {"Richard Majestic Apt 2", Vector3(-915.811000f, -379.432000f, 113.675000f)},
            {"Tinsel Towers Apt 42", Vector3(-614.860000f, 40.678300f, 97.600100f)},
            {"Eclipse Towers Apt 3", Vector3(-773.407000f, 341.766000f, 211.397000f)},
            {"3655 Wild Oats Drive", Vector3(-172.983000f, 494.033000f, 137.654000f)},
            {"2044 North Conker Avenue", Vector3(340.941000f, 437.180000f, 149.390000f)},
            {"2045 North Conker Avenue", Vector3(373.023000f, 416.105000f, 145.701000f)},
            {"2862 Hillcrest Avenue", Vector3(-676.127000f, 588.612000f, 145.170000f)},
            {"2868 Hillcrest Avenue", Vector3(-763.107000f, 615.906000f, 144.140000f)},
            {"2874 Hillcrest Avenue", Vector3(-857.798000f, 682.563000f, 152.653000f)},
            {"3677 Whispymound Drive", Vector3(120.500000f, 549.952000f, 184.097000f)},
            {"2113 Mad Wayne Thunder", Vector3(-1288.000000f, 440.748000f, 97.694600f)},
        };
#pragma endregion

#ifdef NEW_LOAD_IPLS
#pragma region IplLocations
        // This works for ipl loading/unloading with my new functions
        // https://wiki.rage.mp/wiki/Interiors_and_Locations#Online_Apartments
        // Format for these:
        // String name
        // Vector3 coords
        // Float heading
        // Vector of ipls to load '{"vw_casino_main"}', also like this: 'Teleports::Ipls::vCasinoNightclubIpls' or blank '{}'.
        // Vector of ipls to unload '{"vw_casino_main"}', also like this: 'Teleports::Ipls::vCasinoNightclubIpls' or blank '{}'.
        const std::vector<TeleportIplInfo> vCasinoLocations =
        {
            {"Casino", Vector3(1100.000, 220.000, -50.000), 30.0f, {"vw_casino_main"}, {"vw_casino_main"}},
            {"Casino Garage", Vector3(1295.000, 230.000, -50.000), 30.0f, {"vw_casino_garage"}, {"vw_casino_garage"}},
            {"Casino Car Park", Vector3(1380.000, 200.000, -50.000), 30.0f, {"vw_casino_carpark"}, {"vw_casino_carpark"}},
            {"Penthouse", Vector3(976.636, 70.295, 115.164), 30.0f, {"vw_casino_penthouse"}, {"vw_casino_penthouse"}},
            // This somewhat works but displays a tv on the screen.
            {"Casino Nightclub", Vector3(1550.0, 250.0, -48.0), 30.0f, Teleports::Ipls::vCasinoNightclubIpls, 
            {Teleports::Ipls::vCasinoNightclubIpls}}
        };

#pragma endregion

#endif // NEW_LOAD_IPLS
    } // namespace Positions
} // namespace Teleports

// TODO Try to fix IPL support later, for now I just won't use them.
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