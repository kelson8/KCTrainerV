#include "Script.hpp"
#include "MenuTexture.hpp"
#include "ScriptMenu.hpp"
#include "Util/Logger.hpp"
#include "Util/Paths.hpp"
#include "Util/UI.hpp"

#include "Teleports/TeleportLocations.h"

#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

#include <inc/main.h>

// This seems to have the main script init and tick for the menu.

namespace 
{
    std::shared_ptr<KCMainScript> coreScript;
    std::unique_ptr<CScriptMenu<KCMainScript>> scriptMenu;

    bool initialized = false;
}

// These functions are only called in Script.cpp so don't need to be exposed.
namespace KCMenu 
{
    void scriptInit();
    void scriptTick();
}

// ScriptHookV calls ScriptMain when loading a save,
// so it can happen multiple times in a game session.
void KCMenu::ScriptMain() 
{
    auto& teleportLocations = TeleportLocations::GetInstance();
#ifdef LUA_TEST
    auto& luaManager = LuaManager::GetInstance();
#endif //LUA_TEST

    // This check exists to prevent global objects from being
    // initialized multiple times.
    if (!initialized) 
    {
        LOG(INFO, "Script started");
        scriptInit();
        initialized = true;

        
#ifndef NEW_TELEPORTS
        // Get the teleport locations list for use in the teleports, required to be loaded in at script startup.
        teleportLocations.GetTeleportLocations();
#endif //!NEW_TELEPORTS

#ifdef LUA_TEST
        // Run lua init, TODO Test this.
        luaManager.InitializeLuaEnvironment();
#endif

    }
    else {
        LOG(INFO, "Script restarted");
    }

    scriptTick();
}

void KCMenu::scriptInit() 
{
    const auto settingsMenuPath = Paths::GetModPath() / "settings_menu.ini";

    coreScript = std::make_shared<KCMainScript>();

    // The menu being initialized. Note the passed settings,
    // the onInit and onExit lambdas and finally BuildMenu being called.
    scriptMenu = std::make_unique<CScriptMenu<KCMainScript>>(settingsMenuPath.string(),
        []() 
        {
            // I don't want log messages for opening/closing the menu so I'll disable them.
            //LOG(INFO, "Menu opened");
            // When the menu is opened, functions can be called.
            // In this case, the images in the example folder are refreshed.
            
            // I'm not using these so I just turned it off.
            //MenuTexture::UpdateTextures();

        },
        []() 
        {
            //LOG(INFO, "Menu closed");
            // When the menu is closed, functions can be called.
            // In this case, a notification is shown.
            //UI::Notify("Menu was closed");
        },
        // The menu is built and submenus are passed into the CScriptMenu object.
        // See KCMenuMenu.cpp for the actual implementation.
        BuildMenu()
    );
}

// This function starts the infinite loop that updates the script instance and menu, every game tick.
// It keeps running forever, until ScriptHookV restarts or stops the script.
void KCMenu::scriptTick() 
{
    while (true) {
        coreScript->Tick();
        scriptMenu->Tick(*coreScript);
        WAIT(0);
    }
}
