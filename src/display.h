#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <TFT_eSPI.h> // Include the graphics library for the ST7735 driver chip

#define TFT_WIDTH 128
#define TFT_HEIGHT 128

const String image_grinning = "1F600";
const String image_grin = "1F601";
const String image_joy = "1F602";
const String image_smiley = "1F603";
const String image_sweat_smile = "1F605";
const String image_laughing = "1F606";
const String image_innocent = "1F607";
const String image_wink = "1F609";
const String image_neutral_face = "1F610";
const String image_expressionless = "1F611";
const String image_unamused = "1F612";
const String image_sweat = "1F613";
const String image_pensive = "1F614";
const String image_confused = "1F615";
const String image_confounded = "1F616";
const String image_kissing = "1F617";
const String image_kissing_heart = "1F618";
const String image_kissing_smiling_eyes = "1F619";
const String image_angry = "1F620";
const String image_cry = "1F622";
const String image_persevere = "1F623";
const String image_triumph = "1F624";
const String image_disappointed_relieved = "1F625";
const String image_frowning = "1F626";
const String image_anguished = "1F627";
const String image_fearful = "1F628";
const String image_weary = "1F629";
const String image_cold_sweat = "1F630";
const String image_scream = "1F631";
const String image_astonished = "1F632";
const String image_flushed = "1F633";
const String image_sleeping = "1F634";
const String image_dizzy_face = "1F635_200D_1F4AB";
const String image_dead_face = "1F635";
const String image_mask = "1F637";
const String image_slightly_frowning_face = "1F641";
const String image_slightly_smiling_face = "1F642";
const String image_face_with_rolling_eyes = "1F644";
const String image_zipper_mouth_face = "1F910";
const String image_money_mouth_face = "1F911";
const String image_nerd_face = "1F913";
const String image_thinking_face = "1F914";
const String image_hugging_face = "1F917";
const String image_cowboy_hat_face = "1F920";
const String image_nauseated_face = "1F922";
const String image_rofl = "1F923";
const String image_drooling_face = "1F924";
const String image_sneezing_face = "1F927";
const String image_face_with_raised_eyebrow = "1F928";
const String image_starstruck = "1F929";

class Display
{
public:
  Display();    // Constructor to initialize the display
  void begin(); // Initialize the TFT display

  // Function to display an emotion if it exists
  bool showEmotion(const char* emotion);

private:
  TFT_eSPI tft = TFT_eSPI(); // Create a TFT_eSPI object

  // Function to display a bitmap
  void displayBitmap(File& bitmapFile);
};

#endif // DISPLAY_H
