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

#include <iostream>

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
#ifdef DEBUG_MODE
    void AttachConsole();
#endif // DEBUG_MODE
}

#ifdef DEBUG_MODE
/// <summary>
/// This was mostly taken from KCTrainerIV, originally in use in my ReVC tests.
/// Runs a console window for cout and other errors to be displayed
/// Also print from lua into the console if enabled.
/// Only run this in debug builds, I might add a ini option for this sometime.
/// </summary>
void KCMenu::AttachConsole()
{
#ifdef _WIN32
    if (AllocConsole())
    {
        // TODO Possibly add log messages in here for logging when this attaches and errors to the KCTrainerV.log?

        // Redirect standard output, error, and input streams to the console
        FILE* dummy;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
        freopen_s(&dummy, "CONIN$", "r", stdin);

        // Optional: Set the console title
        SetConsoleTitle("KCTrainerV");

        std::cout << "Console attached successfully!" << std::endl;
        //std::cerr << "Error output will also appear here." << std::endl;
    }
    else
    {
        // Handle the case where console allocation fails (unlikely in most scenarios)
        // You might want to log an error message using your in-game system.
        std::cerr << "Failed to allocate console." << std::endl;
    }
#else
    std::cerr << "AllocConsole() is only available on Windows." << std::endl;
#endif // _WIN32
}
#endif //DEBUG_MODE

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

#ifdef LUA_TEST
        // Run lua init BEFORE scriptInit
        LOG(INFO, "Initializing Lua environment before scriptInit.");
        LuaManager::GetInstance().InitializeLuaEnvironment();
        LOG(INFO, "Lua environment initialization complete.");

        // TODO Fix this
        // Now pass the coreScript instance to BindCustomFunctionsToLua
        //if (coreScript) {
        //    LuaManager::GetInstance().BindCustomFunctionsToLua(*coreScript);
        //}
        //else {
        //    LOG(ERROR, "coreScript not initialized when binding custom Lua functions.");
        //}

#endif

        scriptInit();
        initialized = true;

        
#ifndef NEW_TELEPORTS
        // Get the teleport locations list for use in the teleports, required to be loaded in at script startup.
        teleportLocations.GetTeleportLocations();
#endif //!NEW_TELEPORTS

        // The Lua initialization is now done before scriptInit
//#ifdef LUA_TEST
//        // Run lua init, TODO Test this.
//        //luaManager.InitializeLuaEnvironment();
//        LuaManager::GetInstance().InitializeLuaEnvironment();
//#endif

    }
    else {
        LOG(INFO, "Script restarted");
    }

    scriptTick();
}

void KCMenu::scriptInit() 
{
    // TODO Figure out how to add a reload option to the menu, I might use the scriptInit but this might crash the menu.
    const auto settingsMenuPath = Paths::GetModPath() / "settings_menu.ini";

    coreScript = std::make_shared<KCMainScript>();

    // Only run this in debug builds, I might add a ini option for this sometime.
#ifdef DEBUG_MODE
    // Attach the console to the game in scriptInit
    AttachConsole();
#endif

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

#ifdef RELOAD_CONFIG
    // Now that scriptMenu is created, we can potentially pass it to coreScript
    coreScript->SetScriptMenu(scriptMenu.get()); // Add a setter in KCMainScript
#endif
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
