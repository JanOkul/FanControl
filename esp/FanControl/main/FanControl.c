#include "infrared.h"
#include "wifi.h"
#include "http.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "esp_rom_sys.h"
#include "esp_log.h"
#include "esp_pm.h"


#if __has_include("WifiConfig.h")
#include "WifiConfig.h"

#else
#error "WifiConfig.h not found. Please create this file in '/main'."
#endif

#ifndef WIFI_SSID 
#error "WIFI_SSID is not defined. Please add '#define WIFI_SSID \"your_ssid\"' to WifiConfig.h."
#endif

#ifndef WIFI_PASSWORD
#error "WIFI_PASSWORD is not defined. Please add '#define WIFI_PASSWORD \"your_password\"' to WifiConfig.h."
#endif

static const char* TAG = "MAIN";

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    // Initialise the timer, channel and pins needed to send infrared NEC messages.
    initialise_nec();

    // Initalise the WIFI driver and TCP/IP stack.
    initialise_wifi();

    // Create a mDNS server for easy LAN access to device.
    initialise_mdns();
    
    // Create HTTP server for remote control of remote.
    start_http_server();
}

