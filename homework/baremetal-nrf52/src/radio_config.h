#pragma once

#include <stdint.h>

/**
 * @brief Radio configuration settings.
 */

/** @brief Radio frequency channel used for communication. */
#define RADIO_CONFIG_CHANNEL 5

/** @brief Radio mode. 0 selects the 1 Mbit/s Nordic proprietary mode. */
#define RADIO_CONFIG_MODE 0


/**
 * @brief Radio address configuration.
 */

/** @brief Number of address bytes used as the base address. */
#define RADIO_CONFIG_BALEN_ADDRESS_WIDTH 4

/** @brief Base address 0 used by logical radio addresses. */
#define RADIO_CONFIG_BASE0_ADDRESS 0x00FF

/** @brief Base address 1 used by logical radio addresses. */
#define RADIO_CONFIG_BASE1_ADDRESS 0xFF00

/** @brief Prefix bytes for logical radio addresses 0-3. */
#define RADIO_CONFIG_PREFIX0_ADDRESS 0x33221100

/** @brief Prefix bytes for logical radio addresses 4-7. */
#define RADIO_CONFIG_PREFIX1_ADDRESS 0x77665544

/** @brief Logical radio addresses enabled for reception. */
#define RADIO_CONFIG_RX_ADDRESS 0xFF

/** @brief Logical radio address used for transmission. */
#define RADIO_CONFIG_TX_ADDRESS 0x2


/**
 * @brief Radio packet configuration.
 */

/** @brief Maximum payload length in bytes. */
#define PAYLOAD_MAX_LENGTH 80

/** @brief Number of bytes in the S0 field of a packet. */
#define RADIO_CONFIG_S0_LENGTH 0

/** @brief Number of bits in the S1 field of a packet. */
#define RADIO_CONFIG_S1_LENGTH 0

/** @brief Static packet length in bytes. */
#define RADIO_CONFIG_STATIC_LENGTH 0

/** @brief Maximum packet length in bytes. */
#define RADIO_CONFIG_MAX_LENGTH PAYLOAD_MAX_LENGTH


/**
 * @brief CRC configuration.
 */

/** @brief Number of CRC bytes used for packets. */
#define RADIO_CONFIG_CRC_LENGTH 0

/** @brief Polynomial used to calculate the CRC. */
#define RADIO_CONFIG_CRC_POLY 0x00065B

/** @brief Initial value used when calculating the CRC. */
#define RADIO_CONFIG_CRC_INIT 0x000000


/**
 * @brief Data whitening configuration.
 */

/** @brief Enables data whitening for radio packets. */
#define RADIO_CONFIG_WHITENING 1

/** @brief Enables the radio peripheral. */
#define RADIO_CONFIG_POWER 1
