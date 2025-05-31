#include "pch.h"
#include "MarkerScripts.h"


#include "Constants.hpp"
#include "Util/Enums.h"

// Adapted most of this from my FiveM lua scripts.

namespace Scripts
{

	/// <summary>
	/// I have this now setup to draw a marker, and it can be moved with the debug menu.
	/// Now, to figure out how to draw multiple of these from a vector list, map, or something.
	/// Well this doesn't work, it makes the menu screen flicker but doesn't show up.
	/// </summary>
	//void MarkerScripts::Tick()
	void Marker::Tick()
	{
		// Add these just to be safe... Don't want to instantly crash and fight this thing lol.
		//WAIT(0);
		// FIB Building Los Santos
		// Middle of Los Santos marker #1 teleport to FIB Building
		//Vector3 markerPosition = Vector3(236.2, -874.9, 15.25f);
		//Vector3 markerPosition = Vector3(236.2, -874.9, 35.25f);
		//Vector3 scale = Vector3(2.0f, 2.0f, 2.0f);
		//int colorR = 120;
		//int colorG = 255;
		//int colorB = 120;
		//int colorA = 155;

		if (markerEnabled)
		{
			//WAIT(0);
			//{120, 255, 120, 155}
			Marker::Draw(static_cast<int>(MarkerType::VerticalCylinder), Vector3(markerX, markerY, markerZ), 
				scale, colorR, colorG, colorB, colorA);
		}
	}

	//void Draw(int markerType, Vector3 position, Vector3 direction, Vector3 rotation, )

	/// <summary>
	/// Draw a marker to the screen
	/// </summary>
	/// <param name="markerType">The marker type: https://docs.fivem.net/docs/game-references/markers/</param>
	/// <param name="position">Vector3 for the position</param>
	/// <param name="scale">Vector3 for the scale</param>
	/// RGBA colors below, List of colors for some of these:
	/// https://docs.fivem.net/docs/game-references/hud-colors/
	/// <param name="colorR"></param>
	/// <param name="colorG"></param>
	/// <param name="colorB"></param>
	/// <param name="colorA"></param>
	//void MarkerScripts::Draw(int markerType, Vector3 position, Vector3 scale,
	void Marker::Draw(int markerType, Vector3 position, Vector3 scale,
		int colorR, int colorG, int colorB, int colorA)
	{
		// TODO Test this format, I have nil,nil in lua for textureDict and textureName.
		Vector3 zeroVector = Vector3(0.0f, 0.0f, 0.0f);
		DRAW_MARKER(markerType, position, zeroVector, zeroVector, scale, colorR, colorG, colorB, colorA,
			false, true, 2, false, NULL, NULL, false);
	}

} // namespace Scripts