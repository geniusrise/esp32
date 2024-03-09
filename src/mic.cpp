#include "mic.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SAMPLE_RATE (16000)
#define CHANNELS (1)
#define BITS_PER_SAMPLE (32)

#define MAX_SECONDS_TO_BUFFER 5
#define AUDIO_BUFFER_SIZE                                                      \
  (MAX_SECONDS_TO_BUFFER * SAMPLE_RATE * CHANNELS * BITS_PER_SAMPLE / 8)

MicManager::MicManager(int bckPin, int wsPin, int dataPin)
  : _bckPin(bckPin)
  , _wsPin(wsPin)
  , _dataPin(dataPin)
  , _audioBuffer(nullptr)
  , _bufferSize(0)
  , _recordedSize(0)
  , _isRecording(false)
{
  _i2sConfig = { .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
                 .sample_rate = SAMPLE_RATE,
                 .bits_per_sample = i2s_bits_per_sample_t(BITS_PER_SAMPLE),
                 .channel_format = CHANNELS == 1 ? I2S_CHANNEL_FMT_ONLY_RIGHT
                                                 : I2S_CHANNEL_FMT_RIGHT_LEFT,
                 .communication_format = I2S_COMM_FORMAT_I2S_MSB,
                 .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
                 .dma_buf_count = 8,
                 .dma_buf_len = 64,
                 .use_apll = false,
                 .tx_desc_auto_clear = false,
                 .fixed_mclk = 0 };

  _pinConfig = { .bck_io_num = _bckPin,
                 .ws_io_num = _wsPin,
                 .data_out_num = I2S_PIN_NO_CHANGE,
                 .data_in_num = _dataPin };
}

void
MicManager::begin()
{
  i2s_driver_install(I2S_NUM_0, &_i2sConfig, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &_pinConfig);
  i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BITS_PER_SAMPLE, I2S_CHANNEL_MONO);
}

void
MicManager::deinitializeI2S()
{
  i2s_driver_uninstall(I2S_NUM_0);
}

void
MicManager::setCallback(AudioBufferCallback callback)
{
  _callback = callback;
}

void
MicManager::startRecording()
{
  if (_audioBuffer == nullptr) {
    _audioBuffer = static_cast<uint8_t*>(malloc(AUDIO_BUFFER_SIZE));
  }
  _recordedSize = 0;
  _isRecording = true;

  // Start background task for audio collection
  xTaskCreate(audioTaskWrapper, "AudioCollectTask", 2048, this, 1, NULL);
}

void
MicManager::stopRecording()
{
  _isRecording = false;
  // Give some time for the audioTask to exit cleanly before freeing resources
  vTaskDelay(pdMS_TO_TICKS(100));
  if (_audioBuffer != nullptr) {
    free(_audioBuffer);
    _audioBuffer = nullptr;
  }
}

char*
MicManager::getRecordedAudio()
{
  return reinterpret_cast<char*>(_audioBuffer);
}

size_t
MicManager::getRecordedAudioSize()
{
  return _recordedSize;
}

void
MicManager::audioTaskWrapper(void* param)
{
  static_cast<MicManager*>(param)->audioTask();
  vTaskDelete(NULL); // Delete this task when done
}

void
MicManager::audioTask()
{
  size_t bytesRead;
  while (_isRecording) {
    // Determine how much space is left in the buffer
    size_t remainingSpace = AUDIO_BUFFER_SIZE - _recordedSize;

    uint8_t tempBuffer[512];

    // Read only as much as can fit in the remaining space of the buffer
    size_t toRead = sizeof(tempBuffer) <= remainingSpace ? sizeof(tempBuffer)
                                                         : remainingSpace;
    i2s_read(I2S_NUM_0, tempBuffer, toRead, &bytesRead, portMAX_DELAY);

    if (bytesRead > 0) {
      // Ensure we don't read beyond the buffer size
      if (_recordedSize + bytesRead > AUDIO_BUFFER_SIZE) {
        bytesRead = AUDIO_BUFFER_SIZE - _recordedSize;
      }

      // Copy the data into the main audio buffer
      memcpy(_audioBuffer + _recordedSize, tempBuffer, bytesRead);
      _recordedSize += bytesRead;

      // Check if the buffer is half full or more
      if (_recordedSize >= AUDIO_BUFFER_SIZE / 2) {
        // Trigger the callback if set
        if (_callback) {
          _callback();
        }

        // Reset the buffer and its size after the callback has been triggered
        memset(_audioBuffer, 0, AUDIO_BUFFER_SIZE);
        _recordedSize = 0;
      }
    }
  }
}
