#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

// New namespace format for menus
namespace Misc
{
	class GlobalsMenu : public IMenuBuilder
	{
	public:
		// Instance of GlobalsMenu
		static GlobalsMenu& GetInstance()
		{
			static GlobalsMenu instance;
			return instance;
		}

		void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	private:
		GlobalsMenu() {} // Private constructor to prevent external instantiation
		GlobalsMenu(const GlobalsMenu&) = delete;
		GlobalsMenu& operator=(const GlobalsMenu&) = delete;

		// TODO Move into the class.
		int currentPhoneDisplayStatus = 0;
		int currentMilBaseStatus = 0;
	};

} // namespace Misc
