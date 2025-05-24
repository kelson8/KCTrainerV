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

	// This works, need to switch my teleport menus to using it.
#ifdef NEW_TELEPORT_CATEGORIES
	void BuildNewAirportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void BuildNewSafehousesSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

	void BuildTestTeleportSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
#endif
	//

#ifdef NEW_TELEPORTS
	void BuildAirportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void BuildSafehousesSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
#endif //NEW_TELEPORTS

	void BuildDebugSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

#ifdef LUA_TEST
	void BuildLuaTeleportSubMenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
#endif // LUA_TEST


private:
	TeleportMenu() {} // Private constructor to prevent external instantiation
	TeleportMenu(const TeleportMenu&) = delete;
	TeleportMenu& operator=(const TeleportMenu&) = delete;
};

