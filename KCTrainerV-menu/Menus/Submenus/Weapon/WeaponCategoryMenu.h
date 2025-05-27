#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"


namespace Menus
{
	namespace Weapon
	{
		class CategoryMenu : public IMenuBuilder
		{
		public:
			// Instance of CategoryMenu
			static CategoryMenu& GetInstance()
			{
				static CategoryMenu instance;
				return instance;
			}

			void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

			// Setup for the weapon categories.
			void BuildWeaponCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context, const std::string& categoryKey);

		private:
			CategoryMenu() {} // Private constructor to prevent external instantiation
			CategoryMenu(const CategoryMenu&) = delete;
			CategoryMenu& operator=(const CategoryMenu&) = delete;

		};
	} // namespace Weapon
} // namespace Menus

