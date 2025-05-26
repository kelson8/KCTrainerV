#pragma once
#include <string>
#include "IScriptBuilder.h"

#include "Util/Enums.h"

class TextScripts : public IScriptBuilder
{
public:
	// Instance of the TextScripts
	static TextScripts& GetInstance()
	{
		static TextScripts instance;
		return instance;
	}

	void Tick() override;

	// TODO Possibly make these private functions?
	void SetTextEntry(const char* text);
	void SetTextEntry(const char* text, int colorR, int colorG, int colorB, int colorA);

	void TextPosition(float y);
	void TextPosition(float x, float y);
	void SetTextPosition();

	// Displays text
	// Test text
	void SetupText();

	// Coords and heading
	void DisplayCoordinates();
	//

	// Notifications
	void NotificationBottomLeft(std::string notificationString, bool gxt = false);
	//

	// Booleans
	bool drawText = false;
	bool drawCoords = false;


	// TODO Make getters and setters for these values.
	// Floats for coords drawing position
	// The names of these might be a bit confusing, 
	// playerXMenuPosX means (playerX)+MenuPosX, and playerXMenuPosY means (playerY)+MenuPosY
	// Making these a bit easier to go by for debugging.

	// X position
	float playerXMenuPosX = 0.160f;
	float playerXMenuPosY = 0.882f;


	// Y position
	float playerYMenuPosX = 0.160f;
	float playerYMenuPosY = 0.908f;

	// Z position
	float playerZMenuPosX = 0.160f;
	float playerZMenuPosY = 0.935f;
	
	// Heading position on menu
	float headingMenuPosX = 0.160f;
	float headingMenuPosY = 0.960f;

private:
	TextScripts() {} // Private constructor to prevent external instantiation
	TextScripts(const TextScripts&) = delete;
	TextScripts& operator=(const TextScripts&) = delete;

	// Set text font options, for now I'll keep it private it doesn't need accessed outside this.
	void SetupTextOptions();
	void SetupTextOptions(int colorR, int colorG, int colorB, int colorA, bool textProportional, TextJustify textJustify);
	
};

