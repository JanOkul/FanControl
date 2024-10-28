#include "ir_nec.h"

#include "esp_rom_sys.h"
#include "driver/ledc.h"


const uint32_t pulse_delay = 562;
const uint32_t low_delay   = 562;
const uint32_t high_delay  = 1688;

void change_duty(uint32_t channel, uint32_t new_duty) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, new_duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}

// Sends a single NEC bit
void send_bit(uint32_t channel, uint32_t duty, uint32_t delay) {
    change_duty(channel, duty);
    esp_rom_delay_us(pulse_delay);
    change_duty(channel, 0);
    esp_rom_delay_us(delay);

}

// Sends an 8 bit code over NEC, inverted codes can be sent with ~code
void send_8_bit_code(uint32_t channel, uint32_t pin_high, uint8_t code) {
    for (uint8_t shift = 0; shift < 8; shift++) {
        if (code & 0x1) {
            send_bit(channel, pin_high, high_delay);
        } else {
            send_bit(channel, pin_high, low_delay);
        }
        code = code >> 1;
    }
}

void nec_init(nec_config_t *config) {
    // Setup timer struct and call init function
    ledc_timer_config_t timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = config->duty_resolution,
    .timer_num = config->timer_number,
    .freq_hz = config->carrier_freq,
    .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    // Setup channel struct and call init function
    ledc_channel_config_t channel_config = {
    .gpio_num = config->pin,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = config->channel_numer,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = config->timer_number,
    .duty = 0,      // Starts with a duty cycle of 0 (off)                          
    .hpoint = 0     // Goes high at count of 0 (on reset of channel)
    };
    ledc_channel_config(&channel_config);
}

// Highly abstracted send command for NEC
void nec_send(nec_config_t *config, uint8_t address, uint8_t command) {
    const uint32_t pin_high = (1 << config->duty_resolution)/3;
    uint32_t channel = config->channel_numer; 
    // Send header
    change_duty(channel, pin_high);

    // Wait for 9ms to NEC standard
    esp_rom_delay_us(9000);

    change_duty(channel, 0);

    esp_rom_delay_us(4500);

    // Send address code
    send_8_bit_code(channel, pin_high, address);
    
    // Send address inverse
    send_8_bit_code(channel, pin_high, ~address);

    // Send comand code
    send_8_bit_code(channel, pin_high, command);

    // Send comand inverse
    send_8_bit_code(channel, pin_high, ~command);

    // Send end of message pulese
    send_bit(channel, pin_high, low_delay);
}

