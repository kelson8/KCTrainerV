#pragma once
#include <format>
#include <string>

#include "Constants.hpp"

#define LOG(level, fmt, ...) \
    g_Logger.Write(level, fmt, __VA_ARGS__)

enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

class Logger {

public:
    Logger();
    void SetFile(const std::string& fileName);
    void SetMinLevel(LogLevel level);
    void Clear() const;
    bool Error();
    void ClearError();

    template <typename... Args>
    void Write(LogLevel level, std::string_view fmt, Args&&... args) const {
        try {
            write(level, std::vformat(fmt, std::make_format_args(args...)));
        }
        catch (const std::exception& ex) {
            write(ERROR, std::format("Failed to format: [{}], {}", fmt, ex.what()));
        }
    }

private:
    std::string levelText(LogLevel level) const;
    void write(LogLevel, const std::string& txt) const;

    mutable bool mError = false;
    std::string file = "";
    LogLevel minLevel = INFO;
};

extern Logger g_Logger;

/*
* Google Gemini help for __VA_ARGS__, I didn't fully understand this.
What is __VA_ARGS__?
__VA_ARGS__ is a preprocessor identifier that represents a variable number of arguments passed to a function-like macro. 
Think of it as a placeholder for "all the arguments after the explicitly named ones" in a macro definition.

How it Works in Macro Definitions:

When you define a macro that accepts a variable number of arguments using the ellipsis (...), 
the __VA_ARGS__ identifier within the macro's replacement list will be substituted by those extra arguments, 
including any commas separating them.
*/

// New for debug log, from Chaos Mod
#ifdef KCTRAINER_DEBUG
//#define DEBUG_LOG(_text) LOG(_text)
//#define DEBUG_LOG(_text) LOG(DEBUG, _text)
#define DEBUG_LOG(fmt, ...) LOG(DEBUG, fmt, __VA_ARGS__)
#else
//#define DEBUG_LOG(_text)
#define DEBUG_LOG(fmt, ...)
#endif