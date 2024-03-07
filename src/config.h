#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>

struct BluetoothConfig
{
    String deviceName;
    String devicePasskey;
    bool paired;
};

struct LLMEndpointConfig
{
    String host;
    uint16_t port;
    String type;      // "openai" or "geniusrise"
    String endpoints; // Comma-separated list like "llm,multimodal,vision,audio"
};

struct UserConfig
{
    String name;
    String demographics;
    String username;
    String password;
};

class ConfigManager
{
public:
    ConfigManager();
    void loadConfiguration();
    void saveConfiguration();

    String getWiFiSSID() const;
    void setWiFiSSID(const String &ssid);

    String getWiFiPassword() const;
    void setWiFiPassword(const String &password);

    BluetoothConfig getBluetoothConfig() const;
    void setBluetoothConfig(const BluetoothConfig &config);

    LLMEndpointConfig getLLMEndpointConfig() const;
    void setLLMEndpointConfig(const LLMEndpointConfig &config);

    UserConfig getUserConfig() const;
    void setUserConfig(const UserConfig &config);

    String getSystemCommand() const;
    void setSystemCommand(const String &command);

private:
    String wifiSSID;
    String wifiPassword;
    BluetoothConfig bluetoothConfig;
    LLMEndpointConfig llmEndpointConfig;
    UserConfig userConfig;
    String systemCommand;

    void readFromFlash();
    void writeToFlash();
};

#endif // CONFIG_MANAGER_H
