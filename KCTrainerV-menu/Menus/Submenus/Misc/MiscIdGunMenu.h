#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

namespace Misc
{
	class IdGunMenu : public IMenuBuilder
	{
	public:
		// Instance of IdGunMenu
		static IdGunMenu& GetInstance()
		{
			static IdGunMenu instance;
			return instance;
		}

		void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	private:
		IdGunMenu() {} // Private constructor to prevent external instantiation
		IdGunMenu(const IdGunMenu&) = delete;
		IdGunMenu& operator=(const IdGunMenu&) = delete;
	};
} // namespace Misc

