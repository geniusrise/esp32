#include "server.h"

ServerManager::ServerManager(ConfigManager &configManager)
    : server(80), configManager(configManager)
{
}

void ServerManager::begin()
{
    setupRoutes();
    server.begin();
}

void ServerManager::handleClient()
{
    server.handleClient();
}

void ServerManager::setupRoutes()
{
    server.on("/", HTTP_GET, std::bind(&ServerManager::serveIndex, this));
    server.on("/config", HTTP_GET, std::bind(&ServerManager::getConfig, this));
    server.on("/config", HTTP_POST, std::bind(&ServerManager::setConfig, this));
}

void ServerManager::serveIndex()
{
    String contentType, content;
    if (loadFromSPIFFS("/index.html", contentType, content))
    {
        server.send(200, contentType, content);
    }
    else
    {
        server.send(404, "text/plain", "File Not Found");
    }
}

void ServerManager::getConfig()
{
    DynamicJsonDocument doc(2048); // Adjust size as needed based on your configuration complexity

    // WiFi Configuration
    doc["wifi"]["ssid"] = configManager.getWiFiSSID();
    doc["wifi"]["password"] = configManager.getWiFiPassword(); // Ensure this is secure/safe to expose

    // Bluetooth Configuration
    doc["bluetooth"]["deviceName"] = configManager.getBluetoothConfig().deviceName;
    doc["bluetooth"]["devicePasskey"] = configManager.getBluetoothConfig().devicePasskey;
    doc["bluetooth"]["paired"] = configManager.getBluetoothConfig().paired;

    // LLM Endpoint Configuration
    doc["llm"]["host"] = configManager.getLLMEndpointConfig().host;
    doc["llm"]["port"] = configManager.getLLMEndpointConfig().port;
    doc["llm"]["type"] = configManager.getLLMEndpointConfig().type;
    doc["llm"]["endpoints"] = configManager.getLLMEndpointConfig().endpoints;

    // User Configuration
    doc["user"]["name"] = configManager.getUserConfig().name;
    doc["user"]["demographics"] = configManager.getUserConfig().demographics;
    doc["user"]["username"] = configManager.getUserConfig().username;
    doc["user"]["password"] = configManager.getUserConfig().password; // Consider security implications

    // System Command
    doc["systemCommand"] = configManager.getSystemCommand();

    String output;
    serializeJson(doc, output);
    server.send(200, "application/json", output);
}

void ServerManager::setConfig()
{
    if (!server.hasArg("plain"))
    { // Check if body received
        server.send(400, "text/plain", "Request body not received");
        return;
    }

    DynamicJsonDocument doc(2048); // Adjust size based on your needs
    DeserializationError error = deserializeJson(doc, server.arg("plain"));

    if (error)
    {
        server.send(400, "text/plain", "Failed to parse request body");
        return;
    }

    // Update WiFi Configuration
    configManager.setWiFiSSID(doc["wifi"]["ssid"].as<String>());
    configManager.setWiFiPassword(doc["wifi"]["password"].as<String>());

    // Update Bluetooth Configuration
    BluetoothConfig btConfig = {
        doc["bluetooth"]["deviceName"].as<String>(),
        doc["bluetooth"]["devicePasskey"].as<String>(),
        doc["bluetooth"]["paired"].as<bool>()};
    configManager.setBluetoothConfig(btConfig);

    // Update LLM Endpoint Configuration
    LLMEndpointConfig llmConfig = {
        doc["llm"]["host"].as<String>(),
        static_cast<uint16_t>(doc["llm"]["port"].as<unsigned int>()), // Casting to uint16_t
        doc["llm"]["type"].as<String>(),
        doc["llm"]["endpoints"].as<String>()};
    configManager.setLLMEndpointConfig(llmConfig);

    // Update User Configuration
    UserConfig userConfig = {
        doc["user"]["name"].as<String>(),
        doc["user"]["demographics"].as<String>(),
        doc["user"]["username"].as<String>(),
        doc["user"]["password"].as<String>()};
    configManager.setUserConfig(userConfig);

    // Update System Command
    if (doc.containsKey("systemCommand"))
    {
        configManager.setSystemCommand(doc["systemCommand"].as<String>());
    }

    configManager.saveConfiguration();
    server.send(200, "application/json", "{\"message\":\"Configuration updated successfully\"}");
}

String ServerManager::getContentType(const String &filename)
{
    // Simplified content type determination
    if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else
        return "text/plain";
}

bool ServerManager::loadFromSPIFFS(String path, String &contentType, String &content)
{
    // Implement file loading from SPIFFS/LittleFS
    // Placeholder implementation
    return false;
}
