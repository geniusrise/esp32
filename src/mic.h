#ifndef MIC_MANAGER_HPP
#define MIC_MANAGER_HPP

#include "driver/i2s.h"
#include <Arduino.h>

class MicManager
{
public:
  MicManager(int bckPin, int wsPin, int dataPin);
  void begin();
  void startRecording();
  void stopRecording();
  char* getRecordedAudio();
  size_t getRecordedAudioSize();

  static void audioTaskWrapper(void* param);

private:
  int _bckPin, _wsPin, _dataPin;
  i2s_config_t _i2sConfig;
  i2s_pin_config_t _pinConfig;
  uint8_t* _audioBuffer;
  size_t _bufferSize;
  size_t _recordedSize;
  bool _isRecording;

  void initializeI2S();
  void deinitializeI2S();
  void audioTask();
};

#endif // MIC_MANAGER_HPP
