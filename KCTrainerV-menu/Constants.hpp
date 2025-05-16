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
