#ifndef MIC_HPP
#define MIC_HPP

#include "AudioTools.h"
#include "driver/i2s.h"
#include "sd.h"
#include <Arduino.h>

#include "AudioCodecs/AudioEncoded.h"
// #include "AudioCodecs/CodecMP3LAME.h"

using AudioBufferCallback = void (*)(void);

class MicManager
{
public:
  explicit MicManager(int bckPin, int wsPin, int dataPin);
  void startRecording(String fileName);
  void record();
  void stopRecording();

private:
  int mic_BCK, mic_WS, mic_DATA;
  int sd_CS, sd_MISO, sd_MOSI, sd_CLK;
};

#endif // MIC_HPP
