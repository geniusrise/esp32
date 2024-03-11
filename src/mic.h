#ifndef MIC_MANAGER_HPP
#define MIC_MANAGER_HPP

#include "AudioTools.h"
#include "SD.h"
#include "driver/i2s.h"
#include <Arduino.h>

using AudioBufferCallback = void (*)(void);

class MicManager
{
public:
  MicManager(int bckPin, int wsPin, int dataPin);
  void startRecording(String fileName);
  void stopRecording();

  static void audioTaskWrapper(void* param);

private:
  int _bckPin, _wsPin, _dataPin;
  bool _isRecording;

  I2SStream i2sStream;
  I2SConfig cfg;
  StreamCopy copier;
  File file;

  // void initializeI2S();
  // void deinitializeI2S();
  void audioTask();
};

#endif // MIC_MANAGER_HPP
