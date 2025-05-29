#include "pch.h"

#include "GlobalsMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

namespace Misc
{
	/// <summary>
	/// Build the misc globals submenu.
	/// </summary>
	/// <param name="mbCtx"></param>
	/// <param name="context"></param>
	void GlobalsMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
	{
        mbCtx.Title("Globals");
        //-----
        // Globals testing
        //-----

        int minPhoneDisplayStatus = 0;
        // Umm oops, I had the max set to 0 also...
        int maxPhoneDisplayStatus = 10;
        int phoneStepValue = 1;

        int nothing = 0;
        mbCtx.StringArray("--Globals--", { "" }, nothing, { "Attempting to change the script globals here." });

        mbCtx.IntOption("Phone status", currentPhoneDisplayStatus, minPhoneDisplayStatus, maxPhoneDisplayStatus, phoneStepValue);

        // I think this works for putting the phone away.
        // Setting this to 7 seems to disable the phone.
        if (mbCtx.Option("Change Cellphone.phoneStatus", { "Change cell phone globals." }))
        {
            // TODO Figure out how to do this, should only set this if the version is 1.70?
            //if (getGameVersion() >= eGameVersion::VER_1_0_3351_0)
            //{

            //}

            // Global_21066.f_1 = Cellphone.phoneStatus
            int* phoneStatusPtr = (int*)getGlobalPtr(21066 + 1);
            if (phoneStatusPtr)
            {
                *phoneStatusPtr = currentPhoneDisplayStatus;
                log_output("Cellphone status set to: " + std::to_string(currentPhoneDisplayStatus));
            }
        }

        // I don't think this did anything
        // Chop killed status
        //mbCtx.IntOption("Chop killed", currentChopKilledStatus, 0, 1, 1);
        //mbCtx.BoolOption("Chop killed", currentChopKilledStatus);
        //if (mbCtx.Option("Change chop killed status", { "Test for setting chop killed global." }))
        //{
        //    //int* chopStatusPtr = (int*)getGlobalPtr(33233);
        //    bool* chopStatusPtr = (bool*)getGlobalPtr(33233);
        //    if (chopStatusPtr)
        //    {
        //        *chopStatusPtr = currentChopKilledStatus;
        //        log_output("Chop killed status set to: " + std::to_string(currentChopKilledStatus));
        //    }
        //}

        // This should possibly launch the camera?
        // Well this did nothing
        //if (mbCtx.Option("Bit set test"))
        //{
        //    int address = 4546023;
        //    SET_BIT(&address, 15);
        //}

        // Test for toggling the restricted area in the military base with globals.
        // Well this didn't work
        // 

        //mbCtx.IntOption("Military Base Status", currentMilBaseStatus, 0, 1, 1);
        //mbCtx.BoolOption("Military Base Status", currentMilBaseStatus);

        //// Global_33095 = DisableArmyBase
        ////// TODO Attempt to test this option in a loop
        //if (mbCtx.Option("Change DisableArmyBase", { "Change army base to/from restricted." }))
        //{
        //    //int* militaryBaseStatusPtr = (int*)getGlobalPtr(33095);
        //    bool* militaryBaseStatusPtr = (bool*)getGlobalPtr(33095);
        //    if (militaryBaseStatusPtr)
        //    {
        //        *militaryBaseStatusPtr = currentMilBaseStatus;
        //        log_output("Army base status set to: " + std::to_string(currentMilBaseStatus));
        //        log_output(std::format("Global_33095 (immediately after setting): {}", std::to_string(*militaryBaseStatusPtr)));
        //    }
        //}


        //-----
        // Disabled log vehicle models
        //-----
        //std::vector<GTAmodel::Model> g_vehHashes;
        //std::vector<GTAmodel::Model> g_vehHashes_OPENWHEEL;
        //std::vector<GTAmodel::Model> g_vehHashes_SUPER;

        // TODO Test, replicated from Menyoo in ModelNames.cpp.
        // Well I cannot log these, but I think this is how Menyoo is displaying all vehicle names.
        // They aren't lowercase in the vehicleBmp list.
        //if (mbCtx.Option("Log vehicle models"))
        //{
        //    g_vehHashes.clear();
        //    GTAmemory::GenerateVehicleModelList();
        //    auto& hashes = GTAmemory::VehicleModels();

        //    std::unordered_map<VehicleClass, std::vector<Model>*> vDestMap
        //    {
        //        { VehicleClass::Openwheel, &g_vehHashes_OPENWHEEL },
        //        { VehicleClass::Super, &g_vehHashes_SUPER}
        //    };

        //    const bool isMinGameVersion3095 = GTAmemory::GetGameVersion() >= eGameVersion::VER_1_0_3095_0;
        //    for (int d = 0x0; d < 0x20; d++)
        //    {
        //        for (auto& dd : hashes[d])
        //        {
        //            if (std::find(g_vehHashes.begin(), g_vehHashes.end(), Model(dd)) == g_vehHashes.end())
        //            {
        //                g_vehHashes.push_back(dd);
        //                log_output(dd);
        //            }
        //        }
        //    }

        //    

        //    //log_output(hashes);

        //}

        // Not sure how to use this yet.
        //if (mbCtx.Option("Reclass CVehicle test", { "Test for displaying the current vehicle name in memory" }))
        //{
        //    Ped playerPed = PLAYER::PLAYER_PED_ID();
        //    Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

        //    if (ENTITY::DOES_ENTITY_EXIST(currentVehicle))
        //    {
        //        //CVehicle* vehiclePtr = reinterpret_cast<CVehicle*>(GET_ENTITY_ADDRESS)

        //    }
        //}

            // TODO Figure out implementation for this, shouldn't be too hard.
            //if (mbCtx.Option("Test reload menu"), { "Test for reloading the menu config, may crash." })
            //{
            //    
            //}
	}
} // namespace Misc