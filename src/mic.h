#ifndef MIC_MANAGER_HPP
#define MIC_MANAGER_HPP

#include "AudioTools.h"
#include <Arduino.h>

class MicManager {
public:
  MicManager();
  void begin();
  void startRecording();
  void stopRecording();
  char *getRecordedAudio();
  size_t getRecordedAudioSize();

private:
  const int bufferSize = 1024;
  AudioTools::I2SStream i2sStream;
  AudioTools::MemoryStream memoryStream;
  AudioTools::WAVEncoder wavEncoder;
  AudioTools::GainControl gainControl;
};

#endif // MIC_MANAGER_HPP
