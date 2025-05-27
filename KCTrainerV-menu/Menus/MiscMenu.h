#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "../Scripts/Extras/Classes/GTAblip.h"

#include "IMenuBuilder.h"

class MiscMenu : public IMenuBuilder
{
public:
	// Instance of the MiscMenu
	static MiscMenu& GetInstance()
	{
		static MiscMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

private:
	MiscMenu() {} // Private constructor to prevent external instantiation
	MiscMenu(const MiscMenu&) = delete;
	MiscMenu& operator=(const MiscMenu&) = delete;
};

