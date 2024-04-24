#include "mic.h"

#define SAMPLE_RATE 16000
#define CHANNELS 1
#define BITS_PER_SAMPLE 32

// Source: mic over i2s
I2SConfig cfg;
I2SStream i2sStream;

// Destination: SD card over i2c
SPIClass sd_spi;
File file;

EncodedAudioStream encoder(&file, new MP3EncoderLAME());
StreamCopy copier;

MicManager::MicManager(int bckPin,
                       int wsPin,
                       int dataPin,
                       int sdCSPin,
                       int sdMISOPin,
                       int sdMOSIPin,
                       int sdCLKPin)
  : mic_BCK(bckPin)
  , mic_WS(wsPin)
  , mic_DATA(dataPin)
  , sd_CS(sdCSPin)
  , sd_MISO(sdMISOPin)
  , sd_MOSI(sdMOSIPin)
  , sd_CLK(sdCLKPin)
{
  I2SStream i2sStream;
  cfg = i2sStream.defaultConfig(RX_MODE);

  cfg.i2s_format = I2S_STD_FORMAT;
  cfg.bits_per_sample = BITS_PER_SAMPLE;
  cfg.channels = CHANNELS;
  cfg.sample_rate = SAMPLE_RATE;
  cfg.is_master = true;

  cfg.pin_bck = bckPin;
  cfg.pin_ws = wsPin;
  cfg.pin_data = dataPin; // output
}

void
MicManager::startRecording(String fileName)
{

  sd_spi.begin(sd_CLK, sd_MISO, sd_MOSI, sd_CS);

  if (!SD.begin(sd_CS, sd_spi)) {
    printf("SD card failed to initialize");
    return;
  }

  file = SD.open(fileName, FILE_WRITE);
  file.seek(0);

  copier.setCheckAvailableForWrite(false);

  i2sStream.begin(cfg);
  copier.begin(encoder, i2sStream);
}

void
MicManager::record()
{
  copier.copy();
}

void
MicManager::stopRecording()
{
  copier.end();
  i2sStream.end();
  file.close();
}
