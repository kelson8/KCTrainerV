#pragma once
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
    // Natives called
    void ShowText(float x, float y, float scale, const char* text, int font = 0, const Color& rgba = solidWhite, bool outline = false);
    void ShowNotification(std::string message, int* prevNotification = nullptr);
    void ShowSubtitle(std::string message, int duration = 2500);

    Hash Joaat(std::string s);
    std::string RemoveSpecialChars(std::string input);
    std::string GetGxtName(Hash hash); // gxt name from model
};

