// StringUtils.h
#pragma once

#include <string>
#include <algorithm> // For std::find_if, std::isspace

namespace StringUtils {
    // Function to remove leading whitespace
    inline std::string& ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
        return s;
    }

    // Function to remove trailing whitespace
    inline std::string& rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
        return s;
    }

    // Function to remove leading and trailing whitespace
    inline std::string& trim(std::string& s) {
        ltrim(s);
        rtrim(s);
        return s;
    }

    // Overloads that return a new string (safer if you don't want to modify original)
    inline std::string ltrim_copy(std::string s) {
        return ltrim(s);
    }

    inline std::string rtrim_copy(std::string s) {
        return rtrim(s);
    }

    inline std::string trim_copy(std::string s) {
        return trim(s);
    }
}