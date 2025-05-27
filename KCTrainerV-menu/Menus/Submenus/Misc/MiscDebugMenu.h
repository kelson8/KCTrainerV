#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

namespace Misc
{
	class DebugMenu : public IMenuBuilder
	{
	public:
		// Instance of DebugMenu
		static DebugMenu& GetInstance()
		{
			static DebugMenu instance;
			return instance;
		}

		void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	private:
		DebugMenu() {} // Private constructor to prevent external instantiation
		DebugMenu(const DebugMenu&) = delete;
		DebugMenu& operator=(const DebugMenu&) = delete;
	};

} // namespace Misc