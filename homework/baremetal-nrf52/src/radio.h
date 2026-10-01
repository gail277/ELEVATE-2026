#pragma once

#include <stdint.h>

#include "packet.h"

/**
 * @brief Initialises the radio peripheral.
 *
 * Configures the radio peripheral, packet format, addresses,
 * frequency channel, and CRC settings.
 */
void radio_init(void);

/**
 * @brief Enables the radio receiver.
 *
 * Disables the radio before enabling the receiver, waits for the
 * receiver to become ready, and starts listening for packets.
 */
void radio_RX_ramp_up(void);

/**
 * @brief Checks for a received radio packet.
 *
 * Checks whether a packet has been completely received and verifies
 * its CRC status.
 *
 * @return Pointer to the receive buffer if a valid packet was received,
 *         otherwise NULL.
 */
uint8_t *radio_RX(void);

/**
 * @brief Enables the radio transmitter.
 *
 * Disables the radio before enabling the transmitter and waits for
 * the transmitter to become ready.
 */
void radio_TX_ramp_up(void);

/**
 * @brief Transmits the packet stored in the transmit buffer.
 *
 * Starts radio transmission and waits for the transmission to finish.
 */
void radio_TX(void);

/**
 * @brief Sends a message over the radio.
 *
 * Splits the message into packets if it exceeds the maximum payload
 * length and transmits each packet.
 *
 * @param message Pointer to the message data to transmit.
 * @param length Length of the message in bytes.
 */
void radio_send_message(uint8_t *message, uint8_t length);

/**
 * @brief Sends a single packet over the radio.
 *
 * Copies the packet data into the transmit buffer and transmits it.
 *
 * @param packet Pointer to the packet to transmit.
 */
void radio_send_packet(packet_t *packet);

/**
 * @brief Decodes a received message.
 *
 * Extracts the message data from the receive buffer and terminates
 * the message with a null character.
 *
 * @param message Buffer where the decoded message will be stored.
 */
void decode_message(uint8_t *message);

/**
 * @brief Checks whether a radio packet has been received.
 *
 * @return Non-zero if a radio packet is available, otherwise 0.
 */
uint8_t radio_data_available(void);