#include "TextScripts.h"

#include "Scripts/PlayerScripts.h"

#include "Util/UI.hpp"

#include <iostream>
#include <iomanip> // For std::fixed and std::setprecision
#include <format>
#include <string>

#include <inc/natives.h>

// When I get this working, I should be able to draw stuff to the screen.

// To use this (This needs to be run in a loop):
// 1. Set the text: std::string textToDisplay = "Test Text";
// 2. Set the text entry: SetTextEntry(textToDisplay.c_str());
// 3. Set the text position (This is in the bottom left by the map): TextPosition(0.193f, 0.951f - .0525f);


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
/// This function sets the colors mostly to red.
/// </summary>
void TextScripts::SetTextEntry(const char* text)
{
	int colorR = 200;
	int colorG = 88;
	int colorB = 100;
	int colorA = 255;

	// This works like this, I think the previous way I was doing it was incorrect.
	// It was making the menu disappear but now this works
	
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
/// This should setup the text entry for drawing to the screen.
/// This will need to be ran before drawing anything to the screen.
/// Adapted from my FiveM scripts originally in lua.
/// This version of the function takes a colorR, colorG, colorB, and colorA for the colors.
/// </summary>
void TextScripts::SetTextEntry(const char* text, int colorR, int colorG, int colorB, int colorA)
{
	// This works like this, I think the previous way I was doing it was incorrect.
	// It was making the menu disappear but now this works

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
/// Set the text position with a fixed x value and custom y value
/// Adapted from my FiveM scripts
/// </summary>
/// <param name="y"></param>
void TextScripts::TextPosition(float y)
{
	float x = 0.1f;
	HUD::END_TEXT_COMMAND_DISPLAY_TEXT(Vector2(x, y), 0);
}

/// <summary>
/// Set the text position with a custom x and y value
/// TODO Test this, it shouldn't conflict with the other method.
/// Adapted from my FiveM scripts
/// </summary>
/// <param name="y"></param>
void TextScripts::TextPosition(float x, float y)
{
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

/// <summary>
/// Display the player coords, for now I'm testing with just the X value.
/// </summary>
void TextScripts::DisplayCoordinates()
{
	auto& playerScripts = PlayerScripts::GetInstance();
	// playerScripts.GetPlayerCoords();
	// playerScripts.GetPlayerHeading

	Vector3 playerCoords = playerScripts.GetPlayerCoords();
	//float playerX = playerCoords.x;
	//float playerY = playerCoords.y;
	//float playerZ = playerCoords.z;

	float playerX = playerCoords.x;
	float playerY = playerCoords.y;
	float playerZ = playerCoords.z;


	float playerHeading = playerScripts.GetPlayerHeading();

	// I got this to display in the bottom left beside the map, 
	// I had to use my new debug feature that I made to do it in the menu.
	
	// Well I had to reverse the TextPosition values for the X and Z, I think it's because I am subtracting it?
	// Oh well this seems to work now.
	// This works! Now time to finish this.
	// 
	// The float y values are able to be changed if needed, but I mostly change the value to subtract it by.
	// I used {:.2f} to strip the decimals down to 2, makes this a bit less annoying for displaying on the screen.

	// TODO Is there any good reason why I am still subtracting the Y values in here? 
	// Although this works fine now so I'll leave it alone.

	// X coord
	//std::string playerXString = std::format("X: {}", playerX);
	std::string playerXString = std::format("X: {:.2f}", playerX);
	SetTextEntry(playerXString.c_str());
	
	// Overriding my SetTextPosition function, it isn't needed for this.
	//float y1 = .1f;
	//y1 = .1f;
	//TextPosition(y1 - .0125f);
	//TextPosition(y1 - .0525f);
	TextPosition(playerXMenuPosX, playerXMenuPosY - .0525f);

	// Y coord
	//std::string playerYString = std::format("Y: {}", playerY);
	std::string playerYString = std::format("Y: {:.2f}", playerY);
	SetTextEntry(playerYString.c_str());

	//float y2 = .1f;
	//y2 = .1f;
	//TextPosition(y1 - .0325f);
	TextPosition(playerYMenuPosX, playerYMenuPosY - .0325f);

	// Z coord
	//std::string playerZString = std::format("Z: {}", playerZ);
	std::string playerZString = std::format("Z: {:.2f}", playerZ);
	SetTextEntry(playerZString.c_str());

	//float y3 = .1f;
	//y3 = .1f;
	//TextPosition(y1 - .0525f);
	TextPosition(playerZMenuPosX, playerZMenuPosY - .0125f);

	// Heading
	//std::string playerHeadingString = std::format("Heading: {}", playerHeading);
	std::string playerHeadingString = std::format("Heading: {:.2f}", playerHeading);
	SetTextEntry(playerHeadingString.c_str());

	//float y4 = .155f;
	//y4 = .155f;
	//TextPosition(y4 - .0125f);

	// TODO Test this
	//TextPosition(x4, y4 - .0125f);
	TextPosition(headingMenuPosX, headingMenuPosY - .0125f);



}
