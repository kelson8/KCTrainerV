#include "pch.h"
#include "TextScripts.h"

// Player
#include "Scripts/PlayerScripts.h"
#include "Scripts/Player/PlayerTeleportScripts.h"

#include "Util/UI.hpp"

#include <iostream>
#include <iomanip> // For std::fixed and std::setprecision
#include <format>
#include <string>

// This class can draw text to the screen, and also can accept custom color input for the text.

// To use this (This needs to be run in a loop):
// 1. Set the text: std::string textToDisplay = "Test Text";
// 2. Set the text entry: SetTextEntry(textToDisplay.c_str());
// 3. Set the text position (This is in the bottom left by the map): TextPosition(0.160f, 0.882f);

// Lua example from FiveM for usage:
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
/// Main tick event for TextScripts
/// </summary>
void TextScripts::Tick()
{
	//-----
	// Display text on screen
	// Removed this function because all it did was draw 'Test' to the screen.
	//-----
	//if (TextScripts::drawText)
	//{
	//	TextScripts::SetupText();
	//}

	//-----
	// Draw coordinates to the screen
	//-----
	if (TextScripts::drawCoords)
	{
		// Will this slow down the whole menu?
		// Oops, this wait breaks the menu..
		// TODO, figure out how to slow down the coordinates display, I guess it's fine for now.
		//WAIT(100);
		TextScripts::DisplayCoordinates();
	}
}

/// <summary>
/// Setup the text without color, the colors will need to be set.
/// This will run some setup for the font, scale and other
/// Common used values in my functions.
/// </summary>
void TextScripts::SetupTextOptions()
{
	// Extras
	HUD::SET_TEXT_PROPORTIONAL(true);
	// Center justify
	//HUD::SET_TEXT_JUSTIFICATION(0);
	// Left justify
	HUD::SET_TEXT_JUSTIFICATION(1);
	// Right justify
	//HUD::SET_TEXT_JUSTIFICATION(2);

	//SET_TEXT_DROP_SHADOW();
	// I think this makes the text look a bit better
	SET_TEXT_OUTLINE();
	//

	
	//HUD::SET_TEXT_FONT(0); // 0 - 4
	HUD::SET_TEXT_FONT(GTAfont::Arial); // 0 - 4
	//HUD::SET_TEXT_SCALE(0.3f, 0.3f);
	HUD::SET_TEXT_SCALE(0.0f, 0.3f);
}

/// <summary>
/// Setup the text with color
/// This will run some setup for the font, scale and other
/// Common used values in my functions.
/// <param name="colorR">Red color</param>
/// <param name="colorG">Green color</param>
/// <param name="colorB">Blue color</param>
/// <param name="colorA">Alpha for the color</param>
/// <param name="textProportional">Set this to true or false</param>
/// <param name="textJustify">Set with my TextJustifiy enum, either CENTER, LEFT, or RIGHT </param>
/// </summary>
void TextScripts::SetupTextOptions(int colorR, int colorG, int colorB, int colorA, bool textProportional, TextJustify textJustify)
{
	HUD::SET_TEXT_PROPORTIONAL(textProportional);

	HUD::SET_TEXT_JUSTIFICATION(static_cast<int>(textJustify));

	// Not sure what this one does.
	//SET_TEXT_DROP_SHADOW();
	// I think this makes the text look a bit better
	SET_TEXT_OUTLINE();
	//

	HUD::SET_TEXT_FONT(GTAfont::Arial); // 0 - 4
	//HUD::SET_TEXT_SCALE(0.3f, 0.3f);
	HUD::SET_TEXT_SCALE(0.0f, 0.3f);

	HUD::SET_TEXT_COLOUR(colorR, colorG, colorB, colorA);
}

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
	
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);

	TextScripts::SetupTextOptions(colorR, colorG, colorB, colorA, true, TextJustify::LEFT);
}

/// <summary>
/// This should setup the text entry for drawing to the screen.
/// This will need to be ran before drawing anything to the screen.
/// Adapted from my FiveM scripts originally in lua.
/// This version of the function takes a colorR, colorG, colorB, and colorA for the colors.
/// </summary>
void TextScripts::SetTextEntry(const char* text, int colorR, int colorG, int colorB, int colorA)
{
	HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);

	TextScripts::SetupTextOptions(colorR, colorG, colorB, colorA, true, TextJustify::LEFT);
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
/// Adapted from my FiveM scripts
/// </summary>
/// <param name="y"></param>
void TextScripts::TextPosition(float x, float y)
{
	HUD::END_TEXT_COMMAND_DISPLAY_TEXT(Vector2(x, y), 0);
}

/// <summary>
/// Display the player coords, for now I'm testing with just the X value.
/// </summary>
void TextScripts::DisplayCoordinates()
{
	auto& playerScripts = PlayerScripts::GetInstance();
	auto& playerTeleportScripts = Scripts::Player::Positions::GetInstance();
	// playerScripts.GetPlayerCoords();
	// playerScripts.GetPlayerHeading

#ifndef MOVE_PLAYER_TELEPORTS
	Vector3 playerCoords = playerScripts.GetPlayerCoords();
#else
	Vector3 playerCoords = playerTeleportScripts.GetPlayerCoords();
#endif

	//float playerX = playerCoords.x;
	//float playerY = playerCoords.y;
	//float playerZ = playerCoords.z;

	float playerX = playerCoords.x;
	float playerY = playerCoords.y;
	float playerZ = playerCoords.z;


#ifndef MOVE_PLAYER_TELEPORTS
	float playerHeading = playerScripts.GetPlayerHeading();
#else
	float playerHeading = playerTeleportScripts.GetPlayerHeading();
#endif

	// I got this to display in the bottom left beside the map, 
	// I had to use my new debug feature that I made to do it in the menu.
	
	// Well I had to reverse the TextPosition values for the X and Z, I think it's because I am subtracting it?
	// Oh well this seems to work now.
	// This works! Now time to finish this.
	// 
	// The float y values are able to be changed if needed, but I mostly change the value to subtract it by.
	// I used {:.2f} to strip the decimals down to 2, makes this a bit less annoying for displaying on the screen.

	//-----
	// X coord
	//-----
	//std::string playerXString = std::format("X: {}", playerX);
	std::string playerXString = std::format("X: {:.2f}", playerX);
	SetTextEntry(playerXString.c_str());
	
	// Overriding my SetTextPosition function, it isn't needed for this.
	TextPosition(playerXMenuPosX, playerXMenuPosY);

	//-----
	// Y coord
	//-----
	//std::string playerYString = std::format("Y: {}", playerY);
	std::string playerYString = std::format("Y: {:.2f}", playerY);
	SetTextEntry(playerYString.c_str());

	TextPosition(playerYMenuPosX, playerYMenuPosY);

	//-----
	// Z coord
	//-----
	//std::string playerZString = std::format("Z: {}", playerZ);
	std::string playerZString = std::format("Z: {:.2f}", playerZ);
	SetTextEntry(playerZString.c_str());

	TextPosition(playerZMenuPosX, playerZMenuPosY);

	//-----
	// Heading
	//-----
	//std::string playerHeadingString = std::format("Heading: {}", playerHeading);
	std::string playerHeadingString = std::format("Heading: {:.2f}", playerHeading);
	SetTextEntry(playerHeadingString.c_str());

	TextPosition(headingMenuPosX, headingMenuPosY);
}


//----------- Notifications ------------/


// TODO Fix this
// Adapted from Menyoo, for the long string function
//void add_text_component_long_string(const std::string& text)
//{
//	const UINT8 maxStrComponentLength = 99;
//	for (int i = 0; i < text.length(); i += maxStrComponentLength)
//	{
//		const std::string& strComp = text.substr(i, std::min(text.length() - i, maxStrComponentLength));
//		ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(strComp.c_str());
//	}
//}

// Adapted from Menyoo, Print to bottom left and play phone sound
void TextScripts::NotificationBottomLeft(std::string notificationString, bool gxt)
{
	const char* text = notificationString.c_str();

	if (gxt && DOES_TEXT_LABEL_EXIST(text))
		BEGIN_TEXT_COMMAND_THEFEED_POST(text);
	else
	{
		if (notificationString.length() < 100)
		{
			BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
			ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		}
		// TODO Fix this
		//else
		//{
		//	BEGIN_TEXT_COMMAND_THEFEED_POST("jamyfafi");
		//	add_text_component_long_string(s);
		//}
	}

	//Game::Sound::PlayFrontend("Phone_SoundSet_Default", "Text_Arrive_Tone");
	UI::PlayFrontend("Phone_SoundSet_Default", "Text_Arrive_Tone");

	//END_TEXT_COMMAND_THEFEED_POST_TICKER_FORCED(0, 1);
	//return END_TEXT_COMMAND_THEFEED_POST_TICKER(0, 0);
	END_TEXT_COMMAND_THEFEED_POST_TICKER(0, 0);
}


//