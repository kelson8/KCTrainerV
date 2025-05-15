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
#define KCTRAINERDEBUG

// Enable extra debugging features for my menu, such as moving around the Y coords for drawing to the screen.
// Disable in release builds.
#define DEBUG_MODE

// Enable lua test using SOL from the Chaos Mod, this is a very early work in progress
// and copied straight from the chaos mod, it will need quite a bit of work but I should be able to make it work on here.
//#define CHAOSMOD_LUA_TEST

namespace Constants 
{
    static const char* const ScriptName = "KCTrainerV";
    static const char* const NotificationPrefix = "~b~KCTrainerV~w~";
    static const char* const DisplayVersion = "v" STR(VERSION_MAJOR) "."  STR(VERSION_MINOR) "." STR(VERSION_PATCH);

    // This is to create %localappdata%/authorFolder/GTAVMenuExample if for some reason
    // the GTA V folder is not read/writeable for the script.
    static const char* const authorFolder = "kelson8";
    static const char* const ScriptFolder = "KCTrainerV";
}
