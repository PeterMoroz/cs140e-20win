#include "rpi.h"
#include "sw-uart.h"
#include "cs140e-src/cycle-count.h"

// helper: cleans up the code.
static inline void timed_write(int pin, int v, unsigned usec) {
    gpio_write(pin,v);
    delay_us(usec);
}

static inline void delay_ncycles(unsigned n) {
    unsigned cnt = cycle_cnt_read();
    while ((cycle_cnt_read() - cnt) < n) ;
}

// do this first: used timed_write to cleanup.
//  recall: time to write each bit (0 or 1) is in <uart->usec_per_bit>
void sw_uart_putc(sw_uart_t *uart, unsigned char c) {
    int tx = uart->tx;
    unsigned cycles_per_bit = uart->cycles_per_bit;
    // start bit
    gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // bit 0
    if (c & 0x1)
        gpio_set_on(tx);
    delay_ncycles(cycles_per_bit);
    // bit 1
    if (c & 0x2)
        gpio_set_on(tx);
    else
        gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // bit 2
    if (c & 0x4)
        gpio_set_on(tx);
    else
        gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // bit 3
    if (c & 0x8)
        gpio_set_on(tx);
    else
        gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // bit 4
    if (c & 0x10)
        gpio_set_on(tx);
    else
        gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // bit 5
    if (c & 0x20)
        gpio_set_on(tx);
    else
        gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // bit 6
    if (c & 0x40)
        gpio_set_on(tx);
    else
        gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // bit 7
    if (c & 0x80)
        gpio_set_on(tx);
    else
        gpio_set_off(tx);
    delay_ncycles(cycles_per_bit);
    // stop bit
    gpio_set_on(tx);
    delay_ncycles(cycles_per_bit);
}

// do this second: you can type in pi-cat to send stuff.
//      EASY BUG: if you are reading input, but you do not get here in 
//      time it will disappear.
int sw_uart_getc(sw_uart_t *uart, int timeout_usec) {
    int rx = uart->rx;
    unsigned delay_amount = uart->cycles_per_bit;
    unsigned half_delay_amount = delay_amount >> 1;
    unsigned s = timer_get_usec_raw();
    int c = 0;
    // wait for stop bit
    while (1) {
        if (gpio_read(rx) == 0)
            break;
        if (timer_get_usec_raw() - s >= timeout_usec)
            return -1;
    }
    
    delay_ncycles(delay_amount + half_delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;
    c >>= 1;

    delay_ncycles(delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;
    c >>= 1;
    
    delay_ncycles(delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;
    c >>= 1;
    
    delay_ncycles(delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;
    c >>= 1;
    
    delay_ncycles(delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;
    c >>= 1;

    delay_ncycles(delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;
    c >>= 1;
    
    delay_ncycles(delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;
    c >>= 1;
    
    delay_ncycles(delay_amount);
    if (gpio_read(rx) != 0)
        c |= 0x80;

    delay_ncycles(half_delay_amount);
    while (gpio_read(rx) == 0) ;

    return c;
}

void sw_uart_putk(sw_uart_t *uart, const char *msg) {
    for(; *msg; msg++)
        sw_uart_putc(uart, *msg);
}

