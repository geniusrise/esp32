#include "speaker.h"

#define SAMPLE_RATE 16000
#define CHANNELS 1
#define BITS_PER_SAMPLE 32

// Source: SD card over i2c
SPIClass sd_spi;
File file;

// Destination: PCM out via one wire to AMP -> speakers
#define PIN_AMP_LEFT_CHANNEL 10
#define PIN_AMP_RIGHT_CHANNEL 11

Speaker::Speaker(int sdCSPin, int sdMISOPin, int sdMOSIPin, int sdCLKPin)
  : sd_CS(sdCSPin)
  , sd_MISO(sdMISOPin)
  , sd_MOSI(sdMOSIPin)
  , sd_CLK(sdCLKPin)
{
  sd_spi.begin(sdCLKPin, sdMISOPin, sdMOSIPin, sdCSPin);
}

void
Speaker::play(String fileName)
{
  if (!SD.begin(sd_CS, sd_spi)) {
    printf("SD card failed to initialize");
    return;
  }

  file = SD.open(fileName, FILE_READ);

  size_t bytesRead = 1;
  uint8_t buffer[SAMPLE_RATE];

  while (bytesRead > 0) {
    bytesRead = file.read(buffer, sizeof(buffer));

    for (size_t i = 0; i < bytesRead; i++) {
      dacWrite(PIN_AMP_LEFT_CHANNEL, buffer[i]);
      dacWrite(PIN_AMP_RIGHT_CHANNEL, buffer[i]);
      delayMicroseconds(1000000 / SAMPLE_RATE);
    }
  }
}
