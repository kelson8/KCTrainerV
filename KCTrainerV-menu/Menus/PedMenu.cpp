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


PedMenu::PedMenu() :
    m_pedToSpawn(0),
    b_isPedAnEnemy(false)
{

}

void PedMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
	auto& pedScripts = PedScripts::GetInstance();
    auto& playerTeleportScripts = Scripts::Player::Positions::GetInstance();
    auto& playerScripts = PlayerScripts::GetInstance();

    int nothing = 0;

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
    mbCtx.StringArray("--Spawning--", { "" }, nothing);

    // TODO make this accept a set of target coords from the player.
    if (mbCtx.Option("Create heli peds", { "Create a pilot ped in a helicopter to fly around." }))
    {
        Hash heliPilotHash = "s_m_m_pilot_01"_hash;
        Vector3 heliTargetPos = Vector3(224.516, -213.628, 70.052);
        Vector3 spawnPos = playerTeleportScripts.GetPlayerCoords();
        pedScripts.CreateHelicopterPed(PED_TYPE_CIVMALE, heliPilotHash, spawnPos, heliTargetPos, 20.0f);
        UI::Notify("Helicopter and pilot spawned, flying to middle of map.");
    }

    if (mbCtx.Option("Remove heli peds", { "Remove the pilot ped and blow up the helicopter." }))
    {
        pedScripts.RemoveHeliPed();
        pedScripts.BlowupHelicopter();
    }
#endif


    // Well now, I didn't know the StringArray could work like this, I always just used it for separators.
    std::vector<std::string> pedList = { "Lost member #1", "Lost member #2" };

    // This works!
    // TODO Come up with a solution for this instead of using numbers for the case statements.
    // Possibly make it use a ped list?
    mbCtx.StringArray("Ped List", pedList, m_pedToSpawn, {"Select a ped to spawn"});

    // Enemy toggle
    mbCtx.BoolOption("Enemy ped", b_isPedAnEnemy);
    
    // Select ped button
    if (mbCtx.Option("Select ped"))
    {
        Vector3 currentPos = playerTeleportScripts.GetPlayerCoords();
        // Make this offset just a bit
        Vector3 newPos = Vector3(currentPos.x + 5, currentPos.y + 5, currentPos.z);

        //switch (iteratorTest)
        switch (m_pedToSpawn)
        {
        case 0:
            pedScripts.CreatePed(PED_TYPE_CIVMALE, PedHash::Lost01GMY, newPos, 30.0f, b_isPedAnEnemy);
            break;
        case 1:
            pedScripts.CreatePed(PED_TYPE_CIVMALE, PedHash::Lost02GMY, newPos, 30.0f, b_isPedAnEnemy);
            break;
        }

        /*log_output(std::format("Ped {} spawned", iteratorTest));*/
    }

#endif // MEMORY_TESTING

    // TODO Look into creating a ped, 
    // TODO Make a model loader function, possibly make a Functions folder, add ModelFunctions or something and make it there.
}
