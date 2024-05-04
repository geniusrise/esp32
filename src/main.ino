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

#define PIN_MIC_BCK GPIO_NUM_48
#define PIN_MIC_WS GPIO_NUM_18
#define PIN_MIC_DATA GPIO_NUM_16

#define PIN_SPEAKER_LEFT_OUT GPIO_NUM_38
#define PIN_SPEAKER_RIGHT_OUT GPIO_NUM_39

#define PIN_SD_CARD_CS GPIO_NUM_34
#define PIN_SD_CARD_MOSI GPIO_NUM_35
#define PIN_SD_CARD_CLK GPIO_NUM_36
#define PIN_SD_CARD_MISO GPIO_NUM_37

#define INITIAL_DELAY 300
#define MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND 2

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
Speaker speaker = Speaker(PIN_SPEAKER_LEFT_OUT, PIN_SPEAKER_RIGHT_OUT);

bool IN_CONFIG_MODE = false;
String ipAddress;

int touchPinPressedCycles = 0;
String current_filename;

String current_emotion;

#define OPENAI_KEY "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"

void
setup()
{

  // Debug SD card connections
  // int ctr = 0;
  // while (1) {
  //   ctr++;

  //   pinMode(TOUCH_PIN, OUTPUT);

  //   pinMode(PIN_MIC_BCK, OUTPUT);
  //   pinMode(PIN_MIC_WS, OUTPUT);
  //   pinMode(PIN_MIC_DATA, OUTPUT);

  //   pinMode(PIN_SPEAKER_LEFT_OUT, OUTPUT);
  //   pinMode(PIN_SPEAKER_RIGHT_OUT, OUTPUT);

  //   pinMode(PIN_SD_CARD_CS, OUTPUT);
  //   pinMode(PIN_SD_CARD_MOSI, OUTPUT);
  //   pinMode(PIN_SD_CARD_CLK, OUTPUT);
  //   pinMode(PIN_SD_CARD_MISO, OUTPUT);

  //   digitalWrite(TOUCH_PIN, ctr % 2);

  //   digitalWrite(PIN_MIC_BCK, ctr % 2);
  //   digitalWrite(PIN_MIC_WS, ctr % 2);
  //   digitalWrite(PIN_MIC_DATA, ctr % 2);

  //   digitalWrite(PIN_SPEAKER_LEFT_OUT, ctr % 2);
  //   digitalWrite(PIN_SPEAKER_RIGHT_OUT, ctr % 2);

  //   digitalWrite(PIN_SD_CARD_CS, ctr % 2);
  //   digitalWrite(PIN_SD_CARD_MOSI, ctr % 2);
  //   digitalWrite(PIN_SD_CARD_CLK, ctr % 2);
  //   digitalWrite(PIN_SD_CARD_MISO, ctr % 2);

  //   delay(2000);
  // }

  Serial.begin(115200);
  delay(INITIAL_DELAY);

  print_logo();
  delay(INITIAL_DELAY);

  Serial.println("----------- Init SD Card -----------");
  sd.begin();
  delay(INITIAL_DELAY);

  Serial.println("----------- Init display -----------");
  display.begin();
  delay(INITIAL_DELAY);

  Serial.println("----------- Init Speaker -----------");
  speaker.begin();
  delay(INITIAL_DELAY);

  pinMode(TOUCH_PIN, INPUT);

  Serial.println("----------- Init Wifi -----------");
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

  Serial.println("----------- Init Done, device is up -----------");
  delay(INITIAL_DELAY);
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

  if (current_emotion != image_face_with_raised_eyebrow) {
    display.showEmotion(image_face_with_raised_eyebrow.c_str());
    current_emotion = image_face_with_raised_eyebrow;
  }
}

void
normal_loop()
{
  if (digitalRead(TOUCH_PIN) == HIGH) {
    // If GPIO17 is HIGH, show the surprised face
    touchPinPressedCycles += 1;
    if (touchPinPressedCycles == MAX_TOUCH_BUTTON_CYCLES_TO_RESPOND) {

      if (current_emotion != image_cowboy_hat_face) {
        display.showEmotion(image_cowboy_hat_face.c_str());
        current_emotion = image_cowboy_hat_face;
      }

      Serial.println("Start: Listening to user via mic\n");

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

      Serial.println("Stop: Listening to user via mic\n");
      if (current_emotion != image_thinking_face) {
        display.showEmotion(image_thinking_face.c_str());
        current_emotion = image_thinking_face;
      }
      mic.stopRecording();

      // call speech to text API
      OpenAI openai(OPENAI_KEY);
      String response =
        openai.createTranscription(current_filename.c_str(), "whisper-1");

      // then call the LLM

      // finally call text to speech API
    } else {
      // Otherwise, show the happy face
      Serial.println(".");
      if (current_emotion != image_robot) {
        display.showEmotion(image_robot.c_str());
        current_emotion = image_robot;
      }
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
