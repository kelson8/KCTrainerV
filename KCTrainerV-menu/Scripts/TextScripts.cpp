#include "TextScripts.h"

#include "Util/UI.hpp"

#include <inc/natives.h>
#include <format>

// When I get this working, I should be able to draw stuff to the screen.

// Lua example from FiveM for usage:
// TODO Add this to the main loop if I turn on a debug text such as how many cops I killed to show on screen.
/*
CreateThread(function()
	local player = GetPlayerPed(-1)

	-- while debugMode do
	while DebugConfig.debugMode do
		-- while true do
		Wait(1)

		if DebugConfig.aimCamTest then
			setTextEntry()
			BeginTextCommandDisplayText("STRING")
			-- textPositionOne()
			-- EndTextCommandDisplayText(0.0001, 0.0200, 0)
			if IsAimCamActive() then
				AddTextComponentSubstringPlayerName("Is Aiming: True")
				-- AddTextComponentSubstringPlayerName(("Is Aiming: %s"):format(IsAimCamActive()))
				textPositionOne()
			else
				AddTextComponentSubstringPlayerName("Is Aiming: False")
				textPositionOne()
			end
*/

/// <summary>
/// This should setup the text entry for drawing to the screen.
/// This will need to be ran before drawing anything to the screen.
/// Adapted from my FiveM scripts originally in lua.
/// </summary>
void TextScripts::SetTextEntry(const char* text)
{
	int colorR = 200;
	int colorG = 88;
	int colorB = 100;
	int colorA = 255;

	// This works like this, I think the previous way I was doing it was incorrect.
	// It was making the menu dissappear but now this works
	
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	
	// Extras
	HUD::SET_TEXT_PROPORTIONAL(true);
	HUD::SET_TEXT_JUSTIFICATION(0);
	//

	HUD::SET_TEXT_FONT(0); // 0 - 4
	HUD::SET_TEXT_SCALE(0.3, 0.3);
	HUD::SET_TEXT_COLOUR(colorR, colorG, colorB, colorA);
	//HUD::BEGIN_TEXT_COMMAND_PRINT("STRING");

	//HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
}

/// <summary>
/// Set the text position with a fixed x value
/// Adapted from my FiveM scripts
/// </summary>
/// <param name="y"></param>
void TextScripts::TextPosition(float y)
{
	float x = 0.1f;
	HUD::END_TEXT_COMMAND_DISPLAY_TEXT(Vector2(x, y), 0);
}

/// <summary>
/// Set the text position using the above function
/// </summary>
void TextScripts::SetTextPosition()
{
	float y = .1f;
	//TextPosition(0.0200f);
	TextPosition(y - .0125f);
}

/// <summary>
/// Display text, not in use I moved this into the SetTextEntry function.
/// </summary>
/// <param name="text"></param>
void TextScripts::DisplayText(const char* text)
{
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
}

/// <summary>
/// Setup the text.
/// </summary>
void TextScripts::SetupText()
{
	//if (this->drawText)
	//{
		SetTextEntry("Test");

		// TODO Test this here
		//HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");

		//DisplayText("Test");

		SetTextPosition();
	//}

}
