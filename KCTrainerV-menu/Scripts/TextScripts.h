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

	void SetTextEntry(const char* text);
	void TextPosition(float y);
	void SetTextPosition();

	void DisplayText(const char* text);

	void SetupText();

	bool drawText = false;

private:
	TextScripts() {} // Private constructor to prevent external instantiation
	TextScripts(const TextScripts&) = delete;
	TextScripts& operator=(const TextScripts&) = delete;

	
};

