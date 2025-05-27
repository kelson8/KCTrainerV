#include "pch.h"

#include "MiscBlipsMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

// Menyoo
#include "Scripts/Extras/Game.h"
#include "Scripts/Extras/Classes/GTAblip.h"
#include "Scripts/Extras/World.h"
#include "Scripts/Extras/Classes/Model.h"
#include "Memory/GTAmemory.h"

//#define BLIP_TEST

// This doesn't work yet, I moved it out of Misc Menu though.

namespace Misc
{
	/// <summary>
	/// Build the misc blips submenu.
	/// </summary>
	/// <param name="mbCtx"></param>
	/// <param name="context"></param>
	void BlipsMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
	{
            mbCtx.Title("Blips");

            // Scripts
            auto& playerScripts = PlayerScripts::GetInstance();
            auto& vehicleScripts = VehicleScripts::GetInstance();
            auto& pedScripts = PedScripts::GetInstance();
            auto& textScripts = TextScripts::GetInstance();

            // Functions
            auto& fileFunctions = FileFunctions::GetInstance();

            // https://forum.cfx.re/t/help-how-do-i-add-blips-to-my-fivem-map-the-gta-v-map/84873/2

            // Hmm, I didn't know the World namespace had creating blips.
            //GTAblip blip = World::CreateBlip(Teleports::vSafeHouseLocations.at("Michael's House"));
            //GTAblip blip;

            // This seems to work
            // TODO Figure out how to make this a different icon.
            // For some reason it's only the yellow marker with the name 'destination'
#ifdef BLIP_TEST
    // TODO Fix these
            if (mbCtx.Option("Create blip"))
            {
                if (!blip.Exists())
                {
                    //blip = blip.Handle();

                    // Now in here it prints off the blip handle, but I cannot remove it from the function below.
                    // It just says the blip is 0 and doesn't exist.
                    blip = World::CreateBlip(Teleports::Positions::vSafeHouseLocations.at("Michael's House"));

                    blip.AddBlipForCoord(Teleports::Positions::vSafeHouseLocations.at("Michael's House"));

                    //blip.Position_set();
                    blip.SetBlipDisplay(blip, 3);
                    blip.SetScale(1.0f);
                    //blip.SetColour(5);

                    //blip.SetShortRange(true);
                    //blip.SetIcon(BlipIcon::AmmuNation);
                    // Premium Deluxe Motorsport icon
                    blip.SetIcon(BlipIcon::SportsCar);
                    blip.SetBlipName("Death point");

                    log_output(std::format("Added blip with handle: {}.", blip.Handle()));
                }
            }

            // TODO Figure out how to remove these blips
            // Well this says it gets removed but doesn't delete the blip.
            if (mbCtx.Option("Remove blip"))
            {
                if (blip.Exists())
                {
                    log_output(std::format("Removed blip with handle: {}", blip.Handle()));
                    blip.Remove();
                }
                else {
                    log_output(std::format("Blip handle {} is invalid or doesn't exist.", blip.Handle()));
                }
            }
#endif // BLIP_TEST
	}
} // namespace Misc