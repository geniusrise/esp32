#include "wifi.h"
#include "util.h"

/**
 * @brief Constructs a new WiFiManager object.
 *
 * Initializes the WiFi stack.
 */
WiFiManager::WiFiManager()
{
    WiFi.mode(WIFI_OFF); // Ensure WiFi is off before setting up new mode
}

/**
 * @brief Sets up the device in AP (Access Point) mode with the provided SSID and password.
 *
 * @param ssid The SSID for the AP.
 * @param password The password for the AP.
 */
void WiFiManager::setupAPMode(const char *ssid, const char *password)
{
    WiFi.mode(WIFI_AP); // Switch to AP mode
    WiFi.softAP(ssid, password);
}

/**
 * @brief Connects the device to a saved WiFi network using the provided SSID and password.
 *
 * @param ssid The SSID of the WiFi network.
 * @param password The password of the WiFi network.
 */
void WiFiManager::connectToSavedNetwork(const char *ssid, const char *password)
{
    WiFi.mode(WIFI_STA); // Switch to STA (Station) mode
    connectToNetwork(ssid, password);
}

/**
 * @brief Checks if the device is currently connected to a WiFi network.
 *
 * @return true If the device is connected.
 * @return false If the device is not connected.
 */
bool WiFiManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief Gets the IP Address of the device when connected to a WiFi network or running an AP.
 *
 * @return IPAddress The IP Address of the device.
 */
IPAddress WiFiManager::getIPAddress() const
{
    return WiFi.localIP();
}

/**
 * @brief Disconnects the device from the WiFi network.
 */
void WiFiManager::disconnect()
{
    WiFi.disconnect();
}

/**
 * @brief Handles client requests when the device is in AP mode.
 *
 * This should be called in the main loop to handle incoming client requests.
 */
void WiFiManager::handleClient()
{
    // Handle client requests here. This is typically done when running a web server.
    // TODO: need to connect this to the web server
}

/**
 * @brief Internal method to connect to a WiFi network.
 *
 * @param ssid The SSID of the network to connect to.
 * @param password The password of the network.
 * @return true If the connection was successful.
 * @return false If the connection failed.
 */
bool WiFiManager::connectToNetwork(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);

    // Attempt to connect with a timeout
    unsigned long startTime = millis();
    const unsigned long timeout = 86400000; // 1 day timeout
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout)
    {
        delay(500);
        printf(".");
    }

    printf("Connected to %s\n\n", &ssid);

    return WiFi.status() == WL_CONNECTED;
}
