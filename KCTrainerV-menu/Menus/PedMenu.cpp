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

    mbCtx.Title("Peds");

#ifdef MEMORY_TESTING
    if (mbCtx.Option("Set all as cops", { "Set all peds in the area to cops" }))
    {
        pedScripts.SetAllPedsAsCops();
    }

#endif // MEMORY_TESTING

    // TODO Look into creating a ped, 
    // TODO Make a model loader function, possibly make a Functions folder, add ModelFunctions or something and make it there.
}
