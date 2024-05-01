#include "speaker.h"

#define SAMPLE_RATE 16000
#define CHANNELS 1
#define BITS_PER_SAMPLE 32

Speaker::Speaker(int left_channel, int right_channel)
  : left(left_channel)
  , right(right_channel)
{
}

void
Speaker::begin()
{
  pinMode(left, ANALOG);
  pinMode(right, ANALOG);
}

void
Speaker::play(String fileName)
{
  File file = SD.open(fileName, FILE_READ);

  size_t bytesRead = 1;
  uint8_t buffer[SAMPLE_RATE];

  while (bytesRead > 0) {
    bytesRead = file.read(buffer, sizeof(buffer));

    for (size_t i = 0; i < bytesRead; i++) {
      dacWrite(left, buffer[i]);
      dacWrite(right, buffer[i]);
      delayMicroseconds(1000000 / SAMPLE_RATE);
    }
  }
}
