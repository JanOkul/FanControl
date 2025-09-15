#include <stdint.h>
#include "driver/ledc.h"

/**
 * @brief   Sets up timer and channel for NEC transmission
 */
void initialise_nec(void);


/**
 * @brief Sends an infrared message on the NEC protocol
 *        via the configured timer and channel.
 * @param address The NEC address of the receiver.
 * @param command The command to send to the receiver.
 */
void nec_send(uint8_t address, uint8_t command);
