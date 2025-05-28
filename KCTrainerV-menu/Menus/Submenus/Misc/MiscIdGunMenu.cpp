#include "pch.h"

#include "MiscIdGunMenu.h"

#include "Constants.hpp"

#include "common.h"

#include "Util/Hash.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/PedScripts.h"

#include "Scripts/Stats.h"
#include "Scripts/MiscScripts.h"

// Misc
#include "Scripts/Misc/IDGun.h"

namespace Misc
{
	/// <summary>
	/// Build the misc Id Gun submenu.
	/// </summary>
	/// <param name="mbCtx"></param>
	/// <param name="context"></param>
	void IdGunMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
	{
            mbCtx.Title("ID Gun Debug");

            int nothing = 0;
            float stepValue = 0.001f;
            mbCtx.StringArray("--ID Gun Debug--", { "" }, nothing, { "Debug options for the IDGun postion display on screen." });

            //-----
            // ID Gun positions on the menu
            //-----

            // Entity ID Menu Pos X
            mbCtx.FloatOption("Entity ID X", MiscScripts::IDGun::entityIdMenuPosX, 0.f, 1.0f, stepValue);
            // Entity ID Menu Pos Y
            mbCtx.FloatOption("Entity ID Y", MiscScripts::IDGun::entityIdMenuPosY, 0.f, 1.0f, stepValue);

            // Entity Coords Menu Pos X
            mbCtx.FloatOption("Entity Coordinates X", MiscScripts::IDGun::entityCoordsMenuPosX, 0.f, 1.0f, stepValue);
            // Entity Coords Menu Pos Y
            mbCtx.FloatOption("Entity Coordinates Y", MiscScripts::IDGun::entityCoordsMenuPosY, 0.f, 1.0f, stepValue);

            // Entity heading Menu Pos X
            mbCtx.FloatOption("Entity Heading X", MiscScripts::IDGun::entityHeadingMenuPosX, 0.f, 1.0f, stepValue);
            // Entity heading Menu Pos Y
            mbCtx.FloatOption("Entity Heading Y", MiscScripts::IDGun::entityHeadingMenuPosY, 0.f, 1.0f, stepValue);

            // Entity model/Vehicle name Menu Pos X
            mbCtx.FloatOption("Entity Model/Vehicle Name X", MiscScripts::IDGun::entityModelMenuPosX, 0.f, 1.0f, stepValue);
            // Entity model/Vehicle name Menu Pos Y
            mbCtx.FloatOption("Entity Model/Vehicle Name Y", MiscScripts::IDGun::entityModelMenuPosY, 0.f, 1.0f, stepValue);

            if (mbCtx.Option("Reset to defaults", { "Reset the menu position back to defaults." }))
            {
                MiscScripts::IDGun::ResetIdGunPositions();
            }
        
	}
} // namespace Misc