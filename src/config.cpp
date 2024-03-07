#include "config.h"
#include <Preferences.h>

Preferences preferences; // Global preferences object for flash storage access

/**
 * @brief Constructs a new Config Manager object and loads the saved configuration.
 */
ConfigManager::ConfigManager()
{
    loadConfiguration(); // Load configuration on object creation
}

/**
 * @brief Loads the configuration data from the non-volatile storage into the class properties.
 *
 * Opens the non-volatile storage in read-only mode and retrieves the stored values
 * for the configuration. If no value is found, default values are set.
 */
void ConfigManager::loadConfiguration()
{
    preferences.begin("lilygo", true); // Open in read-only mode
    // Retrieve WiFi configuration
    wifiSSID = preferences.getString("wifiSSID", "");
    wifiPassword = preferences.getString("wifiPassword", "");

    // Retrieve Bluetooth configuration
    bluetoothConfig.deviceName = preferences.getString("btDeviceName", "");
    bluetoothConfig.devicePasskey = preferences.getString("btPasskey", "");
    bluetoothConfig.paired = preferences.getBool("btPaired", false);

    // Retrieve LLM endpoint configuration
    llmEndpointConfig.host = preferences.getString("llmHost", "");
    llmEndpointConfig.port = preferences.getUShort("llmPort", 0);
    llmEndpointConfig.type = preferences.getString("llmType", "");
    llmEndpointConfig.endpoints = preferences.getString("llmEndpoints", "");

    // Retrieve user configuration
    userConfig.name = preferences.getString("userName", "");
    userConfig.demographics = preferences.getString("userDemographics", "");
    userConfig.username = preferences.getString("userUsername", "");
    userConfig.password = preferences.getString("userPassword", "");

    // Retrieve any saved system commands
    systemCommand = preferences.getString("systemCommand", "");
    preferences.end(); // Close the preferences
}

/**
 * @brief Saves the current configuration to non-volatile storage.
 *
 * Opens the non-volatile storage in write mode and writes the current values
 * of the class properties to the storage.
 */
void ConfigManager::saveConfiguration()
{
    preferences.begin("lilygo", false); // Open in read-write mode
    // Store WiFi configuration
    preferences.putString("wifiSSID", wifiSSID);
    preferences.putString("wifiPassword", wifiPassword);

    // Store Bluetooth configuration
    preferences.putString("btDeviceName", bluetoothConfig.deviceName);
    preferences.putString("btPasskey", bluetoothConfig.devicePasskey);
    preferences.putBool("btPaired", bluetoothConfig.paired);

    // Store LLM endpoint configuration
    preferences.putString("llmHost", llmEndpointConfig.host);
    preferences.putUShort("llmPort", llmEndpointConfig.port);
    preferences.putString("llmType", llmEndpointConfig.type);
    preferences.putString("llmEndpoints", llmEndpointConfig.endpoints);

    // Store user configuration
    preferences.putString("userName", userConfig.name);
    preferences.putString("userDemographics", userConfig.demographics);
    preferences.putString("userUsername", userConfig.username);
    preferences.putString("userPassword", userConfig.password);

    // Store any system commands
    preferences.putString("systemCommand", systemCommand);
    preferences.end(); // Close the preferences
}

/**
 * @brief Gets the stored WiFi SSID.
 *
 * @return String The stored WiFi SSID.
 */
String ConfigManager::getWiFiSSID() const
{
    return wifiSSID;
}

/**
 * @brief Sets the WiFi SSID and saves the configuration.
 *
 * @param ssid The WiFi SSID to be set.
 */
void ConfigManager::setWiFiSSID(const String &ssid)
{
    wifiSSID = ssid;
    saveConfiguration();
}

/**
 * @brief Gets the stored WiFi password.
 *
 * @return String The stored WiFi password.
 */
String ConfigManager::getWiFiPassword() const
{
    return wifiPassword;
}

/**
 * @brief Sets the WiFi password and saves the configuration.
 *
 * @param password The WiFi password to be set.
 */
void ConfigManager::setWiFiPassword(const String &password)
{
    wifiPassword = password;
    saveConfiguration();
}

/**
 * @brief Gets the stored Bluetooth configuration.
 *
 * @return BluetoothConfig The stored Bluetooth configuration.
 */
BluetoothConfig ConfigManager::getBluetoothConfig() const
{
    return bluetoothConfig;
}

/**
 * @brief Sets the Bluetooth configuration and saves it to the storage.
 *
 * @param config The Bluetooth configuration to be set.
 */
void ConfigManager::setBluetoothConfig(const BluetoothConfig &config)
{
    bluetoothConfig = config;
    saveConfiguration();
}

/**
 * @brief Gets the stored LLM Endpoint configuration.
 *
 * @return LLMEndpointConfig The stored LLM Endpoint configuration.
 */
LLMEndpointConfig ConfigManager::getLLMEndpointConfig() const
{
    return llmEndpointConfig;
}

/**
 * @brief Sets the LLM Endpoint configuration and saves it to the storage.
 *
 * @param config The LLM Endpoint configuration structure containing the host, port, type, and endpoints.
 */
void ConfigManager::setLLMEndpointConfig(const LLMEndpointConfig &config)
{
    llmEndpointConfig = config;
    saveConfiguration();
}

/**
 * @brief Gets the stored user configuration.
 *
 * @return UserConfig The stored user configuration including name, demographics, username, and password.
 */
UserConfig ConfigManager::getUserConfig() const
{
    return userConfig;
}

/**
 * @brief Sets the user configuration and saves it to the storage.
 *
 * @param config The UserConfig structure containing user details to be set.
 */
void ConfigManager::setUserConfig(const UserConfig &config)
{
    userConfig = config;
    saveConfiguration();
}

/**
 * @brief Gets the stored system command.
 *
 * @return String The stored system command.
 */
String ConfigManager::getSystemCommand() const
{
    return systemCommand;
}

/**
 * @brief Sets the system command and saves it to the storage.
 *
 * @param command The system command to be set.
 */
void ConfigManager::setSystemCommand(const String &command)
{
    systemCommand = command;
    saveConfiguration();
}
