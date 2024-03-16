// speaker.h
#ifndef SPEAKER_H
#define SPEAKER_H

#include "AudioTools.h"
#include "SD.h"

class Speaker
{
public:
  Speaker(int sdCSPin, int sdMISOPin, int sdMOSIPin, int sdCLKPin);

  void play(String fileName);

private:
  int sd_CS, sd_MISO, sd_MOSI, sd_CLK;
  SPIClass sd_spi;
  File file;
};

#endif // SPEAKER_H
