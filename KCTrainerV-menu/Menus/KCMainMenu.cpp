#include "ScriptMenu.hpp"
#include "Constants.hpp"
#include "KCMenuScript.hpp"
#include "MenuTexture.hpp"
#include "Script.hpp"

#include "Util/Logger.hpp"
#include "Util/Paths.hpp"
#include "Util/UI.hpp"

#include <inc/main.h>
#include <inc/natives.h>

#include "Scripts/PlayerScripts.h"

// Chaos mod
#include "Util/EntityIterator.h"

namespace 
{
    // A few state variables for demoing the menu
    bool checkBoxStatus = false;
    float someFloat = 0.0f;
    int someInt = 0;
    int intStep = 1;
    int stringsPos = 0;
    int stepChoice = 0;
    int numberOfOptions = 16;

    // Choice of step size to demonstrate variable precision display
    const std::vector<float> floatSteps = 
    {
        1.0f,
        0.1f,
        0.01f,
        0.001f,
        0.0001f,
        0.00001f,
        0.000001f,
    };

    // Random words to go through in the menu
    const std::vector<std::string> strings = 
    {
        "Hello",
        "World!",
    };
}


// Booleans for this file

//void ToggleInvincibility()
//{
//    if (invincibilityEnabled)
//    {
//        
//    }
//}

/*
 * This function builds the menu's submenus, and the submenus are passed into the CScriptMenu constructor.
 * While this provides a cleaner way to define the menu, dynamically created submenu are not possible.
 * CScriptMenu would need to be changed to allow adding and removing submenus on-the-fly.
 */
std::vector<CScriptMenu<KCMainScript>::CSubmenu> KCMenu::BuildMenu() 
{

    //PlayerScripts playerScripts;

    auto& playerScripts = PlayerScripts::GetInstance();

    std::vector<CScriptMenu<KCMainScript>::CSubmenu> submenus;
    submenus.emplace_back("mainmenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            // Title and Subtitle are required on each submenu.
            mbCtx.Title(Constants::ScriptName);
            mbCtx.Subtitle(std::string("~b~") + Constants::DisplayVersion);

            // This is a normal option. It'll return true when "select" is presed.
            if (mbCtx.Option("Click me!", { "This will log something to " + Paths::GetModuleNameWithoutExtension(Paths::GetOurModuleHandle()) + ".log" })) 
            {
                UI::Notify("Check the log file!");
                LOG(INFO, "\"Click me!\" was selected!");
            }

            // This will open a submenu with the name "submenu"
            // Oh, I forgot to add the sub menu up here

            mbCtx.MenuOption("Test Menu", "testmenu", { " This submenu contains testing for the new trainer. " });

#ifdef OLD_MENUS
            mbCtx.MenuOption("Option demos", "submenu", { "This submenu demonstrates usage of some options." });
            mbCtx.MenuOption("Variable size demo", "varmenu", { "This submenu demonstrates how items can be added or removed dynamically." });
#endif

            mbCtx.MenuOption("Title demo", "titlemenu", { "Showcase different title drawing options." });
            mbCtx.MenuOption("Image demo", "imagemenu", { "Showcase OptionPlus image arguments." });
            mbCtx.MenuOption("Script context demo", "scriptcontextmenu", { "Showcase interaction with separate \"game script\" from the menu implementation." });

            // Showing a non-scrolling aligned item is also possible, if the vector only contains one element.
            int nothing = 0;
            mbCtx.StringArray("Version", { Constants::DisplayVersion }, nothing,
                { "Thanks for checking out this menu!", "Author: ikt" });
        });


    // This should enable my test features.
#define _TEST
#ifdef _TEST
    // This fixes the issue with using some of these variables without static:
    // I added the & symbol where this is:  [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
    // https://stackoverflow.com/questions/26903602/an-enclosing-function-local-variable-cannot-be-referenced-in-a-lambda-body-unles

    submenus.emplace_back("testmenu",
        [&](NativeMenu::Menu& mbCtx, KCMainScript& context)
        {
            mbCtx.Title("Test Menu");

            if (mbCtx.Option("Set coords", { "This will set your coords to a value I have set." }))
            {
                //KCMainScript::SetPlayerCoords;
                //PlayerScripts::SetPlayerCoords(PlayerScripts::AIRPORT_RUNWAY);
                playerScripts.SetPlayerCoords(PlayerScripts::AIRPORT_RUNWAY);
            }

            // TODO Figure out how to set this one up, this won't toggle it just yet.
            //mbCtx.BoolOption("Invincibility", PlayerScripts::invincibilityEnabled, { " Turn on/off invincibility" });
            // For now, I'll just make this a button

            if (mbCtx.Option("Toggle invincibility", { " Turn on/off invincibility" }))
            {
                playerScripts.ToggleInvincibility();
            }

            if (mbCtx.Option("Toggle Never Wanted", { "Turn on/off never wanted" }))
            {
                playerScripts.ToggleNeverWanted();
            }


            mbCtx.IntOption("Wanted level", playerScripts.wantedLevel, 0, 5, 1, {"Wanted level to set"});
            if (mbCtx.Option("Set Wanted Level", { "Set your wanted level" }))
            {
                playerScripts.SetWantedLevel();
            }

            if (mbCtx.Option("Kill all peds", { "Kill all peds in the area" }))
            {
                Ped playerPed = playerScripts.GetPlayerPed();
                
                //std::list<Entity> entities;
                // Can be used like this in the for loop:
                // entities.push_back(ped);
                for (auto ped : GetAllPeds()) 
                {
                    // Check if they are the player and not dead, if so do nothing
                    if (!PED::IS_PED_A_PLAYER(ped) && !ENTITY::IS_ENTITY_DEAD(ped, false))
                    {
                        ENTITY::SET_ENTITY_HEALTH(ped, 0, 0);
                    }
                    
                }
                    
                //entities.push_back(ped);
                //for (auto veh : GetAllVehs())
                //    entities.push_back(veh);
                //for (auto prop : GetAllProps())
                //    entities.push_back(prop);
            }


            // This works as a submenu nested within a sub menu, can be useful for later.
            // TODO Use this and create a teleport list.
            mbCtx.MenuOption("Test Sub Menu", "submenutest", { "Show a test submenu within this menu." });

            //if (mbCtx.Option("Toggle bomb bay", { "This will open/close the bomb bay doors in a plane." })) 
            // {
            //    //KCMainScript::ToggleBombBayDoors();
            //    VehicleScripts::ToggleBombBayDoors();
            //}
        }
    );

    submenus.emplace_back("submenutest",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("Test Sub menu");

            if (mbCtx.Option("Notify", { "Test notification" })) 
            {
                UI::Notify("Test notification");
            }
        });
#endif //_TEST


    // TODO Remove these later, I will use them as a reference for now.
#ifdef OLD_MENUS
    submenus.emplace_back("submenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("Option demos");
            mbCtx.Subtitle("");

            mbCtx.BoolOption("BoolOption", checkBoxStatus, { std::string("Boolean is ") + (checkBoxStatus ? "checked" : "not checked") + "." });
            mbCtx.IntOption("IntOption step size", intStep, 1, 100, 1, { "Change the step size for IntOption below" });
            mbCtx.IntOption("IntOption", someInt, -100, 100, intStep, { "Stepsize: " + std::to_string(intStep) });
            mbCtx.FloatArray("FloatOption step size", floatSteps, stepChoice, { "Change the step size for FloatOption below.",
                "Note: this option uses FloatArray." });
            mbCtx.FloatOption("FloatOption", someFloat, -100.0f, 100.0f, floatSteps[stepChoice], { "Try holding left/right, things should speed up." });
            mbCtx.StringArray("StringArray", strings, stringsPos, { "You can also show different strings" });

            mbCtx.Option("Option/Details",
                { "You can put arbitarily long texts in the details. Word wrapping happens automatically.",
                    "Vector elements are newlines." });

            // Some extra information can be shown on the right of the the menu.
            std::vector<std::string> extraInfo = 
            {
                "OptionPlus",
                "This box supports images, in-game sprites and texts. ",
                "Longer texts can be used without problems, this box splits the lines "
                "by itself. As with the details, a new vector element inserts a newline."
            };
            mbCtx.OptionPlus("OptionPlus", extraInfo, nullptr,
                []() 
                {
                    UI::Notify("You pressed RIGHT on an OptionPlus"); 
                },
                []() 
                {
                    UI::Notify("You pressed LEFT on an OptionPlus");
                },
                "OptionPlus extras",
                { "This box also manages string splitting for width." });
        });

    submenus.emplace_back("varmenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("Variable size");
            mbCtx.Subtitle("");

            mbCtx.IntOption("Options #", numberOfOptions, 1, 999, 1, { "Increase or decrease the amount of items." });

            for (int i = 0; i < numberOfOptions; i++) {
                int display = i + 1;
                mbCtx.IntOption("Option number", display, display, display, 0, { "This option was automatically generated." });
            }
        });

#endif //OLD_MENUS

    submenus.emplace_back("titlemenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("Title demo");
            mbCtx.Subtitle("");

            mbCtx.MenuOption("Los Santos Customs background", "title_lscmenu", { "Internal sprites as background." });
            mbCtx.MenuOption("Custom background", "title_pngmenu", { "External textures as background." });
            mbCtx.MenuOption("Custom background 2", "title_pngmenu2", { "External textures as background. Colored footer." });

            mbCtx.MenuOption("Long title text", "longtitlemenu",
                { "Automatically fit and resize long titles. Works for up to 2 lines of text after processing." });
        });


    submenus.emplace_back("title_lscmenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("", "shopui_title_carmod", "shopui_title_carmod");
            mbCtx.Subtitle("Sprite background");
            mbCtx.Option("Option");
        });

    submenus.emplace_back("title_pngmenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            const auto& textures = MenuTexture::GetTextures();
            auto foundTexture = textures.find("custom_background");
            if (foundTexture != textures.end()) {
                mbCtx.Title("", foundTexture->second.Handle);
            }
            else {
                mbCtx.Title("custom_background not found");
            }
            mbCtx.Subtitle("Custom background");
            mbCtx.Option("Option");
        });

    submenus.emplace_back("title_pngmenu2",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            const auto& textures = MenuTexture::GetTextures();
            auto foundTexture = textures.find("custom_background2");
            if (foundTexture != textures.end()) {
                mbCtx.Title("", foundTexture->second.Handle);
            }
            else {
                mbCtx.Title("custom_background2 not found");
            }
            mbCtx.Subtitle("Custom background");
            mbCtx.Option("Option");
            mbCtx.Footer({ 173, 236, 173, 255 });
        });

    submenus.emplace_back("longtitlemenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("Further Adventures in Finance and Felony");
            mbCtx.Subtitle("Long title text");
            mbCtx.Option("Option");
        });


    submenus.emplace_back("imagemenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("Images");
            mbCtx.Subtitle("Image demo");

            {
                std::vector<std::string> extras;
                // The OptionPlus interprets a specifically formatted string to display game textures/sprites.
                // !SPR:{dictionary} {name} W{width}H{height}
                // Or, filled in:
                // !SPR:dock_default seashark W512H256
                // for the seashark in the dock_default txd, with hardcoded size 512x1080.
                extras.push_back(std::format("{}{} {} W{}H{}",
                    mbCtx.SpritePrefix, "dock_default", "seashark",
                    512, 256));
                mbCtx.OptionPlus("Game texture", extras, nullptr, nullptr, nullptr, "Sprite", { "OptionPlus box with sprite." });
            }

            for (const auto& [fileName, texture] : MenuTexture::GetTextures()) 
            {
                std::vector<std::string> extras;
                // The OptionPlus interprets a specifically formatted string to display images.
                // !IMG:{texture_handle}W{width}H{height}
                // Or, filled in:
                // !IMG:20W1920H1080
                // for an image with texture handle 20, size 1920x1080.
                extras.push_back(std::format("{}{}W{}H{}",
                    mbCtx.ImagePrefix, texture.Handle,
                    texture.Width, texture.Height));
                extras.push_back(texture.Filename);
                mbCtx.OptionPlus(texture.Filename, extras, nullptr, nullptr, nullptr, "Image", { "OptionPlus box with custom texture." });
            }
        });

    submenus.emplace_back("scriptcontextmenu",
        [](NativeMenu::Menu& mbCtx, KCMainScript& context) 
        {
            mbCtx.Title("Script Context");
            mbCtx.Subtitle("ScriptContext demo");

            // Here we can show stuff that the "core script" updates,
            // independently of the menu.
            // Notice how the menu doesn't need to include any natives.

            int nothing = 0;
            mbCtx.StringArray("Player health", { context.GetPlayerHealth() }, nothing);

            mbCtx.StringArray("Player vehicle", { context.GetPlayerVehicleName() }, nothing);

            mbCtx.StringArray("Distance traveled", { context.GetDistanceTraveled() }, nothing);

            // This option only appears if the player is in a vehicle.
            if (context.IsPlayerInVehicle()) {
                // This is triggered when pressing "Enter"
                if (mbCtx.Option("Randomize player vehicle color")) {
                    context.ChangePlayerVehicleColor();
                }
            }
        });

    return submenus;
}
