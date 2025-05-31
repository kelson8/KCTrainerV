#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

namespace Misc
{
	class MarkerDebugMenu : public IMenuBuilder
	{
	public:
		// Instance of DebugMenu
		static MarkerDebugMenu& GetInstance()
		{
			static MarkerDebugMenu instance;
			return instance;
		}

		void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	private:
		MarkerDebugMenu() {} // Private constructor to prevent external instantiation
		MarkerDebugMenu(const MarkerDebugMenu&) = delete;
		MarkerDebugMenu& operator=(const MarkerDebugMenu&) = delete;
	};

} // namespace Misc