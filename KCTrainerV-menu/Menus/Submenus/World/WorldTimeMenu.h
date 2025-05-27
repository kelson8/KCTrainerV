#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"


namespace Menus
{
	namespace World
	{
		class TimeMenu : public IMenuBuilder
		{
		public:
			// Instance of TimeMenu
			static TimeMenu& GetInstance()
			{
				static TimeMenu instance;
				return instance;
			}

			void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

		private:
			TimeMenu() {} // Private constructor to prevent external instantiation
			TimeMenu(const TimeMenu&) = delete;
			TimeMenu& operator=(const TimeMenu&) = delete;

			// These variables don't need to be accessed outside this class.
			int hourToSet = 0;
			int minuteToSet = 0;
			int secondToSet = 0;

			int minHourToSet = 0;
			int minMinuteToSet = 0;
			int minSecondToSet = 0;

			int maxHourToSet = 23;
			int maxMinuteToSet = 59;
			int maxSecondToSet = 59;
		};
	} // namespace World
} // namespace Menus

