#pragma once

#include "../Constants.hpp"
#include "ScriptMenu.hpp"
#include "../KCMenuScript.hpp"

// Menu builder interface, all menus should use this class.
class IMenuBuilder 
{
public:
	virtual ~IMenuBuilder() = default; // Important: Virtual destructor for base classes.

	virtual void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) = 0;
};