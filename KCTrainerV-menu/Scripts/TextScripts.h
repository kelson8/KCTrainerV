#pragma once
class TextScripts
{
public:
	// Instance of the TextScripts
	static TextScripts& GetInstance()
	{
		static TextScripts instance;
		return instance;
	}

	// TODO Possibly make these private functions?
	void SetTextEntry(const char* text);
	void TextPosition(float y);
	void TextPosition(float x, float y);
	void SetTextPosition();

	void DisplayText(const char* text);

	// Displays text
	// Test text
	void SetupText();

	// Coords and heading
	void DisplayCoordinates();
	//

	// Booleans
	bool drawText = false;
	bool drawCoords = false;

	// Floats for coords drawing position
	// The names of these might be a bit confusing, 
	// playerXMenuPosX means (playerX)+MenuPosX, and playerXMenuPosY means (playerY)+MenuPosY
	// Making these a bit easier to go by for debugging.

	// X position
	float playerXMenuPosX = 0.190f;
	//float playerXMenuPosY = 0.895f;
	float playerXMenuPosY = 0.949f;


	// Y position
	//float playerYMenuPosX = 0.190f;
	float playerYMenuPosX = 0.193f;
	//float playerYMenuPosY = 0.900f;
	float playerYMenuPosY = 0.951f;

	// Z position
	//float playerZMenuPosX = 0.190f;
	float playerZMenuPosX = 0.189f;
	//float playerZMenuPosY = 0.900f;
	float playerZMenuPosY = 0.953f;
	
	// Heading position on menu
	float headingMenuPosX = 0.205f;
	//float headingMenuPosY = 0.920f;
	float headingMenuPosY = 0.973f;

private:
	TextScripts() {} // Private constructor to prevent external instantiation
	TextScripts(const TextScripts&) = delete;
	TextScripts& operator=(const TextScripts&) = delete;

	
};

