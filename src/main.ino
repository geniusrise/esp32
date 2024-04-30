#include "config.h"
#include "display.h"
#include "driver/gpio.h"
#include "mic.h"
#include "openai.h"
#include "sd.h"
#include "server.h"
#include "speaker.h"
#include "util.h"
#include "wifi.h"
#include <Arduino.h>
#include <NTPClient.h>

#define TOUCH_PIN GPIO_NUM_17
#define MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND 2

#define PIN_MIC_BCK GPIO_NUM_33
#define PIN_MIC_WS GPIO_NUM_34
#define PIN_MIC_DATA GPIO_NUM_16

#define PIN_SPEAKER_LEFT_OUT 1
#define PIN_SPEAKER_RIGHT_OUT 2

#define PIN_SD_CARD_CS GPIO_NUM_18
#define PIN_SD_CARD_MOSI GPIO_NUM_37
#define PIN_SD_CARD_CLK GPIO_NUM_36
#define PIN_SD_CARD_MISO GPIO_NUM_35

// Configuration
ConfigManager config = ConfigManager();

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 3600, 60000);

// Setup
ServerManager server = ServerManager(config);
WiFiManager wiFiManager = WiFiManager(config, server);

// Peripherals
SDCard sd =
  SDCard(PIN_SD_CARD_CS, PIN_SD_CARD_MISO, PIN_SD_CARD_MOSI, PIN_SD_CARD_CLK);

Display display = Display();
MicManager mic = MicManager(PIN_MIC_BCK, PIN_MIC_WS, PIN_MIC_DATA);

bool IN_CONFIG_MODE = false;
String ipAddress;

int touchPinPressedCycles = 0;
String current_filename;

#define OPENAI_KEY "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"

void
setup()
{
  delay(100);

  // gpio_set_direction(PIN_SD_CARD_CS, GPIO_MODE_OUTPUT);
  // gpio_set_direction(PIN_SD_CARD_MISO, GPIO_MODE_OUTPUT);
  // gpio_set_direction(PIN_SD_CARD_MOSI, GPIO_MODE_OUTPUT);
  // gpio_set_direction(PIN_SD_CARD_CLK, GPIO_MODE_OUTPUT);

  // digitalWrite(PIN_SD_CARD_CS, 1);
  // digitalWrite(PIN_SD_CARD_MISO, 1);
  // digitalWrite(PIN_SD_CARD_MOSI, 1);
  // digitalWrite(PIN_SD_CARD_CLK, 1);

  print_logo();
  sd.begin();
  display.begin();

  pinMode(TOUCH_PIN, INPUT);

  String ssid = config.getWiFiSSID();
  String password = config.getWiFiPassword();
  if (ssid == "") {
    color_printf("Going into AP mode for setup......");
    IN_CONFIG_MODE = true;
    wiFiManager.setupAPMode("geniusrise", "geniusrise");

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

      display.showEmotion(image_cowboy_hat_face.c_str());
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
      display.showEmotion(image_thinking_face.c_str());
      mic.stopRecording();

      // call speech to text API
      OpenAI openai(OPENAI_KEY);
      String response =
        openai.createTranscription(current_filename.c_str(), "whisper-1");

      // then call the LLM

      // finally call text to speech API
    } else {
      // Otherwise, show the happy face
      printf(".");
      display.showEmotion(image_smiley.c_str());
    }
  }

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
