#include "display.h"

// Adjusting constants for more expressive faces
#define EYE_X_LEFT (TFT_WIDTH / 3)
#define EYE_X_RIGHT (TFT_WIDTH * 2 / 3)
#define EYE_Y (TFT_HEIGHT / 3)
#define EYE_RADIUS (12) // Larger eyes for anime-like expressions

Display::Display()
{
  // Constructor body if needed
}

void
Display::begin()
{
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK); // Start with a black screen
}

void
Display::showHappyFace()
{
  tft.fillScreen(TFT_YELLOW);
  drawEyes(TFT_BLACK);
  drawMouthHappy();
}

void
Display::showSadFace()
{
  tft.fillScreen(TFT_SKYBLUE);
  drawEyes(TFT_BLACK);
  drawMouthSad();
}

void
Display::showAngryFace()
{
  tft.fillScreen(TFT_RED);
  drawEyes(TFT_WHITE);
  drawMouthAngry();
}

void
Display::showSurprisedFace()
{
  tft.fillScreen(TFT_YELLOW);
  drawEyes(TFT_BLACK);
  drawMouthSurprised();
}

// Enhanced eye drawing with specified color for the iris
void
Display::drawEyes(uint16_t color)
{
  // Outer eye (white part)
  tft.fillCircle(EYE_X_LEFT, EYE_Y, EYE_RADIUS, TFT_WHITE);
  tft.fillCircle(EYE_X_RIGHT, EYE_Y, EYE_RADIUS, TFT_WHITE);

  // Colored iris
  tft.fillCircle(EYE_X_LEFT, EYE_Y, EYE_RADIUS / 2, color);
  tft.fillCircle(EYE_X_RIGHT, EYE_Y, EYE_RADIUS / 2, color);

  // Black pupil
  tft.fillCircle(EYE_X_LEFT, EYE_Y, EYE_RADIUS / 4, TFT_BLACK);
  tft.fillCircle(EYE_X_RIGHT, EYE_Y, EYE_RADIUS / 4, TFT_BLACK);
}

void
Display::drawMouthHappy()
{
  // Drawing a happy mouth with "thicker" lines by manually drawing multiple
  // lines
  for (int y_offset = -2; y_offset <= 2; y_offset++) {
    tft.drawLine(TFT_WIDTH / 2 - 20,
                 TFT_HEIGHT * 2 / 3 + 10 + y_offset,
                 TFT_WIDTH / 2 + 20,
                 TFT_HEIGHT * 2 / 3 + 10 + y_offset,
                 TFT_BLACK);
  }
}

void
Display::drawMouthSad()
{
  // Drawing a sad mouth with "thicker" lines by manually drawing multiple lines
  for (int y_offset = -2; y_offset <= 2; y_offset++) {
    tft.drawLine(TFT_WIDTH / 2 - 20,
                 TFT_HEIGHT * 2 / 3 + 20 + y_offset,
                 TFT_WIDTH / 2 + 20,
                 TFT_HEIGHT * 2 / 3 + 20 + y_offset,
                 TFT_BLACK);
  }
}

void
Display::drawMouthAngry()
{
  // For an angry expression, using multiple lines to simulate thickness
  for (int x_offset = -2; x_offset <= 2; x_offset++) {
    tft.drawLine(TFT_WIDTH / 2 - 15 + x_offset,
                 TFT_HEIGHT * 2 / 3 - 5,
                 TFT_WIDTH / 2 + 15 + x_offset,
                 TFT_HEIGHT * 2 / 3 + 5,
                 TFT_BLACK);
    tft.drawLine(TFT_WIDTH / 2 - 15 + x_offset,
                 TFT_HEIGHT * 2 / 3 + 5,
                 TFT_WIDTH / 2 + 15 + x_offset,
                 TFT_HEIGHT * 2 / 3 - 5,
                 TFT_BLACK);
  }
}

void
Display::drawMouthSurprised()
{
  // Drawing a surprised mouth as a filled circle for thickness
  tft.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT * 2 / 3, 10, TFT_BLACK);
}
