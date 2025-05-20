#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

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
	void BuildTestSubMenu(NativeMenu::Menu mbCtx, KCMainScript& context);

private:
	MiscMenu() {} // Private constructor to prevent external instantiation
	MiscMenu(const MiscMenu&) = delete;
	MiscMenu& operator=(const MiscMenu&) = delete;
};

