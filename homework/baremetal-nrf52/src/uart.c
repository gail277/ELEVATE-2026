#include <stdint.h>
#include <string.h> 
#include <stdlib.h>
#include "uart.h"
#include "myalloc.h"
#include "radio.h"
#include <stdio.h>
#include <stdbool.h>

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
    (volatile uint32_t *)(UART0_BASE + 0x514);



static char uart_name[50] = "Abigail";

static bool show_name = true;

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
    *UART0_EVENTS_RXDRDY = 0;
    *UART0_TASKS_STARTTX = 1;       /* Start transmission of data in TXD register (pg 505)*/
    *UART0_TASKS_STARTRX = 1;       /* Start recieving data in RXD register (pg 506)*/
}


void 
uart_send_char(char c)
{
    *UART0_EVENTS_TXDRDY = 0;
    
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

void 
uart_newline(void){
    /*Not sure whether the terminal sends carriage return or newline, so both is sent */
    uart_send_char('\r');
    uart_send_char('\n');
}

void 
uart_echo(void)
{
    char c;
    char echo[500];
    uint8_t echo_index = 0;
    uart_newline();

    for (;;)
    {
        c = uart_receive_char();
        
        /**
         * Echo the character back to the sender, with special handling 
         * for carriage return and new line to echo the entire string,
         * and when maximum character length is exceeeded.
         */
        if (c == '\r' || c == '\n')
        {
            echo[echo_index] = '\0';
            echo_index = 0;

            uart_newline();
            uart_process_command(echo);
            uart_newline();
            uart_newline();
            uart_send_string("$:");
        }
        //backspace is pressed
        else if (c == '\b'){
            if(echo_index > 0)
            {
                echo_index--;
            }
        }
        else if (echo_index < sizeof(echo) - 1)
        {
            echo[echo_index] = c;
            echo_index++;

            uart_send_char(c);
        }
        else{
            uart_newline();
            uart_send_string("Prompt has exceeeded max character length. Press Enter to echo and start a new prompt");
        }
    }
}


static void uart_send_hex(uintptr_t value)
{
    char hex[] = "0123456789ABCDEF";
    int i;

    uart_send_string("0x");

    for (i = (sizeof(uintptr_t) * 2) - 1; i >= 0; i--)
    {
        uart_send_char(hex[(value >> (i * 4)) & 0xF]);
    }
}


void 
uart_process_command(char *command)
{
   
    /* alloc <size> */
    if (strncmp(command, "/alloc ", 7) == 0)
    {
        uint32_t size;
        void *ptr;

        size = (uint32_t)atoi(command + 7);
        ptr = myalloc(size);

        if (ptr == NULL)
        {
            uart_send_string("Allocation failed");
        }
        else
        {
            uart_send_string("Allocated at: ");
            uart_send_hex((uintptr_t)ptr);
        }

        uart_newline();
    }

    /* free <address> */
    else if (strncmp(command, "/free ", 6) == 0)
    {
        uintptr_t address;

        address = (uintptr_t)strtoul(command + 6, NULL, 0);
        myfree((void *)address);

        uart_send_string("Block freed");
        uart_newline();
    }

    /* send */
    else if (strncmp(command, "/send ", 6) == 0)
    {
        char *msg = command + 6;

        if (*msg != '\0')
        {
            char send_message[PAYLOAD_MAX_LENGTH * 10];

            if (show_name)
            {
                snprintf(send_message, sizeof(send_message), "%s: %s", uart_name, msg);
            }
            else
            {
                snprintf(send_message, sizeof(send_message), "%s", msg);
            }
            radio_send_message((uint8_t *)send_message, (uint8_t)strlen(send_message));
            uart_newline();
            uart_send_string(send_message);
        }

        uart_newline();
    }

    /* change name */
    else if (strncmp(command, "/name ", 6) == 0)
    {
        const char *new_name = command + 6;
        strncpy(uart_name, new_name, sizeof(uart_name) - 1);
        uart_name[sizeof(uart_name) - 1] = '\0';

        uart_newline();
        uart_send_string(" -- Name changed to: ");
        uart_send_string(uart_name);
        if (!show_name)
        {
            uart_send_string("   (note that you are in incognito mode)");
        }
        uart_send_string(" -- ");
        uart_newline();
    }
     /* don't show name */
    else if (strncmp(command, "/anon",5) == 0)
    {
        show_name = false;
        uart_newline();
        uart_send_string(" -- Incognito Mode -- ");
        uart_newline();
    }
     /* show name */
    else if (strncmp(command, "/showname", 9) == 0)
    {
        show_name = true;
        uart_newline();
        uart_send_string(" -- Name will be shown in messages -- ");
        uart_newline();
    }

    else if (strcmp(command, "/chaos") == 0)
    {
        char *msg = 
        "We're no strangers to love\n"
            "You know the rules and so do I\n"
           "A full commitment's what I'm thinking of\n"
            "You wouldn't get this from any other guy\n"
            "I just wanna tell you how I'm feeling\n"
            "Gotta make you understand\n"
            "Never gonna give you up\n"
            "Never gonna let you down\n"
            "Never gonna run around and desert you\n"
            "Never gonna make you cry\n"
            "Never gonna say goodbye\n"
            "Never gonna tell a lie and hurt you";

        // Transmit the hardcoded message
        radio_send_message(
            (uint8_t *)msg,
            (uint8_t)strlen(msg)
        );

        // Print it to the terminal
        uart_newline();
        uart_send_string(msg);
        uart_newline();
    }

    /* help with commands */
    else if (strncmp(command, "/help", 5) == 0)
    {

        uart_send_string(" -------------------------- ");
        uart_newline();
        uart_send_string(" /send <text> -  send a message");
        uart_newline();
        uart_send_string(" /name <name> -  change your name");
        uart_newline();
        uart_send_string(" /anon  -  hide your name in messages");
        uart_newline();
        uart_send_string(" /showname  -  show your name in messages");
        uart_newline();
        uart_send_string(" /chaos  -  enable chaos mode");
        uart_newline();
        uart_send_string(" -------------------------- ");
        uart_newline();
    }


    /* Unknown command */
    else
    {
        uart_send_string(" << Unknown command [refer to /help] >> ");
        uart_newline();
    }

}

uint8_t uart_data_available(void)
{
    return *UART0_EVENTS_RXDRDY;
}