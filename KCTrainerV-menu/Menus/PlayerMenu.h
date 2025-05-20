#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "IMenuBuilder.h"

//class PlayerMenu
class PlayerMenu : public IMenuBuilder
{
public:
	// Instance of the PlayerMenu
	static PlayerMenu& GetInstance()
	{
		static PlayerMenu instance;
		return instance;
	}

#ifdef MOVE_PLAYER_MENU
	//void Build(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;
	void BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

#endif // MOVE_PLAYER_MENU

private:

	PlayerMenu() {} // Private constructor to prevent external instantiation
	PlayerMenu(const PlayerMenu&) = delete;
	PlayerMenu& operator=(const PlayerMenu&) = delete;
};

