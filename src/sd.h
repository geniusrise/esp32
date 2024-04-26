#ifndef SD_H
#define SD_H

#include <SD.h>

SPIClass sd_spi;

class SDCard
{
public:
  SDCard(int sdCSPin, int sdMISOPin, int sdMOSIPin, int sdCLKPin)
  {
    sd_CS = sdCSPin;
    sd_MISO = sdCSPin;
    sd_MOSI = sdMOSIPin;
    sd_CLK = sdCLKPin;
  } // Constructor to initialize the SD Card

  void begin()
  {
    sd_spi.begin(sd_CLK, sd_MISO, sd_MOSI, sd_CS);

    if (!SD.begin(sd_CS, sd_spi)) {
      Serial.println("\n\nSD card initialization failed!\n\n");
      return;
    }
  }

private:
  int sd_CS, sd_MISO, sd_MOSI, sd_CLK;
};

#endif // SD_H
