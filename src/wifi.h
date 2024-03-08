#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

#include <WiFi.h>
#include "config.h"
#include "server.h"

class WiFiManager
{
public:
    WiFiManager(ConfigManager &configManager, ServerManager &serverManager);
    void setupAPMode(const char *ssid, const char *password);
    void connectToSavedNetwork(const char *ssid, const char *password);
    bool isConnected() const;
    IPAddress getIPAddress() const;
    void disconnect();
    void handleClient();

private:
    ServerManager &serverManager;

    bool connectToNetwork(const char *ssid, const char *password);
};

#endif // WIFI_MANAGER_HPP