#include "pch.h"

#include "Util.hpp"

#include <vector>
#include <algorithm> // For std::transform
#include <cctype>    // For std::toupper, std::isspace

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

//---------- Conversions --------------/

// TODO Test these below, they should work.
// Input as a float and returning a double, is that right?

/// <summary>
/// Convert from meters per second to miles per hour.
/// </summary>
float Util::MetersToMph(float metersPerSecond)
{
    return metersPerSecond * 2.237f;
}

/// <summary>
/// Convert from miles per hour to meters per second.
/// </summary>
/// <param name="metersPerSecond"></param>
/// <returns></returns>
float Util::MphToMeters(float milesPerHour)
{
    return milesPerHour / 2.237f;
}

/// <summary>
/// Convert from meters per second to kilometers per hour.
/// </summary>
/// <param name="metersPerSecond"></param>
/// <returns></returns>
float Util::MetersToKph(float metersPerSecond)
{
    return metersPerSecond * 3.6f;
}

/// <summary>
/// Convert from kilometers per hour to meters per second.
/// TODO Test this, should be correct
/// </summary>
/// <param name="metersPerSecond"></param>
/// <returns></returns>
float Util::KphToMeters(float kilometersPerHour)
{
    return kilometersPerHour / 3.6f;
}

//--------- End conversions -------------/


const std::map<std::string, std::string> Util::g_weaponCategoryDisplayNames = 
{
    {"melee", "Melee Weapons"},
    {"handguns", "Handguns"},
    {"smg", "SMGs"},
    {"shotguns", "Shotguns"},
    {"assault_rifles", "Assault Rifles"},
    {"machine_guns", "Machine Guns"},
    {"sniper_rifles", "Sniper Rifles"},
    {"heavy_weapons", "Heavy Weapons"},
    {"throwables", "Throwables"},
    {"misc", "Miscellaneous"}
};

std::string Util::GetWeaponCategoryDisplayName(const std::string& categoryKey) 
{
    auto it = Util::g_weaponCategoryDisplayNames.find(categoryKey);
    if (it != Util::g_weaponCategoryDisplayNames.end()) {
        return it->second;
    }
    // Fallback: If not found in the map, try to auto-format (like your ToDisplayName)
    // Or just return the original key if you prefer
    return categoryKey; // Or apply ToDisplayName(categoryKey) if you want consistent auto-format
}

// This function will intelligently convert internal weapon names to display names.
std::string Util::WeaponToDisplayName(const std::string& internalName) 
{
    // --- Step 1: Direct Overrides (Highest Priority) ---
    // Use this map for names that require specific, non-standard conversions
    // or those where auto-formatting won't give the desired result.
    static const std::map<std::string, std::string> directInternalNameOverrides = 
    {
        {"mg", "MG"}, // To ensure it doesn't become "Mg" by default
        
        // Heavy
        {"rpg", "RPG"},
        {"grenadelauncher", "Grenade Launcher"},
        {"grenadelauncher_smoke", "Smoke Grenade Launcher"},
        {"minigun", "Minigun"},

        {"firework", "Firework Launcher"},
        {"railgun", "Railgun"},
        {"hominglauncher", "Homing Launcher"},
        {"compactlauncher", "Compact Grenade Launcher"},
        {"rayminigun", "Widow Maker"},

        // Throwables
        {"smokegrenade", "Smoke Grenade"},
        {"molotov", "Molotov"},
        {"stickybomb", "Sticky Bomb"},
        {"proxmine", "Proximity Mine"},

        // Misc
        {"bzgas", "BZ Gas"},
        {"snowball", "Snowball"},
        {"pipebomb", "Pipe Bomb"},
        {"ball", "Baseball"},
        {"petrolcan", "Petrol Can"},
        {"fireextinguisher", "Fire Extinguisher"},
        {"hazardcan", "Jerry Can"},

        {"parachute", "Parachute"},
        
        // Melee
        {"unarmed", "Unarmed"}, // Ensure it doesn't become "Unarmed"
        {"weapon_candycane", "Candy Cane"},
        {"weapon_stunrod", "Stun Rod"},

        {"knuckle", "Knuckle Dusters"},
        {"battleaxe", "Battle Axe"},
        {"poolcue", "Pool Cue"},
        {"stone_hatchet", "Stone Hatchet"},
        
        // Handguns
        {"flaregun", "Flare Gun"},
        {"marksmanpistol", "Marksman Pistol"},
        {"revolver", "Revolver"},
        {"pistol50", "Pistol .50"}, // Specific format

        {"doubleaction", "Double-Action Revolver"},

        {"appistol", "AP Pistol"},
        {"combatpistol", "Combat Pistol"},
        {"machinepistol", "Machine Pistol"}, // Direct override
        {"ceramicpistol", "Ceramic Pistol"},
        {"heavypistol", "Heavy Pistol"},
        {"navyrevolver", "Navy Revolver"},
        {"raypistol", "Up-n-Atomizer"},
        {"snspistol", "Sns Pistol"},
        {"snspistol_mk2", "Sns Pistol Mk 2"},
        {"vintagepistol", "Vintage Pistol"},

        // SMGs
        {"microsmg", "Micro SMG"},
        {"smg", "SMG"},
        {"smg_mk2", "SMG MK 2"},
        {"assaultsmg", "Assault SMG"}, // Direct override for problematic ones
        {"combatpdw", "Combat PDW"},

        {"minismg", "Mini SMG"},
        {"raycarbine", "Unholy Hellbringer"},

        // Shotguns
        {"pumpshotgun", "Pump Shotgun"},
        {"pumpshotgun_mk2", "Pump Shotgun Mk 2"},
        {"sawnoffshotgun", "Sawed-Off Shotgun"},
        {"assaultshotgun", "Assault Shotgun"},
        {"bullpupshotgun", "Bullpup Shotgun"},
        {"heavyshotgun", "Heavy Shotgun"},
        {"dbshotgun", "Double-Barrel Shotgun"},
        {"autoshotgun", "Auto Shotgun"},
        {"musket", "Musket"},

        // Assault Rifles
        {"assaultrifle", "Assault Rifle"},
        {"assaultrifle_mk2", "Assault Rifle Mk 2"},
        {"carbinerifle", "Carbine Rifle"},
        {"carbinerifle_mk2", "Carbine Rifle Mk 2"},
        {"advancedrifle", "Advanced Rifle"},
        {"specialcarbine", "Special Carbine"},
        {"specialcarbine_mk2", "Special Carbine Mk 2"},
        {"bullpuprifle", "Bullpup Rifle"},
        {"bullpuprifle_mk2", "Bullpup Rifle Mk 2"},
        {"compactrifle", "Compact Rifle"},

        {"combatmg", "Combat MG"},
        {"gusenberg", "Gusenberg Sweeper"},
        
        // Snipers
        {"sniperrifle", "Sniper Rifle"},
        {"heavysniper", "Heavy Sniper"},
        {"heavysniper_mk2", "Heavy Sniper Mk2"},
        
        {"marksmanrifle", "Marksman Rifle"},
        {"marksmanrifle_mk2", "Marksman Rifle Mk2"}
        
    };

    auto directIt = directInternalNameOverrides.find(internalName);
    if (directIt != directInternalNameOverrides.end()) 
    {
        return directIt->second; // Return directly if a direct override exists
    }

    // --- Step 2: CamelCase / Word Boundary Spacing ---
    std::string tempName = internalName;
    std::replace(tempName.begin(), tempName.end(), '_', ' '); // Replace underscores first

    std::string spacedName;
    if (!tempName.empty()) 
    {
        spacedName += tempName[0]; // Start with the first character
        for (size_t i = 1; i < tempName.length(); ++i) 
        {
            // Insert space if:
            // 1. Lowercase followed by uppercase (e.g., "pistol50" -> "pistol 50" after case conversion)
            // 2. Letter followed by digit (e.g., "mk2" -> "MK 2")
            // 3. Not a space, and previous was a space (to handle multiple words)
            // 4. Also handle cases where a digit follows a non-space, non-digit character (e.g., Pistol50)

            // This is the most complex rule for camelCase-like:
            // If current char is uppercase and previous is lowercase (e.g. 's' 'M' in AssaultSmg -> Assault Smg)
            if (std::isupper(tempName[i]) && std::islower(tempName[i - 1])) 
            {
                spacedName += ' ';
            }
            // If current char is digit and previous is not a digit (e.g., 'l' '5' in Pistol50 -> Pistol 50)
            else if (std::isdigit(tempName[i]) && !std::isdigit(tempName[i - 1]) && !std::isspace(tempName[i - 1])) 
            {
                spacedName += ' ';
            }

            spacedName += tempName[i];
        }
    }

    // --- Step 3: Capitalize First Letter of Each Word ---
    std::string displayName = spacedName; // Start with the spaced name
    bool capitalizeNext = true;
    for (char& c : displayName) 
    {
        if (std::isspace(c)) {
            capitalizeNext = true;
        }
        else if (capitalizeNext) 
        {
            c = static_cast<char>(std::toupper(c));
            capitalizeNext = false;
        }
        else 
        {
            c = static_cast<char>(std::tolower(c)); // Ensure subsequent letters are lowercase
        }
    }

    // --- Step 4: MK2 Handling (after general spacing and initial capitalization) ---
    // Corrects " Mk2" to " MK2"
    size_t mk2Pos = displayName.find(" Mk2");
    if (mk2Pos != std::string::npos) 
    {
        displayName.replace(mk2Pos, 4, " MK2"); // Replace " Mk2" (4 chars) with " MK2" (4 chars)
    }

    // --- Step 5: Final Cleanup (Trimming and removing extra spaces) ---
    // Remove leading/trailing spaces and multiple spaces
    std::stringstream ss(displayName);
    std::string word;
    displayName.clear(); // Clear to rebuild
    while (ss >> word) 
    {
        if (!displayName.empty()) 
        {
            displayName += ' ';
        }
        displayName += word;
    }

    return displayName;
}