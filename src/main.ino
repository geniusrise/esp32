#include "config.h"
#include "display.h"
#include "driver/gpio.h"
#include "mic.h"
#include "openai.h"
#include "server.h"
#include "speaker.h"
#include "util.h"
#include "wifi.h"
#include <Arduino.h>
#include <NTPClient.h>

#define TOUCH_PIN 17
#define MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND 2

#define PIN_MIC_BCK 33
#define PIN_MIC_WS 34
#define PIN_MIC_DATA 16

#define PIN_AUDIO_KIT_SD_CARD_CS 48
#define PIN_AUDIO_KIT_SD_CARD_MOSI 37
#define PIN_AUDIO_KIT_SD_CARD_CLK 36
#define PIN_AUDIO_KIT_SD_CARD_MISO 35

// Configuration
ConfigManager config = ConfigManager();

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 3600, 60000);

// Setup
ServerManager server = ServerManager(config);
WiFiManager wiFiManager = WiFiManager(config, server);

// Peripherals
Display display = Display();
MicManager mic = MicManager(PIN_MIC_BCK,
                            PIN_MIC_WS,
                            PIN_MIC_DATA,
                            PIN_AUDIO_KIT_SD_CARD_CS,
                            PIN_AUDIO_KIT_SD_CARD_MISO,
                            PIN_AUDIO_KIT_SD_CARD_MOSI,
                            PIN_AUDIO_KIT_SD_CARD_CLK);

bool IN_CONFIG_MODE = false;
String ipAddress;

int touchPinPressedCycles = 0;
String current_filename;

#define OPENAI_KEY "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"

void
setup()
{
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

    timeClient.begin();
    while (!timeClient.update()) {
      timeClient.forceUpdate();
    }

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

  delay(1000);
}

void
normal_loop()
{
  if (digitalRead(TOUCH_PIN) == HIGH) {
    // If GPIO17 is HIGH, show the surprised face
    touchPinPressedCycles += 1;
    if (touchPinPressedCycles == MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND) {

      display.showEmotion("cowboy_hat_face");
      printf("Start: Listening to user via mic\n");

      // Prepare filename
      String now = timeClient.getFormattedTime();
      now.replace(":", "-");
      int randomPart = random(1000, 9999);

      current_filename = "/" + now + "--" + randomPart + ".mp3";
      mic.startRecording(current_filename);
    } else if (touchPinPressedCycles > MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND) {
      mic.record();
    }

  } else {
    if (touchPinPressedCycles > 0) {
      touchPinPressedCycles = 0;

      printf("Stop: Listening to user via mic\n");
      display.showEmotion("thinking_face");
      mic.stopRecording();

      // call speech to text API
      OpenAI openai(OPENAI_KEY);
      String response =
        openai.createTranscription(current_filename.c_str(), "whisper-1");

      // then call the LLM

      // finally call text to speech API
    } else {
      display.showEmotion("smiley"); // Otherwise, show the happy face
    }
  }

  // delay(500);
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
