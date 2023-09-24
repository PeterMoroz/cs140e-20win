#include "rpi.h"
#include "sw-uart.h"


// helper: cleans up the code.
static inline void timed_write(int pin, int v, unsigned usec) {
    gpio_write(pin,v);
    delay_us(usec);
}

// do this first: used timed_write to cleanup.
//  recall: time to write each bit (0 or 1) is in <uart->usec_per_bit>
void sw_uart_putc(sw_uart_t *uart, unsigned char c) {
    // start bit
    gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 0
    if (c & 0x1)
        gpio_set_on(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 1
    if (c & 0x2)
        gpio_set_on(uart->tx);
    else
        gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 2
    if (c & 0x4)
        gpio_set_on(uart->tx);
    else
        gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 3
    if (c & 0x8)
        gpio_set_on(uart->tx);
    else
        gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 4
    if (c & 0x10)
        gpio_set_on(uart->tx);
    else
        gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 5
    if (c & 0x20)
        gpio_set_on(uart->tx);
    else
        gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 6
    if (c & 0x40)
        gpio_set_on(uart->tx);
    else
        gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // bit 7
    if (c & 0x80)
        gpio_set_on(uart->tx);
    else
        gpio_set_off(uart->tx);
    delay_us(uart->usec_per_bit);
    // stop bit
    gpio_set_on(uart->tx);
    // in theory delay usec_per_bit should be enough
    // but in fact it works wrong, so double delay
    delay_us(2 * uart->usec_per_bit);
}

// do this second: you can type in pi-cat to send stuff.
//      EASY BUG: if you are reading input, but you do not get here in 
//      time it will disappear.
int sw_uart_getc(sw_uart_t *uart, int timeout_usec) {
    unimplemented();
}

void sw_uart_putk(sw_uart_t *uart, const char *msg) {
    for(; *msg; msg++)
        sw_uart_putc(uart, *msg);
}

