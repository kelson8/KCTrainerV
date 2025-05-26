#include "pch.h"

#include "Scripts/PlayerScripts.h"
#include "Scripts/TextScripts.h"

#include "inc/types.h"

#include "Util/Logger.hpp"
#include "Util/Paths.hpp"

#include "FileFunctions.h"

//------------- File functions ---------------/

#ifdef LOAD_IPLS

extern std::vector<std::string_view> g_loadedIpls; // Declaration (extern)

#endif

/// <summary>
/// Save text positions from debug texts into a file.
/// Will be very useful for logging the values for my debug texts on screen.
/// This seems to work well
/// </summary>
/// <param name="fileName">The file name to save text positions into</param>
/// <returns>If the file was successfully wrote to.</returns>
bool FileFunctions::SavePlayerMenuTextPositions(const std::string& fileName)
{
	auto& playerScripts = PlayerScripts::GetInstance();

	auto& textScripts = TextScripts::GetInstance();

	// Positions
	// X
	float playerXMenuPosX = textScripts.playerXMenuPosX;
	float playerXMenuPosY = textScripts.playerXMenuPosY;
	
	// Y
	float playerYMenuPosX = textScripts.playerYMenuPosX;
	float playerYMenuPosY = textScripts.playerYMenuPosY;

	// Z
	float playerZMenuPosX = textScripts.playerZMenuPosX;
	float playerZMenuPosY = textScripts.playerZMenuPosY;
	
	// Heading
	float playerHeadingMenuPosX = textScripts.headingMenuPosX;
	float playerHeadingMenuPosY = textScripts.headingMenuPosY;

	// String of the list of text positions, this is what gets saved in the file.
	std::string textPositionsString =
		// X
		std::format("PlayerX Menu Pos X: {:.3f}", playerXMenuPosX)
		+ std::format("\nPlayerX Menu Pos Y: {:.3f}", playerXMenuPosY)

		// Y
		+ std::format("\n\nPlayerY Menu Pos X: {:.3f}", playerYMenuPosX)
		+ std::format("\nPlayerY Menu Pos Y: {:.3f}", playerYMenuPosY)
		
		// Z
		+ std::format("\n\nPlayerZ Menu Pos X: {:.3f}", playerZMenuPosX)
		+ std::format("\nPlayerZ Menu Pos Y: {:.3f}", playerZMenuPosY)

		// Heading
		+ std::format("\n\nPlayer Heading Menu Pos X: {:.3f}", playerHeadingMenuPosX)
		+ std::format("\nPlayer Heading Menu Pos Y: {:.3f}", playerHeadingMenuPosY);


	std::filesystem::path modPath = Paths::GetModPath();

	std::string pathString = (modPath / fileName).string();

	std::ofstream outputFile(pathString);

	// Make sure the file is open
	if (outputFile.is_open()) 
	{
		outputFile << textPositionsString;
		outputFile.close();
		std::string fileSavedString = std::format("Menu text positions saved to '{}'", fileName);

		log_output(fileSavedString);
		return true;
	}
	else
	{
		std::string errorString = std::format("Error opening file '{}' for writing!", fileName);
		log_error(errorString);
		return false;
	}
}

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

	if (outputFile.is_open()) 
	{
		outputFile << playerCoordsText << std::endl;
		outputFile << playerHeadingText << std::endl;
		outputFile.close();
		std::string playerCoordsHeadingString = std::format("Player coordinates and heading saved to '{}'", fileName);

		//std::cout << "Player coordinates and heading saved to '" << fileName << "'" << std::endl;
		log_output(playerCoordsHeadingString);
	}
	else 
	{
		std::string errorString = std::format("Error opening file '{}' for writing!", fileName);
		log_error(errorString);
		//std::cerr << "Error opening file '" << fileName << "' for writing!" << std::endl;
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
	auto& teleportLocations = TeleportLocations::GetInstance();

	//float newHeading = 0.0f;
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
			// TODO Setup heading for this
			// TODO Fix this to work
			// New function to set heading and fade.
			//playerScripts.SetPlayerCoords(Vector3(x, y, z), 0.0f, true);
			//playerScripts.SetPlayerCoords(Vector3(x, y, z), newHeading, true);
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
			//playerScripts.SetPlayerHeading(heading);
			//newHeading = heading;
		}
		else
		{
			LOG(ERROR, "Error reading the second line (heading) from the file.");
			//std::cerr << "Error reading the second line (heading) from the file." << std::endl;
		}




		// Close the file
		inputFile.close();

		// Explicitly unload loaded IPLs when teleporting to saved coords
#ifdef LOAD_IPLS
		teleportLocations.UnloadAllLoadedIpls();
#endif
	}
	else {
		//std::cerr << "Error opening file " << fileName << " for reading." << std::endl;
		LOG(ERROR, "Error opening file {} for reading.", fileName);
	}
}


#ifdef _WIN32
bool FileFunctions::DoesFileExist(const std::string& fileName)
{
	// New for C++17:
	return std::filesystem::exists(fileName);
	// Old method:
	//std::ifstream file(fileName);
	//return file.good();
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

