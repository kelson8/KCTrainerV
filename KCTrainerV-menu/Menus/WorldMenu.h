#pragma once
#include "../Constants.hpp"
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "IMenuBuilder.h"

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


private:
	WorldMenu() {} // Private constructor to prevent external instantiation
	WorldMenu(const WorldMenu&) = delete;
	WorldMenu& operator=(const WorldMenu&) = delete;
};

