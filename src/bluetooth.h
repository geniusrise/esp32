#ifndef BLUETOOTH_MANAGER_HPP
#define BLUETOOTH_MANAGER_HPP

#include <Arduino.h>
#include <esp_a2dp_api.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_gap_bt_api.h>

class BluetoothManager
{
public:
    BluetoothManager();
    void begin(const char *deviceName);
    void scanForDevices();
    void connectToDevice(const esp_bd_addr_t remote_bda);

private:
    static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);
    static void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
    static void bt_app_a2d_cb(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param);
    static void bt_app_a2d_data_cb(const uint8_t *data, uint32_t len);
    static void a2d_app_heart_beat(void *arg);
    static esp_bd_addr_t peer_bd_addr; // Device to connect to

    static void logError(esp_err_t err, const char *action);
};

#endif // BLUETOOTH_MANAGER_HPP
