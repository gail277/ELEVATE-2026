#include <stdint.h>
#include <stddef.h>
#include "chat.h"
#include "uart.h"
#include "radio.h"

void chat(void)
{
    char input[500];
    uint8_t input_index = 0;
    uint8_t input_line_active = 1;

    while (1)
    {

         // Check for received radio message
        uint8_t *received = radio_RX();

        if (received != NULL)
        {
            uint8_t message[PAYLOAD_MAX_LENGTH + 1];

            // Clear the current input line
            uart_send_string("\r\033[2K");

            // Move up to where the input line was
            uart_send_string("\033[1A");

            // Print the received message
            decode_message(message);

            // Move to the next line
            uart_newline();

            // Restore current input
            for (uint8_t i = 0; i < input_index; i++)
            {
                uart_send_char(input[i]);
            }

            radio_RX_ramp_up();
        }

        // Check for keyboard input
        if (uart_data_available())
        {
            char c = uart_receive_char();

            if (c == '\r' || c == '\n')
            {
                input[input_index] = '\0';

                uart_send_string("\r\033[2K");

                if (input_index > 0)
                {
                    uart_process_command(input);
                }

                input_index = 0;

                uart_newline();
                input_line_active = 1;
            }
            else if (c == '\b' || c == 127)
            {
                if (input_index > 0)
                {
                    input_index--;

                    // Remove character from terminal
                    uart_send_char('\b');
                    uart_send_char(' ');
                    uart_send_char('\b');
                }
            }
            else
            {
                if (input_index < sizeof(input) - 1)
                {
                    input[input_index] = c;
                    input_index++;

                    uart_send_char(c);
                }
            }
        }

    }
}