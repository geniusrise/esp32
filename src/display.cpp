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
  tft.setRotation(2);
  tft.fillScreen(TFT_OLIVE); // Start with a white screen
}

bool
Display::showEmotion(const char* emotion)
{
  char fileName[100];
  snprintf(fileName, sizeof(fileName), "/emojis/%s.bmp", emotion);

  File myFile = SD.open("/lol.txt", FILE_WRITE);
  myFile.println("testing 1, 2, 3.");
  myFile.close();

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
  // Read the bitmap header
  char header[54];
  bitmapFile.read((uint8_t*)header, 54);

  // Get the image dimensions
  uint32_t width = *((uint32_t*)&header[18]);
  uint32_t height = *((uint32_t*)&header[22]);

  // Prepare the TFT display
  tft.startWrite();
  tft.setAddrWindow(0, 0, width - 1, height - 1);

  // Read and display the bitmap pixels
  uint8_t buffer[TFT_WIDTH]; // Adjust buffer size based on bitmap width
  uint16_t color;
  uint32_t pixelCount = width * height;

  while (pixelCount--) {
    bitmapFile.read(&buffer[0], 2);
    color = ((buffer[1] << 8) | buffer[0]);
    tft.pushColor(color);
  }

  tft.endWrite();
}
