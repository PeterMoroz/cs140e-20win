// implement:
//  void uart_init(void)
//
//  int uart_can_getc(void);
//  int uart_getc(void);
//
//  int uart_can_putc(void);
//  void uart_putc(unsigned c);
//
// see that hello world works.
//
//
#include "rpi.h"

#define GPIO_GPFSEL1   0x20200004
#define GPIO_GPPUD     0x20200094
#define GPIO_GPPUDCLK0 0x20200098


#define AUX_ENABLES 0x20215004
#define AUX_MU_IO   0x20215040
#define AUX_MU_IER  0x20215044
#define AUX_MU_IIR  0x20215048
#define AUX_MU_LCR  0x2021504C
#define AUX_MU_MCR  0x20215050
#define AUX_MU_LSR  0x20215054
#define AUX_MU_CNTL 0x20215060
#define AUX_MU_STAT 0x20215064
#define AUX_MU_BAUD 0x20215068


// called first to setup uart to 8n1 115200  baud,
// no interrupts.
//  - you will need memory barriers, use <dev_barrier()>
//
//  later: should add an init that takes a baud rate.
void uart_init(void) {
    // GPIO function set
    unsigned ra = GET32(GPIO_GPFSEL1);
    ra &= ~(7 << 12);
    ra |= 2 << 12;
    ra &= ~(7 << 15);
    ra |= 2 << 15;
    PUT32(GPIO_GPFSEL1, ra);

    // pull-up/pull-down disable
    PUT32(GPIO_GPPUD, 0x0);
    delay_cycles(150);
    PUT32(GPIO_GPPUDCLK0, (1 << 14) | (1 << 15));
    delay_cycles(150);
    PUT32(GPIO_GPPUDCLK0, 0);


    // enable miniUART
    PUT32(AUX_ENABLES, 0x1);
    // disable transmitter and receiver
    PUT32(AUX_MU_CNTL, 0x0);
    // set 8-bit mode
    PUT32(AUX_MU_LCR, 0x3);
    // disable RTS
    PUT32(AUX_MU_MCR, 0x0);
    // enable FIFO, clear FIFO
    PUT32(AUX_MU_IIR, 0xC6);
    // set baudrate (250 MHz / (115200 * 8)) - 1 = 270
    PUT32(AUX_MU_BAUD, 270);
    // enable transmitter and receiver
    PUT32(AUX_MU_CNTL, 0x3);
}

// 1 = at least one byte on rx queue, 0 otherwise
static int uart_can_getc(void) {
    return GET32(AUX_MU_LSR) & 0x00000001;
}

// returns one byte from the rx queue, if needed
// blocks until there is one.
int uart_getc(void) {
/*	return 0; */
    while (!uart_can_getc()) ;
    return GET32(AUX_MU_IO) & 0x000000FF;
}

// 1 = space to put at least one byte, 0 otherwise.
int uart_can_putc(void) {
    return GET32(AUX_MU_LSR) & 0x00000020;
}

// put one byte on the tx qqueue, if needed, blocks
// until TX has space.
void uart_putc(unsigned c) {
    while (!uart_can_putc()) ;
    PUT32(AUX_MU_IO, c & 0x000000FF);
}


// simple wrapper routines useful later.

// a maybe-more intuitive name for clients.
int uart_has_data(void) {
    return uart_can_getc();
}

int uart_getc_async(void) { 
    if(!uart_has_data())
        return -1;
    return uart_getc();
}
