#pragma once
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VERSION_MAJOR 0
#define VERSION_MINOR 5
#define VERSION_PATCH 0

// This can enable/disable the tests in Memory.cpp, this is untested and may crash.
// If this crashes, just disable it here.
#define MEMORY_TESTING

// Enable debug logging and other debug features.
// Moved into preprocessors in solution.
//#define KCTRAINER_DEBUG

// Enable extra debugging features for my menu, such as moving around the Y coords for drawing to the screen.
// Disable in release builds.
// I have moved this preprocessor into the project solution.
// So now it'll automatically switch on if this project is built with debug, it'll disable if built in release.
// Keeping here for future reference and so I don't forget how I've done this.
//#define DEBUG_MODE

// Enable lua test using SOL from the Chaos Mod, this is a very early work in progress
// and copied straight from the chaos mod, it will need quite a bit of work but I should be able to make it work on here.
// Don't enable this anymore, I have switched to using the LUA_TEST preprocessor below.
//#define CHAOSMOD_LUA_TEST

// This will enable teleports that will use IPLs such as multiplayer locations and other singleplayer locations.
// I will need to set this up and adapt items from Menyoo but I should be able to figure it out.
//#define LOAD_IPLS

// Enable extra logging for IPLs, such as unloading, possibly loading in the future
// TODO Disable in release builds, this isn't very useful unless debugging.
#if defined(LOAD_IPLS) && defined(KCTRAINER_DEBUG)
#define IPL_LOGGING
#endif // LOAD_IPLS

// Enable my new teleport format with categories, this might be very broken.
// Sometimes this crashes it, I might deal with it later.
//#define NEW_TELEPORTS

// Enable some extra chaos features, I will toggle these off if it breaks something.
#define EXTRA_FEATURES

// Enable new lua test
// I will try to add the full list of natives for this like the Chaos mod is doing.
// Instead of trying to replicate the full file.
// I moved this into a separate Visual Studio profile named Debug_Lua, so I can also keep the library disabled
// Keeping here for a reference.
//#define LUA_TEST

// Enables new features I am testing from the Chaos mod
// Well some of the vehicle features seem to crash this.
//#define CHAOSMOD_FEATURES

// Enable extras from Menyoo, mostly the files from the Scripting folder which can be very useful
// TODO Fix these to work, some of them need quite a bit of work.
// Also Vector3 is overridden in one of the Menyoo files, or I would've continued messing with this.
//#define MENYOO_SCRIPT_FILES

// Add test for reloading the config, this needs completed
//#define RELOAD_CONFIG

// TODO Test this, I plan on moving the menus into their own files for better organization.
#define MOVE_PLAYER_MENU

// Move the rest of the menus, I will remove this once I have tested it more.
// This is no longer in use for now, I may use it in the future if I move more menus around
//#define MOVE_MENUS

// If this is on it enables the singleton in VehicleScripts.h
#define VEHICLE_SCRIPTS_SINGLETON

// This toggles my tick events running such as god mode and never wanted, can be turned off for testing.
#define RUN_TICKS

// Enable the Meteor test weather effect from Menyoo, this needs worked on and refactored.
//#define METEOR_SHOWER_TEST

// Tests for refactoring and reorganzing.
// Rename this preprocessor to whatever I am moving around.
// This method of moving code can help with testing.
//#define MOVE_MENU
#define MOVE_PED_MENU

// Enable the vehicle spawner test which runs in the second thread
//#define VEHICLE_SPAWNER_TEST


// Testing moving some stuff over to namespaces, 
// #define MOVE_MENU_NAMESPACE

namespace Constants 
{
    static const char* const ScriptName = "KCTrainerV";
    static const char* const NotificationPrefix = "~b~KCTrainerV~w~";
    static const char* const DisplayVersion = "v" STR(VERSION_MAJOR) "."  STR(VERSION_MINOR) "." STR(VERSION_PATCH);

    // This is to create %localappdata%/authorFolder/KCTrainerV if for some reason
    // the GTA V folder is not read/writeable for the script.
    static const char* const authorFolder = "kelson8";
    static const char* const ScriptFolder = "KCTrainerV";

    static const char* const coordsFileName = "CurrentCoords.txt";
}
