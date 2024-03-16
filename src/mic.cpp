#include "mic.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SAMPLE_RATE (16000)
#define CHANNELS (1)
#define BITS_PER_SAMPLE (32)

#define PIN_AUDIO_KIT_SD_CARD_CS 48
#define PIN_AUDIO_KIT_SD_CARD_MOSI 37
#define PIN_AUDIO_KIT_SD_CARD_CLK 36
#define PIN_AUDIO_KIT_SD_CARD_MISO 35

I2SStream i2sStream;
StreamCopy copier;
SPIClass sd_spi;
File file;
I2SConfig cfg;

MicManager::MicManager(int bckPin, int wsPin, int dataPin)
  : _bckPin(bckPin)
  , _wsPin(wsPin)
  , _dataPin(dataPin)
  , _isRecording(false)
{
  I2SStream i2sStream;
  cfg = i2sStream.defaultConfig(RX_MODE);

  cfg.i2s_format = I2S_STD_FORMAT; // optional because default setting
  cfg.bits_per_sample = BITS_PER_SAMPLE;
  cfg.channels = CHANNELS;
  cfg.sample_rate = SAMPLE_RATE;
  cfg.is_master = true;

  cfg.pin_bck = _bckPin;
  cfg.pin_ws = _wsPin;
  cfg.pin_data = _dataPin; // output
}

void
MicManager::startRecording(String fileName)
{

  sd_spi.begin(PIN_AUDIO_KIT_SD_CARD_CLK,
               PIN_AUDIO_KIT_SD_CARD_MISO,
               PIN_AUDIO_KIT_SD_CARD_MOSI,
               PIN_AUDIO_KIT_SD_CARD_CS);

  if (!SD.begin(PIN_AUDIO_KIT_SD_CARD_CS, sd_spi)) {
    printf("SD card failed to initialize");
    return;
  }

  file = SD.open(fileName, FILE_WRITE);
  file.seek(0);
  copier.setCheckAvailableForWrite(false);
  // i2sStream.begin(cfg);
  // copier.begin(file, i2sStream);

  _isRecording = true;
  // xTaskCreate(audioTaskWrapper, "AudioCollectTask", 2048, this, 1, NULL);
}

void
MicManager::stopRecording()
{
  _isRecording = false;
  // vTaskDelay(pdMS_TO_TICKS(100));

  // copier.end();
  // i2sStream.end();
  file.close();
}

void
MicManager::audioTaskWrapper(void* param)
{
  // static_cast<MicManager*>(param)->audioTask();
  // vTaskDelete(NULL); // Delete this task when done
}

// void
// MicManager::audioTask()
// {
//   while (_isRecording) {
//     copier.copy();
//   }
// }
