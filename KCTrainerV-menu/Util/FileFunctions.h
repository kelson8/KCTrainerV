#pragma once

#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

class FileFunctions {
public:
	// Instance of the FileFunctions
	static FileFunctions& GetInstance()
	{
		static FileFunctions instance;
		return instance;
	}

#ifdef _WIN32
	// TODO Fix for Linux

	// Folder stuff
	static bool DoesFileExist(const std::string& fileName);
	static std::string CurrentDirectory();

	static bool CheckDirectoryExists(LPCSTR folderPath);

	// Saving coords/ loading them from file and teleporting
	void SaveCoordinatesToFile(const std::string& fileName);
	void TeleportToSavedCoords(const std::string& fileName);


	// New for IV
	//void SaveCoordinatesToFile(const std::string& fileName);
#else 
	static bool CheckDirectoryExists(std::string folderPath);
#endif

private:
	FileFunctions() {} // Private constructor to prevent external instantiation
	FileFunctions(const FileFunctions&) = delete;
	FileFunctions& operator=(const FileFunctions&) = delete;
};