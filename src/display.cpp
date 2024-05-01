#include "display.h"

// Assuming dimensions for simplicity
#define BITMAP_WIDTH TFT_WIDTH
#define BITMAP_HEIGHT TFT_HEIGHT

Display::Display()
{
  // Constructor body if needed
}

void
Display::begin()
{
  tft.init();
  // tft.setRotation(2);
  tft.fillScreen(TFT_OLIVE); // Start with a white screen
}

bool
Display::showEmotion(const char* emotion)
{
  char fileName[100];
  snprintf(fileName, sizeof(fileName), "/emojis/%s.bmp", emotion);

  if (SD.exists(fileName)) {
    File bitmapFile = SD.open(fileName, FILE_READ);
    if (bitmapFile) {
      displayBitmap(bitmapFile);
      bitmapFile.close();
      return true;
    } else {
      printf("Failed to open bitmap file: %s\n", fileName);
    }
  } else {
    printf("Bitmap file not found: %s\n", fileName);
  }

  // if (fileName != nullptr) {
  //   free(fileName);
  // }

  return false;
}

void
Display::displayBitmap(File& bitmapFile)
{
  // Read the bitmap file header (first 14 bytes)
  char fileHeader[14];
  bitmapFile.read((uint8_t*)fileHeader, 14);

  // Get the offset to the pixel data
  uint32_t dataOffset = *((uint32_t*)&fileHeader[10]);

  // Read the bitmap info header
  char infoHeader[40];
  bitmapFile.read((uint8_t*)infoHeader, 40);

  // Get the image dimensions
  uint32_t width = *((uint32_t*)&infoHeader[4]);
  uint32_t height = *((uint32_t*)&infoHeader[8]);

  // Get the bits per pixel
  uint16_t bitsPerPixel = *((uint16_t*)&infoHeader[14]);

  // Calculate the total header size
  uint32_t headerSize = dataOffset;

  // Skip any remaining header bytes
  bitmapFile.seek(headerSize);

  // Prepare the TFT display
  tft.startWrite();
  tft.setAddrWindow(0, 0, width - 1, height - 1);

  // Read and display the bitmap pixels
  uint32_t buffer[TFT_WIDTH];
  uint32_t pixelCount = width * height;

  if (bitsPerPixel == 32) {
    // 32-bit RGBA bitmap
    while (pixelCount--) {
      bitmapFile.read((uint8_t*)&buffer[0], 4);
      uint32_t color = buffer[0];
      uint16_t convertedColor = ((color & 0xF80000) >> 8) |
                                ((color & 0x00FC00) >> 5) |
                                ((color & 0x0000F8) >> 3);
      tft.pushColor(convertedColor);
    }
  } else {
    // 16-bit RGB565 bitmap
    while (pixelCount--) {
      bitmapFile.read((uint8_t*)&buffer[0], 2);
      tft.pushColor(buffer[0]);
    }
  }

  tft.endWrite();
}