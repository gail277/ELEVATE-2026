#pragma once

#define UART_H

/* UART functions */

/**
 * @brief Initialises UART0 for transmitting data.
 *
 * Configures P0.06 as the TXD pin, sets the baud rate to
 * 115200, disables parity and hardware flow control, and
 * enables the UART peripheral.
 */
void uart_init(void);

/**
 * @brief Transmits a single character using UART0.
 *
 * Writes the character to the TXD register and waits until
 * the UART is ready to transmit another character.
 *
 * @param c Character to transmit.
 */
void uart_send_char(char c);
