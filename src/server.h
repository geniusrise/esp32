#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <WebServer.h>
#include "config.h"

class ServerManager
{
public:
    ServerManager(ConfigManager &configManager);
    void begin();
    void handleClient();

private:
    WebServer server;
    ConfigManager &configManager;

    void setupRoutes();
    void serveIndex();
    void getConfig();
    void setConfig();
    static String getContentType(const String &filename);
    static bool loadFromSPIFFS(String path, String &contentType, String &content);
};

#endif // SERVER_MANAGER_HPP
