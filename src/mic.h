#ifndef MIC_MANAGER_H
#define MIC_MANAGER_H

#include <driver/i2s.h>

class MicManager {
public:
  MicManager(int i2s_sck_pin, int i2s_ws_pin, int i2s_sd_pin);
  void begin();
  void readAudioData(int16_t *buffer, size_t bufferSize, size_t &bytesRead);
  void end();

private:
  int i2sSCKPin;
  int i2sWSPin;
  int i2sSDPin;
  void setupI2S();
};

#endif // MIC_MANAGER_H
