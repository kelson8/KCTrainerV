#pragma once
#include <string>
#include <vector>
#include <inc/types.h>

namespace UI {
    void Notify(const std::string& message);
    void Notify(const std::string& message, bool removePrevious);

    void PlayFrontend(const std::string& sound_dict, const std::string& sound_name);
    void PlayFrontend_default(const std::string& sound_name);
}

