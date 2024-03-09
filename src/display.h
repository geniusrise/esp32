#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h> // Include the graphics library for the ST7735 driver chip

class Display
{
public:
  Display();    // Constructor to initialize the display
  void begin(); // Initialize the TFT display

  // Functions for different facial expressions
  void showHappyFace();
  void showSadFace();
  void showAngryFace();
  void showSurprisedFace();

private:
  TFT_eSPI tft = TFT_eSPI(); // Create a TFT_eSPI object

  // Private methods to draw facial components
  void drawEyes(uint16_t color);
  void drawMouthHappy();
  void drawMouthSad();
  void drawMouthAngry();
  void drawMouthSurprised();
};

#endif // DISPLAY_H
