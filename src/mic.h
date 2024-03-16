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
  explicit MicManager(int bckPin, int wsPin, int dataPin);
  void startRecording(String fileName);
  void stopRecording();
  void record();

private:
  int _bckPin, _wsPin, _dataPin;
};

#endif // MIC_MANAGER_HPP
