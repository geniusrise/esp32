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

  tft.fillScreen(TFT_BLACK);

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

  printf("Rendering image dim: %dx%d %d bit\n", width, height, bitsPerPixel);

  // Move the file pointer to the start of the bitmap data
  bitmapFile.seek(dataOffset);

  // Calculate the row size and padding
  uint32_t rowSize = ((width * bitsPerPixel + 31) / 32) * 4;
  uint32_t padding = rowSize - width * bitsPerPixel / 8;

  // Allocate memory for one row of bitmap data
  uint8_t* rowData = new uint8_t[rowSize];

  // Display the bitmap on the TFT
  for (int32_t y = height - 1; y >= 0; y--) {
    bitmapFile.read(rowData, rowSize);

    if (bitsPerPixel == 32) {
      // Handle 32-bit RGBA bitmap
      for (uint32_t x = 0; x < width; x++) {
        uint8_t b = rowData[x * 4];
        uint8_t g = rowData[x * 4 + 1];
        uint8_t r = rowData[x * 4 + 2];
        uint8_t a = rowData[x * 4 + 3]; // Alpha channel (unused in this case)
        uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        tft.drawPixel(x, y, color);
      }
    } else if (bitsPerPixel == 24) {
      // Handle 24-bit RGB bitmap
      for (uint32_t x = 0; x < width; x++) {
        uint8_t b = rowData[x * 3];
        uint8_t g = rowData[x * 3 + 1];
        uint8_t r = rowData[x * 3 + 2];
        uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        tft.drawPixel(x, y, color);
      }
    } else {
      printf("Unsupported bits per pixel: %d\n", bitsPerPixel);
      break;
    }
  }

  // Free the memory allocated for row data
  delete[] rowData;
}