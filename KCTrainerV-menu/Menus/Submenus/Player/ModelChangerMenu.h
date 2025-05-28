#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "../../IMenuBuilder.h"

class ModelChangerMenu : public IMenuBuilder
{
#ifdef PLAYER_SKIN_CHANGER_NEW
public:
	// Instance of the ModelChangerMenu
	static ModelChangerMenu& GetInstance()
	{
		static ModelChangerMenu instance;
		return instance;
	}

	void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;
	void BuildClothesChangerSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context);

private:
	ModelChangerMenu() {} // Private constructor to prevent external instantiation
	ModelChangerMenu(const ModelChangerMenu&) = delete;
	ModelChangerMenu& operator=(const ModelChangerMenu&) = delete;

	// The options to be selected in the menu.
	int hairOption = 0;
	int hairTexture = 0;

	int shirtOption = 0;
	int shirtTexture = 0;

	int torsoOption = 0;
	int torsoTexture = 0;

	int pantsOption = 0;
	int pantsTexture = 0;

	int shoesOption = 0;
	int shoesTexture = 0;

	int hairColorOption = 0;

#endif // PLAYER_SKIN_CHANGER_NEW
};

