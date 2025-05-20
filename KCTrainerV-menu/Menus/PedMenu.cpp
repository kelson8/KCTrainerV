#include "pch.h"

#include "../Constants.hpp"
#include "PedMenu.h"

// My scripts
#include "Scripts/PlayerScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/PedScripts.h"
#include "Scripts/MiscScripts.h"
#include "Scripts/TextScripts.h"

#include "Util/FileFunctions.h"


void PedMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
	auto& pedScripts = PedScripts::GetInstance();
	auto& miscScripts = MiscScripts::GetInstance();

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


#endif // MEMORY_TESTING

    // TODO Look into creating a ped, 
    // TODO Make a model loader function, possibly make a Functions folder, add ModelFunctions or something and make it there.
}
