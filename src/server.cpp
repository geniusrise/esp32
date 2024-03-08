#include "server.h"
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

ServerManager::ServerManager(ConfigManager &configManager)
    : server(80), configManager(configManager)
{
}

void ServerManager::begin()
{
    setupRoutes();
    server.begin();
}

void ServerManager::setupRoutes()
{
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        serveIndex(request);
    });

    server.on("/config", HTTP_GET, [this](AsyncWebServerRequest *request) {
        getConfig(request);
    });

    server.on("/config", HTTP_POST, [this](AsyncWebServerRequest *request) {
        setConfig(request);
    }, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        handleConfigBody(request, data, len, index, total);
    });
}

void ServerManager::serveIndex(AsyncWebServerRequest *request)
{
    String html = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>ESP32 Configuration</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    margin: 0;
                    padding: 20px;
                }
                h1 {
                    text-align: center;
                }
                form {
                    max-width: 500px;
                    margin: 0 auto;
                }
                label {
                    display: block;
                    margin-top: 10px;
                }
                input[type="text"],
                input[type="password"] {
                    width: 100%;
                    padding: 5px;
                    margin-top: 5px;
                }
                input[type="submit"] {
                    display: block;
                    width: 100%;
                    padding: 10px;
                    margin-top: 20px;
                    background-color: #4CAF50;
                    color: white;
                    border: none;
                    cursor: pointer;
                }
            </style>
        </head>
        <body>
            <h1>ESP32 Configuration</h1>
            <form>
                <label for="wifi-ssid">WiFi SSID:</label>
                <input type="text" id="wifi-ssid" name="wifi-ssid" required>

                <label for="wifi-password">WiFi Password:</label>
                <input type="password" id="wifi-password" name="wifi-password" required>

                <label for="bt-device-name">Bluetooth Device Name:</label>
                <input type="text" id="bt-device-name" name="bt-device-name" required>

                <label for="bt-device-passkey">Bluetooth Device Passkey:</label>
                <input type="text" id="bt-device-passkey" name="bt-device-passkey" required>

                <label for="llm-host">LLM Host:</label>
                <input type="text" id="llm-host" name="llm-host" required>

                <label for="llm-port">LLM Port:</label>
                <input type="text" id="llm-port" name="llm-port" required>

                <label for="llm-type">LLM Type:</label>
                <input type="text" id="llm-type" name="llm-type" required>

                <label for="llm-endpoints">LLM Endpoints:</label>
                <input type="text" id="llm-endpoints" name="llm-endpoints" required>

                <label for="user-name">User Name:</label>
                <input type="text" id="user-name" name="user-name" required>

                <label for="user-demographics">User Demographics:</label>
                <input type="text" id="user-demographics" name="user-demographics" required>

                <label for="user-username">User Username:</label>
                <input type="text" id="user-username" name="user-username" required>

                <label for="user-password">User Password:</label>
                <input type="password" id="user-password" name="user-password" required>

                <label for="system-command">System Command:</label>
                <input type="text" id="system-command" name="system-command">

                <input type="submit" value="Save Configuration">
            </form>

            <script>
                // JavaScript code to handle form submission and configuration update
                const form = document.querySelector('form');
                form.addEventListener('submit', async (event) => {
                    event.preventDefault();

                    const formData = new FormData(form);
                    const config = Object.fromEntries(formData.entries());

                    try {
                        const response = await fetch('/config', {
                            method: 'POST',
                            headers: {
                                'Content-Type': 'application/json'
                            },
                            body: JSON.stringify(config)
                        });

                        if (response.ok) {
                            alert('Configuration updated successfully!');
                        } else {
                            alert('Failed to update configuration.');
                        }
                    } catch (error) {
                        console.error('Error:', error);
                        alert('An error occurred while updating the configuration.');
                    }
                });
            </script>
        </body>
        </html>
    )";

    request->send(200, "text/html", html);
}

void ServerManager::getConfig(AsyncWebServerRequest *request)
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
    request->send(200, "application/json", output);
}

void ServerManager::setConfig(AsyncWebServerRequest *request)
{
    request->send(200);
}

void ServerManager::handleConfigBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    DynamicJsonDocument doc(2048); // Adjust size based on your needs
    DeserializationError error = deserializeJson(doc, data, len);

    if (error)
    {
        request->send(400, "text/plain", "Failed to parse request body");
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
    request->send(200, "application/json", "{\"message\":\"Configuration updated successfully\"}");
}