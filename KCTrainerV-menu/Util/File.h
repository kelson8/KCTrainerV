#pragma once

// https://github.com/gta-chaos-mod/ChaosModV/blob/master/ChaosMod/Util/File.h

#include <sys/stat.h>

#include <filesystem>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Paths.hpp"

inline bool DoesFileExist(std::string_view fileName)
{
	struct stat temp;
	return stat(fileName.data(), &temp) == 0;
}


inline bool DoesFeatureFlagExist(const std::string &featureFlagName)
{
	// Cannot convert this.
	//std::string filePath = Paths::GetModPath();
	//return DoesFileExist("chaosmod\\." + featureFlagName);
	// TODO Make this not hard-coded
	return DoesFileExist("KCTrainerV\\." + featureFlagName);
}

inline std::vector<std::filesystem::directory_entry> GetFiles(std::string path, std::string extension, bool recursive,
                                                              std::vector<std::string> blacklistedFiles = {})
{
	std::vector<std::filesystem::directory_entry> entries;
	auto handleEntry = [&](const std::filesystem::directory_entry &entry)
	{
		auto path = entry.path();
		if (entry.is_regular_file() && path.has_extension() && path.extension() == extension && entry.file_size() > 0)
		{
			bool addFile = true;
			for (const auto &blacklistedFile : blacklistedFiles)
			{
				if (path.string().ends_with(blacklistedFile))
				{
					addFile = false;
					break;
				}
			}

			if (addFile)
				entries.push_back(entry);
		}
	};

	if (recursive)
		for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
			handleEntry(entry);
	else
		for (const auto &entry : std::filesystem::directory_iterator(path))
			handleEntry(entry);

	return entries;
}