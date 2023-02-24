/*
 * write code to allow blinking using arbitrary pins.    
 * Implement:
 *	- gpio_set_output(pin) --- set GPIO <pin> as an output (vs input) pin.
 *	- gpio_set_on(pin) --- set the GPIO <pin> on.
 * 	- gpio_set_off(pin) --- set the GPIO <pin> off.
 * Use the minimal number of loads and stores to GPIO memory.  
 *
 * start.s defines a of helper functions (feel free to look at the assembly!  it's
 *  not tricky):
 *      uint32_t get32(volatile uint32_t *addr) 
 *              --- return the 32-bit value held at <addr>.
 *
 *      void put32(volatile uint32_t *addr, uint32_t v) 
 *              -- write the 32-bit quantity <v> to <addr>
 * 
 * Check-off:
 *  1. get a single LED to blink.
 *  2. attach an LED to pin 19 and another to pin 20 and blink in opposite order (i.e.,
 *     one should be on, while the other is off).   Note, if they behave weirdly, look
 *     carefully at the wording for GPIO set.
 */
#include "gpio.h"

/*
 * These routines are given by us and are in start.s
 */
// writes the 32-bit value <v> to address <addr>:   *(unsigned *)addr = v;
void put32(volatile void *addr, unsigned v);
// returns the 32-bit value at <addr>:  return *(unsigned *)addr
unsigned get32(const volatile void *addr);
// does nothing.
void nop(void);

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
volatile unsigned *gpio_fsel0 = (void*)(GPIO_BASE + 0x00);
volatile unsigned *gpio_set0  = (void*)(GPIO_BASE + 0x1C);
volatile unsigned *gpio_clr0  = (void*)(GPIO_BASE + 0x28);
volatile unsigned *gpio_lvl0  = (void*)(GPIO_BASE + 0x34);

// Part 1 implement gpio_set_on, gpio_set_off, gpio_set_output

// set <pin> to be an output pin.  note: fsel0, fsel1, fsel2 are contiguous in memory,
// so you can use array calculations!
void gpio_set_output(unsigned pin) {
    // implement this
    // use gpio_fsel0
    volatile unsigned *addr = gpio_fsel0 + (pin / 10);
    unsigned off = (pin % 10) * 3;
    put32(addr, get32(addr) | (0x1 << off));
}

// set GPIO <pin> on.
void gpio_set_on(unsigned pin) {
    // implement this
    // use gpio_set0
    volatile unsigned *addr = gpio_set0;
    if (pin >= 32) {
        addr += 1;
        pin -= 32;
    }
    put32(addr, get32(addr) | (0x1 << pin));
}

// set GPIO <pin> off
void gpio_set_off(unsigned pin) {
    // implement this
    // use gpio_clr0
    volatile unsigned *addr = gpio_clr0;
    if (pin >= 32) {
        addr += 1;
        pin -= 32;
    }
    put32(addr, get32(addr) | (0x1 << pin));
}

// Part 2: implement gpio_set_input and gpio_read

// set <pin> to input.
void gpio_set_input(unsigned pin) {
    // implement.
    volatile unsigned *addr = gpio_fsel0 + (pin / 10);
    unsigned off = (pin % 10) * 3;
    put32(addr, get32(addr) & ~(0x7 << off));
}

// return the value of <pin>
int gpio_read(unsigned pin) {
    unsigned v = 0;

    // implement.
    volatile unsigned *addr = gpio_lvl0;
    if (pin >= 32) {
        addr += 1;
        pin -= 32;
    }
    v = get32(addr) & (1 << pin);
    return v;
}

// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v) {
    if(v)
       gpio_set_on(pin);
    else
       gpio_set_off(pin);
}

