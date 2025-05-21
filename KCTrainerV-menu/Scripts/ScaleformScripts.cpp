#include "pch.h"

#include "ScaleformScripts.h"

#include "../Constants.hpp"
#include "../Scripts/MiscScripts.h"

#include "../common.h"

#include "../Teleports/TeleportLocations.h"

#include "defines.h"

// Menyoo
#include "../Scripts/Extras/Game.h"
#include "../Scripts/Extras/Classes/GTAblip.h"
#include "../Scripts/Extras/World.h"

// TODO Test this later, I originally created this in Menyoo, KCTestScripts.cpp.

namespace Scripts
{
	namespace Scaleforms
	{

		// These are converted from LUA using FiveM and my scripts:
		// https://github.com/kelson8/fivem-scripts/blob/main/kc_test/client/misc/scaleform_test.lua

		int scaleformHandle = 0;


		//------------ Loading bar scaleforms -------------//
		/// <summary>
		/// Setup the loading bar scaleform
		/// </summary>
		/// <param name="scaleformHandle">The scaleform handle, this gets set when the scaleform is shown such 
		/// as in ShowLoadingBarScaleform()</param>
		void LoadingBarScaleform(int scaleformHandle)
		{
			BEGIN_SCALEFORM_MOVIE_METHOD(scaleformHandle, "SET_PROGRESS_BAR"); // Function to call from the AS file.
			//PushScaleformMovieMethodParameterInt
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(50); // Set progress of loading bar
			END_SCALEFORM_MOVIE_METHOD();
		}


		/// <summary>
		/// Show the loading bar scaleform with a progress bar.
		/// </summary>
		void ShowLoadingBarScaleform()
		{
			const char* loadingScreenNewGameScaleform = "LOADINGSCREEN_NEWGAME";
			scaleformHandle = REQUEST_SCALEFORM_MOVIE(loadingScreenNewGameScaleform);
			LoadingBarScaleform(scaleformHandle);
		}

		//------------ End loading bar scaleforms -------------//

		//------------ Popup warning scaleforms -------------//
		// TODO Test this later, I don't know if it works.
		void PopupWarningScaleform(int scaleformHandle)
		{
			int miliseconds = 2000;
			const char* titleMsg = "Hello player";
			const char* warningMsg = "Warning";
			const char* promptMsg = "Continue?";
			bool showBackground = false;
			const char* alertType = "?";
			const char* errorMsg = "....";

			BEGIN_SCALEFORM_MOVIE_METHOD(scaleformHandle, "SHOW_POPUP_WARNING");
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(miliseconds); // Miliseconds
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(titleMsg); // titleMsg
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(warningMsg); // warningMsg
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(promptMsg); // promptMsg
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(showBackground); // showBackground
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(alertType); // alertType
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_TEXTURE_NAME_STRING(errorMsg); // errorMsg
			// Possibly missing this?
			END_SCALEFORM_MOVIE_METHOD();
		}

		void ShowPopupWarningScaleform()
		{
			const char* loadingScreenNewGameScaleform = "LOADINGSCREEN_NEWGAME";
			scaleformHandle = REQUEST_SCALEFORM_MOVIE(loadingScreenNewGameScaleform);
			LoadingBarScaleform(scaleformHandle);
		}

		//------------ End popup warning scaleforms -------------//

		//------------ Orbital Cannon scaleforms -------------//

		void OrbitalCannon1Scaleform(int scaleformHandle)
		{
			BEGIN_SCALEFORM_MOVIE_METHOD(scaleformHandle, "SET_ZOOM_LEVEL");
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(0.0f);
			END_SCALEFORM_MOVIE_METHOD();

			Cam orbitalCannonCam = CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", false);
			//SET_CAM_COORD(orbitalCannonCam, 2.0f, 2.0f, 2.0f);
			SET_CAM_COORD(orbitalCannonCam, Vector3(2.0f, 2.0f, 2.0f));
			SET_CAM_ACTIVE(orbitalCannonCam, true);

			BEGIN_SCALEFORM_MOVIE_METHOD(scaleformHandle, "SHOW_COOLDOWN_METER");

		}

		void ShowOrbitalCannon1Scaleform()
		{
			const char* orbitalCannonScaleform1 = "ORBITAL_CANNON_CAM";
			scaleformHandle = REQUEST_SCALEFORM_MOVIE(orbitalCannonScaleform1);
			OrbitalCannon1Scaleform(scaleformHandle);
		}
		//------------ End Orbital Cannon scaleforms -------------//

		// TODO Setup checkbox toggle in Menyoo somewhere under my test menu.
		bool drawScaleform = false;
		void DrawScaleform()
		{
			// I got this drawing to the screen, it can be turned on with the enable scaleform test button, and 
			// turned off with the disable scaleform test button.
			if (drawScaleform)
			{
				// Check if the scaleform has loaded, if so draw it to the screen.
				if (HAS_SCALEFORM_MOVIE_LOADED(scaleformHandle)) {
					DRAW_SCALEFORM_MOVIE_FULLSCREEN(scaleformHandle, 255, 255, 255, 255, 1);
				}

			}
		}

		/// <summary>
		/// This should run cleanup for the scaleforms like original scripts would.
		/// </summary>
		/// <param name="scaleformHandle"></param>
		void CleanupScaleform(int* scaleformHandle)
		{
			if (HAS_SCALEFORM_MOVIE_LOADED(*scaleformHandle))
			{
				SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(scaleformHandle);
			}
		}

		// TODO Possibly use this for a toggle checkbox in KCTestOptions, instead of modifying the boolean directly
		// TODO Come up with a better solution for getters and setters.

		/// <summary>
		/// Enable the drawing of the scaleform
		/// </summary>
		void SetScaleformDrawEnabled()
		{
			drawScaleform = true;
		}

		/// <summary>
		/// Disable the drawing of the scaleform
		/// </summary>
		void SetScaleformDrawDisabled()
		{
			drawScaleform = false;
		}

		// Scaleform toggle on/off

		// TODO Make this work with multiple of these scaleforms.
		/// <summary>
		/// Show the load bar scaleform, and set drawScaleform to true
		/// </summary>
		void EnableScaleform()
			//void EnableScaleform(int scaleformToLoad)
		{
			// TODO Add this into function as a parameter.

			//int scaleformToLoad = 1;

			//switch (scaleformToLoad) {
			//case 1:
			//	ShowLoadingBarScaleform();
			//	break;
			//case 2:
			//	ShowOrbitalCannon1Scaleform();
			//	break;
			//}
			// This is required to load the scaleform.
			ShowLoadingBarScaleform();
			drawScaleform = true;
		}

		/// <summary>
		/// Disable the load bar scaleform, cleanup, and set drawScaleform to false.
		/// </summary>
		void DisableScaleform()
		{
			CleanupScaleform(&scaleformHandle);
			drawScaleform = false;
		}
	}
}