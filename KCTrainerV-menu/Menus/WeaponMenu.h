#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "IMenuBuilder.h"

class WeaponMenu : public IMenuBuilder
{
public:
	// Instance of the WeaponMenu
	static WeaponMenu& GetInstance()
	{
		static WeaponMenu  instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

private:
	WeaponMenu() {} // Private constructor to prevent external instantiation
	WeaponMenu(const WeaponMenu&) = delete;
	WeaponMenu& operator=(const WeaponMenu&) = delete;
};

