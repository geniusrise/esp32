#include "server.h"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

ServerManager::ServerManager(ConfigManager &configManager)
    : server(80), configManager(configManager) {}

void ServerManager::begin() {
  setupRoutes();
  server.begin();
}

void ServerManager::setupRoutes() {
  server.on("/", HTTP_GET,
            [this](AsyncWebServerRequest *request) { serveIndex(request); });

  server.on("/config", HTTP_GET,
            [this](AsyncWebServerRequest *request) { getConfig(request); });

  server.on(
      "/config", HTTP_POST,
      [this](AsyncWebServerRequest *request) { setConfig(request); }, NULL,
      [this](AsyncWebServerRequest *request, uint8_t *data, size_t len,
             size_t index, size_t total) {
        handleConfigBody(request, data, len, index, total);
      });

  server.on("/reset", HTTP_GET,
            [this](AsyncWebServerRequest *request) { restart(request); });
}

void ServerManager::serveIndex(AsyncWebServerRequest *request) {
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
            <legend>User</legend>
            <label for="user-name">Name:</label>
            <input type="text" id="user-name" name="user-name" placeholder="What would you like me to call you?" required>

            <label for="user-username">User Username:</label>
            <input type="text" id="user-username" name="user-username" placeholder="Enter your geniusrise username" required>

            <label for="user-password">User Password:</label>
            <input type="password" id="user-password" name="user-password" placeholder="Enter your geniusrise key" required>

            <label for="user-settings">User Settings:</label>
            <input type="text" id="user-settings" name="user-settings" placeholder="Enter custom user settings JSON">
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

void ServerManager::getConfig(AsyncWebServerRequest *request) {
  DynamicJsonDocument doc(
      2048); // Adjust size as needed based on your configuration complexity

  // WiFi Configuration
  doc["wifi-ssid"] = configManager.getWiFiSSID();
  doc["wifi-password"] =
      configManager.getWiFiPassword(); // Ensure this is secure/safe to expose

  // User Configuration
  doc["user-name"] = configManager.getUserConfig().name;
  doc["user-username"] = configManager.getUserConfig().username;
  doc["user-password"] =
      configManager.getUserConfig().password; // Consider security implications
  doc["user-settings"] = configManager.getUserConfig().userSettings;

  String output;
  serializeJson(doc, output);
  request->send(200, "application/json", output);
}

void ServerManager::setConfig(AsyncWebServerRequest *request) {
  request->send(200);
}

void ServerManager::handleConfigBody(AsyncWebServerRequest *request,
                                     uint8_t *data, size_t len, size_t index,
                                     size_t total) {
  DynamicJsonDocument doc(2048); // Adjust size based on your needs
  DeserializationError error = deserializeJson(doc, data, len);

  if (error) {
    request->send(400, "text/plain", "Failed to parse request body");
    return;
  }

  // Update WiFi Configuration
  String wifiSSID = doc["wifi-ssid"].as<String>();
  String wifiPassword = doc["wifi-password"].as<String>();

  if (!wifiSSID.isEmpty()) {
    configManager.setWiFiSSID(wifiSSID);
  }

  if (!wifiPassword.isEmpty()) {
    configManager.setWiFiPassword(wifiPassword);
  }

  // Update User Configuration
  String userName = doc["user-name"].as<String>();
  String userUsername = doc["user-username"].as<String>();
  String userPassword = doc["user-password"].as<String>();
  String userSettings = doc["user-settings"].as<String>();

  UserConfig currentConfig = configManager.getUserConfig();

  UserConfig userConfig = {
      userName.isEmpty() ? currentConfig.name : userName,
      userUsername.isEmpty() ? currentConfig.username : userUsername,
      userPassword.isEmpty() ? currentConfig.password : userPassword,
      userSettings.isEmpty() ? currentConfig.userSettings : userSettings};

  configManager.setUserConfig(userConfig);

  configManager.saveConfiguration();
  request->send(200, "application/json",
                "{\"message\":\"Configuration updated successfully\"}");
}

void ServerManager::restart(AsyncWebServerRequest *request) {
  request->send(200);
  ESP.restart();
}
