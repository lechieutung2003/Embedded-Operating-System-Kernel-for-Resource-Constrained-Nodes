#include "uart.h"

#define UART0_DR (*(volatile uint32_t *)0x4000C000)
#define UART0_FR (*(volatile uint32_t *)0x4000C018)
#define UART_FR_TXFF (1 << 5)

void uart_init(void) {
    // UART0 is already enabled and mapped to stdout in QEMU lm3s6965evb by default
}

void uart_putc(char c) {
    // Wait until transmit FIFO is not full
    while (UART0_FR & UART_FR_TXFF);
    UART0_DR = (uint32_t)c;
}

void uart_puts(const char *s) {
    if (!s) return;
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
        }
        uart_putc(*s++);
    }
}

void uart_print_dec(uint32_t val) {
    char buf[12];
    int i = 0;
    if (val == 0) {
        uart_putc('0');
        return;
    }
    while (val > 0) {
        buf[i++] = (val % 10) + '0';
        val /= 10;
    }
    while (i > 0) {
        uart_putc(buf[--i]);
    }
}

void uart_print_hex(uint32_t val) {
    const char hex_chars[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        uart_putc(hex_chars[(val >> i) & 0x0F]);
    }
}
