#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

class PlayerOptionsMenu : public IMenuBuilder
{
public:
	// Instance of PlayerOptionsMenu
	static PlayerOptionsMenu& GetInstance()
	{
		static PlayerOptionsMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

private:
	PlayerOptionsMenu() {} // Private constructor to prevent external instantiation
	PlayerOptionsMenu(const PlayerOptionsMenu&) = delete;
	PlayerOptionsMenu& operator=(const PlayerOptionsMenu&) = delete;
};

