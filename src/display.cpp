#include "display.h"

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
  tft.fillScreen(TFT_WHITE); // Start with a white screen
}

bool
Display::showEmotion(const char* emotion)
{
  char* fileName = (char*)malloc(1024 * sizeof(char));
  sprintf(fileName, "emojis/%s.svg", emotion);

  // if (SD.exists(fileName)) {
  //   File bitmapFile = SD.open(fileName, FILE_READ);
  //   if (bitmapFile) {
  //     displayBitmap(bitmapFile);
  //     bitmapFile.close();
  //     return true;
  //   } else {
  //     Serial.println("Failed to open bitmap file: " + fileName);
  //   }
  // } else {
  //   Serial.println("Bitmap file not found: " + fileName);
  // }

  return false;
}

void
Display::displayBitmap(File& bitmapFile)
{
  // Read the bitmap header
  char header[54];
  bitmapFile.read((uint8_t*)header, 54);

  // Get the image dimensions
  uint32_t width = *((uint32_t*)&header[18]);
  uint32_t height = *((uint32_t*)&header[22]);

  // Prepare the TFT display
  tft.startWrite();
  tft.setAddrWindow(0, 0, width, height);

  // Read and display the bitmap pixels
  uint8_t buffer[BITMAP_WIDTH * 2]; // Adjust buffer size based on bitmap width
  uint16_t color;
  uint32_t pixelCount = width * height;

  while (pixelCount--) {
    bitmapFile.read(&buffer[0], 2);
    color = ((buffer[1] << 8) | buffer[0]);
    tft.pushColor(color);
  }

  tft.endWrite();
}
