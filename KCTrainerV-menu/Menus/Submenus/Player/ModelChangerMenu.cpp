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

// Here are the multiplayer ped types:
/*
Female: mp_f_freemode_01
Male: mp_m_freemode_01
*/

// TODO Move some of this into another file, maybe ModelChangerScript in Scripts? For now I'll leave it here

void RequestAndSetModel(Hash model)
{
    Ped playerId = PLAYER_ID();
    if (IS_MODEL_IN_CDIMAGE(model) && IS_MODEL_A_PED(model))
    {
        REQUEST_MODEL(model);
        while (!HAS_MODEL_LOADED(model))
        {
            WAIT(0);
        }
        SET_PLAYER_MODEL(playerId, model);
        SET_MODEL_AS_NO_LONGER_NEEDED(model);
    }
}

/// <summary>
/// Get the current ped model
/// </summary>
/// <returns></returns>
Hash CheckPlayerModel()
{
    Ped playerId = PLAYER_PED_ID();
    Hash playerModel = GET_ENTITY_MODEL(playerId);

    //log_output(std::format("Ped model: {}", playerModel));

    return playerModel;
}

#pragma region ClothesModels

/// <summary>
/// Set the ped clothes
/// Well this mostly works, only if i change the shoes in the menu though.
/// It changes everything, I made this separate.
/// </summary>
/// <param name="ped"></param>
/// <param name="drawableId"></param>
/// <param name="textureId"></param>
/// <param name="paletteId"></param>
//void SetClothes(Ped ped, int drawableId, int textureId, int paletteId)
void SetClothes(Ped ped, int drawableId, int textureId, int paletteId)
{
    // Hair style
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HAIR, drawableId, textureId, paletteId);

    // Head
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HEAD, drawableId, textureId, paletteId);
    // Torsos
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_UPPR, drawableId, textureId, paletteId);
    // Pants
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_LOWR, drawableId, textureId, paletteId);
    // Shoes
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_FEET, drawableId, textureId, paletteId);
    // Tops
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_JBIB, drawableId, textureId, paletteId);
}

/// <summary>
/// Set the hair style
/// </summary>
void SetHairStyle(Ped ped, int drawableId, int textureId, int paletteId)
{
    // Hair style
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HAIR, drawableId, textureId, paletteId);
}

/// <summary>
/// Set the head style
/// </summary>
void SetHeadStyle(Ped ped, int drawableId, int textureId, int paletteId)
{
    // Head
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HEAD, drawableId, textureId, paletteId);
}

/// <summary>
/// Set the torso style
/// </summary>
void SetTorsoStyle(Ped ped, int drawableId, int textureId, int paletteId)
{
    // Torsos
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_UPPR, drawableId, textureId, paletteId);
}

/// <summary>
/// Set the pants style
/// </summary>
void SetPantsStyle(Ped ped, int drawableId, int textureId, int paletteId)
{
    // Pants
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_LOWR, drawableId, textureId, paletteId);
}

/// <summary>
/// Set the shoes style
/// </summary>
void SetShoesStyle(Ped ped, int drawableId, int textureId, int paletteId)
{
    // Shoes
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_FEET, drawableId, textureId, paletteId);
}

/// <summary>
/// Set the shirt style
/// </summary>
void SetShirtStyle(Ped ped, int drawableId, int textureId, int paletteId)
{
    // Tops
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_JBIB, drawableId, textureId, paletteId);
}

/// <summary>
/// Set the ped hair color
/// </summary>
/// <param name="ped"></param>
/// <param name="color"></param>
/// <param name="highlightColor"></param>
void SetHairColor(Ped ped, int color, int highlightColor)
{
    SET_PED_HAIR_TINT(ped, color, highlightColor);
}

/// <summary>
/// Set default clothes for the MP Female ped.
/// </summary>
/// <param name="ped"></param>
void SetDefaultFemaleClothes(Ped ped)
{
    // 2 = Hair styles
    int hairStyles = 2;
    // 
    int drawable = 2;
    // TODO Figure these two out, for now I think 0 is fine?
    int textureId = 0;
    int paletteId = 0;

    SET_PED_COMPONENT_VARIATION(ped, hairStyles, drawable, textureId, paletteId);

    // Head
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_HEAD, 0, textureId, paletteId);
    // Torsos
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_UPPR, 0, textureId, paletteId);
    // Pants
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_LOWR, 0, textureId, paletteId);
    // Shoes
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_FEET, 0, textureId, paletteId);
    // Tops
    SET_PED_COMPONENT_VARIATION(ped, PV_COMP_JBIB, 0, textureId, paletteId);
}

#pragma endregion

#ifdef PLAYER_SKIN_CHANGER_NEW
void ModelChangerMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Model Changer");
    //Ped playerId = PLAYER_ID();

    Ped playerId = PLAYER_PED_ID();

    // Multiplayer
    if (mbCtx.Option("MP Male", { "Multiplayer Male" }))
    {
        RequestAndSetModel("mp_m_freemode_01"_hash);
    }
    
    if (mbCtx.Option("MP Female", {"Multiplayer Female"}))
    {
        RequestAndSetModel("mp_f_freemode_01"_hash);

        // https://gtaforums.com/topic/985422-solved-c-switching-to-some-peds-makes-you-invisible/
        // Possible fix for being invisible.
        //SET_PED_DEFAULT_COMPONENT_VARIATION(playerId);
        //SET_PED_RANDOM_COMPONENT_VARIATION(playerId, 0);
        SetDefaultFemaleClothes(playerId);
    }

    // Defaults
    if (mbCtx.Option("Micheal"))
    {
        RequestAndSetModel("player_zero"_hash);
    }

    if (mbCtx.Option("Franklin"))
    {
        RequestAndSetModel("player_one"_hash);
    }

    if (mbCtx.Option("Trevor"))
    {
        RequestAndSetModel("player_two"_hash);
    }

    mbCtx.MenuOption("Clothes changer", "PlayerModelClothesSubmenu");

#ifdef PLAYER_SKIN_CHANGER
    

    // TODO Fix this part
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
#endif // PLAYER_SKIN_CHANGER

}



/// <summary>
/// Build the clothes changer sub menu
/// Useful website for these: https://wiki.rage.mp/wiki/Player::setComponentVariation
/// TODO Set this up for the multiplayer models.
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void ModelChangerMenu::BuildClothesChangerSubmenu(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    mbCtx.Title("Clothes");

    //Ped playerId = PLAYER_ID();
    Ped playerId = PLAYER_PED_ID();

    // TODO Switch these below to using this.
    Hash mpFemalePed = "mp_f_freemode_01"_hash;
    
    int stepValue = 1;

    // Max possible values for these
    int maxHairFemale = 80;
    int maxShirtFemale = 414;
    int maxTorsoFemale = 241;
    int maxPantsFemale = 150;
    int maxShoesFemale = 150;

    // TODO Add custom option for textureId instead of hardcoding 0 on each one

    // Hair
    mbCtx.IntOption("Hair", hairOption, 0, maxHairFemale, stepValue);
    if (mbCtx.Option("Change hair"))
    {
        // Female
        if (CheckPlayerModel() == "mp_f_freemode_01"_hash)
        {
            SetHairStyle(playerId, hairOption, 0, 0);
            log_output(std::format("Hair option: {}", hairOption));
        }
    }

    // Shirt
    mbCtx.IntOption("Shirt", shirtOption, 0, maxShirtFemale, stepValue);

    // This does seem to set the shirt texture
    // TODO Add this option to the other items also.
    mbCtx.IntOption("Shirt Texture", shirtTexture, 0, 20, stepValue);
    if (mbCtx.Option("Change shirt"))
    {
        // Female
        if (CheckPlayerModel() == "mp_f_freemode_01"_hash)
        {
            //SetShirtStyle(playerId, shirtOption, 0, 0);
            SetShirtStyle(playerId, shirtOption, shirtTexture, 0);
            log_output(std::format("Shirt option: {}", shirtOption));
        }
    }

    // Torso
    mbCtx.IntOption("Torso", torsoOption, 0, maxTorsoFemale, stepValue);
    if (mbCtx.Option("Change torso"))
    {
        // Female
        if (CheckPlayerModel() == "mp_f_freemode_01"_hash)
        {
            SetTorsoStyle(playerId, torsoOption, 0, 0);
            log_output(std::format("Torso option: {}", torsoOption));
        }
    }
    
    // Pants
    mbCtx.IntOption("Pants", pantsOption, 0, maxPantsFemale, stepValue);
    if (mbCtx.Option("Change pants"))
    {
        // Female
        if (CheckPlayerModel() == "mp_f_freemode_01"_hash)
        {
            SetPantsStyle(playerId, pantsOption, 0, 0);
            log_output(std::format("Pants option: {}", pantsOption));
        }
    }


    // Shoes
    mbCtx.IntOption("Shoes", shoesOption, 0, maxShoesFemale, stepValue);
    if (mbCtx.Option("Change shoes"))
    {
        // Female
        if (CheckPlayerModel() == "mp_f_freemode_01"_hash)
        {
            SetShoesStyle(playerId, shoesOption, 0, 0);
            log_output(std::format("Shoes option: {}", shoesOption));
        }
    }

    // This seems to work
    mbCtx.IntOption("Hair color", hairColorOption, 0, 20, stepValue);

    if (mbCtx.Option("Change hair color"))
    {
        // This lets me change a hair color
        SET_PED_HEAD_BLEND_DATA(playerId, 0, 0, 0, 0, 0, 0, 0, 0, 0, false);
        SetHairColor(playerId, hairColorOption, 0);
    }

    // TODO Fix this to work right, currently shoes changes most of the character.
    //if (mbCtx.Option("Change Clothes", { "Only works for multiplayer models for now!" }))
    //{
    //    // Well a switch doesn't seem to work for this.
    //    //switch (CheckPlayerModel())
    //    //{
    //    //case "mp_f_freemode_01"_hash:
    //    //    break;
    //    //}

    //    // Multiplayer models
    //    
    //    // Male
    //    //if (CheckPlayerModel() == "mp_m_freemode_01"_hash)
    //    //{
    //    //    return;
    //    //}

    //    // Female
    //    if (CheckPlayerModel() == "mp_f_freemode_01"_hash)
    //    {
    //        // TODO Add selector for this.
    //        //SetDefaultFemaleClothes(playerId);
    //        SetClothes(playerId, hairOption, 0, 0);
    //        SetClothes(playerId, shirtOption, 0, 0);
    //        SetClothes(playerId, torsoOption, 0, 0);
    //        SetClothes(playerId, pantsOption, 0, 0);
    //        SetClothes(playerId, shoesOption, 0, 0);

    //        std::string clothesVariantsString = std::format("Hair: {}, Shirt: Torso: {}, Pants: {}, Shoes: {}",
    //            hairOption, shirtOption, torsoOption, pantsOption, shoesOption);
    //        log_output(clothesVariantsString);
    //    }
    //}


}

#endif // PLAYER_SKIN_CHANGER_NEW

