#include <Arduino.h>
#include "speaker.h"
#include "driver/gpio.h"
#include "util.h"
#include "wifi.h"
#include "config.h"
#include "server.h"


int SPEAKER = 18;
int count = 0;

SpeakerManager speakerManager = SpeakerManager(SPEAKER);
ConfigManager config = ConfigManager();
ServerManager server = ServerManager(config);
WiFiManager wiFiManager = WiFiManager(config, server);

bool IN_CONFIG_MODE = false;

void setup() {
    // Initialize the state machine, which will also initialize all other managers
    print_logo();

    String ssid = config.getWiFiSSID();
    if (ssid == "") {
        color_printf("Going into AP mode for setup......");
        IN_CONFIG_MODE = true;
        wiFiManager.setupAPMode("geniusrise", "hellogenius");

    } else {
        wiFiManager.connectToSavedNetwork(ssid.c_str(), config.getWiFiPassword().c_str());
    }
}

void setup_loop() {
    color_printf("------ ------ ------ ------ ------ ------ ------ ------ ------\n");
    color_printf("Main: Setup loop, wifi ssid: geniusrise, password: hellogenius\n");
    color_printf("------ ------ ------ ------ ------ ------ ------ ------ ------\n");
}

void normal_loop() {
    color_printf("------ ------ ------ ------ ------ ------ ------ ------ ------\n");
    color_printf("Main: Start main loop\n");

    speakerManager.playSound(count);
    count += 1;
    if (count > 7) {
        count = 0;
    }

    color_printf("------ ------ ------ ------ ------ ------ ------ ------ ------\n");
    delay(500);
}

void loop() {
    if (IN_CONFIG_MODE) {
        setup_loop();
    } else {
        normal_loop();
    }
}
