#include "pch.h"

#include "Scripts/PlayerScripts.h"

#include "inc/types.h"

#include "Util/Logger.hpp"
#include "Util/Paths.hpp"

#include "FileFunctions.h"

//------------- File functions ---------------/

/// <summary>
/// Save player coordinates to file, adapted from KCTrainerIV
/// Filepath obtained like this below:
/// std::filesystem::path modPath = Paths::GetModPath();
/// std::string pathString = (modPath / fileName).string();
/// </summary>
/// <param name="fileName">The file to write coordinates and heading to.</param>
void FileFunctions::SaveCoordinatesToFile(const std::string& fileName)
{
	auto& playerScripts = PlayerScripts::GetInstance();
	// Get player coords and heading.
	Vector3 playerCoords = playerScripts.GetPlayerCoords();
	float playerX = playerCoords.x;
	float playerY = playerCoords.y;
	float playerZ = playerCoords.z;

	float playerHeading = playerScripts.GetPlayerHeading();

	std::string playerCoordsText = "X: " + std::to_string(playerX)
		+ " Y: " + std::to_string(playerY)
		+ " Z: " + std::to_string(playerZ);

	std::string playerHeadingText = "Heading: " + std::to_string(playerHeading);

	//std::string pluginsPath = "plugins\\";
	std::filesystem::path modPath = Paths::GetModPath();

	std::string pathString = (modPath / fileName).string();

	//std::ofstream outputFile(modPath + fileName);
	std::ofstream outputFile(pathString);

	if (outputFile.is_open()) {
		outputFile << playerCoordsText << std::endl;
		outputFile << playerHeadingText << std::endl;
		outputFile.close();
		std::cout << "Player coordinates and heading saved to '" << fileName << "'" << std::endl;
	}
	else {
		std::cerr << "Error opening file '" << fileName << "' for writing!" << std::endl;
	}
}

/// <summary>
/// Teleport to coords saved in a file, adapted from KCTrainerIV
/// Filepath obtained like this below:
/// std::filesystem::path modPath = Paths::GetModPath();
/// std::string pathString = (modPath / fileName).string();
/// </summary>
/// <param name="fileName">The file to read coordinates and heading from.</param>
void FileFunctions::TeleportToSavedCoords(const std::string& fileName)
{
	auto& playerScripts = PlayerScripts::GetInstance();
	// Example file read:
	/*
		std::string playerCoordsText = "X: " + std::to_string(playerX)
		+ " Y: " + std::to_string(playerY)
		+ " Z: " + std::to_string(playerZ);

	std::string playerHeadingText = "Heading: " + std::to_string(playerHeading);

	*/

	// File contents:
	/*
		X: 103.312599 Y: 846.625977 Z: 45.125286
		Heading: 180.000015

	*/

	std::filesystem::path modPath = Paths::GetModPath();

	std::string pathString = (modPath / fileName).string();

	//std::ofstream outputFile(modPath + fileName);
	std::ifstream inputFile(pathString);

	if (inputFile.is_open())
	{
		std::string line;
		float x, y, z, heading;

		// Read the first line (coordinates)
		if (std::getline(inputFile, line))
		{
			std::stringstream ss(line);
			std::string label;

			// Extract X
			ss >> label >> x; // Reads "X:" into label (which we discard) and then the float into x

			// Extract Y
			ss >> label >> y; // Reads "Y:" into label and then the float into y

			// Extract Z
			ss >> label >> z; // Reads "Z:" into label and then the float into z

			//std::cout << "Read Coordinates - X: " << x << ", Y: " << y << ", Z: " << z << std::endl;

			//SET_CHAR_COORDINATES(GetPlayerChar(), x, y, z);
			playerScripts.SetPlayerCoords(Vector3(x, y, z));
		}
		else
		{
			LOG(ERROR, "Error reading the first line (coordinates) from the file.");
			//std::cerr << "Error reading the first line (coordinates) from the file." << std::endl;
		}

		// Read the second line (heading)
		if (std::getline(inputFile, line))
		{
			std::stringstream ss(line);
			std::string segment;

			// Extract "Heading:"
			std::getline(ss, segment, ' ');

			// Extract the heading value
			ss >> heading;

			//std::cout << "Read Heading - Heading: " << heading << std::endl;

			// TODO Test this
			//SET_CHAR_HEADING(GetPlayerChar(), heading);
			playerScripts.SetPlayerHeading(heading);
		}
		else
		{
			LOG(ERROR, "Error reading the second line (heading) from the file.");
			//std::cerr << "Error reading the second line (heading) from the file." << std::endl;
		}


		// Close the file
		inputFile.close();
	}
	else {
		//std::cerr << "Error opening file " << fileName << " for reading." << std::endl;
		LOG(ERROR, "Error opening file {} for reading.", fileName);
	}
}


#ifdef _WIN32
bool FileFunctions::DoesFileExist(const std::string& fileName)
{
	std::ifstream file(fileName);
	return file.good();
}

std::string FileFunctions::CurrentDirectory()
{
    char current_dir[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, current_dir) != 0)
    {
        return current_dir;
    }
    else {
        return "Error getting current directory.";
    }
}



#endif


/// <summary>
/// Check if a directory exists, for now this only works for Windows.
/// TODO Make this work on Linux if I get around to testing it on there.
/// </summary>
/// <param name="folderPath">The path to check</param>
/// <returns>If the folder exists</returns>
bool
#ifdef _WIN32
FileFunctions::CheckDirectoryExists(LPCSTR folderPath)
#else
FileFunctions::CheckDirectoryExists(std::string folderPath)
#endif //_WIN32
{
#ifdef _WIN32

	// TODO Move this into another function.
	// Create the directory if it doesn't already exist.
	// CreateDirectory(logPath.c_str(), NULL);
	// CreateDirectory(folderPath, NULL);

	// Check if the directory exists
	// https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
	DWORD ftyp = GetFileAttributesA(folderPath);
	if(ftyp == INVALID_FILE_ATTRIBUTES) {
		return false; // Something is wrong with the path
	}

	if(ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		return true; // This is a valid directory
	}

	return false; // Not a directory

	//
	// TRACE("KCNet - 'ViceExtended\logs' folder not found, creating one for you");
#else
	// Not implemented on other systems.
	return false;
#endif //_WIN32
}

//------------- End file functions ---------------/

