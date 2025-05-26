#pragma once
#include <string>
#include <map>

#include <inc/types.h>

// Taken some code from GTAVAddonLoader:
// https://github.com/ikt32/GTAVAddonLoader/blob/master/GTAVAddonLoader/Util/Util.hpp

struct Color {
    int R;
    int G;
    int B;
    int A;
};

const Color solidWhite = {
    255, 255, 255, 255
};

class Util
{
public:
    // Check if console is attached.
    static bool IsConsoleAttached();



    // Natives called
    void ShowText(float x, float y, float scale, const char* text, int font = 0, const Color& rgba = solidWhite, bool outline = false);
    void ShowNotification(std::string message, int* prevNotification = nullptr);
    void ShowSubtitle(std::string message, int duration = 2500);

    Hash Joaat(std::string s);
    std::string RemoveSpecialChars(std::string input);
    std::string GetGxtName(Hash hash); // gxt name from model


    static const std::map<std::string, std::string> g_weaponCategoryDisplayNames;
    static std::string GetWeaponCategoryDisplayName(const std::string& categoryKey);

    // Convert a weapon name from the json file to use a more readable name.
    static std::string WeaponToDisplayName(const std::string& internalName);
};

