#include "Logger.hpp"

#include <chrono>
#include <format>
#include <fstream>
#include <vector>

#include "Util.hpp"

#include "defines.h"

Logger g_Logger;

namespace {
    constexpr const char* const levelStrings[] = {
        " DEBUG ",
        " INFO  ",
        "WARNING",
        " ERROR ",
        " FATAL "
    };
}

Logger::Logger() = default;

void Logger::SetFile(const std::string& fileName) {
    file = fileName;
}

void Logger::SetMinLevel(LogLevel level) {
    minLevel = level;
}

void Logger::Clear() const {
    std::ofstream logFile(file, std::ofstream::out | std::ofstream::trunc);
    logFile.close();
    if (logFile.fail())
        mError = true;
}

bool Logger::Error() {
    return mError;
}

void Logger::ClearError() {
    mError = false;
}

void Logger::write(LogLevel level, const std::string& txt) const {
#ifndef _DEBUG
    if (level < minLevel) return;
#endif
    std::ofstream logFile(file, std::ios_base::out | std::ios_base::app);

    // Get the current time point and convert to local time zone
    const auto now_zoned = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now());

    // Extract the local_time from the zoned_time (still high precision)
    const auto local_time_high_precision = now_zoned.get_local_time();

    // ****** THE CRUCIAL CHANGE FOR SECOND-ONLY PRECISION ******
    // Cast the time_point to one that only has second-level precision.
    // This explicitly truncates any sub-second components.
    const auto local_time_seconds_only = std::chrono::time_point_cast<std::chrono::seconds>(local_time_high_precision);
    //

    // Changed logstring to get current time from time zone, and remove the extra numbers
    // Switched from this: [485585:19:09.930]
    // To this: [19:09.930]
    std::string logString = std::format("[{:%H:%M:%S}] [{}] {}\n",
        local_time_seconds_only, // Pass the zoned_time object directly
        levelText(level),
        txt);

    // Logging still works like this.
    logFile << logString;

    // This should exclude the extra debug logs, mostly "Searching for pattern"    
    
    //if (level != DEBUG)
    //{
    //    // This seems to work, although the clock isn't formatted right in it and some lines are duplicated.
    //    // I'll work on this later
    //    if (Util::IsConsoleAttached())
    //    {
    //        log_output(logString);
    //    }

    //}

    // --- Add Console Output Here ---
    // Only print to console if a console is attached and the log level permits
    // This seems to also work for logging to console.
    //if (level >= INFO && Util::IsConsoleAttached()) {
    //    // Directly print to std::cout
    //    std::cout << logString; // It already has a newline from the format string
    //    std::cout << std::flush; // Ensure immediate output for console
    //}
    
    logFile.close();
    if (logFile.fail())
        mError = true;
}

std::string Logger::levelText(LogLevel level) const {
    if (level > 4 || level < 0) {
        return "  UNK  ";
    }

    return levelStrings[level];
}
