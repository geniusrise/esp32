#ifndef SD_H
#define SD_H

#include <SD.h>

class SDCard
{
public:
  SDCard(int sdCSPin, int sdMISOPin, int sdMOSIPin, int sdCLKPin)
  {
    sd_spi.begin(sdCLKPin, sdMISOPin, sdMOSIPin, sdCSPin);

    if (!SD.begin()) {
      Serial.println("SD card initialization failed!");
      return;
    }
  } // Constructor to initialize the SD Card

private:
  SPIClass sd_spi;
};

#endif // SD_H
