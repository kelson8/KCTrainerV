#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "IMenuBuilder.h"

class PedMenu : public IMenuBuilder
{
public:
	// Instance of the PedMenu
	static PedMenu& GetInstance()
	{
		static PedMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

private:
	PedMenu() {} // Private constructor to prevent external instantiation
	PedMenu(const PedMenu&) = delete;
	PedMenu& operator=(const PedMenu&) = delete;
};

