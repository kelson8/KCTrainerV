#include "pch.h"

#include "Util.hpp"

#include <inc/main.h>
#include <inc/natives.h>

/// <summary>
/// Check if the console is currently attached.
/// </summary>
/// <returns></returns>
bool Util::IsConsoleAttached()
{
    return GetConsoleWindow() != nullptr;
}

// Taken some code from GTAVAddonLoader:
// https://github.com/ikt32/GTAVAddonLoader/blob/master/GTAVAddonLoader/Util/Util.cpp

void Util::ShowText(float x, float y, float scale, const char* text, int font, const Color& rgba, bool outline) {
    HUD::SET_TEXT_FONT(font);
    HUD::SET_TEXT_SCALE(scale, scale);
    HUD::SET_TEXT_COLOUR(rgba.R, rgba.G, rgba.B, rgba.A);
    HUD::SET_TEXT_WRAP(0.0, 1.0);
    HUD::SET_TEXT_CENTRE(0);
    if (outline) HUD::SET_TEXT_OUTLINE();
    HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)(text));
    HUD::END_TEXT_COMMAND_DISPLAY_TEXT({ x, y }, 0);
}

void Util::ShowNotification(std::string message, int* prevNotification) {
    if (prevNotification != nullptr && *prevNotification != 0) {
        HUD::THEFEED_REMOVE_ITEM(*prevNotification);
    }
    HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");

    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)(message.c_str()));

    int id = HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, false);
    if (prevNotification != nullptr) {
        *prevNotification = id;
    }
}

void Util::ShowSubtitle(std::string message, int duration) {
    HUD::BEGIN_TEXT_COMMAND_PRINT("CELL_EMAIL_BCON");

    const int maxStringLength = 99;

    for (int i = 0; i < message.size(); i += maxStringLength) {
        int npos = std::min(maxStringLength, static_cast<int>(message.size()) - i);
        HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)(message.substr(i, npos).c_str()));
    }

    HUD::END_TEXT_COMMAND_PRINT(duration, 1);
}

Hash Util::Joaat(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    Hash hash = 0;
    for (int i = 0; i < s.size(); i++) {
        hash += s[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

std::string Util::GetGxtName(Hash hash) {
    const char* name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(hash);
    std::string displayName = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(name);
    if (displayName == "NULL") {
        displayName = name;
    }
    return displayName;
}

// TODO Implement these

//std::string getMakeName(Hash hash) {
//    char* makeName = MemoryAccess::GetVehicleMakeName(hash);
//    if (makeName == nullptr ||
//        strcmp(makeName, "") == 0 ||
//        strcmp(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(makeName), "NULL") == 0) {
//        return std::string();
//    }
//    return std::string(HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(makeName));
//}
//
//std::string getModelName(Hash hash) {
//    auto modelIt = g_vehicleHashes.find(hash);
//    if (modelIt != g_vehicleHashes.end()) return modelIt->second;
//    return "NOTFOUND";
//}

// End taken from GTAVAddonLoader