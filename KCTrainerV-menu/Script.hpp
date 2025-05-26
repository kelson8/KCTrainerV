#pragma once
#include "KCMenuScript.hpp"
#include "ScriptMenu.hpp"

#include <inc/types.h>
#include <string>
#include <map>

/*
 * Script.hpp exposes functions to initialize and manage the script.
 */
namespace KCMenu 
{
    // ScriptHookV calls this when starting the script.
    void ScriptMain();

    // Thread #2 test
    // TODO Test a second thread for this sometime.
    void Thread2();

    extern std::map<Hash, std::string> g_modelNames; // Assuming 'Hash' is defined as a suitable integer type (e.g., int, DWORD, etc.)
    /*
     * BuildMenu builds the menu with its options and submenus.
     * This function is declared here, so Script.cpp can call it
     * The implementation is in KCMenuMenu.cpp
     */
    std::vector<CScriptMenu<KCMainScript>::CSubmenu> BuildMenu();
}
