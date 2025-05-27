#include "pch.h"

#include "../Constants.hpp"
#include "PedMenu.h"

// My scripts
#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"
#include "Scripts/MiscScripts.h"
#include "Scripts/TextScripts.h"

// Player
#include "Scripts/PlayerScripts.h"
#include "Scripts/Player/PlayerTeleportScripts.h"

#include "Util/FileFunctions.h"

#include "Util/UI.hpp"


void PedMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
	auto& pedScripts = PedScripts::GetInstance();
    auto& playerTeleportScripts = Scripts::Player::Positions::GetInstance();

    mbCtx.Title("Peds");

#ifdef MEMORY_TESTING
    if (mbCtx.Option("Set all as cops", { "Set all peds in the area to cops" }))
    {
        pedScripts.SetAllPedsAsCops();
    }

    mbCtx.BoolOption("Peds attack player", pedScripts.isPedsAttackEnabled, { "Make all peds in the area attack you" });

    // Well this one crashes it.
    //if (mbCtx.Option("Give all peds weapons", { "Give all peds in the area random weapons" }))
    //{
    //    pedScripts.GiveAllPedsRandomWeapons();
    //}

    //mbCtx.BoolOption("Peds drive crazy", miscScripts.isCrazyPedDrivingEnabled, {
    mbCtx.BoolOption("Peds drive crazy", pedScripts.isCrazyPedDrivingEnabled, {
        "Make all peds in the area drive with the rushed driving style." });

    // This needs some testing, once I verify everything works I'll remove the DEBUG_MODE preprocessor here.
#ifdef DEBUG_MODE

    // TODO make this accept a set of target coords from the player.
    // Also, make the function take target coords along with the player coords.
    // Currently the value is hard-coded to go to the middle of the map.
    if (mbCtx.Option("Create heli peds", { "Create a pilot ped in a helicopter to fly around." }))
    {
        Hash heliPilotHash = "s_m_m_pilot_01"_hash;
        pedScripts.CreateHelicopterPed(PED_TYPE_CIVMALE, heliPilotHash, playerTeleportScripts.GetPlayerCoords(), 20.0f);
        UI::Notify("Helicopter and pilot spawned, flying to middle of map.");
    }

    if (mbCtx.Option("Remove heli peds", { "Remove the pilot ped and blow up the helicopter." }))
    {
        pedScripts.RemoveHeliPed();
        pedScripts.BlowupHelicopter();
    }
#endif


#endif // MEMORY_TESTING

    // TODO Look into creating a ped, 
    // TODO Make a model loader function, possibly make a Functions folder, add ModelFunctions or something and make it there.
}
