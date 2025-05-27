#include "pch.h"

#include "WorldTimeMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

namespace Menus {

    namespace World
    {
        /// <summary>
        /// Build the world time submenu.
        /// </summary>
        /// <param name="mbCtx"></param>
        /// <param name="context"></param>
        void TimeMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Time");

            mbCtx.IntOption("Hour", hourToSet, minHourToSet, maxHourToSet, 1, { "Set the hours for the in game clock." });
            mbCtx.IntOption("Minute", minuteToSet, minMinuteToSet, maxMinuteToSet, 1, { "Set the minutes for the in game clock." });
            mbCtx.IntOption("Second", secondToSet, minSecondToSet, maxSecondToSet, 1, { "Set the seconds for the in game clock." });

            if (mbCtx.Option("Set Time", { "Set the time to the above values." }))
            {
                SET_CLOCK_TIME(hourToSet, minuteToSet, secondToSet);
            }
        }
    }

} //namespace World
