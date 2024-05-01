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
const String image_zany_face = "1F92A";
const String image_shushing_face = "1F92B";
const String image_face_with_symbols_over_mouth = "1F92C";
const String image_face_with_hand_over_mouth = "1F92D";
const String image_face_vomiting = "1F92E";
const String image_exploding_head = "1F92F";
const String image_smiling_face_with_3_hearts = "1F970";
const String image_yawning_face = "1F971";
const String image_partying_face = "1F973";
const String image_woozy_face = "1F974";
const String image_hot_face = "1F975";
const String image_cold_face = "1F976";
const String image_pleading_face = "1F97A";
const String image_face_with_monocle = "1F9D0";
const String image_star_struck = "1F929";
const String image_face_with_open_mouth = "1F62E";
const String image_face_with_tongue = "1F61B";
const String image_face_without_mouth = "1F636";
const String image_face_with_medical_mask = "1F637";
const String image_face_with_thermometer = "1F912";
const String image_face_with_head_bandage = "1F915";
const String image_lying_face = "1F925";
const String image_grimacing_face = "1F62C";
const String image_hushed_face = "1F62F";
const String image_anxious_face_with_sweat = "1F630";
const String image_face_with_cowboy_hat = "1F920";
const String image_clown_face = "1F921";
const String image_lying_face_with_tongue = "1F925";
const String image_shushing_face_with_finger = "1F92B";
const String image_face_with_raised_eyebrow_and_monocle = "1F9D0";
const String image_smiling_face_with_heart_eyes = "1F60D";
const String image_smiling_face_with_sunglasses = "1F60E";
const String image_smiling_face_with_halo = "1F607";
const String image_smiling_face_with_horns = "1F608";
const String image_angry_face_with_horns = "1F47F";
const String image_skull = "1F480";
const String image_skull_and_crossbones = "2620";
const String image_pile_of_poo = "1F4A9";
const String image_ghost = "1F47B";
const String image_alien = "1F47D";
const String image_alien_monster = "1F47E";
const String image_robot = "1F916";
const String image_jack_o_lantern = "1F383";

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
