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

    server.on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request) {
        restart(request);
    });
}

void ServerManager::serveIndex(AsyncWebServerRequest *request)
{
String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Geniusrise Configuration</title>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Orbitron&display=swap');

        body {
            font-family: 'Orbitron', sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #181a27;
            color: #e0e0e0;
            background-size: 40px 40px;
            background-position: 0 0, 20px 20px;
        }
        h1 {
            font-size: 6rem;
            font-weight: 900;
            color: yellow;

            text-align: center;
            text-transform: capitalize;
            text-shadow:
                6px 6px 0 #000, /* Shadow right and bottom */
                -1px -1px 0 #000, /* Shadow top and left */
                1px -1px 0 #000, /* Shadow top and right */
                -1px 1px 0 #000, /* Shadow bottom and left */
                0 0 5px #000, /* Soft outer glow for depth */
                8px 8px 0 #000, /* Additional layer for 3D effect */
                10px 10px 0 #000; /* Additional layer for 3D effect */
        }
        form {
            max-width: 900px;
            margin: 0 auto;
            background-color: #1a1a1a;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 0 20px rgba(0, 255, 255, 0.3);
        }
        fieldset {
            border: none;
            margin-bottom: 30px;
        }
        legend {
            font-size: 1.5rem;
            font-weight: bold;
            color: white;
            margin-bottom: 10px;
        }
        label {
            display: block;
            margin-top: 10px;
            color: #e0e0e0;
        }
        input[type="text"],
        input[type="password"] {
            width: 100%;
            padding: 10px;
            margin-top: 5px;
            border: none;
            background-color: #333333;
            color: #e0e0e0;
            border-radius: 5px;
        }
        input[type="submit"],
        input[type="button"] {
            display: block;
            width: 100%;
            padding: 12px;
            margin-top: 20px;
            background-color: white;
            color: #0a0a0a;
            border: none;
            cursor: pointer;
            font-size: 1.1rem;
            text-transform: uppercase;
            letter-spacing: 1px;
            transition: background-color 0.3s ease;
            border-radius: 5px;
        }
        input[type="submit"]:hover,
        input[type="button"]:hover {
            background-color: #00cccc;
        }
    </style>
</head>
<body>
    <h1>Geniusrise</h1>
    <form>
        <fieldset>
            <legend>WiFi</legend>
            <label for="wifi-ssid">WiFi SSID:</label>
            <input type="text" id="wifi-ssid" name="wifi-ssid" placeholder="Enter the SSID of your WiFi network" required>

            <label for="wifi-password">WiFi Password:</label>
            <input type="password" id="wifi-password" name="wifi-password" placeholder="Enter the password for your WiFi network" required>
        </fieldset>

        <fieldset>
            <legend>Bluetooth</legend>
            <label for="bt-device-name">Bluetooth Device Name:</label>
            <input type="text" id="bt-device-name" name="bt-device-name" placeholder="Enter a name for your Bluetooth device">

            <label for="bt-device-passkey">Bluetooth Device Passkey:</label>
            <input type="text" id="bt-device-passkey" name="bt-device-passkey" placeholder="Enter a passkey for your Bluetooth device">
        </fieldset>

        <fieldset>
            <legend>LLM Endpoint</legend>
            <label for="llm-host">Base URL:</label>
            <input type="text" id="llm-host" name="llm-host" placeholder="Enter the hostname or IP address of the LLM endpoint">

            <label for="llm-port">Endpoint:</label>
            <input type="text" id="llm-port" name="llm-port" placeholder="Enter the port number of the LLM endpoint">

            <label for="llm-type">Endpoint Type (openai, ollama, vllm, llama.cpp, geniusrise):</label>
            <input type="text" id="llm-type" name="llm-type" placeholder="Enter the type of LLM being used">

            <label for="llm-endpoints">LLM Endpoints:</label>
            <input type="text" id="llm-endpoints" name="llm-endpoints" placeholder="Enter the available LLM endpoints">
        </fieldset>

        <fieldset>
            <legend>User</legend>
            <label for="user-name">User Name:</label>
            <input type="text" id="user-name" name="user-name" placeholder="Enter your name" required>

            <label for="user-demographics">User Demographics:</label>
            <input type="text" id="user-demographics" name="user-demographics" placeholder="Enter your demographic information" required>

            <label for="user-username">User Username:</label>
            <input type="text" id="user-username" name="user-username" placeholder="Enter your username" required>

            <label for="user-password">User Password:</label>
            <input type="password" id="user-password" name="user-password" placeholder="Enter your password" required>
        </fieldset>

        <fieldset>
            <legend>System</legend>
            <label for="system-command">System Command:</label>
            <input type="text" id="system-command" name="system-command" placeholder="Enter a system command - optional">
        </fieldset>

        <input type="submit" value="Save Configuration">
        <input type="button" id="reset-button" value="Reset Board">
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

        function resetBoard() {
            if (confirm("Are you sure you want to reset the board?")) {
                fetch('/reset')
                    .then(response => {
                        console.log('Board is resetting...');
                    })
                    .catch(error => {
                        console.error('Error:', error);
                        alert('An error occurred while resetting the board.');
                    });
            }
        }

        const resetButton = document.getElementById('reset-button');
        resetButton.addEventListener('click', async (event) => {
            event.preventDefault();
            resetBoard();
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


void ServerManager::restart(AsyncWebServerRequest *request)
{
    request->send(200);
    ESP.restart();
}
