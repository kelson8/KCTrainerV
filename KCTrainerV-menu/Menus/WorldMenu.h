#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "IMenuBuilder.h"

struct WeatherInfo
{
	std::string name;
	std::string weatherName;
};

class WorldMenu : public IMenuBuilder
{
public:
	// Instance of the WorldMenu
	static WorldMenu& GetInstance()
	{
		static WorldMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	void BuildWeatherMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void BuildTimeMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

	

private:
	WorldMenu() {} // Private constructor to prevent external instantiation
	WorldMenu(const WorldMenu&) = delete;
	WorldMenu& operator=(const WorldMenu&) = delete;

	// These variables don't need to be accessed outside this class.
	static const std::vector<WeatherInfo> g_weatherList;

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

