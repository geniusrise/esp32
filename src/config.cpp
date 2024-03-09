#include "config.h"
#include "nvs_flash.h"
#include <Preferences.h>

Preferences preferences; // Global preferences object for flash storage access

/**
 * @brief Constructs a new Config Manager object and loads the saved
 * configuration.
 */
ConfigManager::ConfigManager()
{

  esp_err_t err_init = nvs_flash_init();
  if (err_init == ESP_ERR_NVS_NO_FREE_PAGES ||
      err_init == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err_init = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err_init);

  loadConfiguration(); // Load configuration on object creation
}

/**
 * @brief Loads the configuration data from the non-volatile storage into the
 * class properties.
 *
 * Opens the non-volatile storage in read-only mode and retrieves the stored
 * values for the configuration. If no value is found, default values are set.
 */
void
ConfigManager::loadConfiguration()
{
  preferences.begin("ixaxaar", true); // Open in read-only mode
  // Retrieve WiFi configuration
  wifiSSID = preferences.getString("wifiSSID", "");
  wifiPassword = preferences.getString("wifiPassword", "");

  // Retrieve user configuration
  userConfig.name = preferences.getString("userName", "");
  userConfig.username = preferences.getString("userUsername", "");
  userConfig.password = preferences.getString("userPassword", "");
  userConfig.userSettings = preferences.getString("userSettings", "");

  preferences.end(); // Close the preferences
}

/**
 * @brief Saves the current configuration to non-volatile storage.
 *
 * Opens the non-volatile storage in write mode and writes the current values
 * of the class properties to the storage.
 */
void
ConfigManager::saveConfiguration()
{
  preferences.begin("ixaxaar", false); // Open in read-write mode
  // Store WiFi configuration
  preferences.putString("wifiSSID", wifiSSID);
  preferences.putString("wifiPassword", wifiPassword);

  // Store user configuration
  preferences.putString("userName", userConfig.name);
  preferences.putString("userUsername", userConfig.username);
  preferences.putString("userPassword", userConfig.password);
  preferences.putString("userSettings", userConfig.userSettings);

  preferences.end(); // Close the preferences
}

/**
 * @brief Gets the stored WiFi SSID.
 *
 * @return String The stored WiFi SSID.
 */
String
ConfigManager::getWiFiSSID() const
{
  return wifiSSID;
}

/**
 * @brief Sets the WiFi SSID and saves the configuration.
 *
 * @param ssid The WiFi SSID to be set.
 */
void
ConfigManager::setWiFiSSID(const String& ssid)
{
  wifiSSID = ssid;
  saveConfiguration();
}

/**
 * @brief Gets the stored WiFi password.
 *
 * @return String The stored WiFi password.
 */
String
ConfigManager::getWiFiPassword() const
{
  return wifiPassword;
}

/**
 * @brief Sets the WiFi password and saves the configuration.
 *
 * @param password The WiFi password to be set.
 */
void
ConfigManager::setWiFiPassword(const String& password)
{
  wifiPassword = password;
  saveConfiguration();
}

/**
 * @brief Gets the stored user configuration.
 *
 * @return UserConfig The stored user configuration including name, username,
 * and password.
 */
UserConfig
ConfigManager::getUserConfig() const
{
  return userConfig;
}

/**
 * @brief Sets the user configuration and saves it to the storage.
 *
 * @param config The UserConfig structure containing user details to be set.
 */
void
ConfigManager::setUserConfig(const UserConfig& config)
{
  userConfig = config;
  saveConfiguration();
}
