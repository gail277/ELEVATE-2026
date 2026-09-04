#include <stdint.h>
#include "uart.h"
/* set base adddress for UART0, instance of the UART peripheral */
#define UART0_BASE 0x40002000


/** UART register definitions (refer to Table 134 on page 506
 *  in the nRF52840 Product Specification.)
 */

/* Pointer to register that starts the UART receiver */
static volatile uint32_t *UART0_TASKS_STARTRX =
    (volatile uint32_t *)(UART0_BASE + 0x000);

/*Pointer to register that starts the UART transmitter */
static volatile uint32_t *UART0_TASKS_STARTTX =
    (volatile uint32_t *)(UART0_BASE + 0x008);

/* Pointer to register that indicates when data is ready to be transmitted from TXD */
static volatile uint32_t *UART0_EVENTS_TXDRDY =
    (volatile uint32_t *)(UART0_BASE + 0x11C);

/*Pointer to register that enables UART */
static volatile uint32_t *UART0_ENABLE =
    (volatile uint32_t *)(UART0_BASE + 0x500);

/* Pointer to register for pin select for TXD */
static volatile uint32_t *UART0_PSEL_TXD =
    (volatile uint32_t *)(UART0_BASE + 0x50C);

/* Pointer to register for Baud rate. */
static volatile uint32_t *UART0_BAUDRATE =
    (volatile uint32_t *)(UART0_BASE + 0x524);

/* Pointer to register for configuration of parity and hardware flow control */
static volatile uint32_t *UART0_CONFIG =
    (volatile uint32_t *)(UART0_BASE + 0x56C);

/*Pointer to TXD register*/
static volatile uint32_t *UART0_TXD =
    (volatile uint32_t *)(UART0_BASE + 0x51C);

/* Pointer to register that indicates when data is ready to be received in RXD */
static volatile uint32_t *UART0_EVENTS_RXDRDY =
    (volatile uint32_t *)(UART0_BASE + 0x108);

/*Pointer to RXD register*/
static volatile uint32_t *UART0_RXD =
    (volatile uint32_t *)(UART0_BASE + 0x518);

/* Pointer to register for pin select for RXD */
static volatile uint32_t *UART0_PSEL_RXD =
    (volatile uint32_t *)(UART0_BASE + 0x504);



void 
uart_init(void)
{
    *UART0_PSEL_TXD = 6;            /* select GPIO pin P0.06 for TXD (shown on back of board)*/
    *UART0_PSEL_RXD = 8;            /* select GPIO pin P0.08 for RXD (shown on back of board)*/
    *UART0_BAUDRATE = 0x01D7E000;   /* 115200 baud (refer to page 513-514) */
    *UART0_CONFIG = 0;              /* No parity (error checking), no flow control */
    *UART0_ENABLE = 4;              /* Enable UART */

    /* Clear events before starting */
    *UART0_EVENTS_TXDRDY = 0;
    *UART0_TASKS_STARTTX = 1;       /* Start transmission of data in TXD register (pg 505)*/
}


void 
uart_send_char(char c)
{
    
    /* Write character to TXD register */
    *UART0_TXD = c;

    /**
     * Wait for transmission to complete
     */
    while (*UART0_EVENTS_TXDRDY == 0)
    {
        /* Wait for UART to set TXDRDY to 1 (page 508)*/
    }

    /**
     * Indicate that the transmission is complete by clearing the TXDRDY 
     * event register. This is done by writing 0 to the register (page 503 & 508).
     */
    *UART0_EVENTS_TXDRDY = 0;
}

void 
uart_send_string(char *str)
{
    /*send each character in the string until the null terminator is reached*/
    while (*str != '\0')
    {
        uart_send_char(*str);
        str++;
    }
}


char 
uart_receive_char(void)
{
    while (*UART0_EVENTS_RXDRDY == 0)
    {
        /* Wait for a character to be received */
    }

    char c = *UART0_RXD;

    *UART0_EVENTS_RXDRDY = 0;

    return c;
}

void uart_echo(void)
{
    char c;
    uart_send_char('\r');
    uart_send_char('\n');
    uart_send_string("$:");

    for (;;)
    {
        c = uart_receive_char();

        /**
         * Echo the character back to the sender, with special handling 
         * for carriage return and new line
         */
        if (c == '\r' || c == '\n')
        {
            uart_send_char('\r');
            uart_send_char('\n');
            uart_send_string("$:");
        }
        else
        {
            uart_send_char(c);
        }
    }
}