#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

class ModelChangerMenu : public IMenuBuilder
{
#ifdef PLAYER_SKIN_CHANGER
public:
	// Instance of the ModelChangerMenu
	static ModelChangerMenu& GetInstance()
	{
		static ModelChangerMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

private:
	ModelChangerMenu() {} // Private constructor to prevent external instantiation
	ModelChangerMenu(const ModelChangerMenu&) = delete;
	ModelChangerMenu& operator=(const ModelChangerMenu&) = delete;
#endif // PLAYER_SKIN_CHANGER
};

