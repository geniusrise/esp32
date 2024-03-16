#include "config.h"
#include "display.h"
#include "driver/gpio.h"
#include "server.h"
#include "speaker.h"
#include "util.h"
#include "wifi.h"
#include <Arduino.h>

#define TOUCH_PIN 17
#define MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND 2

ConfigManager config = ConfigManager();
ServerManager server = ServerManager(config);
WiFiManager wiFiManager = WiFiManager(config, server);
Display display = Display();

bool IN_CONFIG_MODE = false;
String ipAddress;

int touchPinPressedCycles = 0;

void
setup()
{
  // Initialize the state machine, which will also initialize all other managers
  delay(100);

  print_logo();
  display.begin();
  display.showEmotion("smiley");

  pinMode(TOUCH_PIN, INPUT);

  String ssid = config.getWiFiSSID();
  String password = config.getWiFiPassword();
  if (ssid == "") {
    color_printf("Going into AP mode for setup......");
    IN_CONFIG_MODE = true;
    wiFiManager.setupAPMode("geniusrise", "hellogenius");

  } else {
    wiFiManager.connectToSavedNetwork(ssid.c_str(),
                                      config.getWiFiPassword().c_str());
    server.begin();
    ipAddress = wiFiManager.getIPAddress().toString();
  }
}

void
setup_loop()
{
  color_printf(
    "------ ------ ------ ------ ------ ------ ------ ------ ------\n");
  color_printf("Main: Setup loop: connect to ssid: geniusrise, password: "
               "hellogenius, then visit http://192.168.218.1\n");
  color_printf(
    "------ ------ ------ ------ ------ ------ ------ ------ ------\n");
}

void
normal_loop()
{
  if (digitalRead(TOUCH_PIN) == HIGH) {
    // If GPIO17 is HIGH, show the surprised face
    touchPinPressedCycles += 1;
    if (touchPinPressedCycles > MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND) {
      display.showEmotion("cowboy_hat_face");
    }
  } else {
    if (touchPinPressedCycles > 0) {
      display.showEmotion("thinking_face");
    } else {
      display.showEmotion("smiley"); // Otherwise, show the happy face
    }
  }

  color_printf(
    "------ ------ ------ ------ ------ ------ ------ ------ ------\n");
  color_printf("Main: Start main loop, ip:");
  printf("%s\n", ipAddress.c_str());

  color_printf(
    "------ ------ ------ ------ ------ ------ ------ ------ ------\n");
  delay(500);
}

void
loop()
{
  if (IN_CONFIG_MODE) {
    setup_loop();
  } else {
    normal_loop();
  }
}
