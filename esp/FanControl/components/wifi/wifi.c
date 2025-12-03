#include "wifi.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "../../main/WifiConfig.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "mdns.h"
static const char* TAG = "WIFI";

void initialise_wifi(void)
{
    // Initialise TCP/IP stack.
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // Init Wi-Fi with default config.
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Configure WIFI credentials.
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // Start and connect to WIFI.
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));
    ESP_LOGI(TAG, "Modem power saving on.");

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_ERROR_CHECK(esp_wifi_connect());
    ESP_LOGI(TAG, "Connecting to WIFI");
}

void initialise_mdns(void)
{
    mdns_init();
    mdns_hostname_set("fancontroller");
    mdns_instance_name_set("Fan Controller");
    mdns_service_set_ttl("_fan", "_tcp", 60);
    if (mdns_service_add(NULL, "_fan", "_tcp", 80, NULL, 0) == ESP_OK) {
        ESP_LOGI(TAG, "mDNS IP discovery service added");
    } else {
        ESP_LOGI(TAG, "mDNS service add failed.");
    }
    ESP_LOGI(TAG, "mDNS started with hostname: fancontroller.local");
}

