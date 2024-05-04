#include "mic.h"

#define SAMPLE_RATE 16000
#define CHANNELS 1
#define BITS_PER_SAMPLE 16

// Source: mic over i2s
I2SConfig cfg;
I2SStream i2sStream;

File input_audio_file;

// Stick to wav for wider applicability
// MP3EncoderLAME mp3;
// WAVEncoder wav;
// EncodedAudioStream encoder(&i2sStream, &wav);
// StreamCopy copyToSD(input_audio_file, encoder);

WAVEncoder wav;
EncodedAudioStream encoder(&input_audio_file, &wav);
StreamCopy copyToSD(encoder, i2sStream);

MicManager::MicManager(int bckPin, int wsPin, int dataPin)
  : mic_BCK(bckPin)
  , mic_WS(wsPin)
  , mic_DATA(dataPin)
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
  input_audio_file = SD.open(fileName, FILE_WRITE);
  input_audio_file.seek(0);

  copyToSD.setCheckAvailableForWrite(false);

  AudioInfo info(SAMPLE_RATE, CHANNELS, BITS_PER_SAMPLE);
  wav.setAudioInfo(info);

  wav.begin();
  i2sStream.begin(cfg);
  copyToSD.begin();
}

void
MicManager::record()
{
  copyToSD.copy();
}

void
MicManager::stopRecording()
{
  wav.end();
  copyToSD.end();
  i2sStream.end();
  input_audio_file.close();
}
