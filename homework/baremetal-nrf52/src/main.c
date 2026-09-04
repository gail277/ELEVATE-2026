#include <stdint.h>
#include "uart.h"

/** LED 1 Pin - P0.13 */
#define LED1_PIN (13)

/** LED 2 Pin - P0.14 */
#define LED2_PIN (14)

/** LED 3 Pin - P0.15 */
#define LED3_PIN (15)

/** LED 4 Pin - P0.16 */
#define LED4_PIN (16)

/**
 * GPIO registers.
 *
 * Refer to 6.9.2 in the nRF52840 Product Specification.
 */
volatile uint32_t *GPIO_OUT    = (volatile uint32_t *)0x50000504;
volatile uint32_t *GPIO_OUTSET = (volatile uint32_t *)0x50000508;
volatile uint32_t *GPIO_OUTCLR = (volatile uint32_t *)0x5000050C;
volatile uint32_t *GPIO_DIR    = (volatile uint32_t *)0x50000514;

void
delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm("nop");
    }
}

int
main(void)
{

     uint8_t       leds[]      = { LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN };
    const uint8_t leds_count  = sizeof(leds) / sizeof(leds[0]);
    uint8_t       current_led = 0;

    /* Configure GPIO direction */
    *GPIO_DIR |= (1 << LED1_PIN);
    *GPIO_DIR |= (1 << LED2_PIN);
    *GPIO_DIR |= (1 << LED3_PIN);
    //*GPIO_DIR |= (1 << LED4_PIN);
    /* Initialize UART*/
    uart_init();

    for(;;){

        /* Turn LED on */
        *GPIO_OUTCLR = (1 << leds[current_led]);
        delay(500000);

        /* Turn LED off */
        *GPIO_OUTSET = (1 << leds[current_led]);
        delay(500000);

        current_led = (current_led + 1) % leds_count;

        uart_send_string("Hello");
        uart_send_char('\r');
        uart_send_char('\n');
        
    }

    return(0);
}
