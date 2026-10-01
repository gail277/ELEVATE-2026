
#pragma once

#include <stdint.h>

#include "radio_config.h"

/**
 * @brief Stores a radio packet's data and length.
 */
typedef struct
{
    uint8_t data[PAYLOAD_MAX_LENGTH];
    uint8_t length;
} packet_t;

/**
 * @brief Creates a packet from the provided data.
 *
 * Copies the specified data into the packet and sets its length.
 *
 * @param packet Pointer to the packet to initialise.
 * @param data Pointer to the data to store in the packet.
 * @param length Number of bytes to copy into the packet.
 */
void packet_create(packet_t *packet, uint8_t *data, uint8_t length);
