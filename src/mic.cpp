#include "mic.h"

MicManager::MicManager()
    : i2sStream(I2S_NUM_0, I2S_PIN_NO_CHANGE, 10, 11, 12),
      memoryStream(bufferSize), wavEncoder(memoryStream),
      gainControl(wavEncoder) {}

void MicManager::begin() {
  auto cfg = i2sStream.defaultConfig(RX_MODE);
  cfg.i2s_format = I2S_STD_FORMAT;
  cfg.bits_per_sample = 16;
  cfg.sample_rate = 44100;
  cfg.channels = 1;
  cfg.pin_ws = 10;
  cfg.pin_bck = 11;
  cfg.pin_data = 12;
  i2sStream.begin(cfg);

  wavEncoder.begin();
  wavEncoder.setAudioInfo(cfg);

  gainControl.begin(gainControl.maxGainDB);
}

void MicManager::startRecording() {
  memoryStream.begin();
  i2sStream.begin();
}

void MicManager::stopRecording() {
  i2sStream.end();
  memoryStream.end();
}

char *MicManager::getRecordedAudio() {
  return (char *)memoryStream.getBuffer();
}

size_t MicManager::getRecordedAudioSize() {
  return memoryStream.availableForWrite();
}
