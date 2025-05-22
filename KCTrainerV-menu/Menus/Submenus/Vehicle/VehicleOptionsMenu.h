#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

class VehicleOptionsMenu : public IMenuBuilder
{
public:
	// Instance of the VehicleOptionsMenu
	static VehicleOptionsMenu& GetInstance()
	{
		static VehicleOptionsMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

private:
	VehicleOptionsMenu() {} // Private constructor to prevent external instantiation
	VehicleOptionsMenu(const VehicleOptionsMenu&) = delete;
	VehicleOptionsMenu& operator=(const VehicleOptionsMenu&) = delete;
};

