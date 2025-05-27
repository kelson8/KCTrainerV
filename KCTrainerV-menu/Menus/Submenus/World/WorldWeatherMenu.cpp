#include "pch.h"

#include "WorldWeatherMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

namespace Menus 
{
		
    namespace World
    {


        const std::vector<WeatherInfo> WeatherMenu::g_weatherList =
        {
            // { "Display Name", "NATIVE_WEATHER_STRING" }
            {"Blizzard", "BLIZZARD"},
            {"Clear", "CLEAR"},
            {"Clearing", "CLEARING"},
            {"Clouds", "CLOUDS"},
            {"Extra Sunny", "EXTRASUNNY"},
            {"Foggy", "FOGGY"},
            {"Halloween", "HALLOWEEN"},
            {"Neutral", "NEUTRAL"},
            {"Overcast", "OVERCAST"},
            {"Rain", "RAIN"},
            {"Rain Halloween", "RAIN_HALLOWEEN"},
            {"Smog", "SMOG"},
            {"Snow", "SNOW"},
            {"Snow Light", "SNOWLIGHT"},
            {"Snow Halloween", "SNOW_HALLOWEEN"},
            {"Thunder", "THUNDER"},
            // Disabled, this weather type does nothing
            //{"Unknown", "UNKNOWN"},
        };

#pragma endregion

        /// <summary>
        /// Build the world weather submenu.
        /// </summary>
        /// <param name="mbCtx"></param>
        /// <param name="context"></param>
        void WeatherMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Weather");

            for (const auto& weatherInfo : WeatherMenu::g_weatherList)
            {
                if (mbCtx.Option(weatherInfo.name))
                {
                    SET_WEATHER_TYPE_NOW(weatherInfo.weatherName.c_str());
                }
            }
        }

    } // namespace World

} //namespace Menus
