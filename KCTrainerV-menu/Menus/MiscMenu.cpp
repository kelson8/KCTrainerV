#include "pch.h"

#include "../Constants.hpp"
#include "MiscMenu.h"



#include "../common.h"

#include "../Teleports/TeleportLocations.h"

#include "defines.h"


// My scripts
#include "Util/FileFunctions.h"
#include "Scripts/MiscScripts.h"
#include "Scripts/NotificationManager.h"
#include "Scripts/TextScripts.h"

/// <summary>
/// Misc Menu - Main Menu
/// </summary>
/// <param name="mbCtx"></param>
/// <param name="context"></param>
void MiscMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
{
    auto& fileFunctions = FileFunctions::GetInstance();
    auto& textScripts = TextScripts::GetInstance();

    mbCtx.Title("Misc");

    // Loading/Unloading multiplayer map

    //These were in MPMenu, adding to my menu I'll probably just remove MPMenu.
    if (mbCtx.Option("Load MP Maps", { "Enable Multiplayer maps" }))
    {
        ON_ENTER_MP();
    }

    if (mbCtx.Option("Unload MP Maps", { "Disable Multiplayer maps" }))
    {
        ON_ENTER_SP();
    }

    // Sub menus

    mbCtx.MenuOption("Music", { "MiscMusicSubmenu" }, { "Play game soundtracks, and music, doesn't work for radio." });

    // Doors sub menu
    // TODO Setup sub menu for this.
    //if (mbCtx.Option("Unlock Casino Door #1"))
    //{
    //    // Well this didn't work
    //    // TODO Figure out how to this, I have it working in FiveM
    //    
    //    // The doors were already unlocked, maybe I could test locking them?
    //    
    //    // I might need to use this, I am doing it in FiveM also.
    //    //ADD_DOOR_TO_SYSTEM()
    //    DOOR_SYSTEM_SET_DOOR_STATE(680601509, static_cast<int>(DoorLockStates::UNLOCKED), false, false);
    //}
    

#ifdef BLIP_TEST
    // TODO Fix this
    mbCtx.MenuOption("Blips", { "MiscBlipsSubmenu" }, { "Testing with blips, adding/removing and more." });
#endif // BLIP_TEST

    mbCtx.MenuOption("Debug Sub Menu", "MiscDebugSubmenu", { "Debug testing menu." });

#ifdef DEBUG_MODE
    mbCtx.MenuOption("IDGun Debug", "MiscIDGunDebugSubmenu", { "IDGun debug menu." });
    mbCtx.MenuOption("Globals Sub Menu", "MiscGlobalsSubmenu", { "Submenu for testing editing game globals" });
#endif

}

