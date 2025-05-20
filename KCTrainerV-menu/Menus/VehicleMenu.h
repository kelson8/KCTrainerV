#pragma once
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

#include "IMenuBuilder.h"

class VehicleMenu : public IMenuBuilder
{
public:
	// Instance of the VehicleMenu
	static VehicleMenu& GetInstance()
	{
		static VehicleMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;
	void BuildVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void BuildSportsVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context);
	void BuildSuperVehicleCategorySubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

private:
	VehicleMenu() {} // Private constructor to prevent external instantiation
	VehicleMenu(const VehicleMenu&) = delete;
	VehicleMenu& operator=(const VehicleMenu&) = delete;
};

