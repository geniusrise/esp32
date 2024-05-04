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

    // need to set MISO to pullup, read
    // https://github.com/espressif/arduino-esp32/issues/524
    pinMode(sd_MISO, INPUT_PULLUP);
    digitalWrite(sd_CS, HIGH);

    sd_spi.begin(sd_CLK, sd_MISO, sd_MOSI, sd_CS);
    sd_spi.setDataMode(SPI_MODE0);

    if (!SD.begin(sd_CS, sd_spi)) {
      Serial.println("\nFailed to mount SD Card.\n\n");
      return;
    }
    Serial.println("SD Card mounted, continuing.\n");
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
