#pragma once
#include <string>
#include <vector>
#include <random> // For randomizing notifications

namespace NotificationManager
{
    // Stores all loaded notification strings
    extern std::vector<std::string> g_notifications;

    // Call this once during mod initialization to load notifications from file
    void LoadNotificationsFromFile(const std::string& filePath);

    // Call this to get a random notification
    std::string GetRandomNotification();

    // Call this to get a specific notification by index
    std::string GetNotificationByIndex(size_t index);

    // Get the total number of loaded notifications
    size_t GetNotificationCount();
}

