#include "infrared.h"

#include "esp_rom_sys.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_mac.h"

// Timer and Channel configuration
const ledc_channel_t CHANNEL = LEDC_CHANNEL_0;          // ESP channel for duty cycle.
const ledc_timer_t TIMER = LEDC_TIMER_0;                // ESP timer for duty cycle.
const int FREQUENCY = 38000;                            // Carrier frequency for NEC
const int LED_PIN = 0;                                  // Output pin for IR LED
const ledc_timer_bit_t DUTY_RESOLUTION = 10;            // PWM Resolution

const uint32_t pin_high = (1 << DUTY_RESOLUTION)/3;     // Duty cycle when pin is high (1)
const uint32_t pin_low = 0;                             // Duty cycle when pin is low (0)

// NEC protocol time gaps between sections of NEC message.
const uint32_t leading_burst_delay = 9000;
const uint32_t header_space_delay = 4500;
const uint32_t common_logical_delay = 562;  // A common delay between both a logical 1 and 0.
const uint32_t logical_zero_delay = 562;
const uint32_t logical_one_delay  = 1687;
const uint32_t final_burst_delay  = 562;

static const char* TAG = "PWM";

/**
 * @brief Changes the active duty cycle of an led channel to high or low.
 * 
 * @param new_duty Either 0 or a duty cycle that represents high
 */
static void change_duty(uint32_t new_duty) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, CHANNEL, new_duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, CHANNEL); 
}


/**
 * @brief  Sends an 8 bit message over the NEC protocol, typically for
 *         sending an address or command 
 *                                                         
 * @param code The 8 bit code that will be transmitted (address, ~address, command, ~command).
 */
static void send_8_bit_code(uint8_t code) {
    // Loops through each bit of the code and sends it
    for (uint8_t unused = 0; unused < 8; unused++) {

        change_duty(pin_high);
        esp_rom_delay_us(common_logical_delay);
        change_duty(pin_low);

        // Send '1'
        if (code & 0x1) {
            esp_rom_delay_us(logical_one_delay);
        } 
        // Send '0'
        else {    
            esp_rom_delay_us(logical_zero_delay);
        }
        code = code >> 1;
    }
}

/**
 * @brief Initialises a timer, and a timer channel on the ESP32-C6's. 
 */
void initialise_nec(void) {
    esp_err_t ret;
    // Setup timer struct and call init function
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = DUTY_RESOLUTION,
        .timer_num = TIMER,
        .freq_hz = FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ret = ledc_timer_config(&timer_config);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "Timer initalised");

    // Setup channel struct and call init function
    ledc_channel_config_t channel_config = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = TIMER,
        .duty = 0,      // Starts with a duty cycle of 0 (off)                          
        .hpoint = 0     // Goes high at count of 0 (on reset of channel)
    };
    ret = ledc_channel_config(&channel_config);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "Timer Channel initalised");
}

/**
 * @brief Sends an NEC message to an infrared reciver device. There is no repeat functionality implemented.
 * 
 * @param address The NEC address of the target device as an 8 bit code.
 * @param command The command you want to send to the target as an 8 bit code.
 */
void nec_send(uint8_t address, uint8_t command) {

    
    // Header of NEC protocol is to send a high burst for 9ms with a 38kHz PWM signal, and send nothing for 4.5ms.
    change_duty(pin_high);
    esp_rom_delay_us(leading_burst_delay);
    change_duty(pin_low);
    esp_rom_delay_us(header_space_delay);

    // Send address code
    send_8_bit_code(address);  
    // Send address inverse
    send_8_bit_code(~address);
    // Send comand code
    send_8_bit_code(command);
    // Send comand inverse
    send_8_bit_code(~command);

    // Send end of message pulse
    change_duty(pin_high);
    esp_rom_delay_us(final_burst_delay);
    change_duty(pin_low);
}

