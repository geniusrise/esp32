#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager
{
public:
    WiFiManager();

    void setupAPMode(const char *ssid, const char *password);
    void connectToSavedNetwork(const char *ssid, const char *password);
    bool isConnected() const;
    IPAddress getIPAddress() const;
    void disconnect();
    void handleClient(); // For handling incoming client requests when in AP mode

private:
    bool connectToNetwork(const char *ssid, const char *password);
};

#endif // WIFI_MANAGER_H
