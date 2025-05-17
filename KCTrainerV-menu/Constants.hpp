#pragma once
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

// This can enable/disable the tests in Memory.cpp, this is untested and may crash.
// If this crashes, just disable it here.
#define MEMORY_TESTING

// Enable debug logging and other debug features.
// TODO Move into preprocessors in solution.
#define KCTRAINER_DEBUG

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
#define LOAD_IPLS

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
//#define LUA_TEST

// Enables new features I am testing from the Chaos mod
// Well some of the vehicle features seem to crash this.
//#define CHAOSMOD_FEATURES

// Enable extras from Menyoo, mostly the files from the Scripting folder which can be very useful
// TODO Fix these to work, some of them need quite a bit of work.
// Also Vector3 is overridden in one of the Menyoo files, or I would've continued messing with this.
//#define MENYOO_SCRIPT_FILES

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
