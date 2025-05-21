#include "UI.hpp"

#include <inc/natives.h>

namespace
{
    int notificationHandle = 0;
}

void UI::Notify(const std::string &message)
{
    Notify(message, true);
}

void UI::Notify(const std::string &message, bool removePrevious)
{
    int *prevNotification = nullptr;
    if (removePrevious)
    {
        prevNotification = &notificationHandle;
    }

    if (prevNotification != nullptr && *prevNotification != 0)
    {
        HUD::THEFEED_REMOVE_ITEM(*prevNotification);
    }
    HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");

    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message.c_str());

    int id = HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, false);
    if (prevNotification != nullptr)
    {
        *prevNotification = id;
    }
}

// Play sounds
// Adapted from Menyoo
void UI::PlayFrontend(const std::string &sound_dict, const std::string &sound_name)
{
    AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name.c_str(), sound_dict.c_str(), FALSE);
}
void UI::PlayFrontend_default(const std::string &sound_name)
{
    AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name.c_str(), "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);
}
//
