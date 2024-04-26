#ifndef DISPLAY_H
#define DISPLAY_H

#include "sd.h"
#include <Arduino.h>
#include <FS.h>
#include <TFT_eSPI.h> // Include the graphics library for the ST7735 driver chip

#define TFT_WIDTH 128
#define TFT_HEIGHT 128

class Display
{
public:
  Display();    // Constructor to initialize the display
  void begin(); // Initialize the TFT display

  // Function to display an emotion if it exists
  bool showEmotion(const String& emotion);

private:
  TFT_eSPI tft = TFT_eSPI(); // Create a TFT_eSPI object

  // Function to display a bitmap
  void displayBitmap(File& bitmapFile);
};

#endif // DISPLAY_H
