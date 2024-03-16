// speaker.h
#ifndef SPEAKER_H
#define SPEAKER_H

#include "AudioLibs/AudioSourceSD.h"
#include "AudioTools.h"

class Speaker
{
public:
  Speaker(int sdCSPin, int sdMISOPin, int sdMOSIPin, int sdCLKPin);

  void play(String fileName);

private:
  int sd_CS, sd_MISO, sd_MOSI, sd_CLK;
};

#endif // SPEAKER_H
