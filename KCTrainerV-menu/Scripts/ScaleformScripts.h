#pragma once
namespace Scripts
{
	namespace Scaleforms
	{
		//bool drawScaleform;

		void LoadingBarScaleform(int scaleformHandle);
		void ShowLoadingBarScaleform();
		void DrawScaleform();

		// TODO Fix this to work, this complains with a parameter in KCTestOptions.
		// I probably need to use something other then AddOption for the menu.
		void EnableScaleform();
		//void EnableScaleform(int scaleformToLoad);
		void DisableScaleform();
	} // namespace Scaleforms
	
} // namespace Scripts

