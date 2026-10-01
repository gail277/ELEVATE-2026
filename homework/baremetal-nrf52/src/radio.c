#include "packet.h"
#include "radio_config.h"
#include "uart.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/** RADIO registers
 * Refer to pages 328-330 of the nRF52840 Product Specification
 */


/* Base address for the RADIO peripheral */
#define RADIO_BASE 0x40001000
#define CLOCK_BASE 0x40000000

/*Buffer for messages recieved/transmitted*/
static uint8_t rx_buffer[PAYLOAD_MAX_LENGTH + 1];
static uint8_t tx_buffer[PAYLOAD_MAX_LENGTH + 1];

/* Enable RADIO in TX mode */
static volatile uint32_t *RADIO_TASKS_TXEN =
    (volatile uint32_t *)(RADIO_BASE + 0x000);

/* Enable RADIO in RX mode */
static volatile uint32_t *RADIO_TASKS_RXEN =
    (volatile uint32_t *)(RADIO_BASE + 0x004);

/* Start RADIO */
static volatile uint32_t *RADIO_TASKS_START =
    (volatile uint32_t *)(RADIO_BASE + 0x008);

/* Stop RADIO */
static volatile uint32_t *RADIO_TASKS_STOP =
    (volatile uint32_t *)(RADIO_BASE + 0x00C);

/* Disable RADIO */
static volatile uint32_t *RADIO_TASKS_DISABLE =
    (volatile uint32_t *)(RADIO_BASE + 0x010);

/* RADIO has ramped up and is ready to be started */
static volatile uint32_t *RADIO_TASKS_READY =
    (volatile uint32_t *)(RADIO_BASE + 0x100);

/* RADIO has been disabled */
static volatile uint32_t *RADIO_EVENTS_DISABLED =
    (volatile uint32_t *)(RADIO_BASE + 0x110);

/* RADIO has ramped up and is ready to be started TX path */
static volatile uint32_t *RADIO_EVENTS_TXREADY =
    (volatile uint32_t *)(RADIO_BASE + 0x154);

/* RADIO has ramped up and is ready to be started RX path */
static volatile uint32_t *RADIO_EVENTS_RXREADY =
    (volatile uint32_t *)(RADIO_BASE + 0x158);

/* Packet Pointer */
static volatile uint32_t *RADIO_PACKETPTR =
    (volatile uint32_t *)(RADIO_BASE + 0x504);

/* Packet configuration register 0 */
static volatile uint32_t *RADIO_PCNF0 =
    (volatile uint32_t *)(RADIO_BASE + 0x514);

/* Packet configuration register 1 */
static volatile uint32_t *RADIO_PCNF1 =
    (volatile uint32_t *)(RADIO_BASE + 0x518);


/* Base address 0 */
static volatile uint32_t *RADIO_BASE0 =
    (volatile uint32_t *)(RADIO_BASE + 0x51C);

/* Base address 1 */
static volatile uint32_t *RADIO_BASE1 =
    (volatile uint32_t *)(RADIO_BASE + 0x520);

/* Prefix address 0 */
static volatile uint32_t *RADIO_PREFIX0 =
    (volatile uint32_t *)(RADIO_BASE + 0x524);

/* Prefix address 1 */
static volatile uint32_t *RADIO_PREFIX1 =
    (volatile uint32_t *)(RADIO_BASE + 0x528);

/* Receive addresses */
static volatile uint32_t *RADIO_RXADDRESSES =
    (volatile uint32_t *)(RADIO_BASE + 0x530);

/* Frequency */
static volatile uint32_t *RADIO_FREQUENCY =
    (volatile uint32_t *)(RADIO_BASE + 0x508);

/* CRC configuration */
static volatile uint32_t *RADIO_CRCCNF =
    (volatile uint32_t *)(RADIO_BASE + 0x534);

/* CRC polynomial */
static volatile uint32_t *RADIO_CRCPOLY =
    (volatile uint32_t *)(RADIO_BASE + 0x538);

/* CRC initial value */
static volatile uint32_t *RADIO_CRCINIT =
    (volatile uint32_t *)(RADIO_BASE + 0x53C);

/* End event */
static volatile uint32_t *RADIO_EVENTS_END =
    (volatile uint32_t *)(RADIO_BASE + 0x10C);

/* CRC status */
static volatile uint32_t *RADIO_CRCSTATUS =
    (volatile uint32_t *)(RADIO_BASE + 0x400);

/* Radio mode */
static volatile uint32_t *RADIO_MODE =
    (volatile uint32_t *)(RADIO_BASE + 0x510);

/* Radio power */
static volatile uint32_t *RADIO_POWER =
    (volatile uint32_t *)(RADIO_BASE + 0xFFC);

/* Transmit address */
static volatile uint32_t *RADIO_TXADDRESS =
    (volatile uint32_t *)(RADIO_BASE + 0x52C);


/* CLOCK registers */

/* Start high-frequency clock task */
static volatile uint32_t *CLOCK_TASKS_HFCLKSTART =
    (volatile uint32_t *)(CLOCK_BASE + 0x000);

/* High-frequency clock started event */
static volatile uint32_t *CLOCK_EVENTS_HFCLKSTARTED =
    (volatile uint32_t *)(CLOCK_BASE + 0x100);


void
radio_init(void)
{
    // Start external high-frequency crystal oscillator
    *CLOCK_TASKS_HFCLKSTART = 1;

    // Wait until the crystal oscillator is running
    while (*CLOCK_EVENTS_HFCLKSTARTED == 0)
        ;

    // Clear event
    *CLOCK_EVENTS_HFCLKSTARTED = 0;

    *RADIO_POWER         = RADIO_CONFIG_POWER;
    *RADIO_TASKS_DISABLE = 1;
    while (*RADIO_EVENTS_DISABLED == 0)
        ;
    *RADIO_EVENTS_DISABLED = 0;

    // Configure radio mode
    *RADIO_MODE = RADIO_CONFIG_MODE;

    // Configure packet format
    *RADIO_PCNF0 = (7 << 0) // Length on air of LENGTH field in number of bits.
                   | (RADIO_CONFIG_S0_LENGTH << 8) |
                   (RADIO_CONFIG_S1_LENGTH << 16) |
                   (0 << 20)    // Include S1 field in RAM only if S1LEN > 0
                   | (0 << 22)  // CILEN = 0
                   | (0 << 24)  // Length of preamble on air = 8-bits
                   | (0 << 25)  // CRCINC = LENGTH excludes CRC
                   | (0 << 26); // TERMLEN = 0

    *RADIO_PCNF1 =
        (RADIO_CONFIG_MAX_LENGTH << 0)      // Maximum length of packet payload
        | (RADIO_CONFIG_STATIC_LENGTH << 8) // Static length in number of bytes
        |
        (RADIO_CONFIG_BALEN_ADDRESS_WIDTH << 16) // Base address length in bytes
        | (0 << 24) // On air endianness of packet, this applies to the S0,
                    // LENGTH, S1 and the PAYLOAD fields. LSB first
        | (RADIO_CONFIG_WHITENING << 25); // Enable or disable packet whitening


    // Configure address
    *RADIO_BASE0       = RADIO_CONFIG_BASE0_ADDRESS;
    *RADIO_BASE1       = RADIO_CONFIG_BASE1_ADDRESS;
    *RADIO_PREFIX0     = RADIO_CONFIG_PREFIX0_ADDRESS;
    *RADIO_PREFIX1     = RADIO_CONFIG_PREFIX1_ADDRESS;
    *RADIO_RXADDRESSES = RADIO_CONFIG_RX_ADDRESS;
    *RADIO_TXADDRESS   = RADIO_CONFIG_TX_ADDRESS;

    // Configure frequency/channel
    *RADIO_FREQUENCY = RADIO_CONFIG_CHANNEL;

    // Configure CRC
    *RADIO_CRCCNF =
        (RADIO_CONFIG_CRC_LENGTH << 0) // Maximum length of packet payload
        | (1 << 8); // CRC calculation does not include address field.

    *RADIO_CRCPOLY = RADIO_CONFIG_CRC_POLY;
    *RADIO_CRCINIT = RADIO_CONFIG_CRC_INIT;

    // Point RADIO at receive buffer
    *RADIO_PACKETPTR = (uint32_t)rx_buffer;
}

void
radio_RX_ramp_up(void)
{
    *RADIO_TASKS_DISABLE = 1;

    while (*RADIO_EVENTS_DISABLED == 0)
        ;
    *RADIO_EVENTS_DISABLED = 0;

    *RADIO_PACKETPTR = (uint32_t)rx_buffer;

    *RADIO_TASKS_RXEN = 1;

    while (*RADIO_EVENTS_RXREADY == 0)
        ;
    *RADIO_EVENTS_RXREADY = 0;

    *RADIO_TASKS_START = 1;
}

uint8_t *
radio_RX(void)
{
    if (*RADIO_EVENTS_END == 0)
    {
        return NULL;
    }

    *RADIO_EVENTS_END = 0;

    if (RADIO_CONFIG_CRC_LENGTH == 0)
    {
        return rx_buffer;
    }
    else
    {
         if (*RADIO_CRCSTATUS == 1)
        {
            return rx_buffer;
        }

        radio_RX_ramp_up();

        return NULL;
    }
}



void
radio_TX_ramp_up(void)
{
    *RADIO_TASKS_DISABLE = 1;

    while (*RADIO_EVENTS_DISABLED == 0)
        ;
    *RADIO_EVENTS_DISABLED = 0;

    *RADIO_TASKS_TXEN = 1;
    while (*RADIO_EVENTS_TXREADY == 0)
        ;
    *RADIO_EVENTS_TXREADY = 0;

    // Transmitter is now ready
}

void
radio_TX(void)
{
    *RADIO_EVENTS_END = 0;

    *RADIO_TASKS_START = 1;

    while (*RADIO_EVENTS_END == 0)
        ;

    *RADIO_EVENTS_END = 0;
}


void
radio_send_message(uint8_t *message, uint8_t length)
{

    uint8_t offset = 0;


    radio_TX_ramp_up();
    while (length > 0)
    {
        uint8_t packet_length;

        if (length > PAYLOAD_MAX_LENGTH)
        {
            packet_length = PAYLOAD_MAX_LENGTH;
        }
        else
        {
            packet_length = length;
        }

        packet_t packet;

        packet_create(&packet, message + offset, packet_length);

        radio_send_packet(&packet);

        offset += packet_length;
        length -= packet_length;
    }

    radio_RX_ramp_up();
}

void
radio_send_packet(packet_t *packet)
{
    tx_buffer[0] = packet->length;

    for (uint8_t i = 0; i < packet->length; i++)
    {
        tx_buffer[i + 1] = packet->data[i];
    }

    *RADIO_PACKETPTR = (uint32_t)tx_buffer;
    radio_TX();
}

void
decode_message(uint8_t *message)
{
    uint8_t length = rx_buffer[0];

    for (uint8_t i = 0; i < length; i++)
    {
        message[i] = rx_buffer[i + 1];
    }

    message[length] = '\0';

    for (uint8_t i = 0; i < length; i++)
    {
        if (message[i] < 32 || message[i] > 126)
        {
            return;
        }
    }

    uart_send_string((char *)message);
}
