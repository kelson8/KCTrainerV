/*
 * DllMain.cpp hosts the DllMain function, which is the entry point for the DLL.
 * Initialization happens here.
 * In this example, it initializes the logger and paths, and registers the main
 * script function to ScriptHookV using scriptRegister.
 */

#include "pch.h"

#include "Script.hpp"
#include "Constants.hpp"
#include "Util/Logger.hpp"
#include "Util/Paths.hpp"

#include <inc/main.h>
#include <filesystem>

#include <iostream>

#include "Scripts/MiscScripts.h"

#include "defines.h"

// For MinHook and Memory functions for my menu.
#ifdef MEMORY_TESTING
#include "Memory/Memory.h"
#endif

HMODULE MainModule = 0;
MODULEINFO MainModuleInfo = { 0 };

namespace fs = std::filesystem;

/// <summary>
/// Runs a console window for cout and other errors to be displayed
/// Also print from lua into the console if enabled.
/// This is now enabled in Script.cpp, keeping here in case I figure out how to use it in the DllMain
/// </summary>
//void AttachConsole()
//{
//#ifdef _WIN32
//    if (AllocConsole())
//    {
//        // Redirect standard output, error, and input streams to the console
//        FILE* dummy;
//        freopen_s(&dummy, "CONOUT$", "w", stdout);
//        freopen_s(&dummy, "CONOUT$", "w", stderr);
//        freopen_s(&dummy, "CONIN$", "r", stdin);
//
//        // Optional: Set the console title
//        SetConsoleTitle("KCTrainerV");
//
//        std::cout << "Console attached successfully!" << std::endl;
//        //std::cerr << "Error output will also appear here." << std::endl;
//    }
//    else
//    {
//        // Handle the case where console allocation fails (unlikely in most scenarios)
//        // You might want to log an error message using your in-game system.
//        std::cerr << "Failed to allocate console." << std::endl;
//    }
//#else
//    std::cerr << "AllocConsole() is only available on Windows." << std::endl;
//#endif
//}

/*
 * As the game is often installed in a default directory, like
 * C:\Program Files (x86)\Steam\steamapps\common\Grand Theft Auto V
 * Windows permissions may not allow scripts to write there.
 * This function detects if that happens and copies the files it needs to
 * %localappdata%\{author}\{scriptname}.
 * 
 * Use Paths::GetModPath() to ensure the relocated path is used, instead of the
 * GTA V directory.
 */
void initializePaths(HMODULE hInstance) 
{
    Paths::SetOurModuleHandle(hInstance);

    auto localAppDataPath = Paths::GetLocalAppDataPath();
    //auto localAppDataModPath = localAppDataPath / Constants::iktFolder / Constants::ScriptFolder;
    auto localAppDataModPath = localAppDataPath / Constants::authorFolder / Constants::ScriptFolder;
    auto originalModPath = Paths::GetModuleFolder(hInstance) / Constants::ScriptFolder;
    Paths::SetModPath(originalModPath);

    bool useAltModPath = false;
    if (fs::exists(localAppDataModPath)) 
    {
        useAltModPath = true;
    }

    fs::path modPath;
    fs::path logFile;

    // Use LocalAppData if it already exists.
    if (useAltModPath) 
    {
        modPath = localAppDataModPath;
        logFile = localAppDataModPath / (Paths::GetModuleNameWithoutExtension(hInstance) + ".log");
    }
    else {
        modPath = originalModPath;
        logFile = modPath / (Paths::GetModuleNameWithoutExtension(hInstance) + ".log");
    }

    Paths::SetModPath(modPath);

    if (!fs::is_directory(modPath) || !fs::exists(modPath)) 
    {
        fs::create_directories(modPath);
    }

    g_Logger.SetFile(logFile.string());
    g_Logger.Clear();

    if (g_Logger.Error()) 
    {
        modPath = localAppDataModPath;
        logFile = localAppDataModPath / (Paths::GetModuleNameWithoutExtension(hInstance) + ".log");
        fs::create_directories(modPath);

        Paths::SetModPath(modPath);
        g_Logger.SetFile(logFile.string());

        fs::copy(originalModPath, localAppDataModPath,
            fs::copy_options::update_existing | fs::copy_options::recursive);

        std::vector<std::string> messages;

        // Fix perms
        for (auto& path : fs::recursive_directory_iterator(localAppDataModPath)) 
        {
            try {
                fs::permissions(path, fs::perms::all);
            }
            catch (std::exception& e) {
                messages.push_back(
                    std::format("Failed to set permissions on [{}]: {}",
                        path.path().string(), e.what()));
            }
        }

        g_Logger.ClearError();
        g_Logger.Clear();

        LOG(WARN, "Copied to [{}] from [{}] due to read/write issues.",
            modPath.string(), originalModPath.string());

        if (!messages.empty()) {
            LOG(WARN, "Encountered issues while updating permissions:");
            for (const auto& message : messages) {
                LOG(WARN, "{}", message);
            }
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved) 
{
    switch (reason) {
        case DLL_PROCESS_ATTACH: 
        {
            // Display the program and module handle in the console
            /* Example:
            [KCTrainerV]: MODULEINFO: lpBaseofDll=00007FF7AE8D0000, SizeOfImage=67644416, EntryPoint=00007FF7B0134A9C
            */
            MainModule = GetModuleHandle(NULL);
            if (!GetModuleInformation(GetCurrentProcess(), MainModule, &MainModuleInfo, sizeof(MainModuleInfo)))
                log_output("Unable to get MODULEINFO from GTA5.exe");
                //addlog(ige::LogType::LOG_INIT, "Unable to get MODULEINFO from GTA5.exe", __FILENAME__);
            else
            {
                std::ostringstream moduleinfostream;
                moduleinfostream << "MODULEINFO: lpBaseofDll=" << MainModuleInfo.lpBaseOfDll
                    << ", SizeOfImage=" << MainModuleInfo.SizeOfImage
                    << ", EntryPoint=" << MainModuleInfo.EntryPoint;

                log_output(moduleinfostream.str());

                //addlog(ige::LogType::LOG_INIT, moduleinfostream.str(), __FILENAME__);
            }

            //

            g_Logger.SetMinLevel(DEBUG);
            initializePaths(hInstance);
            // Log the script name, and when it was built
            LOG(INFO, "{} {} (built {} {})", Constants::ScriptName, Constants::DisplayVersion, __DATE__, __TIME__);
            // Log the data path
            LOG(INFO, "Data path: {}", Paths::GetModPath().string());

#ifdef MEMORY_TESTING
            // This seems to work now without crashing.
            Memory::Init();
#endif

            // Attach the console to the game before scriptRegister, may crash it here.
            // This is being run in Script.cpp, I may try to run it in here if I can get it working.
            //AttachConsole();
            scriptRegister(hInstance, KCMenu::ScriptMain);
            // Run the second thread, mostly just for fading the screen for now.
            // Disabled, I fixed the WAIT function, this can be used for later.
            //scriptRegisterAdditionalThread(hInstance, KCMenu::Thread2);
            LOG(INFO, "Script registered");
            break;
        }

        // TODO Why does this give an error when exiting the game? I think it's this.
        // Gives this error:
        // SCRIPT HOOK V CRITICAL ERROR
        // FATAL: Trying to unregister unk script using module handle <memoryHandle>
        case DLL_PROCESS_DETACH: 
        {
            // TODO Set this to disable never wanted and everything when the menu is reloaded.

#ifdef MEMORY_TESTING
            // Disable MinHook and Memory features on script exit.
            Memory::Uninit();
#endif

            scriptUnregister(hInstance);
            break;
        }
        default: 
        {
            break;
        }
    }
    return TRUE;
}
