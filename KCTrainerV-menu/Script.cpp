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

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/TextScripts.h"
#include "Scripts/WorldScripts.h"

#include "Scripts/Stats.h"

#include "Scripts/NotificationManager.h"
#include "Scripts/PedModelManager.h"

#include "Util/FileFunctions.h"

#include "defines.h"

#ifdef LUA_TEST
#include "Components/LuaManager.h"
#endif //LUA_TEST

#include "Teleports/TeleportManager.h"

#include "Memory/Memory.h"

#include <iostream>

#include <inc/main.h>

#include "Util/Util.hpp"

// New for JSON support
#include <fstream>
#include "nlohmann/json.hpp"

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
    void LoadModelNamesFromJson(const std::string& filePath);

    void LoadWeaponsFromJson(const std::string& fileName);

    void ResetGameStates();

#ifdef PLAYER_SKIN_CHANGER
    void LoadPedsFile();
#endif

    void scriptInit();
    void scriptTick();
#ifdef DEBUG_MODE
    void AttachConsoleToScript();
#endif // DEBUG_MODE

#ifdef LUA_TEST
    void InitLua();
#endif // LUA_TEST

    bool thread2Started = false;
}

#ifdef LUA_TEST
/// <summary>
/// Init for lua environment.
/// </summary>
void KCMenu::InitLua() 
{
    // Run lua init BEFORE scriptInit
    LOG(INFO, "Initializing Lua environment before scriptInit.");
    LuaManager::GetInstance().InitializeLuaEnvironment();

    LOG(INFO, "Lua environment initialization complete.");
}
#endif // LUA_TEST

#ifdef DEBUG_MODE

#pragma region ResetToggles


/// <summary>
/// This function will contain all the logic to revert game states
/// TODO Fix this to work, for now it doesn't get run anywhere.
/// </summary>
void KCMenu::ResetGameStates() 
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& worldScripts = WorldScripts::GetInstance();

    // --- Player Scripts ---
    if (playerScripts.invincibilityEnabled)
    {
        PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), FALSE);
    }

    if (playerScripts.neverWantedEnabled) 
    {
        PLAYER::SET_MAX_WANTED_LEVEL(5); // Default GTA max
        // No need to clear current wanted level here; if player commits crimes, they'll get stars again.
    }

    // --- Vehicle Scripts ---
    // Make sure you get the current vehicle the player is in.
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
    if (ENTITY::DOES_ENTITY_EXIST(playerVehicle))
    { // Check if player is in a vehicle
        if (vehicleScripts.isBulletProofEnabled)
        {
            vehicleScripts.DisableBulletProof();
        }

        if (vehicleScripts.isInvincibleVehicleEnabled)
        {
            ENTITY::SET_ENTITY_INVINCIBLE(playerVehicle, false);
        }
    }

    if (worldScripts.isBlackoutActive)
    {
        GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(FALSE);
    }
}

/// <summary>
/// Reset all toggles back to their original states, should possibly prevent crashing?
/// Runs in DllMain before unregistering the script.
/// TODO Make another function that resets the values themselves back to default.
/// Such as resetting the invincibility, and never wanted states.
/// This just turns off the flags and toggles but doesn't disable everything.
/// </summary>
void KCMenu::ResetToggles()
{
    auto& playerScripts = PlayerScripts::GetInstance();
    auto& pedScripts = PedScripts::GetInstance();
    auto& textScripts = TextScripts::GetInstance();
    auto& vehicleScripts = VehicleScripts::GetInstance();
    auto& worldScripts = WorldScripts::GetInstance();

    // Player scripts
    playerScripts.invincibilityEnabled = false;
    playerScripts.invincibilityFlag = false;

    playerScripts.neverWantedEnabled = false;
    playerScripts.neverWantedFlag = false;

    playerScripts.isMobileRadioEnabled = false;
    playerScripts.mobileRadioFlag = false;

    playerScripts.isExplosiveBulletsEnabled = false;
    playerScripts.isExplosiveMeleeEnabled = false;
    playerScripts.isInfiniteAmmoEnabled = false;

    playerScripts.isSlowAimEnabled = false;
    playerScripts.slowAimFlag = false;

    // Other toggles
    playerScripts.isSuperJumpEnabled = false;
    playerScripts.isSuperRunEnabled = false;

    // Misc scripts
    MiscScripts::IDGun::isIdGunEnabled = false;

    // Ped scripts
    pedScripts.isPedsAttackEnabled = false;
    pedScripts.isCrazyPedDrivingEnabled = false;

    // Stats scripts
    Stats::Cop::isCopsKilledDisplayActive = false;

    // Text scripts
    textScripts.drawCoords = false;

    // Vehicle Scripts
    vehicleScripts.isBulletProofEnabled = false;
    vehicleScripts.bulletProofFlag = false;

    vehicleScripts.isInvincibleVehicleEnabled = false;
    vehicleScripts.invincibilityFlag = false;

    // World scripts
    worldScripts.isRestrictedAreasDisabled = false;
    worldScripts.areasDisabledFlag = false;

    worldScripts.isFireworksStarted = false;
    worldScripts.isPedsCalmActive = false;

    worldScripts.isBlackoutActive = false;
    worldScripts.blackoutFlag = false;

    // Toggles for disabling traffic and peds.
    worldScripts.isVehiclesDisabled = false;
    worldScripts.isPedsDisabled = false;

    LOG(INFO, "All toggles have been reset to defaults.");
}

#pragma endregion

/// <summary>
/// This was mostly taken from KCTrainerIV, originally in use in my ReVC tests.
/// Runs a console window for cout and other errors to be displayed
/// Also print from lua into the console if enabled.
/// Only run this in debug builds, I might add a ini option for this sometime.
/// </summary>
void KCMenu::AttachConsoleToScript()
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
        // If AllocConsole fails, try to attach to an existing console
        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            FILE* pCout;
            FILE* pCin;
            FILE* pCerr;
            freopen_s(&pCin, "CONIN$", "r", stdin);
            freopen_s(&pCout, "CONOUT$", "w", stdout);
            freopen_s(&pCerr, "CONOUT$", "w", stderr);
            LOG(DEBUG, "DEBUG: Attached to parent console.");
            std::cout << "DEBUG: Attached to parent console." << std::endl;
        }
        else {
            LOG(ERROR, "ERROR: Failed to allocate or attach console!");
            std::cerr << "ERROR: Failed to allocate or attach console!" << std::endl;
        }

        // Handle the case where console allocation fails (unlikely in most scenarios)
        // You might want to log an error message using your in-game system.
        //log_error("Failed to allocate console.");
        //LOG(ERROR, "Failed to allocate console.");
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

        // Init the script
        scriptInit();

        // Init lua
        //InitLua();

        initialized = true;

        // Set the mod path
        std::filesystem::path modPath = Paths::GetModPath();

        // Load notifications from the file.
        KCMenu::LoadNotificationFile();

#ifdef LOAD_TELEPORT_INI
    // Test for loading teleport locations from an ini
        
        //std::string teleportsFilePath = (modPath / "teleports.ini").string(); // Or a subfolder like "data/teleports.ini"
        std::string teleportsFilePath = (modPath / Constants::teleportFileName).string();
        TeleportManager::GetInstance().LoadTeleportsFromFile(teleportsFilePath);
#endif

#ifdef PLAYER_SKIN_CHANGER
        // Load the peds from the file list
        // For changing player model, and in the future spawning peds.
        KCMenu::LoadPedsFile();
#endif

        // Load the model names from the json file, for my new IDGun system.
#ifdef LOAD_MODEL_NAMES
        std::string objectsFilePath = (modPath / Constants::objectsFileName).string();
        LoadModelNamesFromJson(objectsFilePath);
#endif

        // TODO Test this, load weapon list from weapons.json in the mod directory
        std::string weaponsFilePath = (modPath / Constants::weaponsFileName).string();
        LoadWeaponsFromJson(weaponsFilePath);
        //


        // The Lua initialization is now done before scriptInit

    }
    else {
        LOG(INFO, "Script restarted");
    }

    scriptTick();
}


#pragma region LoadFiles

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



#ifdef LOAD_MODEL_NAMES

// Global map to store the model hash to name mapping
// Make this blank in here, this should be populated in the LoadModelNamesFromJson function.
std::map<Hash, std::string> KCMenu::g_modelNames = {}; // Assuming 'Hash' is defined as a suitable integer type (e.g., int, DWORD, etc.)

/// <summary>
/// Load the model names from JSON for the object list in the ID Gun.
/// </summary>
/// <param name="filePath"></param>
void KCMenu::LoadModelNamesFromJson(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        log_output(std::format("ERROR: Could not open model names JSON file: {}", filePath));
        return;
    }

    try {
        nlohmann::json j;
        file >> j;

        for (nlohmann::json::const_iterator it = j.begin(); it != j.end(); ++it) {
            try {
                // The keys in your JSON are string representations of hashes
                // Convert the string key to a Hash type (assuming it's an int or similar)
                Hash modelHash = std::stoul(it.key()); // Use stoul for unsigned long, or stoi for int
                std::string modelName = it.value().get<std::string>();
                g_modelNames[modelHash] = modelName;
            }
            catch (const std::exception& e) {
                log_output(std::format("WARNING: Error parsing JSON entry (key: {}, value: {}): {}", it.key(), it.value().dump(), e.what()));
            }
        }
        log_output(std::format("Successfully loaded {} model names from {}", g_modelNames.size(), filePath));

        // Test for displaying a random model
        // This works!!
        
        // Disabled, this was just an extra log value for a test,
        // searching through the objects.json file with a hash name
        //auto it = g_modelNames.find(-808157183);
        //if (it != g_modelNames.end())
        //{
        //    std::string entityModelString = std::format("Entity Model: {}", it->second);
        //    log_output(entityModelString);
        //}
        

    }
    catch (const nlohmann::json::exception& e) {
        log_output(std::format("ERROR: JSON parsing error in {}: {}", filePath, e.what()));
    }
    catch (const std::exception& e) {
        log_output(std::format("ERROR: General error loading model names from {}: {}", filePath, e.what()));
    }
}
#endif

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

// TODO Make WeaponScripts and add this into it.

// Global map to store categorized weapons
std::map<std::string, std::vector<WeaponInfo>> KCMenu::g_weaponCategories;

/// <summary>
/// Load weapons from the json file.
/// </summary>
void KCMenu::LoadWeaponsFromJson(const std::string& filename) {
    //LOG(DEBUG, "Attempting to load weapons from: {}", filename.c_str());

    std::ifstream file(filename);
    if (!file.is_open()) {
        LOG(ERROR, "Failed to open weapons file at {}", filename.c_str());
        return;
    }

    nlohmann::json weaponsData;
    try {
        file >> weaponsData;
        LOG(DEBUG, "SUCCESS: Parsed weapons.json.");
    }
    catch (const nlohmann::json::parse_error& e) {
        LOG(ERROR, "JSON parsing error in weapons.json: {}", e.what());
        return;
    }

    if (weaponsData.empty()) {
        LOG(WARN, "Parsed weapons.json data is empty.");
    }

    for (auto const& [categoryName, categoryValue] : weaponsData.items()) 
    {
        //LOG(DEBUG, "Processing category: {}", categoryName.c_str());
        std::vector<WeaponInfo> weaponsInThisCategory;
        if (categoryValue.is_object()) {
            for (auto const& [weaponInternalName, weaponHashStr] : categoryValue.items()) 
            { 
                // Use weaponInternalName here
                WeaponInfo weapon;
                weapon.internalName = weaponInternalName; // Store original
                weapon.displayName = Util::WeaponToDisplayName(weaponInternalName); // Generate display name
                weapon.hash = static_cast<Hash>(std::stoul(weaponHashStr.get<std::string>(), nullptr, 16));
                weaponsInThisCategory.push_back(weapon);
                //LOG(DEBUG, "    - Added weapon: '{}' (Display: '{}', Hash: 0x{})",
                //    weapon.internalName.c_str(), weapon.displayName.c_str(), weapon.hash);
            }
        }
        KCMenu::g_weaponCategories[categoryName] = weaponsInThisCategory;
        //LOG(DEBUG, std::format("Category '{}' added with {} weapons.", categoryName.c_str(), (int)weaponsInThisCategory.size()));
    }

    if (KCMenu::g_weaponCategories.empty()) {
        LOG(WARN, "g_weaponCategories map is empty after loading all data.");
    }
    else {
        LOG(INFO, std::format("g_weaponCategories successfully loaded {} categories.", (int)KCMenu::g_weaponCategories.size()));
    }
}

#pragma endregion //LoadFiles

/// <summary>
/// Script init
/// </summary>
void KCMenu::scriptInit() 
{
    // TODO Figure out how to add a reload option to the menu, I might use the scriptInit but this might crash the menu.
    const auto settingsMenuPath = Paths::GetModPath() / "settings_menu.ini";

    coreScript = std::make_shared<KCMainScript>();

    // Setup random, so random numbers work
    //srand(GetTickCount());
    srand(GetTickCount64());

    // Only run this in debug builds, I might add a ini option for this sometime.
#ifdef DEBUG_MODE
    // Attach the console to the game in scriptInit
    AttachConsoleToScript();
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
        // Didn't work here either.
//        AttachConsoleToScript();
////
//#ifdef LUA_TEST
//        InitLua();
//#endif

        LOG(DEBUG, "Thread 2 started");
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
