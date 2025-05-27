#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"


namespace Menus
{
	namespace World
	{
		struct WeatherInfo
		{
			std::string name;
			std::string weatherName;
		};

		class WeatherMenu : public IMenuBuilder
		{
		public:
			// Instance of WeatherMenu
			static WeatherMenu& GetInstance()
			{
				static WeatherMenu instance;
				return instance;
			}

			void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

		private:
			WeatherMenu() {} // Private constructor to prevent external instantiation
			WeatherMenu(const WeatherMenu&) = delete;
			WeatherMenu& operator=(const WeatherMenu&) = delete;

			// These variables don't need to be accessed outside this class.
			static const std::vector<WeatherInfo> g_weatherList;
		};
	} // namespace World
} // namespace Menus

