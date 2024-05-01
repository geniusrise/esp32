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
    sd_MISO = sdMISOPin;
    sd_MOSI = sdMOSIPin;
    sd_CLK = sdCLKPin;
  } // Constructor to initialize the SD Card

  void begin()
  {
    sd_spi.begin(sd_CLK, sd_MISO, sd_MOSI);
    delay(100);

    if (!SD.begin(sd_CS, sd_spi)) {
      Serial.println("\n\nSD card initialization failed!\n\n");
      return;
    }
    Serial.println("SD Card mounted, directories:\n");

    File myFile = SD.open("/lol.txt", FILE_WRITE);
    myFile.println("testing 1, 2, 3.");
    myFile.close();
  }

  void printDirectory(File dir, int numTabs)
  {
    while (true) {

      File entry = dir.openNextFile();
      if (!entry) {
        // no more files
        break;
      }
      for (uint8_t i = 0; i < numTabs; i++) {
        Serial.print('\t');
      }
      Serial.print(entry.name());
      if (entry.isDirectory()) {
        Serial.println("/");
        printDirectory(entry, numTabs + 1);
      } else {
        // files have sizes, directories do not
        Serial.print("\t\t");
        Serial.println(entry.size(), DEC);
      }
      entry.close();
    }
  }

private:
  int sd_CS, sd_MISO, sd_MOSI, sd_CLK;
};

#endif // SD_H
