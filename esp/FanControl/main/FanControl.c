// #include "infrared/ir_config.h"
#include "infrared/ir_nec.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

const int FREQ      = 38000;    // Carrier frequency for NEC
const int LED_PIN   = 10;       // Output pin for IR LED
const int RES       = 10;       // PWM Resolution

void app_main(void)
{
    nec_config_t nec_config = {
        .pin = LED_PIN,
        .carrier_freq = FREQ,
        .channel_numer = 0,
        .timer_number = 0,
        .duty_resolution = RES
    };
    nec_init(&nec_config);

    while (1) {
        nec_send(&nec_config, 0x20, 0xC);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

