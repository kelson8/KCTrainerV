#pragma once
#include "ScriptMenu.hpp"
#include "KCMenuScript.hpp"

#include "Scripts/Extras/Classes/GTAblip.h"

#include "../../IMenuBuilder.h"

namespace Misc
{
	class BlipsMenu : public IMenuBuilder
	{
	public:
		// Instance of BlipsMenu
		static BlipsMenu& GetInstance()
		{
			static BlipsMenu instance;
			return instance;
		}

		void Build(NativeMenu::Menu& mbCtx, KCMainScript& context) override;

	private:
		BlipsMenu() {} // Private constructor to prevent external instantiation
		BlipsMenu(const BlipsMenu&) = delete;
		BlipsMenu& operator=(const BlipsMenu&) = delete;

		// Current set blip for the blip menu

		// Hmm, I didn't know the World namespace had creating blips.
		//GTAblip blip = World::CreateBlip(Teleports::vSafeHouseLocations.at("Michael's House"));
		// Well no wonder this never worked, it kept getting changed in the loop.
		GTAblip blip;
	};
} // namespace Misc

