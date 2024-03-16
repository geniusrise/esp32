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
  explicit MicManager(int bckPin,
                      int wsPin,
                      int dataPin,
                      int sdCSPin,
                      int sdMISOPin,
                      int sdMOSIPin,
                      int sdCLKPin);
  void startRecording(String fileName);
  void record();
  void stopRecording();

private:
  int mic_BCK, mic_WS, mic_DATA;
  int sd_CS, sd_MISO, sd_MOSI, sd_CLK;
};

#endif // MIC_MANAGER_HPP
