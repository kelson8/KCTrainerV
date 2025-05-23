#include "Script.hpp"
#include "MenuTexture.hpp"
#include "ScriptMenu.hpp"
#include "Util/Logger.hpp"
#include "Util/Paths.hpp"
#include "Util/UI.hpp"

#include "Constants.hpp"

#include "Teleports/TeleportLocations.h"
#include "Scripts/MiscScripts.h"
#include "Scripts/VehicleScripts.h"
#include "Scripts/VehicleSpawner.h"

#include "Scripts/NotificationManager.h"
#include "Scripts/PedModelManager.h"

#include "Util/FileFunctions.h"

#include "defines.h"

#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

#include "Memory/Memory.h"

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
    void LoadNotificationFile();

#ifdef PLAYER_SKIN_CHANGER
    void LoadPedsFile();
#endif

    void scriptInit();
    void scriptTick();
#ifdef DEBUG_MODE
    void AttachConsole();
#endif // DEBUG_MODE

    bool thread2Started = false;
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

        std::string programBuildString = std::format("Program build time and date: {} @ {}", __DATE__, __TIME__);
        //std::string programVersionString = std::format("KCTrainerV Version: {}", Constants::DisplayVersion);
        std::string programVersionString = std::format("{} Version: {}", Constants::ScriptName, Constants::DisplayVersion);

        // Redirect standard output, error, and input streams to the console
        FILE* dummy;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
        freopen_s(&dummy, "CONIN$", "r", stdin);

        // Optional: Set the console title
        //SetConsoleTitle("KCTrainerV");
        SetConsoleTitle(Constants::ScriptName);

        log_output("Console attached successfully!");
        //std::cout << "Console attached successfully!" << std::endl;


        // Show the time and date when this was built
        log_output(programBuildString);
        //std::cout << programBuildString << std::endl;

        // Show the version string for the trainer
        //std::cout << programVersionString << std::endl;
        log_output(programVersionString);
        //std::cout << programVersionString << std::endl;
    }
    else
    {
        // Handle the case where console allocation fails (unlikely in most scenarios)
        // You might want to log an error message using your in-game system.
        log_error("Failed to allocate console.");
        LOG(ERROR, "Failed to allocate console.");
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

        // Load notifications from the file.
        KCMenu::LoadNotificationFile();

#ifdef PLAYER_SKIN_CHANGER
        // Load the peds from the file list
        // For changing player model, and in the future spawning peds.
        KCMenu::LoadPedsFile();
#endif

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



void KCMenu::LoadNotificationFile()
{
    auto& fileFunctions = FileFunctions::GetInstance();
    // Get the mod path and file name, to check if it exists.
    std::filesystem::path modPath = Paths::GetModPath();
    std::string pathString = (modPath / Constants::notificationFileName).string();

    if (fileFunctions.DoesFileExist(pathString))
    {
        NotificationManager::LoadNotificationsFromFile(pathString);
    }
    else {
        std::string errorMessage = std::format("Notification file {} not found.", pathString);
        LOG(ERROR, errorMessage);
        log_output(errorMessage);
    }
}


#ifdef PLAYER_SKIN_CHANGER
void KCMenu::LoadPedsFile()
{
    auto& fileFunctions = FileFunctions::GetInstance();

    auto& pedModelManager = PedModelManager::GetInstance();


    // Get the mod path and file name, to check if it exists.
    std::filesystem::path modPath = Paths::GetModPath();
    std::string pathString = (modPath / Constants::pedsFileName).string();

    if (fileFunctions.DoesFileExist(pathString))
    {
        pedModelManager.LoadPedsFromFile(pathString);
    }
    else {
        std::string errorMessage = std::format("Peds file {} not found.", pathString);
        LOG(ERROR, errorMessage);
        log_output(errorMessage);
    }
}
#endif

/// <summary>
/// Script init
/// </summary>
void KCMenu::scriptInit() 
{
    // TODO Figure out how to add a reload option to the menu, I might use the scriptInit but this might crash the menu.
    const auto settingsMenuPath = Paths::GetModPath() / "settings_menu.ini";

    coreScript = std::make_shared<KCMainScript>();

    // Setup random, so random numbers work
    srand(GetTickCount());

    // Only run this in debug builds, I might add a ini option for this sometime.
#ifdef DEBUG_MODE
    // Attach the console to the game in scriptInit
    AttachConsole();
#endif

    // From Chaos Mod init, I had to move this below AttachConsole otherwise it seems to break the debug console.
    // Attempt to print game build number
    // We're doing it here as the build number isn't available when the mod is attached to the game process
#ifdef MEMORY_TESTING
    static auto printedGameBuild = []()
    {
        auto gameBuild = Memory::GetGameBuild();
        if (gameBuild.empty())
            gameBuild = "Unknown";

        //LOG("Game Build: " << gameBuild);
        std::string gameBuildString = std::format("Game Build: {}", gameBuild);

        LOG(INFO, gameBuildString);
        //std::cout << "Game Build: " << gameBuild << std::endl;
        log_output(gameBuildString);

        return true;
    }();
#endif // MEMORY_TESTING

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

// This works! I moved the fade thread over to here.
void KCMenu::Thread2()
{
    // Ok this is running.
    if (!thread2Started)
    {
        log_output("Thread 2 started");
        thread2Started = true;
    }
    while (true)
    {
        // Run the vehicle spawner test.
#ifdef VEHICLE_SPAWNER_TEST
        g_vehicleSpawner.VehicleSpawnTick();
#endif

        // Run the fade thread
        MiscScripts::Fade::FadeThread();
        WAIT(0);
    }
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
