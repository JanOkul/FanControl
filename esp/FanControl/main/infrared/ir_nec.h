#ifndef IR_NEC.H
#define IR_NEC.H

#include <stdint.h>
#include "driver/ledc.h"


/**
 * @brief Configuration struct for NEC communication,
 *        there should only be one instance of these. 
 *        configures the output pin, timer and channel
 *        for specifically an esp32-c6, which can only
 *        operate on low speed mode.
 */
typedef struct {
    int pin;
    uint32_t carrier_freq;
    uint32_t duty_resolution;
    ledc_timer_t timer_number;
    ledc_channel_t channel_numer;
} nec_config_t;

/**
 * @brief   This function will set up a timer and channel
 *          on the esp32-c6 for NEC PWM transmission.
 *           
 * 
 * @param config  Pointer to configuration struct for setting up nec communication.
 *                
 */

void nec_init(nec_config_t *config);


/**
 * @brief This function will send an infrared message on the NEC protocol
 *        via the configured timer and channel.
 *   
 * @param channel The same config struct used for setting up the channel in nec_init.
 * @param address The NEC address of the receiver.
 * @param command The command to send to the receiver.
 */
void nec_send(nec_config_t *channel, uint8_t address, uint8_t command);


#endif