#include "NotificationManager.h"
#include "Util/Logger.hpp"
#include <fstream>
#include <regex>
#include <algorithm>

namespace NotificationManager {
    std::vector<std::string> g_notifications;

    // Static variables for random number generation
    static std::mt19937 g_rng;
    static bool g_rng_initialized = false;

    /// <summary>
    /// Load the notifications from the file
    /// </summary>
    /// <param name="filePath">The file to load from</param>
    void LoadNotificationsFromFile(const std::string& filePath) {
        g_notifications.clear(); // Clear any previously loaded notifications

        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::string failedToOpenFileString = std::format("NotificationManager: Failed to open notification file: {}", 
                filePath.c_str());
            LOG(ERROR, failedToOpenFileString);
            return;
        }

        std::string loadNotificationsFromfileString = std::format("NotificationManager: Loading notifications from file: {}",
            filePath.c_str());
        LOG(INFO, loadNotificationsFromfileString);

        std::string line;
        // Regex to match "Notification: " at the beginning of the line
        // Allows for optional whitespace after "Notification:"
        std::regex notification_regex(R"pattern(Notification:\s*(.*))pattern");

        while (std::getline(file, line)) {
            line = std::regex_replace(line, std::regex(R"(\r)"), ""); // Remove carriage returns

            if (line.empty() || line.rfind("#", 0) == 0) { // Check for empty or comment line
                continue;
            }

            std::smatch match;
            if (std::regex_match(line, match, notification_regex)) {
                if (match.size() > 1) {
                    g_notifications.push_back(match.str(1)); // Capture the content after "Notification: "
                }
            }
            else {
                std::string notificationUnparsedLineString = std::format("NotificationManager: Unparsed line in notification file: {}", 
                    line.c_str());
                LOG(WARN, notificationUnparsedLineString);
            }
        }

        if (g_notifications.empty()) {
            std::string notificationNotLoadedString = std::format("NotificationManager: No notifications loaded from file: {}",
                filePath.c_str());
            LOG(WARN, notificationNotLoadedString);
        }
        else {
            std::string notificationsLoadedString = std::format("NotificationManager: Loaded {} notifications.", g_notifications.size());
            LOG(INFO, notificationsLoadedString);
        }

        // Initialize RNG if not already done
        if (!g_rng_initialized) {
            std::random_device rd;
            g_rng.seed(rd());
            g_rng_initialized = true;
        }
    }

    /// <summary>
    /// Get a random notification from the file.
    /// </summary>
    /// <returns></returns>
    std::string GetRandomNotification() {
        if (g_notifications.empty()) {
            return "No notifications loaded.";
        }
        std::uniform_int_distribution<size_t> dist(0, g_notifications.size() - 1);
        return g_notifications[dist(g_rng)];
    }

    std::string GetNotificationByIndex(size_t index) {
        if (index < g_notifications.size()) {
            return g_notifications[index];
        }
        return "Invalid notification index.";
    }

    /// <summary>
    /// Get the amount of notifications in the file.
    /// </summary>
    /// <returns></returns>
    size_t GetNotificationCount() {
        return g_notifications.size();
    }
}
