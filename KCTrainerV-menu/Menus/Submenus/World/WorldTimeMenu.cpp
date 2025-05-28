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

            int nothing = 0;

            mbCtx.StringArray("--Custom time values--", { "" }, nothing);

            mbCtx.IntOption("Hour", hourToSet, minHourToSet, maxHourToSet, 1, { "Set the hours for the in game clock." });
            mbCtx.IntOption("Minute", minuteToSet, minMinuteToSet, maxMinuteToSet, 1, { "Set the minutes for the in game clock." });
            mbCtx.IntOption("Second", secondToSet, minSecondToSet, maxSecondToSet, 1, { "Set the seconds for the in game clock." });

            if (mbCtx.Option("Set Time", { "Set the time to the above values." }))
            {
                SET_CLOCK_TIME(hourToSet, minuteToSet, secondToSet);
            }

            
            mbCtx.StringArray("--Specific times--", { "" }, nothing);

            //-----
            // List of time options
            //-----
            if (mbCtx.Option("Morning"))
            {
                // 6:00
                SET_CLOCK_TIME(6, 0, 0);
            }

            if (mbCtx.Option("Afternoon"))
            {
                // 12:00
                SET_CLOCK_TIME(12, 0, 0);
            }

            if (mbCtx.Option("Evening"))
            {
                // 16:00
                SET_CLOCK_TIME(16, 0, 0);
            }

            if (mbCtx.Option("Night"))
            {
                // 22:00
                SET_CLOCK_TIME(22, 0, 0);
            }
        }
    }

} //namespace World
