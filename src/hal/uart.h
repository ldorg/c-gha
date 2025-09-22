#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    UART_BAUDRATE_9600 = 9600,
    UART_BAUDRATE_115200 = 115200,
    UART_BAUDRATE_230400 = 230400
} uart_baudrate_t;

typedef enum {
    UART_PARITY_NONE,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} uart_parity_t;

typedef struct {
    uart_baudrate_t baudrate;
    uart_parity_t parity;
    uint8_t data_bits;
    uint8_t stop_bits;
} uart_config_t;

int uart_init(const uart_config_t *config);
int uart_write(const uint8_t *data, size_t length);
int uart_read(uint8_t *buffer, size_t max_length);
int uart_printf(const char *format, ...);
void uart_deinit(void);

#endif