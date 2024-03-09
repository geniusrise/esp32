#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <Arduino.h>
#include <map>

struct UserConfig
{
    String name;
    String username;
    String password;
    String userSettings;
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

    UserConfig getUserConfig() const;
    void setUserConfig(const UserConfig &config);

private:
    String wifiSSID;
    String wifiPassword;
    UserConfig userConfig;
    std::map<String, String> customConfig;
};

#endif // CONFIG_MANAGER_HPP