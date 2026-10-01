#pragma once

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

/**
 * @brief Transmits a string using UART0.
 *
 * Sends each character in the string until the null terminator is reached.
 *
 * @param str String to transmit.
 */
void uart_send_string(char *str);

/**
 * @brief Receives a single character using UART0.
 *
 * Waits until a character is received and returns it.
 *
 * @return Received character.
 */
char uart_receive_char(void);

/**
 * @brief Echoes received characters back to the sender.
 *
 * Continuously receives characters and transmits them back, with special handling for carriage return and new line.
 */
void uart_echo(void);

/**
 * @brief Checks whether UART data is available to be received.
 *
 * Checks the UART receive event register to determine whether
 * a character has been received.
 *
 * @return Non-zero if a character is available, otherwise 0.
 */
uint8_t uart_data_available(void);