#include "pch.h"

#include "ModelChangerMenu.h"

#include "Constants.hpp"

#include "Scripts/Extras/Game.h"

#include "Util/UI.hpp"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/VehicleSpawner.h"

#include "Scripts/PedModelManager.h"

#include "Scripts/Vehicles/VehicleData.h"

#ifdef PLAYER_SKIN_CHANGER

void ModelChangerMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{


    mbCtx.Title("Model Changer");

    auto& pedModelManager = PedModelManager::GetInstance();
    const auto& allCategories = pedModelManager.GetAllCategories();

    if (allCategories.empty())
    {
        mbCtx.Option("No ped data loaded. Check logs");
        return;
    }

    if (allCategories.empty()) {
        mbCtx.Option("No ped data loaded. Check logs.");
        return;
    }

    // Iterate through each category and create a submenu for it
    for (const auto& category : allCategories) {
        // Each category gets its own submenu with a unique ID
        std::string categoryMenuId = "ped_category_" + category.name; // e.g., "ped_category_Player"
        if (mbCtx.MenuOption(category.name, categoryMenuId, { "Open " + category.name + " Peds." })) {
            // Nothing more needed here, the menu library handles switching
        }
    }
}

#endif