#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "config.h"

class ServerManager
{
public:
    ServerManager(ConfigManager &configManager);
    void begin();

private:
    AsyncWebServer server;
    ConfigManager &configManager;

    void setupRoutes();
    void serveIndex(AsyncWebServerRequest *request);
    void getConfig(AsyncWebServerRequest *request);
    void setConfig(AsyncWebServerRequest *request);
    void restart(AsyncWebServerRequest *request);
    void handleConfigBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
};

#endif // SERVER_MANAGER_HPP
