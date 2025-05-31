#include "pch.h"

#include "MiscMarkerDebugMenu.h"

#include "Scripts/Player/PlayerTeleportScripts.h"

// Marker
#include "Scripts/Markers/MarkerScripts.h"

// Debug for the marker testing, move its position around, change the colors, and teleport to it.

namespace Misc
{
	/// <summary>
	/// Build the markers debug submenu.
	/// </summary>
	/// <param name="mbCtx"></param>
	/// <param name="context"></param>
	void MarkerDebugMenu::Build(NativeMenu::Menu& mbCtx, KCMainScript& context)
	{
		mbCtx.Title("Marker Debug");

		auto& playerTeleportScripts = Scripts::Player::Positions::GetInstance();
		auto& markerScripts = Scripts::Marker::GetInstance();

		// Toggle marker in the middle of LS test
		mbCtx.BoolOption("Toggle marker", markerScripts.markerEnabled, { "Test for marker in the middle of LS" });

		if (mbCtx.Option("Teleport to marker", {"Teleport to the marker coordinates"}))
		{
			// Offset this just a bit, in case the marker is in the ground.
			Vector3 markerTpPos = Vector3(markerScripts.markerX, markerScripts.markerY + 5, markerScripts.markerZ + 5);
			playerTeleportScripts.SetPlayerCoords(markerTpPos, 10.0f, true);
		}

		mbCtx.FloatOption("Marker X", markerScripts.markerX, -2000, 2000, 1);
		mbCtx.FloatOption("Marker Y", markerScripts.markerY, -2000, 2000, 1);
		mbCtx.FloatOption("Marker Z", markerScripts.markerZ, -2000, 2000, 1);

		mbCtx.IntOption("Color R", markerScripts.colorR, 0, 255, 1);
		mbCtx.IntOption("Color G", markerScripts.colorG, 0, 255, 1);
		mbCtx.IntOption("Color B", markerScripts.colorB, 0, 255, 1);
		mbCtx.IntOption("Color A", markerScripts.colorA, 0, 255, 1);

		if (mbCtx.Option("Reset colors", { "Reset colors to defaults" }))
		{
			// Marker colors
			markerScripts.colorR = 120;
			markerScripts.colorG = 255;
			markerScripts.colorB = 120;
			markerScripts.colorA = 155;
		}


	}
} // namespace Misc
