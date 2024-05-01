// speaker.h
#ifndef SPEAKER_H
#define SPEAKER_H

#include "AudioTools.h"
#include "SD.h"

class Speaker
{
public:
  Speaker(int left_channel, int right_channel);

  void begin();

  void play(String fileName);

private:
  int left, right;
  SPIClass sd_spi;
  File file;
};

#endif // SPEAKER_H
