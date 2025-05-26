#pragma once
#include "KCMenuScript.hpp"
#include "ScriptMenu.hpp"

#include <inc/types.h>
#include <string>
#include <map>

// TODO Make WeaponScripts and add this into it.
// Structure to hold weapon information
struct WeaponInfo {
    std::string internalName; // Store the original JSON key, useful for debugging
    //std::string name; // The name to show in the menu
    std::string displayName; // The name to show in the menu
    Hash hash; // The weapon hash
};

/*
 * Script.hpp exposes functions to initialize and manage the script.
 */
namespace KCMenu 
{
    // ScriptHookV calls this when starting the script.
    void ScriptMain();

    // Thread #2 test
    // TODO Use this second thread for something
    void Thread2();

    extern std::map<Hash, std::string> g_modelNames; // Assuming 'Hash' is defined as a suitable integer type (e.g., int, DWORD, etc.)
    
    extern std::map<std::string, std::vector<WeaponInfo>> g_weaponCategories;

    void ResetToggles();
    /*
     * BuildMenu builds the menu with its options and submenus.
     * This function is declared here, so Script.cpp can call it
     * The implementation is in KCMenuMenu.cpp
     */
    std::vector<CScriptMenu<KCMainScript>::CSubmenu> BuildMenu();
}
