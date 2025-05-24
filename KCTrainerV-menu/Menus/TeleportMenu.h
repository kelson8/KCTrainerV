#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "IMenuBuilder.h"

class TeleportMenu : public IMenuBuilder
{
public:
	// Instance of the TeleportMenu
	static TeleportMenu& GetInstance()
	{
		static TeleportMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	void BuildTeleportLocationsSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);


	// New teleport locations with categories, format from my vehicle spawner.
	void BuildAirportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void BuildSafehousesSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

	void BuildOtherSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	//

	void BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

#ifdef LUA_TEST
	void BuildLuaTeleportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
#endif // LUA_TEST


private:
	TeleportMenu() {} // Private constructor to prevent external instantiation
	TeleportMenu(const TeleportMenu&) = delete;
	TeleportMenu& operator=(const TeleportMenu&) = delete;
};

