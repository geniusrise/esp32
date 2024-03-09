#include "config.h"
#include "driver/gpio.h"
#include "server.h"
#include "speaker.h"
#include "util.h"
#include "wifi.h"
#include <Arduino.h>

ConfigManager config = ConfigManager();
ServerManager server = ServerManager(config);
WiFiManager wiFiManager = WiFiManager(config, server);

bool IN_CONFIG_MODE = false;
String ipAddress;

void
setup()
{
  // Initialize the state machine, which will also initialize all other managers
  print_logo();

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

  // String ssid = config.getWiFiSSID();
  // String password = config.getWiFiPassword();
  // if (ssid != "") {
  //     color_printf("Going into AP mode for setup......");
  //     IN_CONFIG_MODE = false;
  //     wiFiManager.connectToSavedNetwork(ssid.c_str(), password.c_str());
  //     server.begin();
  //     ipAddress = wiFiManager.getIPAddress().toString();
  // }
}

void
normal_loop()
{
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
