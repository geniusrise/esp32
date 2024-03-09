#include "mic.h"

MicManager::MicManager(int i2s_sck_pin, int i2s_ws_pin, int i2s_sd_pin)
    : i2sSCKPin(i2s_sck_pin), i2sWSPin(i2s_ws_pin), i2sSDPin(i2s_sd_pin) {}

void MicManager::begin() { setupI2S(); }

void MicManager::setupI2S() {
  const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 16000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format =
          I2S_CHANNEL_FMT_ONLY_LEFT, // Assuming mono microphone input
      .communication_format = I2S_COMM_FORMAT_I2S_MSB,
      .intr_alloc_flags = 0, // Default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false,
      .tx_desc_auto_clear = false, // Not applicable to RX
      .fixed_mclk = 0};

  const i2s_pin_config_t pin_config = {.bck_io_num = i2sSCKPin,
                                       .ws_io_num = i2sWSPin,
                                       .data_out_num =
                                           I2S_PIN_NO_CHANGE, // Not used in RX
                                       .data_in_num = i2sSDPin};

  // Install and start i2s driver
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void MicManager::readAudioData(int16_t *buffer, size_t bufferSize,
                               size_t &bytesRead) {
  // Read data from I2S
  i2s_read(I2S_NUM_0, (void *)buffer, bufferSize, &bytesRead, portMAX_DELAY);
}

void MicManager::end() {
  i2s_driver_uninstall(I2S_NUM_0); // Stop and uninstall i2s driver
}
