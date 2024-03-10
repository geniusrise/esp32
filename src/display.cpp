#include "display.h"
#include "svgs/svgs.h"

// Add more as needed, with appropriate external declarations

// Assuming dimensions for simplicity
#define BITMAP_WIDTH 128
#define BITMAP_HEIGHT 128

Display::Display()
{
  // Constructor body if needed
}

void
Display::begin()
{
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_WHITE); // Start with a black screen
}

bool
Display::showEmotion(const String& emotion)
{
  const unsigned short* bitmap = nullptr;

  if (emotion == "grinning") {
    bitmap = image_grinning;
    // } else if (emotion == "grin") {
    //   bitmap = image_grin;
    // } else if (emotion == "joy") {
    //   bitmap = image_joy;
  } else if (emotion == "smiley") {
    bitmap = image_smiley;
    // } else if (emotion == "smile") {
    //   bitmap = image_smile;
    // } else if (emotion == "sweat_smile") {
    //   bitmap = image_sweat_smile;
    // } else if (emotion == "laughing") {
    //   bitmap = image_laughing;
    // } else if (emotion == "innocent") {
    //   bitmap = image_innocent;
    // } else if (emotion == "wink") {
    //   bitmap = image_wink;
    // } else if (emotion == "neutral_face") {
    //   bitmap = image_neutral_face;
    // } else if (emotion == "expressionless") {
    //   bitmap = image_expressionless;
    // } else if (emotion == "unamused") {
    //   bitmap = image_unamused;
    // } else if (emotion == "sweat") {
    //   bitmap = image_sweat;
    // } else if (emotion == "pensive") {
    //   bitmap = image_pensive;
    // } else if (emotion == "confused") {
    //   bitmap = image_confused;
    // } else if (emotion == "confounded") {
    //   bitmap = image_confounded;
    // } else if (emotion == "kissing") {
    //   bitmap = image_kissing;
    // } else if (emotion == "kissing_heart") {
    //   bitmap = image_kissing_heart;
    // } else if (emotion == "kissing_smiling_eyes") {
    //   bitmap = image_kissing_smiling_eyes;
    // } else if (emotion == "angry") {
    //   bitmap = image_angry;
    // } else if (emotion == "cry") {
    //   bitmap = image_cry;
    // } else if (emotion == "persevere") {
    //   bitmap = image_persevere;
    // } else if (emotion == "triumph") {
    //   bitmap = image_triumph;
    // } else if (emotion == "disappointed_relieved") {
    //   bitmap = image_disappointed_relieved;
    // } else if (emotion == "frowning") {
    //   bitmap = image_frowning;
    // } else if (emotion == "anguished") {
    //   bitmap = image_anguished;
    // } else if (emotion == "fearful") {
    //   bitmap = image_fearful;
    // } else if (emotion == "weary") {
    //   bitmap = image_weary;
    // } else if (emotion == "cold_sweat") {
    //   bitmap = image_cold_sweat;
    // } else if (emotion == "scream") {
    //   bitmap = image_scream;
    // } else if (emotion == "astonished") {
    //   bitmap = image_astonished;
    // } else if (emotion == "flushed") {
    //   bitmap = image_flushed;
    // } else if (emotion == "sleeping") {
    //   bitmap = image_sleeping;
    // } else if (emotion == "dizzy_face") {
    //   bitmap = image_dizzy_face;
    // } else if (emotion == "dizzy_face") {
    //   bitmap = image_dizzy_face;
    // } else if (emotion == "mask") {
    //   bitmap = image_mask;
    // } else if (emotion == "slightly_frowning_face") {
    //   bitmap = image_slightly_frowning_face;
    // } else if (emotion == "slightly_smiling_face") {
    //   bitmap = image_slightly_smiling_face;
    // } else if (emotion == "face_with_rolling_eyes") {
    //   bitmap = image_face_with_rolling_eyes;
    // } else if (emotion == "zipper_mouth_face") {
    //   bitmap = image_zipper_mouth_face;
    // } else if (emotion == "money_mouth_face") {
    //   bitmap = image_money_mouth_face;
    // } else if (emotion == "nerd_face") {
    //   bitmap = image_nerd_face;
  } else if (emotion == "thinking_face") {
    bitmap = image_thinking_face;
    // } else if (emotion == "hugging_face") {
    //   bitmap = image_hugging_face;
    // } else if (emotion == "cowboy_hat_face") {
    //   bitmap = image_cowboy_hat_face;
    // } else if (emotion == "nauseated_face") {
    //   bitmap = image_nauseated_face;
    // } else if (emotion == "rofl") {
    //   bitmap = image_rofl;
    // } else if (emotion == "drooling_face") {
    //   bitmap = image_drooling_face;
    // } else if (emotion == "sneezing_face") {
    //   bitmap = image_sneezing_face;
  } else if (emotion == "face_with_raised_eyebrow") {
    bitmap = image_face_with_raised_eyebrow;
    // } else if (emotion == "starstruck") {
    //   bitmap = image_starstruck;
  }

  if (bitmap != nullptr) {
    displayBitmap(bitmap, 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);
    return true;
  } else {
    // Emotion not found, you could clear the display or leave as is.
    // tft.fillScreen(TFT_BLACK); // Optional: clear screen if emotion not found
    return false;
  }
}

void
Display::displayBitmap(const unsigned short* bitmap, int x, int y, int w, int h)
{

  tft.setSwapBytes(false);
  tft.invertDisplay(1);

  tft.startWrite(); // Begin write operation
  // tft.setAddrWindow(x, y, w, h);
  tft.pushImage(0, 0, 128, 128, bitmap);
  tft.endWrite(); // End write operation
}
